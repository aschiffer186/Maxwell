#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>
#include <type_traits>

#include "dimensional_product.hpp"
#include "utility.hpp"

namespace maxwell {
template <auto DimensionalProduct, auto Multiplier, string_literal Name = "">
  requires dimension_product<decltype(DimensionalProduct)> &&
           rational<decltype(Multiplier)>
class unit_type;

namespace _detail {
template <typename> constexpr bool dependent_false = false;
}

template <auto DimensionalProduct, auto Multiplier, string_literal Name>
unit_type<DimensionalProduct, Multiplier, Name>
underlying_unit(unit_type<DimensionalProduct, Multiplier, Name>) {
  static_assert(
      _detail::dependent_false<decltype(Multiplier)>,
      "underlying_unit must not be evaluated. Are you using this function "
      "outside of decltype/noexept specifier/type trait/concept?");
}

template <typename T>
using underlying_unit_type = decltype(underlying_unit(std::declval<T>()));

namespace _detail {
template <typename, typename = void>
struct has_underlying_unit : std::false_type {};

template <typename T>
struct has_underlying_unit<T, std::void_t<underlying_unit_type<T>>>
    : std::true_type {};
} // namespace _detail

template <typename T>
concept unit = _detail::has_underlying_unit<std::remove_cvref_t<T>>::value;

namespace _detail {
template <unit LHS, unit RHS> struct unit_product_impl {
  using type = unit_type<LHS::dimensional_product * RHS::dimensional_product,
                         LHS::multiplier * RHS::multiplier>;
};

template <unit LHS, unit RHS> struct unit_quotient_impl {
  using type = unit_type<LHS::dimensional_product / RHS::dimensional_product,
                         LHS::multiplier / RHS::multiplier>;
};
} // namespace _detail

template <unit LHS, unit RHS>
struct unit_product : _detail::unit_product_impl<LHS, RHS>::type {
  constexpr static std::string unit_name() {
    std::string name = LHS::unit_name();
    name.append("*");
    name.append(RHS::unit_name());
    return name;
  }
};

template <unit LHS, unit RHS>
struct unit_quotient : _detail::unit_quotient_impl<LHS, RHS>::type {
  constexpr static std::string unit_name() {
    std::string name = LHS::unit_name();
    name.append("/");
    name.append(RHS::unit_name());
    return name;
  }
};

template <auto DimensionalProduct, auto Multiplier, string_literal Name>
  requires dimension_product<decltype(DimensionalProduct)> &&
           rational<decltype(Multiplier)>
struct unit_type {
  constexpr static dimension_product auto dimensional_product =
      DimensionalProduct;
  constexpr static rational auto multiplier = Multiplier;
  constexpr static string_literal name = Name;

  constexpr static std::string unit_name() {
    return std::string{name.begin(), name.end()};
  }
};

template <unit LHS, unit RHS> consteval unit auto operator*(LHS, RHS) {
  return unit_product<LHS, RHS>{};
}

template <rational LHS, unit RHS> consteval unit auto operator*(LHS, RHS) {
  return unit_type<RHS::dimensional_product, LHS{} * RHS::multiplier>{};
}

template <unit LHS, unit RHS> consteval unit auto operator/(LHS, RHS) {
  return unit_quotient<LHS, RHS>{};
}

template <auto Dimension, string_literal Name> struct make_base_unit {
  using type =
      unit_type<dimension_product_type<decltype(Dimension)>{}, one, Name>;
};

template <auto Dimension, string_literal Name>
using make_base_unit_t = make_base_unit<Dimension, Name>::type;

template <auto Definition, string_literal Name> struct make_derived_unit {
  using type =
      unit_type<Definition.dimensional_product, Definition.multiplier, Name>;
};

template <auto Definition, string_literal Name>
using make_derived_unit_t = make_derived_unit<Definition, Name>::type;

template <unit From, unit To>
consteval double conversion_factor(From, To) noexcept {
  return static_cast<double>(To::multiplier) /
         static_cast<double>(From::multiplier);
}

template <auto U>
concept dimensionless = unit<decltype(U)>;

template <auto From, auto To>
concept unit_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> &&
    From.dimensional_product == To.dimensional_product;
// ft*m -> nm^2 == (ft * (m / ft)) * m
// = (ft * m / ft * (nm / m)) * m
// = (ft * (m / ft) * (nm / m)) * (nm / m)
// = nm^2

// m * m == unit_product_t<m, m>
// If m^2 : make_unit_t<m * m, m^2>, underlying_unit_t<m^2> = unit_type<m * m,
// 1, m^2> != unit_product_t<m, m>.
} // namespace maxwell

#endif