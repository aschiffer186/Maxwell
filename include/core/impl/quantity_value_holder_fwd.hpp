#ifndef QUANTITY_VALUE_HOLDER_FWD_HPP
#define QUANTITY_VALUE_HOLDER_FWD_HPP

#include "core/quantity.hpp"
#include "core/unit.hpp"
#include <type_traits>

namespace maxwell {
MODULE_EXPORT template <auto Q, typename T = double>
  requires quantity<decltype(Q)>
class quantity_holder;

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