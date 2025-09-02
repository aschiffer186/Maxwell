/// \file template_string.hpp
/// \brief Definition of NTTP representation of string literals.

#ifndef TEMPLATE_STRING_HPP
#define TEMPLATE_STRING_HPP

#include <algorithm>  // ranges::copy, lexicographical_compare_three_way
#include <array>      // array
#include <compare>    // strong_ordering
#include <concepts>   // unsigned_integral
#include <cstddef>    // size_t
#include <functional> // hash
#include <iterator>   // contiguous_iterator

#include "config.hpp"

/// \namespace maxwell::utility
/// \brief Namespace for uility classes.
namespace maxwell::utility {
/// \brief Representation of string literal as template parameter.
///
/// Class template \c template_string is a simple wrapper around a
/// null-terminated string literal that can be used as a template parameter.
///
/// \tparam Length The length of the string literal excluding the null
/// terminator.
MODULE_EXPORT template <std::size_t Length> struct template_string {
  std::array<char, Length> _data;

  /// \brief Constructor
  ///
  /// Creates a \c template_string instance from a character array.
  /// The resulting \c template_string contains a copy of the data of the array.
  ///
  /// \post <tt>std::ranges::equal(str, *this)</tt>
  /// \param str The character array used to initialize \c *this.
  constexpr template_string(const char (&str)[Length]) {
    std::ranges::copy(str, _data.begin());
  }

  /// \brief Returns an iterator to the beginning of the string
  ///
  /// Returns an iterator pointing to the first character of the string.
  /// The returned iterator models \c std::contiguous_iterator.
  ///
  /// \return An iterator to the beginning of the string.
  constexpr std::contiguous_iterator auto begin() const noexcept {
    return _data.begin();
  }

  /// \brief Returns an iterator pointing past the end of the string
  ///
  /// Returns an iterator pointing past the end of the string.
  /// Dereferencing this iterator is undefined beheavior.
  /// The returned iterator models \c std::contiguous_iterator.
  ///
  /// \return An iterator pointing past the end of the string.
  constexpr std::contiguous_iterator auto end() const noexcept {
    return _data.end() - 1;
  }

  /// \brief Returns the number of elements in the string.
  ///
  /// Returns the number of elements in the string.
  /// Equivalent to <tt>std::distance(begin(), end())</tt>.
  ///
  //// \return The number of elements in the string.
  constexpr std::unsigned_integral auto size() const noexcept { return Length; }
};

MODULE_EXPORT template <std::size_t N>
template_string(const char (&)[N]) -> template_string<(N)>;

/// \brief Three way comparison operator of \c template_string
///
/// Perfoms a lexicographical comparison of two \c template_string instances
/// using three way comparison.
///
/// \tparam N1 The length of the left hand side of the comaprison.
/// \tparam N2 The length of the right hand side of the comparison.
/// \param lhs The left hand side of the comparison.
/// \param rhs The right hand side of the comparison.
/// \return The order between the fist non-equivalent pair of elements.
MODULE_EXPORT template <std::size_t N1, std::size_t N2>
constexpr auto operator<=>(const template_string<N1>& lhs,
                           const template_string<N2>& rhs) {
  return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                rhs.begin(), rhs.end());
}

/// \brief Equality operator
///
/// Compares two \c template_string instances for equality.
///
/// \tparam N1 The length of the left hand side of the comparison.
/// \tparam N2 The length of the right hand side of the comparison.
/// \param lhs The left hand side of the comparison.
/// \param rhs The right hand side of the comparison.
/// \return \c true if the strings are equal.
MODULE_EXPORT template <std::size_t N1, std::size_t N2>
constexpr auto operator==(const template_string<N1>& lhs,
                          const template_string<N2>& rhs) -> bool {
  return (lhs <=> rhs) == std::strong_ordering::equal;
}

/// \brief Concatenation operator
///
/// Concatenates two \c template_string instances.
///
/// \tparam L The length of the first string.
/// \tparam R The length of the second string.
/// \param lhs The left hand side of the concatenation
/// \param rhs The right hand side of the concatenation
/// \return The concatenation of \c lhs and \c rhs.
MODULE_EXPORT template <std::size_t L, std::size_t R>
constexpr auto operator+(const template_string<L>& lhs,
                         const template_string<R>& rhs) {
  char data[L + R - 1];
  auto it = std::ranges::copy(lhs, data);
  std::ranges::copy(rhs, it.out);
  data[L + R - 2] = '\0';
  return template_string(data);
}
} // namespace maxwell::utility

MODULE_EXPORT template <std::size_t N>
struct std::hash<maxwell::utility::template_string<N>> {
  std::size_t
  operator()(const maxwell::utility::template_string<N>& str) noexcept {
    return std::hash<std::string_view>{}(
        std::string_view{str.begin(), str.end()});
  }
};

#endif