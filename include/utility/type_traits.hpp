/// \file type_traits.hpp
/// \brief Common type trait and template metaprogramming utilities.

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#ifndef MAXWELL_MODULES
#include <concepts>
#include <type_traits>
#endif

#include "config.hpp"

/// \namespace maxwell::utility
/// \brief Namespace for uility classes.
namespace maxwell::utility {
/// \brief Force expression to be evaluated at compile-time
///
/// Forces an expression to be treated as a constant expression.
/// \tparam Value The value of the expression to treat as a constant expression.
MODULE_EXPORT template <auto Value> constexpr auto as_constant = Value;

MODULE_EXPORT template <typename T, typename U>
concept similar = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
} // namespace maxwell::utility

namespace maxwell {
/// \brief Trait to determine if conversions from \c std::chrono::duration
/// should be enabled.
///
/// The \c enable_chrono_conversions trait helps determine if a quantity
/// allows conversions to/from \c std::chrono::duration types.
/// By default, \c enable_chrono_conversions<Q>::value is \c false.
/// This template can be specialized to inherit from \c std::true_type instead
/// to enable to conversions.
///
/// \tparam Q The quantity to enable
MODULE_EXPORT template <auto Q>
struct enable_chrono_conversions : std::false_type {};

/// \brief Helper variable template for \c enable_chrono_conversions.
MODULE_EXPORT template <auto Q>
constexpr bool enable_chrono_conversions_v =
    enable_chrono_conversions<Q>::value;

/// \brief Trait to return the units of a quantity
///
/// Returns the units of a quantity.
/// This helps prevent formatting functions from being dependent
/// on quantity headers.
///
/// \tparam Q The quantity to find the units of.
MODULE_EXPORT template <typename Q> constexpr auto units_of = Q::units;
} // namespace maxwell

#endif