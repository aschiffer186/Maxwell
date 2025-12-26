#ifndef SCALE_HPP
#define SCALE_HPP

#include <cmath>   // pow
#include <utility> // forward

#include "core/unit.hpp"
#include "utility/compile_time_math.hpp"

namespace maxwell {
template <auto FromScale, auto ToScale> struct scale_converter {
  template <auto FromUnit, auto ToUnit, typename U>
  static constexpr auto convert(U&& u) {
    constexpr double factor = conversion_factor(FromUnit, ToUnit);
    constexpr double offset = conversion_offset(FromUnit, ToUnit);
    return std::forward<U>(u) * factor + offset;
  }
};

template <> struct scale_converter<decibel_scale_type{}, linear_scale_type{}> {
  template <auto FromUnit, auto ToUnit, typename U>
  static constexpr auto convert(U&& u) {
    constexpr double factor = conversion_factor(FromUnit, ToUnit);
    constexpr double offset = conversion_offset(FromUnit, ToUnit);
    return std::pow(10.0, std::forward<U>(u) / 10.0) * factor + offset;
  }
};

template <> struct scale_converter<linear_scale_type{}, decibel_scale_type{}> {
  template <auto FromUnit, auto ToUnit, typename U>
  static constexpr auto convert(U&& u) {
    constexpr double factor = conversion_factor(FromUnit, ToUnit);
    constexpr double offset = conversion_offset(FromUnit, ToUnit);
    return 10.0 * utility::log10(std::forward<U>(u) * factor + offset);
  }
};

} // namespace maxwell

#endif