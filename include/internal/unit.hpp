#ifndef UNIT_HPP
#define UNIT_HPP

#include <concepts>
#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>

#include "dimension.hpp"

namespace maxwell {
namespace _detail {
struct unit_base {};
} // namespace _detail

template <typename Tp>
concept unit =
    std::is_base_of_v<_detail::unit_base, std::remove_cvref_t<Tp>> && requires(std::remove_cvref_t<Tp> unit) {
      { unit.get_amount() } -> std::same_as<dimension>;
      { unit.get_current() } -> std::same_as<dimension>;
      { unit.get_length() } -> std::same_as<dimension>;
      { unit.get_luminosity() } -> std::same_as<dimension>;
      { unit.get_mass() } -> std::same_as<dimension>;
      { unit.get_temperature() } -> std::same_as<dimension>;
      { unit.get_time() } -> std::same_as<dimension>;
    };

// \class unit_type
/// \brief Definition of unit of measurement
///
/// A \c unit_type represents a unit of measurement. It represents a unit
/// derived from the SI base units (with the exception of the base unit of mass
/// is the gram, not te kilogram). A \c unit_type is the product of the base
/// units specified by the template parameters, possibly scaled by an
/// appropriate power of exponentiation. The base units are part of the units
/// type, so two instantiations of \c unit_type are the same type if and only if
/// they represent the same unit. If a unit does not have a particular
/// dimension, use \c null_measure as the template parameter for that dimension.
///
/// Some units have identical dimensions; different units with the same
/// dimensions can be distinguinshed by providing a tag type. This allows for
/// different units with the same dimensionality to compare unequal.
///
/// Because a units dimension is part of its type, unit errors can be caught at
/// compile-time rather than run-time. Additionally, unit conversion can be
/// performed automatically at compile-time rather than at runtime.
///
/// Using C++ 20 features, units can generally be manipulated as values rather
/// than requiring metaprogramming on the units type (e.g. it is possible to
/// perform transformations on values of units or compare two units with \c ==
/// instead of needing to use metafunctions).
///
/// \tparam Amount The amount dimension of the unit
/// \tparam Currrent The current dimension of the unit
/// \tparam Length The length dimension of the unit
/// \tparam Luminosity The luminosity dimension of the unit
/// \tparam Mass The mass dimension of the unit
/// \tparam Temerature The temperature dimension of the unit
/// \tparam Time The time dimension of the unit
/// \tparam Tag The tag of the unit (see above)
/// \tparam ExtraMultiplier Extra multiplier for dimensionless units that are
/// not unitless
template <dimension Amount, dimension Current, dimension Length, dimension Luminosity, dimension Mass,
          dimension Temperature, dimension Time, typename Tag = void, std::intmax_t ExtraMultiplier = 1>
struct unit_type : _detail::unit_base {
  static constexpr dimension amount = Amount;
  static constexpr dimension current = Current;
  static constexpr dimension length = Length;
  static constexpr dimension luminosity = Luminosity;
  static constexpr dimension mass = Mass;
  static constexpr dimension temperature = Temperature;
  static constexpr dimension time = Time;
  using tag = Tag;

  consteval dimension get_amount() const noexcept { return amount; }

  consteval dimension get_current() const noexcept { return current; }

  consteval dimension get_length() const noexcept { return length; }

  consteval dimension get_luminosity() const noexcept { return luminosity; }

  consteval dimension get_mass() const noexcept { return mass; }

  consteval dimension get_temperature() const noexcept { return temperature; }

  consteval dimension get_time() const noexcept { return time; }

  consteval unit auto to_SI_base_units() const noexcept {
    return unit_type<Amount.to_SI_base_dimension(), Current.to_SI_base_dimension(), Length.to_SI_base_dimension(),
                     Luminosity.to_SI_base_dimension(), Mass.to_SI_base_dimension(), Temperature.to_SI_base_dimension(),
                     Time.to_SI_base_dimension(), tag>{};
  }

  template <std::int8_t NewPrefix> constexpr unit auto adjust_prefix_amount() const noexcept {
    constexpr dimension adjusted = amount.adjust_prefix(NewPrefix);
    return unit_type<adjusted, current, length, luminosity, mass, temperature, time, tag>{};
  }

