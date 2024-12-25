//
// Created by MaxPlays on 20/12/2024.
//

// manager.cpp

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
#include <__ranges/split_view.h>

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

// Структура для представления узла дерева
struct TreeNode {
    int id;
    pid_t pid;
    int port;
    TreeNode* left;
    TreeNode* right;
    bool available;

    TreeNode(int node_id, pid_t process_id, int node_port)
        : id(node_id), pid(process_id), port(node_port), left(nullptr), right(nullptr), available(true) {}
};

// Рекурсивная функция для удаления узла и его детей
void deleteSubtree(TreeNode* node) {
    if (!node) return;

    // Удаление левых и правых поддеревьев
    deleteSubtree(node->left);
    deleteSubtree(node->right);

    // Завершаем процесс, связанный с текущим узлом, если он доступен
    if (node->available && node->pid > 0) {
        kill(node->pid, SIGTERM);
    }

    // Удаляем текущий узел
    delete node;
}

// Функция для поиска и удаления узла по ID
TreeNode* deleteNodeById(TreeNode* root, int id) {
    if (!root) return nullptr;

    if (root->id == id) {
        // Удаляем весь поддерево, включая текущий узел
        deleteSubtree(root);
        return nullptr;
    }

    // Рекурсивно ищем и удаляем в левом и правом поддереве
    root->left = deleteNodeById(root->left, id);
    root->right = deleteNodeById(root->right, id);

    return root;
}

// Функция для получения порта по ID узла
int get_port(int node_id) {
    return 5000 + node_id;
}

