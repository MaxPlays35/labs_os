//
// Created by MaxPlays on 22/12/2024.
//

#include "Block.h"

Block::Block(const std::size_t start, const std::size_t end,
             const std::size_t page, const bool used): start(start), end(end), page(page), size(end - start + 1),
                                                       used(used) {
}

Block &Block::operator=(Block && block) noexcept {
    start = block.start;
    end = block.end;
    page = block.page;
    size = block.size;
    used = block.used;

    return *this;
}

std::size_t Block::get_start() const {
    return start;
}

std::size_t Block::get_end() const {
    return end;
}

std::size_t Block::get_page() const {
    return page;
}

std::size_t Block::get_size() const {
    return size;
}

void Block::set_start(std::size_t start_new) {
    start = start_new;
    size = end - start_new + 1;
}

void Block::set_end(std::size_t end_new) {
    end = end_new;
    size = end - start + 1;
}

void Block::set_used(bool used_new) {
    used = used_new;
}

bool Block::get_used() const {
    return used;
}

bool Block::operator==(const Block & other) const {
    return start == other.start and end == other.end and page == other.end;
}

bool Block::operator<(const Block & other) const {
    if (page != other.page) {
        return page < other.page;
    }

    return start < other.start;
}
