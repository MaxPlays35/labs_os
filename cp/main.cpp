//
// Created by MaxPlays on 22/12/2024.
//

#include <iostream>
#include <memory>

#include "src/memoryResource/MemoryResource.h"

int main() {
    std::shared_ptr<CustomMemoryResource> resource = std::make_shared<CustomMemoryResource>();

    std::pmr::vector<int> vector(resource.get());

    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(3);
    vector.push_back(4);

    std::cout << "Allocated: " << resource->get_allocated_memory() << std::endl;
    std::cout << "Used: " << resource->get_used_memory() << std::endl;

    return 0;
}
