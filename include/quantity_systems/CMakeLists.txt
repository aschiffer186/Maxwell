add_library(Maxwell-Systems INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/isq.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/si.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/si_constants.hpp

)
target_include_directories(Maxwell-Systems
INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_DIR}>
)
target_link_libraries(Maxwell-Systems INTERFACE Maxwell-Utility Maxwell-Base)