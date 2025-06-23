#ifndef QUANTITY_SYSTEM_HPP
#define QUANTITY_SYSTEM_HPP

#include "dimensional_product.hpp"
#include "kind.hpp"
#include "unit.hpp"
#include "utility.hpp"

namespace maxwell {

template <string_literal... BaseQuantityNames> struct unit_system {
  template <string_literal Name>
    requires((Name == BaseQuantityNames) || ...)
  using base_dimension_type = base_dimension_type<Name>;

  template <string_literal Name>
  constexpr static inline base_dimension_type<Name> base_dimension{};

  constexpr static auto dimension_one =
      (null_dimension_type<base_dimension<BaseQuantityNames>>{} * ...);

  template <string_literal Name>
    requires((Name == BaseQuantityNames) || ...)
  using base_kind =
      kind_type<Name, dimension_product_type<base_dimension_type<Name>>{}>;

  template <string_literal QuantityName, string_literal UnitName>
  using base_unit = make_base_unit_t<base_dimension<QuantityName>, UnitName>;

  template <string_literal UnitName>
  using one_unit = unit_type<dimension_one, one, UnitName>;
};
} // namespace maxwell

#endif