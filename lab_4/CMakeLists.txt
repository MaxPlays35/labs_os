cmake_minimum_required(VERSION 3.28)
project(lab_4)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME}_first_lib SHARED
        src/libraryFirst/libraryFirst.h
        src/libraryFirst/piFirst.cpp
        src/libraryFirst/translationFirst.cpp)
add_library(${PROJECT_NAME}_second_lib SHARED
        src/librarySecond/librarySecond.h
        src/librarySecond/piSecond.cpp
        src/librarySecond/translationSecond.cpp)

add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}_static src/static/static.cpp)
add_executable(${PROJECT_NAME}_dynamic src/dynamic/dynamic.cpp)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_first_lib)
target_link_libraries(${PROJECT_NAME}_static ${PROJECT_NAME}_first_lib)

add_executable(${PROJECT_NAME}_tests tests/tests.cpp)

target_include_directories(${PROJECT_NAME}_tests PUBLIC "src")
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_first_lib gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND tests)