#ifndef OTHER_HPP
#define OTHER_HPP

#include "core/scale.hpp"
#include "core/unit.hpp"
#include "isq.hpp"
#include "si.hpp"
#include "utility/compile_time_math.hpp"
#include "utility/config.hpp"

namespace maxwell::other {
namespace _detail {
constexpr auto sec_to_min = utility::value_type<1.0 / 60.0>{};
constexpr auto min_to_hour = utility::value_type<1.0 / 60.0>{};
constexpr auto hour_to_day = utility::value_type<1.0 / 24.0>{};
constexpr auto day_to_week = utility::value_type<1.0 / 7.0>{};
constexpr auto week_to_year = utility::value_type<1.0 / 52.1429>{};
} // namespace _detail

namespace time {
MODULE_EXPORT constexpr struct minute_unit_type
    : derived_unit<_detail::sec_to_min * si::second_unit, "min"> {
} minute_unit;

MODULE_EXPORT constexpr struct hour_unit_type
    : derived_unit<_detail::min_to_hour * minute_unit, "hr"> {
} hour_unit;

MODULE_EXPORT constexpr struct day_unit_type
    : derived_unit<_detail::hour_to_day * hour_unit, "day"> {
} day_unit;

MODULE_EXPORT constexpr struct week_unit_type
    : derived_unit<_detail::day_to_week * day_unit, "week"> {
} week_unit;

MODULE_EXPORT constexpr struct year_unit_type
    : derived_unit<_detail::week_to_year * week_unit, "year"> {
} year_unit;

MODULE_EXPORT template <typename T = double>
using minute = quantity_value<minute_unit, minute_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using hour = quantity_value<hour_unit, hour_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using day = quantity_value<day_unit, day_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using week = quantity_value<week_unit, week_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using year = quantity_value<year_unit, year_unit.quantity, T>;

namespace symbols {
MODULE_EXPORT constexpr unit auto min = minute_unit;
MODULE_EXPORT constexpr unit auto hr = hour_unit;
MODULE_EXPORT constexpr unit auto day = day_unit;
MODULE_EXPORT constexpr unit auto week = week_unit;
MODULE_EXPORT constexpr unit auto yr = year_unit;
} // namespace symbols
} // namespace time

namespace angle {
MODULE_EXPORT constexpr struct arcminute_unit_type
    : derived_unit<value<60.0> * si::degree_unit, "arcmin"> {
} arcminute_unit;

MODULE_EXPORT constexpr struct arcsecond_unit_type
    : derived_unit<value<60.0> * arcminute_unit, "arcs"> {
} arcsecond_unit;

namespace symbols {
MODULE_EXPORT constexpr unit auto min = arcminute_unit;
MODULE_EXPORT constexpr unit auto s = arcsecond_unit;
} // namespace symbols
} // namespace angle

namespace chemical {
struct ph_scale_type {};

MODULE_EXPORT constexpr struct molar_unit_type
    : derived_unit<si::mole_unit / si::cubic_meter_unit, "M"> {
} molar_unit;

struct ph_unit_type
    : unit_type<"pH", molar_unit.quantity, 1.0, 0.0, ph_scale_type> {};
constexpr ph_unit_type ph_unit{};

MODULE_EXPORT template <typename T = double>
using molar = quantity_value<molar_unit, molar_unit.quantity, T>;

MODULE_EXPORT template <typename T = double>
using ph = quantity_value<ph_unit, ph_unit.quantity, T>;
} // namespace chemical
} // namespace maxwell::other

namespace maxwell {
template <>
struct scale_converter<linear_scale_type{}, other::chemical::ph_scale_type{}> {
  template <auto FromUnit, auto ToUnit, typename U>
  static constexpr auto convert(U&& u) {
    constexpr double factor = conversion_factor(FromUnit, ToUnit);
    constexpr double offset = conversion_offset(FromUnit, ToUnit);
    return -utility::log10(std::forward<U>(u) * factor + offset);
  }
};

template <>
struct scale_converter<other::chemical::ph_scale_type{}, linear_scale_type{}> {
  template <auto FromUnit, auto ToUnit, typename U>
  static constexpr auto convert(U&& u) {
    constexpr double factor = conversion_factor(FromUnit, ToUnit);
    constexpr double offset = conversion_offset(FromUnit, ToUnit);
    return (std::pow(10.0, -std::forward<U>(u))) / factor + offset;
  }
};
} // namespace maxwell

#endif