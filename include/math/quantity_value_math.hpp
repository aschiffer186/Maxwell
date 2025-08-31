#ifndef QUANTITY_VALUE_MATH_HPP
#define QUANTITY_VALUE_MATH_HPP

#include <cmath>

#include "core/quantity_value.hpp"
#include "quantity_systems/isq.hpp"
#include "quantity_systems/si.hpp"
#include "utility/config.hpp"

namespace maxwell::math {
template <auto U, auto Q, typename T>
MAXWELL_BASIC_CMATH_CONSTEXPR quantity_value<U, Q, T>
abs(quantity_value<U, Q, T>& x) {
  return quantity_value<U, Q, T>(std::abs(x.get_value()));
}

MAXWELL_EXTENDED_CMATH_CONSTEXPR double
sin(quantity_of<isq::plane_angle> auto x) {
  return std::sin(si::radian<>{x}.get_value());
}

MAXWELL_EXTENDED_CMATH_CONSTEXPR double
cos(quantity_of<isq::plane_angle> auto x) {
  return std::cos(si::radian<>{x}.get_value());
}

MAXWELL_EXTENDED_CMATH_CONSTEXPR double
tan(quantity_of<isq::plane_angle> auto x) {
  return std::tan(si::radian<>{x}.get_value());
}

MAXWELL_EXTENDED_CMATH_CONSTEXPR double
sec(quantity_of<isq::plane_angle> auto x) {
  return 1.0 / std::cos(si::radian<>{x}.get_value());
}

MAXWELL_EXTENDED_CMATH_CONSTEXPR double
csc(quantity_of<isq::plane_angle> auto x) {
  return 1.0 / std::sin(si::radian<>{x}.get_value());
}

MAXWELL_EXTENDED_CMATH_CONSTEXPR double
cot(quantity_of<isq::plane_angle> auto x) {
  return 1.0 / std::tan(si::radian<>{x}.get_value());
}
} // namespace maxwell::math

#endif