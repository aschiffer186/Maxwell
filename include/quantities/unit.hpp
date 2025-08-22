/// \file unit.hpp
/// \brief Definition of class template \c unit_type

#ifndef UNIT_HPP
#define UNIT_HPP

#include <type_traits> // false_type, remove_cvref_t, true_type
#include <utility>     // declval

#include "compile_time_math.hpp"
#include "dimension.hpp"
#include "quantity.hpp"

namespace maxwell {
template <utility::template_string Name, quantity auto Quantity,
          utility::rational auto Multiplier>
struct unit_type {
  constexpr static auto name = Name;
  constexpr static quantity auto quantity = Quantity;
  constexpr static dimension_product auto dimensions = quantity.dimensions;
  constexpr static utility::rational auto multiplier = Multiplier;

  using quantity_rep = std::remove_cvref_t<decltype(quantity)>;

  constexpr static auto base_units() {
    return unit_type<Name, Quantity, utility::one>{};
  }
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

/// \cond
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
/// \endcond

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

template <auto Val, utility::template_string Name> struct make_derived_unit;

template <auto U, utility::template_string Name>
  requires unit<decltype(U)>
struct make_derived_unit<U, Name> {
  using type = unit_type<Name, U.quantity, U.multiplier>;
};

template <auto Q, utility::template_string Name>
  requires quantity<decltype(Q)>
struct make_derived_unit<Q, Name> {
  using type = unit_type<Name, Q, utility::one>;
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

constexpr utility::rational_type<1, 1, 30> quetta_prefix;
constexpr utility::rational_type<1, 1, 27> ronna_prefix;
constexpr utility::rational_type<1, 1, 24> yotta_prefix;
constexpr utility::rational_type<1, 1, 21> zetta_prefix;
constexpr utility::rational_type<1, 1, 18> exa_prefix;
constexpr utility::rational_type<1, 1, 15> peta_prefix;
constexpr utility::rational_type<1, 1, 12> tera_prefix;
constexpr utility::rational_type<1, 1, 9> giga_prefix;
constexpr utility::rational_type<1, 1, 6> mega_prefix;
constexpr utility::rational_type<1, 1, 3> kilo_prefix;
constexpr utility::rational_type<1, 1, 2> hecto_prefix;
constexpr utility::rational_type<1, 1, 1> deca_prefix;
constexpr utility::rational_type<1, 1, -1> deci_prefix;
constexpr utility::rational_type<1, 1, -2> centi_prefix;
constexpr utility::rational_type<1, 1, -3> milli_prefix;
constexpr utility::rational_type<1, 1, -6> micro_prefix;
constexpr utility::rational_type<1, 1, -9> nano_prefix;
constexpr utility::rational_type<1, 1, -12> pico_prefix;
constexpr utility::rational_type<1, 1, -15> femto_prefix;
constexpr utility::rational_type<1, 1, -18> atto_prefix;
constexpr utility::rational_type<1, 1, -21> zepto_prefix;
constexpr utility::rational_type<1, 1, -24> yocto_prefix;
constexpr utility::rational_type<1, 1, -27> ronto_prefix;
constexpr utility::rational_type<1, 1, -30> quecto_prefix;

template <utility::rational auto Prefix, auto U, utility::template_string Name>
struct prefixed_unit : make_derived_unit_t<Prefix * U, Name> {
  using type = make_derived_unit_t<Prefix * U, Name>;
};

template <utility::rational auto Prefix, auto U, utility::template_string Name>
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