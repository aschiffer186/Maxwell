add_subdirectory(Quantity)
add_subdirectory(Units)

add_library(Maxwell INTERFACE Maxwell.hh)
target_include_directories(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Maxwell INTERFACE Maxwell-Quantity Maxwell-Units)