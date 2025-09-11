/// \file compile_time_math.hpp
/// \brief Definition of compile-time rational number and related operations.

#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#include <cassert>     // assert
#include <compare>     // strong_ordering
#include <cstdint>     // intmax_t
#include <numeric>     // gcd
#include <ratio>       // ratio
#include <type_traits> // false_type, remove_cvref_t, true_type

#include "config.hpp"

/// \namespace maxwell::utility
/// \brief Namespace for uility classes.
namespace maxwell::utility {

/// \brief Computes the value of exponentiation by a positive power
///
/// Computes the value of \f$base^{pow}\f$ where \c base and \c pow are both
/// integers, and \c pow is a positive number.
/// This guarantees the return type is also an integer.
///
/// \param base The base of the exponentiation.
/// \param pow The power of the exponentiation.
/// \return \f$base^{pow}\f$
MODULE_EXPORT constexpr auto pos_pow(std::intmax_t base,
                                     std::intmax_t pow) noexcept
    -> std::intmax_t {
  assert(pow >= 0);
  if (pow == 0) {
    return 1;
  } else if (pow % 2 == 0) {
    const std::intmax_t res = pos_pow(base, pow / 2);
    return res * res;
  } else {
    return base * pos_pow(base, pow - 1);
  }
}

/// \brief Computes powers of 10
///
/// Computes \f$10^{pow}\f$ at compile time where \c pow is a positive integer.
///
/// \param pow The exponent to which 10 is raised.
/// \return \f$10^{pow}\f$
MODULE_EXPORT constexpr auto pos_pow_10(std::intmax_t pow) noexcept
    -> std::intmax_t {
  return pos_pow(10, pow);
}

/// \brief Compile-time representation of rational number.
///
/// Class template \c rational_type provides a compile-time representation of a
/// rational number in scientific notation. This allows for expressing larger
/// numbers than can be represented by \c std::intmax_t alone. An instance of \c
/// rational_type is equivalent to \f$\frac{Numerator}{Denominator} \times
/// 10^{Exponent}\f$.
/// A program is ill-formed if \c Denominator is zero.
///
/// \tparam Numerator The numerator of the rational number.
/// \tparam Denominator The denominator of the rational number.
/// \tparam Exponent The exponent of the rational number.
MODULE_EXPORT template <std::intmax_t Numerator, std::intmax_t Denominator>
struct rational_type;

/// \cond
namespace _detail {
template <typename> struct is_rational_type : std::false_type {};

template <std::intmax_t Numerator, std::intmax_t Denominator>
struct is_rational_type<rational_type<Numerator, Denominator>>
    : std::true_type {};

template <typename> struct is_ratio : std::false_type {};

template <std::intmax_t N, std::intmax_t D>
struct is_ratio<std::ratio<N, D>> : std::true_type {};
} // namespace _detail
/// \endcond

/// \brief Concept indicating a type is an instantiation of \c rational_type.
///
/// \tparam T The type to check.
MODULE_EXPORT template <typename T>
concept rational = _detail::is_rational_type<std::remove_cvref_t<T>>::value;

/// \brief Concept indicating a type is an instantiation of \c std::ratio.
///
/// \tparam T The type to check
MODULE_EXPORT template <typename T>
concept ratio = _detail::is_ratio<std::remove_cvref_t<T>>::value;

MODULE_EXPORT template <std::intmax_t Numerator, std::intmax_t Denominator>
struct rational_type {
  static_assert(Denominator != 0, "Attempting to divide by zero");

  /// The numerator of the rational number
  static constexpr std::intmax_t numerator = Numerator;
  /// The denominator of the rational number
  static constexpr std::intmax_t denominator = Denominator;
  /// The exponent of the rational number

