#ifndef OTHER_HPP
#define OTHER_HPP

#include "core/unit.hpp"
#include "isq.hpp"
#include "si.hpp"
#include "utility/compile_time_math.hpp"
#include "utility/config.hpp"

namespace maxwell::other {
namespace _detail {
constexpr auto min_to_sec = utility::value_type<60.0>{};
constexpr auto hour_to_min = utility::value_type<1.0 / 60.0>{};
constexpr auto day_to_hour = utility::value_type<1.0 / 24.0>{};
constexpr auto week_to_day = utility::value_type<1.0 / 7.0>{};
} // namespace _detail

namespace time {
MODULE_EXPORT constexpr struct minute_unit_type
    : derived_unit<_detail::min_to_sec * si::second_unit, "min"> {
} minute_unit;

MODULE_EXPORT constexpr struct hour_unit_type
    : derived_unit<_detail::hour_to_min * minute_unit, "hr"> {
} hour_unit;

MODULE_EXPORT constexpr struct day_unit_type
    : derived_unit<_detail::day_to_hour * hour_unit, "day"> {
} day_unit;

MODULE_EXPORT constexpr struct week_unit_type
    : derived_unit<_detail::week_to_day * day_unit, "week"> {
} week_unit;

MODULE_EXPORT template <typename T = double>
using minute = quantity_value<minute_unit, minute_unit.quantity, T>;
} // namespace time

namespace angle {}
} // namespace maxwell::other

#endif