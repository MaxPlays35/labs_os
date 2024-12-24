//
// Created by MaxPlays on 22/12/2024.
//

#include "SimpleMemoryResource.h"

#include <algorithm>
#include <bits/ranges_algo.h>

std::tuple<Block, size_t> SimpleMemoryResource::get_block(std::size_t bytes) {
    for (std::size_t i = 0; i < blocks_.size(); ++i) {
        if (blocks_[i].get_used()) {
            continue;
        }

        if (blocks_[i].get_size() >= bytes) {
            return std::make_tuple(Block(blocks_[i].get_start(), blocks_[i].get_start() + bytes - 1,
                                         blocks_[i].get_page(), true), i);
        }
    }

    int last_page = blocks_.empty() ? -1 : blocks_.back().get_page();

    return std::make_tuple(Block(0, bytes - 1, last_page + 1, true), blocks_.size());
}

void *SimpleMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    auto [block, idx] = get_block(bytes);

    if (idx == blocks_.size()) {
        pages_.emplace_back(static_cast<char *>(std::malloc(max_size)));
        blocks_.emplace_back(0, max_size - 1, block.get_page(), false);
        blocks_.back().set_start(block.get_end() + 1);
        blocks_.push_back(block);
        std::sort(blocks_.begin(), blocks_.end());

        return pages_[block.get_page()].get() + block.get_start();
    }

    if (blocks_[idx].get_size() == block.get_size()) {
        blocks_[idx].set_used(true);

        return pages_[block.get_page()].get() + block.get_start();
    }

    blocks_[idx].set_start(block.get_end() + 1);
    blocks_.push_back(block);
    std::sort(blocks_.begin(), blocks_.end());

    return pages_[block.get_page()].get() + block.get_start();;
}

void SimpleMemoryResource::do_deallocate(void * ptr, size_t bytes, size_t alignment) {
    std::size_t idx = 0;
    bool found = false;

    for (std::size_t i = 0; i < blocks_.size(); ++i) {
        if (!blocks_[i].get_used()) {
            continue;
        }

        if (pages_[blocks_[i].get_page()].get() + blocks_[i].get_start() == ptr) {
            idx = i;
            found = true;
            break;
        }
    }

    if (!found) {
        throw std::logic_error("Trying to free a block that doesn't exist");
    }

    blocks_[idx].set_used(false);

    if (idx != 0 and blocks_[idx].get_start() != 0 and !blocks_[idx - 1].get_used()) {
        std::size_t start = blocks_[idx - 1].get_start();
        blocks_[idx].set_start(start);
        blocks_.erase(std::ranges::remove(blocks_, blocks_[idx - 1]).begin(), blocks_.end());
    }

    if (idx != blocks_.size() - 1 and blocks_[idx].get_end() != max_size - 1 and !blocks_[idx + 1].get_used()) {
        std::size_t end = blocks_[idx + 1].get_end();
        blocks_[idx].set_end(end);
        blocks_.erase(std::ranges::remove(blocks_, blocks_[idx + 1]).begin(), blocks_.end());
    }
}

bool SimpleMemoryResource::do_is_equal(const std::pmr::memory_resource & other) const noexcept {
    return this == &other;
}

std::size_t SimpleMemoryResource::get_allocated_memory() const {
    return max_size * pages_.size();
}

std::size_t SimpleMemoryResource::get_used_memory() const {
    std::size_t used = 0;

    for (auto & block: blocks_) {
        if (!block.get_used()) {
            continue;
        }

        used += block.get_size();
    }

    return used;
}
