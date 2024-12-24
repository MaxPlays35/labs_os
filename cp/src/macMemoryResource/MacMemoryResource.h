//
// Created by MaxPlays on 24/12/2024.
//
#pragma once

#include <forward_list>
#include <unordered_map>
#include <memory>
#include <vector>
#include <bits/memory_resource.h>

#include "../baseMemoryResource/BaseMemoryResource.h"
#include "../block/Block.h"

using Page = std::unique_ptr<char[]>;

class MacMemoryResource : public BaseMemoryResource {
    static constexpr size_t max_size = 4096;
    std::vector<std::forward_list<Block>> free_blocks_;
    std::vector<Page> pages_;
    std::unordered_map<void*, Block> blocks_;

    std::size_t round(std::size_t n) const;

    std::size_t index(std::size_t size) const;

    void allocate_page(std::size_t block_size);

public:
    MacMemoryResource();

    void *do_allocate(size_t bytes, size_t alignment) override;

    void do_deallocate(void * ptr, size_t bytes, size_t alignment) override;

    bool do_is_equal(const std::pmr::memory_resource & other) const noexcept override;

    std::size_t get_allocated_memory() const override;

    std::size_t get_used_memory() const override;
};
