//
// Created by MaxPlays on 24/12/2024.
//
#pragma once
#include <memory>
#include <type_traits>
#include <vector>

#include "../baseMemoryResource/BaseMemoryResource.h"

template<class T>
    requires std::is_default_constructible_v<T>
    size_t benchmark_allocator(size_t iterations, std::shared_ptr<BaseMemoryResource> memory_resource){
    std::pmr::polymorphic_allocator<T> allocator(memory_resource.get());

    std::vector<T*> objects;
    for (auto i = 0; i < iterations; ++i){
        objects.push_back(new (allocator.allocate(1)) T());
    }

    auto usedMem = memory_resource->get_used_memory();

    for (auto obj: objects){
        allocator.deallocate(obj, 1);
    }

    return usedMem;
}
