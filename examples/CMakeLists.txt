add_executable(normal_shock ${CMAKE_CURRENT_SOURCE_DIR}/normal_shock.cpp)
target_link_libraries(normal_shock PRIVATE Maxwell)

add_executable(shapes ${CMAKE_CURRENT_SOURCE_DIR}/shapes.cpp)
target_link_libraries(shapes PRIVATE Maxwell)

add_executable(currency ${CMAKE_CURRENT_SOURCE_DIR}/currency.cpp)
target_link_libraries(currency PRIVATE Maxwell)