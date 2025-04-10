#include <eigen3/Eigen/Dense>

#include <Maxwell.hpp>
#include <iostream>

using namespace maxwell;

Eigen::Matrix3d rotate_x(angle auto theta) {
  //   return Eigen::Matrix3d{1.0, 0.0, 0.0, 0.0, cos(theta), sin(theta), 0, sin(theta), cos(theta)};
  return Eigen::Matrix3d{{1.0, 0.0, 0.0}, {cos(theta), -sin(theta), 0.0}, {0.0, sin(theta), cos(theta)}};
}

Eigen::Matrix3d rotate_y(angle auto theta) {
  return Eigen::Matrix3d{{cos(theta), 0.0, -sin(theta)}, {0.0, 1.0, 0.0}, {sin(theta), 0.0, cos(theta)}};
}

Eigen::Matrix3d rotate_z(angle auto theta) {
  return Eigen::Matrix3d{{cos(theta), sin(theta), 0.0}, {-sin(theta), cos(theta), 0.0}, {0.0, 0.0, 1.0}};
}

template <unit auto U>
quantity<U, Eigen::Vector3d> rotate(const quantity<U, Eigen::Vector3d>& value, angle auto roll, angle auto pitch,
                                    angle auto yaw) {
  return rotate_z(yaw) * rotate_y(pitch) * rotate_x(roll) * value;
}

int main() {
  quantity<meter_unit, Eigen::VectorXd> v{Eigen::Vector3d{1.0, 2.0, 3.0}};
  //   auto rotated = rotate(v, radian{0.5}, radian{0.5}, radian{0.5});

  Eigen::Matrix3d mat;
  Eigen::Vector3d vec;
  //   Eigen::Vector3d vec2 = mat * vec;
  //   std::cout << "Rotated vector: " << rotated.get_magnitude().transpose() << std::endl;
  return 0;
}