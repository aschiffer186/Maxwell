/// \file quantity_system.hpp
/// \brief Definition of \c quantity_system helper class template

#ifndef QUANTITY_SYSTEM_HPP
#define QUANTITY_SYSTEM_HPP

#include "dimension.hpp"
#include "quantity.hpp"
#include "template_string.hpp"
#include "unit.hpp"

namespace maxwell {
template <utility::template_string... Dimensions> struct quantity_system {
  template <utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_dimension = base_dimension_type<Name>;

  template <utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_dimension_product = dimension_product_type<base_dimension<Name>>;

  template <utility::template_string Kind>
    requires((Kind == Dimensions) || ...)
  using base_quantity =
      quantity_type<Kind, base_dimension_product<Kind>{}, false>;

  using dimensionless_quantity = quantity_type<
      "[]", dimension_product_type<dimension_type<"Null", utility::zero>>{},
      false>;

  template <utility::template_string Name, auto Definition>
  struct make_derived {
    using quantity = make_derived_quantity_t<Name, Definition>;
    using unit = make_derived_unit_t<Name, Definition>;
  };
};
} // namespace maxwell

#endif