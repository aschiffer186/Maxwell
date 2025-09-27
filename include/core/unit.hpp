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
/// \brief Represents an instance of a quantity used as a reference.
///
/// Class template \c unit_type represents a particular instance of a quantity
/// that can be used as a reference for measurements. Any quantity can be
/// constructed by multiplying a \c unit_type by a numeric value.
///
/// Units can be a multiple of a base unit, e.g. kilometers are a multiple of
/// 1000 of the base unit meters. This is represented by the \c multiplier
/// template parameter. Specifically, the multiplier is the amount you need to
/// multiply the unit by to represent one of the base unit. In the case of
/// kilometers, the multiplier is 0.001 since one meter is 1/1000th of a
/// kilometer.
///
/// A unit can also have a reference value, which is useful for units that do
/// not start at zero. An example is Celsisus which is offset by 273.15 from the
/// Kelvin scale.
///
/// \tparam Name The name of the unit.
/// \tparam Quantity The quantity the unit is a reference for.
/// \tparam Multiplier The multiplier of the unit relative to the base unit of
/// the quantity.
/// \tparam Reference The reference value of the unit. Defaults to 0.0.
/// \tparam Scale The scale type of the unit. Defaults to \c linear_scale_type
MODULE_EXPORT template <
    utility::template_string Name, quantity auto Quantity, double Multiplier,
    auto Reference = 0.0, typename Scale = linear_scale_type,
    dimension_product auto DimForMultiplier = std::conditional_t<
        Quantity.dimensions == dimension_one,
        dimension_product_type<dimension_type<"[]", utility::one>>,
        dimension_product_type<>>{}>
struct unit_type;

/// \cond
namespace _detail {
template <utility::template_string Name, quantity auto Quantity,
          double Multiplier, auto Reference = 0.0, typename Scale,
          auto DimForMultiplier>
constexpr unit_type<Name, Quantity, Multiplier, Reference, Scale>
    underlying_unit(unit_type<Name, Quantity, Multiplier, Reference, Scale,
                              DimForMultiplier>);

template <typename T>
using underlying_unit_t = decltype(underlying_unit(std::declval<T>()));

template <typename, typename = void>
struct has_underlying_unit : std::false_type {};

template <typename T>
struct has_underlying_unit<T, std::void_t<underlying_unit_t<T>>>
    : std::true_type {};
} // namespace _detail
/// \endcond

/// \brief Concept indicating a type models an instantiation of \c unit_type.
///
/// Concept indicating a type models an instantiation of \c unit_type, ignoring
/// an \c cvref qualifiers
///
/// \tparam T The type to check.
MODULE_EXPORT template <typename T>
concept unit = _detail::has_underlying_unit<std::remove_cvref_t<T>>::value;

template <utility::template_string Name, quantity auto Quantity,
          double Multiplier, auto Reference, typename Scale,
          dimension_product auto DimForMultiplier>
struct unit_type {
  /// The name of the unit
  constexpr static auto name = Name;
  /// The quantity the unit is a reference for.
  constexpr static quantity auto quantity = Quantity;
  /// The dimensions of the unit
  constexpr static dimension_product auto dimensions = quantity.dimensions;
  /// Dimension product used for helping to calculate multiplier
  constexpr static dimension_product auto dim_for_multiplier = DimForMultiplier;
  /// The multiplier of the unit relative to the base unit of the quantity.
  constexpr static auto multiplier = Multiplier;
  /// The reference value of the unit.
  constexpr static auto reference = Reference;
  /// The scale type of the unit.
  constexpr static Scale scale;

  using scale_type = Scale;
  /// The representation type of the quantity the unit is a reference for.
  using quantity_rep = std::remove_cvref_t<decltype(quantity)>;

  /// \brief Returns the base unit for the unit.
  ///
  /// Returns the base unit for the units. This is a unit that
  /// represents the same quantity but has a multiplier of 1.0 and a
  /// reference of 0.0.
  ///
  /// \return The base unit for the unit.
  constexpr static auto base_units() {
    return unit_type<Name, Quantity, 1.0>{};
  }

