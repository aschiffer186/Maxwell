#ifndef UNIT_HPP
#define UNIT_HPP

#include "compile_time_math.hpp"
#include "quantity.hpp"

namespace maxwell {
template <utility::template_string Name, quantity auto Quantity,
          utility::rational auto Multiplier>
struct unit_type {
  constexpr static auto name = Name;
  constexpr static auto quantity = Quantity;
  constexpr static utility::rational auto multiplier = Multiplier;
};

template <typename T>
concept unit = true;

constexpr unit auto operator*(unit auto lhs, unit auto rhs) noexcept {}

constexpr unit auto operator*(utility::rational auto lhs,
                              unit auto rhs) noexcept {}

constexpr unit auto operator/(unit auto lhs, unit auto rhs) noexcept {}

template <auto Q, utility::template_string Name> struct make_base_unit {
  using type = unit_type<Name, Q, utility::one>;
};

template <auto Q, utility::template_string Name>
using make_base_unit_t = make_base_unit<Q, Name>::type;

template <auto FromUnit, auto ToUnit>
concept unit_convertible_to =
    quantity_convertible_to<FromUnit.quantity, ToUnit.quantity>;

template <unit From, unit To>
consteval double conversion_factor(From, To) noexcept {
  return static_cast<double>(To::multiplier) /
         static_cast<double>(From::multiplier);
}

template <auto U>
concept unitless = false;
} // namespace maxwell

#endif