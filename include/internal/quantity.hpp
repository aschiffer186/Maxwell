/**
 * @file quantity.hpp
 * @author Alex Schiffer
 * @brief Definition of \c quantity class template
 * @version 1.0
 * @date 2025-04-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <chrono>           // duration
#include <compare>          // three_way_comparable_width
#include <concepts>         // assignable_from, constructible_from, equality_comparable_with
#include <format>           // formatter
#include <functional>       // hash
#include <initializer_list> // initializer_list
#include <limits>           // numeric_limits
#include <ostream>          // ostream
#include <ratio>            // ratio, ratio_divide
#include <string_view>      // formatter<string_view>
#include <type_traits> // false_type, remove_cvref_t, is_default_constructible_v, is_constructible_v, is_nothrow_assignable_v, is_nothrow_constructible_v, true_type
#include <utility>     // forward, move, swap

#include "config.hpp"
#include "dimension.hpp"
#include "unit.hpp"
#include "utility.hpp"

/// \namespace maxwell Enclosing namespace of all public Maxwell API
namespace maxwell {
/// \cond
template <auto U, typename T = double> class quantity;

namespace _detail {
template <typename> struct is_quantity : std::false_type {};

template <typename T, auto U> struct is_quantity<quantity<U, T>> : std::true_type {};

template <typename T> constexpr bool is_quantity_v = is_quantity<T>::value;

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
/// \endcond

/// \brief Class template \c quantity represents a physical quantity with both a magnitude and units
///
/// An instance of \c quantity represents a physical quantity with both a magnitude and units.
/// Instances of \c quantity can be manipulated like arithmetic types, but only when such operations
/// obey the rules of dimensional analysis. Additionally, instances of \c quantity can only be created
/// from values with units that are convertible to the \c quantities units.
///
/// The magnitude of the quantity is allocated within the storage of the quantity object.
///
/// \note For the purposes of exposition, a quantity contains a single member variable of type \c T called \c mag
/// representing the magnitude of the quantity.
///
/// The units of a \c quantity are part of its type, allowing for checking of dimensional coherence and
/// performance of unit conversions at compile-time.
///
/// \sa unit_type
///
/// \tparam U A value representing the units of the quantity. \c U must be an instantiation of \c unit_type.
/// \tparam T The type of the quantity's magnitue. \c T shall not be a _cv_ qualified type or \c std::in_place_t,
///           and must be \c destructible.
template <auto U, typename T> class quantity {
  static_assert(!std::is_const_v<T>);
  static_assert(!std::is_volatile_v<T>);
  static_assert(!std::same_as<std::remove_cv_t<T>, std::in_place_t>);
  static_assert(std::destructible<T>);
  static_assert(unit<decltype(U)>, "Units of quantity must be an instantiation of unit_type");

public:
  /// The magnitude type of thq quantity
  using magnitude_type = T;
  /// The type of the quantity's units
  using units_type = std::remove_cv_t<decltype(U)>;
  /// The quantity's units
  static constexpr units_type units = U;

  /// \brief Default constructor
  ///
  /// _Constraints_: \c std::is_default_constructible_v<T> is \c true <br>
  /// _Effects_: Constructs a \c quantity whose magnitude is value initialzed. <br>
  /// _Remarks_: This constructor is \c constexpr if and only if the value initialization of \c T
  ///            would satisfy the requirements for a constexpr function. The exception specifier is
  ///            equivalent to \c std::is_nothrow_default_constructible_v<T>
  ///
  /// \throws Any exceptions thrown by the default constructor of \c T
  constexpr quantity()
    requires std::is_default_constructible_v<magnitude_type>
  = default;

  /// \brief Constructor
  ///
  /// _Constraints_: <tt>std::is_constructible_v<T, Up></tt> is \c true and \c Up is not a specialization of
  ///                 \c quantity. <br>
  /// _Effects_: Direct intializes the magnitude of \c *this with \c std::forward<Up>(u). <br>
  /// _Remarks_: This constructor is a constexpr constructor if the selcted constructor of \c T satisfies the
  /// requirements
  ///            for a constexpr function. The exception specification is equivalent to
  ///            <tt>std::is_nothrow_constructible_v<T, Up></tt>. This constructor is explicit if the \c quantity object
  ///            is not unitless of <tt>std::is_convertible_to_v<Up, T></tt> is \c false.
  ///
  /// \tparam Up The type of the value used to initialize the magnitude of \c *this.
  /// \param[in] u The value used to initialize the magnitude of \c *this.
  /// \throws Any exceptions thrown by the constructor of \c magnitude_type.
  template <typename Up = magnitude_type>
    requires std::constructible_from<magnitude_type, Up> && (!_detail::is_quantity_v<std::remove_cvref_t<Up>>)
  constexpr explicit(!unitless_unit<units> || !std::convertible_to<Up, magnitude_type>)
      quantity(Up&& u) noexcept(std::is_nothrow_constructible_v<T, Up>)
      : magnitude_(std::forward<Up>(u)) {}

  /// \brief Constructor
  ///
  /// _Constraints_: <tt>std::is_constructible_v<T, Args...></tt> is \c true. <br>
  /// _Effects_: Direct non-list-initializes the magnitude of \c *this with \c std::forward<Args>(args)... <br>
  /// _Remarks_: This constructor is a constexpr constructor if the selected constructor of \c T is a constexpr
  /// constructor.
  ///
  /// \tparam Args The type of the argument(s) used to initialize the magnitude of \c *this.
  /// \param[in] args The value(s) used to initialize the magnitude of \c *this.
  /// \throws Any exceptions thrown by the selected constructor of \c T.
  template <typename... Args>
    requires std::constructible_from<magnitude_type, Args...>
  constexpr explicit quantity(std::in_place_t, Args&&... args) : magnitude_(std::forward<Args>(args)...) {}

  /// \c Constructor
  ///
  /// _Constraints_: <tt>std::is_constructible_v<T, std::initialize_list<Up>&, Args...></tt> is \c true. <br>
  /// _Effects_: Direct non-list-initializes the magnitude of \c *this with <tt>il, std::forward<Args>(args)...</tt>
  /// <br>
  /// _Remarks_: This constructor is a constexpr constructor if the selected constructor of \c T is a constexpr
  /// constructor.
  ///
  /// \throws Any exceptions thrown by the selected constructor of \c T
  template <typename Up, typename... Args>
    requires std::constructible_from<magnitude_type, std::initializer_list<Up>&, Args...>
  constexpr explicit quantity(std::in_place_t, std::initializer_list<Up> il, Args&&... args)
      : magnitude_(il, std::forward<Args>(args)...) {}

  /// \brief Constructor
  ///
  /// _Constraints_:
  ///  - <tt>std::is_constructible_v<T, Rep></tt> is \c true.
  ///  - The units of \c *this have dimensions of time.
  ///
  /// _Effects_: Initializes the magnitude of \c *this with \c dur.count(), potentially first multiplying the value of
  ///            \c dur.count() by a constant to convert from the units of <tt>std::chrono::duration<Rep, Period></tt>
  ///            to the units of \c *this <br>
  /// _Remarks_: If a unit conversion is required, the conversion factor is calculated at compile-time.
  ///
  /// \tparam Rep An arithmetic type representing the number of ticks of the duration object.
  /// \tparam Period A \c std::ratio representing the tick period of the duration object.
  /// \param[in] dur The \c std::chrono::duration object used to initialize the magnitude of \c *this.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename Rep, typename Period>
    requires std::constructible_from<magnitude_type, Rep> && time_unit<units>
  MAXWELL_CONSTEXPR23 quantity(std::chrono::duration<Rep, Period> dur)
      : magnitude_(dur.cout() * _detail::from_chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>()) {}

  /// \brief Converting constructor
  ///
  /// _Constraints_: <tt>std::is_constructible_v<T, Up></tt> is \c true. <br>
  /// _Effects_: Direct non-initializes the magnitude of \c *this with \c q.get_magnitude(). <br>
  /// _Remarks_: This constructor is explicit if <tt>std::is_convertible_v<const Up&, T></tt> is \c false.
  ///            This constructor is a constexpr constructor if the selected constructor of \c T is a constexpr
  ///            constructor.
  ///
  /// \tparam Up The type of the magnitude of the \c quantity used to initialize \c *this.
  /// \param[in] q The \c quantity used to initialize the magnitude of \c *this.
  /// \throw Any exceptins thrown by the selected constructor of \c T.
  template <typename Up>
    requires std::constructible_from<magnitude_type, Up>
  constexpr explicit(!std::convertible_to<const Up&, magnitude_type>) quantity(const quantity<units, Up>& q)
      : magnitude_(q.get_magnitude()) {}

  /// \brief Converting constructor
  ///
  /// _Constraints_: <tt>std::is_constructible_v<T, Up></tt> is \c true. <br>
  /// _Effects_: Direct non-initializes the magnitude of \c *this with \c std::move(q.get_magnitude()). <br>
  /// _Remarks_: This constructor is explicit if <tt>std::is_convertible_v<const Up&, T></tt> is \c false. This
  ///            constructor is a constexpr constructor if the selected constructor of \c T is a constexpr constructor.
  ///
  /// \tparam Up The type of the magnitude of the \c quantity used to initialize \c *this.
  /// \param[in] q The \c quantity used to initialize the magnitude of \c *this.
  /// \throw Any exceptins thrown by the selected constructor of \c T.
  template <typename Up>
    requires std::constructible_from<magnitude_type, Up>
  constexpr explicit(!std::convertible_to<Up, magnitude_type>) quantity(quantity<units, Up>&& q)
      : magnitude_(std::move(q).get_magnitude()) {}

  /// \brief Converting constructor
  ///
  /// _Constraints_:
  ///   - <tt>std::is_constructible_v<T, Up></tt> is \c true
  ///   - \c unit<decltype(V)> is modeled.
  ///
  /// _Mandates_: <tt>unit_convertible_to<V, units></tt> is modeled. <br>
  /// _Effects_: Direct non-list-initializes the magnitude of \c *this from \c q.get_magnitude(), first performing any
  ///            arithmetic operations necessary to convert from the units of \c q to the units of \c *this. <br>
  /// _Remarks_: The values needed to convert from the units of \c q to the units of \c *this are calculated at
  ///            compile-time. This constructor is explicit if <tt>std::is_convertible_v<const Up&, T></tt> is false.
  ///            This constructor is a constexpr constructor if the selected constructor of \c T is a constexpr
  ///            constructor and arthmetic operations on \c T are usable in constant expressions.
  ///
  /// \tparam V The units of the quantity used to initialize the magnitude of \c *this.
  /// \tparam Up The type of the magnitude of the quantity used to initialize the magnitude of \c *this.
  /// \param[in] q The \c quantity used to initialize the magnitude of \c *this.
  /// \throw Any exceptions thrown by the selected constructor of \c T or arthmetic operations on \c T.
  template <auto V, typename Up>
    requires std::constructible_from<magnitude_type, Up> && unit<decltype(V)>
  constexpr explicit(!std::is_convertible_v<const Up&, T>) quantity(const quantity<V, Up>& q)
      : magnitude_(q.get_magnitude() * conversion_factor(V, units) + conversion_offset(V, units)) {
    static_assert(unit_convertible_to<V, U>,
                  "Attempting to construct a quantity from another quantity with incompatible units");
  }

  /// \brief Converting constructor
  ///
  /// _Constraints_:
  ///   - <tt>std::is_constructible_v<T, Up></tt> is \c true.
  ///   - \c unit<decltype(V)> is modeled.
  ///
  /// _Mandates_: <tt>unit_convertible_to<V, units></tt> is modeled <br>
  /// _Effects_: Direct non-list-initializes the magnitude of \c *this from \c std::move(q).get_magnitude(), first
  ///            performing any arithmetic operations necessary to convert from the units of \c q to the units of \c
  ///            *this. <br>
  /// _Remarks_: The values needed to convert from the units of \c q to the units of \c *this are calculated at
  ///            compile-time. This constructor is explicit if <tt>std::is_convertible_v<Up, T></tt> is false. This
  ///            constructor is a constexpr constructor if the selected constructor of \c T is a constexpr constructor
  ///            and arthmetic operations on \c T are usable in constant expressions.
  ///
  /// \tparam V The units of the quantity used to initialize the magnitude of \c *this.
  /// \tparam Up The type of the magnitude of the quantity used to initialize the magnitude of \c *this.
  /// \param[in] q The \c quantity used to initialize the magnitude of \c *this.
  /// \throw Any exceptions thrown by the selected constructor of \c T or arithmetic operations on \c T.
  template <typename Up, auto V>
    requires std::constructible_from<magnitude_type, Up>
  constexpr explicit(!std::is_convertible_v<Up, T>) quantity(quantity<V, Up>&& q)
      : magnitude_(std::move(q).get_magnitude() * conversion_factor(V, units) + conversion_offset(V, units)) {
    static_assert(unit_convertible_to<V, U>,
                  "Attempting to construct a quantity from another quantity with incompatible units");
  }

  /// \brief Assignment operator
  ///
  /// _Constraints_:
  ///    - <tt>std::is_constructible_v<T, Up></tt> is \c true.
  ///    - <tt>std::is_swappable_v<T, Up></tt> is \c true.
  ///    - \c unit<decltype(V)> is modeled.
  ///
  /// _Mandates_: <tt>unit_convertible_to<V, units></tt> is modeled. <br>
  /// _Effects_: Assigns the magnitude of \c other to the magnitude of \c *this, first
  ///            performing any arithmetic operations necessary to convert from the units of \c q to the units of \c
  ///            *this. <br>
  /// _Remarks_: The values needed to convert from the units of \c q to the units of \c *this are calculated at
  ///            compile-time.
  /// \tparam Up The type of the magnitude of the quantity whose magnitude will be assigned to \c *this.
  /// \tparam V The units of the quantity whose magnitude will be assigned to \c *this.
  /// \param[in] other The quantity whose magnitude will be assigned to \c *this.
  /// \return \c *this
  template <typename Up, auto V>
    requires std::constructible_from<magnitude_type, Up> && unit<decltype(V)>
  constexpr quantity& operator=(quantity other) {
    static_assert(unit_convertible_to<V, U>, "Attempting to assign quantities with incompatible units");
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
  /// Units)*dur.count()/tt>
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
  MAXWELL_CONSTEXPR23 quantity& operator=(std::chrono::duration<Rep, Period> dur) {
    using std::swap;
    quantity temp(std::move(dur));
    swap(temp.magnitude_, magnitude_);
    return *this;
  }

  /// \brief Assignment operator
  ///
  /// _Constraints_:
  ///   - <tt>std::is_assignable_v<Up, T></tt> is \c true.
  ///   - \c *this is unitless
  ///
  /// _Effcts_: Assigns \c std::forward<Up>(other) to the magnitude of \c *this.
  ///
  /// \tparam Up The type of the value to assign to the magnitude of \c *this.
  /// \param[in] other The value to assign to the magnitude of \c *this.
  /// \return *this
  template <typename Up = magnitude_type>
    requires unitless_unit<units> && std::assignable_from<magnitude_type, Up>
  constexpr quantity& operator=(Up&& other) noexcept(std::is_nothrow_assignable_v<magnitude_type&, Up>) {
    magnitude_ = std::forward<Up>(other);
    return *this;
  }

  /// \brief Returns the units of \c *this
  ///
  /// _Effects_: Returns the units of \c *this. <br>
  /// _Remarks_: This function is a constexpr function.
  ///
  /// \return The units of \c *this
  constexpr units_type get_units() const noexcept { return units; }

  /// \brief Returns the magnitude of the \c *this
  ///
  /// _Effects_: Returns \c mag. <br>
  /// _Remarks_: This function is a constxpr function.
  ///
  /// \return A \c const reference to the magnitude of \c *this.
  constexpr const magnitude_type& get_magnitude() const& noexcept { return magnitude_; }

  /// \brief Returns the magnitude of the \c *this
  ///
  /// _Effects_: Returns \c std::move(mag). <br>
  /// _Remarks_: This function is a constexpr function.
  ///
  /// \return An rvalue-reference to the magnitude of \c *this.
  constexpr magnitude_type&& get_magnitude() && noexcept { return std::move(magnitude_); }

  /// \brief Returns the magnitude of the \c *this
  ///
  /// _Effects_: Returns \c std::move(mag). <br>
  /// _Remarks_: This function is a constexpr function.
  ///
  /// \return A \c const rvalue reference to the magnitude of \c *this.
  constexpr const magnitude_type&& get_magnitude() const&& noexcept { return std::move(magnitude_); }

  /// \brief Conversion operator to underlying magnitude type
  ///
  /// _Effects_: Returns \c mag. <br>
  /// _Remarks_: The conversion operator is explicit if the \c quantity object is not unitless. This function
  ///            is a constexpr function.
  ///
  /// \note It is not recommended to use this function unless the \c quantity is unitless.
  /// \return The magnitude of \c *this.
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

  /// \brief Returns a new \c quantity with the same dimensions of \c
  /// *this but in SI base units
  ///
  /// Returns a new \c quantity with the same dimension of \c *this, but
  /// in SI base units. The magnitude of the returned quantity is equivalent to
  /// \c this->magnitude() converted to SI base units
  ///
  /// \return A new \c quantity with the same dimension of \c *this in SI
  /// base units.
  constexpr auto to_SI_base_units() const { return quantity<units.to_SI_base_units(), magnitude_type>(*this); }

  // --- Quantity manipulation ---
  /// \brief Addition operator
  ///
  /// Adds the magnitude of the specified quantity to the magnitude of
  /// \c *this
  ///
  /// \param other The quantity to add to \c *this
  /// \return A reference to the modified value \c *this
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

  /// \brief Subtrction operator
  ///
  /// Subtraction the magnitude of the specified quantity from the magnitude of
  /// \c *this
  ///
  /// \param other The quantity to subtract from this \c *this
  /// \return A reference to the modified value \c *this
  constexpr quantity& operator-=(const quantity& other)
    requires subtractable<magnitude_type>
  {
    if constexpr (requires(magnitude_type value) { value -= value; }) {
      magnitude_ -= other.get_magnitude();
    } else {
      magnitude_ = magnitude_ - other.magnitude_;
    }
    return *this;
  }

  /// \brief Converting addition operator
  ///
  /// Converts the specified quantity to the units of \c *this then adds the converted
  /// magnitude to \c *this
  ///
  /// \param other The quantity to add to \c *this
  /// \return A reference to the modified value of \c *this
  template <typename Up, unit auto V>
    requires addable_with<T, Up>
  constexpr quantity& operator+=(const quantity<V, Up>& other) {
    static_assert(unit_convertible_to<V, U>, "Attempting to add quantities with incompatible units");
    return *this += quantity(other);
  }

  /// \brief Converting subtraction operator
  ///
  /// Converts the specified quantity to the units of \c *this then subtracts the converted
  /// magnitude to \c *this
  ///
  /// \param other The quantity to subtract \c *this
  /// \return A reference to the modified value of \c *this
  template <typename Up, unit auto V>
    requires subtractable_with<T, Up>
  constexpr quantity& operator-=(const quantity<V, Up>& other) {
    static_assert(unit_convertible_to<V, U>, "Attempting to subtract quantities with incompatible units");
    return *this -= quantity(other);
  }

  /// \brief Multiplication operator
  ///
  /// Multiplies the quantity by the specified scalar value
  ///
  /// \param scalar the scalar value to multiply \c *this by
  /// \return A reference to the modified value of \c *this
  constexpr quantity& operator*=(const magnitude_type& scalar)
    requires multiply<magnitude_type>
  {
    if constexpr (requires(magnitude_type value) { value *= value; }) {
      magnitude_ *= scalar;
    } else {
      magnitude_ = magnitude_ * scalar;
    }
    return *this;
  }

  /// \brief Division operator
  ///
  /// Divides the quantity by the specified scalar value
  ///
  /// \param scalar the scalar value to divide \c *this by
  /// \return A reference to the modified value of \c *this
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

  /// \brief Pre-increment operator
  ///
  /// Incerements the magnitude of \c *this by one
  ///
  /// \return A reference to the modified value \c *this
  constexpr quantity& operator++() {
    ++magnitude_;
    return *this;
  }

  /// \brief Pre-decrement operator
  ///
  /// Decrements the magnitude of \c *this by one
  ///
  /// \return A reference to the modified value \c *this
  constexpr quantity& operator--() {
    --magnitude_;
    return *this;
  }

  /// \brief Post-increment operator
  ///
  /// Increments the magnitude of \c *this by one and
  /// returns a copy of the \c quantity prior to the increment
  ///
  /// \return A copy of the \c quantity prior to the increment
  constexpr quantity& operator++(int) {
    quantity temp{*this};
    ++magnitude_;
    return temp;
  }

  /// \brief Post-decrement operator
  ///
  /// Decrements the magnitude of \c *this by one and
  /// returns a copy of the \c quantity prior to the decremet
  ///
  /// \return A copy of the \c quantity prior to the decrement
  constexpr quantity& operator--(int) {
    quantity temp{*this};
    --magnitude_;
    return temp;
  }

private:
  magnitude_type magnitude_{};
};

/// \brief Equality operator
///
/// Compares the magnitudes of two quantities for equality. Two
/// quanities are if their magnitudes, after conversion to the quantity's SI base units,
/// are equal.
///
/// Warning: floating-point equality will be performed if the underlying magnitude's are
///          floating-point types.
///
/// \pre <tt>std::equality_comparable_with<S1, S2><\tt>
///
/// \tparam U1 The units of the left hand side quantity
/// \tparam S1 The magnitude type of the left hand side quantity
/// \tparam U2 The units of the right hand side quantity
/// \tparam S2 The magnitude type of the right hand side quantity
///
/// \param lhs The left hand side of the comparison
/// \param rhs The right hand side of the comparison
/// \return \c true if the two quantities are equal
template <unit auto U1, typename S1, unit auto U2, typename S2>
  requires std::equality_comparable_with<S1, S2>
constexpr bool operator==(const quantity<U1, S1>& lhs, const quantity<U2, S2>& rhs) {
  static_assert(unit_convertible_to<U1, U2>, "Attempting to compare quantities with incompatible units");
  return lhs.to_SI_base_units().get_magnitude() == rhs.to_SI_base_units().get_magnitude();
}

/// \brief Three-way comparison operator
///
/// Performs three-way comparison on two quantities. Prior to the conversion, both
/// quantities are converted to SI base units to ensure equivalent quantities with
/// different magnitudes are compared correctly
///
/// \pre <tt>std::three_way_comparable_with<S1, S2><\tt>
///
/// \tparam U1 The units of the left hand side quantity
/// \tparam S1 The magnitude type of the left hand side quantity
/// \tparam U2 The units of the right hand side quantity
/// \tparam S2 The magnitude type of the right hand side quantity
///
/// \param lhs The left hand side of the comparison
/// \param rhs The right hand side of the comparison
/// \return the three way comparison of the quantities
template <unit auto U1, typename S1, unit auto U2, typename S2>
  requires std::three_way_comparable_with<S1, S2>
constexpr std::compare_three_way_result_t<S1, S2> operator<=>(const quantity<U1, S1>& lhs,
                                                              const quantity<U2, S2>& rhs) {
  static_assert(unit_convertible_to<U1, U2>,
                "Attempting to perform three-way-comparison on quantities with incompatible units");
  return lhs.to_SI_base_units().get_magnitude() <=> rhs.to_SI_base_units().get_magnitude();
}

/// \brief Multiplication operator
///
/// Multiplies two quantities together. The resulting quantity has units of
/// S1 * S2. The type of the magnitude of the resulting quantity is determined
/// from the type of S1 * S2. It is not recommended to use \c auto for the result
/// of this type as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation).
///
/// In addition, multiplication may not be commutative or associative if the multiplication
/// of \c S1 and \c S2 is not commutative or associative. For example, if \c S1 and \c S2 are
/// matrices, the multiplication may not be commutative or associative.
///
/// \pre <tt>std::multiply_with<S1, S2><\tt>
/// \tparam S1 The type of the magnitude of the left hand side of the multiplication
/// \tparam U1 The units of the left hand side of the multiplication
/// \tparam S2 The type of the magnitude of the right hand side of the multiplication
/// \tparam U2 The units of the right hand side of the multiplication
/// \param lhs The left hand side of the multiplication
/// \param rhs The right hand side of the multiplication
/// \return The product of the two quantities
template <typename S1, unit auto U1, typename S2, unit auto U2>
constexpr auto operator*(const quantity<U1, S1>& lhs, const quantity<U2, S2>& rhs)
  requires multiply_with<S1, S2>
{
  const unit auto return_units = lhs.get_units() * rhs.get_units();

  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() * rhs.get_magnitude())>;
  return quantity<return_units, return_scalar_type>{lhs.get_magnitude() * rhs.get_magnitude()};
}

/// \brief Divides operator
///
/// Divides two quantities together. The resulting quantity has units of
/// S1 * S2. The type of the magnitude of the resulting quantity is determined
/// from the type of S1 / S2. It is not recommended to use \c auto for the result
/// of this type as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation).
///
/// Note: if both \c S1 and \c S2 are integer types, integer division is performed.
///
/// \pre <tt>std::divide_with<S1, S2><\tt>
///
/// \tparam S1 The type of the magnitude of the left hand side of the division
/// \tparam U1 The units of the left hand side of the division
/// \tparam S2 The type of the magnitude of the right hand side of the division
/// \tparam U2 The units of the right hand side of the division
/// \param lhs The left hand side of the division
/// \param rhs The right hand side of the division
/// \return The quotient of the two quantities
template <typename S1, unit auto U1, typename S2, unit auto U2>
constexpr auto operator/(const quantity<U1, S1>& lhs, const quantity<U2, S2>& rhs)
  requires divide_with<S1, S2>
{
  const unit auto return_units = lhs.get_units() / rhs.get_units();

  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() / rhs.get_magnitude())>;
  return quantity<return_units, return_scalar_type>{lhs.get_magnitude() / rhs.get_magnitude()};
}

/// \brief Multiplication operator
///
/// Multiplies a quantity by a scalar value. The resulting quantity has the
/// same units as the original quantity. The type of the magnitude of the resulting
/// quantity is determined from the type of S1 * S2. It is not recommended to use \c auto
/// for the result of this type as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation).
///
/// In addition, multiplication may not be commutative or associative if the multiplication
/// of \c M1 and \c M2 is not commutative or associative. For example, if \c M1 and \c m2 are
/// matrices, the multiplication may not be commutative or associative.
///
/// \pre <tt>std::multiply_with<S1, S2><\tt>
/// \pre \c M2 is not a specialization of \c quantity or \c unit_type
///
/// \tparam U1 The units of the left hand side of the multiplication
/// \tparam M1 The type of the magnitude of the left hand side of the multiplication
/// \tparam M2 The type of the magnitude of the right hand side of the multiplication
/// \param lhs The left hand side of the multiplication
/// \param rhs The right hand side of the multiplication
/// \return The product of the two quantities
template <unit auto U1, typename M1, typename M2>
  requires multiply_with<M1, M2> && (!_detail::is_quantity_v<std::remove_cvref_t<M2>>) && (!unit<M2>)
constexpr auto operator*(const quantity<U1, M1>& lhs, const M2& rhs) {
  // NOTE: Multiplication is NOT guaranteed to be commutative for all possible magnitude_types (e.g. matrices).
  //       DO NOT re-write this in terms of another multiplication operator
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() * rhs)>;
  return quantity<U1, return_scalar_type>{lhs.get_magnitude() * rhs};
}

/// \brief Multiplication operator
///
/// Multiplies a quantity by a scalar value. The resulting quantity has the
/// same units as the original quantity. The type of the magnitude of the resulting
/// quantity is determined from the type of S1 * S2. It is not recommended to use \c auto
/// for the result of this type as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation).
///
/// In addition, multiplication may not be commutative or associative if the multiplication
/// of \c M1 and \c M2 is not commutative or associative. For example, if \c M1 and \c m2 are
/// matrices, the multiplication may not be commutative or associative.
///
/// \pre <tt>std::multiply_with<S1, S2><\tt>
/// \pre \c M2 is not a specialization of \c quantity or \c unit_type
///
/// \tparam U1 The units of the right hand side of the multiplication
/// \tparam M1 The type of the magnitude of the right hand side of the multiplication
/// \tparam M2 The type of the magnitude of the left hand side of the multiplication
/// \param lhs The left hand side of the multiplication
/// \param rhs The right hand side of the multiplication
/// \return The product of the two quantities
template <unit auto U1, typename M1, typename M2>
  requires multiply_with<M1, M2> && (!unitless_unit<U1>) && (!_detail::is_quantity_v<std::remove_cvref_t<M2>>) &&
           (!unit<M2>)
constexpr auto operator*(const M2& lhs, const quantity<U1, M1>& rhs) {
  // NOTE: Multiplication is NOT guaranteed to be commutative for all possible magnitude_types (e.g. matrices)
  //       DO NOT re-write this in terms of another multiplication operator
  using return_scalar_type = std::remove_cvref_t<decltype(lhs * rhs.get_magnitude())>;
  return quantity<U1, return_scalar_type>{lhs * rhs.get_magnitude()};
}

/// \brief Division operator
///
/// divides a quantity by a scalar value. The resulting quantity has the
/// same units as the original quantity. The type of the magnitude of the resulting
/// quantity is determined from the type of <tt>S1 / S2</tt> and may not be the same as \c M1 or \c M2.
/// It is not recommended to use \c auto
/// for the result of this type as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation).
///
/// Note, if both \c M1 and \c M2 are integer types, integer division is performed.
///
/// \pre <tt>std::divide_with<S1, S2><\tt>
/// \pre \c M2 is not a specialization of \c quantity or \c unit_type
///
/// \tparam U1 The units of the right hand side of the division
/// \tparam M1 The type of the magnitude of the right hand side of the division
/// \tparam M2 The type of the magnitude of the left hand side of the division
/// \param lhs The left hand side of the division
/// \param rhs The right hand side of the division
/// \return The quotient of the two quantities
template <unit auto U1, typename M1, typename M2>
  requires divide_with<M1, M2> && (!unitless_unit<U1>) && (!_detail::is_quantity_v<std::remove_cvref_t<M2>>)
constexpr auto operator/(const quantity<U1, M1>& lhs, const M2& rhs) {
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() / rhs)>;
  return quantity<U1, return_scalar_type>(lhs.get_magnitude() / rhs);
}

/// \brief Addition operator
///
/// First converts both quantities to SI base units then computes their sum.
/// The magnitude type of the resulting quantity is determined from the type of
/// \c M1 and \c M2 and may not be the same as \c M1 or \c M2.
/// It is not recommended to use \c auto as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation or the units are not as expected).
///
/// \pre <tt>std::addable_with<S1, S2><\tt>
/// \pre <tt>std::convertible_to<S1, S2><\tt>
///
/// \tparam S1 The type of the magnitude of the left hand side of the addition
/// \tparam U1 The units of the left hand side of the addition
/// \tparam S2 The type of the magnitude of the right hand side of the addition
/// \tparam U2 The units of the right hand side of the addition
/// \param lhs The left hand side of the addition
/// \param rhs The right hand side of the addition
/// \return The sum of the two quantities
template <typename M1, unit auto U1, typename M2, unit auto U2>
  requires addable_with<M1, M2>
constexpr auto operator+(quantity<U1, M1> lhs, const quantity<U2, M2>& rhs) {
  static_assert(unit_convertible_to<U1, U2>, "Attempting to add quantities with incompatible units");
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() + rhs.get_magnitude())>;

  return quantity<U1.to_SI_base_units(), return_scalar_type>(lhs.to_SI_base_units().get_magnitude() +
                                                             rhs.to_SI_base_units().get_magnitude());
}

/// \brief Subtraction operator
///
/// First converts both quantities to SI base units then computes their difference.
/// The magnitude type of the resulting quantity is determined from the type of
/// <tt>M1 - M2</tt> and may not be the same as \c M1 or \c M2.
/// It is not recommended to use \c auto as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation or the units are not as expected).
///
/// \pre <tt>std::subtractable_with<S1, S2><\tt>
/// \pre <tt>std::convertible_to<S1, S2><\tt>
///
/// \tparam S1 The type of the magnitude of the left hand side of the subtraction
/// \tparam U1 The units of the left hand side of the subtraction
/// \tparam S2 The type of the magnitude of the right hand side of the subtraction
/// \tparam U2 The units of the right hand side of the subtraction
/// \param lhs The left hand side of the subtraction
/// \param rhs The right hand side of the subtraction
/// \return The difference the two quantities
template <typename M1, unit auto U1, typename M2, unit auto U2>
  requires subtractable_with<M1, M2>
constexpr auto operator-(quantity<U1, M1> lhs, const quantity<U2, M2>& rhs) {
  static_assert(unit_convertible_to<U1, U2>, "Attempting to subtract quantities with incompatible units");
  using return_scalar_type = std::remove_cvref_t<decltype(lhs.get_magnitude() - rhs.get_magnitude())>;

  return quantity<U1.to_SI_base_units(), return_scalar_type>(lhs.to_SI_base_units().get_magnitude() -
                                                             rhs.to_SI_base_units().get_magnitude());
}

/// \brief Negation operator
///
/// Negates the magnitude of the specified quantity. The returned quantity has the
/// same units as the original quantity; however, the magnitude type may be different.
/// It is not recommended to use \c auto
/// for the result of this type as it may produce unexpected results (e.g. if expression templates are
/// used for lazy evaluation).
///
/// \tparam M The type of the magnitude of the quantity
/// \tparam U The units of the quantity
/// \param x The quantity to negate
/// \return The negated quantity
template <typename M, unit auto U> constexpr auto operator-(const quantity<U, M>& x) {
  using scalar_return_type = std::remove_cvref_t<decltype(-x.get_magnitude())>;
  return quantity<U, scalar_return_type>(-x.get_magnitude());
}

/// \brief Concept for quantities representing amount
///
/// Concept modeling a quantity that represents an amount.
/// This concept is modeled if the quantity is convertible to mole.
///
/// \tparam T The type of the quantity
template <typename T>
concept amount = amount_unit<T::units>;

/// \brief Concept for quantities representing current
///
/// Concept modeling a quantity that represents a current.
/// This concept is modeled if the quantity is convertible to ampere.
///
/// \tparam T The type of the quantity
template <typename T>
concept current = current_unit<T::units>;

/// \brief Concept for quantities representing length
///
/// Concept modeling a quantity that represents a length.
/// This concept is modeled if the quantity is convertible to meter.
///
/// \tparam T The type of the quantity
template <typename T>
concept length = length_unit<T::units>;

/// \brief Concept for quantities representing luminosity
///
/// Concept modeling a quantity that represents a luminosity.
/// This concept is modeled if the quantity is convertible to candela.
///
/// \tparam T The type of the quantity
template <typename T>
concept luminosity = luminosity_unit<T::units>;

/// \brief Concept for quantities representing mass
///
/// Concept modeling a quantity that represents an mass.
/// This concept is modeled if the quantity is convertible to kilogram.
///
/// \tparam T The type of the quantity
template <typename T>
concept mass = mass_unit<T::units>;

/// \brief Concept for quantities representing temperature
///
/// Concept modeling a quantity that represents a temperature
/// This concept is modeled if the quantity is convertible to kelvin.
///
/// \tparam T The type of the quantity
template <typename T>
concept temperature = temperature_unit<T::units>;

/// \brief Concept for quantities representing time
///
/// Concept modeling a quantity that represents a time
/// This concept is modeled if the quantity is convertible to mole.
///
/// \tparam T The type of the quantity
template <typename T>
concept time = time_unit<T::units>;

/// \brief Concept for scalar quantities
///
/// Concept modeling a quantity that has not units.
///
/// \tparam T The type of the quantity
template <typename T>
concept scalar = unitless_unit<T::units>;

/// \brief Convenience type alias for a quantity whose \c magnitude_type is \c int
///
/// \tparam U the units of the quantity
template <unit auto U> using int_quantity = quantity<U, int>;
// Quantity printing

template <typename T, unit auto U> std::ostream& operator<<(std::ostream& os, const quantity<U, T>& q) {
  os << std::format("{}", q);
  return os;
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
    temp.append(" ").append(q.get_units().unit_string());
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
  requires(!std::numeric_limits<T>::is_specialized)
struct std::numeric_limits<maxwell::quantity<U, T>> : std::numeric_limits<T> {};

template <typename T, auto U>
  requires maxwell::unit<decltype(U)> && std::numeric_limits<T>::is_specialized
struct std::numeric_limits<maxwell::quantity<U, T>> {
private:
  using b_limits = std::numeric_limits<T>;

public:
  static constexpr bool is_specialized = true;
  static constexpr bool is_signed = b_limits::is_signed;
  static constexpr bool is_integer = b_limits::is_integer;
  static constexpr bool is_exact = b_limits::is_exact;
  static constexpr bool has_infinity = b_limits::has_infinity;
  static constexpr bool has_quiet_NaN = b_limits::has_quiet_NaN;
  static constexpr bool has_signaling_NaN = b_limits::has_signaling_NaN;
  static constexpr bool has_denorm = b_limits::has_denorm;
  static constexpr bool has_denorm_loss = b_limits::has_denorm_loss;
  static constexpr float_round_style round_style = b_limits::round_style;
  static constexpr bool is_iec559 = b_limits::is_iec559;
  static constexpr bool is_bounded = b_limits::is_bounded;
  static constexpr bool is_modulo = b_limits::is_modulo;
  static constexpr int digits = b_limits::digits;
  static constexpr int digits10 = b_limits::digits10;
  static constexpr int max_digits10 = b_limits::max_digits10;
  static constexpr int radix = b_limits::radix;
  static constexpr int min_exponent = b_limits::min_exponent;
  static constexpr int min_exponent_10 = b_limits::min_exponent10;
  static constexpr int max_eponent = b_limits::max_exponent;
  static constexpr int max_exponent_10 = b_limits::max_exponent10;
  static constexpr bool traps = b_limits::traps;
  static constexpr bool tinyness_before = b_limits::tinyness_before;

  static constexpr maxwell::quantity<U, T> min() noexcept { return maxwell::quantity<U, T>(b_limits::min()); }

  static constexpr maxwell::quantity<U, T> lowest() noexcept { return maxwell::quantity<U, T>(b_limits::lowest()); }

  static constexpr maxwell::quantity<U, T> max() noexcept { return maxwell::quantity<U, T>(b_limits::max()); }

  static constexpr maxwell::quantity<U, T> epsilon() noexcept { return maxwell::quantity<U, T>(b_limits::epsilon()); }

  static constexpr maxwell::quantity<U, T> round_error() noexcept {
    return maxwell::quantity<U, T>(b_limits::round_error());
  }

  static constexpr maxwell::quantity<U, T> infinity() noexcept { return maxwell::quantity<U, T>(b_limits::infinity()); }

  static constexpr maxwell::quantity<U, T> quiet_NaN() noexcept {
    return maxwell::quantity<U, T>(b_limits::quiet_NaN());
  }

  static constexpr maxwell::quantity<U, T> signaling_NaN() noexcept {
    return maxwell::quantity<U, T>(b_limits::signaling_NaN());
  }

  static constexpr maxwell::quantity<U, T> denorm_min() noexcept {
    return maxwell::quantity<U, T>(b_limits::denorm_min());
  }
};

#endif