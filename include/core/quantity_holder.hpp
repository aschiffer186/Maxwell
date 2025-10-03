/// \file quantity_holder.hpp
/// \brief Definition of class template \c quantity_holder.

#ifndef QUANTITY_HOLDER_HPP
#define QUANTITY_HOLDER_HPP

#include <chrono>           // duration
#include <functional>       // hash
#include <initializer_list> // initializer_list
#include <stdexcept>        // runtime_error
#include <string>           // string
#include <type_traits>      // false_type, remove_cvref_t, true_type

#include "quantity.hpp"
#include "quantity_value.hpp"

namespace maxwell {
MODULE_EXPORT template <auto Q, typename T = double>
  requires quantity<decltype(Q)>
class quantity_holder;

class incompatible_quantity_holder : public std::runtime_error {
public:
  explicit incompatible_quantity_holder(const std::string& message)
      : std::runtime_error(message) {}
};

/// \cond
namespace _detail {
template <typename> struct is_quantity_holder : std::false_type {};

template <auto Q, typename T>
struct is_quantity_holder<quantity_holder<Q, T>> : std::false_type {};

template <typename T>
constexpr bool is_quantity_holder_v =
    is_quantity_holder<std::remove_cvref_t<T>>::value;

template <typename Derived> class quantity_holder_arithmetic_operators {
  MODULE_EXPORT friend constexpr auto operator++(Derived& d) -> Derived& {
    ++d.value_;
    return d;
  }

  MODULE_EXPORT friend constexpr auto operator++(Derived& d, int) -> Derived {
    auto temp{d};
    ++d;
    return temp;
  }

  MODULE_EXPORT friend constexpr auto operator--(Derived& d) -> Derived& {
    --d.value_;
    return d;
  }

  MODULE_EXPORT friend constexpr auto operator--(Derived& d, int) -> Derived {
    auto temp{d};
    --d;
    return temp;
  }

  MODULE_EXPORT friend constexpr auto operator-(const Derived& d) -> Derived {
    return Derived{-d.value_};
  }

  MODULE_EXPORT template <auto Q2, typename T2>
  friend constexpr auto operator+=(Derived& lhs,
                                   const quantity_holder<Q2, T2>& rhs)
      -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot add quantities of different kinds");
    if (lhs.reference_ != rhs.get_reference()) {
      throw incompatible_quantity_holder(
          "Cannot add quantities whose units have different reference "
          "points.");
    }
    lhs.value_ += Derived(rhs).value_;
    return lhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator+=(Derived& lhs,
                                   const quantity_value<U2, Q2, T2>& rhs)
      -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot add quantities of different kinds");
    if (lhs.reference_ != U2.get_reference()) {
      throw incompatible_quantity_holder(
          "Cannot add quantities whose units have different reference "
          "points.");
    }
    lhs.value_ += Derived(rhs).value_;
    return lhs;
  }

  MODULE_EXPORT template <typename T2>
    requires(!quantity_value_like<T2> && !is_quantity_holder<T2>::value &&
             quantity_convertible_to<Derived::quantity, number>)
  friend constexpr auto operator+=(Derived& lhs, T2&& rhs) -> Derived& {
    lhs.value_ += std::forward<T2>(rhs);
    return lhs;
  }

  MODULE_EXPORT template <auto Q2, typename T2>
  friend constexpr auto operator-=(Derived& lhs,
                                   const quantity_holder<Q2, T2>& rhs)
      -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot subtract quantities of different kinds");
    if (lhs.reference_ != rhs.get_reference()) {
      throw incompatible_quantity_holder(
          "Cannot subtract quantities whose units have different reference "
          "points.");
    }
    lhs.value_ -= Derived(rhs).value_;
    return lhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator-=(Derived& lhs,
                                   const quantity_value<U2, Q2, T2>& rhs)
      -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot subtract quantities of different kinds");
    if (lhs.reference_ != U2.get_reference()) {
      throw incompatible_quantity_holder(
          "Cannot subtract quantities whose units have different reference "
          "points.");
    }
    lhs.value_ -= Derived(rhs).value_;
    return lhs;
  }

  MODULE_EXPORT template <typename T2>
    requires(!quantity_value_like<T2> && !is_quantity_holder<T2>::value &&
             quantity_convertible_to<Derived::quantity, number>)
  friend constexpr auto operator-=(Derived& lhs, T2&& rhs) -> Derived& {
    lhs.value_ -= std::forward<T2>(rhs);
    return lhs;
  }

