#ifndef QUANTITY_LIMITS_HPP
#define QUANTITY_LIMITS_HPP

#include <limits>

#include "config.hpp"
#include "quantity_value.hpp"

template <auto U, auto Q, typename T>
struct std::numeric_limits<maxwell::quantity_value<U, Q, T>> {
private:
  using base_t = std::numeric_limits<T>;
  using self_t = maxwell::quantity_value<U, Q, T>;

public:
  /// \brief \c true since \c std::numeric_limits is specialized for \c
  /// quantity_value.
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = base_t::is_signed;
  static constexpr bool is_integer = base_t::is_integer;
  static constexpr bool is_exact = base_t::is_exact;
  static constexpr bool has_infinity = base_t::has_infinity;
  static constexpr bool has_quiet_NaN = base_t::has_quiet_NaN;
  static constexpr bool has_signaling_NaN = base_t::has_signaling_NaN;
#ifndef MAXWELL_HAS_CXX23
  static constexpr float_denorm_style has_denorm = base_t::has_denorm;
#else
  [[deprecated(
      "Deprecated in C++23")]] static constexpr float_denorm_style has_denorm =
      base_t::has_denorm;
#endif
  static constexpr bool has_denorm_loss = base_t::has_denorm_loss;
  static constexpr float_round_style round_style = base_t::float_round_style;
  static constexpr bool is_iec559 = base_t::is_iec559;
  static constexpr bool is_bounded = base_t::is_bounded;
  static constexpr bool is_modulo = base_t::is_modulo;
};

#endif