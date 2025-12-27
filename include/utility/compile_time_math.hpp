/// \file compile_time_math.hpp
/// \brief Definition of compile-time rational number and related operations.

#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#ifndef MAXWELL_MODULES
#include <cassert>     // assert
#include <compare>     // strong_ordering
#include <cstdint>     // intmax_t
#include <limits>      // numeric_limits
#include <numeric>     // gcd
#include <ratio>       // ratio
#include <type_traits> // false_type, remove_cvref_t, true_type
#endif

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
MODULE_EXPORT constexpr auto pos_pow(const std::intmax_t base,
                                     const std::intmax_t pow) noexcept
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
MODULE_EXPORT constexpr auto pos_pow_10(const std::intmax_t pow) noexcept
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
constexpr rational auto reduce(const rational_type<Num, Den> /*r*/) noexcept {
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
constexpr rational auto operator*(const rational_type<N1, D1> lhs,
                                  const rational_type<N2, D2> rhs) noexcept {
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
constexpr rational auto operator/(const rational_type<N1, D1> lhs,
                                  const rational_type<N2, D2> rhs) noexcept {
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
constexpr rational auto operator+(const rational_type<N1, D1> /*lhs*/,
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
constexpr rational auto
operator-(const rational_type<N1, D1> /*lhs*/,
          const rational_type<N2, D2> /*rhs*/) noexcept {
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
constexpr rational auto from_ratio(const std::ratio<N, D> /*rat*/) noexcept {
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
constexpr auto operator<=>(const rational_type<N1, D1> /*lhs*/,
                           rational_type<N2, D2> /*rhs*/)
    -> std::strong_ordering {
  static_assert(D1 != 0);
  static_assert(D2 != 0);

  return (N1 * D2) <=> (N2 * D1);
}

MODULE_EXPORT template <std::intmax_t N1, std::intmax_t D1, std::intmax_t N2,
                        std::intmax_t D2>
constexpr auto operator==(const rational_type<N1, D1> lhs,
                          const rational_type<N2, D2> rhs) -> bool {
  return (lhs <=> rhs) == std::strong_ordering::equal;
}

constexpr double ln(double x) noexcept;

/// \brief Computes base raised to a rational power.
///
/// Computes the value of a real number rasied to a rational power.
/// \tparam N The numerator of the rational power.
/// \tparam D The denominator of the rational power.
/// \tparam E The exponent of the rational power.
/// \param base The base to be raised to the rational power.
/// \return The value of base raised to the rational power
namespace _detail {
// Integer exponentiation for doubles (positive exponent) using binary exp.
constexpr double pow_double_pos(double base, std::intmax_t exp) noexcept {
  double result = 1.0;
  while (exp > 0) {
    if (exp & 1)
      result *= base;
    base *= base;
    exp >>= 1;
  }
  return result;
}

// Newton iteration for nth root: returns x^(1/n) for x > 0 and n > 0.
constexpr double nth_root_impl(const double x, const std::intmax_t n,
                               double curr, const double prev) noexcept {
  if (curr == prev)
    return curr;
  const double t = pow_double_pos(curr, n - 1);
  const double next =
      (static_cast<double>(n - 1) * curr + x / t) / static_cast<double>(n);
  return nth_root_impl(x, n, next, curr);
}

constexpr double nth_root(const double x, const std::intmax_t n) noexcept {
  assert(n > 0);
  if (x == 0.0)
    return 0.0;
  if (n == 1)
    return x;
  // Initial guess: use x/n which often works; for small x keep x.
  double init = x;
  if (x > 1.0)
    init = x / static_cast<double>(n);
  return nth_root_impl(x, n, init, 0.0);
}
} // namespace _detail

/// \brief Computes base raised to a rational power.
///
/// Supports arbitrary rational powers N/D at compile time.
MODULE_EXPORT template <std::intmax_t N, std::intmax_t D>
constexpr double pow(const double base,
                     const rational_type<N, D> /*pow*/) noexcept {
  static_assert(D != 0, "Denominator must not be zero");

  // Reduce the rational exponent at compile time.
  constexpr std::intmax_t g = std::gcd(N, D);
  constexpr std::intmax_t num = N / g;
  constexpr std::intmax_t den = D / g;

  // Handle special cases with zero base.
  if (base == 0.0) {
    // 0^0 is undefined; force a runtime assert to catch misuse.
    if (num == 0) {
      assert(false && "0^0 is undefined");
      return 1.0;
    }
    // 0^(negative) -> division by zero
    if (num < 0) {
      assert(false && "zero base with negative exponent (division by zero)");
      return std::numeric_limits<double>::infinity();
    }
    return 0.0;
  }

  const bool neg_base = (base < 0.0);
  const double abs_base = neg_base ? -base : base;

  // If base < 0 and denominator even, result is non-real.
  if (neg_base && (den % 2 == 0)) {
    assert(false && "negative base with even denominator -> non-real result");
    return 0.0;
  }

  // Compute the den-th root of the absolute base.
  double root = 0.0;
  if (abs_base == 1.0) {
    root = 1.0; // exactly 1 for all roots
  } else {
    root = _detail::nth_root(abs_base, den);
  }

  const std::intmax_t abs_num = (num < 0) ? -num : num;
  double abs_pow = _detail::pow_double_pos(root, abs_num);

  if (num < 0)
    abs_pow = 1.0 / abs_pow;

  // Restore sign for odd numerator when base was negative.
  if (neg_base && (num % 2 != 0))
    abs_pow = -abs_pow;

  return abs_pow;
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

constexpr double sqrt_impl(const double x, const double curr,
                           const double prev) noexcept {
  if (curr == prev) {
    return curr;
  }
  return sqrt_impl(x, 0.5 * (curr + x / curr), curr);
}
} // namespace _detail
/// \endcond

constexpr double sqrt(double x) noexcept {
  assert(x >= 0.0);
  assert(!(x != x));
  return _detail::sqrt_impl(x, x, 0.0);
}

// Forward declaration of ln so compile-time checks can reference it.

// Compile-time examples / smoke tests
namespace _compile_time_checks {
constexpr double absd(const double x) noexcept { return x < 0.0 ? -x : x; }
constexpr bool approx_equal(const double a, const double b,
                            const double eps = 1e-12) noexcept {
  return absd(a - b) <= eps;
}

static_assert(maxwell::utility::pow<2, 1>(
                  2.0, maxwell::utility::rational_type<2, 1>{}) == 4.0);
// square root of 9 is approximately 3
static_assert(approx_equal(
    maxwell::utility::pow<1, 2>(9.0, maxwell::utility::rational_type<1, 2>{}),
    3.0));
// cube root inverse: (-8)^(-1/3) == -0.5
static_assert(approx_equal(maxwell::utility::pow<-1, 3>(
                               -8.0, maxwell::utility::rational_type<-1, 3>{}),
                           -0.5));
} // namespace _compile_time_checks

// constexpr natural logarithm for double using range reduction and
// atanh-series: ln(y) = 2 * (z + z^3/3 + z^5/5 + ...), where z = (y-1)/(y+1)
// Range reduction: repeatedly take sqrt to bring y close to 1 and then
// multiply the result by 2^k where k is the number of sqrt operations.
constexpr double ln(const double x) noexcept {
  assert(x > 0.0 && "ln domain error: x must be positive");
  if (x == 1.0)
    return 0.0;

  // Handle subnormal / tiny values by scaling up using powers of two via
  // frexp-like loop We'll use repeated sqrt reduction: write x = y^(2^k) =>
  // ln(x) = 2^k * ln(y)
  int k = 0;
  double y = x;
  // Reduce until y is in (0.5, 2.0) to ensure good convergence of series
  // around 1.
  while (y > 2.0) {
    y = _detail::nth_root(y, 2); // sqrt
    ++k;
    if (k > 60)
      break; // avoid infinite loops
  }

  while (y < 0.5) {
    y = _detail::nth_root(y, 2); // inverse of sqrt step
    --k;
    if (k < -60)
      break;
  }

  // Now y should be in a reasonable range; shift to form suitable for atanh
  // series
  const double z = (y - 1.0) / (y + 1.0);

  // Sum series 2 * sum_{n=0..} z^{2n+1} / (2n+1)
  constexpr int MAX_ITERS = 300;
  constexpr double EPS = 1e-16;

  double term = z; // z^(2n+1) initially n=0
  double sum = term;
  for (int n = 1; n < MAX_ITERS; ++n) {
    term *= z * z; // increase power by 2
    const double add = term / (2 * n + 1);
    sum += add;
    if (!(add != add)) { /* no-op to keep constexpr-friendly */
    }
    if ((add < 0.0 ? -add : add) < EPS)
      break;
  }

  double result = 2.0 * sum;
  // Multiply back by 2^k: ln(x) = 2^k * ln(y)
  if (k > 0) {
    for (int i = 0; i < k; ++i)
      result *= 2.0;
  } else if (k < 0) {
    for (int i = 0; i < -k; ++i)
      result *= 2.0;
  }

  return result;
}

constexpr double log10(const double x) noexcept {
  constexpr double ln10_inv = 1.0 / 2.30258509299;
  const double ln_value = ln(x);
  return ln_value * ln10_inv;
}
} // namespace maxwell::utility

namespace maxwell {
/// Convience type alias for creating a \c value_type instance.
template <double Value> constexpr auto value = utility::value_type<Value>{};

/// Convience type alias for creating a \c rational_type instance.
template <std::intmax_t N, std::intmax_t D>
constexpr utility::rational auto rational = utility::rational_type<N, D>{};
} // namespace maxwell
#endif