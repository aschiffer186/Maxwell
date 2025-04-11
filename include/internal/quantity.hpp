#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <chrono>
#include <concepts>
#include <format>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <ostream>
#include <ratio>
#include <string_view>
#include <type_traits>
#include <utility>

#include "config.hpp"
#include "dimension.hpp"
#include "unit.hpp"
#include "utility.hpp"

namespace maxwell {
template <unit auto U, typename T = double> class quantity;

namespace _detail {
template <typename> struct is_basic_quantity : std::false_type {};

template <typename T, unit auto U> struct is_basic_quantity<quantity<U, T>> : std::true_type {};

template <typename T> constexpr bool is_basic_quantity_v = is_basic_quantity<T>::value;

template <typename D, unit auto U>
  requires time_unit<U>
constexpr double from_chrono_conversion_factor() {
  using Period = std::ratio_divide<std::ratio<1>, typename D::period>;

  const unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
                  dimension{1, 0, from_ratio<Period>()}>
      as_maxwell_unit;
  return conversion_factor(as_maxwell_unit, U);
}

template <typename D, unit auto U>
  requires time_unit<U>
constexpr double to_chrono_conversion_factor() {
  using Period = std::ratio_divide<std::ratio<1>, typename D::period>;
  const unit_type<null_dimension, null_dimension, null_dimension, null_dimension, null_dimension, null_dimension,
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
template <unit auto U, typename T> class quantity {
  static_assert(!std::is_const_v<T>);

public:
  /// The type of the quantity's magnitude
  using magnitude_type = T;
  /// The type of the quantity's units
  using units_type = std::remove_cvref_t<decltype(U)>;
  /// The quantity's units
  static constexpr units_type units = U;

  /// \brief Default constructor
  ///
  /// Creates a \c Quantity whose magnitude is default constructed.
  ///
  /// \pre \c magnitude_type is default constructible
  /// \post <tt> this->magnitude()</tt> is equal to \c magnitude_type{}
  ///
  /// \throws any exceptions thrown by the default constructor of \c
  /// magnitude_type
  constexpr quantity()
    requires std::is_default_constructible_v<magnitude_type>
  = default;

  /// \brief Constructor
  ///
  /// Constructs a \c basic_quantity whose magnitude is constructed using the
  /// value \c std::forward<Up>(magnitude)
  ///
  /// \pre \c magnitude_type is constructible from \c Up&&
  /// \post \c this->magnitude() is equal to \c
  /// magnitude_type(std::forward<Up>(magnitude))
  ///
  /// \throws Any exceptions thrown by the constructor of \c magnitude_type
  template <typename Up = magnitude_type>
    requires std::constructible_from<magnitude_type, Up> && (!_detail::is_basic_quantity_v<std::remove_cvref_t<Up>>)
  constexpr explicit(!unitless_unit<units> || !std::convertible_to<Up, magnitude_type>)
      quantity(Up&& u) noexcept(std::is_nothrow_constructible_v<magnitude_type, Up&&>)
      : magnitude_(std::forward<Up>(u)) {}

  /// \c Constructor
  ///
  /// Constructs a \c basic_quantity whose magnitude s constructed in place from
  /// the specified arguments using the expression \c
  /// std::forward<Args>(args)...
  ///
  /// \pre \c magnitude_type is constructible from \c Args
  /// \post \c this->magnitude() is equal to \c
  /// magnitude_type(std::forward<Args>(args)...)
  ///
  /// \throws Any exceptions thrown by the constructor of \c magnitude_type
  template <typename... Args>
    requires std::constructible_from<magnitude_type, Args...>
  constexpr explicit quantity(std::in_place_t,
                              Args&&... args) noexcept(std::is_nothrow_constructible_v<magnitude_type, Args&&...>)
      : magnitude_(std::forward<Args>(args)...) {}

  /// \c Constructor
  ///
  /// Constructs a \c basic_quantity whose magnitude s constructed in place from
  /// the specified initializer list and arguments arguments using the
  /// expression <tt>magnitude_type(il, std::forward<Args>(args)...)</tt>
  ///
  /// \pre \c magnitude_type is constructible from \c Args
  /// \post \c this->magnitude() is equal to <tt>magnitude_type(il,
  /// std::forward<Args>(args)...)</t>>
  ///
  /// \throws Any exceptions thrown by the constructor of \c magnitude_type
  template <typename Up, typename... Args>
    requires std::constructible_from<magnitude_type, std::initializer_list<Up>, Args&&...>
  constexpr explicit quantity(std::in_place_t, std::initializer_list<Up> il, Args&&... args) noexcept(
      std::is_nothrow_constructible_v<magnitude_type, std::initializer_list<Up>, Args&&...>)
      : magnitude_(il, std::forward<Args>(args)...) {}

  /// \brief Constructor
  ///
  /// Constructs a \c basic_quantity from a \c std::chrono::duration type,
  /// allowing for integration with the standard library. This constructor is
  /// implicit if there would be no loss of information converting from the
  /// specified \c std::chrono::duration type.
  ///
  /// \pre \c magnitude_type is constructible from \c Rep
  /// \pre \c units has dimensions of time
  /// \post \c this->magnitude() is equal to \c dur.count()
  ///
  /// \param dur the \c std::chrono::duration value to construct from
  ///
  /// \throw any exceptions thrown by \c dur or by the constructor of \c
  /// magnitude_type
  template <typename Rep, typename Period>
    requires std::constructible_from<magnitude_type, Rep> && time_unit<units>
  MAXWELL_CONSTEXPR23 quantity(std::chrono::duration<Rep, Period> dur)
      : magnitude_(dur.cout() * _detail::from_chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>()) {}

  /// \brief Converting constructor
  ///
  /// Constructs a \c basic_quantity from the specified \c basic_quantity with
  /// different units, automatically converting the units of the specified
  /// quantity to \c Units. The magnitude of \c *this is copy constructed from
  /// \c q.magnitude() then multiplied by the appropriate conversion factor
  /// between \c Other and \c Units.
  ///
  /// \pre \c magnitude_type is constructible from \c Up
  /// \pre \c Other is convertible to \c Units.
  /// \post <tt>this->magnitude() == conversion_factor(Other,
  /// Units)*q.magnitude()</tt>
  ///
  /// \tparam Up the type of the magnitude of the \c basic_quantity to construct
  /// from \tparam Other the units ofthe \c basic_quantity to construct from
  /// \param q the \c basic_quantity to construct from
  ///
  /// \throw any exceptions thrown by the move constructor of \c magnitude_type
  template <typename Up, unit auto V>
    requires unit_convertible_to<V, units> && std::constructible_from<magnitude_type, Up>
  constexpr quantity(const quantity<V, Up>& q)
      : magnitude_(q.get_magnitude() * conversion_factor(q.get_units(), units) + conversion_offset(V, units)) {}

  template <typename Up>
    requires std::constructible_from<magnitude_type, Up>
  constexpr explicit(!std::convertible_to<Up, magnitude_type>) quantity(const quantity<units, Up>& q)
      : magnitude_(q.get_magnitude()) {}

  template <typename Up>
    requires std::constructible_from<magnitude_type, Up>
  constexpr explicit(!std::convertible_to<Up, magnitude_type>) quantity(quantity<units, Up>&& q)
      : magnitude_(std::move(q).get_magnitude()) {}

  /// \brief Converting constructor
  ///
  /// Constructs a \c basic_quantity from the specified \c basic_quantity with
  /// different units, automatically converting the units of the specified
  /// quantity to \c Units. The magnitude of \c *this is move constructed from
  /// \c q.magnitude() then multiplied by the appropriate conversion factor
  /// between \c Other and \c Units.
  ///
  /// \pre \c magnitude_type is constructible from \c Up
  /// \pre \c Other is convertible to \c Units.
  /// \post <tt>this->magnitude() == conversion_factor(Other,
  /// Units)*q.magnitude()</tt>
  ///
  /// \tparam Up the type of the magnitude of the \c basic_quantity to construct
  /// from \tparam Other the units ofthe \c basic_quantity to construct from
  /// \param q the \c basic_quantity to construct from
  ///
  /// \throw any exceptions thrown by the move constructor of \c magnitude_type
  template <typename Up, unit auto V>
    requires unit_convertible_to<V, units> && std::constructible_from<magnitude_type, Up>
  constexpr quantity(quantity<V, Up>&& q) noexcept(
      std::is_nothrow_constructible_v<magnitude_type, std::add_rvalue_reference_t<Up>>)
      : magnitude_(std::move(q).get_magnitude() * conversion_factor(q.get_units(), units) +
                   conversion_offset(V, units)) {}

  /// \brief Converting assignment operator
  ///
  /// Converts the magitude of \c q from \c Other to \c Units then assigns the
  /// value to the magnitude of \c *this.
  ///
  /// \pre \c Up is swappable with \c magnitude_type
  /// \pre \c Other is convertible to \c units
  /// \post <tt>this->magnitude() == conversion_factor(Other,
  /// Units)*q.magnitude()</tt>
  ///
  /// \tparam Up the type of the magnitude of the \c basic_quantity being
  /// assigned from \tparam Other the units of the \c basic_quantity being
  /// assigned from \param q the \c basic_quantity to assign to \c *this
  ///
  /// \return a reference to \c *this
  ///
  /// \throws any exceptions through by swapping \c Up and \c magnitude_type
  template <typename Up, unit auto V>
    requires unit_convertible_to<V, units> && std::constructible_from<magnitude_type, Up>
  constexpr quantity& operator=(quantity other) {
    using std::swap;
    quantity temp(std::move(other));
    swap(temp.magnitude_, magnitude_);
    return *this;
  }

  /// \brief Converting assignment operator
  ///
  /// Converts the magitude of \c dur from the period specified by the \c
  /// std::chrono::duration to \c Units
  ///
  /// \pre \c magnitude_type is constructible from \c Rep
  /// \pre \c time_unit<units>
  /// \post <tt>this->magnitude() == conversion_factor(Other,
  /// Units)*dur.coun</tt>
  ///
  /// \tparam Rep the representation type of the \c std::chrono::duration
  /// \tparam Period the period of the \c std::chrono::duration
  /// \param dur the \c std::chrono::duration to assign to \c *this
  ///
  /// \return a refeence to \c *this
  ///
  /// \throws any exceptions thrown by the copy constructor of \c dur
  template <typename Rep, typename Period>
    requires std::assignable_from<magnitude_type&, Rep> && time_unit<units>
  MAXWELL_CONSTEXPR23 quantity(std::chrono::duration<Rep, Period> dur)
      : magnitude_(dur.count() * _detail::from_chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>()) {
    return *this;
  }

  /// \brief Assignment operator
  ///
  /// Assigns the specified value to the magnitude of \c *this. This operator
  /// only applies to unitless quantities.
  ///
  /// \pre \c magnitude_type is assignable from \c Up
  /// \pre \c *this is unitless
  ///
  /// \tparam Up the the type of the value to assign to \c *this
  /// \param up the value to assign to \c *this
  ///
  /// \return a reference to \c *this
  template <typename Up = magnitude_type>
    requires unitless_unit<units> && std::assignable_from<magnitude_type&, Up>
  constexpr quantity& operator=(Up&& other) noexcept(std::is_nothrow_assignable_v<magnitude_type&, Up>) {
    magnitude_ = std::forward<Up>(other);
    return *this;
  }

  /// \brief Returns the units of \c *this
  ///
  /// \return The units of \c *this
  constexpr units_type get_units() const noexcept { return units; }

  /// \brief Returns the magnitude of the \c *this
  ///
  /// \return A \c const reference to the magnitude of \c *this
  constexpr const magnitude_type& get_magnitude() const& noexcept { return magnitude_; }

  /// \brief Returns the magnitude of the \c *this
  ///
  /// \return An rvalue reference to the magnitude of \c *this
  constexpr magnitude_type&& get_magnitude() && noexcept { return std::move(magnitude_); }

  /// \brief Returns the magnitude of the \c *this
  ///
  /// \return A \c const rvalue reference to the magnitude of \c *this
  constexpr const magnitude_type&& get_magnitude() const&& noexcept { return std::move(magnitude_); }

  /// \brief Conversion operator to underlying magnitude type
  ///
  /// Converts the quantity to the underlying type. This operation is implicit
  /// if the quantity is unitless. It is highly recommended to only call this
  /// operator if the quantity is unitless.
  ///
  /// \return the underlying magnitude of the \c basic_quantity
  constexpr explicit(!unitless_unit<units>) operator magnitude_type() const noexcept { return magnitude_; }

  /// \brief Conversion operator to \c std::chrono::duration
  ///
  /// Converts the quantity to a \c std::chrono::duration. This conversion is implicit if
  /// the conversion would not result in a loss of information.
  /// \pre \c units has dimensions of time
  /// \pre \c Rep is constructible from \c magnitude_type
  ///
  /// \tparam Rep the representation type of the \c std::chrono::duration
  /// \tparam Period the period of the \c std::chrono::duration
  /// \return a \c std::chrono::duration that is the same value \c *this
  template <typename Rep, typename Period>
    requires time_unit<units> && multiply_with<magnitude_type, Rep>
  MAXWELL_CONSTEXPR23 operator std::chrono::duration<Rep, Period>() const {
    return std::chrono::duration<Rep, Period>(static_cast<Rep>(
        magnitude_ * _detail::to_chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>()));
  }

  /// \brief Returns a new \c basic_quantity with the same dimensions of \c
  /// *this but in SI base units
  ///
  /// Returns a new \c basic_quantity with the same dimension of \c *this, but
  /// in SI base units. The magnitude of the returned quantity is equivalent to
  /// \c this->magnitude() converted to SI base units
  ///
  /// \return A new \c basic_quantity with the same dimension of \c *this in SI
  /// base units.
  constexpr auto to_SI_base_units() const { return quantity<units.to_SI_base_units(), magnitude_type>(*this); }

  // --- Quantity manipulation ---
  constexpr quantity& operator+=(const quantity& other)
    requires addable<magnitude_type>
  {
    if constexpr (requires(magnitude_type value) { value += value; }) {
      magnitude_ += other.get_magnitude();
    } else {
      magnitude_ = magnitude_ + other.magnitude_;
    }
    return *this;
  }

  constexpr quantity& operator-=(const quantity& other) noexcept(nothrow_subtractable<magnitude_type>)
    requires subtractable<magnitude_type>
  {
    if constexpr (requires(magnitude_type value) { value -= value; }) {
      magnitude_ -= other.get_magnitude();
    } else {
      magnitude_ = magnitude_ - other.magnitude_;
    }
    return *this;
  }

  template <typename Up, unit auto V>
    requires addable_with<T, Up> && unit_convertible_to<V, units>
  constexpr quantity& operator+=(const quantity<V, Up>& other) noexcept(nothrow_addable_with<T, Up>) {
    return *this += quantity(other);
  }

  template <typename Up, unit auto V>
    requires unit_convertible_to<V, units> && subtractable_with<T, Up>
  constexpr quantity& operator-=(const quantity<V, Up>& other) noexcept(nothrow_subtractable_with<T, Up>) {
    return *this -= quantity(other);
  }

  constexpr quantity& operator*=(const magnitude_type& scalar) noexcept(nothrow_multiply<magnitude_type>)
    requires multiply<magnitude_type>
  {
    if constexpr (requires(magnitude_type value) { value *= value; }) {
      magnitude_ *= scalar;
    } else {
      magnitude_ = magnitude_ * scalar;
    }
    return *this;
  }

  constexpr quantity& operator/=(const magnitude_type& scalar) noexcept(nothrow_divide<magnitude_type>)
    requires divide<magnitude_type>
  {
    if constexpr (requires(magnitude_type value) { value /= value; }) {
      magnitude_ /= scalar;
    } else {
      magnitude_ = magnitude_ / scalar;
    }
    return *this;
  }

  constexpr quantity& operator++() {
    ++magnitude_;
    return *this;
  }

  constexpr quantity& operator--() {
    --magnitude_;
    return *this;
  }

  constexpr quantity& operator++(int) {
    quantity temp{*this};
    ++magnitude_;
    return temp;
  }

  constexpr quantity& operator--(int) {
    quantity temp{*this};
    --magnitude_;
    return temp;
  }

private:
  magnitude_type magnitude_{};
};

template <unit auto U1, typename S1, unit auto U2, typename S2>
constexpr bool operator==(const quantity<U1, S1>& lhs, const quantity<U2, S2>& rhs) {
  return lhs.to_SI_base_units().get_magnitude() == rhs.to_SI_base_units().get_magnitude();
}

template <unit auto U1, typename S1, unit auto U2, typename S2>
constexpr auto operator<=>(const quantity<U1, S1>& lhs, const quantity<U2, S2>& rhs) {
  return lhs.to_SI_base_units().get_magnitude() <=> rhs.to_SI_base_units().get_magnitude();
}

template <typename S1, unit auto U1, typename S2, unit auto U2>
constexpr auto operator*(const quantity<U1, S1>& lhs, const quantity<U2, S2>& rhs) {
  const auto lhs_base_units = lhs.to_SI_base_units();
  const auto rhs_base_units = rhs.to_SI_base_units();

  const unit auto return_units = lhs_base_units.get_units() * rhs_base_units.get_units();

  using return_scalar_type =
      std::remove_cvref_t<decltype(lhs_base_units.get_magnitude() * rhs_base_units.get_magnitude())>;
  return quantity<return_units, return_scalar_type>{lhs_base_units.get_magnitude() * rhs_base_units.get_magnitude()};
}

template <typename S1, unit auto U1, typename S2, unit auto U2>
constexpr auto operator/(const quantity<U1, S1>& lhs, const quantity<U2, S2>& rhs) {
  const auto lhs_base_units = lhs.to_SI_base_units();
  const auto rhs_base_units = rhs.to_SI_base_units();

  using return_scalar_type =
      std::remove_cvref_t<decltype(lhs_base_units.get_magnitude() / rhs_base_units.get_magnitude())>;
  return quantity<lhs_base_units.get_units() / rhs_base_units.get_units(), return_scalar_type>{
      lhs_base_units.get_magnitude() / rhs_base_units.get_magnitude()};
}

template <unit auto U1, typename M1, typename M2>
  requires multiply_with<M1, M2> && (!unitless_unit<U1>) && (!_detail::is_basic_quantity_v<std::remove_cvref_t<M2>>) &&
           (!unit<M2>)
constexpr auto operator*(const quantity<U1, M1>& lhs, const M2& rhs) noexcept(nothrow_multiply_with<M1, M2>) {
  // NOTE: Multiplication is NOT guaranteed to be commutative for all possible magnitude_types (e.g. matrices)
  //       DO NOT re-write this as return lhs * rhs!
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() * rhs)>;
  return quantity<U1, return_scalar_type>{lhs.get_magnitude() * rhs};
}

template <unit auto U1, typename M1, typename M2>
  requires multiply_with<M1, M2> && (!unitless_unit<U1>) && (!_detail::is_basic_quantity_v<std::remove_cvref_t<M2>>) &&
           (!unit<M2>)
constexpr auto operator*(const M2& lhs, const quantity<U1, M1>& rhs) noexcept(nothrow_multiply_with<M1, M2>) {
  // NOTE: Multiplication is NOT guaranteed to be commutative for all possible magnitude_types (e.g. matrices)
  //       DO NOT re-write this as return rhs * lhs!
  using return_scalar_type = std::remove_cvref_t<decltype(lhs * rhs.get_magnitude())>;
  return quantity<U1, return_scalar_type>{lhs * rhs.get_magnitude()};
}

template <unit auto U1, typename M1, typename M2>
  requires divide_with<M1, M2> && (!unitless_unit<U1>) && (!_detail::is_basic_quantity_v<std::remove_cvref_t<M2>>)
constexpr auto operator/(const quantity<U1, M1>& lhs, const M2& rhs) noexcept(nothrow_multiply_with<M1, M2>) {
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() / rhs)>;
  return quantity<U1, return_scalar_type>(lhs.get_magnitude() / rhs);
}

template <typename M1, unit auto U1, typename M2, unit auto U2>
  requires unit_convertible_to<U1, U2> && addable_with<M1, M2>
constexpr auto operator+(quantity<U1, M1> lhs, const quantity<U2, M2>& rhs) noexcept(nothrow_addable_with<M1, M2>) {
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() + rhs.get_magnitude())>;

