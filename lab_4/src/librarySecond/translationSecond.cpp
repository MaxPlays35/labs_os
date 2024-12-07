//
// Created by Aleksandr Mikhailov on 04.12.2024.
//

#include "librarySecond.h"

std::string translation(long x) {
    int count = std::floor(std::log(x) / std::log(3.0f) + 1);

    if (x == 3) {
        count = 2;
    }

    std::string result(count, '0');

    for (int i = 0; i < count; ++i) {
        result[count - i - 1] = '0' + (x % 3);
        x /= 3;
    }

    return result;
}