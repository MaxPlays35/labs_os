//
// Created by Aleksandr Mikhailov on 04.12.2024.
//

#include "libraryFirst.h"

float pi(int k) {
    float res = 0.0f;
    float one = 1.0f;

    for (int i = 0; i < k; ++i) {
        res += one / (2.0f * i + 1.0f);
        one *= -1;
    }

    return res * 4.0f;
}