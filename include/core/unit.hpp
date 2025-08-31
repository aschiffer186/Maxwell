/// \file unit.hpp
/// \brief Definition of class template \c unit_type

#ifndef UNIT_HPP
#define UNIT_HPP

#include <concepts>
#include <type_traits> // false_type, remove_cvref_t, true_type
#include <utility>     // declval

#include "dimension.hpp"
#include "quantity.hpp"
#include "utility/compile_time_math.hpp"

namespace maxwell {
template <utility::template_string Name, quantity auto Quantity,
          auto Multiplier>
struct unit_type;

template <utility::template_string Name, quantity auto Quantity,
          auto Multiplier>
constexpr unit_type<Name, Quantity, Multiplier>
underlying_unit(const unit_type<Name, Quantity, Multiplier>&);

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
          auto Multiplier>
struct unit_type {
  constexpr static auto name = Name;
  constexpr static quantity auto quantity = Quantity;
  constexpr static dimension_product auto dimensions = quantity.dimensions;
  constexpr static auto multiplier = Multiplier;

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
                         LHS::multiplier * RHS::multiplier>;
};

template <unit LHS, unit RHS> struct unit_quotient_impl {
  using type = unit_type<LHS::name + utility::template_string{"*"} + RHS::name,
                         LHS::quantity / RHS::quantity,
                         LHS::multiplier / RHS::multiplier>;
};
} // namespace _detail
/// \endcond

template <unit LHS, unit RHS>
struct unit_product : _detail::unit_product_impl<LHS, RHS>::type {};

template <unit LHS, unit RHS>
struct unit_quotient : _detail::unit_quotient_impl<LHS, RHS>::type {};

template <unit LHS, unit RHS>
consteval unit auto operator*(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  return unit_product<LHS, RHS>{};
}

template <auto Value, unit RHS>
constexpr unit auto operator*(utility::value_type<Value> lhs, RHS) noexcept {
  return unit_type<RHS::name, RHS::quantity, lhs.value * RHS::multiplier>{};
}

template <unit LHS, unit RHS> constexpr unit auto operator/(LHS, RHS) noexcept {
  return unit_quotient<LHS, RHS>{};
}

template <auto Q, utility::template_string Name> struct make_base_unit {
  using type = unit_type<Name, Q, 1.0>;
};

template <auto Q, utility::template_string Name>
using make_base_unit_t = make_base_unit<Q, Name>::type;

template <auto Val, utility::template_string Name> struct make_derived_unit;

template <auto U, utility::template_string Name>
  requires unit<decltype(U)>
struct make_derived_unit<U, Name> {
  using type = unit_type<Name, U.quantity, U.multiplier>;
};

template <auto Q, utility::template_string Name>
  requires quantity<decltype(Q)>
struct make_derived_unit<Q, Name> {
  using type = unit_type<Name, Q, 1.0>;
};

template <auto Val, utility::template_string Name>
using make_derived_unit_t = make_derived_unit<Val, Name>::type;

template <auto FromUnit, auto ToUnit>
concept unit_convertible_to =
    dimension_convertible_to<FromUnit.dimensions, ToUnit.dimensions>;

template <unit From, unit To>
constexpr double conversion_factor(From, To) noexcept {
  return static_cast<double>(To::multiplier) /
         static_cast<double>(From::multiplier);
}

constexpr double quetta_prefix{1e30};
constexpr double ronna_prefix{1e27};
constexpr double yotta_prefix{1e24};
constexpr double zetta_prefix{1e21};
constexpr double exa_prefix{1e18};
constexpr double peta_prefix{1e15};
constexpr double tera_prefix{1e12};
constexpr double giga_prefix{1e9};
constexpr double mega_prefix{1e6};
constexpr double kilo_prefix{1e3};
constexpr double hecto_prefix{1e2};
constexpr double deca_prefix{1e1};
constexpr double deci_prefix{1e-1};
constexpr double centi_prefix{1e-2};
constexpr double milli_prefix{1e-3};
constexpr double micro_prefix{1e-6};
constexpr double nano_prefix{1e-9};
constexpr double pico_prefix{1e-12};
constexpr double femto_prefix{1e-15};
constexpr double atto_prefix{1e-18};
constexpr double zepto_prefix{1e-21};
constexpr double yocto_prefix{1e-24};
constexpr double ronto_prefix{1e-27};
constexpr double quecto_prefix{1e-30};

