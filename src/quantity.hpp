#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <chrono>
#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include "core.hpp"
#include "isq_units.hpp"
#include "kind.hpp"
#include "unit.hpp"

namespace maxwell {
template <auto Unit, kind Kind, typename Magnitude = double>
  requires unit<decltype(Unit)>
class quantity_type;

namespace _detail {
template <typename> struct is_quantity : std::false_type {};

template <typename Magnitude, auto Unit, kind Kind>
struct is_quantity<quantity_type<Unit, Kind, Magnitude>> : std::true_type {};
} // namespace _detail

template <auto Unit, kind Kind, typename Magnitude>
  requires unit<decltype(Unit)>
class quantity_type {
public:
  using magnitude_type = Magnitude;
  using kind = Kind;
  using unit_type = std::remove_cvref_t<decltype(Unit)>;

  constexpr quantity_type()
    requires std::is_default_constructible_v<Magnitude>
  = default;

  template <typename U = Magnitude>
    requires std::constructible_from<Magnitude, U> &&
             (!_detail::is_quantity<std::remove_cvref_t<U>>::value)
  constexpr explicit(!std::convertible_to<U, Magnitude> || !dimensionless<Unit>)
      quantity_type(U&& value) noexcept(
          std::is_nothrow_constructible_v<Magnitude, U>)
      : value_(std::forward<U>(value)) {}

  template <typename... Args>
    requires std::constructible_from<Magnitude, Args...>
  constexpr quantity_type(std::in_place_t, Args&&... args) noexcept(
      std::is_nothrow_constructible_v<Magnitude, Args...>)
      : value_(std::forward<Args>(args)...) {}

  template <typename U, typename... Args>
    requires std::constructible_from<Magnitude, std::initializer_list<U>&,
                                     Args...>
  constexpr quantity_type(
      std::in_place_t, std::initializer_list<U> il,
      Args&&... args) noexcept(std::
                                   is_nothrow_constructible_v<
                                       Magnitude, std::initializer_list<U>&,
                                       Args...>)
      : value_(il, std::forward<Args>(args)...) {}

  template <auto FromUnit, typename FromKind, typename FromMagnitude>
  constexpr quantity_type(
      const quantity_type<FromUnit, FromKind, FromMagnitude>& other)
      : value_(other.magnitude()) {
    static_assert(unit_convertible_to<FromUnit, Unit>,
                  "Units of other cannot be converted to units of quantity "
                  "being constructed");
    static_assert(kind_convertible_to<FromKind, Kind>,
                  "Kind of other is not compatible with kind of quantity being "
                  "constructed. Use from_same_dimension to forcibly construct "
                  "from a quantity of a different kind.");
  }

  template <typename Rep, typename Period>
    requires std::constructible_from<Magnitude, Rep>
  MAXWELL_CXX_23_CONSTEXPR
  quantity_type(std::chrono::duration<Rep, Period> other)
      : value_(other.count()) {
    static_assert(
        isq::time<quantity_type>,
        "Can only construct time quantities from std::chrono::duration");
  }

  template <auto FromUnit, typename FromKind, typename FromMagnitude>
    requires std::constructible_from<Magnitude, FromMagnitude>
  constexpr quantity_type(
      quantity_type<FromUnit, FromKind, FromMagnitude>&& other)
      : value_(std::move(other).magnitude()) {
    static_assert(unit_convertible_to<FromUnit, Unit>,
                  "Units of other cannot be converted to units of quantity "
                  "being constructed");
    static_assert(kind_convertible_to<FromKind, Kind>,
                  "Kind of other is not compatible with kind of quantity being "
                  "constructed. Use from_same_dimension to forcibly construct "
                  "from a quantity of a different kind.");
  }

  template <auto FromUnit, typename K2, typename M>
    requires std::constructible_from<Magnitude, M>
  constexpr static quantity_type
  from_same_dimension(const quantity_type<FromUnit, K2, M>& other) {
    static_assert(unit_convertible_to<FromUnit, Unit>,
                  "Units of other cannot be converted to units of quantity "
                  "being constructed");
    const auto converted =
        conversion_factor(FromUnit, Unit) * other.magnitude();
    return quantity_type(std::move(converted));
  }

  template <auto U2, typename K2, typename M>
    requires std::constructible_from<Magnitude, M>
  constexpr static quantity_type
  from_same_dimension(quantity_type<U2, K2, M>&& other) {
    static_assert(unit_convertible_to<U2, Unit>,
                  "Units of other cannot be converted to units of quantity "
                  "being constructed");
    const auto converted =
        conversion_factor(U2, Unit) * std::move(other).magnitude();
    return quantity_type(converted);
  }

  template <auto U2, typename K2, typename M>
  constexpr quantity_type& operator=(quantity_type<U2, K2, M> other) {
    quantity_type temp(std::move(other));
    swap(temp, *this);
    return *this;
  }

  template <typename M>
    requires std::assignable_from<Magnitude, M>
  constexpr quantity_type& operator=(M&& other) {
    static_assert(
        dimensionless<Unit>,
        "Can only explicitly assign value of dimensionless quantities");
    value_ = std::forward<M>(other);
    return *this;
  }

  constexpr const magnitude_type& magnitude() const& noexcept { return value_; }

  constexpr magnitude_type&& magnitude() && noexcept {
    return std::move(value_);
  }

  constexpr const magnitude_type&& magnitude() const&& noexcept {
    return std::move(value_);
  }

  constexpr unit_type units() const noexcept { return Unit; }

  constexpr quantity_type& operator++() {
    ++value_;
    return *this;
  }

  constexpr quantity_type operator++(int) {
    quantity_type temp(*this);
    ++(*this);
    return temp;
  }

  constexpr quantity_type& operator--() {
    --value_;
    return *this;
  }

  constexpr quantity_type operator--(int) {
    quantity_type temp(*this);
    --(*this);
    return temp;
  }

  friend auto constexpr operator<=>(const quantity_type& lhs,
                                    const quantity_type& rhs) = default;

  friend constexpr bool operator==(const quantity_type& lhs,
                                   const quantity_type& rhs) = default;

private:
  friend void
  swap(quantity_type& lhs,
       quantity_type& rhs) noexcept(std::is_nothrow_swappable_v<Magnitude>) {
    using std::swap;
    swap(lhs.value_, rhs.value_);
  }

private:
  Magnitude value_{};
};

template <typename M1, auto U1, kind K1, typename M2, auto U2, kind K2>
auto constexpr operator*(const quantity_type<U1, K1, M1>&,
                         const quantity_type<U2, K2, M2>&) {
  constexpr unit auto product_unit = U1 * U2;
  using product_magnitude_type =
      decltype(std::declval<M1>() * std::declval<M2>());
  return quantity_type<product_unit, kind_product_t<K1, K2>,
                       product_magnitude_type>();
}

template <typename M1, auto U1, kind K1, typename M2, auto U2, kind K2>
auto constexpr operator/(const quantity_type<U1, K1, M1>&,
                         const quantity_type<U2, K2, M2>&) {
  constexpr unit auto quotient_unit = U1 / U2;
  using quotient_magnitude_type =
      decltype(std::declval<M1>() / std::declval<M2>());
  return quantity_type<quotient_unit, kind_quotient_t<K1, K2>,
                       quotient_magnitude_type>();
}

template <typename Magnitude, auto Unit, kind K>
  requires unit<decltype(Unit)>
auto constexpr dim(const quantity_type<Unit, K, Magnitude>&) noexcept {
  return Unit.dimensional_product;
}
} // namespace maxwell

#endif