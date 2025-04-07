#ifndef QUANTITY_MATH_HPP
#define QUANTITY_MATH_HPP

#include <cmath>
#include <concepts>

#include "config.hpp"
#include "internal/unit.hpp"
#include "quantity.hpp"

namespace maxwell {
namespace _detail {
template <typename T>
concept cmath_compatible_type = std::floating_point<T> || std::integral<T> || std::unsigned_integral<T>;
}

template <unit auto U, typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR23 quantity<U, T> abs(const quantity<U, T>& q) MATH_NOEXCEPT(abs) {
  return quantity<U, T>(std::abs(q.get_magnitude()));
}

template <unit auto U, typename T>
  requires _detail::cmath_compatible_type<T>
MAXWELL_MATH_CONSTEXPR23 quantity<U, T> fabs(const quantity<U, T>& q) MATH_NOEXCEPT(fabs) {
  return quantity<U, T>(std::fabs(q.get_magnitude()));
}

template <typename T>
  requires _detail::cmath_compatible_type<T>
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
} // namespace maxwell

#endif