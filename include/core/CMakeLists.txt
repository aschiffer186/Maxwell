add_library(Maxwell-Base INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/dimension.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity_holder.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity_value.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unit.hpp 
)
target_include_directories(Maxwell-Base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Maxwell-Base INTERFACE Maxwell-Utility Maxwell-Formatting)