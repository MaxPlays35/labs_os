//
// Created by Aleksandr Mikhailov on 27.09.2024.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

void ParentWork(std::istream & in, std::ostream & out) {
    std::string str;
    std::string errors;
    std::string filename;
    int buffer = 0;

    out << "Enter a name for file" << std::endl;
    in >> filename;

    int pipesP2C[2];
    int pipesC2P[2];

    if (pipe(pipesP2C) == -1 || pipe(pipesC2P) == -1) {
        std::cerr << "Creating pipes failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    int child_pid = fork();

    if (child_pid == -1) {
        std::cerr << "Fork failed" << std::endl;
    }

    if (child_pid == 0) {
        close(pipesP2C[1]);
        close(pipesC2P[0]);
        dup2(pipesP2C[0], STDIN_FILENO);
        dup2(pipesC2P[1], STDERR_FILENO);

        constexpr auto* child_path = "./child";

        if (execl(child_path, filename.c_str(), nullptr) == -1) {
            std::cerr << "Exec failed" << std::endl;
            exit(EXIT_FAILURE);
        }

    } else {
        close(pipesP2C[0]);
        close(pipesC2P[1]);

        std::getline(in, str);
        while(std::getline(in, str)) {
            write(pipesP2C[1], (str + '\n').c_str(), str.size() + 1);

            if (str == "!") {
                break;
            }

            read(pipesC2P[0], &buffer, sizeof(buffer));

            if (!buffer) {
                out << "Verification failed" << std::endl;
            }

        }
    }

    wait(nullptr);
}
