//
// Created by Aleksandr Mikhailov on 04.12.2024.
//
#include "cmath"

#include "libraryFirst.h"

std::string translation(long x) {
    int count = std::floor(std::log2(x) + 1);

    if (x == 2) {
        count = 2;
    }

    std::string result(count, '0');

    for (int i = 0; i < count; ++i) {
        result[count - i - 1] = '0' + (x % 2);
        x /= 2;
    }

    return result;
}