  template <std::int8_t NewPrefix> constexpr unit auto adjust_prefix_current() const noexcept {
    constexpr dimension adjusted = current.adjust_prefix(NewPrefix);
    return unit_type<amount, adjusted, length, luminosity, mass, temperature, time, tag>{};
  }

  template <std::int8_t NewPrefix> constexpr unit auto adjust_prefix_length() const noexcept {
    constexpr dimension adjusted = length.adjust_prefix(NewPrefix);
    return unit_type<amount, current, adjusted, luminosity, mass, temperature, time, tag>{};
  }

  template <std::int8_t NewPrefix> constexpr unit auto adjust_prefix_luminosity() const noexcept {
    constexpr dimension adjusted = luminosity.adjust_prefix(NewPrefix);
    return unit_type<amount, current, length, adjusted, mass, temperature, time, tag>{};
  }

  template <std::int8_t NewPrefix> constexpr unit auto adjust_prefix_mass() const noexcept {
    constexpr dimension adjusted = mass.adjust_prefix(NewPrefix);
    return unit_type<amount, current, length, luminosity, adjusted, temperature, time, tag>{};
  }

  template <std::int8_t NewPrefix> constexpr unit auto adjust_prefix_temperature() const noexcept {
    constexpr dimension adjusted = temperature.adjust_prefix(NewPrefix);
    return unit_type<amount, current, length, luminosity, mass, adjusted, time, tag>{};
  }

  template <std::int8_t NewPrefix> constexpr unit auto adjust_prefix_time() const noexcept {
    constexpr dimension adjusted = time.adjust_prefix(NewPrefix);
    return unit_type<amount, current, length, luminosity, mass, temperature, adjusted, tag>{};
  }

  template <rational Multiplier> constexpr unit auto adjust_scale_amount() const noexcept {
    constexpr dimension adjusted = amount.adjust_scale(Multiplier);
    return unit_type<adjusted, current, length, luminosity, mass, temperature, time, tag>{};
  }

  template <rational Multiplier> constexpr unit auto adjust_scale_current() const noexcept {
    constexpr dimension adjusted = current.adjust_scale(Multiplier);
    return unit_type<amount, adjusted, length, luminosity, mass, temperature, time, tag>{};
  }

  template <rational Multiplier> constexpr unit auto adjust_scale_length() const noexcept {
    constexpr dimension adjusted = length.adjust_scale(Multiplier);
    return unit_type<amount, current, adjusted, luminosity, mass, temperature, time, tag>{};
  }

  template <rational Multiplier> constexpr unit auto adjust_scale_luminosity() const noexcept {
    constexpr dimension adjusted = luminosity.adjust_scale(Multiplier);
    return unit_type<amount, current, length, adjusted, mass, temperature, time, tag>{};
  }

  template <rational Multiplier> constexpr unit auto adjust_scale_mass() const noexcept {
    constexpr dimension adjusted = mass.adjust_scale(Multiplier);
    return unit_type<amount, current, length, luminosity, adjusted, temperature, time, tag>{};
  }

  template <rational Multiplier> constexpr unit auto adjust_scale_temperature() const noexcept {
    constexpr dimension adjusted = temperature.adjust_scale(Multiplier);
    return unit_type<amount, current, length, luminosity, mass, adjusted, time, tag>{};
  }

  template <rational Multiplier> constexpr unit auto adjust_scale_time() const noexcept {
    constexpr dimension adjusted = time.adjust_scale(Multiplier);
    return unit_type<amount, current, length, luminosity, mass, temperature, adjusted, tag>{};
  }

  template <rational Addend> constexpr unit auto adjust_offset_amount() const noexcept {
    constexpr dimension adjusted = amount.adjust_offset(Addend);
    return unit_type<adjusted, current, length, luminosity, mass, temperature, time, tag>{};
  }

  template <rational Addend> constexpr unit auto adjust_offset_current() const noexcept {
    constexpr dimension adjusted = current.adjust_offset(Addend);
    return unit_type<amount, adjusted, length, luminosity, mass, temperature, time, tag>{};
  }

  template <rational Addend> constexpr unit auto adjust_offset_length() const noexcept {
    constexpr dimension adjusted = length.adjust_offset(Addend);
    return unit_type<amount, current, adjusted, luminosity, mass, temperature, time, tag>{};
  }

