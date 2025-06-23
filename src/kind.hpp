#ifndef KIND_HPP
#define KIND_HPP

#include "dimensional_product.hpp"
#include "utility.hpp"

namespace maxwell {
template <string_literal Kind, auto Dim>
  requires dimension_product<decltype(Dim)>
struct kind_type {
  constexpr static dimension_product auto dimensions = Dim;

  constexpr static auto name() { return Kind; }
};

template <string_literal Kind, auto Dim>
kind_type<Kind, Dim> base_kind(kind_type<Kind, Dim>);

template <typename T>
using base_kind_t = decltype(base_kind(std::declval<T>()));

namespace _detail {
template <typename, typename = void> struct is_kind : std::false_type {};

template <typename T>
struct is_kind<T, std::void_t<base_kind_t<T>>> : std::true_type {};
} // namespace _detail

template <typename T>
concept kind = _detail::is_kind<std::remove_cvref_t<T>>::value;

template <typename From, typename To>
concept kind_convertible_to =
    std::derived_from<std::remove_cvref_t<From>, std::remove_cvref_t<To>>;

template <kind LHS, kind RHS> struct kind_product {
  using type = kind_type<LHS::name() + string_literal{"*"} + RHS::name(),
                         LHS::dimensions * RHS::dimensions>;
};

template <kind LHS, kind RHS>
using kind_product_t = kind_product<LHS, RHS>::type;

template <kind LHS, kind RHS> struct kind_quotient {
  using type = kind_type<LHS::name() + string_literal{"/"} + RHS::name(),
                         LHS::dimensions / RHS::dimensions>;
};

template <kind LHS, kind RHS>
using kind_quotient_t = kind_quotient<LHS, RHS>::type;

namespace _detail {
template <string_literal Derived, kind Base> struct derived_kind_impl : Base {
  constexpr static dimension_product auto dimensions = Base::dimensions;
  constexpr static auto name() { return Derived; }
};
} // namespace _detail

template <string_literal Derived, kind Base> struct make_derived_kind {
  using type = _detail::derived_kind_impl<Derived, Base>;
};

template <string_literal Derived, kind Base>
using make_derived_kind_t = make_derived_kind<Derived, Base>::type;
} // namespace maxwell

#endif