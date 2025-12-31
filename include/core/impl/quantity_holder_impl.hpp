#include "core/impl/quantity_value_holder_fwd.hpp"
#ifndef QUANTITY_HOLDER_HPP
#error                                                                         \
    "Do not include quantity_holder_impl.hpp directly; include quantity_holder.hpp instead"
#endif

namespace maxwell {
template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr quantity_holder<Q, T>::quantity_holder(
    const unit auto units) noexcept(std::is_nothrow_default_constructible_v<T>)
  requires std::is_default_constructible_v<T>
    : multiplier_(units.multiplier), reference_(units.reference) {
  static_assert(quantity_convertible_to<units.quantity, Q>);
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename Up>
  requires std::constructible_from<T, Up> &&
               (!_detail::is_quantity_holder_v<Up>) &&
               (!_detail::quantity_value_like<Up> && !unit<Up>)
constexpr quantity_holder<Q, T>::quantity_holder(const unit auto units, Up&& u)
    : value_(std::forward<Up>(u)), multiplier_(units.multiplier),
      reference_(units.reference) {
  static_assert(quantity_convertible_to<Q, decltype(units)::quantity>,
                "Cannot convert from units of other to quantity of value "
                "being constructed");
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename Up>
  requires std::constructible_from<T, Up> &&
               (!_detail::is_quantity_holder_v<Up>) &&
               (!_detail::quantity_value_like<Up> && !unit<Up>)
constexpr quantity_holder<Q, T>::quantity_holder(Up&& u,
                                                 const double multiplier,
                                                 const double reference)
    : value_(std::forward<Up>(u)), multiplier_(multiplier),
      reference_(reference) {}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename... Args>
  requires std::constructible_from<T, Args...>
constexpr quantity_holder<Q, T>::quantity_holder(const unit auto units,
                                                 std::in_place_t,
                                                 Args&&... args)
    : value_(std::forward<Args>(args)...), multiplier_(units.multiplier),
      reference_(units.reference) {}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename U, typename... Args>
  requires std::constructible_from<T, std::initializer_list<U>&, Args...>
constexpr quantity_holder<Q, T>::quantity_holder(const unit auto units,
                                                 std::in_place_t,
                                                 std::initializer_list<U> il,
                                                 Args&&... args)
    : value_(il, std::forward<Args>(args)...), multiplier_(units.multiplier),
      reference_(units.reference) {}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename Rep, typename Period>
  requires std::constructible_from<T, Rep> && enable_chrono_conversions_v<Q>
constexpr quantity_holder<Q, T>::quantity_holder(
    const std::chrono::duration<Rep, Period>& d)
    : value_(d.count()), multiplier_(static_cast<double>(Period::den) /
                                     static_cast<double>(Period::num)) {}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <auto FromQuantity, auto FromUnit, typename Up>
  requires std::constructible_from<T, Up>
constexpr quantity_holder<Q, T>::quantity_holder(
    quantity_value<FromUnit, FromQuantity, Up> other)
    : value_(std::move(other).get_value_unsafe()),
      multiplier_(FromUnit.multiplier), reference_(FromUnit.reference) {
  static_assert(quantity_convertible_to<FromUnit.quantity, Q>,
                "Cannot convert from units of other to quantity of value "
                "being constructed");
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <auto FromQuantity, typename Up>
  requires std::constructible_from<T, Up>
constexpr quantity_holder<Q, T>::quantity_holder(
    quantity_holder<FromQuantity, Up> other)
    : value_(std::move(other).get_value_unsafe()),
      multiplier_(other.get_multiplier()), reference_(other.get_reference()) {
  static_assert(
      quantity_convertible_to<FromQuantity, Q>,
      "Attempting to construct quantity holder from incompatible quantity");
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <auto FromQuantity, typename Up>
  requires std::constructible_from<T, Up> && std::swappable<T>
constexpr auto
quantity_holder<Q, T>::operator=(quantity_holder<FromQuantity, Up> other)
    -> quantity_holder& {

  static_assert(
      quantity_convertible_to<FromQuantity, Q>,
      "Attempting to assign quantity holder value with incompatible quantity");

  using std::swap;
  quantity_holder temp(std::move(other));
  swap(temp.value_, value_);
  swap(temp.multiplier_, multiplier_);
  swap(temp.reference_, reference_);
  return *this;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <auto FromUnits, auto FromQuantity, typename Up>
  requires std::constructible_from<T, Up>
constexpr auto quantity_holder<Q, T>::operator=(
    quantity_value<FromUnits, FromQuantity, Up> other) -> quantity_holder& {
  static_assert(
      quantity_convertible_to<FromQuantity, Q>,
      "Attempting to assign quantity holder value with incompatible quantity");

  using std::swap;
  quantity_holder temp(std::move(other));
  swap(temp.value_, value_);
  swap(temp.multiplier_, multiplier_);
  swap(temp.reference_, reference_);
  return *this;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename Rep, typename Period>
  requires std::constructible_from<T, Rep> && enable_chrono_conversions_v<Q>
constexpr auto
quantity_holder<Q, T>::operator=(const std::chrono::duration<Rep, Period>& d)
    -> quantity_holder& {
  using std::swap;
  quantity_holder temp(d);
  swap(temp.value_, value_);
  swap(temp.multiplier_, multiplier_);
  swap(temp.reference_, reference_);
  return *this;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename Up>
  requires(!_detail::quantity_holder_like<Up>) &&
          (!_detail::quantity_value_like<Up> && !unit<Up>) &&
          std::is_assignable_v<T&, Up> && quantity_convertible_to<number, Q>
constexpr auto quantity_holder<Q, T>::operator=(Up&& other)
    -> quantity_holder& {
  value_ = std::forward<Up>(other);
  return *this;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_value_unsafe() const& noexcept
    -> const T& {
  return value_;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_value_unsafe() const&& noexcept
    -> const T&& {
  return std::move(value_);
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_value_unsafe() && noexcept -> T&& {
  return std::move(value_);
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_multiplier() const noexcept
    -> double {
  return multiplier_;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_reference() const noexcept -> double {
  return reference_;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <unit ToUnit>
constexpr auto quantity_holder<Q, T>::as(const ToUnit /*to_unit*/) const
    -> quantity_value<ToUnit{}, Q, T> {
  static_assert(quantity_convertible_to<Q, ToUnit::quantity>,
                "Cannot convert to specified units");

  const double multiplier = ToUnit::multiplier / multiplier_;
  double offset{};
  if (ToUnit::multiplier == 1.0 && multiplier_ == 1.0) {
    offset = ToUnit::reference - reference_;
  } else if (multiplier == 1.0) {
    offset = ToUnit::reference - ToUnit::multiplier * reference_;
  } else {
    offset = ToUnit::reference - reference_ / multiplier_;
  }
  return quantity_value<ToUnit{}, Q, T>(value_ * multiplier + offset);
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <unit ToUnit>
constexpr auto quantity_holder<Q, T>::in(const ToUnit /*to_unit*/) const
    -> value_type {
  static_assert(quantity_convertible_to<Q, ToUnit::quantity>,
                "Cannot convert to specified units");

  const double multiplier = ToUnit::multiplier / multiplier_;
  double offset{};
  if (ToUnit::multiplier == 1.0 && multiplier_ == 1.0) {
    offset = ToUnit::reference - reference_;
  } else if (multiplier == 1.0) {
    offset = ToUnit::reference - ToUnit::multiplier * reference_;
  } else {
    offset = ToUnit::reference - reference_ / multiplier_;
  }
  return value_ * multiplier + offset;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::in_base_units() const
    -> quantity_holder<Q, T> {
  const double multiplier = conversion_factor(multiplier_, 1.0);
  const double offset = conversion_offset(multiplier_, reference_, 1.0, 0.0);
  return quantity_holder<Q, T>(value_ * multiplier + offset, 1.0, 0.0);
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr quantity_holder<Q, T>::operator value_type() const
  requires(quantity_convertible_to<Q, number> && !Q.derived)
{
  return value_;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <unit U>
constexpr auto quantity_holder<Q, T>::contains(const U /*unit*/) const noexcept
    -> bool {
  return (multiplier_ == U::multiplier) && (reference_ == U::reference);
}
} // namespace maxwell