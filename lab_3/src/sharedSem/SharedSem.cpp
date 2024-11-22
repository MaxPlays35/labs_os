//
// Created by Aleksandr Mikhailov on 22.11.2024.
//

#include "SharedSem.h"

#include <sys/fcntl.h>

SharedSem::SharedSem(const std::string & name) : name_(name) {
    sem_ = sem_open(name_.c_str(), O_CREAT, 0644, 0);
}

void SharedSem::wait() const {
    sem_wait(sem_);
}

void SharedSem::post() const {
    sem_post(sem_);
}

SharedSem::~SharedSem() {
    sem_close(sem_);
    sem_unlink(name_.c_str());
}
