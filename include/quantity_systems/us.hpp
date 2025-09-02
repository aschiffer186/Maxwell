/// \file us.hpp
/// \brief Definition of US customary units

#ifndef US_HPP
#define US_HPP

#include "si.hpp"
#include "utility/compile_time_math.hpp"

namespace maxwell::us {
namespace _detail {
constexpr auto meter_to_foot = value<3.2804>;
constexpr auto foot_to_inch = value<0.08333>;
constexpr auto foot_to_yard = value<3>;
} // namespace _detail

MODULE_EXPORT constexpr struct foot_unit_type
    : derived_unit<_detail::meter_to_foot * si::meter_unit, "ft"> {
} foot_unit;

MODULE_EXPORT constexpr struct inch_unit_type
    : derived_unit<_detail::foot_to_inch * foot_unit, "in"> {
} inch_unit;

template <typename T = double>
using foot = quantity_value<foot_unit, foot_unit.quantity, T>;

template <typename T = double>
using inch = quantity_value<inch_unit, inch_unit.quantity, T>;
} // namespace maxwell::us

#endif
