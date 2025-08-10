add_library(Maxwell-Formatting INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/formatting.hpp
)
target_include_directories(Maxwell-Formatting INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Maxwell-Formatting INTERFACE Maxwell-Quantity Maxwell-Utility)