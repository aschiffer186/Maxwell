/// \file unit.hpp
/// \brief Definition of class template \c unit_type

#ifndef UNIT_HPP
#define UNIT_HPP

#include <type_traits> // false_type, remove_cvref_t, true_type
#include <utility>     // declval

#include "core/scale.hpp"
#include "dimension.hpp"
#include "quantity.hpp"
#include "utility/compile_time_math.hpp"

namespace maxwell {
template <utility::template_string Name, quantity auto Quantity,
          auto Multiplier, auto Reference = 0.0,
          typename Scale = linear_scale_type>
struct unit_type;

template <utility::template_string Name, quantity auto Quantity,
          auto Multiplier, auto Reference = 0.0, typename Scale>
constexpr unit_type<Name, Quantity, Multiplier, Reference, Scale>
    underlying_unit(unit_type<Name, Quantity, Multiplier, Reference, Scale>);

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

template <utility::template_string Name, quantity auto Quantity,
          auto Multiplier, auto Reference, typename Scale>
struct unit_type {
  constexpr static auto name = Name;
  constexpr static quantity auto quantity = Quantity;
  constexpr static dimension_product auto dimensions = quantity.dimensions;
  constexpr static auto multiplier = Multiplier;
  constexpr static auto reference = Reference;
  constexpr static Scale scale;

  using quantity_rep = std::remove_cvref_t<decltype(quantity)>;

