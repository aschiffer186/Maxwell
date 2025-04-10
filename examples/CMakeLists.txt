add_executable(pendulum pendulum.cpp)
target_link_libraries(pendulum PRIVATE Maxwell)

find_package(Eigen3)
add_executable(eigen_example eigen_example.cpp)
target_link_libraries(eigen_example PRIVATE Maxwell Eigen3::Eigen)

add_executable(rotation_matrix rotation_matrix.cpp)
target_link_libraries(rotation_matrix PRIVATE Maxwell Eigen3::Eigen)