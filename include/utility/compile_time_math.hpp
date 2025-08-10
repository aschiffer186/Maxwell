/// \file rational.hpp
/// \brief Definition of compile-time rational number and related operations.

#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#include <cassert>
#include <compare>
#include <cstdint>
#include <numeric>
#include <ratio>
#include <type_traits>

/// \namespace maxwell::utility
/// \brief Namespace for uility classes.
namespace maxwell::utility {

constexpr std::intmax_t pos_pow(std::intmax_t base,
                                std::intmax_t pow) noexcept {
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
constexpr std::intmax_t pos_pow_10(std::intmax_t pow) noexcept {
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
template <std::intmax_t Numerator, std::intmax_t Denominator,
          std::intmax_t Exponent>
struct rational_type;

/// \cond
namespace _detail {
template <typename> struct is_rational_type : std::false_type {};

template <std::intmax_t Numerator, std::intmax_t Denominator,
          std::intmax_t Exponent>
struct is_rational_type<rational_type<Numerator, Denominator, Exponent>>
    : std::true_type {};

template <typename> struct is_ratio : std::false_type {};

template <std::intmax_t N, std::intmax_t D>
struct is_ratio<std::ratio<N, D>> : std::true_type {};
} // namespace _detail
/// \endcond

/// \brief Concept indicating a type is an instantiation of \c rational_type.
///
/// \tparam T The type to check.
template <typename T>
concept rational = _detail::is_rational_type<std::remove_cvref_t<T>>::value;

/// \brief Concept indicating a type is an instantiation of \c std::ratio.
///
/// \tparam T The type to check
template <typename T>
concept ratio = _detail::is_ratio<std::remove_cvref_t<T>>::value;

template <std::intmax_t Numerator, std::intmax_t Denominator,
          std::intmax_t Exponent>
struct rational_type {
  static_assert(Denominator != 0, "Attempting to divide by zero");

  static constexpr std::intmax_t numerator = Numerator;
  static constexpr std::intmax_t denominator = Denominator;
  static constexpr std::intmax_t exponent = Exponent;

  constexpr explicit operator double() const noexcept {
    const double pow =
        (exponent >= 0) ? pos_pow_10(numerator) : 1.0 / pos_pow_10(-exponent);
    return pow * static_cast<double>(numerator) /
           static_cast<double>(denominator);
  }
};

constexpr rational_type<0, 1, 0> zero;
constexpr rational_type<1, 1, 0> one;

template <std::intmax_t Num, std::intmax_t Den, std::intmax_t Exp>
constexpr rational auto reduce(rational_type<Num, Den, Exp> /*r*/) noexcept {
  static_assert(Den != 0, "Attempting to divide by zero");
  if constexpr (Num == 0) {
    return rational_type<0, 1, Exp>{};
  } else {
    constexpr std::intmax_t gcd = std::gcd(Num, Den);
    constexpr std::intmax_t new_num = Num / gcd;
    constexpr std::intmax_t new_den = Den / gcd;
    if constexpr (new_den < 0) {
      return rational_type<-new_num, -new_den, Exp>{};
    } else {
      return rational_type<new_num, new_den, Exp>{};
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
template <std::intmax_t N1, std::intmax_t D1, std::intmax_t E1,
          std::intmax_t N2, std::intmax_t D2, std::intmax_t E2>
constexpr rational auto operator*(rational_type<N1, D1, E1> lhs,
                                  rational_type<N2, D2, E2> rhs) noexcept {
  if constexpr (std::gcd(N1, D1) != 1) {
    return reduce(lhs) * rhs;
  } else if constexpr (std::gcd(N2, D2) != 1) {
    return lhs * reduce(rhs);
  } else {
    constexpr auto res_num = N1 * N2;
    constexpr auto res_den = D1 * D2;
    constexpr auto res_exp = E1 + E2;
    return reduce(rational_type<res_num, res_den, res_exp>{});
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
template <std::intmax_t N1, std::intmax_t D1, std::intmax_t E1,
          std::intmax_t N2, std::intmax_t D2, std::intmax_t E2>
constexpr rational auto operator/(rational_type<N1, D1, E1> lhs,
                                  rational_type<N2, D2, E2> rhs) noexcept {
  static_assert(D1 != 0, "Attempting to divide by zero");
  static_assert(D2 != 0, "Attempting to divide by zero");
  static_assert(N2 != 0, "Attempting to divide by zero");
  if constexpr (std::gcd(N1, D1) != 1) {
    return reduce(lhs) / rhs;
  } else if constexpr (std::gcd(N2, D2) != 1) {
    return lhs / reduce(rhs);
  } else {
    constexpr auto res_num = N1 / N2;
    constexpr auto res_den = D1 / D2;
    constexpr auto res_exp = E1 - E2;
    return reduce(rational_type<res_num, res_den, res_exp>{});
  }
}

template <std::intmax_t N1, std::intmax_t D1, std::intmax_t E1,
          std::intmax_t N2, std::intmax_t D2, std::intmax_t E2>
constexpr rational auto operator+(rational_type<N1, D1, E1> /*lhs*/,
                                  rational_type<N2, D2, E2> /*rhs*/) noexcept {
  constexpr std::intmax_t common_pow = E1;
  constexpr std::intmax_t pow_diff = common_pow - E2;
  if constexpr (pow_diff > 0) {
    constexpr std::intmax_t new_den = D2;
    constexpr std::intmax_t new_num = N2 * pos_pow_10(pow_diff);
    constexpr std::intmax_t output_num = (new_num * D1) + (N1 * new_den);
    constexpr std::intmax_t output_den = D1 * new_den;
    return rational_type<output_num, output_den, common_pow>{};
  } else {
    constexpr std::intmax_t new_num = N2;
    constexpr std::intmax_t new_den = D2 * pos_pow_10(-pow_diff);
    constexpr std::intmax_t output_num = (new_num * D1) + (N1 * new_den);
    constexpr std::intmax_t output_den = D1 * new_den;
    return reduce<output_num, output_den, common_pow>({});
  }
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
template <std::intmax_t N, std::intmax_t D>
constexpr rational auto from_ratio(std::ratio<N, D> /*rat*/) noexcept {
  static_assert(D != 0, "Attempting to divide by zero");
  return rational_type<N, D, 0>{};
}

template <std::intmax_t N1, std::intmax_t D1, std::intmax_t E1,
          std::intmax_t N2, std::intmax_t D2, std::intmax_t E2>
constexpr std::strong_ordering operator<=>(rational_type<N1, D1, E1> /*lhs*/,
                                           rational_type<N2, D2, E2> /*rhs*/) {
  static_assert(D1 != 0);
  static_assert(D2 != 0);
  const std::intmax_t common_pow = E1;
  const std::intmax_t pow_diff = common_pow - E2;
  if (pow_diff > 0) {
    const std::intmax_t new_den = D2;
    const std::intmax_t new_num = N2 * pos_pow_10(pow_diff);
    return (new_num * D1) <=> (N1 * new_den);
  } else {
    const std::intmax_t new_num = N2;
    const std::intmax_t new_den = D2 * pos_pow_10(-pow_diff);
    return (new_num * D1) <=> (N1 * new_den);
  }
}
} // namespace maxwell::utility

#endif