template <auto Prefix, auto U, utility::template_string Name>
struct prefixed_unit {
public:
  // clang-format off

  using type = unit_type<Name, U.quantity,
                pow(Prefix, U.quantity.dimensions.dimension_exponent_sum()) *
                    U.multiplier>;
  
  // clang-format on 
};

template <auto Prefix, auto U, utility::template_string Name>
using prefixed_unit_t = prefixed_unit<Prefix, U, Name>::type;

template <auto U>
constexpr unit auto quetta_unit =
    prefixed_unit_t<quetta_prefix, U, utility::template_string{"Q"} + U.name>{};
template <auto U>
constexpr unit auto ronna_unit =
    prefixed_unit_t<ronna_prefix, U, utility::template_string{"R"} + U.name>{};
template <auto U>
constexpr unit auto yotta_unit =
    prefixed_unit_t<yotta_prefix, U, utility::template_string{"Y"} + U.name>{};
template <auto U>
constexpr unit auto zetta_unit =
    prefixed_unit_t<zetta_prefix, U, utility::template_string{"Z"} + U.name>{};
template <auto U>
constexpr unit auto exa_unit =
    prefixed_unit_t<exa_prefix, U, utility::template_string{"E"} + U.name>{};
template <auto U>
constexpr unit auto peta_unit =
    prefixed_unit_t<peta_prefix, U, utility::template_string{"P"} + U.name>{};
template <auto U>
constexpr unit auto tera_unit =
    prefixed_unit_t<tera_prefix, U, utility::template_string{"T"} + U.name>{};
template <auto U>
constexpr unit auto giga_unit =
    prefixed_unit_t<giga_prefix, U, utility::template_string{"G"} + U.name>{};
template <auto U>
constexpr unit auto mega_unit =
    prefixed_unit_t<mega_prefix, U, utility::template_string{"M"} + U.name>{};
template <auto U>
constexpr unit auto kilo_unit =
    prefixed_unit_t<kilo_prefix, U, utility::template_string{"k"} + U.name>{};
template <auto U>
constexpr unit auto hecto_unit =
    prefixed_unit_t<hecto_prefix, U, utility::template_string{"h"} + U.name>{};
template <auto U>
constexpr unit auto deca_unit =
    prefixed_unit_t<deca_prefix, U, utility::template_string{"da"} + U.name>{};
template <auto U>
constexpr unit auto deci_unit =
    prefixed_unit_t<deci_prefix, U, utility::template_string{"d"} + U.name>{};
template <auto U>
constexpr unit auto centi_unit =
    prefixed_unit_t<centi_prefix, U, utility::template_string{"c"} + U.name>{};
template <auto U>
constexpr unit auto milli_unit =
    prefixed_unit_t<milli_prefix, U, utility::template_string{"m"} + U.name>{};
template <auto U>
constexpr unit auto micro_unit =
    prefixed_unit_t<micro_prefix, U, utility::template_string{"μ"} + U.name>{};
template <auto U>
constexpr unit auto nano_unit =
    prefixed_unit_t<nano_prefix, U, utility::template_string{"n"} + U.name>{};
template <auto U>
constexpr unit auto pico_unit =
    prefixed_unit_t<pico_prefix, U, utility::template_string{"p"} + U.name>{};
template <auto U>
constexpr unit auto femto_unit =
    prefixed_unit_t<femto_prefix, U, utility::template_string{"f"} + U.name>{};
template <auto U>
constexpr unit auto atto_unit =
    prefixed_unit_t<atto_prefix, U, utility::template_string{"a"} + U.name>{};
template <auto U>
constexpr unit auto zepto_unit =
    prefixed_unit_t<zepto_prefix, U, utility::template_string{"z"} + U.name>{};
template <auto U>
constexpr unit auto yocto_unit =
    prefixed_unit_t<yocto_prefix, U, utility::template_string{"y"} + U.name>{};
template <auto U>
constexpr unit auto ronto_unit =
    prefixed_unit_t<ronto_prefix, U, utility::template_string{"r"} + U.name>{};
template <auto U>
constexpr unit auto quecto_unit =
    prefixed_unit_t<quecto_prefix, U, utility::template_string{"q"} + U.name>{};

template <auto U>
concept unitless =
    std::same_as<typename decltype(U)::quantity_rep, decltype(number)>;
} // namespace maxwell

#endif