  template <rational Addend> constexpr unit auto adjust_offset_luminosity() const noexcept {
    constexpr dimension adjusted = luminosity.adjust_offset(Addend);
    return unit_type<amount, current, length, adjusted, mass, temperature, time, tag>{};
  }

  template <rational Addend> constexpr unit auto adjust_offset_mass() const noexcept {
    constexpr dimension adjusted = mass.adjust_offset(Addend);
    return unit_type<amount, current, length, luminosity, adjusted, temperature, time, tag>{};
  }

  template <rational Addend> constexpr unit auto adjust_offset_temperature() const noexcept {
    constexpr dimension adjusted = temperature.adjust_offset(Addend);
    return unit_type<amount, current, length, luminosity, mass, adjusted, time, tag>{};
  }

  template <rational Addend> constexpr unit auto adjust_offset_time() const noexcept {
    constexpr dimension adjusted = time.adjust_offset(Addend);
    return unit_type<amount, current, length, luminosity, mass, temperature, adjusted, tag>{};
  }

  template <typename T> constexpr unit auto add_tag() const noexcept {
    return unit_type<amount, current, length, luminosity, mass, temperature, time, T>{};
  }

  constexpr unit auto to_SI_base_unis() const noexcept { return *this; }

  constexpr static std::string unit_string() { return ""; }
};

constexpr bool operator==(unit auto lhs, unit auto rhs) noexcept {
  return lhs.get_amount() == rhs.get_amount() && lhs.get_current() == rhs.get_current() &&
         lhs.get_length() == rhs.get_length() && lhs.get_luminosity() == rhs.get_luminosity() &&
         lhs.get_mass() == rhs.get_mass() && lhs.get_temperature() == rhs.get_temperature() &&
         lhs.get_time() == rhs.get_time() && std::same_as<typename decltype(lhs)::tag, typename decltype(rhs)::tag>;
};

// --- Unit Traits ---

template <typename, typename> struct is_tag_convertible : std::false_type {};

template <typename T, typename U> struct is_tag_convertible<const T, U> : is_tag_convertible<T, U> {};

template <typename T, typename U> struct is_tag_convertible<T, const U> : is_tag_convertible<T, U> {};

template <typename T, typename U> struct is_tag_convertible<const T, const U> : is_tag_convertible<T, U> {};

template <typename Tag> struct is_tag_convertible<Tag, Tag> : std::true_type {};

template <auto From, auto To>
concept amount_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && (From.get_amount().power == To.get_amount().power);

template <auto From, auto To>
concept current_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && (From.get_current().power == To.get_current().power);

template <auto From, auto To>
concept length_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && (From.get_length().power == To.get_length().power);

template <auto From, auto To>
concept luminosity_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && (From.get_luminosity().power == To.get_luminosity().power);

template <auto From, auto To>
concept mass_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && (From.get_mass().power == To.get_mass().power);

template <auto From, auto To>
concept temperature_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && (From.get_temperature().power == To.get_temperature().power);

template <auto From, auto To>
concept time_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && (From.get_time().power == To.get_time().power);

template <auto From, auto To>
concept unit_convertible_to =
    amount_convertible_to<From, To> && current_convertible_to<From, To> && length_convertible_to<From, To> &&
    luminosity_convertible_to<From, To> && mass_convertible_to<From, To> && temperature_convertible_to<From, To> &&
    time_convertible_to<From, To> &&
    is_tag_convertible<typename decltype(From)::tag, typename decltype(To)::tag>::value;

// --- Unit conversion ---

/// \cond
namespace _detail {
constexpr std::array pow10{1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22, 1e-21, 1e-20, 1e-19, 1e-18,
                           1e-17, 1e-16, 1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10, 1e-9,  1e-8,  1e-7,  1e-6,  1e-5,
                           1e-4,  1e-3,  1e-2,  1e-1,  1e-0,  1e1,   1e2,   1e3,   1e4,   1e5,   1e6,   1e7,   1e8,
                           1e9,   1e10,  1e11,  1e12,  1e13,  1e14,  1e15,  1e16,  1e17,  1e18,  1e19,  1e20,  1e21,
                           1e22,  1e23,  1e24,  1e25,  1e26,  1e27,  1e28,  1e29,  1e30};

constexpr double pow(double base, std::intmax_t power) noexcept {
  if (power < 0) {
    return 1.0 / pow(base, -power);
  }

  if (power == 0) {
    return 1.0;
  }

  if (power == 1) {
    return base;
  }

  if (power % 2 == 0) {
    return pow(base * base, power / 2);
  }

  return pow(base * base, (power - 1) / 2);
}

constexpr double conversion_factor_prefix(std::int8_t from, std::int8_t to) noexcept {
  if ((from - to) < 30) {
    return pow10[(from - to) + 30];
  } else {
    return pow(10, (from - to));
  }
}

constexpr double conversion_factor_offset(const rational& to, const rational& from) noexcept {
  const rational res_ratio = to / from;
  return static_cast<double>(res_ratio);
}
} // namespace _detail
/// \endcond

/// \brief Calculates the conversion factor due to diffrent tags
///
/// Calculates the factor the magnitude of aquantity with units \c from needs to
/// be multiplied to be converted to a quantity with units \c to due to
/// different unit tags, By default, this returns 1. To add a conversion factor
/// for different tags, provide an overload of this function
///
/// \pre \c from is convertible to \c to
///
/// \param from The starting unit
/// \param to The target unit
/// \return The factor the magnitude of a quantity with units \c from needs to
/// be multiplied to be converted to a quantity with units \c to
template <typename, typename> struct tag_conversion_factor;

template <typename Tag> struct tag_conversion_factor<Tag, Tag> {
  static constexpr double factor = 1.0;
};

/// \brief Calculate the conversion factor to go from \c from to \c to
///
/// Calculates the factor the magnitude of a quantity with units \c from needs
/// to be multiplied to be converted to a quantity with units \c to
///
/// \pre \c from is convertible to \c to
///
/// \param from The starting unit
/// \param to The target unit
/// \return The factor the magnitude of a quantity with units \c from needs to
/// be multiplied to be converted to a quantity with units \c to
constexpr double conversion_factor(unit auto from, unit auto to) noexcept
  requires unit_convertible_to<from, to>
{
  using From = decltype(from);
  using To = decltype(to);

  // Short circuit for identical types
  if (from == to) {
    return 1.0;
  } else {

    double conversionFactor{1.0};
    // Convert prefixes
    conversionFactor *= _detail::conversion_factor_prefix(from.get_amount().prefix, to.get_amount().prefix);
    conversionFactor *= _detail::conversion_factor_prefix(from.get_current().prefix, to.get_current().prefix);
    conversionFactor *= _detail::conversion_factor_prefix(from.get_length().prefix, to.get_length().prefix);
    conversionFactor *= _detail::conversion_factor_prefix(from.get_luminosity().prefix, to.get_luminosity().prefix);
    conversionFactor *= _detail::conversion_factor_prefix(from.get_mass().prefix, to.get_mass().prefix);
    conversionFactor *= _detail::conversion_factor_prefix(from.get_temperature().prefix, to.get_temperature().prefix);
    conversionFactor *= _detail::conversion_factor_prefix(from.get_time().prefix, to.get_time().prefix);
    // conversionFactor *= _detail::conversion_factor_prefix(from.prefix, to.prefix);
    conversionFactor *= tag_conversion_factor<typename From::tag, typename To::tag>::factor;

    // Convert ratios
    conversionFactor *= _detail::conversion_factor_offset(from.get_amount().scale, to.get_amount().scale);
    conversionFactor *= _detail::conversion_factor_offset(from.get_current().scale, to.get_current().scale);
    conversionFactor *= _detail::conversion_factor_offset(from.get_length().scale, to.get_length().scale);
    conversionFactor *= _detail::conversion_factor_offset(from.get_luminosity().scale, to.get_luminosity().scale);
    conversionFactor *= _detail::conversion_factor_offset(from.get_mass().scale, to.get_mass().scale);
    conversionFactor *= _detail::conversion_factor_offset(from.get_temperature().scale, to.get_temperature().scale);
    conversionFactor *= _detail::conversion_factor_offset(from.get_time().scale, to.get_time().scale);
    return conversionFactor;
  }
}

/// \brief Calculates the conversion between two units
///
/// Calculates the conversion offset that must be applied to convert from
/// the specified unit to the target unit.
///
/// \pre \c from is convertible to \c to
/// \param from the unit to convert from
/// \param to the target unit
/// \return the conversion offset that must be applied
constexpr double conversion_offset(unit auto from, unit auto to) noexcept
  requires unit_convertible_to<from, to>
{
  if (from == to) {
    return 0.0;
  } else {
    const double amount_difference = static_cast<double>(to.get_amount().offset - from.get_amount().offset);
    const double current_difference = static_cast<double>(to.get_current().offset - from.get_current().offset);
    const double length_difference = static_cast<double>(to.get_length().offset - from.get_length().offset);
    const double luminosity_difference = static_cast<double>(to.get_luminosity().offset - from.get_luminosity().offset);
    const double mass_difference = static_cast<double>(to.get_mass().offset - from.get_mass().offset);
    const double temperature_difference =
        static_cast<double>(to.get_temperature().offset - from.get_temperature().offset);
    const double time_difference = static_cast<double>(to.get_time().offset - from.get_time().offset);

    return /*conversion_factor(from, to) **/
        (amount_difference + current_difference + length_difference + luminosity_difference + mass_difference +
         temperature_difference + time_difference);
  }
}

// --- Unit Synthesis ---
/// \cond
namespace _detail {

template <unit auto LHS, unit auto RHS> struct unit_product_impl {
  /// Type of the product of LHS and RHS
  using type = unit_type<LHS.get_amount() * RHS.get_amount(), LHS.get_current() * RHS.get_current(),
                         LHS.get_length() * RHS.get_length(), LHS.get_luminosity() * RHS.get_luminosity(),
                         LHS.get_mass() * RHS.get_mass(), LHS.get_temperature() * RHS.get_temperature(),
                         LHS.get_time() * RHS.get_time(), typename decltype(LHS)::tag>;
};

template <unit auto LHS, unit auto RHS> using unit_product_impl_t = unit_product_impl<LHS, RHS>::type;

/// \brief The type representing the quotient of two units
///
/// \tparam LHS The type of the dividend
/// \tparam RHS The type of the divisor
template <unit auto LHS, unit auto RHS> struct unit_quotient_impl {
  /// Type of the quotient of LHS and RHS
  using type = unit_type<LHS.get_amount() / RHS.get_amount(), LHS.get_current() / RHS.get_current(),
                         LHS.get_length() / RHS.get_length(), LHS.get_luminosity() / RHS.get_luminosity(),
                         LHS.get_mass() / RHS.get_mass(), LHS.get_temperature() / RHS.get_temperature(),
                         LHS.get_time() / RHS.get_time(), typename decltype(LHS)::tag>;
};

template <unit auto LHS, unit auto RHS> using unit_quotient_impl_t = unit_quotient_impl<LHS, RHS>::type;

template <unit auto U> struct unit_sqrt_impl {
  using type = unit_type<sqrt(U.get_amount()), sqrt(U.get_current()), sqrt(U.get_length()), sqrt(U.get_luminosity()),
                         sqrt(U.get_mass()), sqrt(U.get_temperature()), sqrt(U.get_time()), typename decltype(U)::tag>;
};

template <unit auto U> using unit_sqrt_impl_t = unit_sqrt_impl<U>::type;
} // namespace _detail
/// \endcond

/// \brief The type representing the product of two units
template <unit LHS, unit RHS> struct unit_product_type : _detail::unit_product_impl_t<LHS{}, RHS{}> {
  using base_type = _detail::unit_product_impl_t<LHS{}, RHS{}>;

  constexpr static std::string unit_string() { return LHS::unit_string() + "*" + RHS::unit_string(); }
};

/// \brief The type representing the quotient of two units
///
/// \tparam LHS The type of the dividend
/// \tparam RHS The type of the divisor
template <unit LHS, unit RHS> struct unit_quotient_type : _detail::unit_quotient_impl_t<LHS{}, RHS{}> {
  using base_type = _detail::unit_quotient_impl_t<LHS{}, RHS{}>;

  constexpr static std::string unit_string() { return LHS::unit_string() + "/" + RHS::unit_string(); }
};

template <unit U> struct unit_sqrt_type : _detail::unit_sqrt_impl_t<U{}> {
  using base_type = _detail::unit_sqrt_impl<U{}>;

  constexpr static std::string unit_string() { return base_type::unit_string(); }
};
/// \brief Multiplies two units
///
/// Multiplies two units; the dimensions of the resulting unit is the product of
/// the dimensions of lhs and rhs. This function carries forward the tag and the
/// multipliler of the first unit in the multiplication.
///
/// \param lhs The left hand side of the multiplication
/// \param rhs The right hand side of the multiplication
/// \return The product of two units
template <unit LHS, unit RHS> constexpr unit auto operator*(LHS, RHS) noexcept { return unit_product_type<LHS, RHS>{}; }

/// \brief Divides two units
///
/// Divides two units; the dimensions of the resulting unit is the division of
/// the dimensions of lhs and rhs. This function carries forward the tag and the
/// multipliler of the first unit in the division
///
/// \param lhs The dividend
/// \param rhs The divisor
/// \return The quotient of two units
template <unit LHS, unit RHS> constexpr unit auto operator/(LHS, RHS) noexcept {
  return unit_quotient_type<LHS, RHS>{};
}

// --- Unit Categories ---

// --- SI base units ---

struct mole_unit_type : unit_type<base_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                                  null_dimension, null_dimension> {
  using base_type = unit_type<base_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                              null_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "mol"; }
};
constexpr mole_unit_type mole_unit;