int main() {
    // Инициализация ZeroMQ контекста
    zmq::context_t context(1);

    // Создание ROUTER сокета для менеджера
    zmq::socket_t manager_socket(context, ZMQ_DEALER);
    manager_socket.bind("tcp://*:4000"); // Порт менеджера

    // Структура для управления деревом
    TreeNode* root = nullptr;
    std::unordered_map<int, TreeNode*> nodes_map;

    zmq::socket_t children_socket(context, ZMQ_DEALER);

    // Очередь для уровня обхода (для добавления в сбалансированное место)
    std::queue<TreeNode*> node_queue;

    // Слушаем сообщения от узлов
    zmq::pollitem_t items[] = {
        { static_cast<void*>(manager_socket), 0, ZMQ_POLLIN, 0 }
    };

    std::set<int> unfinished;

    std::vector<std::thread> threads;

    // Регистрация обработчика сигналов для корректного завершения
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    auto th = std::thread([&manager_socket, &unfinished]() {
        // Ожидание ответа с таймаутом

        while (true) {
            zmq::pollitem_t poll_items[] = {
                            { static_cast<void*>(manager_socket), 0, ZMQ_POLLIN, 0 }
                        };

            zmq::poll(poll_items, 1, 3000); // 5 секунд
            // std::cout << "CONSUMING" << std::endl;

            if (poll_items[0].revents & ZMQ_POLLIN) {
                // Получение всех частей сообщения
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
            break; // EOF
        }

        // Разделение команды на токены
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

            // Проверка, существует ли узел с таким ID
            if (nodes_map.find(node_id) != nodes_map.end()) {
                std::cout << "Error: Already exists\n";
                continue;
            }

            // Если дерево пусто, создаём корень
            if (root == nullptr) {
                // Спавним процесс вычислительного узла
                pid_t pid = fork();
                if (pid == -1) {
                    std::cout << "Error: Fork failed\n";
                    continue;
                }
                if (pid == 0) {
                    // Дочерний процесс: запускаем compute_node
                    char id_str[10];
                    char parent_port_str[10];
                    sprintf(id_str, "%d", node_id);
                    sprintf(parent_port_str, "%d", 4000); // Менеджер слушает на порту 4000
                    execl("./lab_5_child", "./lab_5_child", id_str, parent_port_str, (char*)NULL);
                    // Если execl не удалось
                    exit(1);
                } else {
                    // Родительский процесс
                    root = new TreeNode(node_id, pid, get_port(node_id));
                    children_socket.connect("tcp://localhost:" + std::to_string(get_port(node_id)));
                    nodes_map[node_id] = root;
                    node_queue.push(root);
                    std::cout << "Ok: " << pid << "\n";
                }
                continue;
            }

            // Поиск первого узла в очереди с менее чем двумя детьми
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

            // Спавним процесс вычислительного узла
            pid_t pid = fork();
            if (pid == -1) {
                std::cout << "Error: Fork failed\n";
                continue;
            }
            if (pid == 0) {
                // Дочерний процесс: запускаем compute_node
                char id_str[10];
                char parent_port_str[10];
                sprintf(id_str, "%d", node_id);
                sprintf(parent_port_str, "%d", parent->port);
                execl("./lab_5_child", "./lab_5_child", id_str, parent_port_str, (char*)NULL);
                // Если execl не удалось
                exit(1);
            } else {
                // Родительский процесс
                TreeNode* new_node = new TreeNode(node_id, pid, get_port(node_id));
                nodes_map[node_id] = new_node;

                std::string msg = std::to_string(id) + " " + "created " + std::to_string(parent->id) + " " + std::to_string(new_node->id);
                ++id;

                zmq::message_t message(msg.size());
                memcpy(message.data(), msg.c_str(), msg.size());

                children_socket.send(message, zmq::send_flags::none);

                // manager_socket.send(message, zmq::send_flags::none);

                // Присоединение к дереву
                if (parent->left == nullptr) {
                    parent->left = new_node;
                } else {
                    parent->right = new_node;
                    // После добавления двух детей, узел больше не нужен в очереди
                    node_queue.pop();
                }

                // Добавляем нового узла в очередь, так как он может принять своих детей
                node_queue.push(new_node);

                std::cout << "Ok: " << pid << "\n";
            }
        }
        else if (command == "exec") {
            // Формат: exec id [params]
            int target_id;
            ss >> target_id;
            if (ss.fail()) {
                std::cout << "Error: Invalid node_id\n";
                continue;
            }

            std::string params;
            getline(ss, params);
            // Удаление ведущего пробела
            if (!params.empty() && params[0] == ' ') {
                params = params.substr(1);
            }

            // Проверка существования узла
            if (nodes_map.find(target_id) == nodes_map.end()) {
                std::cout << "Error: Node not found\n";
                continue;
            }

            TreeNode* target_node = nodes_map[target_id];
            if (!target_node->available) {
                std::cout << "Error: Node is unavailable\n";
                continue;
            }

            // Отправка команды через дерево
            // Отправляем на корень
            std::string msg = std::to_string(id) + " " + "exec " + std::to_string(target_id) + " " + params;
            ++id;

            // Send the message to the root node
            // zmq::message_t identity; // Empty, as ROUTER identifies
            // zmq::message_t empty;
            zmq::message_t message(msg.size());
            memcpy(message.data(), msg.c_str(), msg.size());

            // Отправляем в менеджер (ROUTER socket) -> compute_node
            // manager_socket.send(identity, zmq::send_flags::sndmore);
            // manager_socket.send(empty, zmq::send_flags::sndmore);
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
            // Формат: ping id
            int target_id;
            ss >> target_id;
            if (ss.fail()) {
                std::cout << "Error: Invalid node_id\n";
                continue;
            }

            // Проверка существования узла
            if (nodes_map.find(target_id) == nodes_map.end()) {
                std::cout << "Error: Not found\n";
                continue;
            }

            TreeNode* target_node = nodes_map[target_id];
            if (!target_node->available) {
                std::cout << "Ok: 0\n";
                continue;
            }

            // Отправка ping команды через дерево
            std::string msg = std::to_string(id) + " " + "ping " + std::to_string(target_id);
            ++id;

            // Send the message to the root node
            zmq::message_t message(msg.size());
            memcpy(message.data(), msg.c_str(), msg.size());

            // Отправляем в менеджер (ROUTER socket) -> compute_node
            // manager_socket.send(identity, zmq::send_flags::sndmore);
            // manager_socket.send(empty, zmq::send_flags::sndmore);
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

        // Обработка завершённых дочерних процессов
        while (waitpid(-1, NULL, WNOHANG) > 0) {

        }
    }

    for (auto & thread : threads) {
        thread.join();
    }

    th.join();
}
