add_library(Maxwell-Systems INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/isq.hpp
)
target_include_directories(Maxwell-Systems INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Maxwell-Systems INTERFACE Maxwell-Utility Maxwell-Base)