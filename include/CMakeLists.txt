add_library(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell.hpp)
target_include_directories(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if (DISABLE_PREDEFINED_UNITS)
    target_compile_definitions(Maxwell INTERFACE -DDISABLE_PREDEFINED_UNITS)
endif()