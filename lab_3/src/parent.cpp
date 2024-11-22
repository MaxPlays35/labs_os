//
// Created by Aleksandr Mikhailov on 27.09.2024.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/semaphore.h>
#include <sys/stat.h>

#include "sharedFile/SharedFile.h"
#include "sharedMem/SharedMem.h"
#include "sharedSem/SharedSem.h"

const size_t kFileLength = 1024;

void ParentWork(std::istream & in, std::ostream & out) {
    std::string str;
    std::string errors;
    std::string filename;

    auto sem1 = SharedSem("/sync1");
    auto sem2 = SharedSem("/sync2");

    int buffer = 0;

    out << "Enter a name for file" << std::endl;
    in >> filename;

    auto file1 = SharedFile("/tmp1.txt");
    auto file2 = SharedFile("/tmp2.txt");

    auto bufferP2C = SharedMem(file1.getFd(), kFileLength);
    auto bufferC2P = SharedMem(file2.getFd(), 1);

    int child_pid = fork();

    if (child_pid == -1) {
        std::cerr << "Fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        constexpr auto* child_path = "./lab_3_child";

        if (execl(child_path, filename.c_str(), nullptr) == -1) {
            std::cerr << "Exec failed" << std::endl;
            std::cerr << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

    } else {

        std::getline(in, str);
        while(true) {
            if (!std::getline(in, str)) {
                break;
            }

            std::strcpy(bufferP2C.buffer, str.c_str());

            sem2.post();
            sem1.wait();

            if (str == "!") {
                break;
            }

            if (!bufferC2P.buffer[0]) {
                out << "Verification failed" << std::endl;
            }
        }
    }

    wait(nullptr);
}
