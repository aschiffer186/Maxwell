/// \file quantity_holder.hpp
/// \brief Definition of class template \c quantity_holder.

#ifndef QUANTITY_HOLDER_HPP
#define QUANTITY_HOLDER_HPP

#include <functional>  // hash
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
} // namespace _detail
/// \endcond

MODULE_EXPORT template <auto Q, typename T>
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
  constexpr auto as() const -> quantity_value<ToQuantity, ToUnit, T> {
    static_assert(quantity_convertible_to<Q, ToUnit.quantity>,
                  "Cannot convert to specified unit");

    return quantity_value<ToQuantity, ToUnit, T>(
        value_ * multiplier_ / static_cast<double>(ToUnit.multiplier));
  }

private:
  friend class std::hash<maxwell::quantity_holder<Q, T>>;

  T value_{};
  double multiplier_{1.0};
};
} // namespace maxwell

MODULE_EXPORT template <auto Q, typename T>
struct std::hash<maxwell::quantity_holder<Q, T>> {
  auto
  operator()(const maxwell::quantity_holder<Q, T>& q) noexcept -> std::size_t {
    std::size_t hash_code = std::hash<T>{}(q.value_);
    hash_code ^=
        std::hash<decltype(q.quantity_kind.name)>{}(q.quantity_kind.name) +
        0x9e377b9b + (hash_code << 6) + (hash_code >> 2);
    return hash_code;
  }
};

#endif