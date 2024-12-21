cmake_minimum_required(VERSION 3.28)
project(lab_5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module
include(FetchContent)

# Fetch and add cpp-modern-kafka
FetchContent_Declare(
        cpp-kafka
        GIT_REPOSITORY https://github.com/mfontanini/cppkafka.git
        GIT_TAG        v0.4.1  # You can specify a specific tag or commit
)


# If cpp-modern-kafka has dependencies like librdkafka, fetch them as well
FetchContent_Declare(
        RdKafka
        GIT_REPOSITORY https://github.com/confluentinc/librdkafka.git
        GIT_TAG        v2.6.1  # Specify the desired version
)

FetchContent_MakeAvailable(RdKafka)
set(CPPKAFKA_BUILD_SHARED 0)
set(RDKAFKA_BUILD_STATIC ON CACHE BOOL "")
message(STATUS "${RdKafka_BINARY_DIR}")
find_package(RdKafka ${FIND_PACKAGE_QUIET} CONFIG)
set(RdKafka_DIR "C:\\Users\\MaxPlays\\CLionProjects\\labs_os2\\cmake-build-debug\\_deps\\rdkafka-build\\generated\\RdKafkaConfig.cmake")
FetchContent_MakeAvailable(cpp-kafka)



add_library(${PROJECT_NAME}_lib src/tree/Tree.h src/tree/Tree.cpp)
add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}_child src/child.cpp)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_child)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)
target_link_libraries(${PROJECT_NAME}_child ${PROJECT_NAME}_lib)


add_executable(${PROJECT_NAME}_tests tests/tests.cpp)

target_include_directories(${PROJECT_NAME}_tests PUBLIC "src")
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib gtest_main)

add_test(NAME ${PROJECT_NAME}_tests COMMAND tests)