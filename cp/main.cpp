//
// Created by MaxPlays on 22/12/2024.
//

#include <fstream>
#include <iostream>
#include <memory>

#include "src/bench/Bench.h"
#include "src/macMemoryResource/MacMemoryResource.h"
#include "src/freeMemoryResource/FreeMemoryResource.h"
#include "src/testClass/TestClass.h"
#include "src/utils/Utils.h"

void benchmark(size_t iterations){
    std::cout << "Running benchmark for " << iterations << " iterations" << std::endl;
    auto freeAllocator = std::shared_ptr<BaseMemoryResource>(new FreeMemoryResource());
    auto macAllocator = std::shared_ptr<BaseMemoryResource>(new MacMemoryResource());

    std::ofstream file("data.txt", std::ios_base::app);
    auto [freeTime, freeUsedMemory] = measure_time([&freeAllocator, iterations](){
      return simple_benchmark_allocator<TestClass>(iterations, freeAllocator);
    });

    auto [macTime, macUsedMemory] = measure_time([&macAllocator, iterations](){
      return simple_benchmark_allocator<TestClass>(iterations, macAllocator);
    });

    file << iterations << " ";
    file << freeTime << " " << freeAllocator->get_allocated_memory() << " " << freeUsedMemory << " ";
    file << macTime << " " << macAllocator->get_allocated_memory() << " " << macUsedMemory;
    file << std::endl;
}

int main(int argv, char** argc){
    {
        std::ofstream file("data.txt", std::ios_base::app);
        file << "allocations freeTime freeAllocatedMemory freeUsedMem macTime MacAllocatedMemory macUsedMem" << std::endl;
    }
    auto macAllocator = std::shared_ptr<BaseMemoryResource>(new MacMemoryResource());
    auto freeAllocator = std::shared_ptr<BaseMemoryResource>(new FreeMemoryResource());
    realistic_benchmark_allocator<char>(5000, freeAllocator);
    // realistic_benchmark_allocator<char>(5000, freeAllocator);
    // for (auto i = 10; i < 5000; i+=10){
    //     benchmark(i);
    // }
}