struct ampere_unit_type : unit_type<null_dimension, base_dimension, null_dimension, null_dimension, null_dimension,
                                    null_dimension, null_dimension> {
  using base_type = unit_type<null_dimension, base_dimension, null_dimension, null_dimension, null_dimension,
                              null_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "A"; }
};
constexpr ampere_unit_type ampere_unit;

struct meter_unit_type : unit_type<null_dimension, null_dimension, base_dimension, null_dimension, null_dimension,
                                   null_dimension, null_dimension> {
  using base_type = unit_type<null_dimension, null_dimension, base_dimension, null_dimension, null_dimension,
                              null_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "m"; }
};
constexpr meter_unit_type meter_unit;

struct candela_unit_type : unit_type<null_dimension, null_dimension, null_dimension, base_dimension, null_dimension,
                                     null_dimension, null_dimension> {
  using base_type = unit_type<null_dimension, null_dimension, null_dimension, base_dimension, null_dimension,
                              null_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "cd"; }
};
constexpr candela_unit_type candela_unit;

struct kilogram_unit_type : unit_type<null_dimension, null_dimension, null_dimension, null_dimension,
                                      dimension{.power = one, .prefix = 3}, null_dimension, null_dimension> {
  using base_type = unit_type<null_dimension, null_dimension, null_dimension, null_dimension,
                              dimension{.power = one, .prefix = 3}, null_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "kg"; }
};
constexpr kilogram_unit_type kilogram_unit;

