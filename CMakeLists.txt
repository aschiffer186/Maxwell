cmake_minimum_required(VERSION 3.28)
project(Maxwell VERSION 1.0.0 LANGUAGES CXX)

option(DISABLE_PREDEFINED_UNITS "Disables all library-defined units except for SI base units" OFF)
option(EXPLICITLY_INSTANTIATE_UNITS "Create explicit instantiations for derived units" OFF)
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

if (DISABLE_PREDEFINED_UNITS)
    add_compile_definitions(-DNO_PREDEFINED_DERIVED_UNITS)
endif()

if (EXPLICITLY_INSTANTIATE_UNITS)
    add_compile_definitions(-DEXPLICITLY_INSTANTIATE_UNITS)
endif()

add_subdirectory(include)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(unit-tests)
endif()

if (BUILD_EXAMPLES)
add_subdirectory(examples)
endif()