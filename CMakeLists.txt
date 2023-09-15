cmake_minimum_required(VERSION 3.22)
project(Maxwell VERSION 0.1.0 LANGUAGES CXX)

add_library(Maxwell Maxwell/test.cpp)
target_compile_features(Maxwell PRIVATE cxx_std_20)

add_subdirectory(Tests)
