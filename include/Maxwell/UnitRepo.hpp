/// \file UnitRepo.hpp
/// \author Alex Schiffer
/// \brief File containing definition of common units and macros to create new units
/// \version 0.1
/// \date 2024-11-02
///
/// \copyright Copyright (c) 2024
#ifndef UNIT_REPO_HPP
#define UNIT_REPO_HPP

#include <numbers>
#include <type_traits>

#include "Unit.hpp"

namespace Maxwell
{
/// \cond
namespace _detail
{
struct RadianTag
{
};

struct DegreeTag
{
};
} // namespace _detail
/// \endcond

/// \brief Create a new unit
///
/// Creates a new unit with the specified definition.
/// The macro expands to the followng
/// 1. A compile-time constant with the name nameUnit initialized by the specified definition
/// 2. A A type alias nameUnitType
///
/// \param name the name of the new unit
/// \param def the definition for the new unit (may be any C++ expression whose type is an instantiation of \c UnitType)
#define MAKE_UNIT(name, def)                                                                                           \
    constexpr auto name##Unit = def;                                                                                   \
    using name##UnitType      = decltype(name##Unit);

/// \brief Create a new unit
///
/// Creates a new unit with the specified definition and description string.
/// The macro expands to the followng
/// 1. A compile-time constant with the name nameUnit initialized by the specified definition
/// 2. A specialization of unitString equal to the specified description
/// 3. A type alias nameUnitType
///
/// \param name the name of the new unit
/// \param def the definition for the new unit (may be any C++ expression whose type is an instantiation of \c UnitType)
/// \param str the description of the unit (may be any C++ expression convertible to a \c std::string)
#define MAKE_UNIT_WITH_DESC(name, def, str)                                                                            \
    constexpr auto name##Unit = def;                                                                                   \
    template <>                                                                                                        \
    inline const std::string unitString<name##Unit> = str;                                                             \
    using name##UnitType                            = decltype(name##Unit);

constexpr std::intmax_t quetta = 30;
constexpr std::intmax_t ronna  = 27;
constexpr std::intmax_t yotta  = 24;
constexpr std::intmax_t zetta  = 21;
constexpr std::intmax_t exa    = 18;
constexpr std::intmax_t peta   = 15;
constexpr std::intmax_t tera   = 12;
constexpr std::intmax_t giga   = 9;
constexpr std::intmax_t mega   = 6;
constexpr std::intmax_t kilo   = 3;
constexpr std::intmax_t hecto  = 2;
constexpr std::intmax_t deca   = 1;
constexpr std::intmax_t deci   = -1;
constexpr std::intmax_t centi  = -2;
constexpr std::intmax_t milli  = -3;
constexpr std::intmax_t micro  = -6;
constexpr std::intmax_t nano   = -9;
constexpr std::intmax_t pico   = -12;
constexpr std::intmax_t femto  = -15;
constexpr std::intmax_t atto   = -18;
constexpr std::intmax_t zepto  = -21;
constexpr std::intmax_t yocto  = -24;
constexpr std::intmax_t ronto  = -27;
constexpr std::intmax_t quecto = -30;

#ifndef NO_PREDEFINE_DERIVED_UNITS

#define MAKE_METRIC_PREFIXES(name, dimension)                                                                          \
    constexpr Unit auto quetta##name = name.adjustMultiplier##dimension<quetta>();                                     \
    using quetta##name##Type         = decltype(quetta##name);                                                         \
    constexpr Unit auto kilo##name   = name.adjustMultiplier##dimension<kilo>();                                       \
    using kilo##name##Type           = decltype(kilo##name);

/// \brief Create a new unit and corresponding metric prefixes
///
/// Creates a new unit with the specified definition as well as all of its metric prefixes.
/// The macro expands to the followng
/// 1. A compile-time constant with the name nameUnit initialized by the specified definition
/// 2. A type alias nameUnitType
/// 3. A compile-time constant and type alias for each metric prefix of the unit
///
/// \param name the name of the new unit
/// \param def the definition for the new unit (may be any C++ expression whose type is an instantiation of \c UnitType)
/// \param dimension the dimension to apply the prefix to (must be capitalized)
#define MAKE_UNIT_WITH_PREFIXES(name, def, dimension)                                                                  \
    MAKE_UNIT(name, def)                                                                                               \
    MAKE_METRIC_PREFIXES(name##Unit, dimension)

#define MAKE_UNIT_WITH_PREFIXES_DESC(name, def, dimension, desc)                                                       \
    MAKE_UNIT_WITH_DESC(name, def, desc)                                                                               \
    MAKE_METRIC_PREFIXES(name##Unit, dimension)

MAKE_METRIC_PREFIXES(moleUnit, Amount)
MAKE_METRIC_PREFIXES(ampereUnit, Current)
MAKE_METRIC_PREFIXES(meterUnit, Length)
MAKE_METRIC_PREFIXES(candelaUnit, Luminosity)
MAKE_METRIC_PREFIXES(gramUnit, Mass)
MAKE_METRIC_PREFIXES(kelvinUnit, Temperature)
MAKE_METRIC_PREFIXES(secondUnit, Time)

constexpr auto radianUnit = unitlessUnit.addTag<_detail::RadianTag>();
template <>
inline const std::string unitString<radianUnit> = "rad";
using radianUnitType                            = decltype(radianUnit);
MAKE_METRIC_PREFIXES(radianUnit, Time)

constexpr auto degreeUnit = unitlessUnit.addTag<_detail::DegreeTag>();
template <>
inline const std::string unitString<degreeUnit> = "deg";
using degreeUnitType                            = decltype(degreeUnit);

template <auto U>
concept AngleUnit =
    Unit<decltype(U)> && U.amount() == Internal::nullMeasure && U.current() == Internal::nullMeasure &&
    U.length() == Internal::nullMeasure && U.luminosity() == Internal::nullMeasure &&
    U.mass() == Internal::nullMeasure && U.temperature() == Internal::nullMeasure && U.time().power() == 0 &&
    (std::same_as<std::remove_cv_t<typename decltype(U)::Tag>, _detail::RadianTag> ||
     std::same_as<std::remove_cv_t<typename decltype(U)::Tag>, _detail::DegreeTag>);

template <>
struct is_tag_convertible<_detail::RadianTag, _detail::DegreeTag> : std::true_type
{
};

template <>
struct is_tag_convertible<_detail::DegreeTag, _detail::RadianTag> : std::true_type
{
};

/**
 * @brief Calculate conversion factor
 *
 * Calculate conversion factor from radians to degrees,
 * i.e. the constant to multiply a quantity in radians by
 * to get a value in degrees
 *
 * @return conversion factor from radians to degrees
 */
auto consteval tagConversionFactor(radianUnitType, degreeUnitType) noexcept -> double
{
    return 180.0 / std::numbers::pi;
}

/**
 * @brief Calculate conversion factor
 *
 * Calculate conversion factor from degrees to radians,
 * i.e. the constant to multiply a quantity in degrees by
 * to get a value in radians
 *
 * @return conversion factor from degrees to radians
 */
auto consteval tagConversionFactor(degreeUnitType, radianUnitType) noexcept -> double
{
    return std::numbers::pi / 180.0;
}

MAKE_UNIT_WITH_PREFIXES_DESC(hertz, unitlessUnit / secondUnit, Time, "Hz")
MAKE_UNIT_WITH_PREFIXES_DESC(newton, kilogramUnit* meterUnit / (secondUnit * secondUnit), Mass, "N")
MAKE_UNIT_WITH_PREFIXES_DESC(pascal, newtonUnit / (meterUnit * meterUnit), Mass, "Pa")
MAKE_UNIT_WITH_PREFIXES_DESC(joule, newtonUnit* meterUnit, Mass, "J")
MAKE_UNIT_WITH_PREFIXES_DESC(watt, jouleUnit / secondUnit, Mass, "J/s")
MAKE_UNIT_WITH_PREFIXES_DESC(coulomb, secondUnit* ampereUnit, Current, "C")
MAKE_UNIT_WITH_PREFIXES_DESC(volt, wattUnit / ampereUnit, Mass, "V")
MAKE_UNIT_WITH_PREFIXES_DESC(farad, coulombUnit / voltUnit, Mass, "F")
MAKE_UNIT_WITH_PREFIXES_DESC(ohm, voltUnit / ampereUnit, Mass, "Ω")
MAKE_UNIT_WITH_PREFIXES_DESC(siemens, unitlessUnit / ohmUnit, Time, "S")
MAKE_UNIT_WITH_PREFIXES_DESC(weber, voltUnit* secondUnit, Mass, "Wb")
MAKE_UNIT_WITH_PREFIXES_DESC(tesla, weberUnit / (meterUnit * meterUnit), Mass, "T")

#endif
} // namespace Maxwell

#endif