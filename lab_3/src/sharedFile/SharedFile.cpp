//
// Created by Aleksandr Mikhailov on 22.11.2024.
//

#include "SharedFile.h"

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>

constexpr int kFileLength = 1024;

SharedFile::SharedFile(const std::string & name) : name_(name) {
    fd_ = shm_open(name_.c_str(), O_CREAT | O_RDWR, 0644);
    ftruncate(fd_, kFileLength);
}

int SharedFile::getFd() const {
    return fd_;
}

SharedFile::~SharedFile() {
    close(fd_);
    shm_unlink(name_.c_str());
}
