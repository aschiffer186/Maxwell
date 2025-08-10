/// \file type_traits.hpp
/// \brief Common type trait and template metaprogramming utilities.

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <concepts>
#include <type_traits>

/// \namespace maxwell::utility
/// \brief Namespace for uility classes.
namespace maxwell::utility {
/// \brief Force expression to be evaluated at compile-time
///
/// Forces an expression to be treated as a constant expression.
/// \tparam Value The value of the expression to treat as a constant expression.
template <auto Value> constexpr auto as_constant = Value;

template <typename T, typename U>
concept similar = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
} // namespace maxwell::utility

#endif