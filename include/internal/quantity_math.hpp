/**
 * @file quantity_math.hpp
 * @author Alex Schiffer
 * @brief Strongly typed units aware math functions
 * @version 1.0
 * @date 2025-04-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef QUANTITY_MATH_HPP
#define QUANTITY_MATH_HPP

#include <cmath>
#include <concepts>

#include "config.hpp"
#include "quantity.hpp"
#include "quantity_repo.hpp"
#include "unit.hpp"
#include "unit_repo.hpp"

namespace maxwell {
namespace _detail {
template <typename T>
concept cmath_compatible_type = std::floating_point<T> || std::integral<T> || std::unsigned_integral<T>;
}

/// \brief Converts the absolute value of a quantity
///
/// _Constraints_: The expression `std::abs(std::declval<T>())` is well formed. <br>
/// _Effects_: Returns a new \c quantity object whose magitude is equal to absolute value of the magnitude of \c q.
///
/// \tparam U The units of \c q
/// \tparam T The magnitude of \c q
/// \param[in] q The quantity to find the absolute value of.
/// \return The absolute value of \c q.
template <unit auto U, typename T>
  requires requires(T a) { std::abs(a); }
MAXWELL_MATH_CONSTEXPR23 quantity<U, T> abs(const quantity<U, T>& q) MATH_NOEXCEPT(abs) {
  return quantity<U, T>(std::abs(q.get_magnitude()));
}

/// \brief Converts the floating point remainder of the division of two quantities
///
/// _Constriants_: The expression `std::fmod(std::declval<T>(), std::declval<T>())` is well formed. <br>
/// _Effects_: Returns the floating point remainder of the division of \c x by \c y. <br>
/// _Remarks_: The units of the returned quantity are `U1 / U2`.
///
/// \tparam U1 The units of \c x.
/// \tparam U2 The units of \c y.
/// \tparam T The type of the magnitude of \c x and \c y.
/// \param[in] x The left hand side of the division.
/// \param[in] y The right hand side of the division.
/// \return The floating point remainder of the division of \c x by \c y.
template <unit auto U1, unit auto U2, typename T>
  requires requires(T a) { std::fmod(a, a); }
MAXWELL_MATH_CONSTEXPR23 quantity<U1 / U2, T>
fmod(const quantity<U1, T>& x,
     const quantity<U2, T>& y) noexcept(noexcept(std::fmod(x.get_magnitude(), y.get_magnitude()))) {
  return quantity<U1 / U2, T>(std::fmod(x.get_magnitude(), y.get_magnitude()));
}

/// \brief Converts the signed remainder of the division of two quantities
///
/// _Constriants_: The expression `std::fmod(std::declval<T>(), std::declval<T>())` is well formed. <br>
/// _Effects_: Returns the signed remainder of the division of `x / y` <br>
/// _Remarks_: The units of the returned quantity are `U1 / U2`.
///
/// \tparam U1 The units of \c x.
/// \tparam U2 The units of \c y.
/// \tparam T The type of the magnitude of \c x and \c y.
/// \param[in] x The left hand side of the division.
/// \param[in] y The right hand side of the division.
/// \return The signed remainder of the division of \c x by \c y.
template <unit auto U1, unit auto U2, typename T>
  requires requires(T a) { std::remainder(a, a); }
MAXWELL_MATH_CONSTEXPR23 quantity<U1 / U2, T>
remainder(const quantity<U1, T>& x,
          const quantity<U2, T>& y) noexcept(noexcept(std::remainder(x.get_magnitude(), y.get_magnitude()))) {
  return quantity<U1 / U2, T>(std::remainder(x.get_magnitude(), y.get_magnitude()));
}

/// \brief Converts the remainer and last 3 bits of the division of two quantities.
///
/// _Constraints_: The expression `std::remquo(std::declval<T>(), std::declval<T>(), std::declval<int*>())` is well
/// formed. <br>
/// _Effects_: Returns the remainder of `x / y` and stores the sign and last three bits of `x / y` in \c
/// quo. <br>
/// _Remarks_: The units of the returned quantity are `U1 / U2`.
///
/// \tparam U1 The units of \c x.
/// \tparam U2 The units of \c y.
/// \tparam T The type of the magnitude of \c x and \c y.
/// \param[in] x The left hand side of the division.
/// \param[in] y The right hand side of the division.
/// \param[out] quo Pointer to \c int to store the sign and last three bits of `x / y`.
/// \return The signed remainder of the division of \c x by \c y.
template <unit auto U1, unit auto U2, typename T>
  requires requires(T a, int b) { std::remquo(a, a, &b); }
MAXWELL_MATH_CONSTEXPR23 quantity<U1 / U2, T>
remquo(const quantity<U1, T>& x, const quantity<U2, T>& y,
       int* quo) noexcept(noexcept(std::remquo(x.get_magnitude(), y.get_magnitude(), std::declval<int*>()))) {
  T mag = std::remquo(x.get_magnitude(), y.get_magnitude(), quo);
  return quantity<U1 / U2, T>(mag);
}

template <typename T>
  requires requires(T a) { std::exp(a); }
MAXWELL_MATH_CONSTEXPR26 T exp(quantity<scalar_unit, T> x) MATH_NOEXCEPT(exp) {
  return std::exp(x.get_magnitude());
}

template <typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 T exp2(quantity<scalar_unit, T> x) MATH_NOEXCEPT(exp2) {
  return std::exp2(x.get_magnitude());
}

template <typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 T expm1(quantity<scalar_unit, T> x) MATH_NOEXCEPT(expm1) {
  return std::expm1(x.get_magnitude());
}

template <typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 T log(quantity<scalar_unit, T> x) MATH_NOEXCEPT(log) {
  return std::log(x.get_magnitude());
}

template <typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 T log10(quantity<scalar_unit, T> x) MATH_NOEXCEPT(log10) {
  return std::log10(x.get_magnitude());
}

template <typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 T log1p(quantity<scalar_unit, T> x) MATH_NOEXCEPT(log1p) {
  return std::log1p(x.get_magnitude());
}

template <typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 T log2(quantity<scalar_unit, T> x) MATH_NOEXCEPT(log2) {
  return std::log2(x.get_magnitude());
}

template <int Power, unit auto U, typename T> MAXWELL_MATH_CONSTEXPR26 auto pow(quantity<U, T> x) MATH_NOEXCEPT(pow) {
  if constexpr (Power == 0) {
    return quantity<scalar_unit, T>(1);
  } else if constexpr (Power == 1) {
    return x;
  } else if constexpr (Power < 0) {
    return quantity<scalar_unit, T>(1) / pow<-Power>(x);
  } else {
    return x * pow<Power - 1>(x);
  }
}

template <unit auto U, typename T>
MAXWELL_MATH_CONSTEXPR26 quantity<unit_sqrt_type<decltype(U)>{}, T> sqrt(const quantity<U, T>& q) MATH_NOEXCEPT(sqrt) {
  return quantity<unit_sqrt_type<decltype(U)>{}, T>(std::sqrt(q.get_magnitude()));
}

template <auto U, typename T>
  requires angle_unit<U> && _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 double sin(const quantity<U, T>& q) MATH_NOEXCEPT(sin) {
  return std::sin(radian{q}.get_magnitude());
}

template <auto U, typename T>
  requires angle_unit<U> && _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 double cos(const quantity<U, T>& q) MATH_NOEXCEPT(cos) {
  return std::cos(radian{q}.get_magnitude());
}

template <auto U, typename T>
  requires angle_unit<U> && _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 double tan(const quantity<U, T>& q) MATH_NOEXCEPT(tan) {
  return std::tan(radian{q}.get_magnitude());
}

template <auto U, typename T>
  requires angle_unit<U> && _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 double csc(const quantity<U, T>& q) MATH_NOEXCEPT(sin) {
  return 1.0 / sin(q);
}

template <auto U, typename T>
  requires angle_unit<U> && _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 double sec(const quantity<U, T>& q) MATH_NOEXCEPT(cos) {
  return 1.0 / cos(q);
}

template <auto U, typename T>
  requires angle_unit<U> && _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR26 double cot(const quantity<U, T>& q) MATH_NOEXCEPT(tan) {
  return 1.0 / tan(q);
}
} // namespace maxwell

#endif