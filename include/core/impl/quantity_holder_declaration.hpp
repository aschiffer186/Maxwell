
#ifndef QUANTITY_HOLDER_HPP
#error                                                                         \
    "Do not include quantity_holder_impl.hpp directly; include quantity_holder.hpp instead"
#endif

#include <chrono>           // duration
#include <functional>       // hash
#include <initializer_list> // initializer_list
#include <stdexcept>        // runtime_error
#include <string>           // string
#include <type_traits>      // false_type, remove_cvref_t, true_type

#include "../quantity.hpp"
#include "quantity_value_holder_fwd.hpp"

namespace maxwell {
class incompatible_quantity_holder : public std::runtime_error {
public:
  explicit incompatible_quantity_holder(const std::string& message)
      : std::runtime_error(message) {}
};

/// \cond
namespace _detail {
template <typename Derived> class quantity_holder_arithmetic_operators {
  friend constexpr auto operator++(Derived& d) -> Derived& {
    ++d.value_;
    return d;
  }

  friend constexpr auto operator++(Derived& d, int) -> Derived {
    auto temp{d};
    ++d;
    return temp;
  }

  friend constexpr auto operator--(Derived& d) -> Derived& {
    --d.value_;
    return d;
  }

  friend constexpr auto operator--(Derived& d, int) -> Derived {
    auto temp{d};
    --d;
    return temp;
  }

  constexpr auto operator-(const Derived& d) -> Derived {
    return Derived{-d.value_};
  }

  template <auto Q2, typename T2>
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

  template <auto U2, auto Q2, typename T2>
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

  template <typename T2>
    requires(!quantity_value_like<T2> && !is_quantity_holder<T2>::value &&
             quantity_convertible_to<Derived::quantity, number>)
  friend constexpr auto operator+=(Derived& lhs, T2&& rhs) -> Derived& {
    lhs.value_ += std::forward<T2>(rhs);
    return lhs;
  }

  template <auto Q2, typename T2>
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

  template <auto U2, auto Q2, typename T2>
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

  template <typename T2>
    requires(!quantity_value_like<T2> && !is_quantity_holder<T2>::value &&
             quantity_convertible_to<Derived::quantity, number>)
  friend constexpr auto operator-=(Derived& lhs, T2&& rhs) -> Derived& {
    lhs.value_ -= std::forward<T2>(rhs);
    return lhs;
  }

  template <auto Q2, typename T2>
  friend constexpr auto operator+(Derived lhs,
                                  const quantity_holder<Q2, T2>& rhs) {
    return lhs += rhs;
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator+(Derived lhs,
                                  const quantity_value<U2, Q2, T2>& rhs) {
    return lhs += rhs;
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator+(const quantity_value<U2, Q2, T2>& lhs,
                                  const Derived& rhs) {
    return rhs += lhs;
  }

  template <auto Q2, typename T2>
  friend constexpr auto operator-(Derived lhs,
                                  const quantity_holder<Q2, T2>& rhs) {
    return lhs -= rhs;
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator-(Derived lhs,
                                  const quantity_value<U2, Q2, T2>& rhs) {
    return lhs -= rhs;
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator-(const quantity_value<U2, Q2, T2>& lhs,
                                  const Derived& rhs) {
    return Derived(lhs) -= rhs;
  }

  template <auto Q2, typename T2>
  friend constexpr auto operator*(const Derived& lhs,
                                  const quantity_holder<Q2, T2>& rhs) {

    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind * Q2, result_type>(
        lhs.get_value() * rhs.get_value(),
        lhs.multiplier_ * rhs.get_multiplier());
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator*(const Derived& lhs,
                                  const quantity_value<U2, Q2, T2>& rhs) {
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind * Q2, result_type>(
        lhs.get_value() * rhs.get_value(),
        lhs.multiplier_ * U2.get_multiplier());
  }

  template <typename T2>
    requires requires(Derived lhs, T2 rhs) { lhs.get_value() * rhs; } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator*(const Derived& lhs, const T2& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs.get_value() * rhs)>;
    return quantity_holder<Derived::quantity_kind, result_type>(
        lhs.get_value() * rhs, lhs.multiplier_);
  }

  template <typename T2>
    requires requires(T2 lhs, Derived rhs) { lhs * rhs.get_value(); } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator*(const T2& lhs, const Derived& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind, result_type>(
        lhs * rhs.get_value(), rhs.multiplier_);
  }

  template <auto Q2, typename T2>
  friend constexpr auto operator/(const Derived& lhs,
                                  const quantity_holder<Q2, T2>& rhs) {
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind / Q2, result_type>(
        lhs.get_value() / rhs.get_value(),
        lhs.multiplier_ / rhs.get_multiplier());
  }

  template <typename T2>
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

  constexpr explicit quantity_holder(unit auto units) noexcept(
      std::is_nothrow_default_constructible_v<T>)
    requires std::is_default_constructible_v<T>;

  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_holder_v<Up>) &&
             (!_detail::quantity_value_like<Up> && !unit<Up>)
  constexpr quantity_holder(Up&& u, unit auto units);

  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_holder_v<Up>) &&
             (!_detail::quantity_value_like<Up> && !unit<Up>)
  constexpr quantity_holder(Up&& u, double multiplier, double reference);

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr quantity_holder(std::in_place_t, Args&&... args, unit auto units);

  template <typename U, typename... Args>
    requires std::constructible_from<T, std::initializer_list<U>, Args...>
  constexpr quantity_holder(std::in_place_t, std::initializer_list<U> il,
                            Args&&... args, unit auto units);

  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && enable_chrono_conversions_v<Q>
  constexpr explicit quantity_holder(
      const std::chrono::duration<Rep, Period>& d);

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(quantity_value<FromQuantity, FromUnit, Up> other);

  constexpr auto get_value() const& noexcept -> const T&;
  constexpr auto get_value() && noexcept -> T&&;
  constexpr auto get_value() const&& noexcept -> const T&&;

  constexpr auto get_multiplier() const noexcept -> double;

  constexpr auto get_reference() const noexcept -> double;

  template <auto ToUnit>
  constexpr auto as() const -> quantity_value<ToUnit, Q, T>;

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

// --- Class Template Argument Deduction Guides ---

template <unit Unit, typename T>
quantity_holder(T, Unit) -> quantity_holder<Unit::quantity, T>;

template <auto Q, auto U, typename T>
  requires quantity<decltype(Q)>
quantity_holder(quantity_value<Q, U, T>) -> quantity_holder<Q, T>;

template <auto Q, typename T>
quantity_holder(quantity_holder<Q, T>) -> quantity_holder<Q, T>;
} // namespace maxwell
