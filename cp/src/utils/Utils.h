//
// Created by MaxPlays on 24/12/2024.
//
#pragma once
#include <functional>
#include <tuple>

std::tuple<double, std::size_t> measure_time(const std::function<std::size_t()>& function);
