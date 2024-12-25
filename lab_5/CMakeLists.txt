cmake_minimum_required(VERSION 3.28)
project(lab_5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module
include(FetchContent)

# Загрузка и сборка ZeroMQ
FetchContent_Declare(
        zeromq
        GIT_REPOSITORY https://github.com/zeromq/libzmq.git
        GIT_TAG        v4.3.5  # Укажите нужную версию
)

FetchContent_MakeAvailable(zeromq)

# Загрузка и сборка cppzmq
FetchContent_Declare(
        cppzmq
        GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
        GIT_TAG        v4.8.0  # Укажите нужную версию
)

FetchContent_MakeAvailable(cppzmq)




add_library(${PROJECT_NAME}_lib src/tree/Tree.h src/tree/Tree.cpp)
add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}_child src/child.cpp)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_child)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib cppzmq)
target_link_libraries(${PROJECT_NAME}_child ${PROJECT_NAME}_lib cppzmq)


add_executable(${PROJECT_NAME}_tests tests/tests.cpp)

target_include_directories(${PROJECT_NAME}_tests PUBLIC "src")
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND tests)