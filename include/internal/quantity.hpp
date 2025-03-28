#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <chrono>
#include <concepts>
#include <initializer_list>
#include <ratio>
#include <type_traits>
#include <utility>

#include "internal/config.hpp"
#include "internal/dimension.hpp"
#include "internal/utility.hpp"
#include "unit.hpp"

namespace maxwell {
template <typename T, unit auto U> class basic_quantity;

namespace _detail {
template <typename> struct is_basic_quantity : std::false_type {};

template <typename T, unit auto U>
struct is_basic_quantity<basic_quantity<T, U>> : std::true_type {};

template <typename T>
constexpr bool is_basic_quantity_v = is_basic_quantity<T>::value;

template <typename D, unit auto U>
  requires time_unit<U>
constexpr double from_chrono_conversion_factor() {
  using Period = std::ratio_divide<std::ratio<1>, typename D::period>;

  const unit_type<null_dimension, null_dimension, null_dimension,
                  null_dimension, null_dimension, null_dimension,
                  dimension{1, 0, from_ratio<Period>()}>
      as_maxwell_unit;
  return conversion_factor(as_maxwell_unit, U);
}

template <typename D, unit auto U>
  requires time_unit<U>
constexpr double to_chrono_conversion_factor() {
  using Period = std::ratio_divide<std::ratio<1>, typename D::period>;
  const unit_type<null_dimension, null_dimension, null_dimension,
                  null_dimension, null_dimension, null_dimension,
                  dimension{1, 0, from_ratio<Period>()}>
      as_maxwell_unit;
  return conversion_factor(U, as_maxwell_unit);
}
} // namespace _detail

/// \class basic_quantity
/// \brief A dimensioned quantity
///
/// <tt>class basic_quantity</tt> represents a value that has both a magnitude
/// and units. The units are part of the quantity's type, allowing for
/// verification of unit coherency and conversion of units at compile-time.
/// A program is ill-formed if an invalid operation is performed between
/// two quantities with incompatability types.
///
/// \sa unitType
///
/// Mandates: \c T is not cv-qualified, not \c std::in_place_t, not a
/// specialization of \c std::chrono::duration, not a specialization of \c
/// basic_quantity, and is not a specialization of \c unit_type or \c
/// measure_type
///
/// \tparam T the type of the quantity's magnitude
/// \tparam U the quantity's units
template <typename T, unit auto U> class basic_quantity {
  static_assert(!std::is_const_v<T>);

public:
  using scalar_type = T;
  using units_type = std::remove_cvref_t<decltype(U)>;

  static constexpr units_type units = U;

  constexpr basic_quantity() noexcept
    requires std::is_default_constructible_v<scalar_type>
  = default;

  template <typename Up = scalar_type>
    requires std::constructible_from<scalar_type, Up> &&
             (!_detail::is_basic_quantity_v<std::remove_cvref_t<Up>>)
  constexpr explicit(!unitless_unit<units>) basic_quantity(Up&& u) noexcept(
      std::is_nothrow_constructible_v<scalar_type, Up&&>)
      : magnitude_{std::forward<Up>(u)} {}

  template <typename... Args>
    requires std::constructible_from<scalar_type, Args...>
  constexpr explicit basic_quantity(std::in_place_t, Args&&... args) noexcept(
      std::is_nothrow_constructible_v<scalar_type, Args&&...>)
      : magnitude_{std::forward<Args>(args)...} {}

  template <typename Up, typename... Args>
    requires std::constructible_from<scalar_type, std::initializer_list<Up>,
                                     Args&&...>
  constexpr explicit basic_quantity(
      std::in_place_t, std::initializer_list<Up> il,
      Args&&... args) noexcept(std::
                                   is_nothrow_constructible_v<
                                       scalar_type, std::initializer_list<Up>,
                                       Args&&...>)
      : magnitude_{il, std::forward<Args>(args)...} {}

  template <typename Rep, typename Period>
    requires std::constructible_from<scalar_type, Rep> && time_unit<units>
  MAXWELL_CONSTEXPR23 basic_quantity(std::chrono::duration<Rep, Period> time) {}

  template <typename Up, unit auto V>
    requires std::constructible_from<scalar_type, Up> &&
             unit_convertible_to<V, units>
  constexpr basic_quantity(const basic_quantity<Up, V>&) {}

  template <typename Up, unit auto V>
    requires std::constructible_from<scalar_type, Up> &&
             unit_convertible_to<V, units>
  constexpr basic_quantity(basic_quantity<Up, V>&&) noexcept(
      std::is_nothrow_constructible_v<scalar_type,
                                      std::add_rvalue_reference_t<Up>>) {}

  template <typename Up, unit auto V>
    requires std::constructible_from<scalar_type, Up> &&
             unit_convertible_to<V, units>
  constexpr basic_quantity& operator=(const basic_quantity&) {
    return *this;
  }

  template <typename Rep, typename Period>
    requires std::assignable_from<scalar_type&, Rep> && time_unit<units>
  MAXWELL_CONSTEXPR23 basic_quantity(std::chrono::duration<Rep, Period> time) {
    return *this;
  }

  template <typename Up = scalar_type>
    requires std::assignable_from<scalar_type&, Up>
  constexpr basic_quantity& operator=(Up&& other) noexcept(
      std::is_nothrow_assignable_v<scalar_type&, Up>) {
    magnitude_ = std::forward<Up>(other);
    return *this;
  }

  constexpr units_type get_units() const noexcept { return U; }

  constexpr const scalar_type& get_magnitude() const& noexcept {
    return magnitude_;
  }

  constexpr scalar_type&& get_magnitude() && noexcept {
    return std::move(magnitude_);
  }

  constexpr const scalar_type&& get_magnitude() const&& noexcept {
    return std::move(magnitude_);
  }

  constexpr explicit(!unitless_unit<units>)
  operator scalar_type() const noexcept {
    return magnitude_;
  }

  // --- Quantity comparison ---

  friend auto constexpr operator<=>(const basic_quantity&,
                                    const basic_quantity&) = default;

  friend constexpr bool operator==(const basic_quantity&,
                                   const basic_quantity&) = default;

  // --- Quantity manipulation ---

  constexpr basic_quantity& operator+=(const basic_quantity& other)
    requires addable<scalar_type>
  {
    magnitude_ += other.magnitude();
    return *this;
  }

  constexpr basic_quantity& operator-=(const basic_quantity& other) noexcept(
      nothrow_subtractable<scalar_type>)
    requires subtractable<scalar_type>
  {
    magnitude_ -= other.magnitude();
    return *this;
  }

  template <typename Up, unit auto V>
    requires addable_with<T, Up> && unit_convertible_to<V, units>
  constexpr basic_quantity&
  operator+=(const basic_quantity<Up, V>& other) noexcept(
      nothrow_subtractable_with<T, Up>) {}

  constexpr basic_quantity&
  operator*=(const scalar_type& scalar) noexcept(nothrow_multiply<scalar_type>)
    requires multiply<scalar_type>
  {
    magnitude_ *= scalar;
    return *this;
  }

  constexpr basic_quantity&
  operator/=(const scalar_type& scalar) noexcept(nothrow_divide<scalar_type>)
    requires divide<scalar_type>
  {
    magnitude_ /= scalar;
    return *this;
  }

private:
  scalar_type magnitude_{};
};

template <typename S1, unit auto U1, typename S2, unit auto U2>
constexpr auto operator*(const basic_quantity<S1, U1>& lhs,
                         const basic_quantity<S2, U2>&) {
  return basic_quantity<S1, U1 * U2>{lhs.get_magnitude()};
}

template <typename S1, unit auto U1, typename S2, unit auto U2>
constexpr auto operator/(const basic_quantity<S1, U1>& lhs,
                         const basic_quantity<S2, U2>&) {
  return basic_quantity<S1, U1 / U2>{lhs.get_magnitude()};
}

using mole = basic_quantity<double, mole_unit>;
using ampere = basic_quantity<double, ampere_unit>;

template <typename T>
concept amount = amount_unit<typename T::units_type{}>;

template <typename T>
concept current = current_unit<typename T::units_type{}>;

template <typename T>
concept length = length_unit<typename T::units_type{}>;

template <typename T>
concept luminosity = luminosity_unit<typename T::units_type{}>;

template <typename T>
concept mass = mass_unit<typename T::units_type{}>;

template <typename T>
concept temperature = temperature_unit<typename T::units_type{}>;

template <typename T>
concept time = time_unit<typename T::units_type{}>;
} // namespace maxwell

#endif