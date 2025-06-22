/// \file utility.hpp
/// \brief Definition of common utilities used throughout Maxwell.

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <ratio>
#include <type_traits>

/// \namespace maxwell
/// \brief Definition of public API of Maxwell.
namespace maxwell {

/// \brief Concept modeling if two types are similar.
///
/// Concept modeling if two type are similar. This concept is
/// modeled if two types are the same up to <em>cv</em>-ref qualifiers.
///
/// \tparam T The first type to test.
/// \tparam U The second type to test.
template <typename T, typename U>
concept similar = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

/// \brief Basic string literal that can be used as template parameter.
///
/// Class \c string_literal wraps a string literal, allowing it to be used
/// as a non-type template parameter. It provides the minimum functionality
/// required to be treated as a \c sized_range.
template <std::size_t Length> struct string_literal {
  static_assert(Length > 0);

  std::array<char, Length> data;

  /// \brief Constructor
  ///
  /// Constructs a string literal whose data is a copy of the specified
  /// character array.
  ///
  /// \pre \c str has a null terminator.
  ///
  /// \param str The character array to copy into the string literal.
  constexpr string_literal(const char (&str)[Length]) {
    std::ranges::copy(str, data.begin());
  }

  /// \brief Returns the size of the string literal.
  ///
  /// \return The size of the string literal, excluding the null terminator.
  constexpr std::size_t size() const noexcept { return Length - 1; }

  /// \brief Returns an iterator to the beginning of the string literal.
  ///
  /// \return An iterator the beginning of the string literal.
  constexpr auto begin() const noexcept { return data.begin(); }

  /// \brief Returns an iterator past the end of the string literal.
  ///
  /// \return An iterator past the end of the string literal.
  constexpr auto end() const noexcept { return data.end(); }
};

template <std::size_t L> string_literal(const char (&)[L]) -> string_literal<L>;

/// \brief Concatenates two string literals
///
/// Concatenates two string literals. The resultant string literal is
/// equal to the left-hand string literal minus the null-terminator +
/// the right-hand string literal minus the null-terminator + a null-terminator.
///
/// \tparam L The length of the left-hand string literal.
/// \tparam R The length of the right-hand string literal.
/// \param lhs The left-hand string literal of the concatenation
/// \param rhs The right-hand string literal of the concatenation
/// \return The two string literals concatenated.
template <std::size_t L, std::size_t R>
constexpr string_literal<L + R - 1> operator+(const string_literal<L>& lhs,
                                              const string_literal<R>& rhs) {
  char data[L + R - 1];
  auto it = std::ranges::copy(lhs.begin(), std::prev(lhs.end()), data);
  std::ranges::copy(rhs, it.out);
  return string_literal(data);
}

/// \brief Three way comparison operator
///
/// Implements three way comparison for two string literals.
/// The result of the three way comparison is the result of the three
/// way comparison of the underlying character arrays.
///
/// \tparam L1 The length of the left-hand side.
/// \tparam L2 The length of the right-hand side.
/// \param lhs The left-hand side of the three way comparison.
/// \param rhs The right-hand side of the three way comparison.
/// \return \c true if the string literals are equal.
template <std::size_t L1, std::size_t L2>
constexpr auto operator<=>(const string_literal<L1>& lhs,
                           const string_literal<L2>& rhs) {
  if (L1 != L2) {
    return L1 <=> L2;
  }
  return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
}

/// \brief Equality operator
///
/// Computes if two string literals are equal.
///
/// \tparam L1 The length of the left-hand side.
/// \tparam L2 The length of the right-hand side.
/// \param lhs The left-hand side of the equality comparison.
/// \param rhs The right-hand side of the quality comparison.
/// \return \c true if the string literals are equal.
template <std::size_t L1, std::size_t L2>
constexpr auto operator==(const string_literal<L1>& lhs,
                          const string_literal<L2>& rhs) {
  if (L1 != L2) {
    return false;
  }
  return std::ranges::equal(lhs, rhs);
}

/// \cond
namespace _detail {
template <typename> struct is_ratio : std::false_type {};

template <std::intmax_t N, std::intmax_t D>
struct is_ratio<std::ratio<N, D>> : std::true_type {};

consteval std::intmax_t pos_pow_10(std::intmax_t pow) noexcept {
  assert(pow >= 0);
  if (pow == 0) {
    return 1;
  } else if (pow % 2 == 0) {
    const std::intmax_t res = pos_pow_10(pow / 2);
    return res * res;
  } else {
    constexpr std::intmax_t base{10};
    return base * pos_pow_10(pow - 1);
  }
}
} // namespace _detail
/// \endcond

/// \brief Concept modeling if a type is an instantiation of \c std::ratio
template <typename T>
concept ratio_like = _detail::is_ratio<std::remove_cvref_t<T>>::value;

/// \brief Compile-time rational number
///
/// Class template \c rational_type implements a compile-time rational number.
/// Unlike \c std::ratio, \c rational_type maintains an exponent parameter,
/// allowing it to more easily express very large or very small rational types
/// without overflow. In other words, an instantiation of \c rational_number
/// is equal to <br>
///
/// \f$ \frac{N}{D}\times 10^E \f$
///
/// \tparam N The numerator of the rational type.
/// \tparam D The denominator of the rational type.
/// \tparam E The exponent of the rational type.
template <std::intmax_t N, std::intmax_t D, std::intmax_t E>
struct rational_type {
  static_assert(D != 0);

