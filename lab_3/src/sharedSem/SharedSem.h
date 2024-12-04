//
// Created by Aleksandr Mikhailov on 22.11.2024.
//
#pragma once
#include <string>
#include <sys/semaphore.h>

class SharedSem {
private:
    sem_t* sem_;
    const std::string name_;
public:
    explicit SharedSem(std::string name);

    void wait();

    void post();

    ~SharedSem();
};