  /// \brief Returns the sum of the exponents of the unit's dimensions.
  ///
  /// Returns the sum of the exponents of the unit's dimensions. Note,
  /// if this is a derived units, this function will never return zero
  /// even if the quantity has dimensions of number.
  ///
  /// \brief Return the sum of the exponents of the quantity's units.
  constexpr static utility::rational auto dimension_sum() {
    if constexpr (DimForMultiplier == dimension_product_type<>{}) {
      return quantity.dimension_sum();
    } else {
      return DimForMultiplier.dimension_exponent_sum();
    }
  }
};

/// \brief Compares two units for equality.
///
/// Compares two units for equality. Two units are equal if they represent the
/// same quantity and have the same multiplier and reference.
///
/// \warning This function is primary intended for library testing and
/// development. It may not behave as expected in user-facing code.
///
/// \param lhs The left-hand side unit.
/// \param rhs The right-hand side unit.
/// \return \c true if the units are equal, \c false otherwise.
[[deprecated("The use of operator== is deprecated in user facing code; use "
             "convertible_to concets")]] constexpr bool
operator==(unit auto lhs, unit auto rhs) noexcept {
  return lhs.quantity == rhs.quantity && lhs.multiplier == rhs.multiplier &&
         lhs.reference == rhs.reference;
}

/// \cond
namespace _detail {
template <unit LHS, unit RHS> struct unit_product_impl {
  using type = unit_type<LHS::name + utility::template_string{"*"} + RHS::name,
                         LHS::quantity * RHS::quantity,
                         LHS::multiplier * RHS::multiplier,
                         LHS::reference * RHS::multiplier + RHS::reference,
                         typename LHS::scale_type,
                         LHS::dim_for_multiplier * RHS::dim_for_multiplier>;
};

template <unit LHS, unit RHS> struct unit_quotient_impl {
  using type = unit_type<LHS::name + utility::template_string{"*"} + RHS::name,
                         LHS::quantity / RHS::quantity,
                         LHS::multiplier / RHS::multiplier,
                         LHS::reference - RHS::reference / RHS::multiplier,
                         typename LHS::scale_type,
                         LHS::dim_for_multiplier / RHS::dim_for_multiplier>;
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
  using type = unit_type<
      utility::template_string{"pow("} + U::name +
          utility::template_string{", "} + utility::template_string{")"},
      pow<R>(U::quantity), utility::pow(U::multiplier, R), U::reference>;
};
template <unit LHS, unit RHS>
struct unit_product : _detail::unit_product_impl<LHS, RHS>::type {};

template <unit LHS, unit RHS>
struct unit_quotient : _detail::unit_quotient_impl<LHS, RHS>::type {};

template <unit U> struct unit_sqrt : _detail::unit_sqrt_impl<U>::type {};

template <unit U, auto R>
  requires utility::rational<decltype(R)>
struct unit_pow : _detail::unit_pow_impl<U, R>::type {};
} // namespace _detail
/// \endcond

/// \brief Multiplies two units.
///
/// Computes the product of two units. The resulting unit has a quantity that is
/// the product of the quantities of the two units, a multiplier that is the
/// product of the multipliers of the two units, and a reference that is
/// computed based on the references and multipliers of the two units.
///
/// \tparam LHS The type of the left-hand side unit.
/// \tparam RHS The type of the right-hand side unit.
/// \param lhs The left-hand side unit.
/// \param rhs The right-hand side unit.
/// \return The product of the two units.
MODULE_EXPORT template <unit LHS, unit RHS>
consteval unit auto operator*(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  return _detail::unit_product<LHS, RHS>{};
}

/// \brief Multiplies a unit by a numeric value.
///
/// Computes the product of a unit and a numeric value. The resulting unit has
/// the same quantity as the unit, a multiplier that is the product of the
/// multiplier of the unit and the numeric value, and the same reference as the
/// unit.
///
/// \tparam Value The numeric value.
/// \tparam RHS The type of the right-hand side unit.
/// \param lhs The numeric value.
/// \param rhs The right-hand side unit.
/// \return The product of the numeric value and the unit.
MODULE_EXPORT template <auto Value, unit RHS>
constexpr unit auto operator*(utility::value_type<Value> lhs, RHS) noexcept {
  return unit_type<RHS::name, RHS::quantity, lhs.value * RHS::multiplier,
                   RHS::reference>{};
}

