//
// Created by MaxPlays on 24/12/2024.
//

#include <cmath>

#include "MacMemoryResource.h"

std::size_t MacMemoryResource::round(std::size_t n) const {
    if (n == 0) {
        return 1;
    }

    double p = std::ceil(std::log2(static_cast<double>(n)));
    auto power = static_cast<std::size_t>(1ULL << static_cast<unsigned>(p));

    return power;
}

std::size_t MacMemoryResource::index(std::size_t size) const {
    return static_cast<std::size_t>(std::log2(static_cast<double>(size)));
}

void MacMemoryResource::allocate_page(std::size_t block_size) {
    pages_.emplace_back(static_cast<char *>(std::malloc(max_size)));
    std::size_t idx = index(block_size);
    std::size_t page_idx = pages_.size() - 1;

    for (std::size_t i = 0; i < max_size / block_size; ++i) {
        free_blocks_[idx].emplace_front(block_size * i, block_size * (i + 1) - 1, page_idx, false);
    }
}

MacMemoryResource::MacMemoryResource() : free_blocks_(13) {
}

void * MacMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    std::size_t block_size = round(bytes);
    std::size_t idx = index(block_size);

    if (free_blocks_[idx].empty()) {
        allocate_page(block_size);
    }

    auto block = free_blocks_[idx].front();
    free_blocks_[idx].pop_front();

    block.set_used(true);
    blocks_[pages_[block.get_page()].get() + block.get_start()] = block;

    return pages_[block.get_page()].get() + block.get_start();
}

void MacMemoryResource::do_deallocate(void * ptr, size_t bytes, size_t alignment) {
    auto & block = blocks_[ptr];

    std::size_t idx = index(block.get_size());

    block.set_used(false);
    free_blocks_[idx].push_front(block);
}

bool MacMemoryResource::do_is_equal(const std::pmr::memory_resource & other) const noexcept {
    return this == &other;
}

std::size_t MacMemoryResource::get_allocated_memory() const {
    return max_size * pages_.size();
}

std::size_t MacMemoryResource::get_used_memory() const {
    std::size_t used = 0;

    for (auto & [_, block]: blocks_) {
        if (!block.get_used()) {
            continue;
        }

        used += block.get_size();
    }

    return used;
}


