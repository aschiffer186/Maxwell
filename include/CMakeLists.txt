add_subdirectory(core)
add_subdirectory(formatting)
add_subdirectory(quantity_systems)
add_subdirectory(utility)

add_library(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell.hpp) 
target_link_libraries(Maxwell INTERFACE Maxwell-Utility Maxwell-Base Maxwell-Formatting Maxwell-Systems)
target_include_directories(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Maxwell_Core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell_Core.hpp)
target_link_libraries(Maxwell_Core INTERFACE Maxwell-Utility Maxwell-Base Maxwell-Formatting)
target_include_directories(Maxwell INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})