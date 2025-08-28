cmake_minimum_required(VERSION 3.25)

set(MAXWELL_VERSION_MAJOR 0)
set(MAXWELL_VERSION_MINOR 1)
set(MAXWELL_VERSION_PATCH 0)

set(MAXWELL_VERSION "${MAXWELL_VERSION_MAJOR}.${MAXWELL_VERSION_MINOR}.${MAXWELL_VERSION_PATCH}")

project(Maxwell LANGUAGES CXX VERSION ${MAXWELL_VERSION})

option(BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(include) 

add_subdirectory(examples)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()