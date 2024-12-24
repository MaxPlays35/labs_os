//
// Created by MaxPlays on 24/12/2024.
//
#pragma once
#include <array>
#include <string>

class TestClass {
    int id_;
    std::string string_;
    std::array<std::size_t, 5> array_{};
public:
    TestClass();

    ~TestClass() = default;
};
