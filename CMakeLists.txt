cmake_minimum_required(VERSION 3.27)

set(MAXWELL_VERSION 1.0.0)
project(Maxwell LANGUAGES CXX VERSION ${MAXWELL_VERSION})

set(MAXWELL_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set(MAXWELL_MINOR_VERSION ${PROJECT_VERSION_MINOR})
set(MAXWELL_PATCH_VERSION ${PROJECT_VERSION_PATCH})

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" ON)
option(DISABLE_PREDEFINED_UNITS "Disable predefined units" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(include)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()