add_executable(normal_shock ${CMAKE_CURRENT_SOURCE_DIR}/normal_shock.cpp)
target_link_libraries(normal_shock PRIVATE Maxwell)