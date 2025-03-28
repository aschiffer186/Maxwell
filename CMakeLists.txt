cmake_minimum_required(VERSION 3.28)
project(Maxwell LANGUAGES CXX)

add_subdirectory(include)

enable_testing()
add_subdirectory(tests)