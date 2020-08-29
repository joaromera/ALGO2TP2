cmake_minimum_required(VERSION 3.5)
project(Database)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ggdb3 -g -Wall -Wextra -pedantic")

file(GLOB SOURCE_FILES src/*.cpp src/*.h src/*.hpp)

file(GLOB TEST_SOURCES tests/*.cpp)

add_subdirectory(tests/google-test)

add_executable(runTests ${TEST_SOURCES} ${SOURCE_FILES})

target_link_libraries(runTests gtest gtest_main)

add_test(runTests runTests)
