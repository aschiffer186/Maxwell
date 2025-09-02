#ifndef SCALE_HPP
#define SCALE_HPP

#include <concepts> // same_a

#include "utility/config.hpp"
#include "utility/type_traits.hpp"

namespace maxwell {
MODULE_EXPORT template <typename T, typename V>
concept scale = requires(T, V v) {
  { T::to_linear(v) } -> std::same_as<V>;
  { T::to_non_linear(v) } -> std::same_as<V>;
};

MODULE_EXPORT struct linear_scale_type {
  static constexpr auto to_linear(const auto& value) noexcept { return value; }

  static constexpr auto to_non_linear(const auto& value) noexcept {
    return value;
  }
};

template <typename T>
concept linear_scale = utility::similar<T, linear_scale_type>;

MODULE_EXPORT struct decibel_scale_type {
  static constexpr auto to_linear(const auto& value) noexcept { return value; }

  static constexpr auto to_non_linear(const auto& value) noexcept {
    return value;
  }
};
} // namespace maxwell

#endif