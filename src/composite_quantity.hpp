#ifndef COMPOSITE_QUANTITY_HPP
#define COMPOSITE_QUANTITY_HPP

#include "unit.hpp"

namespace maxwell {
template <typename Rep, auto... Units>
  requires(unit<decltype(Units)> && ...)
class composite_quantity {};
} // namespace maxwell

#endif