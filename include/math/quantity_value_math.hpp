/// \file quantity_value_math.hpp
/// \brief Provides mathematical functions for quantity values

#ifndef QUANTITY_VALUE_MATH_HPP
#define QUANTITY_VALUE_MATH_HPP

#include <cmath>

#include "core/quantity.hpp"
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
MAXWELL_BASIC_CMATH_CONSTEXPR auto abs(const quantity_value<U, Q, T>& x)
    -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::abs(x.get_value()));
}

/// \brief Computes the absolute value of a \c quantity_holder
///
/// Computes the absolute value of a \c quantity_holder. Equivalent to
/// <tt>quantity_value<U, Q, T>(std::abs(x.get_value()))</tt>. The behavior is
/// undefined if the result cannot be represented by the type \c T.
///
/// \tparam Q The quantity type of the \c quantity_holder
/// \tparam T The type of the numerical value of the \c quantity_holder
/// \param x The \c quantity_holder to compute the absolute value of
/// \return The absolute value of \c x
MODULE_EXPORT template <auto Q, typename T>
MAXWELL_BASIC_CMATH_CONSTEXPR auto abs(const quantity_holder<Q, T>& x)
    -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(std::abs(x.get_value()), x.get_multiplier(),
                               x.get_reference());
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
  const double r = si::radian<>{x}.get_value();
  return std::sin(r);
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

/// \brief Computes the secant of an angle quantity.
///
/// Computes the secant of an angle quantity. If the argument is +/- infinity,
/// NaN is returned and \c FE_INVALID is raised. If the argument is NaN, NaN
/// is returned. If a domain error occurs, an implementation-defined value is
/// returned.
///
/// \param x The angle quantity to compute the secant of.
/// \return The secant of \c x.
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
sec(quantity_of<isq::plane_angle> auto x) -> double {
  return 1.0 / std::cos(si::radian<>{x}.get_value());
}

/// \brief Computes the cosecant of an angle quantity.
///
/// Computes the cosecant of an angle quantity. If the argument is +/-
/// infinity, NaN is returned and \c FE_INVALID is raised. If the argument is
/// NaN, NaN is returned. If a domain error occurs, an implementation-defined
/// value is returned.
///
/// \param x The angle quantity to compute the cosecant of.
/// \return The cosecant of \c x.
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
csc(quantity_of<isq::plane_angle> auto x) -> double {
  return 1.0 / std::sin(si::radian<>{x}.get_value());
}

/// \brief Computes the cotangent of an angle quantity.
///
/// Computes the cotangent of an angle quantity. If the argument is +/-
/// infinity, NaN is returned and \c FE_INVALID is raised. If the argument is
/// NaN, NaN is returned. If a domain error occurs, an implementation-defined
/// value is returned.
///
/// \param x The angle quantity to compute the cotangent of.
/// \return The cotangent of \c x.
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
cot(quantity_of<isq::plane_angle> auto x) -> double {
  return 1.0 / std::tan(si::radian<>{x}.get_value());
}

/// \brief Computes the arcsine of a value
///
/// Computes the arcsin of a value in the domain [-1, 1].
///
/// \param x The value to compute the arcsine of.
/// \return The arcsine of \c x in radians
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto asin(double x)
    -> si::radian<> {
  return si::radian<>{std::asin(x)};
}

/// \brief Computes the arcsine of a value
///
/// Computes the arcsin of a value in the domain [-1, 1].
///
/// \param x The value to compute the arcsine of.
/// \return The arcsine of \c x in degrees
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto asind(double x)
    -> si::degree<> {
  return si::degree<>(asin(x));
}

/// \brief Computes the arccosine of a value
///
/// Computes the arccosine of a value in the domain [-1, 1].
///
/// \param x The value to compute the arccosine of.
/// \return The arccosine of \c x in radians
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto acos(double x)
    -> si::radian<> {
  return si::radian<>{std::acos(x)};
}

