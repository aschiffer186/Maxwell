#ifndef SCALE_HPP
#define SCALE_HPP

#include <concepts> // same_a

#include "utility/config.hpp"
#include "utility/type_traits.hpp"

namespace maxwell {
struct linear_scale_type;
struct decibel_scale_type;

MODULE_EXPORT template <typename T, typename V>
concept scale =
    requires(T, V v, const linear_scale_type& l, const decibel_scale_type& d) {
      { T::from_scale(v, l) } -> std::same_as<V>;
      { T::from_scale(v, d) } -> std::same_as<V>;
    };

MODULE_EXPORT struct linear_scale_type {
  static constexpr auto from_scale(const auto& value,
                                   const linear_scale_type&) noexcept {
    return value;
  }

  static constexpr auto from_scale(const auto& value,
                                   const decibel_scale_type&) noexcept {
    return value;
  }
};

MODULE_EXPORT struct decibel_scale_type {
  static constexpr auto from_scale(const auto& value,
                                   const linear_scale_type&) noexcept {
    return value;
  }

  static constexpr auto from_scale(const auto& value,
                                   const decibel_scale_type&) noexcept {
    return value;
  }
};

} // namespace maxwell

#endif