if(BUILD_EIGEN_INTERFACE)
    find_package(Eigen3 3.3 REQUIRED NO_MODULE)
    add_compile_definitions("ENABLE_EIGEN_INTERFACE")
endif()

add_library(Maxwell INTERFACE Maxwell.hpp)
target_include_directories(Maxwell INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Maxwell)
# TODO: Make Eigen usable with Maxwell
if (BUILD_EIGEN_INTERFACE)
    target_link_libraries(Maxwell INTERFACE Eigen3::Eigen)
endif()