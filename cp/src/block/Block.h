//
// Created by MaxPlays on 22/12/2024.
//
#pragma once
#include <cstddef>
#include <bits/ranges_cmp.h>


class Block {
private:
    std::size_t start;
    std::size_t end;
    std::size_t page;
    std::size_t size;
    bool used;

public:
    Block() = default;

    Block(std::size_t start, std::size_t end, std::size_t page, bool used);

    Block(const Block & block) = default;

    Block(Block && block) noexcept = default;

    Block & operator=(const Block & block) = default;

    Block & operator=(Block && block) noexcept;

    std::size_t get_start() const;

    std::size_t get_end() const;

    std::size_t get_page() const;

    std::size_t get_size() const;

    void set_start(std::size_t start_new);

    void set_end(std::size_t end_new);

    void set_used(bool used_new);

    bool get_used() const;

    bool operator==(const Block & other) const;

    bool operator<(const Block &) const;

    ~Block() = default;
};
