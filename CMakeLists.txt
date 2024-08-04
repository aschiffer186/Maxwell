cmake_minimum_required(VERSION 3.25)
project(Maxwell)

set(CMAKE_CXX_SCAN_FOR_MODULES "OFF")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

add_subdirectory(Maxwell)

option(NO_PREDEFIENED_DERIVED_UNITS "Don't instantiate derived units" OFF)
option(EXPLICITLY_INSTANTIATE_QUANTITIES "Explicilty instantiate all quantities" OFF)

enable_testing()
add_subdirectory(Tests)