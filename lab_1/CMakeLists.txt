cmake_minimum_required(VERSION 3.28)
project(lab_1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME}_lib src/parent.cpp src/parent.h src/utils.cpp src/utils.h)
add_executable(${PROJECT_NAME} main.cpp)
add_executable(child src/child.cpp)

add_dependencies(${PROJECT_NAME} child)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

add_executable(${PROJECT_NAME}_tests tests/tests.cpp)
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND tests)