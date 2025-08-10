#ifndef QUANTITY_HOLDER_HPP
#define QUANTITY_HOLDER_HPP

#include "quantity.hpp"
#include "quantity_value.hpp"
#include <type_traits>

namespace maxwell {
template <auto Q, typename T = double>
  requires quantity<decltype(Q)>
class quantity_holder;

namespace _detail {
template <typename> struct is_quantity_holder : std::false_type {};

template <auto Q, typename T>
struct is_quantity_holder<quantity_holder<Q, T>> : std::false_type {};

template <typename T>
constexpr bool is_quantity_holder_v = is_quantity_holder<T>::value;
} // namespace _detail

template <auto Q, typename T>
  requires quantity<decltype(Q)>
class quantity_holder {
public:
  using value_type = T;
  using quantity_kind_type = decltype(Q);

  static constexpr quantity auto quantity_kind = Q;

  constexpr quantity_holder() = default;

  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_holder_v<Up>)
  constexpr explicit quantity_holder(Up&& u) : value_(std::forward<Up>(u)) {}

  template <auto FromQuantity, auto FromUnit, typename Up>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(
      const quantity_value<FromQuantity, FromUnit, Up>& other)
      : value_(other.get_value()),
        multiplier_(static_cast<double>(FromUnit.multiplier)) {
    static_assert(quantity_convertible_to<FromUnit.quantity, Q>,
                  "Cannot convert from units of other to quantity of value "
                  "being constructed");
  }

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(quantity_value<FromQuantity, FromUnit, Up>&& other)
      : value_(std::move(other).get_value()),
        multiplier_(static_cast<double>(FromUnit.multplier)) {
    static_assert(quantity_convertible_to<FromUnit.quantity, Q>,
                  "Cannot convert from units of other to quantity of value "
                  "being constructed");
  }

  template <auto ToQuantity, auto ToUnit>
  constexpr quantity_value<ToQuantity, ToUnit, T> as() const {
    static_assert(quantity_convertible_to<Q, ToUnit.quantity>,
                  "Cannot convert to specified unit");

    return quantity_value<ToQuantity, ToUnit, T>(
        value_ * multiplier_ / static_cast<double>(ToUnit.multiplier));
  }

private:
  T value_{};
  double multiplier_{1.0};
};
} // namespace maxwell

#endif