  /// \brief Conversion operator
  ///
  /// Returns a floating point representation of the rational number.
  ///
  /// \return A floating point representation of the rational number.
  constexpr explicit operator double() const noexcept {
    return static_cast<double>(numerator) / static_cast<double>(denominator);
  }
};

/// Constant representing the rational number zero.
MODULE_EXPORT constexpr rational_type<0, 1> zero;
/// Constant representing the rational number one.
MODULE_EXPORT constexpr rational_type<1, 1> one;

/// \brief Reduces a rational number to its simplest form.
///
/// Reduces a rational number to its simplest form.
///
/// \tparam Num The numerator of the rational number.
/// \tparam Den The denominator of the rational number
MODULE_EXPORT template <std::intmax_t Num, std::intmax_t Den>
constexpr rational auto reduce(rational_type<Num, Den> /*r*/) noexcept {
  static_assert(Den != 0, "Attempting to divide by zero");
  if constexpr (Num == 0) {
    return rational_type<0, 1>{};
  } else {
    constexpr std::intmax_t gcd = std::gcd(Num, Den);
    constexpr std::intmax_t new_num = Num / gcd;
    constexpr std::intmax_t new_den = Den / gcd;
    if constexpr (new_den < 0) {
      return rational_type<-new_num, -new_den>{};
    } else {
      return rational_type<new_num, new_den>{};
    }
  }
}

/// \brief Multiplication operator.
///
/// Computes the product of two rational numbers. The resulting rational
/// number is in the most reduced form possible.
/// \tparam N1 The numerator of the left-hand side of the multiplication.
/// \tparam D1 The denominator of the left-hand side of the multiplication.
/// \tparam E1 The exponent of the left-hand side of the multiplication.
/// \tparam N2 The numerator of the right-hand side of the multiplication.
/// \tparam D2 The denominator of the right-hand side of the multiplication.
/// \tparam E2 The exponent of the right-hand side of the multiplication.
/// \return The product of two rational numbers.
MODULE_EXPORT template <std::intmax_t N1, std::intmax_t D1, std::intmax_t N2,
                        std::intmax_t D2>
constexpr rational auto operator*(rational_type<N1, D1> lhs,
                                  rational_type<N2, D2> rhs) noexcept {
  if constexpr (std::gcd(N1, D1) != 1) {
    return reduce(lhs) * rhs;
  } else if constexpr (std::gcd(N2, D2) != 1) {
    return lhs * reduce(rhs);
  } else {
    constexpr auto res_num = N1 * N2;
    constexpr auto res_den = D1 * D2;
    return reduce(rational_type<res_num, res_den>{});
  }
}

/// \brief Division operator.
///
/// Computes the quotient of two rational numbers. The resulting rational
/// number is in the most reduced form possible.
/// \tparam N1 The numerator of the left-hand side of the division.
/// \tparam D1 The denominator of the left-hand side of the division.
/// \tparam E1 The exponent of the left-hand side of the division.
/// \tparam N2 The numerator of the right-hand side of the division.
/// \tparam D2 The denominator of the right-hand side of the division.
/// \tparam E2 The exponent of the right-hand side of the division.
/// \return The quotient of two rational numbers.
MODULE_EXPORT template <std::intmax_t N1, std::intmax_t D1, std::intmax_t N2,
                        std::intmax_t D2>
constexpr rational auto operator/(rational_type<N1, D1> lhs,
                                  rational_type<N2, D2> rhs) noexcept {
  static_assert(D1 != 0, "Attempting to divide by zero");
  static_assert(D2 != 0, "Attempting to divide by zero");
  static_assert(N2 != 0, "Attempting to divide by zero");
  if constexpr (std::gcd(N1, D1) != 1) {
    return reduce(lhs) / rhs;
  } else if constexpr (std::gcd(N2, D2) != 1) {
    return lhs / reduce(rhs);
  } else {
    constexpr auto res_num = N1 * D2;
    constexpr auto res_den = D1 * N2;
    return reduce(rational_type<res_num, res_den>{});
  }
}

/// \brief Division operator.
///
/// Computes the sum of two rational numbers.
/// The resulting rational number is in the most reduced form possible.
///
/// \tparam N1 The numerator of the left-hand side of the addition.
/// \tparam D1 The denominator of the left-hand side of the addition.
/// \tparam E1 The exponent of the left-hand side of the addition.
/// \tparam N2 The numerator of the right-hand side of the addition.
/// \tparam D2 The denominator of the right-hand side of the addition.
/// \tparam E2 The exponent of the right-hand side of the addition.
/// \return The sum of two rational numbers.
MODULE_EXPORT template <std::intmax_t N1, std::intmax_t D1, std::intmax_t N2,
                        std::intmax_t D2>
constexpr rational auto operator+(rational_type<N1, D1> /*lhs*/,
                                  rational_type<N2, D2> /*rhs*/) noexcept {
  constexpr std::intmax_t output_num = (N2 * D1) + (N1 * D2);
  constexpr std::intmax_t output_den = D1 * D2;
  return reduce<output_num, output_den>({});
}

/// \brief Subtraction operator.
///
/// Computes the difference of two rational numbers. The resulting rational
/// number is in the most reduced form possible.
///
/// \tparam N1 The numerator of the left-hand side of the subtraction.
/// \tparam D1 The denominator of the left-hand side of the subtraction.
/// \tparam E1 The exponent of the left-hand side of the subtraction.
/// \tparam N2 The numerator of the right-hand side of the subtraction.
/// \tparam D2 The denominator of the right-hand side of the subtraction.
/// \tparam E2 The exponent of the right-hand side of the subtraction.
/// \return The difference of two rational numbers.
MODULE_EXPORT template <std::intmax_t N1, std::intmax_t D1, std::intmax_t N2,
                        std::intmax_t D2>
constexpr rational auto operator-(rational_type<N1, D1> /*lhs*/,
                                  rational_type<N2, D2> /*rhs*/) noexcept {
  constexpr std::intmax_t output_num = (N1 * D2) - (N2 * D1);
  constexpr std::intmax_t output_den = D1 * D2;
  return reduce<output_num, output_den>({});
}

/// \brief Creates a \c rational_type from a \c std::ratio.
///
/// Creates a \c rational_type instance from a \c std::ratio.
/// The resulting \c rational_type will have an exponent of zero.
///
/// \tparam N The numerator
/// \tparam D The denominator
/// \return A \c rational_type instance equivalent to the \c std::ratio
/// type.
MODULE_EXPORT template <std::intmax_t N, std::intmax_t D>
constexpr rational auto from_ratio(std::ratio<N, D> /*rat*/) noexcept {
  static_assert(D != 0, "Attempting to divide by zero");
  return rational_type<N, D>{};
}

/// \brief Three way comparison operator
///
/// Three-way comparison operator for \c rational_type.
/// The comparison is performed on the most reduced form of the provided
/// rational numbers.
///
/// \tparam N1 The numerator of the left hand side of the comparison.
/// \tparam D1 The denominator of the left hand side of the comparison.
/// \tparam E1 The exponent of the left hand side of the comparison.
/// \tparam N2 The numerator of the right hand side of the comparison.
/// \tparam D2 The denominator of the right hand side of the comparison.
/// \tparam E2 The exponent of the right hand side of the comparison.
/// \param lhs The left hand side of the comparison.
/// \param rhs The right hand side of the comparison.
/// \return A \c std::strong_ordering value indicating the result of the
/// comparison.
MODULE_EXPORT template <std::intmax_t N1, std::intmax_t D1, std::intmax_t N2,
                        std::intmax_t D2>
constexpr auto operator<=>(rational_type<N1, D1> /*lhs*/,
                           rational_type<N2, D2> /*rhs*/)
    -> std::strong_ordering {
  static_assert(D1 != 0);
  static_assert(D2 != 0);

  return (N1 * D2) <=> (N2 * D1);
}

MODULE_EXPORT template <std::intmax_t N1, std::intmax_t D1, std::intmax_t N2,
                        std::intmax_t D2>
constexpr auto operator==(rational_type<N1, D1> lhs, rational_type<N2, D2> rhs)
    -> bool {
  return (lhs <=> rhs) == std::strong_ordering::equal;
}

/// \brief Computes base raised to a rational power.
///
/// Computes the value of a real number rasied to a rational power.
/// \tparam N The numerator of the rational power.
/// \tparam D The denominator of the rational power.
/// \tparam E The exponent of the rational power.
/// \param base The base to be raised to the rational power.
/// \return The value of base raised to the rational power
MODULE_EXPORT template <std::intmax_t N, std::intmax_t D>
constexpr double pow(double base, rational_type<N, D>) noexcept {
  assert(N % D == 0 && "For now only integer powers are supported");
  const int exp = N / D;
  double result = 1.0;

  int pos_exp = (exp < 0) ? -exp : exp;

  for (int i = 0; i < pos_exp; ++i) {
    result *= base;
  }

  if (exp < 0) {
    return 1.0 / result;
  }

  return result;
}

/// \brief Holds a compile-time value for unit synthesis.
///
/// Class template \c value_type holds a compile-time constant to aid in
/// creating new units. This distinguishes overloads of multiplication between
/// units from multiplication between unit and value to create a \c
/// quantity_value instance.
///
/// \tparam Value The compile-time value. Must be a structural type.
MODULE_EXPORT template <auto Value> struct value_type {
  /// The compile-time value.
  constexpr static auto value = Value;
};

/// \cond
namespace _detail {
template <typename> struct is_value_type : std::false_type {};

template <auto Value>
struct is_value_type<value_type<Value>> : std::true_type {};

template <typename T> struct is_value_type<const T> : is_value_type<T> {};
} // namespace _detail
/// \endcond

// template <typename T> constexpr T ln(T x) {
// #ifndef __GNUC__
// #endif
//   return x
// }
} // namespace maxwell::utility

namespace maxwell {
template <auto Value> constexpr auto value = utility::value_type<Value>{};
}
#endif