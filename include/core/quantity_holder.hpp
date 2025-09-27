/// \file quantity_holder.hpp
/// \brief Definition of class template \c quantity_holder.

#ifndef QUANTITY_HOLDER_HPP
#define QUANTITY_HOLDER_HPP

#include <functional> // hash
#include <initializer_list>
#include <type_traits> // false_type, remove_cvref_t, true_type

#include "quantity.hpp"
#include "quantity_value.hpp"

namespace maxwell {
MODULE_EXPORT template <auto Q, typename T = double>
  requires quantity<decltype(Q)>
class quantity_holder;

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

  template <auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator+=(Derived& lhs, const quantity_holder<Q2, T2>& rhs) -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot add quantities of different kinds");
    lhs.value_ += Derived(rhs).value_;
    return lhs;
  }

  template <auto U2, auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator+=(Derived& lhs, const quantity_value<U2, Q2, T2>& rhs) -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot add quantities of different kinds");
    lhs.value_ += Derived(rhs).value_;
    return lhs;
  }

  template <auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator-=(Derived& lhs, const quantity_holder<Q2, T2>& rhs) -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot subtract quantities of different kinds");
    lhs.value_ -= Derived(rhs).value_;
    return lhs;
  }

  template <auto U2, auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator-=(Derived& lhs, const quantity_value<U2, Q2, T2>& rhs) -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity_kind> &&
                      quantity_convertible_to<Derived::quantity_kind, Q2>,
                  "Cannot subtract quantities of different kinds");
    lhs.value_ -= Derived(rhs).value_;
    return lhs;
  }

  template <auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator+(Derived lhs, const quantity_holder<Q2, T2>& rhs) {
    return lhs += rhs;
  }

  template <auto U2, auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator+(Derived lhs, const quantity_value<U2, Q2, T2>& rhs) {
    return lhs += rhs;
  }

  template <auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator-(Derived lhs, const quantity_holder<Q2, T2>& rhs) {
    return lhs -= rhs;
  }

  template <auto U2, auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator-(Derived lhs, const quantity_value<U2, Q2, T2>& rhs) {
    return lhs -= rhs;
  }

  template <auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator*(const Derived& lhs, const quantity_holder<Q2, T2>& rhs) {

    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind * Q2, result_type>(
        lhs.get_value() * rhs.get_value(),
        lhs.multiplier_ * rhs.get_multiplier());
  }

  template <auto Q2, typename T2>
  MODULE_EXPORT friend constexpr auto
  operator/(const Derived& lhs, const quantity_holder<Q2, T2>& rhs) {
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity_kind / Q2, result_type>(
        lhs.get_value() / rhs.get_value(),
        lhs.multiplier_ / rhs.get_multiplier());
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
  constexpr explicit quantity_holder(Up&& u) : value_(std::forward<Up>(u)) {}

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr explicit quantity_holder(std::in_place_t, Args&&... args)
      : value_(std::forward<Args>(args)...) {}

  template <typename U, typename... Args>
    requires std::constructible_from<T, std::initializer_list<U>, Args...>
  constexpr explicit quantity_holder(std::in_place_t,
                                     std::initializer_list<U> il,
                                     Args&&... args)
      : value_(il, std::forward<Args>(args)...) {}

  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && enable_chrono_conversions_v<Q>
  constexpr explicit quantity_holder(
      const std::chrono::duration<Rep, Period>& d)
      : value_(d.count()), multiplier_(static_cast<double>(Period::den) /
                                       static_cast<double>(Period::num)) {}

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(quantity_value<FromQuantity, FromUnit, Up> other)
      : value_(std::move(other).get_value()), multiplier_(FromUnit.multiplier),
        reference_(FromUnit.reference) {
    static_assert(quantity_convertible_to<FromUnit.quantity, Q>,
                  "Cannot convert from units of other to quantity of value "
                  "being constructed");
  }

  template <unit Unit, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(Up&& u, Unit)
      : value_(std::forward<Up>(u)), multiplier_(Unit::multiplier),
        reference_(Unit::reference) {
    static_assert(quantity_convertible_to<Unit::quantity, Q>,
                  "Cannot convert from units of other to quantity of value "
                  "being constructed");
  }

  constexpr auto get_value() const& noexcept -> const T& { return value_; }
  constexpr auto get_value() && noexcept -> T&& { return std::move(value_); }
  constexpr auto get_value() const&& noexcept -> const T&& {
    return std::move(value_);
  }

  constexpr auto get_multiplier() const noexcept -> double {
    return multiplier_;
  }

  constexpr auto get_reference() const noexcept -> double { return reference_; }

  template <auto ToUnit>
  constexpr auto as() const -> quantity_value<Q, ToUnit, T> {
    static_assert(quantity_convertible_to<Q, ToUnit.quantity>,
                  "Cannot convert to specified unit");

    return quantity_value<Q, ToUnit, T>(value_ * multiplier_ /
                                        static_cast<double>(ToUnit.multiplier));
  }

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
    std::size_t hash_code = std::hash<T>{}(q.value_);
    hash_code ^=
        std::hash<decltype(q.quantity_kind.name)>{}(q.quantity_kind.name) +
        0x9e377b9b + (hash_code << 6) + (hash_code >> 2);
    return hash_code;
  }
};

#endif