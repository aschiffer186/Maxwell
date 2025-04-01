cmake_minimum_required(VERSION 3.27)
project(Maxwell LANGUAGES CXX)

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)

add_subdirectory(include)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()