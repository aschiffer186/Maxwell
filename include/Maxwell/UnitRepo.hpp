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

namespace maxwell
{
/// \cond
namespace _detail
{
struct radian_tag
{
};

struct degree_tag
{
};
} // namespace _detail
/// \endcond

/// \brief Create a new unit
///
/// Creates a new unit with the specified definition.
/// The macro expands to the followng
/// 1. A compile-time constant with the name name_unit initialized by the specified definition
/// 2. A A type alias name_unitType
///
/// \param name the name of the new unit
/// \param def the definition for the new unit (may be any C++ expression whose type is an instantiation of \c
/// _unitType)
#define MAKE_UNIT(name, def)                                                                                           \
    constexpr auto name##_unit = def;                                                                                  \
    using name##_unit##_type   = decltype(name##_unit);

/// \brief Create a new unit
///
/// Creates a new unit with the specified definition and description string.
/// The macro expands to the followng
/// 1. A compile-time constant with the name name_unit initialized by the specified definition
/// 2. A specialization of unitString equal to the specified description
/// 3. A type alias name_unitType
///
/// \param name the name of the new unit
/// \param def the definition for the new unit (may be any C++ expression whose type is an instantiation of \c
/// _unitType) \param str the description of the unit (may be any C++ expression convertible to a \c std::string)
#define MAKE_UNIT_WITH_DESC(name, def, str)                                                                            \
    constexpr auto name##_unit = def;                                                                                  \
    template <>                                                                                                        \
    inline const std::string unit_string<name##_unit> = str;                                                           \
    using name##_unit##_type                          = decltype(name##_unit);

#ifndef NO_PREDEFINE_DERIVED_UNITS

#define MAKE_METRIC_PREFIXES(name, dimension)                                                                          \
    constexpr unit auto quetta##name = name.adjust_multiplier_##dimension<quetta>();                                   \
    using quetta##name##_type        = std::remove_const_t<decltype(quetta##name)>;                                    \
    constexpr unit auto ronna##name  = name.adjust_multiplier_##dimension<ronna>();                                    \
    using ronna##name##_type         = std::remove_const_t<decltype(ronna##name)>;                                     \
    constexpr unit auto yotta##name  = name.adjust_multiplier_##dimension<yotta>();                                    \
    using yotta##name##_type         = std::remove_const_t<decltype(yotta##name)>;                                     \
    constexpr unit auto zetta##name  = name.adjust_multiplier_##dimension<zetta>();                                    \
    using zetta##name##_type         = std::remove_const_t<decltype(zetta##name)>;                                     \
    constexpr unit auto exa##name    = name.adjust_multiplier_##dimension<exa>();                                      \
    using exa##name##_type           = std::remove_const_t<decltype(exa##name)>;                                       \
    constexpr unit auto peta##name   = name.adjust_multiplier_##dimension<peta>();                                     \
    using peta##name##_type          = std::remove_const_t<decltype(peta##name)>;                                      \
    constexpr unit auto tera##name   = name.adjust_multiplier_##dimension<tera>();                                     \
    using tera##name##_type          = std::remove_const_t<decltype(tera##name)>;                                      \
    constexpr unit auto giga##name   = name.adjust_multiplier_##dimension<giga>();                                     \
    using giga##name##_type          = std::remove_const_t<decltype(giga##name)>;                                      \
    constexpr unit auto mega##name   = name.adjust_multiplier_##dimension<mega>();                                     \
    using mega##name##_type          = std::remove_const_t<decltype(mega##name)>;                                      \
    constexpr unit auto kilo##name   = name.adjust_multiplier_##dimension<kilo>();                                     \
    using kilo##name##_type          = std::remove_const_t<decltype(kilo##name)>;                                      \
    constexpr unit auto hecto##name  = name.adjust_multiplier_##dimension<hecto>();                                    \
    using hecto##name##_type         = std::remove_const_t<decltype(hecto##name)>;                                     \
    constexpr unit auto deca##name   = name.adjust_multiplier_##dimension<deca>();                                     \
    using deca##name##_type          = std::remove_const_t<decltype(deca##name)>;                                      \
    constexpr unit auto deci##name   = name.adjust_multiplier_##dimension<deci>();                                     \
    using deci##name##_type          = std::remove_const_t<decltype(deci##name)>;                                      \
    constexpr unit auto centi##name  = name.adjust_multiplier_##dimension<centi>();                                    \
    using centi##name##_type         = std::remove_const_t<decltype(centi##name)>;                                     \
    constexpr unit auto milli##name  = name.adjust_multiplier_##dimension<milli>();                                    \
    using milli##name##_type         = std::remove_const_t<decltype(milli##name)>;                                     \
    constexpr unit auto micro##name  = name.adjust_multiplier_##dimension<micro>();                                    \
    using micro##name##_type         = std::remove_const_t<decltype(micro##name)>;                                     \
    constexpr unit auto nano##name   = name.adjust_multiplier_##dimension<nano>();                                     \
    using nano##name##_type          = std::remove_const_t<decltype(nano##name)>;                                      \
    constexpr unit auto femto##name  = name.adjust_multiplier_##dimension<femto>();                                    \
    using femto##name##_type         = std::remove_const_t<decltype(femto##name)>;                                     \
    constexpr unit auto pico##name   = name.adjust_multiplier_##dimension<pico>();                                     \
    using pico##name##_type          = std::remove_const_t<decltype(pico##name)>;                                      \
    constexpr unit auto atto##name   = name.adjust_multiplier_##dimension<atto>();                                     \
    using atto##name##_type          = std::remove_const_t<decltype(atto##name)>;                                      \
    constexpr unit auto zepto##name  = name.adjust_multiplier_##dimension<zepto>();                                    \
    using zepto##name##_type         = std::remove_const_t<decltype(zepto##name)>;                                     \
    constexpr unit auto yocto##name  = name.adjust_multiplier_##dimension<yocto>();                                    \
    using yocto##name##_type         = std::remove_const_t<decltype(yocto##name)>;                                     \
    constexpr unit auto ronto##name  = name.adjust_multiplier_##dimension<ronto>();                                    \
    using ronto##name##_type         = std::remove_const_t<decltype(ronto##name)>;                                     \
    constexpr unit auto quecto##name = name.adjust_multiplier_##dimension<quecto>();                                   \
    using quecto##name##_type        = std::remove_const_t<decltype(quecto##name)>;

/// \brief Create a new unit and corresponding metric prefixes
///
/// Creates a new unit with the specified definition as well as all of its metric prefixes.
/// The macro expands to the followng
/// 1. A compile-time constant with the name name_unit initialized by the specified definition
/// 2. A type alias name_unitType
/// 3. A compile-time constant and type alias for each metric prefix of the unit
///
/// \param name the name of the new unit
/// \param def the definition for the new unit (may be any C++ expression whose type is an instantiation of \c
/// _unitType) \param dimension the dimension to apply the prefix to (must be capitalized)
#define MAKE_UNIT_WITH_PREFIXES(name, def, dimension)                                                                  \
    MAKE_UNIT(name, def)                                                                                               \
    MAKE_METRIC_PREFIXES(name##_unit, dimension)

#define MAKE_UNIT_WITH_PREFIXES_DESC(name, def, dimension, desc)                                                       \
    MAKE_UNIT_WITH_DESC(name, def, desc)                                                                               \
    MAKE_METRIC_PREFIXES(name##_unit, dimension)

#define ESC(...) __VA_ARGS__

#define MAKE_SCALED_UNIT_WITH_DESC(name, base, amount, dimension, desc)                                                \
    constexpr unit auto name##_unit = base.adjust_scale_##dimension<ESC amount>();                                     \
    template <>                                                                                                        \
    inline const std::string unit_string<name##_unit> = desc;                                                          \
    using name##_unit##_type                          = std::remove_const_t<decltype(name##_unit)>;

MAKE_METRIC_PREFIXES(mole_unit, amount)
MAKE_METRIC_PREFIXES(ampere_unit, current)
MAKE_METRIC_PREFIXES(meter_unit, length)
MAKE_METRIC_PREFIXES(candela_unit, luminosity)
MAKE_METRIC_PREFIXES(gram_unit, mass)
MAKE_METRIC_PREFIXES(kelvin_unit, temperature)
MAKE_METRIC_PREFIXES(second_unit, time)

constexpr auto radian_unit = unitless_unit_type{}.add_tag<_detail::radian_tag>();
template <>
inline const std::string unit_string<radian_unit> = "rad";
using radian_unit_type                            = decltype(radian_unit);
MAKE_METRIC_PREFIXES(radian_unit, extra)

constexpr auto degree_unit = unitless_unit_type{}.add_tag<_detail::degree_tag>();
template <>
inline const std::string unit_string<degree_unit> = "deg";
using degree_unit_type                            = decltype(degree_unit);

/// \brief Specifies a unit has dimensions of angle
///
/// Specifies a unit has dimensions of angle, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept angle_unit = unit_convertible_to<U, radian_unit>;

template <>
struct is_tag_convertible<_detail::radian_tag, _detail::degree_tag> : std::true_type
{
};

template <>
struct is_tag_convertible<_detail::degree_tag, _detail::radian_tag> : std::true_type
{
};

/// \brief Calculate conversion factor
///
/// Calculate conversion factor from radians to degrees,
/// i.e. the constant to multiply a quantity in radians by
/// to get a value in degrees
///
/// \return conversion factor from radians to degrees
template <>
struct tag_conversion_factor<radian_unit_type::tag, degree_unit_type::tag>
{
    static constexpr double factor = 180.0 / std::numbers::pi;
};

/// \brief Calculate conversion factor
///
/// Calculate conversion factor from degrees to radians,
/// i.e. the constant to multiply a quantity in degrees by
/// to get a value in radians
///
/// \return conversion factor from degrees to radians
template <>
struct tag_conversion_factor<degree_unit_type::tag, radian_unit_type::tag>
{
    static constexpr double factor = std::numbers::pi / 180.0;
};

/// \cond
namespace _detail
{
struct steradian_tag
{
};
} // namespace _detail
/// \endcond

constexpr unit auto steradian_unit = unitless_unit_type{}.add_tag<_detail::steradian_tag>();
template <>
inline const std::string unit_string<steradian_unit> = "sr";
using steradian_unitType                             = std::remove_const_t<decltype(steradian_unit)>;
MAKE_METRIC_PREFIXES(steradian_unit, extra)

// 22 SI Special Derived _units
// clang-format off
MAKE_UNIT_WITH_PREFIXES_DESC(hertz, unitless_unit_type{} / second_unit, time, "Hz")
MAKE_UNIT_WITH_PREFIXES_DESC(newton, kilogram_unit * meter_unit / (second_unit * second_unit), mass, "N")

template<auto U> 
concept force_unit = unit_convertible_to<U, newton_unit>;

MAKE_UNIT_WITH_PREFIXES_DESC(pascal, newton_unit / (meter_unit * meter_unit), mass, "Pa")

constexpr unit auto atm_unit = pascal_unit.template adjust_scale_mass<std::ratio<101'325>>();
template<>
inline const std::string unit_string<atm_unit> = "atm";
using atm_unit_type = std::remove_const_t<decltype(atm_unit)>;

template<auto U> 
concept pressure_unit = unit_convertible_to<U, pascal_unit>;

MAKE_UNIT_WITH_PREFIXES_DESC(joule, newton_unit * meter_unit, mass, "J")
template<auto U> 
concept energy_unit = unit_convertible_to<U, joule_unit>;

MAKE_UNIT_WITH_PREFIXES_DESC(watt, joule_unit / second_unit, mass, "J/s")
template<auto U> 
concept power_unit = unit_convertible_to<U, watt_unit>;

MAKE_UNIT_WITH_PREFIXES_DESC(coulomb, second_unit * ampere_unit, current, "C")
MAKE_UNIT_WITH_PREFIXES_DESC(volt, watt_unit / ampere_unit, mass, "V")
MAKE_UNIT_WITH_PREFIXES_DESC(farad, coulomb_unit / volt_unit, mass, "F")
MAKE_UNIT_WITH_PREFIXES_DESC(ohm, volt_unit / ampere_unit, mass, "Ω")
MAKE_UNIT_WITH_PREFIXES_DESC(siemens, unitless_unit_type{} / ohm_unit, time, "S")
MAKE_UNIT_WITH_PREFIXES_DESC(weber, volt_unit * second_unit, mass, "Wb")
MAKE_UNIT_WITH_PREFIXES_DESC(tesla, weber_unit / (meter_unit * meter_unit), mass, "T")
MAKE_UNIT_WITH_PREFIXES_DESC(henry, weber_unit / ampere_unit, mass, "L")
constexpr unit auto celsius_unit = kelvin_unit.template adjust_offset_temperature<std::ratio<-27315, 100>>();
using celsius_unit_type = std::remove_const_t<decltype(celsius_unit)>;
template<>
inline const std::string unit_string<celsius_unit> = "°C";

MAKE_UNIT_WITH_PREFIXES_DESC(lumen, candela_unit * steradian_unit, luminosity, "lm")
MAKE_UNIT_WITH_PREFIXES_DESC(lux, candela_unit / (meter_unit * meter_unit), luminosity, "lx")

/// \cond
namespace _detail 
{
struct becquerel_tag 
{

};

struct sievert_tag 
{

};
}
/// \endcond

constexpr unit auto becquerel_unit = hertz_unit.add_tag<_detail::becquerel_tag>();
template<>
inline const std::string unit_string<becquerel_unit> = "Bq";
using becquerel_unit_type = std::remove_const_t<decltype(becquerel_unit)>;
MAKE_METRIC_PREFIXES(becquerel_unit, time)

MAKE_UNIT_WITH_PREFIXES_DESC(gray, joule_unit / kilogram_unit, mass, "Gy")

constexpr unit auto sievert_unit = gray_unit.add_tag<_detail::sievert_tag>();
template<>
inline const std::string unit_string<sievert_unit> = "Sv";
using sievert_unit_type = std::remove_const_t<decltype(sievert_unit)>;
MAKE_METRIC_PREFIXES(sievert_unit, mass)

MAKE_UNIT_WITH_PREFIXES_DESC(katal, mole_unit / second_unit, amount, "kat")

MAKE_UNIT(meter_per_second, meter_unit / second_unit)
MAKE_UNIT(meter_per_second_per_second, meter_per_second_unit / second_unit)

MAKE_UNIT(square_meter, meter_unit * meter_unit)
MAKE_UNIT(square_centimeter, centimeter_unit * centimeter_unit)
MAKE_UNIT(square_kilometer, kilometer_unit * kilometer_unit)

MAKE_UNIT(cubic_meter, square_meter_unit * meter_unit)

MAKE_SCALED_UNIT_WITH_DESC(liter, cubic_meter_unit, (std::ratio<1, 1'000>), length, "L")

// Imperial units 
MAKE_SCALED_UNIT_WITH_DESC(foot, meter_unit, (std::ratio<10'000, 3'048>), length, "ft")
MAKE_SCALED_UNIT_WITH_DESC(inch, foot_unit, (std::ratio<12, 1>), length, "in")
MAKE_SCALED_UNIT_WITH_DESC(yard, foot_unit, (std::ratio<1, 3>), length, "yd")
MAKE_SCALED_UNIT_WITH_DESC(mile, foot_unit, (std::ratio<1, 5'280>), length, "mi")

MAKE_UNIT_WITH_DESC(square_foot, foot_unit * foot_unit, "ft^2")
MAKE_UNIT_WITH_DESC(square_inch, inch_unit * inch_unit, "in^2")
MAKE_UNIT_WITH_DESC(square_yard, yard_unit * yard_unit, "yd^2")
MAKE_UNIT_WITH_DESC(square_mile, mile_unit * mile_unit, "mi^2") 

MAKE_UNIT_WITH_DESC(cubic_foot, foot_unit * foot_unit * foot_unit, "ft^3")
MAKE_UNIT_WITH_DESC(cubic_inch, inch_unit * inch_unit * inch_unit, "in^3")
MAKE_UNIT_WITH_DESC(cubic_yard, yard_unit * yard_unit * yard_unit, "yd^3")
MAKE_UNIT_WITH_DESC(cubic_mile, mile_unit * mile_unit * mile_unit, "mi^3")

MAKE_SCALED_UNIT_WITH_DESC(pound, kilogram_unit, (std::ratio<100'000'000, 45'359'237>), mass, "lb")

MAKE_SCALED_UNIT_WITH_DESC(minute, second_unit, (std::ratio<1, 60>), time, "min")
MAKE_SCALED_UNIT_WITH_DESC(hour, minute_unit, (std::ratio<1, 60>), time, "hr")
MAKE_SCALED_UNIT_WITH_DESC(day, hour_unit, (std::ratio<1, 24>), time, "day")
MAKE_SCALED_UNIT_WITH_DESC(week, day_unit, (std::ratio<1, 7>), time, "week")
MAKE_SCALED_UNIT_WITH_DESC(year, day_unit, (std::ratio<1, 365>), time, "yr")

// clang-format on
constexpr unit auto fahrenheit_unit = celsius_unit.template adjust_scale_temperature<std::ratio<9, 5>>()
                                          .template adjust_offset_temperature<std::ratio<32>>();
// clang-format off
using fahrenheit_unit_type = std::remove_const_t<decltype(fahrenheit_unit)>;
template<>
inline const std::string unit_string<fahrenheit_unit> = "°F";

// clang-format on
#undef ESC
#endif
} // namespace maxwell

#endif