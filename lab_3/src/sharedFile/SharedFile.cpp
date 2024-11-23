//
// Created by Aleksandr Mikhailov on 22.11.2024.
//

#include "SharedFile.h"

#include <iostream>
#include <ostream>
#include <utility>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>

#include "../consts/Consts.h"

SharedFile::SharedFile(std::string name) : name_(std::move(name)) {
    fd_ = shm_open(name_.c_str(), O_CREAT | O_RDWR, 0644);
    if (fd_ == -1) {
        std::cerr << "Error opening shared file: " << name_ << std::endl;
        exit(EXIT_FAILURE);
    }

    ftruncate(fd_, kFileLength);
}

int SharedFile::getFd() const {
    return fd_;
}

SharedFile::~SharedFile() {
    close(fd_);
    shm_unlink(name_.c_str());
}
