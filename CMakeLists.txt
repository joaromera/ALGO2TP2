cmake_minimum_required(VERSION 3.5)
project(tp2)

set(CMAKE_CXX_STANDARD 17)

# Algunos flags para pasar al compilador (gnu++11 en vez de c++11 para que sea cross-plat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ggdb3 -g -Wall -Wextra -pedantic")

# Leemos todos los archivos fuentes en ./src
file(GLOB SOURCE_FILES src/*.cpp src/*.h src/*.hpp)

# Leemos todos los archivos de test en ./tests
file(GLOB TEST_SOURCES tests/*.cpp)

add_subdirectory(tests/google-test)

# Creamos el ejecutable para correr los tests
add_executable(correrTests ${TEST_SOURCES} ${SOURCE_FILES})

# Necesitamos asociar los archivos del framework de testing
target_link_libraries(correrTests gtest gtest_main)

add_test(correrTests correrTests)

