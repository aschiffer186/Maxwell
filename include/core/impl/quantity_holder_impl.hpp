#ifndef QUANTITY_HOLDER_HPP
#error                                                                         \
    "Do not include quantity_holder_impl.hpp directly; include quantity_holder.hpp instead"
#endif

namespace maxwell {
template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr quantity_holder<Q, T>::quantity_holder(unit auto units) noexcept(
    std::is_nothrow_default_constructible_v<T>)
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
constexpr quantity_holder<Q, T>::quantity_holder(Up&& u, unit auto units)
    : value_(std::forward<Up>(u)), multiplier_(units.multiplier),
      reference_(units.reference) {
  static_assert(quantity_convertible_to<decltype(units)::quantity, Q>,
                "Cannot convert from units of other to quantity of value "
                "being constructed");
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename Up>
  requires std::constructible_from<T, Up> &&
               (!_detail::is_quantity_holder_v<Up>) &&
               (!_detail::quantity_value_like<Up> && !unit<Up>)
constexpr quantity_holder<Q, T>::quantity_holder(Up&& u, double multiplier,
                                                 double reference)
    : value_(std::forward<Up>(u)), multiplier_(multiplier),
      reference_(reference) {}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename... Args>
  requires std::constructible_from<T, Args...>
constexpr quantity_holder<Q, T>::quantity_holder(std::in_place_t,
                                                 Args&&... args,
                                                 unit auto units)
    : value_(std::forward<Args>(args)...), multiplier_(units.multiplier),
      reference_(units.reference) {}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename U, typename... Args>
  requires std::constructible_from<T, std::initializer_list<U>, Args...>
constexpr quantity_holder<Q, T>::quantity_holder(std::in_place_t,
                                                 std::initializer_list<U> il,
                                                 Args&&... args,
                                                 unit auto units)
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
    quantity_value<FromQuantity, FromUnit, Up> other)
    : value_(std::move(other).get_value()), multiplier_(FromUnit.multiplier),
      reference_(FromUnit.reference) {
  static_assert(quantity_convertible_to<FromUnit.quantity, Q>,
                "Cannot convert from units of other to quantity of value "
                "being constructed");
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_value() const& noexcept -> const T& {
  return value_;
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_value() const&& noexcept
    -> const T&& {
  return std::move(value_);
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::get_value() && noexcept -> T&& {
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
template <auto ToUnit>
constexpr auto quantity_holder<Q, T>::as() const
    -> quantity_value<Q, ToUnit, T> {
  static_assert(quantity_convertible_to<Q, ToUnit.quantity>,
                "Cannot convert to specified unit");

  return quantity_value<Q, ToUnit, T>(value_ * multiplier_ /
                                      static_cast<double>(ToUnit.multiplier));
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr auto quantity_holder<Q, T>::in_base_units() const
    -> quantity_holder<Q, T> {
  return quantity_holder<Q, T>(value_ * multiplier_);
}

template <auto Q, typename T>
  requires quantity<decltype(Q)>
constexpr quantity_holder<Q, T>::operator value_type() const {
  return value_;
}
} // namespace maxwell