  constexpr static auto base_units() {
    return unit_type<Name, Quantity, utility::one>{};
  }
};

constexpr bool operator==(unit auto lhs, unit auto rhs) noexcept {
  return std::same_as<decltype(lhs), decltype(rhs)>;
}

/// \cond
namespace _detail {
template <unit LHS, unit RHS> struct unit_product_impl {
  using type = unit_type<LHS::name + utility::template_string{"*"} + RHS::name,
                         LHS::quantity * RHS::quantity,
                         LHS::multiplier * RHS::multiplier,
                         LHS::reference * RHS::multiplier + RHS::reference>;
};

template <unit LHS, unit RHS> struct unit_quotient_impl {
  using type = unit_type<LHS::name + utility::template_string{"*"} + RHS::name,
                         LHS::quantity / RHS::quantity,
                         LHS::multiplier / RHS::multiplier,
                         LHS::reference - RHS::reference / RHS::multiplier>;
};

template <unit U> struct unit_sqrt_impl {
  using type =
      unit_type<utility::template_string{"sqrt("} + U::name +
                    utility::template_string{"}"},
                sqrt(U::quantity), utility::sqrt(U::multiplier), U::reference>;
};

template <unit U, auto R>
  requires utility::rational<decltype(R)>
struct unit_pow_impl {
  using type = unit_type<utility::template_string{"pow("} + U::name +
                             utility::template_string{", "} +
                             utility::template_string{")"},
                         pow<R>(U::quantity), utility::pow(U::multiplier, R),
                         utility::pow(U::reference, R)>;
};
} // namespace _detail
/// \endcond

template <unit LHS, unit RHS>
struct unit_product : _detail::unit_product_impl<LHS, RHS>::type {};

template <unit LHS, unit RHS>
struct unit_quotient : _detail::unit_quotient_impl<LHS, RHS>::type {};

template <unit U> struct unit_sqrt : _detail::unit_sqrt_impl<U>::type {};

template <unit U, auto R>
  requires utility::rational<decltype(R)>
struct unit_pow : _detail::unit_pow_impl<U, R>::type {};

template <unit LHS, unit RHS>
consteval unit auto operator*(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  return unit_product<LHS, RHS>{};
}

template <auto Value, unit RHS>
constexpr unit auto operator*(utility::value_type<Value> lhs, RHS) noexcept {
  return unit_type<RHS::name, RHS::quantity, lhs.value * RHS::multiplier>{};
}

template <unit U> constexpr unit auto sqrt(U /*unit*/) noexcept {
  return unit_sqrt<U>{};
}

template <unit U, auto R>
  requires utility::rational<decltype(R)>
constexpr unit auto pow(U /*unit*/) noexcept {
  return unit_pow<U, R>{};
}

template <unit U, std::intmax_t R>
  requires utility::rational<decltype(R)>
constexpr unit auto pow(U /*unit*/) noexcept {
  return unit_pow<U, utility::rational_type<R, 1>{}>{};
}

template <auto Value>
constexpr unit auto operator+(unit auto lhs,
                              utility::value_type<Value> rhs) noexcept {
  return unit_type<lhs.name, lhs.quantity, lhs.multiplier,
                   lhs.multiplier * lhs.reference + rhs.value>{};
}

template <auto Value>
constexpr unit auto operator-(unit auto lhs,
                              utility::value_type<Value> rhs) noexcept {
  return unit_type<lhs.name, lhs.quantity, lhs.multiplier,
                   lhs.multiplier * lhs.reference - rhs.value>{};
}

template <unit LHS, unit RHS> constexpr unit auto operator/(LHS, RHS) noexcept {
  return unit_quotient<LHS, RHS>{};
}

template <auto Q, utility::template_string Name> struct base_unit_impl {
  using type = unit_type<Name, Q, 1.0>;
};

template <auto Q, utility::template_string Name>
using base_unit = base_unit_impl<Q, Name>::type;

template <auto Val, utility::template_string Name> struct derived_unit_impl;

template <auto U, utility::template_string Name>
  requires unit<decltype(U)>
struct derived_unit_impl<U, Name> {
  using type = unit_type<Name, U.quantity, U.multiplier, U.reference>;
};

template <auto Q, utility::template_string Name>
  requires quantity<decltype(Q)>
struct derived_unit_impl<Q, Name> {
  using type = unit_type<Name, Q, 1.0>;
};

template <auto Val, utility::template_string Name>
using derived_unit = derived_unit_impl<Val, Name>::type;

template <auto FromUnit, auto ToUnit>
concept unit_convertible_to =
    dimension_convertible_to<FromUnit.dimensions, ToUnit.dimensions>;

// 1 km in cm = 1 m / 1e-3 km *  100 cm / 1 m

template <unit From, unit To>
constexpr auto conversion_factor(From, To) noexcept -> double {
  return static_cast<double>(To::multiplier) /
         static_cast<double>(From::multiplier);
}

template <unit From, unit To>
constexpr auto conversion_offset(From, To) noexcept -> double {
  if (To::multiplier == 1.0) {
    return static_cast<double>(To::reference - From::reference);
  } else {
    return From::reference - To::reference / To::multiplier;
  }
}

// 1 m -> km = 1e-3 km / 1 m

constexpr double base_to_quetta_prefix{1e-30};
constexpr double base_to_ronna_prefix{1e-27};
constexpr double base_to_yotta_prefix{1e-24};
constexpr double base_to_zetta_prefix{1e-21};
constexpr double base_to_exa_prefix{1e-18};
constexpr double base_to_peta_prefix{1e-15};
constexpr double base_to_tera_prefix{1e-12};
constexpr double base_to_giga_prefix{1e-9};
constexpr double base_to_mega_prefix{1e-6};
constexpr double base_to_kilo_prefix{1e-3};
constexpr double base_to_hecto_prefix{1e-2};
constexpr double base_to_deca_prefix{1e-1};
constexpr double base_to_deci_prefix{1e1};
constexpr double base_to_centi_prefix{1e2};
constexpr double base_to_milli_prefix{1e3};
constexpr double base_to_micro_prefix{1e6};
constexpr double base_to_nano_prefix{1e9};
constexpr double base_to_pico_prefix{1e12};
constexpr double base_to_femto_prefix{1e15};
constexpr double base_to_atto_prefix{1e18};
constexpr double base_to_zepto_prefix{1e21};
constexpr double base_to_yocto_prefix{1e24};
constexpr double base_to_ronto_prefix{1e27};
constexpr double base_to_quecto_prefix{1e30};

template <auto Prefix, auto U, utility::template_string Name>
struct prefixed_unit {
public:
  // clang-format off

  using type = unit_type<Name, U.quantity,
                pow(Prefix, U.quantity.dimension_sum()) *
                    U.multiplier>;
  
