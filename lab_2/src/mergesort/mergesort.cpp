//
// Created by Aleksandr Mikhailov on 25.10.2024.
//

#include "mergesort.h"

void merge(std::shared_ptr<std::vector<int>> array, const size_t left, const size_t middle, const size_t right) {
    size_t leftSize = middle - left + 1;
    size_t rightSize = right - middle;

    std::vector<int> leftArray(leftSize);
    std::vector<int> rightArray(rightSize);

    for (size_t i = 0; i < leftSize; ++i)
        leftArray[i] = (*array)[left + i];
    for (size_t j = 0; j < rightSize; ++j)
        rightArray[j] = (*array)[middle + 1 + j];

    size_t i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize) {
        if (leftArray[i] <= rightArray[j]) {
            (*array)[k] = leftArray[i];
            i++;
        } else {
            (*array)[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < leftSize) {
        (*array)[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < rightSize) {
        (*array)[k] = rightArray[j];
        j++;
        k++;
    }
}

void mergeSortInternal(std::shared_ptr<std::vector<int>> array, const size_t left, const size_t right) {
    if (left < right) {
        const size_t middle = left + (right - left) / 2;

        mergeSortInternal(array, left, middle);
        mergeSortInternal(array, middle + 1, right);

        merge(array, left, middle, right);
    }
}

void mergeSort(std::vector<int> &array) {
    const auto pointer = std::make_shared<std::vector<int>>(array);
    mergeSortInternal(pointer, 0, array.size() - 1);
}
