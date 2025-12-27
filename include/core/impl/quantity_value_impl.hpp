#include "core/scale.hpp"
#include "core/unit.hpp"

#ifndef QUANTITY_VALUE_HPP
#error "Do not include this file directly; include quantity_value.hpp instead"
#endif

namespace maxwell {
template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <typename Up>
             requires std::constructible_from<T, Up> &&
                      (!_detail::is_quantity_value_v<Up> &&
                       !_detail::is_quantity_holder_v<Up>)
constexpr quantity_value<U, Q, T>::quantity_value(Up&& value)
    : value_(std::forward<Up>(value)) {}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <typename... Args>
             requires std::constructible_from<T, Args...>
constexpr quantity_value<U, Q, T>::quantity_value(std::in_place_t,
                                                  Args&&... args)
    : value_(std::forward<Args>(args)...) {}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <typename Up, typename... Args>
             requires std::constructible_from<T, std::initializer_list<Up>&,
                                              Args...>
constexpr quantity_value<U, Q, T>::quantity_value(std::in_place_t,
                                                  std::initializer_list<Up> il,
                                                  Args&&... args)
    : value_(il, std::forward<Args>(args)...) {}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <typename Rep, typename Period>
             requires std::constructible_from<T, Rep> && utility::ratio<Period>
MAXWELL_CONSTEXPR23 quantity_value<U, Q, T>::quantity_value(
    const std::chrono::duration<Rep, Period>& d)
    : value_(
          utility::as_constant<_detail::chrono_conversion_factor(Period{}, U)> *
          d.count()) {
  static_assert(enable_chrono_conversions_v<Q>,
                "Attempting to construct a quantity_value that does not "
                "represent time from a std::chrono::duration instance");
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> &&
           quantity<decltype(Q)>
           template <auto FromQuantity, auto FromUnit, typename Up>
             requires std::constructible_from<T, Up> &&
                      unit<decltype(FromUnit)> &&
                      ::maxwell::quantity<decltype(FromQuantity)>
constexpr quantity_value<U, Q, T>::quantity_value(
    const quantity_value<FromUnit, FromQuantity, Up>& other)
    : value_(scale_converter<FromUnit.scale, U.scale>::template convert<
             FromUnit, U>(other.get_value())) {
  static_assert(
      quantity_convertible_to<FromQuantity, Q>,
      "Attempting to construct value from incompatible quantity. Note, "
      "quantities can be incompatible even if they have the same units.");
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> &&
           quantity<decltype(Q)>
           template <auto FromQuantity, auto FromUnit, typename Up>
             requires std::constructible_from<T, Up> &&
                      unit<decltype(FromUnit)> &&
                      ::maxwell::quantity<decltype(FromQuantity)>
constexpr quantity_value<U, Q, T>::quantity_value(
    quantity_value<FromUnit, FromQuantity, Up>&& other)
    : value_(scale_converter<FromUnit.scale, U.scale>::template convert<
             FromUnit, U>(std::move(other).get_value())) {
  static_assert(
      quantity_convertible_to<FromQuantity, Q>,
      "Attempting to construct value from incompatible quantity. Note, "
      "quantities can be incompatible even if they have the same units.");
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <auto FromQuantity, typename Up>
             requires std::constructible_from<T, Up> &&
                      ::maxwell::quantity<decltype(FromQuantity)>
constexpr quantity_value<U, Q, T>::quantity_value(
    const quantity_holder<FromQuantity, T>& other)
    : value_(other.get_value() *
                 conversion_factor(other.get_multiplier(), U.multiplier) +
             conversion_offset(other.get_multiplier(), other.get_reference(),
                               U.multiplier, U.reference)) {
  static_assert(
      quantity_convertible_to<FromQuantity, Q>,
      "Attempting to construct value from incompatible quantity. Note, "
      "quantities can be incompatible even if they have the same units.");
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <auto FromQuantity, typename Up>
             requires std::constructible_from<T, Up> &&
                      ::maxwell::quantity<decltype(FromQuantity)>
constexpr quantity_value<U, Q, T>::quantity_value(
    quantity_holder<FromQuantity, T>&& other)
    : value_(std::move(other).get_value() *
                 conversion_factor(other.get_multiplier(), U.multiplier) +
             conversion_offset(other.get_multiplier(), other.get_reference(),
                               U.multiplier, U.reference)) {
  static_assert(
      quantity_convertible_to<FromQuantity, Q>,
      "Attempting to construct value from incompatible quantity. Note, "
      "quantities can be incompatible even if they have the same units.");
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> &&
           quantity<decltype(Q)>
           template <auto FromQuantity, auto FromUnit, typename Up>
             requires std::constructible_from<T, Up> && std::swappable<T>
constexpr auto quantity_value<U, Q, T>::operator=(
    quantity_value<FromUnit, FromQuantity, Up> other) -> quantity_value& {

  static_assert(quantity_convertible_to<FromQuantity, Q>,
                "Attempting to construct value from "
                "incompatible quantity. Note, "
                "quantities can be incompatible even if "
                "they have the same units.");

  using std::swap;
  quantity_value temp(std::move(other));
  swap(temp.value_, value_);
  return *this;
} // namespace maxwell

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <typename Rep, typename Period>
             requires std::constructible_from<T, Rep> && std::swappable<T> &&
                      enable_chrono_conversions_v<Q>
constexpr auto
quantity_value<U, Q, T>::operator=(const std::chrono::duration<Rep, Period>& d)
    -> quantity_value& {
  using std::swap;
  quantity_value temp(d);
  swap(temp.value_, value_);
  return *this;
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
                              template <typename Up>
             requires(!_detail::is_quantity_value_v<Up> &&
                      std::is_assignable_v<T&, Up> && unitless<U>)
constexpr auto quantity_value<U, Q, T>::operator=(Up&& other)
    -> quantity_value& {
  value_ = std::forward<U>(other);
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
constexpr auto quantity_value<U, Q, T>::get_value() const& noexcept
    -> const T& {
  return value_;
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
constexpr auto quantity_value<U, Q, T>::get_value() && noexcept -> T&& {
  return std::move(value_);
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
constexpr auto quantity_value<U, Q, T>::get_value() const&& noexcept
    -> const T&& {
  return std::move(value_);
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
constexpr quantity_value<U, Q, T>::operator value_type() const {
  return value_;
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
constexpr auto quantity_value<U, Q, T>::get_units() const noexcept
    -> units_type {
  return units;
}

template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
constexpr auto quantity_value<U, Q, T>::in_base_units() const
    -> quantity_value<U.base_units(), Q, T> {
  constexpr unit auto base_units = U.base_units();
  return quantity_value<base_units, Q, T>(*this);
}
} // namespace maxwell