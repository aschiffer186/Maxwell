cmake_minimum_required(VERSION 3.27)

set(MAXWELL_MAJOR_VERSION 1)
set(MAXWELL_MINOR_VERSION 0)
set(MAXWELL_PATCH_VERSION 0)
set(MAXWELL_VERSION "${MAXWELL_MAJOR_VERSION}.${MAXWELL_MINOR_VERSION}.${MAXWELL_PATCH_VERSION}")
project(Maxwell LANGUAGES CXX VERSION ${MAXWELL_VERSION})


option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" ON)
option(DISABLE_PREDEFINED_UNITS "Disable predefined units" OFF)

add_subdirectory(include)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()