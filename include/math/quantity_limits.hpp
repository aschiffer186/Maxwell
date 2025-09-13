/// \file quantity_limits.hpp
/// \brief Provides specialization of \c std::numeric_limits for instantiations
/// of \c quantity_value

#ifndef QUANTITY_LIMITS_HPP
#define QUANTITY_LIMITS_HPP

#include <limits>

#include "core/quantity_value.hpp"
#include "utility/config.hpp"

/// \brief Specialization of \c std::numeric_limits for instantiations of \c
/// quantity_value
///
/// Specialization of \c std::numeric_limits for \c quantity_value types. The
/// values of this class are the same as the values of \c std::numeric_limits<T>
/// but wrapped in the
/// \c quantity_value type for type safety.
///
/// \tparam U The units of the \c quantity_value
/// \tparam Q The quantity type of the \c quantity_value
/// \tparam T The type of the numerical value of the \c quantity_value
MODULE_EXPORT template <auto U, auto Q, typename T>
struct std::numeric_limits<maxwell::quantity_value<U, Q, T>> {
private:
  using base_t = std::numeric_limits<T>;
  using self_t = maxwell::quantity_value<U, Q, T>;

public:
  /// \c true if \c std::numeric_limits<T> is specialized.
  static constexpr bool is_specialized = base_t::is_specialized;
  /// Equivalent to \c std::numeric_limits<T>::is_signed
  static constexpr bool is_signed = base_t::is_signed;
  /// Equivalent to \c std::numeric_limits<T>::is_integer
  static constexpr bool is_integer = base_t::is_integer;
  /// Equivalent to \c std::numeric_limits<T>::is_floating_point
  static constexpr bool is_floating_point = base_t::is_floating_point;
  /// Equivalent to \c std::numeric_limits<T>::is_exact
  static constexpr bool is_exact = base_t::is_exact;
  /// Equivalent to \c std::numeric_limits<T>::has_infinity
  static constexpr bool has_infinity = base_t::has_infinity;
  /// Equivalent to \c std::numeric_limits<T>::has_quiet_NaN
  static constexpr bool has_quiet_NaN = base_t::has_quiet_NaN;
  /// Equivalent to \c std::numeric_limits<T>::has_signaling_NaN
  static constexpr bool has_signaling_NaN = base_t::has_signaling_NaN;
#ifndef MAXWELL_HAS_CXX23
  /// Equivalent to \c std::numeric_limits<T>::has_denorm
  static constexpr float_denorm_style has_denorm = base_t::has_denorm;
#else
  /// Equivalent to \c std::numeric_limits<T>::has_denorm
  [[deprecated(
      "Deprecated in C++23")]] static constexpr float_denorm_style has_denorm =
      base_t::has_denorm;
#endif
  /// Equivalent to \c std::numeric_limits<T>::has_denorm_loss
  static constexpr bool has_denorm_loss = base_t::has_denorm_loss;
  /// Equivalent to \c std::numeric_limits<T>::round_style
  static constexpr float_round_style round_style = base_t::round_style;
  /// Equivalent to \c std::numeric_limits<T>::is_iec559
  static constexpr bool is_iec559 = base_t::is_iec559;
  /// Equivalent to \c std::numeric_limits<T>::is_bounded
  static constexpr bool is_bounded = base_t::is_bounded;
  /// Equivalent to \c std::numeric_limits<T>::is_modulo
  static constexpr bool is_modulo = base_t::is_modulo;
  /// Equivalent to \c std::numeric_limits<T>::digits
  static constexpr int digits = base_t::digits;
  /// Equivalent to \c std::numeric_limits<T>::digits10
  static constexpr int digits10 = base_t::digits10;
  /// Equivalent to \c std::numeric_limits<T>::max_digits10
  static constexpr int max_digits10 = base_t::max_digits10;
  /// Equivalent to \c std::numeric_limits<T>::radix
  static constexpr int radix = base_t::radix;
  /// Equivalent to \c std::numeric_limits<T>::min_exponent
  static constexpr int min_exponent = base_t::min_exponent;
  /// Equivalent to \c std::numeric_limits<T>::min_exponent10
  static constexpr int min_exponent10 = base_t::min_exponent10;
  /// Equivalent to \c std::numeric_limits<T>::max_exponent
  static constexpr int max_exponent = base_t::max_exponent;
  /// Equivalent to \c std::numeric_limits<T>::max_exponent10
  static constexpr int max_exponent10 = base_t::max_exponent10;
  /// Equivalent to \c std::numeric_limits<T>::traps
  static constexpr bool traps = base_t::traps;
  /// Equivalent to \c std::numeric_limits<T>::tinyness_before
  static constexpr bool tinyness_before = base_t::tinyness_before;

