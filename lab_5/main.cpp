//
// Created by MaxPlays on 20/12/2024.
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cstdio>
#include <set>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <thread>

#include "src/tree/Tree.h"

std::vector<std::string> split(const std::string &s, char delimiter = ' ') {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(s);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool running = true;

void signal_handler(int signum) {
    running = false;
}

int get_port(int node_id) {
    return 5000 + node_id;
}

int main() {
    zmq::context_t context(1);

    zmq::socket_t manager_socket(context, ZMQ_DEALER);
    manager_socket.bind("tcp://*:4000"); // Порт менеджера

    TreeNode* root = nullptr;
    std::unordered_map<int, TreeNode*> nodes_map;

    zmq::socket_t children_socket(context, ZMQ_DEALER);

    std::queue<TreeNode*> node_queue;

    zmq::pollitem_t items[] = {
        { static_cast<void*>(manager_socket), 0, ZMQ_POLLIN, 0 }
    };

    std::set<int> unfinished;

    std::vector<std::thread> threads;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    auto th = std::thread([&manager_socket, &unfinished]() {
        while (true) {
            zmq::pollitem_t poll_items[] = {
                            { static_cast<void*>(manager_socket), 0, ZMQ_POLLIN, 0 }
                        };

            zmq::poll(poll_items, 1, 3000);

            if (poll_items[0].revents & ZMQ_POLLIN) {
                std::vector<zmq::message_t> recv_msgs;
                while (true) {
                    zmq::message_t msg;
                    manager_socket.recv(msg, zmq::recv_flags::none);
                    recv_msgs.push_back(std::move(msg));
                    if (!manager_socket.get(zmq::sockopt::rcvmore)) {
                        break;
                    }
                }

                for (auto & message : recv_msgs) {
                    std::string reply_str(static_cast<char*>(message.data()), message.size());
                    auto lines = split(reply_str, '\n');
                    auto number = std::stoi(split(lines[0], ' ')[1]);
                    if (unfinished.contains(number)) {
                        std::cout << lines[1] << "\n";
                        unfinished.erase(number);
                    }
                }
            }
        }
    });

    int id = 0;

    while (running) {
        std::cout << "> ";
        std::cout.flush();
        std::string input;
        if (!getline(std::cin, input)) {
            break;
        }

        std::stringstream ss(input);
        std::string command;
        ss >> command;

        if (command == "create") {
            int node_id;
            ss >> node_id;
            if (ss.fail()) {
                std::cout << "Error: Invalid node_id\n";
                continue;
            }

            if (nodes_map.find(node_id) != nodes_map.end()) {
                std::cout << "Error: Already exists\n";
                continue;
            }

            if (root == nullptr) {
                pid_t pid = fork();
                if (pid == -1) {
                    std::cout << "Error: Fork failed\n";
                    continue;
                }
                if (pid == 0) {
                    char id_str[10];
                    char parent_port_str[10];
                    sprintf(id_str, "%d", node_id);
                    sprintf(parent_port_str, "%d", 4000);
                    execl("./lab_5_child", "./lab_5_child", id_str, parent_port_str, (char*)NULL);
                    exit(1);
                } else {
                    root = new TreeNode(node_id, pid, get_port(node_id));
                    children_socket.connect("tcp://localhost:" + std::to_string(get_port(node_id)));
                    nodes_map[node_id] = root;
                    node_queue.push(root);
                    std::cout << "Ok: " << pid << "\n";
                }
                continue;
            }

            TreeNode* parent = nullptr;
            while (!node_queue.empty()) {
                parent = node_queue.front();
                if (parent->left == nullptr || parent->right == nullptr) {
                    break;
                }
                node_queue.pop();
            }

            if (parent == nullptr) {
                std::cout << "Error: No available parent node\n";
                continue;
            }

            pid_t pid = fork();
            if (pid == -1) {
                std::cout << "Error: Fork failed\n";
                continue;
            }
            if (pid == 0) {
                char id_str[10];
                char parent_port_str[10];
                sprintf(id_str, "%d", node_id);
                sprintf(parent_port_str, "%d", parent->port);
                execl("./lab_5_child", "./lab_5_child", id_str, parent_port_str, (char*)NULL);
                exit(1);
            } else {
                TreeNode* new_node = new TreeNode(node_id, pid, get_port(node_id));
                nodes_map[node_id] = new_node;

                std::string msg = std::to_string(id) + " " + "created " + std::to_string(parent->id) + " " + std::to_string(new_node->id);
                ++id;

                zmq::message_t message(msg.size());
                memcpy(message.data(), msg.c_str(), msg.size());

                children_socket.send(message, zmq::send_flags::none);

                if (parent->left == nullptr) {
                    parent->left = new_node;
                } else {
                    parent->right = new_node;
                    node_queue.pop();
                }

                node_queue.push(new_node);

                std::cout << "Ok: " << pid << "\n";
            }
        }
        else if (command == "exec") {
            int target_id;
            ss >> target_id;
            if (ss.fail()) {
                std::cout << "Error: Invalid node_id\n";
                continue;
            }

            std::string params;
            getline(ss, params);
            if (!params.empty() && params[0] == ' ') {
                params = params.substr(1);
            }

            if (nodes_map.find(target_id) == nodes_map.end()) {
                std::cout << "Error: Node not found\n";
                continue;
            }

            TreeNode* target_node = nodes_map[target_id];
            if (!target_node->available) {
                std::cout << "Error: Node is unavailable\n";
                continue;
            }

            std::string msg = std::to_string(id) + " " + "exec " + std::to_string(target_id) + " " + params;
            ++id;

            zmq::message_t message(msg.size());
            memcpy(message.data(), msg.c_str(), msg.size());

            children_socket.send(message, zmq::send_flags::none);
            unfinished.emplace(id - 1);

            threads.emplace_back([&target_node, &id, &unfinished, &root, target_id]() {
                std::this_thread::sleep_for(std::chrono::seconds(5000));

                if (unfinished.contains(id - 1)) {
                    std::cout << "Error:" << target_id << ": Node is unavailable\n";
                    unfinished.erase(id - 1);
                    root = deleteNodeById(root, target_id);
                    std::cout << "DELETING " << target_id << std::endl;
                }
            });
        }
        else if (command == "ping") {
            int target_id;
            ss >> target_id;
            if (ss.fail()) {
                std::cout << "Error: Invalid node_id\n";
                continue;
            }

            if (nodes_map.find(target_id) == nodes_map.end()) {
                std::cout << "Error: Not found\n";
                continue;
            }

            TreeNode* target_node = nodes_map[target_id];
            if (!target_node->available) {
                std::cout << "Ok: 0\n";
                continue;
            }

            std::string msg = std::to_string(id) + " " + "ping " + std::to_string(target_id);
            ++id;

            zmq::message_t message(msg.size());
            memcpy(message.data(), msg.c_str(), msg.size());

            children_socket.send(message, zmq::send_flags::none);
            unfinished.emplace(id - 1);

            threads.push_back(std::move(std::thread([&target_node, &id, &unfinished, &root, target_id]() {
                std::this_thread::sleep_for(std::chrono::seconds(5));

                if (unfinished.contains(id - 1)) {
                    std::cout << "Ok: 0\n";
                    unfinished.erase(id - 1);
                    root = deleteNodeById(root, target_id);
                    std::cout << "DELETING " << target_id << std::endl;
                }
            })));
        }
        else {
            std::cout << "Error: Unknown command\n";
        }

        while (waitpid(-1, NULL, WNOHANG) > 0) {

        }
    }

    for (auto & thread : threads) {
        thread.join();
    }

    th.join();
}
