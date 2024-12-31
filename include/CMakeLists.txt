add_library(Maxwell INTERFACE Maxwell.hpp)
target_include_directories(Maxwell INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell)