//
// Created by Aleksandr Mikhailov on 04.12.2024.
//
#include "librarySecond.h"

float pi(int k) {
    float result = 1.0f;

    for (int i = 1; i <= k; ++i){
        result *= (4.0f * i * i) / (4.0f * i * i - 1.0f);
    }

    return 2.0f * result;
}