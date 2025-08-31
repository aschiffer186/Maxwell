/// \file us.hpp
/// \brief Definition of US customary units

#ifndef US_HPP
#define US_HPP

#include "si.hpp"
#include "utility/compile_time_math.hpp"

namespace maxwell::us {
namespace _detail {
constexpr auto meter_to_foot = utility::value_type<3.2804>{};
constexpr auto foot_to_inch = utility::value_type<0.08333>{};
constexpr auto foot_to_yard = utility::value_type<3>{};
} // namespace _detail

constexpr struct foot_unit_type
    : make_derived_unit_t<_detail::meter_to_foot * si::meter_unit, "ft"> {
} foot_unit;

constexpr struct inch_unit_type
    : make_derived_unit_t<_detail::foot_to_inch * foot_unit, "in"> {
} inch_unit;
} // namespace maxwell::us

#endif