struct kelvin_unit_type : unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                                    base_dimension, null_dimension> {
  using base_type = unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                              base_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "K"; }
};
constexpr kelvin_unit_type kelvin_unit;

struct second_unit_type : unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                                    base_dimension, null_dimension> {
  using base_type = unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                              base_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "s"; }
};
constexpr second_unit_type second_unit;

struct scalar_unit_type : unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                                    null_dimension, null_dimension> {
  using base_type = unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                              null_dimension, null_dimension>;

  constexpr static std::string unit_string() { return "[]"; }
};
constexpr scalar_unit_type scalar_unit;

template <auto U>
concept amount_unit = unit_convertible_to<U, mole_unit>;

template <auto U>
concept current_unit = unit_convertible_to<U, ampere_unit>;

template <auto U>
concept length_unit = unit_convertible_to<U, meter_unit>;

template <auto U>
concept luminosity_unit = unit_convertible_to<U, candela_unit>;

template <auto U>
concept mass_unit = unit_convertible_to<U, kilogram_unit>;

template <auto U>
concept temperature_unit = unit_convertible_to<U, kelvin_unit>;

template <auto U>
concept time_unit = unit_convertible_to<U, second_unit>;

template <auto U>
concept unitless_unit = unit_convertible_to<U, scalar_unit>;
} // namespace maxwell

#endif