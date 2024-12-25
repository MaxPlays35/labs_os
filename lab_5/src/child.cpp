//
// Created by MaxPlays on 20/12/2024.
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <csignal>

std::vector<std::string> split(const std::string &s, char delimiter = ' ') {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(s);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int get_port(int node_id) {
    return 5000 + node_id;
}

bool running = true;

void signal_handler(int signum) {
    running = false;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: compute_node <id> <parent_port>\n";
        return 1;
    }

    int node_id = std::stoi(argv[1]);
    int parent_port = std::stoi(argv[2]);

    // std::cout << "STARTED NODE WITH node_id=" << node_id << ",parent_port=" << parent_port << ",own_port=" <<
            // get_port(node_id) << std::endl;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    zmq::context_t context(1);

    zmq::socket_t parent_dealer(context, ZMQ_DEALER);
    std::string parent_addr = "tcp://localhost:" + std::to_string(parent_port);
    parent_dealer.connect(parent_addr);

    zmq::socket_t children_router(context, ZMQ_DEALER);
    int my_port = get_port(node_id);
    std::string bind_address = "tcp://*:" + std::to_string(my_port);
    children_router.bind(bind_address);

    zmq::socket_t children_socket(context, ZMQ_DEALER);

    std::unordered_map<std::string, int> kv_store;

    std::unordered_map<int, int> children;

    while (running) {
        zmq::pollitem_t items[] = {
            {static_cast<void *>(children_router), 0, ZMQ_POLLIN, 0},
            // {static_cast<void *>(children_router), 0, ZMQ_POLLIN, 0}
        };

        zmq::poll(items, 1, 100);

        if (items[0].revents & ZMQ_POLLIN) {
            std::vector<zmq::message_t> recv_msgs;
            while (true) {
                zmq::message_t msg;
                children_router.recv(msg, zmq::recv_flags::none);
                recv_msgs.push_back(std::move(msg));
                if (!children_router.get(zmq::sockopt::rcvmore)) {
                    break;
                }
            }

            if (recv_msgs.empty()) continue;

            // std::cout << "NODE_ID=" << node_id << "RECEIVED COMMAND" << std::endl;

            std::string command_str(static_cast<char *>(recv_msgs.back().data()), recv_msgs.back().size());
            std::vector<std::string> tokens = split(command_str, ' ');
            if (tokens.empty()) continue;
            // std::cout << "NODE_ID=" << node_id << tokens[0] << ' ' << tokens[1] << ' ' << tokens[2] << std::endl;

            std::string cmd = tokens[1];

            if (cmd == "exec") {
                if (tokens.size() < 3) {
                    std::string reply = "Res: " + tokens[0] + "\n";
                    reply += "Error:" + std::to_string(node_id) + ": Invalid exec command";
                    zmq::message_t reply_zmsg(reply.size());
                    memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                    parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                    continue;
                }

                int target_id = std::stoi(tokens[2]);

                if (target_id == node_id) {
                    if (tokens.size() == 4) {
                        std::string name = tokens[3];
                        if (kv_store.find(name) != kv_store.end()) {
                            int value = kv_store[name];
                            std::string reply = "Res: " + tokens[0] + "\n";
                            reply += "Ok:" + std::to_string(node_id) + ": " + std::to_string(value);
                            zmq::message_t reply_zmsg(reply.size());
                            memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                            parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                        } else {
                            std::string reply = "Res: " + tokens[0] + "\n";
                            reply += "Ok:" + std::to_string(node_id) + ": '" + name + "' not found";
                            zmq::message_t reply_zmsg(reply.size());
                            memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                            parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                        }
                    } else if (tokens.size() == 5) {
                        std::string name = tokens[3];
                        int value = std::stoi(tokens[4]);
                        kv_store[name] = value;
                        std::string reply = "Res: " + tokens[0] + "\n";
                        reply += "Ok:" + std::to_string(node_id);
                        zmq::message_t reply_zmsg(reply.size());
                        memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                        parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                    } else {
                        std::string reply = "Res: " + tokens[0] + "\n";
                        reply += "Error:" + std::to_string(node_id) + ": Invalid exec command";
                        zmq::message_t reply_zmsg(reply.size());
                        memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                        parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                    }
                } else {
                    for (const auto &child: children) {
                        const int child_identity = child.first;
                        int child_port = child.second;
                        // std::cout << "NODE_ID=" << node_id << "FORWARDING TO " << child_identity << ' ' << child_port <<
                                // std::endl;

                        std::string forward_cmd = command_str;

                        zmq::message_t forward_zmsg(forward_cmd.size());
                        memcpy(forward_zmsg.data(), forward_cmd.c_str(), forward_cmd.size());

                        children_socket.connect("tcp://localhost:" + std::to_string(child_port));
                        children_socket.send(zmq::buffer(forward_cmd), zmq::send_flags::none);
                        children_socket.disconnect("tcp://localhost:" + std::to_string(child_port));

                        // std::cout << "NODE_ID=" << node_id << "SENT DONE" << std::endl;
                    }
                }
            } else if (cmd == "ping") {
                if (tokens.size() != 3) {
                    std::string reply = "Res: " + tokens[0] + "\n";
                    reply += "Error:" + std::to_string(node_id) + ": Invalid ping command";
                    zmq::message_t reply_zmsg(reply.size());
                    memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                    parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                    continue;
                }

                int target_id = std::stoi(tokens[2]);

                if (target_id == node_id) {
                    std::string reply = "Res: " + tokens[0] + "\n";
                    reply += "Ok:1";
                    zmq::message_t reply_zmsg(reply.size());
                    memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                    parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                } else {
                    for (const auto &child: children) {
                        const int child_identity = child.first;
                        int child_port = child.second;
                        // std::cout << "NODE_ID=" << node_id << "FORWARDING TO " << child_identity << ' ' << child_port <<
                                // std::endl;

                        std::string forward_cmd = command_str;

                        zmq::message_t forward_zmsg(forward_cmd.size());
                        memcpy(forward_zmsg.data(), forward_cmd.c_str(), forward_cmd.size());

                        children_socket.connect("tcp://localhost:" + std::to_string(child_port));
                        children_socket.send(zmq::buffer(forward_cmd), zmq::send_flags::none);
                        children_socket.disconnect("tcp://localhost:" + std::to_string(child_port));

                        // std::cout << "NODE_ID=" << node_id << "SENT DONE" << std::endl;
                    }
                }
            } else if (cmd == "created") {
                // std::cout << "NODE_ID=" << node_id << "created GOYDA" << std::endl;
                if (tokens.size() < 4) {
                    std::string reply = "Res: " + tokens[0] + "\n";
                    reply += "Error:" + std::to_string(node_id) + ": Invalid created command";
                    zmq::message_t reply_zmsg(reply.size());
                    memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                    parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                    continue;
                }

                int target_id = std::stoi(tokens[2]);

                if (target_id == node_id) {
                    if (tokens.size() == 4) {
                        int child = std::stoi(tokens[3]);
                        children[child] = get_port(child);
                        // std::cout << "NODE_ID=" << node_id << "ADDED" << std::endl;
                    } else {
                        std::string reply = "Res: " + tokens[0] + "\n";
                        reply += "Error:" + std::to_string(node_id) + ": Invalid exec command";
                        zmq::message_t reply_zmsg(reply.size());
                        memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                        parent_dealer.send(reply_zmsg, zmq::send_flags::none);
                    }
                } else {
                    for (const auto &child: children) {
                        const int child_identity = child.first;
                        int child_port = child.second;
                        // std::cout << "NODE_ID=" << node_id << "FORWARDING TO " << child_identity << ' ' << child_port <<
                                // std::endl;

                        std::string forward_cmd = command_str;

                        zmq::message_t forward_zmsg(forward_cmd.size());
                        memcpy(forward_zmsg.data(), forward_cmd.c_str(), forward_cmd.size());

                        children_socket.connect("tcp://localhost:" + std::to_string(child_port));
                        children_socket.send(zmq::buffer(forward_cmd), zmq::send_flags::none);
                        children_socket.disconnect("tcp://localhost:" + std::to_string(child_port));

                        // std::cout << "NODE_ID=" << node_id << "SENT DONE" << std::endl;
                    }
                }
            } else if (command_str.starts_with("Res")) {
                // std::cout << "NODE_ID=" << node_id << "SENDING TO PARENT" << std::endl;
                parent_dealer.send(zmq::buffer(command_str), zmq::send_flags::none);
            } else {
                std::string reply = "Res: " + tokens[0] + "\n";
                reply += "Error:" + std::to_string(node_id) + ": Unknown command";
                zmq::message_t reply_zmsg(reply.size());
                memcpy(reply_zmsg.data(), reply.c_str(), reply.size());
                parent_dealer.send(reply_zmsg, zmq::send_flags::none);
            }
        }
    }

    parent_dealer.close();
    children_socket.close();
    // std::cout << "FINISHING" << std::endl;

    return 0;
}
