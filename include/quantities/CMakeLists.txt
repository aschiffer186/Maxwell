add_library(Maxwell-Quantity INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/dimension.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity_holder.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/quantity_value.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/unit.hpp 
)
target_include_directories(Maxwell-Quantity INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Maxwell-Quantity INTERFACE Maxwell-Utility)