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

#ifndef NO_PREDEFINE_DERIVED_UNITS

#define MAKE_METRIC_PREFIXES(name, dimension)                                                                          \
    constexpr Unit auto quetta##name = name.adjustMultiplier##dimension<quetta>();                                     \
    using quetta##name##Type         = std::remove_const_t<decltype(quetta##name)>;                                    \
    constexpr Unit auto ronna##name  = name.adjustMultiplier##dimension<ronna>();                                      \
    using ronna##name##Type          = std::remove_const_t<decltype(ronna##name)>;                                     \
    constexpr Unit auto yotta##name  = name.adjustMultiplier##dimension<yotta>();                                      \
    using yotta##name##Type          = std::remove_const_t<decltype(yotta##name)>;                                     \
    constexpr Unit auto zetta##name  = name.adjustMultiplier##dimension<zetta>();                                      \
    using zetta##name##Type          = std::remove_const_t<decltype(zetta##name)>;                                     \
    constexpr Unit auto exa##name    = name.adjustMultiplier##dimension<exa>();                                        \
    using exa##name##Type            = std::remove_const_t<decltype(exa##name)>;                                       \
    constexpr Unit auto peta##name   = name.adjustMultiplier##dimension<peta>();                                       \
    using peta##name##Type           = std::remove_const_t<decltype(peta##name)>;                                      \
    constexpr Unit auto tera##name   = name.adjustMultiplier##dimension<tera>();                                       \
    using tera##name##Type           = std::remove_const_t<decltype(tera##name)>;                                      \
    constexpr Unit auto giga##name   = name.adjustMultiplier##dimension<giga>();                                       \
    using giga##name##Type           = std::remove_const_t<decltype(giga##name)>;                                      \
    constexpr Unit auto mega##name   = name.adjustMultiplier##dimension<mega>();                                       \
    using mega##name##Type           = std::remove_const_t<decltype(mega##name)>;                                      \
    constexpr Unit auto kilo##name   = name.adjustMultiplier##dimension<kilo>();                                       \
    using kilo##name##Type           = std::remove_const_t<decltype(kilo##name)>;                                      \
    constexpr Unit auto hecto##name  = name.adjustMultiplier##dimension<hecto>();                                      \
    using hecto##name##Type          = std::remove_const_t<decltype(hecto##name)>;                                     \
    constexpr Unit auto deca##name   = name.adjustMultiplier##dimension<deca>();                                       \
    using deca##name##Type           = std::remove_const_t<decltype(deca##name)>;                                      \
    constexpr Unit auto deci##name   = name.adjustMultiplier##dimension<deci>();                                       \
    using deci##name##Type           = std::remove_const_t<decltype(deci##name)>;                                      \
    constexpr Unit auto centi##name  = name.adjustMultiplier##dimension<centi>();                                      \
    using centi##name##Type          = std::remove_const_t<decltype(centi##name)>;                                     \
    constexpr Unit auto milli##name  = name.adjustMultiplier##dimension<milli>();                                      \
    using milli##name##Type          = std::remove_const_t<decltype(milli##name)>;                                     \
    constexpr Unit auto micro##name  = name.adjustMultiplier##dimension<micro>();                                      \
    using micro##name##Type          = std::remove_const_t<decltype(micro##name)>;                                     \
    constexpr Unit auto nano##name   = name.adjustMultiplier##dimension<nano>();                                       \
    using nano##name##Type           = std::remove_const_t<decltype(nano##name)>;                                      \
    constexpr Unit auto femto##name  = name.adjustMultiplier##dimension<femto>();                                      \
    using femto##name##Type          = std::remove_const_t<decltype(femto##name)>;                                     \
    constexpr Unit auto pico##name   = name.adjustMultiplier##dimension<pico>();                                       \
    using pico##name##Type           = std::remove_const_t<decltype(pico##name)>;                                      \
    constexpr Unit auto atto##name   = name.adjustMultiplier##dimension<atto>();                                       \
    using atto##name##Type           = std::remove_const_t<decltype(atto##name)>;                                      \
    constexpr Unit auto zepto##name  = name.adjustMultiplier##dimension<zepto>();                                      \
    using zepto##name##Type          = std::remove_const_t<decltype(zepto##name)>;                                     \
    constexpr Unit auto yocto##name  = name.adjustMultiplier##dimension<yocto>();                                      \
    using yocto##name##Type          = std::remove_const_t<decltype(yocto##name)>;                                     \
    constexpr Unit auto ronto##name  = name.adjustMultiplier##dimension<ronto>();                                      \
    using ronto##name##Type          = std::remove_const_t<decltype(ronto##name)>;                                     \
    constexpr Unit auto quecto##name = name.adjustMultiplier##dimension<quecto>();                                     \
    using quecto##name##Type         = std::remove_const_t<decltype(quecto##name)>;

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

