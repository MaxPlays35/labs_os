//
// Created by MaxPlays on 27/09/2024.
//

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/semaphore.h>
#include <sys/stat.h>

#include "sharedFile/SharedFile.h"
#include "sharedMem/SharedMem.h"
#include "sharedSem/SharedSem.h"

constexpr size_t kFileLength = 1024;

int main(int argc, char* argv[]) {
    if (argc != 1) {
        std::cerr << "Bad usage" << std::endl;
        return -1;
    }

    auto sem1 = SharedSem("/sync1");
    auto sem2 = SharedSem("/sync2");

    auto file1 = SharedFile("/tmp1.txt");
    auto file2 = SharedFile("/tmp2.txt");

    auto bufferP2C = SharedMem(file1.getFd(), kFileLength);
    auto bufferC2P = SharedMem(file2.getFd(), 1);

    FILE* file_old = fopen(argv[0], "w");
    dup2(fileno(file_old), STDOUT_FILENO);

    constexpr int one = 1;
    constexpr int zero = 0;
    int length = 0;

    while(true) {
        sem2.wait();
        // std::cerr << "Got from parent" << std::endl;

        if (bufferP2C.buffer[0] == '!') {
            sem1.post();
            break;
        }

        std::string temp(bufferP2C.buffer, std::strlen(bufferP2C.buffer));
        // std::cerr << temp << std::endl;

        if (temp.ends_with('.') or temp.ends_with(';')) {
            std::cout << temp << std::endl;
            bufferC2P.buffer[0] = one;
        } else {
            bufferC2P.buffer[0] = zero;
        }

        // std::cerr << "Release to parent" << std::endl;
        sem1.post();
    }

    close(fileno(file_old));
    return 0;
}
