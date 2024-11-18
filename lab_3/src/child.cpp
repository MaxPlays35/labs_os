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

extern int errno;

int main(int argc, char* argv[]) {
    if (argc != 1) {
        std::cerr << "Bad usage" << std::endl;
        return -1;
    }

    sem_t* sem1 = sem_open("/sync1", O_CREAT, 0644, 0);
    sem_t* sem2 = sem_open("/sync2", O_CREAT, 0644, 0);

    auto file1 = shm_open("/tmp1.txt", O_CREAT | O_RDWR, 0644);
    auto file2 = shm_open("/tmp2.txt", O_CREAT | O_RDWR, 0644);

    char* bufferP2C = static_cast<char*>(mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, file1, 0));
    char* bufferC2P = static_cast<char*>(mmap(nullptr, 1, PROT_READ | PROT_WRITE, MAP_SHARED, file2, 0));
    FILE* file_old = fopen(argv[0], "w");
    dup2(fileno(file_old), STDOUT_FILENO);

    constexpr int one = 1;
    constexpr int zero = 0;
    int length = 0;

    while(true) {
        sem_wait(sem2);
        std::cerr << "Got from parent" << std::endl;

        if (bufferP2C[0] == '!') {
            sem_post(sem1);
            break;
        }

        std::string temp(bufferP2C, strlen(bufferP2C));
        std::cerr << temp << std::endl;

        if (temp.ends_with('.') or temp.ends_with(';')) {
            std::cout << temp << std::endl;
            bufferC2P[0] = one;
        } else {
            bufferC2P[0] = zero;
        }

        std::cerr << "Release to parent" << std::endl;
        sem_post(sem1);
    }

    close(fileno(file_old));
    munmap(bufferP2C, 1024);
    munmap(bufferC2P, 1);
    return 0;
}