#define ESC(...) __VA_ARGS__

#define MAKE_SCALED_UNIT_WITH_DESC(name, base, amount, dimension, desc)                                                \
    constexpr Unit auto name##Unit = base.adjustScale##dimension<ESC amount>();                                        \
    template <>                                                                                                        \
    inline const std::string unitString<name##Unit> = desc;                                                            \
    using name##UnitType                            = std::remove_const_t<decltype(name##Unit)>;

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
MAKE_METRIC_PREFIXES(radianUnit, Extra)

constexpr auto degreeUnit = unitlessUnit.addTag<_detail::DegreeTag>();
template <>
inline const std::string unitString<degreeUnit> = "deg";
using degreeUnitType                            = decltype(degreeUnit);

/// \brief Specifies a unit has dimensions of angle
///
/// Specifies a unit has dimensions of angle, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept AngleUnit = UnitConvertibleTo<U, radianUnit>;

template <>
struct is_tag_convertible<_detail::RadianTag, _detail::DegreeTag> : std::true_type
{
};

template <>
struct is_tag_convertible<_detail::DegreeTag, _detail::RadianTag> : std::true_type
{
};

/// \brief Calculate conversion factor
///
/// Calculate conversion factor from radians to degrees,
/// i.e. the constant to multiply a quantity in radians by
/// to get a value in degrees
///
/// \return conversion factor from radians to degrees
consteval double tagConversionFactor(radianUnitType, degreeUnitType) noexcept
{
    return 180.0 / std::numbers::pi;
}

/// \brief Calculate conversion factor
///
/// Calculate conversion factor from degrees to radians,
/// i.e. the constant to multiply a quantity in degrees by
/// to get a value in radians
///
/// \return conversion factor from degrees to radians
consteval double tagConversionFactor(degreeUnitType, radianUnitType) noexcept
{
    return std::numbers::pi / 180.0;
}

/// \cond
namespace _detail
{
struct SteradianTag
{
};
} // namespace _detail
/// \endcond

constexpr Unit auto steradianUnit = unitlessUnit.addTag<_detail::SteradianTag>();
template <>
inline const std::string unitString<steradianUnit> = "sr";
using steradianUnitType                            = std::remove_const_t<decltype(steradianUnit)>;
MAKE_METRIC_PREFIXES(steradianUnit, Extra)

