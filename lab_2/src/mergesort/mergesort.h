//
// Created by Aleksandr Mikhailov on 25.10.2024.
//
#pragma once
#include <vector>

void merge(std::shared_ptr<std::vector<int>> array, size_t left, size_t middle, size_t right);

void mergeSortInternal(std::shared_ptr<std::vector<int>> array, size_t left, size_t right);

void mergeSort(std::vector<int>& array);
