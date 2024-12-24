cmake_minimum_required(VERSION 3.28)
project(cp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME}_lib
        src/simpleMemoryResource/SimpleMemoryResource.h
        src/simpleMemoryResource/SimpleMemoryResource.cpp
        src/macMemoryResource/MacMemoryResource.h
        src/macMemoryResource/MacMemoryResource.cpp
        src/block/Block.h
        src/block/Block.cpp
        src/testClass/TestClass.h
        src/testClass/TestClass.cpp
        src/bench/Bench.h
        src/bench/Bench.cpp
        src/baseMemoryResource/BaseMemoryResource.h
        src/baseMemoryResource/BaseMemoryResource.cpp
        src/utils/Utils.h
        src/utils/Utils.cpp)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME}_tests tests/tests.cpp)

target_include_directories(${PROJECT_NAME}_tests PUBLIC "src")
#target_include_directories(${PROJECT_NAME} PUBLIC "src")
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND tests)