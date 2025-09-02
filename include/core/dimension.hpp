/// \file dimension.hpp
/// \brief Definition of \c dimension_product_type and related class, functions,
/// and concets.

#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include <tuple>       // tuple
#include <type_traits> // false_type, true_type, remove_cvref_t

#include "utility/compile_time_math.hpp"
#include "utility/config.hpp"
#include "utility/template_string.hpp"
#include "utility/type_traits.hpp"

/// \namespace maxwell
/// \brief Definition of public API of Maxwell.
namespace maxwell {
/// \brief Dimension of a base quantity.
/// Class template \c dimension_type represents the dimension of a base quantity
/// raised to some power.
///
/// \tparam Power The power to which the dimension is raised.
/// \tparam Name A symbol representing the dimension
MODULE_EXPORT template <utility::template_string Name,
                        utility::rational auto Power>
struct dimension_type;

/// \cond
namespace _detail {
template <typename> struct is_dimension_type : std::false_type {};

template <utility::rational auto Power, utility::template_string Name>
struct is_dimension_type<dimension_type<Name, Power>> : std::true_type {};
} // namespace _detail
/// \endcond

MODULE_EXPORT template <utility::template_string Name,
                        utility::rational auto Power>
struct dimension_type {
  static constexpr utility::rational auto power = Power;
  static constexpr auto name = Name;
};

/// \brief Concept modeling if a type is an instantiation of \c dimension_type.
/// \tparam T The type to check.
MODULE_EXPORT template <typename T>
concept dimension = _detail::is_dimension_type<std::remove_cvref_t<T>>::value;

/// \brief Equality operator
///
/// Compares two instances of \c dimension_type for equality.
/// Two dimensions are equal if they are raised to the same power and
/// have the same name.
///
/// \tparam Power1 The power of the left-hand side dimension
/// \tparam Name1 The power of the left-hand side dimension
/// \tparam Power2 The power of the right-hand side dimension
/// \tparam Name2 The name of the right-hand side dimension.
/// \return \c true if the dimensions are equal.
MODULE_EXPORT constexpr auto operator==(dimension auto lhs,
                                        dimension auto rhs) noexcept -> bool {
  return lhs.name == rhs.name && lhs.power == rhs.power;
}

/// \brief Computes the inverse of a dimension
///
// Computes the inverse of a dimension i.e. 1 / dimension.
MODULE_EXPORT template <dimension D> struct dimension_inverse {
  using inverse_power =
      utility::rational_type<-D::power.numerator, D::power.denominator,
                             D::power.exponent>;
  using type = dimension_type<D::name, inverse_power{}>;
};

/// \brief Helper alias for dimension inverse.
/// \tparam D The dimension to compute the inverse of.
MODULE_EXPORT template <dimension D>
using dimension_inverse_t = dimension_inverse<D>::type;

/// \brief Helper alias to create a base dimension.
/// \tparam Name The name of the base dimenson.
MODULE_EXPORT template <utility::template_string Name>
using base_dimension_type = dimension_type<Name, utility::one>;

/// \brief Helper alias to create a null dimension.
/// \tparam D The dimension to nullify.
MODULE_EXPORT template <auto D>
using null_dimension_type = dimension_type<D.name, utility::zero>;

/// \brief Product of dimensions
///
/// Class template \c dimension_product_type represents a product of dimesions
/// where each factor in the product is the dimension of a base quantity raised
/// to some power.
/// For example, given the base quantities A, B, C, ... from a
/// set and each quantity has power a, b, c, ... respectively, the dimension
/// product is given by <br> \f$A^{a} \cdot B^{b} \cdot C^{c} \cdot \ldots\f$
///
/// \pre \c Dimensions... is sorted in increasing alphabetical order by the name
/// of each dimension.
///
/// \tparam The dimensions that are multiplied together to form the product.
///
MODULE_EXPORT template <dimension... Dimensions> struct dimension_product_type;

/// \cond
namespace _detail {
template <typename> struct is_dimension_product_type : std::false_type {};

template <dimension... Dimensions>
struct is_dimension_product_type<dimension_product_type<Dimensions...>>
    : std::true_type {};
} // namespace _detail
/// \endcond

/// \brief Concept modeling if a type is an instantiation of \c
/// dimension_product_type
MODULE_EXPORT template <typename T>
concept dimension_product =
    _detail::is_dimension_product_type<std::remove_cvref_t<T>>::value;

MODULE_EXPORT template <dimension... Dimensions> struct dimension_product_type {
  /// Tuple representation of the dimension product
  using tuple_type = std::tuple<Dimensions...>;

  /// \brief Returns a tuple containing the dimensions of the dimension product.
  ///
  /// \return A \c std::tuple containing the dimensions of the dimension
  /// product.
  consteval static auto as_tuple() -> tuple_type { return tuple_type{}; }

  /// \brief Returns the sum of the dimension exponents.
  ///
  /// Computes the sum of the exponents of all dimensions in the dimension
  /// product.
  /// \return The sum of the dimension exponents.
  constexpr static utility::rational auto dimension_exponent_sum() {
    return (Dimensions::power + ... + utility::zero);
  }
};

MODULE_EXPORT constexpr dimension_product_type<
    dimension_type<"[]", utility::one>>
    dimension_one;

/// \brief Equality operator
///
/// Compares two dimensional products for equality. Two dimensional products are
/// equal if all dimensions not raised to power 0 are equal.
///
/// \tparam LHS The dimensions of the left-hand side dimensional product.
/// \tparam RHS The dimensions of the right-hand side dimensional product.
/// \param LHS The left hand side of the equality comparison.
/// \param RHS The right hand side of the equality comparison.
/// \return \c true if the dimensional products are equal
MODULE_EXPORT template <dimension... LHS, dimension... RHS>
constexpr auto
operator==(dimension_product_type<LHS...> /*lhs*/,
           dimension_product_type<RHS...> /*rhs*/) noexcept -> bool {
  return utility::similar<dimension_product_type<LHS...>,
                          dimension_product_type<RHS...>>;
}

/// \brief Concept modeling a dimension products that can be converted to each
/// other.
///
/// Two types \c From and \c To model \c dimension_convertible_to if \c From can
/// be converted to \c To.
/// This concept is only modeled when the dimensions are equal.
///
/// \tparam From The dimension being converted from.
/// \tparam To The target dimension.
MODULE_EXPORT template <auto From, auto To>
concept dimension_convertible_to =
    dimension_product<decltype(From)> && dimension_product<decltype(To)> &&
    (From == To);

/// \brief Multiplies two dimensions.
///
/// Computes the product of dimensions.
/// Given dimensions \f(A^{a}\f) and \f(B^{b}\f), their product is \f$A^{a}
/// \cdot B^{b} \f$.
/// Given dimension \f$A^{a_1}\f$ and \f$A^{a_2}\f$, their
/// product is \f$A^{a_1 + a_2}\f$.
///
/// \tparam LHS The type of the left-hand side of the multiplication.
/// \tparam RHS The type of the right-hand side of the multiplication.
/// \param lhs The left-hand side of the multiplication.
/// \param rhs The right-hand side of the multiplication.
/// \return The product of the two dimensions as a \c dimension_product_type.
MODULE_EXPORT template <dimension LHS, dimension RHS>
constexpr dimension_product auto operator*(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  if constexpr (LHS::name < RHS::name) {
    return dimension_product_type<LHS, RHS>{};
  } else if constexpr (RHS::name < LHS::name) {
    return dimension_product_type<RHS, LHS>{};
  } else {
    if constexpr (LHS::power + RHS::power == utility::zero) {
      return dimension_product_type<>{};
    } else {
      return dimension_product_type<
          dimension_type<LHS::name, LHS::power + RHS::power>>{};
    }
  }
}

/// \brief Multiplies a dimension and dimension product
///
/// Computes the product of a dimension and a dimension product.
/// Given a dimension \f$A^a\f$ and dimension product \f$B^b\cdot C^c \cdot
/// \ldots\f$, their product is given \f$A^a \cdot B^b \cdot C^c \cdot
/// \ldots\f$.
///
/// \tparam LHS The type of the left-hand side of the multiplication.
/// \tparam RHS The dimenions of the dimensional product of the right-hand side
/// of the multiplication.
/// \param lhs The left-hand side of the multiplication.
/// \param rhs The right-hand side of the multiplication.
/// \return The product of a dimension and a dimensional product.
MODULE_EXPORT template <dimension LHS>
constexpr dimension_product auto
operator*(LHS, dimension_product auto rhs) noexcept {
  return dimension_product_type<LHS>{} * rhs;
}

/// \brief Multiplies a dimension and dimension product
///
/// Computes the product of a dimension and a dimension product.
/// Given a dimension \f$A^a\f$ and dimension product \f$B^b\cdot C^c \cdot
/// \ldots\f$, their product is given \f$A^a \cdot B^b \cdot C^c \cdot
/// \ldots\f$.
///
/// \tparam LHS The dimenions of the dimensional product of the left-hand side
/// of the multiplication.
/// \tparam RHS The type of the right-hand side of the multiplication.
/// \param lhs The left-hand side of the multiplication.
/// \param rhs The right-hand side of the multiplication.
/// \return The product of a dimension and a dimensional product.
MODULE_EXPORT template <dimension RHS>
constexpr dimension_product auto operator*(dimension_product auto lhs,
                                           RHS) noexcept {
  return lhs * dimension_product_type<RHS>{};
}

/// \brief Multiplies two dimension products.
///
/// Computes the product of two dimension products.
/// Given dimension products \f$A^{a_1} \cdot D^d \f$ and \f$ A^{a_2} \cdot B^b
/// \cdot C^c \f$, their product is given by \f$ A^{a_1 + a_2} \cdot B^b \cdot
/// C^c \cdot D^d \f$.
//
/// \tparam LHS The dimenions of the dimensional product of
/// the left-hand side of the multiplication.
/// \tparam RHS The dimenions of the
/// dimensional product of the right-hand side of the multiplication.
/// \param lhs The left-hand side of the multiplication.
/// \param rhs The right-hand side of the multiplication.
/// \return The product of two dimensional products.
MODULE_EXPORT template <dimension LHS, dimension... LHSRest, dimension RHS,
                        dimension... RHSRest>
constexpr dimension_product auto
operator*(dimension_product_type<LHS, LHSRest...>,
          dimension_product_type<RHS, RHSRest...>) noexcept {
  if constexpr (sizeof...(LHSRest) == 0 && sizeof...(RHSRest) == 0) {
    return LHS{} * RHS{};
  } else if constexpr (LHS::name < RHS::name) {
    if constexpr (sizeof...(LHSRest) == 0) {
      return dimension_product_type<LHS, RHS, RHSRest...>{};
    } else if constexpr (sizeof...(RHSRest) == 0) {
      return dimension_product_type<LHS>{} *
             (dimension_product_type<LHSRest...>{} *
              dimension_product_type<RHS>{});
    } else {
      return dimension_product_type<LHS>{} *
             (dimension_product_type<LHSRest...>{} *
              dimension_product_type<RHS, RHSRest...>{});
    }
  } else if constexpr (RHS::name < LHS::name) {
    if constexpr (sizeof...(LHSRest) == 0) {
      return dimension_product_type<RHS>{} *
             (dimension_product_type<LHS>{} *
              dimension_product_type<RHSRest...>{});
    } else if constexpr (sizeof...(RHSRest) == 0) {
      return dimension_product_type<RHS, LHS, LHSRest...>{};
    } else {
      return dimension_product_type<RHS>{} *
             (dimension_product_type<LHS, LHSRest...>{} *
              dimension_product_type<RHSRest...>{});
    }
  } else {
    if constexpr (LHS::power + RHS::power != utility::zero) {
      constexpr auto new_power = LHS::power + RHS::power;
      using first = dimension_type<LHS::name, new_power>;
      if constexpr (sizeof...(LHSRest) == 0) {
        return dimension_product_type<first>{} *
               dimension_product_type<RHSRest...>{};
      } else if constexpr (sizeof...(RHSRest) == 0) {
        return dimension_product_type<first>{} *
               dimension_product_type<LHSRest...>{};
      } else {
        return dimension_product_type<first>{} *
               (dimension_product_type<LHSRest...>{} *
                dimension_product_type<RHSRest...>{});
      }
    } else {
      if constexpr (sizeof...(LHSRest) == 0) {
        return dimension_product_type<RHSRest...>{};
      } else if constexpr (sizeof...(RHSRest) == 0) {
        return dimension_product_type<LHSRest...>{};
      } else {
        return dimension_product_type<LHSRest...>{} *
               dimension_product_type<RHSRest...>{};
      }
    }
  }
}

/// \cond
MODULE_EXPORT constexpr dimension_product auto
operator*(dimension_product_type<>, dimension_product_type<>) {
  return dimension_product_type<>{};
}

MODULE_EXPORT template <dimension D, dimension... Ds>
constexpr dimension_product auto operator*(dimension_product_type<D, Ds...>,
                                           dimension_product_type<>) {
  return dimension_product_type<D, Ds...>{};
}

template <dimension D, dimension... Ds>
constexpr dimension_product auto operator*(dimension_product_type<>,
                                           dimension_product_type<D, Ds...>) {
  return dimension_product_type<D, Ds...>{};
}
/// \endcond

/// \brief Divides two dimensions.
///
/// Computes the qutient of two dimensions.
/// Given dimensions \f(A^{a}\f) and \f(B^{b}\f), their quotient is \f$A^{a}
/// \cdot B^{-b} \f$.
/// Given dimension \f$A^{a_1}\f$ and \f$A^{a_2}\f$, their quotient is \f$A^{a_1
/// - a_2}\f$.
///
/// \tparam LHS The type of the left-hand side of the division.
/// \tparam RHS The type of the right-hand side of the division.
/// \param lhs The left-hand side of the division.
/// \param rhs The right-hand side of the division.
/// \return The quotient of the two dimensions as a \c dimension_product_type.
MODULE_EXPORT template <dimension LHS, dimension RHS>
constexpr dimension_product auto operator/(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  if constexpr (LHS::name < RHS::name) {
    return dimension_product_type<LHS, dimension_inverse_t<RHS>>{};
  } else if constexpr (RHS::name < LHS::name) {
    return dimension_product_type<dimension_inverse_t<RHS>, LHS>{};
  } else {
    if constexpr (LHS::power - RHS::power == utility::zero) {
      return dimension_product_type<>{};
    } else {
      return dimension_product_type<
          dimension_type<LHS::name, LHS::power - RHS::power>>{};
    }
  }
}

/// \brief Divides a dimension and dimension product
///
/// Computes the product of a dimension and a dimension product. Given a
/// dimension \f$A^a\f$ and dimension product \f$B^b\cdot C^c \cdot \ldots\f$,
/// their quotient is given \f$A^a \cdot B^{-b} \cdot C^{-c} \cdot \ldots\f$.
///
/// \tparam LHS The type of the left-hand side of the division.
/// \tparam RHS The dimenions of the dimensional product of the right-hand side
/// of the division.
/// \param lhs The left-hand side of the division.
/// \param rhs The right-hand side of the division.
/// \return The quotient of a dimension and a dimensional product.
MODULE_EXPORT template <dimension LHS>
constexpr dimension_product auto
operator/(LHS /*rhs*/, dimension_product auto rhs) noexcept {
  return dimension_product_type<LHS>{} / rhs;
}

/// \brief Divides a dimension and dimension product
///
/// Computes the product of a dimension and a dimension product. Given a
/// dimension \f$A^a\f$ and dimension product \f$B^b\cdot C^c \cdot \ldots\f$,
/// their quotient is given \f$A^a \cdot B^{-b} \cdot C^{-c} \cdot \ldots\f$.
///
/// \tparam LHS The dimenions of the dimensional product of the left-hand side
/// of the division.
/// \tparam RHS The type of the right-hand side of the division.
/// \param lhs The left-hand side of the division.
/// \param rhs The right-hand side of the division.
/// \return The quotient of a dimension and a dimensional product.
MODULE_EXPORT template <dimension RHS>
constexpr dimension_product auto operator/(dimension_product auto lhs,
                                           RHS /*rhs*/) noexcept {
  return lhs / dimension_product_type<RHS>{};
}

/// \brief Divides two dimension products.
///
/// Computes the quotient of two dimension products. Given dimension
/// products \f$A^{a_1} \cdot D^d \f$ and \f$ A^{a_2} \cdot B^b \cdot C^c \f$,
/// their quotient is given by \f$ A^{a_1 - a_2} \cdot B^{-b} \cdot C^{-c} \cdot
/// D^d \f$.
///
/// \tparam LHS The dimenions of the dimensional product of the
/// left-hand side of the division.
/// \tparam RHS The dimenions of the dimensional
/// product of the right-hand side of the division.
/// \param lhs The left-hand side of the division.
/// \param rhs The right-hand side of the division.
/// \return The quotient of two dimensional products.
MODULE_EXPORT template <dimension LHS, dimension... LHSRest, dimension RHS,
                        dimension... RHSRest>
constexpr dimension_product auto
operator/(dimension_product_type<LHS, LHSRest...>,
          dimension_product_type<RHS, RHSRest...>) noexcept {
  if constexpr (sizeof...(LHSRest) == 0 && sizeof...(RHSRest) == 0) {
    return LHS{} / RHS{};
  } else if constexpr (LHS::name < RHS::name) {
    if constexpr (sizeof...(LHSRest) == 0) {
      return dimension_product_type<LHS, dimension_inverse_t<RHS>,
                                    dimension_inverse_t<RHSRest>...>{};
    } else if constexpr (sizeof...(RHSRest) == 0) {
      return dimension_product_type<LHS>{} *
             (dimension_product_type<LHSRest...>{} *
              dimension_product_type<dimension_inverse_t<RHS>>{});
    } else {
      return dimension_product_type<LHS>{} *
             (dimension_product_type<LHSRest...>{} *
              dimension_product_type<dimension_inverse_t<RHS>,
                                     dimension_inverse_t<RHSRest>...>{});
    }
  } else if constexpr (RHS::name < LHS::name) {
    if constexpr (sizeof...(LHSRest) == 0) {
      return dimension_product_type<dimension_inverse_t<RHS>>{} *
             (dimension_product_type<LHS>{} *
              dimension_product_type<dimension_inverse_t<RHSRest>...>{});
    } else if constexpr (sizeof...(RHSRest) == 0) {
      return dimension_product_type<dimension_inverse_t<RHS>, LHS,
                                    LHSRest...>{};
      ;
    } else {
      return dimension_product_type<dimension_inverse_t<RHS>>{} *
             (dimension_product_type<LHS, LHSRest...>{} *
              dimension_product_type<dimension_inverse_t<RHSRest>...>{});
    }
  } else {
    if constexpr (LHS::power - RHS::power != utility::zero) {
      using first = dimension_type<LHS::name, LHS::power - RHS::power>;
      if constexpr (sizeof...(LHSRest) == 0) {
        return dimension_product_type<first>{} *
               dimension_product_type<dimension_inverse_t<RHSRest>...>{};
      } else if constexpr (sizeof...(RHSRest) == 0) {
        return dimension_product_type<first>{} *
               dimension_product_type<LHSRest...>{};
      } else {
        return dimension_product_type<first>{} *
               (dimension_product_type<LHSRest...>{} *
                dimension_product_type<dimension_inverse_t<RHSRest>...>{});
      }
    } else {
      if constexpr (sizeof...(LHSRest) == 0) {
        return dimension_product_type<dimension_inverse_t<RHSRest>...>{};
      } else if constexpr (sizeof...(RHSRest) == 0) {
        return dimension_product_type<LHSRest...>{};
      } else {
        return dimension_product_type<LHSRest...>{} *
               dimension_product_type<dimension_inverse_t<RHSRest>...>{};
      }
    }
  }
}

/// \cond
MODULE_EXPORT constexpr dimension_product auto
operator/(dimension_product_type<>, dimension_product_type<>) {
  return dimension_product_type<>{};
}

MODULE_EXPORT template <dimension D, dimension... Ds>
constexpr dimension_product auto operator/(dimension_product_type<D, Ds...>,
                                           dimension_product_type<>) {
  return dimension_product_type<dimension_inverse_t<D>,
                                dimension_inverse_t<Ds>...>{};
}
/// \endcond

MODULE_EXPORT template <dimension D, dimension... Ds>
constexpr dimension_product auto operator/(dimension_product_type<>,
                                           dimension_product_type<D, Ds...>) {
  return dimension_product_type<dimension_inverse_t<D>,
                                dimension_inverse_t<Ds>...>{};
}
/// \endcond
} // namespace maxwell

#endif