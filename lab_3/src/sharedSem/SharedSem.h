//
// Created by Aleksandr Mikhailov on 22.11.2024.
//
#pragma once
#include <string>
#include <sys/semaphore.h>

class SharedSem {
private:
    sem_t* sem_;
    std::string name_;
public:
    explicit SharedSem(const std::string & name);

    void wait() const;

    void post() const;

    ~SharedSem();
};

