add_library(Maxwell-Formatting INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/formatting.hpp
)
target_link_libraries(Maxwell-Formatting INTERFACE Maxwell-Base Maxwell-Utility)
target_include_directories(Maxwell-Formatting
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)