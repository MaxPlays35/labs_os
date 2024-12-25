//
// Created by MaxPlays on 22/12/2024.
//
#include "freeMemoryResource/FreeMemoryResource.h"
#include "gtest/gtest.h"
#include "macMemoryResource/MacMemoryResource.h"
#include "testClass/TestClass.h"

TEST(FreeAllocatorTests, ShouldAllocateAndDeallocateCorrectly) {
    auto freeMemoryResource = std::shared_ptr<BaseMemoryResource>(new FreeMemoryResource());
    auto classAllocator = std::pmr::polymorphic_allocator<TestClass>(freeMemoryResource.get());

    std::vector<TestClass*> array;

    for (std::size_t i = 0; i < 10; ++i) {
        array.push_back(new (classAllocator.allocate(1)) TestClass());
    }

    EXPECT_EQ(freeMemoryResource->get_used_memory(), 80 * 10);

    classAllocator.deallocate(array.back(), 1);
    array.pop_back();

    classAllocator.deallocate(array.back(), 1);
    array.pop_back();

    EXPECT_EQ(freeMemoryResource->get_used_memory(), 80 * 8);

    for (auto & ptr : array) {
        classAllocator.deallocate(ptr, 1);
    }

    EXPECT_EQ(freeMemoryResource->get_used_memory(), 0);
}

TEST(MacAllocatorTests, ShouldAllocateAndDeallocateCorrectly) {
    auto macMemoryResource = std::shared_ptr<BaseMemoryResource>(new MacMemoryResource());
    auto classAllocator = std::pmr::polymorphic_allocator<TestClass>(macMemoryResource.get());

    std::vector<TestClass*> array;

    for (std::size_t i = 0; i < 10; ++i) {
        array.push_back(new (classAllocator.allocate(1)) TestClass());
    }

    EXPECT_EQ(macMemoryResource->get_used_memory(), 128 * 10);

    classAllocator.deallocate(array.back(), 1);
    array.pop_back();

    classAllocator.deallocate(array.back(), 1);
    array.pop_back();

    EXPECT_EQ(macMemoryResource->get_used_memory(), 128 * 8);

    for (auto & ptr : array) {
        classAllocator.deallocate(ptr, 1);
    }

    EXPECT_EQ(macMemoryResource->get_used_memory(), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}