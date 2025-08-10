#ifndef QUANTITY_SYSTEM_HPP
#define QUANTITY_SYSTEM_HPP

#include "dimension.hpp"
#include "quantity.hpp"
#include "template_string.hpp"

namespace maxwell {
template <utility::template_string... Dimensions> struct quantity_system {
  template <utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_dimension = base_dimension_type<Name>;

  template <utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_dimension_product = dimension_product_type<base_dimension<Name>>;

  template <utility::template_string Kind, utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_quantity = quantity_type<Kind, base_dimension_product<Name>{}>;
};
} // namespace maxwell

#endif