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

const size_t kFileLength = 1024;

void ParentWork(std::istream & in, std::ostream & out) {
    std::string str;
    std::string errors;
    std::string filename;
    sem_t* sem1 = sem_open("/sync1", O_CREAT, 0644, 0);
    sem_t* sem2 = sem_open("/sync2", O_CREAT, 0644, 0);
    int buffer = 0;

    std::cout << errno << std::endl;



    out << "Enter a name for file" << std::endl;
    in >> filename;

    auto file1 = shm_open("/tmp1.txt", O_CREAT | O_RDWR, 0644);
    auto file2 = shm_open("/tmp2.txt", O_CREAT | O_RDWR, 0644);

    struct stat mapstat;
    fstat(file1, &mapstat);

    if (mapstat.st_size < kFileLength) {
        ftruncate(file1, kFileLength);
    }

    fstat(file2, &mapstat);

    if (mapstat.st_size < kFileLength) {
        ftruncate(file2, kFileLength);
    }

    char* bufferP2C = static_cast<char*>(mmap(nullptr, kFileLength, PROT_READ | PROT_WRITE, MAP_SHARED, file1, 0));
    char* bufferC2P = static_cast<char*>(mmap(nullptr, 1, PROT_READ | PROT_WRITE, MAP_SHARED, file2, 0));
    std::cout << errno << std::endl;

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

            strcpy(bufferP2C, str.c_str());
            std::cout << *bufferP2C << std::endl;
            sem_post(sem2);
            sem_wait(sem1);

            if (str == "!") {
                break;
            }

            if (!bufferC2P[0]) {
                out << static_cast<int>(bufferC2P[0]) << std::endl;
                out << "Verification failed" << std::endl;
            }
        }
    }

    munmap(bufferP2C, kFileLength);
    munmap(bufferC2P, 1);
    close(file1);
    close(file2);
    sem_close(sem1);
    sem_close(sem2);
    close(file1);
    sem_unlink("/sync1");
    sem_unlink("/sync2");
    shm_unlink("/tmp1.txt");
    shm_unlink("/tmp2.txt");
    wait(nullptr);
}