  MODULE_EXPORT template <auto Q2, typename T2>
  friend constexpr auto operator+(Derived lhs,
                                  const quantity_holder<Q2, T2>& rhs) {
    return lhs += rhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator+(Derived lhs,
                                  const quantity_value<U2, Q2, T2>& rhs) {
    return lhs += rhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator+(const quantity_value<U2, Q2, T2>& lhs,
                                  const Derived& rhs) {
    return rhs += lhs;
  }

  MODULE_EXPORT template <auto Q2, typename T2>
  friend constexpr auto operator-(Derived lhs,
                                  const quantity_holder<Q2, T2>& rhs) {
    return lhs -= rhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator-(Derived lhs,
                                  const quantity_value<U2, Q2, T2>& rhs) {
    return lhs -= rhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator-(const quantity_value<U2, Q2, T2>& lhs,
                                  const Derived& rhs) {
    return Derived(lhs) -= rhs;
  }

  MODULE_EXPORT template <auto Q2, typename T2>
  friend constexpr auto operator*(const Derived& lhs,
                                  const quantity_holder<Q2, T2>& rhs) {

    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind * Q2, result_type>(
        lhs.get_value() * rhs.get_value(),
        lhs.multiplier_ * rhs.get_multiplier());
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator*(const Derived& lhs,
                                  const quantity_value<U2, Q2, T2>& rhs) {
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind * Q2, result_type>(
        lhs.get_value() * rhs.get_value(),
        lhs.multiplier_ * U2.get_multiplier());
  }

  MODULE_EXPORT template <typename T2>
    requires requires(Derived lhs, T2 rhs) { lhs.get_value() * rhs; } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator*(const Derived& lhs, const T2& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs.get_value() * rhs)>;
    return quantity_holder<Derived::quantity_kind, result_type>(
        lhs.get_value() * rhs, lhs.multiplier_);
  }

  MODULE_EXPORT template <typename T2>
    requires requires(T2 lhs, Derived rhs) { lhs * rhs.get_value(); } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator*(const T2& lhs, const Derived& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind, result_type>(
        lhs * rhs.get_value(), rhs.multiplier_);
  }

  MODULE_EXPORT template <auto Q2, typename T2>
  friend constexpr auto operator/(const Derived& lhs,
                                  const quantity_holder<Q2, T2>& rhs) {
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind / Q2, result_type>(
        lhs.get_value() / rhs.get_value(),
        lhs.multiplier_ / rhs.get_multiplier());
  }

  MODULE_EXPORT template <typename T2>
    requires requires(Derived lhs, T2 rhs) { lhs.get_value() / rhs; } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator/(const Derived& lhs, const T2& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs.get_value() / rhs)>;
    return quantity_holder<Derived::quantity_kind, result_type>(
        lhs.get_value() / rhs, lhs.multiplier_);
  }
};
} // namespace _detail
/// \endcond

MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
class quantity_holder
    : _detail::quantity_holder_arithmetic_operators<quantity_holder<Q, T>> {
public:
  using value_type = T;
  using quantity_kind_type = decltype(Q);
  static constexpr quantity auto quantity_kind = Q;

  constexpr quantity_holder() = default;

  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_holder_v<Up>) &&
             (!_detail::quantity_value_like<Up>)
  constexpr explicit quantity_holder(Up&& u, unit auto units);

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr explicit quantity_holder(std::in_place_t, Args&&... args,
                                     unit auto units);

  template <typename U, typename... Args>
    requires std::constructible_from<T, std::initializer_list<U>, Args...>
  constexpr explicit quantity_holder(std::in_place_t,
                                     std::initializer_list<U> il,
                                     Args&&... args, unit auto units);

  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && enable_chrono_conversions_v<Q>
  constexpr explicit quantity_holder(
      const std::chrono::duration<Rep, Period>& d);

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(quantity_value<FromQuantity, FromUnit, Up> other);

  template <unit Unit, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(Up&& u, Unit);

  constexpr auto get_value() const& noexcept -> const T&;
  constexpr auto get_value() && noexcept -> T&&;
  constexpr auto get_value() const&& noexcept -> const T&&;

  constexpr auto get_multiplier() const noexcept -> double;

  constexpr auto get_reference() const noexcept -> double;

  template <auto ToUnit>
  constexpr auto as() const -> quantity_value<Q, ToUnit, T>;

  constexpr auto in_base_units() const -> quantity_holder<Q, T>;

  constexpr explicit(!quantity_convertible_to<Q, number>)
  operator value_type() const;

private:
  template <typename Up = T>
    requires std::constructible_from<Up, T>
  constexpr quantity_holder(Up&& other, double multiplier)
      : value_(std::forward<Up>(other)), multiplier_(multiplier) {}

  friend class std::hash<maxwell::quantity_holder<Q, T>>;

  friend class _detail::quantity_holder_arithmetic_operators<
      quantity_holder<Q, T>>;

  T value_{};
  double multiplier_{1.0};
  double reference_{0.0};
};

// --- Begin Implementation ----

template <auto Q, typename T>
  requires quantity<decltype(Q)>
template <typename Up>
  requires std::constructible_from<T, Up> &&
               (!_detail::is_quantity_holder_v<Up>) &&
               (!_detail::quantity_value_like<Up>)
constexpr quantity_holder<Q, T>::quantity_holder(Up&& u, unit auto units)
    : value_(std::forward<Up>(u)), multiplier_(units.multiplier),
      reference_(units.reference) {}

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
template <unit Unit, typename Up>
  requires std::constructible_from<T, Up>
constexpr quantity_holder<Q, T>::quantity_holder(Up&& u, Unit)
    : value_(std::forward<Up>(u)), multiplier_(Unit::multiplier),
      reference_(Unit::reference) {
  static_assert(quantity_convertible_to<Unit::quantity, Q>,
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

// --- Class Template Argument Deduction Guides ---

template <unit Unit, typename T>
quantity_holder(T, Unit) -> quantity_holder<Unit::quantity, T>;

template <auto Q, auto U, typename T>
  requires quantity<decltype(Q)>
quantity_holder(quantity_value<Q, U, T>) -> quantity_holder<Q, T>;

template <auto Q, typename T>
quantity_holder(quantity_holder<Q, T>) -> quantity_holder<Q, T>;
} // namespace maxwell

MODULE_EXPORT template <auto Q, typename T>
struct std::hash<maxwell::quantity_holder<Q, T>> {
  auto operator()(const maxwell::quantity_holder<Q, T>& q) noexcept
      -> std::size_t {
    std::size_t hash_code = std::hash<T>{}(q.in_base_units().value_);
    return hash_code;
  }
};

#endif