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

#include "consts/Consts.h"
#include "sharedFile/SharedFile.h"
#include "sharedMem/SharedMem.h"
#include "sharedSem/SharedSem.h"

int main(int argc, char* argv[]) {
    if (argc != 1) {
        std::cerr << "Bad usage" << std::endl;
        return -1;
    }

    auto sem1 = SharedSem(kSemParentName);
    auto sem2 = SharedSem(kSemChildName);

    auto file1 = SharedFile(kParentFilename);
    auto file2 = SharedFile(kChildFilename);

    auto bufferP2C = SharedMem(file1.getFd(), kFileLength);
    auto bufferC2P = SharedMem(file2.getFd(), 1);

    FILE* file_old = fopen(argv[0], "w");
    dup2(fileno(file_old), STDOUT_FILENO);

    int length = 0;

    while(true) {
        sem2.wait();

        if (bufferP2C.buffer[0] == '!') {
            sem1.post();
            break;
        }

        std::string temp(bufferP2C.buffer, std::strlen(bufferP2C.buffer));

        if (temp.ends_with('.') or temp.ends_with(';')) {
            std::cout << temp << std::endl;
            bufferC2P.buffer[0] = kOne;
        } else {
            bufferC2P.buffer[0] = kZero;
        }

        sem1.post();
    }

    close(fileno(file_old));
    return 0;
}
