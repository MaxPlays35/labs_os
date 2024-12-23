//
// Created by MaxPlays on 22/12/2024.
//
#pragma once


#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <bits/memory_resource.h>

#include "../block/Block.h"

using Page = std::unique_ptr<char[]>;

class CustomMemoryResource : public std::pmr::memory_resource {
    static constexpr size_t max_size = 4096;
    std::vector<Block> blocks_;
    std::vector<Page> pages_;
    std::tuple<Block, size_t> get_block(std::size_t bytes);

public:
    void *do_allocate(size_t bytes, size_t alignment) override;

    void do_deallocate(void * ptr, size_t bytes, size_t alignment) override;

    bool do_is_equal(const std::pmr::memory_resource & other) const noexcept override;

    std::size_t get_allocated_memory() const;

    std::size_t get_used_memory() const;
};
