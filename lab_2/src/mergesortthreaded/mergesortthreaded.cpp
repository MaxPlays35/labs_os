//
// Created by Aleksandr Mikhailov on 26.10.2024.
//

#include "mergesortthreaded.h"

#include <chrono>
#include <iostream>
#include <pthread.h>

#include "../threadwork/threadwork.h"
#include "../mergesort/mergesort.h"

long long mergeSortThreaded(std::shared_ptr<std::vector<int>> array, int threads) {
    std::vector<ThreadWork> work;
    std::vector<pthread_t> threads_array(threads);

    const size_t size_array = (*array).size();
    int partSize = std::ceil(static_cast<double>(size_array) / threads);


    //make thread work
    for (size_t i = 0; i < threads; ++i) {
        auto left = i * partSize;
        auto right = std::min((i + 1) * partSize - 1, size_array - 1);
        work.emplace_back(array, left, right);
    }

    auto start = std::chrono::high_resolution_clock::now();

    //run threads
    for (size_t i = 0; i < threads; ++i) {
        pthread_t t;
        pthread_create(&t, nullptr, threadRoutine, &work[i]);
        if (t == nullptr) {
            std::cerr << "Failed to create thread" << std::endl;
            return 1;
        }
        threads_array[i] = t;
    }

    //waiting all threads
    for (size_t i = 0; i < threads; ++i) {
        pthread_join(threads_array[i], nullptr);
    }

    //merging all parts
    for (int currentSize = partSize; currentSize < size_array; currentSize *= 2) {
        for (int left = 0; left < size_array; left += 2 * currentSize) {
            int middle = std::min(left + currentSize - 1, (int)size_array - 1);
            int right = std::min(left + 2 * currentSize - 1, (int)size_array - 1);

            if (middle < right) {
                merge(array, left, middle, right);
            }
        }
    }



    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