  return quantity<U1.to_SI_base_units(), return_scalar_type>(lhs.to_SI_base_units().get_magnitude() +
                                                             rhs.to_SI_base_units().get_magnitude());
}

template <typename M1, unit auto U1, typename M2, unit auto U2>
  requires unit_convertible_to<U1, U2> && subtractable_with<M1, M2>
constexpr auto operator-(quantity<U1, M1> lhs, const quantity<U2, M2>& rhs) noexcept(subtractable_with<M1, M2>) {
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() - rhs.get_magnitude())>;

  return quantity<U1.to_SI_base_units(), return_scalar_type>(lhs.to_SI_base_units().get_magnitude() -
                                                             rhs.to_SI_base_units().get_magnitude());
}

template <typename M, unit auto U> constexpr quantity<U, M> operator-(const quantity<U, M>& x) {
  return quantity<U, M>(-x.get_magnitude());
}

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

template <typename T>
concept scalar = unitless_unit<typename T::units_type{}>;

template <unit auto U> using int_quantity = quantity<U, int>;
// Quantity printing

template <typename T, unit auto U> std::ostream& operator<<(std::ostream& os, const quantity<U, T>& q) {
  os << std::format("{}", q);
}

template <typename T, unit U>
  requires(!unit<T>)
constexpr quantity<U{}, T> operator*(T&& lhs, U) {
  return quantity<U{}, T>(std::forward<T>(lhs));
}

