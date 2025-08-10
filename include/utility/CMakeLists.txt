add_library(Maxwell-Utility INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_time_math.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/template_string.hpp
)
target_include_directories(Maxwell-Utility INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})