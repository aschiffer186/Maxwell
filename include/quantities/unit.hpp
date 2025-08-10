#ifndef UNIT_HPP
#define UNIT_HPP

#include "compile_time_math.hpp"
#include "quantity.hpp"
#include <type_traits>

namespace maxwell {
template <utility::template_string Name, quantity auto Quantity,
          utility::rational auto Multiplier>
struct unit_type {
  constexpr static auto name = Name;
  constexpr static auto quantity = Quantity;
  constexpr static utility::rational auto multiplier = Multiplier;
};

template <utility::template_string Name, quantity auto Quantity,
          utility::rational auto Multiplier>
constexpr unit_type<Name, Quantity, Multiplier>
    underlying_unit(unit_type<Name, Quantity, Multiplier>);

template <typename T>
using underlying_unit_t = decltype(underlying_unit(std::declval<T>()));

/// \cond
namespace _detail {
template <typename, typename = void>
struct has_underlying_unit : std::false_type {};

template <typename T>
struct has_underlying_unit<T, std::void_t<underlying_unit_t<T>>>
    : std::true_type {};
} // namespace _detail
/// \endcond

template <typename T>
concept unit = _detail::has_underlying_unit<std::remove_cvref_t<T>>::value;

namespace _detail {
template <unit LHS, unit RHS> struct unit_product_impl {
  using type = unit_type<LHS::name + utility::template_string{"*"} + RHS::name,
                         LHS::quantity * RHS::quantity,
                         LHS::multiplier * RHS::multiplier>;
};

template <unit LHS, unit RHS> struct unit_quotient_impl {
  using type = unit_type<LHS::name + utility::template_string{"*"} + RHS::name,
                         LHS::quantity * RHS::quantity,
                         LHS::multiplier / RHS::multiplier>;
};
} // namespace _detail

template <unit LHS, unit RHS>
struct unit_product : _detail::unit_product_impl<LHS, RHS>::type {};

template <unit LHS, unit RHS>
struct unit_quotient : _detail::unit_quotient_impl<LHS, RHS>::type {};

template <unit LHS, unit RHS>
consteval unit auto operator*(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  return unit_product<LHS, RHS>{};
}

template <unit RHS>
consteval unit auto operator*(utility::rational auto lhs, RHS) noexcept {
  using LHS = decltype(lhs);
  return unit_type<RHS::name, RHS::quantity, LHS{} * RHS::multiplier>{};
}

template <unit LHS, unit RHS> consteval unit auto operator/(LHS, RHS) noexcept {
  return unit_quotient<LHS, RHS>{};
}

template <auto Q, utility::template_string Name> struct make_base_unit {
  using type = unit_type<Name, Q, utility::one>;
};

template <auto Q, utility::template_string Name>
using make_base_unit_t = make_base_unit<Q, Name>::type;

template <auto FromUnit, auto ToUnit>
concept unit_convertible_to =
    quantity_convertible_to<FromUnit.quantity, ToUnit.quantity>;

template <unit From, unit To>
constexpr double conversion_factor(From, To) noexcept {
  return static_cast<double>(To::multiplier) /
         static_cast<double>(From::multiplier);
}

template <auto U>
concept unitless = false;
} // namespace maxwell

#endif