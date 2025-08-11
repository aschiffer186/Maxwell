/// \file quantity.hpp
/// \brief Definition of class template \c quantity_type

#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <concepts>    // derived_from
#include <type_traits> // false_type, remove_cvref_t, true_type
#include <utility>     // declval

#include "dimension.hpp"
#include "template_string.hpp"

namespace maxwell {
template <utility::template_string Kind, auto Dim>
  requires dimension_product<decltype(Dim)>
struct quantity_type {
  constexpr static dimension_product auto dimensions = Dim;
  constexpr static auto kind = Kind;
};

template <utility::template_string Kind, auto Dim>
auto quantity_base(quantity_type<Kind, Dim>) -> quantity_type<Kind, Dim>;

template <typename T>
using quantity_base_t = decltype(quantity_base(std::declval<T>()));

/// \cond
namespace _detail {
template <typename, typename = void> struct is_quantity : std::false_type {};

template <typename T>
struct is_quantity<T, std::void_t<quantity_base_t<T>>> : std::true_type {};
} // namespace _detail
/// \endcond

template <typename T>
concept quantity = _detail::is_quantity<std::remove_cvref_t<T>>::value;

template <auto From, auto To>
concept quantity_convertible_to =
    quantity<decltype(From)> && quantity<decltype(To)> &&
    dimension_convertible_to<From.dimensions, To.dimensions> &&
    std::derived_from<std::remove_cvref_t<decltype(From)>,
                      std::remove_cvref_t<decltype(To)>>;

template <quantity LHS, quantity RHS> struct quantity_product {
  using type =
      quantity_type<LHS::kind + utility::template_string{"*"} + RHS::kind,
                    LHS::dimensions * RHS::dimensions>;
};

template <quantity LHS, quantity RHS>
using quantity_product_t = quantity_product<LHS, RHS>::type;

template <quantity LHS, quantity RHS> struct quantity_quotient {
  using type =
      quantity_type<LHS::kind + utility::template_string{"/"} + RHS::kind,
                    LHS::dimensions / RHS::dimensions>;
};

template <quantity LHS, quantity RHS>
using quantity_quotient_t = quantity_quotient<LHS, RHS>::type;

constexpr quantity auto operator*(quantity auto lhs,
                                  quantity auto rhs) noexcept {
  return quantity_product_t<decltype(lhs), decltype(rhs)>{};
}

constexpr quantity auto operator/(quantity auto lhs,
                                  quantity auto rhs) noexcept {
  return quantity_quotient_t<decltype(lhs), decltype(rhs)>{};
}

/// \cond
namespace _detail {
template <utility::template_string Kind, auto Base>
  requires quantity<decltype(Base)>
struct derived_quantity_impl : decltype(Base) {
  constexpr static dimension_product auto dimensions = Base.dimensions;
  constexpr static auto kind = Kind;
};
} // namespace _detail
/// \endcond

template <utility::template_string Derived, auto Base>
struct make_derived_quantity {
  using type = _detail::derived_quantity_impl<Derived, Base>;
};

template <utility::template_string Derived, auto Base>
using make_derived_quantity_t = make_derived_quantity<Derived, Base>::type;
} // namespace maxwell

#endif