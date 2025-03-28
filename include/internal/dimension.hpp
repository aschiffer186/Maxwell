#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include "config.hpp"
#include "utility.hpp"
#include <cstdint>

namespace maxwell {
/// @brief Class template representing one dimension of a unit
///
/// Class template \c dimension represents one dimension in terms of the
/// corresponding SI base unit. A dimension is given by 10^prefix * scale *
/// SI-BASE-UNIT^power + offset.
///
struct dimension {
  /// The power of the dimension
  rational power;
  /// The metric prefix of the dimension
  std::int8_t prefix;
  /// The scale factor of the dimension
  rational scale{one};
  /// The offset of the dimension
  rational offset{zero};

  /// @brief Returns a new dimension with its prefix adjusted
  ///
  /// Returns a new dimension that is the same as \c *this but with the
  /// specified
  /// prefix.
  ///
  ///
  /// @param new_prefix the new prefix for the dimension
  /// @return a new dimension with its prefix adjusted
  constexpr dimension adjust_prefix(std::int8_t new_prefix) const noexcept {
    return dimension{power, new_prefix, scale, offset};
  }

  /// @brief Returns a new dimension with its scale adjusted
  ///
  /// Returns a dimension that is the same as \c *this except its scale has
  /// been multiplied by the specified amount
  ///
  /// @param multiplier the amount to multiply the scale by
  /// @return a new dimension with its scale adjusted
  constexpr dimension adjust_scale(const rational& multiplier) const noexcept {
    return dimension{power, prefix, scale * multiplier, offset};
  }

  /// @brief Returns a new dimension with its offest adjusted
  ///
  /// Returns a dimension that is the same as \c *this except its with the
  /// specified value added to its offset
  ///
  /// @param addend the amount to the add to the offset
  /// @return a new dimension with its offest adjusted
  constexpr dimension adjust_offset(const rational& addend) const noexcept {
    return dimension{power, prefix, scale, offset * scale + addend};
  }

  /// @brief Returns the equivalent SI base dimension
  ///
  /// Returns the equivalent SI base dimension of \c *this, i.e, the
  /// dimension with the same power as \c *this, but with no scaling or offset.
  ///
  /// @return the equivalent SI base dimension
  /// @throws nothing
  constexpr dimension to_SI_base_dimension() const noexcept {
    return {power, 0, one, zero};
  }

  friend constexpr bool operator==(const dimension&,
                                   const dimension&) = default;
};

constexpr std::int8_t quetta = 30;
constexpr std::int8_t ronna = 27;
constexpr std::int8_t yotta = 24;
constexpr std::int8_t zetta = 21;
constexpr std::int8_t exa = 18;
constexpr std::int8_t peta = 15;
constexpr std::int8_t tera = 12;
constexpr std::int8_t giga = 9;
constexpr std::int8_t mega = 6;
constexpr std::int8_t kilo = 3;
constexpr std::int8_t hecto = 2;
constexpr std::int8_t deca = 1;
constexpr std::int8_t deci = -1;
constexpr std::int8_t centi = -2;
constexpr std::int8_t milli = -3;
constexpr std::int8_t micro = -6;
constexpr std::int8_t nano = -9;
constexpr std::int8_t pico = -12;
constexpr std::int8_t femto = -15;
constexpr std::int8_t atto = -18;
constexpr std::int8_t zepto = -21;
constexpr std::int8_t yocto = -24;
constexpr std::int8_t ronto = -27;
constexpr std::int8_t quecto = -30;

constexpr dimension null_dimension{zero, 0};
constexpr dimension base_dimension{one, 0};

/// \brief Multiplies two dimensions
///
/// Multiplies two dimensions. The resulting dimensions has a power that is the
/// sum of the power of \c lhs and \c rhs. If the two inputs have the same
/// dimensions, scale factor, and offset, they are copied to the resulting unit.
/// Otherwise, the returned unit is a coherent unit.
///
/// \param lhs one dimension to multiply
/// \param rhs the other dimensin to multiply
/// \return the product of \c lhs and \c rhs
constexpr dimension operator*(const dimension& lhs,
                              const dimension& rhs) noexcept {

  const std::int8_t prefix = (lhs.prefix == rhs.prefix)
                                 ? lhs.prefix
                                 : ((rhs.prefix == 0)   ? lhs.prefix
                                    : (lhs.prefix == 0) ? rhs.prefix
                                                        : 0);

  return dimension{lhs.power + rhs.power, prefix, lhs.scale * rhs.scale,
                   rhs.scale};
}

/// \brief Divides two dimensions
///
/// Dividies two dimensions. The resulting dimension has a power that is the
/// difference of the power of \c lhs and \c rhs. If the two inputs have the
/// same prefix, scale factor, and offset, they are copied to the resulting
/// unit. Otherwise, the returned unit is a coherent unit.
///
/// \param lhs the dividend
/// \param rhs the divisor
/// \return the quotient of \c lhs and \c rhs
constexpr dimension operator/(const dimension& lhs,
                              const dimension& rhs) noexcept {
  const std::int8_t prefix = (lhs.prefix == rhs.prefix)
                                 ? lhs.prefix
                                 : ((rhs.prefix == 0)   ? lhs.prefix
                                    : (lhs.prefix == 0) ? rhs.prefix
                                                        : 0);
  return dimension{lhs.power - rhs.power, prefix, lhs.scale / rhs.scale,
                   lhs.scale};
}
} // namespace maxwell

#endif