  /// \brief Returns the minimum value the \c quantity_value can represent
  ///
  /// Returns the minimum value the \c quantity_value can represent. Equivalent
  /// to <tt>quantity_value<U, Q, T>(std::numeric_limits<T>::min())</tt>.
  ///
  /// \note If \c T is a floating-point type, this is different from \c min().
  ///
  /// \return The minimum value the \c quantity_value can represent.
  static constexpr auto min() noexcept -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::min()};
  }

  /// \brief Returns the lowest value the \c quantity_value can represent
  ///
  /// Returns the lowest value the \c quantity_value can represent. Equivalent
  /// to <tt>quantity_value<U, Q, T>(std::numeric_limits<T>::lowest())</tt>.
  ///
  /// \note If \c T is a floating-point type, this is different from \c min().
  ///
  /// \return The lowest value the \c quantity_value can represent.
  static constexpr auto lowest() noexcept -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::lowest()};
  }

  /// \brief Returns the maximum value the \c quantity_value can represent
  ///
  /// Returns the maximum value the \c quantity_value can represent. Equivalent
  /// to <tt>quantity_value<U, Q, T>(std::numeric_limits<T>::max())</tt>.
  ///
  ///
  /// \return The maximum value the \c quantity_value can represent.
  static constexpr auto max() noexcept -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::max()};
  }

  /// \brief Returns the machine epsilon for the \c quantity_value
  ///
  /// Returns the machine epsilon for the \c quantity_value. Equivalent to
  /// <tt>quantity_value<U, Q, T>(std::numeric_limits<T>::epsilon())</tt>.
  ///
  /// \return The machine epsilon for the \c quantity_value.
  static constexpr auto epsilon() noexcept -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::epsilon()};
  }

  /// \brief Returns the largest rounding error for the \c quantity_value.
  ///
  /// Returns the largest rounding error possible in units in the last place
  /// (ULPs). Equivalent to <tt>quantity_value<U, Q,
  /// T>(std::numeric_limits<T>::round_error())</tt>.
  ///
  /// \return The rounding error for the \c quantity_value.
  static constexpr auto round_error() noexcept
      -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::round_error()};
  }

  /// \brief Returns a representation of positive infinity for the \c
  /// quantity_value.
  ///
  /// Returns a representation of positive infinity for the \c quantity_value.
  /// Equivalent to <tt>quantity_value<U, Q,
  /// T>(std::numeric_limits<T>::infinity())</tt>.
  ///
  /// \return A representation of positive infinity for the \c quantity_value.
  static constexpr auto infinity() noexcept
      -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::infinity()};
  }

  /// \brief Returns a quiet NaN (not-a-number) representation for the \c
  /// quantity_value.
  ///
  /// Returns a quiet NaN (not-a-number) representation for the \c
  /// quantity_value. Equivalent to <tt>quantity_value<U, Q,
  /// T>(std::numeric_limits<T>::quiet_NaN())</tt>.
  ///
  /// \return A quiet NaN representation for the \c quantity_value.
  static constexpr auto quiet_NaN() noexcept
      -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::quiet_NaN()};
  }

  /// \brief Returns a signaling NaN (not-a-number) representation for the \c
  /// quantity_value.
  ///
  /// Returns a signaling NaN (not-a-number) representation for the \c
  /// quantity_value. Equivalent to <tt>quantity_value<U, Q,
  /// T>(std::numeric_limits<T>::signaling_NaN())</tt>.
  ///
  /// \return A signaling NaN representation for the \c quantity_value.
  static constexpr auto signal_NaN() noexcept
      -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::signal_NaN()};
  }

  /// \brief Returns the smallest positive subnormal value for the \c
  /// quantity_value.
  ///
  /// Returns the smallest positive subnormal value for the \c quantity_value.
  /// Equivalent to <tt>quantity_value<U, Q,
  /// T>(std::numeric_limits<T>::denorm_min())</tt>.
  ///
  /// \return The smallest positive subnormal value for the \c quantity_value.
  static constexpr auto denorm_min() noexcept
      -> maxwell::quantity_value<U, Q, T> {
    return self_t{base_t::denorm_min()};
  }
};

#endif