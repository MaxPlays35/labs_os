//
// Created by Aleksandr Mikhailov on 27.09.2024.
//

#include <fstream>
#include <random>
#include <__algorithm/ranges_sort.h>

#include "gtest/gtest.h"
#include "mergesortthreaded/mergesortthreaded.h"

TEST(VERIFY_TESTS, SINGLE_THREAD) {
    std::random_device rd;
    std::mt19937 mt(0);
    std::uniform_int_distribution<int> dist(1, 100);

    size_t size = 1e6;
    std::vector<int> v(size);
    for (auto i = 0; i < size; ++i){
        v[i] = dist(mt);
    }

    const auto pointer = std::make_shared<std::vector<int>>(v);

    const auto time = mergeSortThreaded(pointer, 1);

    std::cout << "Elapsed time: " << time << "ms" << std::endl;

    std::ranges::sort(v);

    EXPECT_EQ(*pointer, v);
}

TEST(VERIFY_TESTS, TWO_THREADS) {
    std::random_device rd;
    std::mt19937 mt(0);
    std::uniform_int_distribution<int> dist(1, 100);

    size_t size = 1e6;
    std::vector<int> v(size);
    for (auto i = 0; i < size; ++i){
        v[i] = dist(mt);
    }

    const auto pointer = std::make_shared<std::vector<int>>(v);

    const auto time = mergeSortThreaded(pointer, 2);

    std::cout << "Elapsed time: " << time << "ms" << std::endl;

    std::ranges::sort(v);

    EXPECT_EQ(*pointer, v);
}

TEST(VERIFY_TESTS, FOUR_THREADS) {
    std::random_device rd;
    std::mt19937 mt(0);
    std::uniform_int_distribution<int> dist(1, 100);

    size_t size = 1e6;
    std::vector<int> v(size);
    for (auto i = 0; i < size; ++i){
        v[i] = dist(mt);
    }

    const auto pointer = std::make_shared<std::vector<int>>(v);

    const auto time = mergeSortThreaded(pointer, 4);

    std::cout << "Elapsed time: " << time << "ms" << std::endl;

    std::ranges::sort(v);

    EXPECT_EQ(*pointer, v);
}

TEST(VERIFY_TESTS, EIGHT_THREADS) {
    std::random_device rd;
    std::mt19937 mt(0);
    std::uniform_int_distribution<int> dist(1, 100);

    size_t size = 1e6;
    std::vector<int> v(size);
    for (auto i = 0; i < size; ++i){
        v[i] = dist(mt);
    }

    const auto pointer = std::make_shared<std::vector<int>>(v);

    const auto time = mergeSortThreaded(pointer, 8);

    std::cout << "Elapsed time: " << time << "ms" << std::endl;

    std::ranges::sort(v);

    EXPECT_EQ(*pointer, v);
}