// 22 SI Special Derived Units
// clang-format off
MAKE_UNIT_WITH_PREFIXES_DESC(hertz, unitlessUnit / secondUnit, Time, "Hz")
MAKE_UNIT_WITH_PREFIXES_DESC(newton, kilogramUnit * meterUnit / (secondUnit * secondUnit), Mass, "N")
MAKE_UNIT_WITH_PREFIXES_DESC(pascal, newtonUnit / (meterUnit * meterUnit), Mass, "Pa")
MAKE_UNIT_WITH_PREFIXES_DESC(joule, newtonUnit * meterUnit, Mass, "J")
MAKE_UNIT_WITH_PREFIXES_DESC(watt, jouleUnit / secondUnit, Mass, "J/s")
MAKE_UNIT_WITH_PREFIXES_DESC(coulomb, secondUnit * ampereUnit, Current, "C")
MAKE_UNIT_WITH_PREFIXES_DESC(volt, wattUnit / ampereUnit, Mass, "V")
MAKE_UNIT_WITH_PREFIXES_DESC(farad, coulombUnit / voltUnit, Mass, "F")
MAKE_UNIT_WITH_PREFIXES_DESC(ohm, voltUnit / ampereUnit, Mass, "Ω")
MAKE_UNIT_WITH_PREFIXES_DESC(siemens, unitlessUnit / ohmUnit, Time, "S")
MAKE_UNIT_WITH_PREFIXES_DESC(weber, voltUnit * secondUnit, Mass, "Wb")
MAKE_UNIT_WITH_PREFIXES_DESC(tesla, weberUnit / (meterUnit * meterUnit), Mass, "T")
MAKE_UNIT_WITH_PREFIXES_DESC(henry, weberUnit / ampereUnit, Mass, "L")
// Celsius
MAKE_UNIT_WITH_PREFIXES_DESC(lumen, candelaUnit * steradianUnit, Luminosity, "lm")
MAKE_UNIT_WITH_PREFIXES_DESC(lux, candelaUnit / (meterUnit * meterUnit), Luminosity, "lx")

/// \cond
namespace _detail 
{
struct BecquerelTag 
{

};

struct SievertTag 
{

};
}
/// \endcond

constexpr Unit auto becquerelUnit = hertzUnit.addTag<_detail::BecquerelTag>();
template<>
inline const std::string unitString<becquerelUnit> = "Bq";
using becquerelUnitType = std::remove_const_t<decltype(becquerelUnit)>;
MAKE_METRIC_PREFIXES(becquerelUnit, Time)

MAKE_UNIT_WITH_PREFIXES_DESC(gray, jouleUnit / kilogramUnit, Mass, "Gy")

constexpr Unit auto sievertUnit = grayUnit.addTag<_detail::SievertTag>();
template<>
inline const std::string unitString<sievertUnit> = "Sv";
using sievertUnitType = std::remove_const_t<decltype(sievertUnit)>;
MAKE_METRIC_PREFIXES(sievertUnit, Mass)

MAKE_UNIT_WITH_PREFIXES_DESC(katal, moleUnit / secondUnit, Amount, "kat")

MAKE_UNIT(meterPerSecond, meterUnit / secondUnit)
MAKE_UNIT(meterPerSecondPerSecond, meterPerSecondUnit / secondUnit)
MAKE_UNIT(squareMeter, meterUnit * meterUnit)
MAKE_UNIT(cubicMeter, squareMeterUnit * meterUnit)

MAKE_SCALED_UNIT_WITH_DESC(liter, candelaUnit, (std::ratio<1'000, 1>), Length, "L")

// Imperial units 
MAKE_SCALED_UNIT_WITH_DESC(foot, meterUnit, (std::ratio<328'084, 100'000>), Length, "ft")
MAKE_SCALED_UNIT_WITH_DESC(inch, footUnit, (std::ratio<1, 12>), Length, "in")
MAKE_SCALED_UNIT_WITH_DESC(yard, footUnit, (std::ratio<3, 1>), Length, "yd")
MAKE_SCALED_UNIT_WITH_DESC(mile, footUnit, (std::ratio<5'280, 1>), Length, "mi")

MAKE_SCALED_UNIT_WITH_DESC(pound, kilogramUnit, (std::ratio<453'592, 1'000'000)>, Mass, "lb")

MAKE_SCALED_UNIT_WITH_DESC(minute, secondUnit, (std::ratio<60, 1>), Time, "min")
MAKE_SCALED_UNIT_WITH_DESC(hour, minuteUnit, (std::ratio<60, 1>), Time, "hr")
MAKE_SCALED_UNIT_WITH_DESC(day, hourUnit, (std::ratio<24, 1>), Time, "day")
MAKE_SCALED_UNIT_WITH_DESC(week, dayUnit, (std::ratio<7, 1>), Time, "week")
MAKE_SCALED_UNIT_WITH_DESC(year, dayUnit, (std::ratio<52, 1>), Time, "yr")

// clang-format on
#undef ESC
#endif
} // namespace Maxwell

#endif