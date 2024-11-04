//
// Created by Aleksandr Mikhailov on 26.10.2024.
//

#include "threadwork.h"

#include "../mergesort/mergesort.h"

void* threadRoutine(void *arg) {
    auto args = static_cast<ThreadWork *>(arg);
    mergeSortInternal(args->array, args->left, args->right);
}