/// \brief Computes the square root of a unit.
///
/// Computes the square root of a unit. The resulting unit has a quantity that
/// is the square root of the quantity of the unit, a multiplier that is the
/// square root of the multiplier of the unit, and the same reference as the
/// unit.
///
/// \tparam U The type of the unit.
/// \param unit The unit to compute the square root of.
/// \return The square root of the unit.
MODULE_EXPORT template <unit U> constexpr unit auto sqrt(U /*unit*/) noexcept {
  return _detail::unit_sqrt<U>{};
}

/// \brief Raises a unit to a rational power.
///
/// Raises a unit to a rational power. The resulting unit has a quantity that
/// is the quantity of the unit raised to the power, a multiplier that is the
/// multiplier of the unit raised to the power, and the reference of the unit
/// raised to the power.
///
/// \note The exponent must be known at compile time.
///
/// \tparam U The type of the unit.
/// \tparam R The exponent to raise the unit to.
/// \param unit The unit to raise to the power.
/// \return The unit raised to the power.
MODULE_EXPORT template <auto R, unit U>
  requires utility::rational<decltype(R)>
constexpr unit auto pow(U /*unit*/) noexcept {
  return _detail::unit_pow<U, R>{};
}

/// \brief Raises a unit to an integral power.
///
/// Raises a unit to an integral power. The resulting unit has a quantity that
/// is the quantity of the unit raised to the power, a multiplier that is the
/// multiplier of the unit raised to the power, and the reference of the unit
/// raised to the power.
///
/// \note The exponent must be known at compile time.
///
/// \tparam U The type of the unit.
/// \tparam R The exponent to raise the unit to.
/// \param unit The unit to raise to the power.
/// \return The unit raised to the power.
MODULE_EXPORT template <std::intmax_t R, unit U>
constexpr unit auto pow(U /*unit*/) noexcept {
  return _detail::unit_pow<U, utility::rational_type<R, 1>{}>{};
}

/// \brief Changes the reference of a unit.
///
/// Changes the reference of a unit by adding the specified numerical value to
/// the current reference of the unit.
///
/// \tparam Value The numerical value to add to the reference.
/// \param lhs The left-hand side unit.
/// \param rhs The numerical value to add to the reference.
/// \return A new unit with the updated reference.
MODULE_EXPORT template <auto Value>
constexpr unit auto operator+(unit auto lhs,
                              utility::value_type<Value> rhs) noexcept {
  return unit_type<lhs.name, lhs.quantity, lhs.multiplier,
                   lhs.multiplier * lhs.reference + rhs.value>{};
}

/// \brief Changes the reference of a unit.
///
/// Changes the reference of a unit by subtracting the specified numerical value
/// from the current reference of the unit.
///
/// \tparam Value The numerical value to subtract from the reference.
/// \param lhs The left-hand side unit.
/// \param rhs The numerical value to subtract from the reference.
/// \return A new unit with the updated reference.
MODULE_EXPORT template <auto Value>
constexpr unit auto operator-(unit auto lhs,
                              utility::value_type<Value> rhs) noexcept {
  return unit_type<lhs.name, lhs.quantity, lhs.multiplier,
                   lhs.multiplier * lhs.reference - rhs.value>{};
}

/// \brief Divides two units.
///
/// Computes the quotient of two units. The resulting unit has a quantity that
/// is the quotient of the quantities of the two units, a multiplier that is the
/// quotient of the multipliers of the two units, and a reference that is
/// computed based on the references and multipliers of the two units.
///
/// \tparam LHS The type of the left-hand side unit.
/// \tparam RHS The type of the right-hand side unit.
/// \param lhs The left-hand side unit.
/// \param rhs The right-hand side unit.
/// \return The quotient of the two units.
MODULE_EXPORT template <unit LHS, unit RHS>
constexpr unit auto operator/(LHS, RHS) noexcept {
  return _detail::unit_quotient<LHS, RHS>{};
}

