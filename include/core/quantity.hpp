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
template <utility::template_string Kind, auto Dim, bool Derived>
  requires dimension_product<decltype(Dim)>
struct quantity_type {
  constexpr static dimension_product auto dimensions = Dim;
  constexpr static auto kind = Kind;
  constexpr static bool derived = Derived;
};

constexpr quantity_type<"[]", dimension_one, false> number;

template <utility::template_string Kind, auto Dim, bool Derived>
auto quantity_base(quantity_type<Kind, Dim, Derived>)
    -> quantity_type<Kind, Dim, Derived>;

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

template <quantity LHS, quantity RHS> struct quantity_product {
  using type =
      quantity_type<LHS::kind + utility::template_string{"*"} + RHS::kind,
                    LHS::dimensions * RHS::dimensions,
                    LHS::derived || RHS::derived>;
};

template <quantity LHS, quantity RHS>
using quantity_product_t = quantity_product<LHS, RHS>::type;

template <quantity LHS, quantity RHS> struct quantity_quotient {
  using type =
      quantity_type<LHS::kind + utility::template_string{"/"} + RHS::kind,
                    LHS::dimensions / RHS::dimensions,
                    LHS::derived || RHS::derived>;
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
template <utility::template_string Kind, auto Base, bool IsDerived = true>
  requires quantity<decltype(Base)>
struct derived_quantity_impl : std::remove_cvref_t<decltype(Base)> {
  constexpr static dimension_product auto dimensions = Base.dimensions;
  constexpr static auto kind = Kind;
  constexpr static bool derived = IsDerived;
};
} // namespace _detail
/// \endcond

template <utility::template_string Derived, auto Base, bool IsDerived = true>
struct make_derived_quantity {
  using type = _detail::derived_quantity_impl<Derived, Base, IsDerived>;
};

template <utility::template_string Derived, auto Base, bool IsDerived = true>
using make_derived_quantity_t =
    make_derived_quantity<Derived, Base, IsDerived>::type;

/// \cond
namespace _detail {
template <utility::template_string Derived, auto Base, bool IsDerived>
auto derived_base(_detail::derived_quantity_impl<Derived, Base, IsDerived>)
    -> _detail::derived_quantity_impl<Derived, Base, IsDerived>;

template <typename T>
using derived_base_t = decltype(derived_base(std::declval<T>()));

template <typename, typename = void>
struct has_derived_base : std::false_type {};

template <typename T>
struct has_derived_base<T, std::void_t<derived_base_t<T>>> : std::true_type {};

template <typename T> struct has_derived_base<const T> : has_derived_base<T> {};

template <quantity From, quantity To>
consteval bool quantity_convertible_to_impl(From, To) noexcept {
  // If dimensions size > 1 we know it's a product or a quotient
  const std::unsigned_integral auto from_dim_size =
      std::tuple_size_v<decltype(From::dimensions.as_tuple())>;

  if (from_dim_size > 1 && !has_derived_base<From>::value) {
    return From::dimensions == To::dimensions;
  } else if (!From::derived && !To::derived) {
    return From::dimensions == To::dimensions;
  } else if (!From::derived && To::derived) {
    return false;
  } else if (From::derived && !To::derived) {
    return std::derived_from<From, To> && From::dimensions == To::dimensions;
  } else {
    return std::derived_from<From, To> && From::dimensions == To::dimensions;
  }
}
} // namespace _detail
/// \endcond

template <auto From, auto To>
concept quantity_convertible_to =
    _detail::quantity_convertible_to_impl(From, To);
} // namespace maxwell

#endif