/// \file quantity.hpp
/// \brief Definition of class template \c quantity_type

#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <concepts>    // derived_from
#include <type_traits> // false_type, remove_cvref_t, true_type
#include <utility>     // declval

#include "dimension.hpp"
#include "utility/template_string.hpp"

namespace maxwell {
constexpr auto number_kind = utility::template_string{"[]"};

/// \brief Represents a physical quantity.
///
/// Class template \c quantity_type represents a physical quantity as defined
/// in ISQ-80000.
/// The \c quantity_type class template is the main class involved in the
/// compile-time verification of units in Maxwell.
/// When two instances of \c quantity_type do not model \c
/// quantity_convertible_to, it is not possible to assign them to each other,
/// compare them, or add or subtract them.
///
/// An instance of \c quantity_type can represent a base quantity or a derived
/// quantity.
/// If it is a base quantity, it will have a dimension of one for
/// exactly one base quantity and zero for all others.
/// Derived quantities can be created using the \c make_derived_quantity_t type
/// alias. This allows for creating quantities from other quantities using
/// arithmetic operations and for creating derived quantities
///  with the same dimensions as but incompatible with the base quantity.
///
/// \tparam Kind A string representing the kind of the quantity.
/// \tparam Dim The dimensions of the quantity.
/// \tparam Derived Whether the quantity is a user-defined derived quantity.
MODULE_EXPORT template <utility::template_string Kind, auto Dim, bool Derived>
  requires dimension_product<decltype(Dim)>
struct quantity_type {
  /// The dimensions of the quantity.
  constexpr static dimension_product auto dimensions = Dim;
  /// The kind of the quantity.
  constexpr static auto kind = Kind;
  /// A flag indicating if the quantity is a user-defined derived quantity.
  constexpr static bool derived = Derived;

