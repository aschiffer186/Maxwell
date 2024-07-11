add_library(Maxwell INTERFACE
    Maxwell.hpp
    Dimension.hpp
    Unit.hpp)
target_include_directories(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})