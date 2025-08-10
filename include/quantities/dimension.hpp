#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include "compile_time_math.hpp"
#include "template_string.hpp"
#include "type_traits.hpp"

namespace maxwell {
template <utility::template_string Name, utility::rational auto Power>
struct dimension_type;

namespace _detail {
template <typename> struct is_dimension_type : std::false_type {};

template <utility::rational auto Power, utility::template_string Name>
struct is_dimension_type<dimension_type<Name, Power>> : std::true_type {};
} // namespace _detail

template <utility::template_string Name, utility::rational auto Power>
struct dimension_type {
  static constexpr utility::rational auto power = Power;
  static constexpr auto name = Name;
};

template <typename T>
concept dimension = _detail::is_dimension_type<std::remove_cvref_t<T>>::value;

constexpr bool operator==(dimension auto lhs, dimension auto rhs) noexcept {
  return lhs.name == rhs.name && lhs.power == rhs.power;
}

// 1/m = m^-1

template <dimension D> struct dimension_inverse {
  using inverse_power =
      utility::rational_type<-D::power.numerator, D::power.denominator,
                             D::power.exponent>;
  using type = dimension_type<D::name, inverse_power{}>;
};

template <dimension D> using dimension_inverse_t = dimension_inverse<D>::type;

template <utility::template_string Name>
using base_dimension_type = dimension_type<Name, utility::one>;

template <auto D>
using null_dimension_type = dimension_type<D.name, utility::zero>;

template <dimension... Dimensions> struct dimension_product_type;

namespace _detail {
template <typename> struct is_dimension_product_type : std::false_type {};

template <dimension... Dimensions>
struct is_dimension_product_type<dimension_product_type<Dimensions...>>
    : std::true_type {};
} // namespace _detail

template <typename T>
concept dimension_product =
    _detail::is_dimension_product_type<std::remove_cvref_t<T>>::value;

template <dimension... Dimensions> struct dimension_product_type {
  using tuple_type = std::tuple<Dimensions...>;
  consteval static auto as_tuple() { return tuple_type{}; }
};

template <dimension LHS, dimension... LHSRest, dimension RHS,
          dimension... RHSRest>
constexpr bool
operator==(dimension_product_type<LHS, LHSRest...> /*lhs*/,
           dimension_product_type<RHS, RHSRest...> /*rhs*/) noexcept {
  if constexpr (sizeof...(LHSRest) == sizeof...(RHSRest)) {
    return utility::similar<dimension_product_type<LHS, LHSRest...>,
                            dimension_product_type<RHS, RHSRest...>>;
  } else if constexpr (LHS::power == utility::zero) {
    return dimension_product_type<LHSRest...>{} ==
           dimension_product_type<RHS, RHSRest...>{};
  } else if constexpr (RHS::power == utility::zero) {
    return dimension_product_type<LHS, LHSRest...>{} ==
           dimension_product_type<RHSRest...>{};
  } else if constexpr (utility::similar<LHS, RHS>) {
    return dimension_product_type<LHSRest...>{} ==
           dimension_product_type<RHSRest...>{};
  } else {
    return false;
  }
}

template <auto From, auto To>
concept dimension_convertible_to =
    dimension_product<decltype(From)> && dimension_product<decltype(To)> &&
    From == To;

template <dimension LHS, dimension RHS>
constexpr dimension_product auto operator*(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  if constexpr (LHS::name < RHS::name) {
    return dimension_product_type<LHS, RHS>{};
  } else if constexpr (RHS::name < LHS::name) {
    return dimension_product_type<RHS, LHS>{};
  } else {
    return dimension_product_type<
        dimension_type<LHS::power + RHS::power, LHS::name>>{};
  }
}

template <dimension LHS>
constexpr dimension_product auto
operator*(LHS, dimension_product auto rhs) noexcept {
  return dimension_product_type<LHS>{} * rhs;
}

template <dimension RHS>
constexpr dimension_product auto operator*(dimension_product auto lhs,
                                           RHS) noexcept {
  return lhs * dimension_product_type<RHS>{};
}

template <dimension LHS, dimension... LHSRest, dimension RHS,
          dimension... RHSRest>
consteval dimension_product auto
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
    using first = dimension_type<LHS::name, LHS::power + RHS::power>;
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
  }
}

template <dimension LHS, dimension RHS>
constexpr dimension_product auto operator/(LHS /*lhs*/, RHS /*rhs*/) noexcept {
  if constexpr (LHS::name < RHS::name) {
    return dimension_product_type<LHS, dimension_inverse_t<RHS>>{};
  } else if constexpr (RHS::name < LHS::name) {
    return dimension_product_type<dimension_inverse_t<RHS>, LHS>{};
  } else {
    return dimension_product_type<
        dimension_type<LHS::power - RHS::power, LHS::name>>{};
  }
}

template <dimension LHS>
constexpr dimension_product auto
operator/(LHS /*rhs*/, dimension_product auto rhs) noexcept {
  return dimension_product_type<LHS>{} / rhs;
}

template <dimension RHS>
constexpr dimension_product auto operator/(dimension_product auto lhs,
                                           RHS /*rhs*/) noexcept {
  return lhs / dimension_product_type<RHS>{};
}

template <dimension LHS, dimension... LHSRest, dimension RHS,
          dimension... RHSRest>
consteval dimension_product auto
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
  }
}
} // namespace maxwell

#endif