//
// Created by MaxPlays on 24/12/2024.
//
#pragma once

#include <cstddef>
#include <bits/memory_resource.h>

class BaseMemoryResource : public std::pmr::memory_resource{
public:
    virtual std::size_t get_allocated_memory() const = 0;

    virtual std::size_t get_used_memory() const = 0;
};
