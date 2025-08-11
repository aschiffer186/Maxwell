#ifndef QUANTITY_VALUE_HPP
#define QUANTITY_VALUE_HPP

#include <chrono> // duration
#include <compare>
#include <concepts>         // constructible_from
#include <functional>       // hash
#include <initializer_list> // initializer_list
#include <type_traits>      // remove_cvref_t
#include <utility>          // forward, in_place_t, move

#include "config.hpp"
#include "quantity.hpp"
#include "type_traits.hpp"
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

  // --- Constructors ---

  constexpr quantity_value() = default;

  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_value_v<Up>)
  constexpr explicit(!std::convertible_to<Up, T> || !unitless<U>)
      quantity_value(Up&& value)
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

  template <typename Rep, typename Period>
    requires enable_chrono_conversions_v<Q> && std::constructible_from<T, Rep>
  MAXWELL_CONSTEXPR23 explicit quantity_value(
      const std::chrono::duration<Rep, Period>& d) {}

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && unit<decltype(FromUnit)>
  constexpr quantity_value(
      const quantity_value<FromQuantity, FromUnit, Up>& other)
      : value_(other.get_value() *
               utility::as_constant<conversion_factor(FromUnit, U)>) {
    static_assert(unit_convertible_to<FromUnit, U>,
                  "Units of other cannot be converted to units of value being "
                  "constructed");
  }

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && unit<decltype(FromUnit)>
  constexpr quantity_value(
      quantity_value<FromQuantity, FromUnit, Up>&& other) noexcept
      : value_(std::move(other).get_value() *
               utility::as_constant<conversion_factor(FromUnit, U)>) {
    static_assert(unit_convertible_to<FromUnit, U>,
                  "Units of other cannot be converted to units of value being "
                  "constructed");
    static_assert(quantity_convertible_to<FromQuantity, Q>,
                  "Attempting to construct value from incompatible quantity");
  }

  // --- Assignment Operators ---

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && std::swappable<T>
  constexpr auto operator=(quantity_value<FromQuantity, FromUnit, Up> other)
      -> quantity_value& {
    using std::swap;

    static_assert(unit_convertible_to<FromUnit, U>,
                  "Units of other cannot be converted to units of value being "
                  "constructed");
    static_assert(quantity_convertible_to<FromQuantity, Q>,
                  "Attempting to construct value from incompatible quantity");

    quantity_value temp(std::move(other));
    swap(temp.value_, value_);
    return *this;
  }

  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && std::swappable<T> &&
                 enable_chrono_conversions_v<Q>
  constexpr auto
  operator=(const std::chrono::duration<Rep, Period>& d) -> quantity_value& {
    using std::swap;
    quantity_value temp(d);
    swap(temp.value_, value_);
    return *this;
  }

  template <typename Up = T>
    requires(!_detail::is_quantity_value_v<Up> && std::is_assignable_v<T&, Up>)
  constexpr auto operator=(Up&& other) -> quantity_value& {
    static_assert(unitless<U>);

    value_ = std::forward<U>(other);
  }

  // --- Accessor Methods ---

  constexpr auto get_value() const& noexcept -> const T& { return value_; }
  constexpr auto get_value() && noexcept -> T&& { return std::move(value_); }
  constexpr auto get_value() const&& noexcept -> const T&& {
    return std::move(value_);
  }

  constexpr explicit(!unitless<U>) operator value_type() const {
    return value_;
  }

  constexpr auto get_units() const noexcept -> units_type { return units; }

  constexpr auto in_base_units() const { return *this; }

  // --- Arithmetic Operators ---

  constexpr auto operator++() -> quantity_value& {
    ++value_;
    return *this;
  }

  constexpr auto operator++(int) -> quantity_value {
    const quantity_value temp(*this);
    ++(*this);
    return temp;
  }

  constexpr auto operator--() -> quantity_value& {
    --value_;
    return *this;
  }

  constexpr auto operator--(int) -> quantity_value& {
    const quantity_value temp(*this);
    --(*this);
    return temp;
  }

  constexpr auto operator+=(const quantity_value& rhs) -> quantity_value& {
    value_ += rhs.value_;
    return *this;
  }

  constexpr auto operator-=(const quantity_value& rhs) -> quantity_value& {
    value_ -= rhs.value_;
    return *this;
  }

  constexpr auto operator*=(const quantity_value& rhs) -> quantity_value& {
    value_ *= rhs.value_;
    return *this;
  }

  constexpr auto operator/=(const quantity_value& rhs) -> quantity_value& {
    value_ /= rhs.value_;
    return *this;
  }

  constexpr auto operator%=(const quantity_value& rhs) -> quantity_value& {
    value_ %= rhs.value_;
    return *this;
  }

  template <auto FromQuantity, auto FromUnit, typename Up>
  constexpr auto
  operator+=(const quantity_value<FromQuantity, FromUnit, Up>& rhs)
      -> quantity_value& {
    static_assert(unit_convertible_to<FromUnit, U>,
                  "Cannot add quantities with incompatible units");
    return *this += quantity_value(rhs);
  }

  template <auto FromQuantity, auto FromUnit, typename Up>
  constexpr auto
  operator-=(const quantity_value<FromQuantity, FromUnit, Up>& rhs)
      -> quantity_value& {
    static_assert(unit_convertible_to<FromUnit, U>,
                  "Cannot add quantities with incompatible units");
    return *this -= quantity_value(rhs);
  }

  // --- Comparison Functions ---

  friend auto operator<=>(const quantity_value& lhs, const quantity_value& rhs)
    requires std::three_way_comparable<T>
  = default;

  friend bool operator==(const quantity_value& lhs, const quantity_value& rhs)
    requires std::equality_comparable<T>
  = default;

private:
  template <auto Q2, auto U2, auto T2> friend class quantity_value;

  T value_{};
};
} // namespace maxwell

template <auto Q, auto U, typename T>
struct std::hash<maxwell::quantity_value<Q, U, T>> {
  auto operator()(const maxwell::quantity_value<Q, U, T>& q) noexcept
      -> std::size_t {
    return 0;
  }
};

#endif