  /// \brief Returns the sum of the exponents of quantity's dimensions.
  ///
  /// Returns the sum of the exponents of the quantity's dimensions. Note,
  /// if this is a derived quantity, this function will never return zero
  /// even if the quantity has dimensions of number.
  ///
  /// \brief Return the sum of the exponents of the quantity's dimensions.
  constexpr static utility::rational auto dimension_sum() {
    constexpr utility::rational auto sum_temp =
        dimensions.dimension_exponent_sum();
    if constexpr ((sum_temp == utility::zero) && (Kind != number_kind)) {
      return utility::one;
    } else {
      return sum_temp;
    }
  }
};

/// Constant representing a quantity to represents a number
MODULE_EXPORT constexpr quantity_type<number_kind, dimension_one, false> number;

/// \cond
namespace _detail {
template <utility::template_string Kind, auto Dim, bool Derived>
auto quantity_base(quantity_type<Kind, Dim, Derived>)
    -> quantity_type<Kind, Dim, Derived>;

template <typename T>
using quantity_base_t = decltype(quantity_base(std::declval<T>()));

template <typename, typename = void> struct is_quantity : std::false_type {};

template <typename T>
struct is_quantity<T, std::void_t<quantity_base_t<T>>> : std::true_type {};
} // namespace _detail
/// \endcond

/// Concept indicating that a type models an ISO-80000 quantity.
///
/// \tparam T The type to check
MODULE_EXPORT template <typename T>
concept quantity = _detail::is_quantity<std::remove_cvref_t<T>>::value;

/// \cond
namespace _detail {
struct quantity_product_tag {};

struct quantity_quotient_tag {};

template <quantity LHS, quantity RHS> struct quantity_product_impl {
  using type =
      quantity_type<LHS::kind + utility::template_string{"*"} + RHS::kind,
                    LHS::dimensions * RHS::dimensions,
                    LHS::derived || RHS::derived>;
};

template <quantity LHS, quantity RHS>
struct quantity_product : quantity_product_impl<LHS, RHS>::type,
                          quantity_product_tag {};

template <quantity LHS, quantity RHS> struct quantity_quotient_impl {
  using type =
      quantity_type<LHS::kind + utility::template_string{"*"} + RHS::kind,
                    LHS::dimensions / RHS::dimensions,
                    LHS::derived || RHS::derived>;
};

template <quantity LHS, quantity RHS>
struct quantity_quotient : quantity_quotient_impl<LHS, RHS>::type,
                           quantity_quotient_tag {};
} // namespace _detail
/// \endcond

/// \brief Computes the type of the product of two quantities.
///
/// \tparam LHS The type of the lhs quantity.
/// \tparam RHS The type of the rhs quantity.
MODULE_EXPORT template <quantity LHS, quantity RHS>
using quantity_product_t = _detail::quantity_product<LHS, RHS>;

/// \brief Computes the type of the quotient of two quantities.
///
/// \tparam LHS The type of the lhs quantity.
/// \tparam RHS The type of the rhs quantity.
MODULE_EXPORT template <quantity LHS, quantity RHS>
using quantity_quotient_t = _detail::quantity_quotient<LHS, RHS>;

/// \brief Multiplies two quantities.
///
/// Multiplies two quantities. The dimension product of the resulting quantity
/// is the product of the dimension products of the lhs and rhs quantities.
/// The name of the product is <tt>lhs.name + "+" + rhs.name</tt>.
///
/// \sa dimension_product_type
///
/// \param lhs The left hand side of the multiplication.
/// \param rhs The right hand side of the multiplication.
/// \return The product of two quantities.
MODULE_EXPORT constexpr quantity auto operator*(quantity auto lhs,
                                                quantity auto rhs) noexcept {
  return quantity_product_t<decltype(lhs), decltype(rhs)>{};
}

/// \brief Divides two quantities.
///
/// Divides two quantities. The dimension product of the resulting quantity
/// is the quotient of the dimension products of the lhs and rhs quantities.
/// The name of the quotient is <tt>lhs.name + "-" + rhs.name</tt>.
///
/// \sa dimension_product_type
///
/// \param lhs The left hand side of the quotient.
/// \param rhs The right hand side of the quotient.
/// \return The quotient of two quantities.
MODULE_EXPORT constexpr quantity auto operator/(quantity auto lhs,
                                                quantity auto rhs) noexcept {
  return quantity_quotient_t<decltype(lhs), decltype(rhs)>{};
}

/// \cond
namespace _detail {
template <auto Base, utility::template_string Kind, bool IsDerived = true>
  requires quantity<decltype(Base)>
struct derived_quantity_impl : std::remove_cvref_t<decltype(Base)> {
  constexpr static dimension_product auto dimensions = Base.dimensions;
  constexpr static auto kind = Kind;
  constexpr static bool derived = IsDerived;
};
} // namespace _detail
/// \endcond

template <auto Base, utility::template_string Derived, bool IsDerived = true>
struct derived_quantity_impl {
  using type = _detail::derived_quantity_impl<Base, Derived, IsDerived>;
};

template <auto Base, utility::template_string Derived, bool IsDerived = true>
using derived_quantity =
    derived_quantity_impl<Base, Derived, IsDerived>::type;

/// \cond
namespace _detail {
template <auto Base, utility::template_string Derived, bool IsDerived>
auto derived_base(_detail::derived_quantity_impl<Base, Derived, IsDerived>)
    -> _detail::derived_quantity_impl<Base, Derived, IsDerived>;

template <typename T>
using derived_base_t = decltype(derived_base(std::declval<T>()));

template <typename, typename = void>
struct has_derived_base : std::false_type {};

template <typename T>
struct has_derived_base<T, std::void_t<derived_base_t<T>>> : std::true_type {};

template <typename T> struct has_derived_base<const T> : has_derived_base<T> {};

template <quantity From, quantity To>
consteval auto quantity_convertible_to_impl(From, To) noexcept -> bool {
  // If dimensions size > 1 we know it's a product or a quotient
  if (std::derived_from<From, _detail::quantity_product_tag> &&
      !has_derived_base<From>::value) {
    return From::dimensions == To::dimensions;
  } else if (std::derived_from<From, _detail::quantity_quotient_tag> &&
             !has_derived_base<From>::value) {
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

/// \brief Concept modeling the convertibility between two quantities.
///
/// Concept \c quantity_convertible_to models that a quantity \c From is
/// convertible to a quantity \c To. \c From is convertible to \c To if
/// it has the same dimensions as \c To and it is derived from \c To.
/// Note that the relationship is not symmetric. It is possible for \c From to
/// be convertible to \c To but not vice versa.
///
/// \note
/// This concept operates on values, not types.
///
/// \tparam From The quantity to convert from.
/// \tparam To The quantity to convert to.
template <auto From, auto To>
concept quantity_convertible_to =
    _detail::quantity_convertible_to_impl(From, To);

template <typename T, auto Q>
concept quantity_of = quantity_convertible_to<T::quantity_kind, Q>;

template <auto> struct is_angle_like : std::false_type {};

template <auto Q>
concept angle_like = is_angle_like<Q>::value;
} // namespace maxwell

#endif