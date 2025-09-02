/// \file quantity_system.hpp
/// \brief Definition of \c quantity_system helper class template

#ifndef QUANTITY_SYSTEM_HPP
#define QUANTITY_SYSTEM_HPP

#include "dimension.hpp"
#include "quantity.hpp"
#include "utility/template_string.hpp"

namespace maxwell {
/// \brief Defines a quantity system.
///
/// Helper class template to define a quantity system. Provides definitions
/// for all base quantities in the base system as less as a dimensioness
/// quantity (quantity of dimension one/number). It is highly recommended that
/// this class template be used to define a new quantity system to avoid relying
/// on implementation details that are subject to change.
///
/// \tparam Dimension The dimensions the base quantities will represent.
MODULE_EXPORT template <utility::template_string... Dimensions>
struct quantity_system {
  /// Type alias for a base dimension in the system.
  /// \tparam Name The name of the dimension
  template <utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_dimension = base_dimension_type<Name>;

  /// Type alias for a dimension product representing a base dimension.
  /// \tparam Name The name of the base dimension.
  template <utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_dimension_product = dimension_product_type<base_dimension<Name>>;

  /// Type alias for a base quantity in the quantity system.
  /// All quantities in the system should be defined from instantiations of this
  /// type alias to ensure the system is consistent.
  ///
  /// \tparam Name The name of the quantity.
  template <utility::template_string Name>
    requires((Name == Dimensions) || ...)
  using base_quantity =
      quantity_type<Name, base_dimension_product<Name>{}, false>;

  /// Type alias representing the quantity of dimension one in the system.
  /// This ensures this quantity is uniquely defined.
  using dimensionless_quantity = quantity_type<"[]", dimension_one, false>;
};
} // namespace maxwell

#endif