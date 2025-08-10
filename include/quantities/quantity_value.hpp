#ifndef QUANTITY_VALUE_HPP
#define QUANTITY_VALUE_HPP

#include <concepts>
#include <functional>
#include <initializer_list>
#include <utility>

#include "quantity.hpp"
#include "unit.hpp"

namespace maxwell {
namespace _detail {
template <double V> constexpr double constant = V;
}

template <auto Q, auto U, typename T = double>
  requires unit<decltype(U)> && quantity<decltype(Q)>
class quantity_value;

namespace _detail {
template <typename> struct is_quantity_value : std::false_type {};

template <auto Q, auto U, typename T>
struct is_quantity_value<quantity_value<Q, U, T>> : std::true_type {};

template <typename T>
constexpr bool is_quantity_value_v =
    is_quantity_value<std::remove_cvref_t<T>>::value;
} // namespace _detail

template <auto Q, auto U, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
class quantity_value {
  static_assert(
      quantity_convertible_to<Q, U.quantity>,
      "Attempting to instantiate quantity value with incompatible units");

public:
  using value_type = T;
  using quantity_kind_type = std::remove_cv_t<decltype(U)>;
  using units_type = std::remove_cv_t<decltype(U)>;

  static constexpr unit auto units = U;
  static constexpr quantity auto quantity_kind = Q;

  constexpr quantity_value() = default;

  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_value_v<Up>)
  constexpr explicit quantity_value(Up&& value)
      : value_(std::forward<Up>(value)) {}

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr explicit quantity_value(std::in_place_t, Args&&... args)
      : value_(std::forward<Args>(args)...) {}

  template <typename Up, typename... Args>
    requires std::constructible_from<T, std::initializer_list<Up>&, Args...>
  constexpr explicit quantity_value(std::in_place_t,
                                    std::initializer_list<Up> il,
                                    Args&&... args)
      : value_(il, std::forward<Args>(args)...) {}

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && unit<decltype(FromUnit)>
  constexpr quantity_value(
      const quantity_value<FromQuantity, FromUnit, Up>& other)
      : value_(other.get_value() *
               _detail::constant<conversion_factor(FromUnit, U)>) {
    static_assert(unit_convertible_to<FromUnit, U>,
                  "Units of other cannot be converted to units of value being "
                  "constructed");
  }

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && unit<decltype(FromUnit)>
  constexpr quantity_value(
      quantity_value<FromQuantity, FromUnit, Up>&& other) noexcept
      : value_(std::move(other).get_value() *
               _detail::constant<conversion_factor(FromUnit, U)>) {
    static_assert(unit_convertible_to<FromUnit, U>,
                  "Units of other cannot be converted to units of value being "
                  "constructed");
  }

  constexpr const T& get_value() const& noexcept { return value_; }

  constexpr T&& get_value() && noexcept { return std::move(value_); }

  constexpr const T&& get_value() const&& noexcept { return std::move(value_); }

  constexpr explicit(!unitless<U>) operator value_type() const {
    return value_;
  }

  constexpr units_type get_units() const noexcept { return units; }

  constexpr quantity_value& operator++() {
    ++value_;
    return *this;
  }

  constexpr quantity_value operator++(int) {
    const quantity_value temp(*this);
    ++(*this);
    return temp;
  }

  constexpr quantity_value& operator--() {
    --value_;
    return *this;
  }

  constexpr quantity_value operator--(int) {
    const quantity_value temp(*this);
    --(*this);
    return temp;
  }

  constexpr quantity_value& operator+=(const quantity_value& rhs) {
    value_ += rhs.value_;
    return *this;
  }

  constexpr quantity_value& operator-=(const quantity_value& rhs) {
    value_ -= rhs.value_;
    return *this;
  }

  constexpr quantity_value& operator*=(const quantity_value& rhs) {
    value_ *= rhs.value_;
    return *this;
  }

  constexpr quantity_value& operator/=(const quantity_value& rhs) {
    value_ /= rhs.value_;
    return *this;
  }

  constexpr quantity_value& operator%=(const quantity_value& rhs) {
    value_ %= rhs.value_;
    return *this;
  }

private:
  T value_{};
};
} // namespace maxwell

template <auto Q, auto U, typename T>
struct std::hash<maxwell::quantity_value<Q, U, T>> {
  std::size_t operator()(const maxwell::quantity_value<Q, U, T>& q) noexcept {
    return 0;
  }
};

#endif