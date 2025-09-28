/// \file quantity_value_math.hpp
/// \brief Provides mathematical functions for quantity values

#ifndef QUANTITY_VALUE_MATH_HPP
#define QUANTITY_VALUE_MATH_HPP

#include <cmath>

#include "core/quantity_holder.hpp"
#include "core/quantity_value.hpp"
#include "quantity_systems/isq.hpp"
#include "quantity_systems/si.hpp"
#include "utility/config.hpp"

/// \namespace maxwell::math
/// \brief Mathematical functions for quantity values
namespace maxwell::math {
/// \brief Computes the absolute value of a \c quantity_value
///
/// Computes the absolute value of a \c quantity_value. Equivalent to
/// <tt>quantity_value<U, Q, T>(std::abs(x.get_value()))</tt>. The behavior is
/// undefined if the result cannot be represented by the type \c T.
///
/// \tparam U The units of the \c quantity_value
/// \tparam Q The quantity type of the \c quantity_value
/// \tparam T The type of the numerical value of the \c quantity_value
/// \param x The \c quantity_value to compute the absolute value of
/// \return The absolute value of \c x
MODULE_EXPORT template <auto U, auto Q, typename T>
MAXWELL_BASIC_CMATH_CONSTEXPR auto abs(quantity_value<U, Q, T>& x)
    -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::abs(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
MAXWELL_BASIC_CMATH_CONSTEXPR auto abs(quantity_holder<Q, T>& x)
    -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(std::abs(x.get_value()), x.get_units());
}

/// \brief Computes the sine of an angle quantity.
///
/// Computes the sine of an angle quantity. If the argument is +/- infinity,
/// NaN is returned and \c FE_INVALID is raised. If the argument is NaN, NaN
/// is returned.
///
/// \param x The angle quantity to compute the sine of.
/// \return The sine of \c x in the range [-1, 1].
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
sin(quantity_of<isq::plane_angle> auto x) -> double {
  return std::sin(si::radian<>{x}.get_value());
}

/// \brief Computes the cosine of an angle quantity.
///
/// Computes the cosine of an angle quantity. If the argument is +/- infinity,
/// NaN is returned and \c FE_INVALID is raised. If the argument is NaN, NaN is
/// returned.
///
/// \param x The angle quantity to compute the cosine of.
/// \return The cosine of \c x in the range [-1, 1].
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
cos(quantity_of<isq::plane_angle> auto x) -> double {
  return std::cos(si::radian<>{x}.get_value());
}

/// \brief Computes the tangent of an angle quantity.
///
/// Computes the tangent of an angle quantity. If the argument is +/- infinity,
/// NaN is returned and \c FE_INVALID is raised. If the argument is NaN, NaN is
/// returned. If a domain error occurs, an implementation-defined value is
/// returned.
///
/// \param x The angle quantity to compute the tangent of.
/// \return The tangent of \c x.
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

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto atand2(double y, double x)
    -> si::degree<> {
  return si::degree<>(atan2(y, x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto acsc(double x)
    -> si::radian<> {
  return si::radian<>{std::asin(1.0 / x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto acscd(double x)
    -> si::degree<> {
  return si::degree<>(acsc(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto asec(double x)
    -> si::radian<> {
  return si::radian<>{std::acos(1.0 / x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto asecd(double x)
    -> si::degree<> {
  return si::degree<>(asec(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto acot(double x)
    -> si::radian<> {
  return si::radian<>{std::atan(1.0 / x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto acotd(double x)
    -> si::degree<> {
  return si::degree<>(acot(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
exp(quantity_of<isq::dimensionless> auto x) -> double {
  return std::exp(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
log(quantity_of<isq::dimensionless> auto x) -> double {
  return std::log(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
log10(quantity_of<isq::dimensionless> auto x) -> double {
  return std::log10(x.get_value());
}
} // namespace maxwell::math

#endif