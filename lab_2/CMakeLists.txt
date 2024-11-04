cmake_minimum_required(VERSION 3.28)
project(lab_2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME}_lib
        src/mergesort/mergesort.cpp src/mergesort/mergesort.h
        src/threadwork/threadwork.cpp src/threadwork/threadwork.h
        src/mergesortthreaded/mergesortthreaded.cpp src/mergesortthreaded/mergesortthreaded.h)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "src")

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME}_tests tests/tests.cpp)

target_include_directories(${PROJECT_NAME}_tests PUBLIC "src")
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND tests)