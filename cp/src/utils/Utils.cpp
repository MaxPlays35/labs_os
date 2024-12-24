//
// Created by MaxPlays on 24/12/2024.
//

#include "Utils.h"

#include <chrono>

std::tuple<double, std::size_t> measure_time(const std::function<std::size_t()> & function) {
    auto start = std::chrono::high_resolution_clock::now();
    auto res = function();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return std::make_tuple(elapsed.count(), res);
}
