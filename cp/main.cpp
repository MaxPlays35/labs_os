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
      return benchmark_allocator<TestClass>(iterations, freeAllocator);
    });

    auto [macTime, macUsedMemory] = measure_time([&macAllocator, iterations](){
      return benchmark_allocator<TestClass>(iterations, macAllocator);
    });

    file << iterations << " ";
    file << freeTime << " " << freeAllocator->get_allocated_memory() << " " << freeUsedMemory << " ";
    file << macTime << " " << macAllocator->get_allocated_memory() << " " << macUsedMemory;
    file << std::endl;
}

int main(int argv, char** argc){
    {
        std::ofstream file("data.txt", std::ios_base::app);
        file << "allocations timeSimple allocatedMemorySimple usedMemSimple timeTwin allocatedMemoryTwin usedMemTwin" << std::endl;
    }
    for (auto i = 10; i < 5000; i+=10){
        benchmark(i);
    }
}