/// \brief Computes the arccosine of a value
///
/// Computes the arccosine of a value in the domain [-1, 1].
///
/// \param x The value to compute the arccosine of.
/// \return The arccosine of \c x in degrees
MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto acosd(double x)
    -> si::degree<> {
  return si::degree<>(acos(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto atan(double x)
    -> si::radian<> {
  return si::radian<>{std::atan(x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto atand(double x)
    -> si::degree<> {
  return si::degree<>(atan(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto atan2(double y,
                                                                 double x)
    -> si::radian<> {
  return si::radian<>{std::atan2(y, x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto atan2d(double y,
                                                                  double x)
    -> si::degree<> {
  return si::degree<>(atan2(y, x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto acsc(double x)
    -> si::radian<> {
  return si::radian<>{std::asin(1.0 / x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto acscd(double x)
    -> si::degree<> {
  return si::degree<>(acsc(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto asec(double x)
    -> si::radian<> {
  return si::radian<>{std::acos(1.0 / x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto asecd(double x)
    -> si::degree<> {
  return si::degree<>(asec(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto acot(double x)
    -> si::radian<> {
  return si::radian<>{std::atan(1.0 / x)};
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR inline auto acotd(double x)
    -> si::degree<> {
  return si::degree<>(acot(x));
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
exp(quantity_of<isq::dimensionless> auto x) -> double {
  return std::exp(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
exp2(quantity_of<number> auto x) -> double {
  return std::exp2(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
expm1(quantity_of<number> auto x) {
  return std::expm1(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
log(quantity_of<number> auto x) -> double {
  return std::log(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
log2(quantity_of<number> auto x) -> double {
  return std::log2(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
log10(quantity_of<number> auto x) -> double {
  return std::log10(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
log1p(quantity_of<number> auto x) -> double {
  return std::log1p(x.get_value());
}

/// \brief Computes the power of a quantity value to a rational exponent
///
/// Raises a quantity value to a rational exponent. The resulting quantity value
/// has units that are the units of the original quantity value raised to the
/// power, a quantity type that is the quantity type of the original quantity
/// value raised to the power, and the same numerical type.
///
/// \tparam R The rational exponent to raise the quantity value to.
/// \tparam U The units of the quantity value.
/// \tparam Q The quantity type of the quantity value.
/// \tparam T The type of the numerical value of the quantity value.
/// \param x The quantity value to raise to the power.
/// \return The quantity value raised to the power.
MODULE_EXPORT template <auto R, auto U, auto Q, typename T>
  requires utility::rational<decltype(R)>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto pow(quantity_value<U, Q, T> x)
    -> quantity_value<pow<R>(U), pow<R>(Q), T> {
  return quantity_value<pow<R>(U), pow<R>(Q), T>(
      std::pow(x.get_value(), static_cast<double>(R)));
}

/// \brief Computes the power of a quantity value to an integer exponent
///
/// Raises a quantity value to an integer exponent. The resulting quantity value
/// has units that are the units of the original quantity value raised to the
/// power, a quantity type that is the quantity type of the original quantity
/// value raised to the power, and the same numerical type.
///
/// \tparam P The integer exponent to raise the quantity value to.
/// \tparam U The units of the quantity value.
/// \tparam Q The quantity type of the quantity value.
/// \tparam T The type of the numerical value of the quantity value.
/// \param x The quantity value to raise to the power.
/// \return The quantity value raised to the power.
MODULE_EXPORT
template <std::intmax_t P, auto U, auto Q, typename T>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto pow(quantity_value<U, Q, T> x)
    -> quantity_value<pow<P>(U), pow<P>(Q), T> {
  return quantity_value<pow<P>(U), pow<P>(Q), T>(std::pow(x.get_value(), P));
}

MODULE_EXPORT template <auto R, auto Q, typename T>
  requires utility::rational<decltype(R)>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto pow(quantity_holder<Q, T> x)
    -> quantity_holder<pow<R>(Q), T> {
  return quantity_holder<pow<R>(Q), T>(
      std::pow(x.get_value(), static_cast<double>(R)),
      std::pow(x.get_multiplier(), static_cast<double>(R)), x.get_reference());
}

MODULE_EXPORT template <std::intmax_t P, auto Q, typename T>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto pow(quantity_holder<Q, T> x)
    -> quantity_holder<pow<P>(Q), T> {
  return quantity_holder<pow<P>(Q), T>(std::pow(x.get_value(), P),
                                       std::pow(x.get_multiplier(), P),
                                       x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto sqrt(quantity_value<U, Q, T> x)
    -> quantity_value<sqrt(U), sqrt(Q), T> {
  return quantity_value<sqrt(U), sqrt(Q), T>(std::sqrt(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto sqrt(quantity_holder<Q, T> x)
    -> quantity_holder<sqrt(Q), T> {
  return quantity_holder<sqrt(Q), T>(std::sqrt(x.get_value()),
                                     std::sqrt(x.get_multiplier()),
                                     x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto cbrt(quantity_value<U, Q, T> x)
    -> quantity_value<pow<rational<1, 3>>(U), pow<rational<1, 3>>(Q), T> {
  return quantity_value<pow<rational<1, 3>>(U), pow<rational<1, 3>>(Q), T>(
      std::cbrt(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
MAXWELL_EXTENDED_CMATH_CONSTEXPR auto cbrt(quantity_holder<Q, T> x)
    -> quantity_holder<pow<rational<1, 3>>(Q), T> {
  return quantity_holder<pow<rational<1, 3>>(Q), T>(
      std::cbrt(x.get_value()), std::cbrt(x.get_multiplier()),
      x.get_reference());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
erf(quantity_of<number> auto x) -> double {
  return std::erf(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
erfc(quantity_of<number> auto x) -> double {
  return std::erfc(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
tgamma(quantity_of<number> auto x) -> double {
  return std::tgamma(x.get_value());
}

MODULE_EXPORT MAXWELL_EXTENDED_CMATH_CONSTEXPR auto
lgamma(quantity_of<number> auto x) -> double {
  return std::lgamma(x.get_value());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto ceil(quantity_value<U, Q, T> x)
    -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::ceil(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto ceil(quantity_holder<Q, T> x)
    -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(std::ceil(x.get_value()), x.get_multiplier(),
                               x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto floor(quantity_value<U, Q, T> x)
    -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::floor(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto floor(quantity_holder<Q, T> x)
    -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(std::floor(x.get_value()), x.get_multiplier(),
                               x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto trunc(quantity_value<U, Q, T> x)
    -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::trunc(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto trunc(quantity_holder<Q, T> x)
    -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(std::trunc(x.get_value()), x.get_multiplier(),
                               x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto round(quantity_value<U, Q, T> x)
    -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::round(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto round(quantity_holder<Q, T> x)
    -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(std::round(x.get_value()), x.get_multiplier(),
                               x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto lround(quantity_value<U, Q, T> x)
    -> quantity_value<U, Q, long> {
  return quantity_value<U, Q, long>(std::round(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto lround(quantity_holder<Q, T> x)
    -> quantity_holder<Q, long> {
  return quantity_holder<Q, long>(std::round(x.get_value()), x.get_multiplier(),
                                  x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto llround(quantity_value<U, Q, T> x)
    -> quantity_value<U, Q, long long> {
  return quantity_value<U, Q, long long>(std::round(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
MAXWELL_BASIC_CMATH_CONSTEXPR auto llround(quantity_holder<Q, T> x)
    -> quantity_holder<Q, long long> {
  return quantity_holder<Q, long long>(std::round(x.get_value()),
                                       x.get_multiplier(), x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
auto nearbyint(quantity_value<U, Q, T> x) -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::nearbyint(x.get_value()));
}

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
auto nearbyint(quantity_holder<Q, T> x) -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(std::nearbyint(x.get_value()),
                               x.get_multiplier(), x.get_reference());
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
auto rint(quantity_value<U, Q, T> x) -> quantity_value<U, Q, T> {
  return quantity_value<U, Q, T>(std::rint(x.get_value()));
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
auto lrint(quantity_value<U, Q, T> x) -> quantity_value<U, Q, long> {
  return quantity_value<U, Q, long>(std::rint(x.get_value()));
}

MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
auto llrint(quantity_value<U, Q, T> x) -> quantity_value<U, Q, long long> {
  return quantity_value<U, Q, long long>(std::rint(x.get_value()));
}

} // namespace maxwell::math

#endif