#ifndef QUANTITY_MATH_HPP
#define QUANTITY_MATH_HPP

#include <cmath>

#include "config.hpp"
#include "internal/unit.hpp"
#include "quantity.hpp"

namespace maxwell {
template <unit auto U, typename T>
MAXWELL_MATH_CONSTEXPR26 quantity<unit_sqrt_type<decltype(U)>{}, T> sqrt(const quantity<U, T>& q) MATH_NOEXCEPT(sqrt) {
  return quantity<unit_sqrt_type<decltype(U)>{}, T>(std::sqrt(q.get_magnitude()));
}
} // namespace maxwell

#endif