  /// The numerator of the rational type.
  constexpr static std::intmax_t num = N;
  /// The denominatorof the rational type.
  constexpr static std::intmax_t den = D;
  /// The exponent of the rational type.
  constexpr static std::intmax_t exp = E;

  /// \brief Convert \c rational_number to a \c double
  consteval operator long double() const noexcept {
    const long double pow = (exp >= 0) ? _detail::pos_pow_10(num)
                                       : (1.0 / _detail::pos_pow_10(-exp));
    return pow * static_cast<long double>(num) / static_cast<long double>(den);
  }
};

/// \cond
namespace _detail {
template <typename> struct is_rational_type : std::false_type {};

template <std::intmax_t N, std::intmax_t D, std::intmax_t E>
struct is_rational_type<rational_type<N, D, E>> : std::true_type {};

template <std::intmax_t Num, std::intmax_t Den, std::intmax_t Exp>
consteval auto reduce() {
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
} // namespace _detail
/// \endcond

/// \brief Concept modeling if a type is an instantiation of \c rational_type
template <typename T>
concept rational = _detail::is_rational_type<std::remove_cvref_t<T>>::value;

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
consteval rational auto operator*(rational_type<N1, D1, E1> /*lhs*/,
                                  rational_type<N2, D2, E2> /*rhs*/) noexcept {
  if constexpr (std::gcd(N1, D1) != 1) {
    constexpr std::intmax_t g = std::gcd(N1, D1);
    constexpr std::intmax_t new_n = N1 / g;
    constexpr std::intmax_t new_d = D1 / g;
    return rational_type<new_n, new_d, E1>{} * rational_type<N2, D2, E2>{};
  } else if constexpr (std::gcd(N2, D2) != 1) {
    constexpr std::intmax_t g = std::gcd(N2, D2);
    constexpr std::intmax_t new_n = N2 / g;
    constexpr std::intmax_t new_d = D2 / g;
    return rational_type<N1, D1, E1>{} * rational_type<new_n, new_d, E2>{};
  } else {
    constexpr std::intmax_t res_num = N1 * N2;
    constexpr std::intmax_t res_den = D1 * D2;
    constexpr std::intmax_t res_exp = E1 * E2;
    return _detail::reduce<res_num, res_den, res_exp>();
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
consteval rational auto operator/(rational_type<N1, D1, E1> /*lhs*/,
                                  rational_type<N2, D2, E2> /*rhs*/) noexcept {
  if constexpr (std::gcd(N1, D1) != 1) {
    constexpr std::intmax_t g = std::gcd(N1, D1);
    constexpr std::intmax_t new_n = N1 / g;
    constexpr std::intmax_t new_d = D1 / g;
    return rational_type<new_n, new_d, E1>{} / rational_type<N2, D2, E2>{};
  } else if constexpr (std::gcd(N2, D2) != 1) {
    constexpr std::intmax_t g = std::gcd(N2, D2);
    constexpr std::intmax_t new_n = N2 / g;
    constexpr std::intmax_t new_d = D2 / g;
    return rational_type<N1, D1, E1>{} / rational_type<new_n, new_d, E2>{};
  } else {
    return _detail::reduce<N1 * D2, D1 * N2, E1 - E2>();
  }
}

/// \brief Addition operator
///
/// Computes the sum of two rational numbers. The resulting rational number
/// is in the most reduced form possible.
///
/// \tparam N1 The numerator of the left-hand side of the addition.
/// \tparam D1 The denominator of the left-hand side of the addition.
/// \tparam E1 The expomnent of the left-hand side of the additin.
/// \tparam N2 The numerator of the right-hand side of the additoion.
/// \tparam D2 The denominator of the right-hand side of the addition.
/// \tparam E2 The expomnent of the right-hand side of the addition.
/// \return The sum of two rational numbers.
template <std::intmax_t N1, std::intmax_t D1, std::intmax_t E1,
          std::intmax_t N2, std::intmax_t D2, std::intmax_t E2>
consteval auto operator+(rational_type<N1, D1, E1> /*lhs*/,
                         rational_type<N2, D2, E2> /*rhs*/) noexcept {
  constexpr std::intmax_t common_pow = E1;
  constexpr std::intmax_t pow_diff = common_pow - E2;

  if constexpr (pow_diff > 0) {
    constexpr std::intmax_t new_den = D2;
    constexpr std::intmax_t new_num = N2 * _detail::pos_pow_10(pow_diff);
    constexpr std::intmax_t output_num = (new_num * D1) + (N1 * new_den);
    constexpr std::intmax_t output_den = D1 * new_den;
    return rational_type<output_num, output_den, common_pow>{};
  } else {
    constexpr std::intmax_t new_num = N2;
    constexpr std::intmax_t new_den = D2 * _detail::pos_pow_10(-pow_diff);
    constexpr std::intmax_t output_num = (new_num * D1) + (N1 * new_den);
    constexpr std::intmax_t output_den = D1 * new_den;
    return _detail::reduce<output_num, output_den, common_pow>();
  }
}

/// \brief Subtraction operator
///
/// Computes the difference of two rational numbers. The resulting rational
/// number is in the most reduced form possible.
///
/// \tparam N1 The numerator of the left-hand side of the subtraction.
/// \tparam D1 The denominator of the left-hand side of the subtraction.
/// \tparam E1 The expomnent of the left-hand side of the subtraction.
/// \tparam N2 The numerator of the right-hand side of the asubtraction.
/// \tparam D2 The denominator of the right-hand side of the subtraction.
/// \tparam E2 The expomnent of the right-hand side of the subtraction.
/// \return The difference of two rational numbers.
template <std::intmax_t N1, std::intmax_t D1, std::intmax_t E1,
          std::intmax_t N2, std::intmax_t D2, std::intmax_t E2>
consteval auto operator-(rational_type<N1, D1, E1> /*lhs*/,
                         rational_type<N2, D2, E2> /*rhs*/) noexcept {
  constexpr std::intmax_t common_pow = E1;
  constexpr std::intmax_t pow_diff = common_pow - E2;

  if constexpr (pow_diff > 0) {
    constexpr std::intmax_t new_den = D2;
    constexpr std::intmax_t new_num = N2 * _detail::pos_pow_10(pow_diff);
    constexpr std::intmax_t output_num = (N1 * new_den) - (new_num * D1);
    constexpr std::intmax_t output_den = D1 * new_den;
    return rational_type<output_num, output_den, common_pow>{};
  } else {
    constexpr std::intmax_t new_num = N2;
    constexpr std::intmax_t new_den = D2 * _detail::pos_pow_10(-pow_diff);
    constexpr std::intmax_t output_num = (N1 * new_den) - (new_num * D1);
    constexpr std::intmax_t output_den = D1 * new_den;
    return _detail::reduce<output_num, output_den, common_pow>();
  }
}

constexpr rational_type<1, 1, 0> one;
constexpr rational_type<0, 1, 0> zero;

// Metric prefixes
constexpr rational_type<1, 1, 30> quetta;
constexpr rational_type<1, 1, 27> ronna;
constexpr rational_type<1, 1, 24> yotta;
constexpr rational_type<1, 1, 21> zetta;
constexpr rational_type<1, 1, 18> exa;
constexpr rational_type<1, 1, 15> peta;
constexpr rational_type<1, 1, 12> tera;
constexpr rational_type<1, 1, 9> giga;
constexpr rational_type<1, 1, 6> mega;
constexpr rational_type<1, 1, 3> kilo;
constexpr rational_type<1, 1, 2> hecto;
constexpr rational_type<1, 1, 1> deca;
constexpr rational_type<1, 1, -1> deci;
constexpr rational_type<1, 1, -2> centi;
constexpr rational_type<1, 1, -3> milli;
constexpr rational_type<1, 1, -6> micro;
constexpr rational_type<1, 1, -9> nano;
constexpr rational_type<1, 1, -12> pico;
constexpr rational_type<1, 1, -15> femto;
constexpr rational_type<1, 1, -18> atto;
constexpr rational_type<1, 1, -21> zepto;
constexpr rational_type<1, 1, -24> yocto;
constexpr rational_type<1, 1, -27> ronto;
constexpr rational_type<1, 1, -30> quecto;
} // namespace maxwell

#endif