  // clang-format on 
};

template <auto Prefix, auto U, utility::template_string Name>
using prefixed_unit_t = prefixed_unit<Prefix, U, Name>::type;

template <auto U>
constexpr unit auto quetta_unit =
    prefixed_unit_t<base_to_quetta_prefix, U, utility::template_string{"Q"} + U.name>{};
template <auto U>
constexpr unit auto ronna_unit =
    prefixed_unit_t<base_to_ronna_prefix, U, utility::template_string{"R"} + U.name>{};
template <auto U>
constexpr unit auto yotta_unit =
    prefixed_unit_t<base_to_yotta_prefix, U, utility::template_string{"Y"} + U.name>{};
template <auto U>
constexpr unit auto zetta_unit =
    prefixed_unit_t<base_to_zetta_prefix, U, utility::template_string{"Z"} + U.name>{};
template <auto U>
constexpr unit auto exa_unit =
    prefixed_unit_t<base_to_exa_prefix, U, utility::template_string{"E"} + U.name>{};
template <auto U>
constexpr unit auto peta_unit =
    prefixed_unit_t<base_to_peta_prefix, U, utility::template_string{"P"} + U.name>{};
template <auto U>
constexpr unit auto tera_unit =
    prefixed_unit_t<base_to_tera_prefix, U, utility::template_string{"T"} + U.name>{};
template <auto U>
constexpr unit auto giga_unit =
    prefixed_unit_t<base_to_giga_prefix, U, utility::template_string{"G"} + U.name>{};
template <auto U>
constexpr unit auto mega_unit =
    prefixed_unit_t<base_to_mega_prefix, U, utility::template_string{"M"} + U.name>{};
template <auto U>
constexpr unit auto kilo_unit =
    prefixed_unit_t<base_to_kilo_prefix, U, utility::template_string{"k"} + U.name>{};
template <auto U>
constexpr unit auto hecto_unit =
    prefixed_unit_t<base_to_hecto_prefix, U, utility::template_string{"h"} + U.name>{};
template <auto U>
constexpr unit auto deca_unit =
    prefixed_unit_t<base_to_deca_prefix, U, utility::template_string{"da"} + U.name>{};
template <auto U>
constexpr unit auto deci_unit =
    prefixed_unit_t<base_to_deci_prefix, U, utility::template_string{"d"} + U.name>{};
template <auto U>
constexpr unit auto centi_unit =
    prefixed_unit_t<base_to_centi_prefix, U, utility::template_string{"c"} + U.name>{};
template <auto U>
constexpr unit auto milli_unit =
    prefixed_unit_t<base_to_milli_prefix, U, utility::template_string{"m"} + U.name>{};
template <auto U>
constexpr unit auto micro_unit =
    prefixed_unit_t<base_to_micro_prefix, U, utility::template_string{"μ"} + U.name>{};
template <auto U>
constexpr unit auto nano_unit =
    prefixed_unit_t<base_to_nano_prefix, U, utility::template_string{"n"} + U.name>{};
template <auto U>
constexpr unit auto pico_unit =
    prefixed_unit_t<base_to_pico_prefix, U, utility::template_string{"p"} + U.name>{};
template <auto U>
constexpr unit auto femto_unit =
    prefixed_unit_t<base_to_femto_prefix, U, utility::template_string{"f"} + U.name>{};
template <auto U>
constexpr unit auto atto_unit =
    prefixed_unit_t<base_to_atto_prefix, U, utility::template_string{"a"} + U.name>{};
template <auto U>
constexpr unit auto zepto_unit =
    prefixed_unit_t<base_to_zepto_prefix, U, utility::template_string{"z"} + U.name>{};
template <auto U>
constexpr unit auto yocto_unit =
    prefixed_unit_t<base_to_yocto_prefix, U, utility::template_string{"y"} + U.name>{};
template <auto U>
constexpr unit auto ronto_unit =
    prefixed_unit_t<base_to_ronto_prefix, U, utility::template_string{"r"} + U.name>{};
template <auto U>
constexpr unit auto quecto_unit =
    prefixed_unit_t<base_to_quecto_prefix, U, utility::template_string{"q"} + U.name>{};

template <auto U>
concept unitless = quantity_convertible_to<U.quantity, number> && !_detail::has_derived_base<decltype(U.quantity)>::value;

template <auto Lhs, auto Rhs>
concept unit_comparable_with = unit_convertible_to<Lhs, Rhs> && Lhs.reference == Rhs.reference;

template <auto Lhs, auto Rhs> 
concept unit_addable_with = unit_convertible_to<Lhs, Rhs> && Lhs.reference == Rhs.reference; 

template <auto Lhs, auto Rhs> 
concept unit_subtractable_from = unit_convertible_to<Lhs, Rhs> && Lhs.reference == Rhs.reference;
} // namespace maxwell

#endif