//
// Created by Aleksandr Mikhailov on 22.11.2024.
//
#pragma once
#include <cstddef>

class SharedMem {
public:
    char* buffer;
    const size_t size;

    SharedMem(int fd, size_t size);

    ~SharedMem();
};
