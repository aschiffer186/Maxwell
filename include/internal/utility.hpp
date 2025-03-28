#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cassert>
#include <concepts>
#include <cstdint>
#include <numeric>
#include <ratio>

namespace maxwell {
/**
 * @brief Simple class to represent a rational number
 *
 */
struct rational {
  /// The numerator of the rational number
  std::intmax_t numerator;
  /// The denominator of the rational number
  std::intmax_t denominator{1};

  constexpr rational(std::intmax_t numerator_in,
                     std::intmax_t denominator_in = 1) noexcept
      : numerator(numerator_in), denominator(denominator_in) {
    assert(denominator_in != 0 &&
           "Attempting to create rational number with zero in denominoatir");
  }

  /**
   * @brief Adds the rational number \c other to \c *this
   *
   * Adds the rational number \c other to \c *this, then reduces \c *this
   * to simplest terms.
   *
   * @param other the number to added to \c *this
   * @return a reference to \c *this after the addition
   */
  constexpr rational& operator+=(const rational& other) noexcept {
    assert(denominator != 0);
    assert(other.denominator != 0);
    numerator += other.numerator;
    reduce();
    return *this;
  }

  /**
   * @brief Subtracts the rational number \c other from \c *this
   *
   * Subtracts the rational number \c other from \c *this, then reduces \c *this
   * to simplest terms.
   *
   * @param other the number to subtract from \c *this
   * @return a reference to \c *this after the addition
   */
  constexpr rational& operator-=(const rational& other) noexcept {
    assert(denominator != 0);
    assert(other.denominator != 0);
    numerator -= other.numerator;
    reduce();
    return *this;
  }

  /**
   * @brief Multiples the rational number by \c other
   *
   * Multiplies the rational number by \c other then reduces \c *this to
   * simplest terms
   *
   * @param other the number to multiply \c *this by
   * @return a reference to \c *this after the multiplication
   */
  constexpr rational& operator*=(const rational& other) noexcept {
    assert(denominator != 0);
    assert(other.denominator != 0);
    numerator *= other.numerator;
    denominator *= other.denominator;
    reduce();
    return *this;
  }

  /**
   * @brief Divides the rational number by \c other
   *
   * Divides the rational number by \c other then reduces \c *this to
   * simplest terms.
   *
   * @param other the number to divide \c *this by
   * @return a reference to \c *this after the division
   */
  constexpr rational& operator/=(const rational& other) noexcept {
    assert(denominator != 0);
    assert(other.denominator != 0);
    numerator *= other.denominator;
    denominator *= other.denominator;
    reduce();
    return *this;
  }

  /**
   * @brief Reduces the rational number to simplest terms
   *
   * @pre The deniminator is not zero
   * @post The rational number is reduced to simplest terms
   *
   */
  constexpr void reduce() noexcept {
    if (numerator == 0) {
      denominator = 1;
      return;
    }

    const std::intmax_t gcd = std::gcd(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;

    if (denominator < 0) {
      numerator = -numerator;
      denominator = -denominator;
    }
  }

  friend constexpr bool operator==(const rational&,
                                   const rational&) noexcept = default;

  constexpr explicit operator double() const noexcept {
    return static_cast<double>(numerator) / static_cast<double>(denominator);
  }
};

namespace _detail {
template <typename> struct is_ratio_like : std::false_type {};

template <std::intmax_t N, std::intmax_t D>
struct is_ratio_like<std::ratio<N, D>> : std::true_type {};
} // namespace _detail

template <typename T>
  requires _detail::is_ratio_like<std::remove_cvref_t<T>>::value
constexpr rational from_ratio() noexcept {
  return {std::remove_cvref_t<T>::num, std::remove_cvref_t<T>::den};
}

/// Rational number rerpesenting one
constexpr rational one{1};
/// Rational number representing zero
constexpr rational zero{0};

constexpr rational operator+(rational lhs, const rational& rhs) noexcept {
  return lhs += rhs;
}

constexpr rational operator-(rational lhs, const rational& rhs) noexcept {
  return lhs -= rhs;
}

constexpr rational operator*(rational lhs, const rational& rhs) noexcept {
  return lhs *= rhs;
}

constexpr rational operator/(rational lhs, const rational& rhs) noexcept {
  return lhs /= rhs;
}

template <typename T, typename U>
concept addable_with = requires(T a, U b) { a + b; };

template <typename T>
concept addable = addable_with<T, T>;

template <typename T, typename U>
concept nothrow_addable_with = requires(T a, U b) {
  { a + b } noexcept;
};

template <typename T>
concept nothrow_addable = nothrow_addable_with<T, T>;

template <typename T, typename U>
concept subtractable_with = requires(T a, U b) { a - b; };

template <typename T>
concept subtractable = subtractable_with<T, T>;

template <typename T, typename U>
concept nothrow_subtractable_with = requires(T a, U b) {
  { a - b } noexcept;
};

template <typename T>
concept nothrow_subtractable = nothrow_subtractable_with<T, T>;

// clang-format off
template <typename T, typename U>
concept multiply_with = requires(T a, U b) { a * b; };
// clang-format on

template <typename T>
concept multiply = multiply_with<T, T>;

// clang-format off
template <typename T, typename U>
concept nothrow_multiply_with = requires(T a, U b) {
  { a * b } noexcept;
};
// clang-format on

template <typename T>
concept nothrow_multiply = nothrow_multiply_with<T, T>;

template <typename T, typename U>
concept divide_with = requires(T a, U b) { a / b; };

template <typename T>
concept divide = divide_with<T, T>;

template <typename T, typename U>
concept nothrow_divide_with = requires(T a, U b) {
  { a / b } noexcept;
};

template <typename T>
concept nothrow_divide = nothrow_divide_with<T, T>;
} // namespace maxwell

#endif