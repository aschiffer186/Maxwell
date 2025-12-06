/// \file quantity_value.hpp
/// \brief Definition of class template \c quantity_value.hpp

#ifndef QUANTITY_VALUE_HPP
#define QUANTITY_VALUE_HPP

#include "core/impl/quantity_value_holder_fwd.hpp"
#include "impl/quantity_value_declaration.hpp"
#include "impl/quantity_value_impl.hpp"

namespace maxwell {
MODULE_EXPORT template <typename Q>
using quetta =
    quantity_value<quetta_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using ronna =
    quantity_value<ronna_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using yotta =
    quantity_value<yotta_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using zetta =
    quantity_value<zetta_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using exa =
    quantity_value<exa_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using peta =
    quantity_value<peta_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using tera =
    quantity_value<tera_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using giga =
    quantity_value<giga_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using mega =
    quantity_value<mega_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using kilo =
    quantity_value<kilo_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using hecto =
    quantity_value<hecto_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using deca =
    quantity_value<deca_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using deci =
    quantity_value<deci_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using centi =
    quantity_value<centi_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using milli =
    quantity_value<milli_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using micro =
    quantity_value<micro_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using nano =
    quantity_value<nano_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using pico =
    quantity_value<pico_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using femto =
    quantity_value<femto_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using atto =
    quantity_value<atto_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using zepto =
    quantity_value<zepto_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using yocto =
    quantity_value<yocto_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using ronto =
    quantity_value<ronto_unit<Q::units>, Q::quantity, typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using quecto =
    quantity_value<quecto_unit<Q::units>, Q::quantity, typename Q::value_type>;

/// \brief Convets between two different quantities
///
/// Explicit cast between two different quantities. This function only checks
/// that the quantity being converted to has the same dimensions as the quantity
/// being converted from. It allows for an explicit conversion between
/// quantities with different kinds. Automatically converts the numerical value
/// of the input quantity from the input units to the destination units.
///
/// \tparam ToUnits The units of the quantity being converted to.
/// \tparam ToQuantity The quantity of the quantity being converted to. Defaults
/// to \c ToUnits.quantity.
/// \tparam FromUnits The units of the quantity being converted from.
/// \tparam FromQuantity The quantity of the quantity being converted from.
/// \tparam T The type of the numerical value of the quantity being converted
/// from.
/// \param value The quantity being converted from.
/// \return A \c quantity_value with the specified units and quantity, and the
/// converted numerical value.
MODULE_EXPORT template <_detail::quantity_value_like ToType, auto FromUnits,
                        auto FromQuantity, typename T = double>
constexpr auto
quantity_cast(const quantity_value<FromUnits, FromQuantity, T>& value)
    -> ToType {
  static_assert(ToType::quantity.dimensions == FromQuantity.dimensions,
                "Cannot convert between quantities with different dimensions");

  constexpr double multiplier = conversion_factor(FromUnits, ToType::units);
  constexpr double offset = conversion_offset(FromUnits, ToType::units);
  return ToType((value.get_value() * multiplier) + offset);
}

/// \brief Creates a \c quantity_value from a number and a unit
///
/// Creates a quantity_value from the specified numer and unit. The resulting
/// \c quantity_value has the specified number and units.
///
/// \tparam T The type of the numerical value of the quantity.
/// \tparam U The units of the quantity
/// \param value The numerical value of the quantity
/// \param U The units of the quantity.
/// \return A \c quantity_value with the specified value and units.
MODULE_EXPORT template <typename T, unit U>
  requires(!_detail::is_quantity_value_v<T> && !unit<T>)
constexpr auto operator*(T&& value, U) -> quantity_value<U{}, U::quantity, T> {
  return quantity_value<U{}, U::quantity, T>(std::forward<T>(value));
}

} // namespace maxwell

/// \brief Specialization of \c std::hash
///
/// Specialization of \c std::hash for \c quantity_value.
/// Two \c quantity_value instances have the same hash code if they represent
/// the same value after being converted to base units.
/// Two \c quantity_values instances will have difference hash codes if they
/// have the same value and same units, but represent different quantities.
///
/// \note The specialization of \c std::hash is not suitable as cryptographic
/// hash function.
///
/// \tparam Q The quantity of the \c quantity_value
/// \tparam U The units of the \c quantity_value
/// \tparam T The underlying type of the \c quantity_value
MODULE_EXPORT template <auto Q, auto U, typename T>
struct std::hash<maxwell::quantity_value<Q, U, T>> {
  auto operator()(const maxwell::quantity_value<Q, U, T>& q) noexcept
      -> std::size_t {
    const auto in_base_units = q.in_base_units();
    std::size_t hash_code = std::hash<T>{}(in_base_units.value_);
    return hash_code;
  }
};

#endif