template <typename T, unit auto U, unit U2> constexpr quantity<U * U2{}, T> operator*(const quantity<U, T>& lhs, U2) {
  return quantity<U * U2{}, T>(lhs.get_magnitude());
}

template <typename T, unit auto U, unit U2> constexpr quantity<U * U2{}, T> operator*(quantity<U, T>&& lhs, U2) {
  return quantity<U * U2{}, T>(std::move(lhs).get_magnitude());
}

template <typename T, unit auto U, unit U2> constexpr quantity<U / U2{}, T> operator/(quantity<U, T>&& lhs, U2) {
  return quantity<U / U2{}, T>(std::move(lhs).get_magnitude());
}
} // namespace maxwell

template <typename T, maxwell::unit auto U>
struct std::formatter<maxwell::quantity<U, T>> : std::formatter<std::string_view> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const maxwell::quantity<U, T>& q, std::format_context& ctx) const {
    std::string temp;
    std::format_to(std::back_inserter(temp), "{}", q.get_magnitude());
    return std::formatter<std::string_view>::format(temp, ctx);
  }
};

template <typename T, maxwell::unit auto U> struct std::hash<maxwell::quantity<U, T>> {
  std::size_t operator()(const maxwell::quantity<U, T>& q) const noexcept {
    const auto as_SI_base_units = q.to_SI_base_units();
    return std::hash<T>{}(as_SI_base_units) ^
           (std::hash<std::string>{}(as_SI_base_units.get_units().unit_string()) << 1);
  }
};

template <typename T, maxwell::unit auto U>
struct std::numeric_limits<maxwell::quantity<U, T>> : std::numeric_limits<T> {};

#endif