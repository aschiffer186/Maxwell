#ifndef QUANTITY_VECTOR_HPP
#define QUANTITY_VECTOR_HPP

#include "unit.hpp"
#include <cstddef>

namespace maxwell {
template <std::size_t NumRows, std::size_t NumColumns, unit auto... Units>
  requires(sizeof...(Units) == NumRows * NumColumns)
struct unit_matrix {
  std::tuple<decltype(Units)...> units;
};
} // namespace maxwell

#endif