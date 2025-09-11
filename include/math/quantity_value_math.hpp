#ifndef QUANTITY_VALUE_MATH_HPP
#define QUANTITY_VALUE_MATH_HPP

#include <cmath>

#include "core/quantity_value.hpp"
#include "quantity_systems/isq.hpp"
#include "quantity_systems/si.hpp"
#include "utility/config.hpp"

namespace maxwell::math {
MODULE_EXPORT template <auto U, auto Q, typename T>
MAXWELL_BASIC_CMATH_CONSTEXPR auto abs(quantity_value<U, Q, T>& x)
    -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::abs(x.get_value()));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
sin(quantity_of<isq::plane_angle> auto x) -> double {
  return std::sin(si::radian<>{x}.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
cos(quantity_of<isq::plane_angle> auto x) -> double {
  return std::cos(si::radian<>{x}.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
tan(quantity_of<isq::plane_angle> auto x) -> double {
  return std::tan(si::radian<>{x}.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
sec(quantity_of<isq::plane_angle> auto x) -> double {
  return 1.0 / std::cos(si::radian<>{x}.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
csc(quantity_of<isq::plane_angle> auto x) -> double {
  return 1.0 / std::sin(si::radian<>{x}.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
cot(quantity_of<isq::plane_angle> auto x) -> double {
  return 1.0 / std::tan(si::radian<>{x}.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto asin(double x)
    -> si::radian<> {
  return si::radian<>{std::asin(x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto asind(double x)
    -> si::degree<> {
  return si::degree<>(asin(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto acos(double x)
    -> si::radian<> {
  return si::radian<>{std::acos(x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto acosd(double x)
    -> si::degree<> {
  return si::degree<>(acos(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto atan(double x)
    -> si::radian<> {
  return si::radian<>{std::atan(x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto atand(double x)
    -> si::degree<> {
  return si::degree<>(atan(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto atan2(double y, double x)
    -> si::radian<> {
  return si::radian<>{std::atan2(y, x)};
}
} // namespace maxwell::math

#endif