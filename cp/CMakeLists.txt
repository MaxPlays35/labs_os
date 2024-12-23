cmake_minimum_required(VERSION 3.28)
project(cp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME}_lib
        src/memoryResource/MemoryResource.h
        src/memoryResource/MemoryResource.cpp
        src/block/Block.h
        src/block/Block.cpp)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME}_tests tests/tests.cpp)

target_include_directories(${PROJECT_NAME}_tests PUBLIC "src")
#target_include_directories(${PROJECT_NAME} PUBLIC "src")
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND tests)