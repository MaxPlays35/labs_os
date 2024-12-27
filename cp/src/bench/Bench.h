//
// Created by MaxPlays on 24/12/2024.
//
#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>
#include <random>

#include "../baseMemoryResource/BaseMemoryResource.h"

template<class T>
    requires std::is_default_constructible_v<T>
size_t simple_benchmark_allocator(size_t iterations, std::shared_ptr<BaseMemoryResource> memory_resource) {
    std::pmr::polymorphic_allocator<T> allocator(memory_resource.get());

    std::vector<T *> objects;
    for (auto i = 0; i < iterations; ++i) {
        objects.push_back(new(allocator.allocate(1)) T());
    }

    auto usedMem = memory_resource->get_used_memory();

    for (auto obj: objects) {
        allocator.deallocate(obj, 1);
    }

    return usedMem;
}

template<class T>
    requires std::is_default_constructible_v<T>
void realistic_benchmark_allocator(size_t iterations, std::shared_ptr<BaseMemoryResource> memory_resource) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> probDist(0.0, 1.0);
    std::uniform_int_distribution<size_t> sizeDist(16, 4096);
    std::pmr::polymorphic_allocator<T> allocator(memory_resource.get());

    std::ofstream file("bench_1.txt", std::ios_base::app);
    file << "id time used allocated type objects" << std::endl;

    double allocationProbability = 0.8f;
    std::size_t operation_id = 0;


    std::vector<T *> activePointers;
    activePointers.reserve(iterations);

    size_t allocations = 0;
    size_t deallocations = 0;

    auto totalStart = std::chrono::high_resolution_clock::now();
    double allocTime = 0.0;
    double freeTime = 0.0;

    for (size_t i = 0; i < iterations; ++i) {
        double p = probDist(gen);
        if (p < allocationProbability || activePointers.empty()) {
            size_t size = sizeDist(gen);
            auto startAlloc = std::chrono::high_resolution_clock::now();
            T * ptr = allocator.allocate(size);
            auto endAlloc = std::chrono::high_resolution_clock::now();
            auto execution = std::chrono::duration<double, std::milli>(endAlloc - startAlloc).count();
            allocTime += execution;

            if (ptr != nullptr) {
                activePointers.push_back(ptr);
                file << operation_id++ << ' ' << execution << ' ' << memory_resource->get_used_memory() << ' ' <<
                        memory_resource->get_allocated_memory() << ' ' << "ALLOCATE" << ' ' << activePointers.size() <<
                        std::endl;;
                allocations++;
            }
        } else {
            std::uniform_int_distribution<size_t> indexDist(0, activePointers.size() - 1);
            size_t index = indexDist(gen);
            T * ptr = activePointers[index];

            auto startFreeOp = std::chrono::high_resolution_clock::now();
            allocator.deallocate(ptr, 1);
            auto endFreeOp = std::chrono::high_resolution_clock::now();
            auto execution = std::chrono::duration<double, std::milli>(endFreeOp - startFreeOp).count();
            freeTime += execution;

            activePointers[index] = activePointers.back();
            activePointers.pop_back();

            file << operation_id++ << ' ' << execution << ' ' << memory_resource->get_used_memory() << ' ' <<
                    memory_resource->get_allocated_memory() << ' ' << "DEALLOCATE" << ' ' << activePointers.size() <<
                    std::endl;

            deallocations++;
        }
    }

    std::size_t offset = 0;
    for (T * ptr: activePointers) {
        auto startFreeOp = std::chrono::high_resolution_clock::now();
        allocator.deallocate(ptr, 1);
        auto endFreeOp = std::chrono::high_resolution_clock::now();
        ++offset;
        auto execution = std::chrono::duration<double, std::milli>(endFreeOp - startFreeOp).count();
        freeTime += execution;
        file << operation_id++ << ' ' << execution << ' ' << memory_resource->get_used_memory() << ' ' <<
                memory_resource->get_allocated_memory() << ' ' << "DEALLOCATE" << ' ' << activePointers.size() - offset
                <<
                std::endl;
        deallocations++;
    }

    auto totalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> totalTime = totalEnd - totalStart;

    std::cout << "Total operations: " << iterations << "\n";
    std::cout << "Allocations: " << allocations << "\n";
    std::cout << "Deallocations: " << deallocations << "\n";
    std::cout << "Total time: " << totalTime.count() << " ms\n";
    std::cout << "Total allocation time: " << allocTime << " ms\n";
    std::cout << "Total deallocation time: " << freeTime << " ms\n";
    if (allocations > 0)
        std::cout << "Avg allocation time: " << (allocTime / allocations) << " ms\n";
    if (deallocations > 0)
        std::cout << "Avg deallocation time: " << (freeTime / deallocations) << " ms\n";
}
