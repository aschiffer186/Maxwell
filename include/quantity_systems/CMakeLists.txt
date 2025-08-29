add_library(Maxwell-Systems INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/isq.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/si.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/si_constants.hpp

)
target_include_directories(Maxwell-Systems INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Maxwell-Systems INTERFACE Maxwell-Utility Maxwell-Base)