/// \cond
namespace _detail {
template <auto Q, utility::template_string Name> struct base_unit_impl {
  using type = unit_type<Name, Q, 1.0>;
};

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
} // namespace _detail
/// \endcond

/// \brief Creates a new base unit.
///
/// Creates a named base unit that serves as a reference for the quantity.
/// Derived units can be created from the base unit using prefixes or
/// arithmetic operations.
///
/// \tparam Q The quantity the base unit is a reference for.
/// \tparam Name The name of the base unit.
MODULE_EXPORT template <auto Q, utility::template_string Name>
using base_unit = _detail::base_unit_impl<Q, Name>::type;

/// \brief Creates a new derived unit.
///
/// Creates a named derived unit from either an existing unit or a quantity.
/// The derived unit will have the same quantity, multiplier, and reference as
/// the existing unit if a unit is provided. If a quantity is provided, the
/// derived unit will have a multiplier of 1.0 and a reference of 0.0.
///
/// \tparam Val The existing unit or quantity to derive the new unit from.
/// \tparam Name The name of the derived unit.
MODULE_EXPORT template <auto Val, utility::template_string Name>
using derived_unit = _detail::derived_unit_impl<Val, Name>::type;

MODULE_EXPORT template <unit From, unit To>
constexpr auto conversion_factor(From, To) noexcept -> double {
  return static_cast<double>(To::multiplier) /
         static_cast<double>(From::multiplier);
}

MODULE_EXPORT template <unit From, unit To>
constexpr auto conversion_offset(From, To) noexcept -> double {
  if (To::multiplier == 1.0 && From::multiplier == 1.0) {
    return static_cast<double>(To::reference - From::reference);
  } else if (From::multiplier == 1.0) {
    return To::reference - To::multiplier * From::reference;
  } else {
    return To::reference - From::reference / From::multiplier;
  }
}

MODULE_EXPORT constexpr double base_to_quetta_prefix{1e-30};
MODULE_EXPORT constexpr double base_to_ronna_prefix{1e-27};
MODULE_EXPORT constexpr double base_to_yotta_prefix{1e-24};
MODULE_EXPORT constexpr double base_to_zetta_prefix{1e-21};
MODULE_EXPORT constexpr double base_to_exa_prefix{1e-18};
MODULE_EXPORT constexpr double base_to_peta_prefix{1e-15};
MODULE_EXPORT constexpr double base_to_tera_prefix{1e-12};
MODULE_EXPORT constexpr double base_to_giga_prefix{1e-9};
MODULE_EXPORT constexpr double base_to_mega_prefix{1e-6};
MODULE_EXPORT constexpr double base_to_kilo_prefix{1e-3};
MODULE_EXPORT constexpr double base_to_hecto_prefix{1e-2};
MODULE_EXPORT constexpr double base_to_deca_prefix{1e-1};
MODULE_EXPORT constexpr double base_to_deci_prefix{1e1};
MODULE_EXPORT constexpr double base_to_centi_prefix{1e2};
MODULE_EXPORT constexpr double base_to_milli_prefix{1e3};
MODULE_EXPORT constexpr double base_to_micro_prefix{1e6};
MODULE_EXPORT constexpr double base_to_nano_prefix{1e9};
MODULE_EXPORT constexpr double base_to_pico_prefix{1e12};
MODULE_EXPORT constexpr double base_to_femto_prefix{1e15};
MODULE_EXPORT constexpr double base_to_atto_prefix{1e18};
MODULE_EXPORT constexpr double base_to_zepto_prefix{1e21};
MODULE_EXPORT constexpr double base_to_yocto_prefix{1e24};
MODULE_EXPORT constexpr double base_to_ronto_prefix{1e27};
MODULE_EXPORT constexpr double base_to_quecto_prefix{1e30};

template <auto Prefix, auto U, utility::template_string Name>
struct prefixed_unit {
public:
  // clang-format off

