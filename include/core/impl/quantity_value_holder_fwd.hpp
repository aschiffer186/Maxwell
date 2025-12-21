#ifndef QUANTITY_VALUE_HOLDER_FWD_HPP
#define QUANTITY_VALUE_HOLDER_FWD_HPP

#include "core/quantity.hpp"
#include "core/unit.hpp"
#include <type_traits>

namespace maxwell {
/// \brief Holds a numerical value associated with a physical quantity.
MODULE_EXPORT template <auto Q, typename T = double>
  requires quantity<decltype(Q)>
class quantity_holder;

/// \brief Class template representing the value of a quantity with a particular
/// unit.
///
/// Class template \c quantity_value represents the value of a quantity
/// expressed in a particular unit. Both the quantity and unit of the \c
/// quantity_value are specified at compile-time and are part of the type of the
/// \c quantity_value. By making the quantity and unit part of the type of the
/// \c quantity_value, unit coherence can be verified at compile-time. It is not
/// permited to mix instances of \c quantity_value representing different
/// quantities. However, two \c quantity_value instances may have the same unit,
/// but represent different quantities, e.g. length and wavelength can both be
/// expressed in nanometers, but may represent different quantities. This allows
/// for an additional level of type safety. <br>
///
/// By making the unit part of the type, unit conversions can be performed
/// automatically at compile-time rather than run-time. This makes the library
/// extremely efficient. <br>
///
/// Both the units and the quantity of the \c quantity_value are specified as
/// non-type template parameters (NTTPs).
/// Using NTTPs allows for more natural definitions of custom units and
/// definitions.
///
/// \warning Using an integral type with \c quantity_value will perform
/// truncation when converting units and integer division when performing
/// division.
///
/// \pre \c Q is convertible to the quantity of the specified units \c U.
///       The program is ill-formed if this is violated.
///
/// \tparam U The units of the \c quantity_value.
/// \tparam Q The quantity of the \c quantity_value. Default: the quantity of
/// the units.
/// \tparam T The type of the \c quantity_value. Default: \c double.
MODULE_EXPORT template <auto U, auto Q = U.quantity, typename T = double>
  requires unit<decltype(U)> && quantity<decltype(Q)>
class quantity_value;

namespace _detail {
template <typename> struct is_quantity_value : std::false_type {};

template <auto U, auto Q, typename T>
struct is_quantity_value<quantity_value<U, Q, T>> : std::true_type {};

template <typename T>
constexpr bool is_quantity_value_v =
    is_quantity_value<std::remove_cvref_t<T>>::value;

template <typename T>
concept quantity_value_like = is_quantity_value_v<T>;

template <typename> struct is_quantity_holder : std::false_type {};

template <auto Q, typename T>
struct is_quantity_holder<quantity_holder<Q, T>> : std::true_type {};

template <typename T>
constexpr bool is_quantity_holder_v = is_quantity_holder<T>::value;

template <typename T>
concept quantity_holder_like = is_quantity_value_v<std::remove_cvref_t<T>>;
} // namespace _detail
} // namespace maxwell

#endif