add_library(Maxwell-Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/quantity_value_math.hpp)
target_include_directories(Maxwell-Math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Maxwell-Math INTERFACE Maxwell-Base Maxwell-Systems Maxwell-Utility)