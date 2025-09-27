/// \file us.hpp
/// \brief Definition of US customary units

#ifndef US_HPP
#define US_HPP

#include "si.hpp"
#include "utility/compile_time_math.hpp"
#include "utility/config.hpp"

namespace maxwell::us {
namespace _detail {
constexpr auto meter_to_foot = value<1 / 0.3048>;
constexpr auto foot_to_inch = value<12.0>;
constexpr auto foot_to_yard = value<1.0 / 3.0>;
constexpr auto foot_to_mile = value<1.0 / 5'280.0>;
// constexpr auto
constexpr auto celsius_to_fahrenheit = value<1.8>;
constexpr auto celsius_to_fahrenheit_offset = value<32.0>;
} // namespace _detail

MODULE_EXPORT constexpr struct foot_unit_type
    : derived_unit<_detail::meter_to_foot * si::meter_unit, "ft"> {
} foot_unit;

MODULE_EXPORT constexpr struct inch_unit_type
    : derived_unit<_detail::foot_to_inch * foot_unit, "in"> {
} inch_unit;

MODULE_EXPORT constexpr struct yard_unit_type
    : derived_unit<_detail::foot_to_yard * foot_unit, "yd"> {
} yard_unit;

MODULE_EXPORT constexpr struct mile_unit_type
    : derived_unit<_detail::foot_to_mile * foot_unit, "mi"> {
} mile_unit;

MODULE_EXPORT constexpr struct fahrenheit_unit_type
    : derived_unit<_detail::celsius_to_fahrenheit * si::celsius_unit +
                       _detail::celsius_to_fahrenheit_offset,
                   "°F"> {
} fahrenheit_unit;

MODULE_EXPORT template <typename T = double>
using foot = quantity_value<foot_unit, foot_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using inch = quantity_value<inch_unit, inch_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using yard = quantity_value<yard_unit, yard_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using mile = quantity_value<mile_unit, mile_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using fahrenheit = quantity_value<fahrenheit_unit, fahrenheit_unit.quantity, T>;
} // namespace maxwell::us

#endif
