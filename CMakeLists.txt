cmake_minimum_required(VERSION 3.25)
project(Maxwell)

set(CMAKE_CXX_SCAN_FOR_MODULES "OFF")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

add_subdirectory(Maxwell)

enable_testing()
add_subdirectory(Tests)