  using type = unit_type<Name, U.quantity,
                pow(Prefix, U.dimension_sum()) *
                    U.multiplier>;
  
  // clang-format on 
};

MODULE_EXPORT template <auto Prefix, auto U, utility::template_string Name>
using prefixed_unit_t = prefixed_unit<Prefix, U, Name>::type;

MODULE_EXPORT template <auto U>
constexpr unit auto quetta_unit =
    prefixed_unit_t<base_to_quetta_prefix, U, utility::template_string{"Q"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto ronna_unit =
    prefixed_unit_t<base_to_ronna_prefix, U, utility::template_string{"R"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto yotta_unit =
    prefixed_unit_t<base_to_yotta_prefix, U, utility::template_string{"Y"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto zetta_unit =
    prefixed_unit_t<base_to_zetta_prefix, U, utility::template_string{"Z"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto exa_unit =
    prefixed_unit_t<base_to_exa_prefix, U, utility::template_string{"E"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto peta_unit =
    prefixed_unit_t<base_to_peta_prefix, U, utility::template_string{"P"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto tera_unit =
    prefixed_unit_t<base_to_tera_prefix, U, utility::template_string{"T"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto giga_unit =
    prefixed_unit_t<base_to_giga_prefix, U, utility::template_string{"G"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto mega_unit =
    prefixed_unit_t<base_to_mega_prefix, U, utility::template_string{"M"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto kilo_unit =
    prefixed_unit_t<base_to_kilo_prefix, U, utility::template_string{"k"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto hecto_unit =
    prefixed_unit_t<base_to_hecto_prefix, U, utility::template_string{"h"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto deca_unit =
    prefixed_unit_t<base_to_deca_prefix, U, utility::template_string{"da"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto deci_unit =
    prefixed_unit_t<base_to_deci_prefix, U, utility::template_string{"d"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto centi_unit =
    prefixed_unit_t<base_to_centi_prefix, U, utility::template_string{"c"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto milli_unit =
    prefixed_unit_t<base_to_milli_prefix, U, utility::template_string{"m"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto micro_unit =
    prefixed_unit_t<base_to_micro_prefix, U, utility::template_string{"μ"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto nano_unit =
    prefixed_unit_t<base_to_nano_prefix, U, utility::template_string{"n"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto pico_unit =
    prefixed_unit_t<base_to_pico_prefix, U, utility::template_string{"p"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto femto_unit =
    prefixed_unit_t<base_to_femto_prefix, U, utility::template_string{"f"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto atto_unit =
    prefixed_unit_t<base_to_atto_prefix, U, utility::template_string{"a"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto zepto_unit =
    prefixed_unit_t<base_to_zepto_prefix, U, utility::template_string{"z"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto yocto_unit =
    prefixed_unit_t<base_to_yocto_prefix, U, utility::template_string{"y"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto ronto_unit =
    prefixed_unit_t<base_to_ronto_prefix, U, utility::template_string{"r"} + U.name>{};
MODULE_EXPORT template <auto U>
constexpr unit auto quecto_unit =
    prefixed_unit_t<base_to_quecto_prefix, U, utility::template_string{"q"} + U.name>{};

MODULE_EXPORT template <auto U>
concept unitless = quantity_convertible_to<U.quantity, number> && !U.quantity.derived;

MODULE_EXPORT template <auto LHS, auto RHS>
concept unit_comparable_with = quantity_convertible_to<LHS.quantity, RHS.quantity> && LHS.reference == RHS.reference;

MODULE_EXPORT template <auto LHS, auto RHS>
concept unit_addable_with = quantity_convertible_to<LHS.quantity, RHS.quantity> && quantity_convertible_to<RHS.quantity, LHS.quantity> && LHS.reference == RHS.reference;

MODULE_EXPORT template <auto LHS, auto RHS>
concept unit_subtractable_from = quantity_convertible_to<LHS.quantity, RHS.quantity> && quantity_convertible_to<RHS.quantity, LHS.quantity> && LHS.reference == RHS.reference;
} // namespace maxwell

#endif