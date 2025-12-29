#ifndef QUANTITY_VALUE_HPP
#error "Do not include this file directly; include quantity_value.hpp instead"
#endif

#include <chrono>           // duration
#include <compare>          // spaceship operator
#include <concepts>         // constructible_from, convertible_to, swappable
#include <format>           // formatter
#include <initializer_list> // initializer_list
#include <iterator>         // back_inserter
#include <ostream>          // ostream
#include <string_view>      // string_view
#include <type_traits> // false_type, is_assignable_v, remove_cvref_t, true_type
#include <utility>     // forward, in_place_t, move

#include "core/quantity.hpp"
#include "core/unit.hpp"
#include "quantity_value_holder_fwd.hpp"
#include "utility/compile_time_math.hpp"
#include "utility/config.hpp"
#include "utility/type_traits.hpp"

template <auto U, auto Q, typename T>
struct std::formatter<maxwell::quantity_value<U, Q, T>>
    : formatter<string_view> {
  auto constexpr parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto constexpr format(const maxwell::quantity_value<U, Q, T>& q,
                        format_context& ctx) const {
    string str;
    format_to(back_inserter(str), "{} {}", q.get_value_unsafe(), q.get_units());
    return formatter<string_view>::format(str, ctx);
  }
};

namespace maxwell {
/// \cond
namespace _detail {
/// \endcond

/// \brief Provides operator overloads for \c quantity_value instances.
///
/// \tparam Derived The \c quantity_value type to provide arithmetic operators
/// for.
template <quantity_value_like Derived> class _quantity_value_operators {
  /// \brief Negation operator
  ///
  /// Negates the value of a \c quantity_value instance.
  ///
  /// \param q The quantity value instance to negate.
  /// \return The negated value of the `quantity_value` instance.
  friend constexpr auto operator-(const Derived& q) -> Derived {
    return std::remove_cvref_t<decltype(q)>(-q.get_value_unsafe());
  }

  /// \brief Pre-increment operator
  ///
  /// Increments the numerical value of a \c quantity_value instance and returns
  /// a reference to the modified \c quantity_value.
  ///
  /// \param q The quantity value instance to increment.
  /// \return A reference to the modified \c quantity_value instance.
  friend constexpr auto operator++(Derived& q) -> Derived& {
    ++q.value_;
    return q;
  }

  /// \brief Post-increment operator
  ///
  /// Increments the numerical value of a \c quantiity_value instance and
  /// returns a copy of the \c quantity_value before it was modified.
  ///
  /// \param q The quantity value instance to increment.
  /// \return A reference to the modified \c quantity_value instance.
  friend constexpr auto operator++(Derived& q, int) -> Derived {
    auto temp{q};
    ++q;
    return temp;
  }

  /// \brief Pre-decrement operator
  ///
  /// Decrement the numerical value of a \c quantity_value instance and returns
  /// a reference to the modified \c quantity_value.
  ///
  /// \param q The quantity value instance to decrement.
  /// \return A reference to the modified \c quantity_value instance.
  friend constexpr auto operator--(Derived& q) -> Derived& {
    --q.value_;
    return q;
  }

  /// \brief Post-decrement operator
  ///
  /// Decrements the numerical value of a \c quantiity_value instance and
  /// returns a copy of the \c quantity_value before it was modified.
  ///
  /// \param q The quantity value instance to decrement.
  /// \return A reference to the modified \c quantity_value instance.
  friend constexpr auto operator--(Derived& q, int) -> Derived {
    auto temp{q};
    --q;
    return temp;
  }

  /// \brief Addition assignment operator
  ///
  /// Adds the numerical value of another \c quantity_value instance to this
  /// instance and returns a reference to the modified \c quantity_value.
  /// Automatically converts the units of the other instance to the units of
  /// this instance if necessary.
  ///
  /// The program is ill-formed if the quantities of the two instances are not
  /// the same or if the units of the two instances have different reference
  /// points.
  ///
  /// \tparam U2 The units of the other \c quantity_value instance.
  /// \tparam T2 The type of the other \c quantity_value instance.
  /// \param lhs The \c quantity_value instance to modify.
  /// \param rhs The other \c quantity_value instance to add.
  /// \return A reference to the modified \c quantity_value instance.
  template <auto U2, auto Q2, typename T2>
    requires requires(typename Derived::value_type lhs, T2 rhs) { lhs += rhs; }
  friend constexpr auto operator+=(Derived& lhs,
                                   const quantity_value<U2, Q2, T2>& rhs)
      -> Derived& {
    static_assert(unit_addable_with<Derived::units, U2>,
                  "Cannot add quantities of different kinds or quantities "
                  "whose units have different reference points.");
    if constexpr (U2.multiplier == Derived::units.multiplier) {
      lhs.value_ += rhs.get_value_unsafe();
    } else {
      lhs.value_ += Derived(rhs).get_value_unsafe();
    }
    return lhs;
  }

  /// \brief Addition assignment operator
  ///
  /// Adds the numeric value of a \c quantity_holder instance to the numerical
  /// value of \c lhs, converting units if necessary. Returns a reference to the
  /// modified \c quantity_value. The program is ill-formed if the quantities of
  /// the two values being added are not convertible to each other.
  ///
  /// \tparam Q2 The quantity of the \c quantity_holder instance.
  /// \tparam T2 The type of the numerical value of the \c quantity_holder
  /// instance.
  /// \param lhs The left-hand side of the addition.
  /// \param rhs The right-hand side of the addition.
  /// \return A reference to the modified \c quantity_value instance.
  /// \throw incompatible_quantity_holder if the the units of the quantities
  /// being added have difference reference points.
  template <auto Q2, typename T2>
  friend constexpr auto operator+=(Derived& lhs,
                                   const quantity_holder<Q2, T2>& rhs)
      -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity> &&
                      quantity_convertible_to<Derived::quantity, Q2>,
                  "Cannot add quantities of different kinds");
    if (Derived::units.reference != rhs.get_reference()) [[unlikely]] {
      throw incompatible_quantity_holder(
          "Cannot add quantities whose units have different reference "
          "points.");
    }
    if (rhs.get_multiplier() == Derived::units.multiplier) {
      lhs.value_ += rhs.get_value_unsafe();
    } else {
      lhs.value_ += Derived(rhs).get_value_unsafe();
    }
    return lhs;
  }

  /// \brief Subtraction assignment operator
  ///
  /// Subtracts the numerical value of another \c quantity_value instance from
  /// this instance and returns a reference to the modified \c quantity_value.
  /// Automatically converts the units of the other instance to the units of
  /// this instance if necessary.
  ///
  /// The program is ill-formed if the quantities of the two instances are not
  /// the same or if the units of the two instances have different reference
  /// points.
  ///
  /// \tparam U2 The units of the other \c quantity_value instance.
  /// \tparam T2 The type of the other \c quantity_value instance.
  /// \param lhs The \c quantity_value instance to modify.
  /// \param rhs The other \c quantity_value instance to subtract.
  /// \return A reference to the modified \c quantity_value instance.
  template <auto U2, auto Q2, typename T2>
    requires requires(typename Derived::value_type lhs, T2 rhs) { lhs -= rhs; }
  friend constexpr auto operator-=(Derived& lhs,
                                   const quantity_value<U2, Q2, T2>& rhs)
      -> Derived& {
    static_assert(unit_subtractable_from<Derived::units, U2>,
                  "Cannot subtract quantities of different kinds or quantities "
                  "whose units have different reference points.");
    if constexpr (U2.multiplier == Derived::units.multiplier) {
      lhs.value_ -= rhs.get_value_unsafe();
    } else {
      lhs.value_ -= Derived(rhs).get_value_unsafe();
    }
    return lhs;
  }

  /// \brief Subtraction assignment operator
  ///
  /// Subtracts the numeric value of a \c quantity_holder instance from the
  /// numerical value of \c lhs, converting units if necessary. Returns a
  /// reference to the modified \c quantity_value. The program is ill-formed if
  /// the quantities of the two values being added are not convertible to each
  /// other.
  ///
  /// \tparam Q2 The quantity of the \c quantity_holder instance.
  /// \tparam T2 The type of the numerical value of the \c quantity_holder
  /// instance.
  /// \param lhs The left-hand side of the subtraction.
  /// \param rhs The right-hand side of the subtraction.
  /// \return A reference to the modified \c quantity_value instance.
  /// \throw incompatible_quantity_holder if the the units of the quantities
  /// being added have difference reference points.
  template <auto Q2, typename T2>
  friend constexpr auto operator-=(Derived& lhs,
                                   const quantity_holder<Q2, T2>& rhs)
      -> Derived& {
    static_assert(quantity_convertible_to<Q2, Derived::quantity> &&
                      quantity_convertible_to<Derived::quantity, Q2>,
                  "Cannot subtract quantities of different kinds");
    if (Derived::units.reference != rhs.get_reference()) [[unlikely]] {
      throw incompatible_quantity_holder(
          "Cannot subtract quantities whose units have different reference "
          "points.");
    }
    if (rhs.get_multiplier() == Derived::units.multiplier) {
      lhs.value_ -= rhs.get_value_unsafe();
    } else {
      lhs.value_ -= Derived(rhs).get_value_unsafe();
    }
    return lhs;
  }

  /// \brief Addition assignment operator for numeric types
  ///
  /// Adds a numeric value to the numerical value of this \c quantity_value
  /// instance and returns a reference to the modified \c quantity_value.
  ///
  /// This operator is only available if the units of the \c quantity_value are
  /// unitless.
  ///
  /// \tparam T2 The type of the numeric value.
  /// \param lhs The \c quantity_value instance to modify.
  /// \param rhs The numeric value to add.
  /// \return A reference to the modified \c quantity_value instance.
  template <typename T2>
    requires(!quantity_value_like<T2>) &&
            requires(typename Derived::value_type lhs, T2 rhs) {
              lhs += rhs;
            } && unitless<Derived::units>
  friend constexpr auto operator+=(Derived& lhs, T2&& rhs) -> Derived {
    lhs.value_ += std::forward<T2>(rhs);
    return lhs;
  }

  template <typename T2>
    requires(!quantity_value_like<T2>) &&
            requires(typename Derived::value_type lhs, T2 rhs) { lhs -= rhs; }
            friend constexpr auto operator-=(Derived& lhs, T2&& rhs) -> Derived&
              requires unitless<Derived::units>
  {
    lhs.value_ -= std::forward<T2>(rhs);
    return lhs;
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr quantity_value_like auto
  operator+(Derived lhs, const quantity_value<U2, Q2, T2>& rhs) {
    static_assert(unit_addable_with<Derived::units, U2>,
                  "Cannot add quantities of different kinds or quantities "
                  "whose units have different reference points.");
    return lhs += rhs;
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr quantity_value_like auto
  operator-(Derived lhs, const quantity_value<U2, Q2, T2>& rhs) {
    static_assert(unit_subtractable_from<Derived::units, U2>,
                  "Cannot subtract quantities of different kinds or quantities "
                  "whose units have different reference points.");
    return lhs -= rhs;
  }

  template <typename T>
    requires(!quantity_value_like<T> && !is_quantity_holder_v<T>)
  friend constexpr quantity_value_like auto operator+(Derived lhs, T&& rhs) {
    using result_number_type =
        std::remove_cvref_t<decltype(lhs.get_value_unsafe() +
                                     std::forward<T>(rhs))>;
    return quantity_value<Derived::units, Derived::quantity,
                          result_number_type>(lhs.get_value_unsafe() +
                                              std::forward<T>(rhs));
  }

  template <typename T>
    requires(!quantity_value_like<T> && !is_quantity_holder_v<T>)
  friend constexpr quantity_value_like auto operator+(T&& lhs, Derived rhs) {
    using result_number_type =
        std::remove_cvref_t<decltype(std::forward<T>(lhs) +
                                     rhs.get_value_unsafe())>;
    return quantity_value<Derived::units, Derived::quantity,
                          result_number_type>(std::forward<T>(lhs) +
                                              rhs.get_value_unsafe());
  }

  template <typename T>
    requires(!quantity_value_like<T> && !is_quantity_holder_v<T>)
  friend constexpr quantity_value_like auto operator-(Derived lhs, T&& rhs) {
    return lhs -= std::forward<T>(rhs);
  }

  template <typename T>
    requires(!quantity_value_like<T> && !is_quantity_holder_v<T>)
  friend constexpr quantity_value_like auto operator-(T&& lhs, Derived rhs)
    requires unitless<decltype(rhs)::units>
  {
    return rhs -= std::forward<T>(lhs);
  }

  friend constexpr quantity_value_like auto
  operator*(const Derived& lhs, const quantity_value_like auto& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using result_type = std::remove_cvref_t<decltype(lhs.get_value_unsafe() *
                                                     rhs.get_value_unsafe())>;
    constexpr unit auto result_units = lhs_type::units * rhs_type::units;
    constexpr quantity auto result_quantity =
        lhs_type::quantity * rhs_type::quantity;
    return quantity_value<result_units, result_quantity, result_type>(
        lhs.get_value_unsafe() * rhs.get_value_unsafe());
  }

  template <typename T>
    requires(!quantity_value_like<T> && !unit<T> && !is_quantity_holder_v<T> &&
             !utility::_detail::is_value_type<T>::value)
  friend constexpr quantity_value_like auto operator*(const Derived& lhs,
                                                      const T& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using product_type =
        std::remove_cvref_t<decltype(lhs.get_value_unsafe() * rhs)>;
    return quantity_value<lhs_type::units, lhs_type::quantity, product_type>(
        lhs.get_value_unsafe() * rhs);
  }

  template <typename T>
    requires(!quantity_value_like<T> && !unit<T> && !is_quantity_holder_v<T> &&
             !utility::_detail::is_value_type<T>::value)
  friend constexpr quantity_value_like auto operator*(const T& lhs,
                                                      const Derived& rhs) {
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using product_type =
        std::remove_cvref_t<decltype(lhs * rhs.get_value_unsafe())>;
    return quantity_value<rhs_type::units, rhs_type::quantity, product_type>(
        lhs * rhs.get_value_unsafe());
  }

  friend constexpr quantity_value_like auto
  operator/(const Derived& lhs, const quantity_value_like auto& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using result_type = std::remove_cvref_t<decltype(lhs.get_value_unsafe() /
                                                     rhs.get_value_unsafe())>;
    constexpr unit auto result_units = lhs_type::units / rhs_type::units;
    constexpr quantity auto result_quantity =
        lhs_type::quantity / rhs_type::quantity;
    return quantity_value<result_units, result_quantity, result_type>(
        lhs.get_value_unsafe() / rhs.get_value_unsafe());
  }

  template <typename T>
    requires(!quantity_value_like<T> && !quantity_holder_like<T> && !unit<T>)
  friend constexpr quantity_value_like auto operator/(const Derived& lhs,
                                                      const T& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using quotient_type =
        std::remove_cvref_t<decltype(lhs.get_value_unsafe() / rhs)>;
    return quantity_value<lhs_type::units, lhs_type::quantity, quotient_type>(
        lhs.get_value_unsafe() / rhs);
  }

  template <typename T>
    requires(!quantity_value_like<T> && !quantity_holder_like<T> && !unit<T> &&
             !utility::_detail::is_value_type<T>::value)
  friend constexpr quantity_value_like auto operator/(const T& lhs,
                                                      const Derived& rhs) {
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using quotient_type =
        std::remove_cvref_t<decltype(lhs / rhs.get_value_unsafe())>;
    return quantity_value<inv(rhs_type::units), inv(rhs_type::quantity),
                          quotient_type>(lhs / rhs.get_value_unsafe());
  }

  friend constexpr quantity_value_like auto
  operator%(const Derived& lhs, const quantity_value_like auto& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using result_type = std::remove_cvref_t<decltype(lhs.get_value_unsafe() %
                                                     rhs.get_value_unsafe())>;
    constexpr unit auto result_units = lhs_type::units / rhs_type::units;
    constexpr quantity auto result_quantity =
        lhs_type::quantity_kind / rhs_type::quantity_kind;
    return quantity_value<result_units, result_quantity, result_type>(
        lhs.get_value_unsafe() % rhs.get_value_unsafe());
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator<=>(const Derived& lhs,
                                    const quantity_value<U2, Q2, T2>& rhs)
    requires std::three_way_comparable_with<
        std::remove_cvref_t<decltype(lhs.get_value_unsafe())>,
        std::remove_cvref_t<decltype(rhs.get_value_unsafe())>>
  {
    static_assert(unit_comparable_with<Derived::units, U2>,
                  "Cannot compare quantities of different kinds");
    return lhs.in_base_units().get_value_unsafe() <=>
           rhs.in_base_units().get_value_unsafe();
  }

  template <auto U2, auto Q2, typename T2>
  friend auto operator==(const Derived& lhs,
                         const quantity_value<U2, Q2, T2>& rhs) -> bool
    requires std::equality_comparable_with<
        std::remove_cvref_t<decltype(lhs.get_value_unsafe())>,
        std::remove_cvref_t<decltype(rhs.get_value_unsafe())>>
  {
    static_assert(unit_comparable_with<Derived::units, U2>,
                  "Cannot compare quantities of different kinds");
    return lhs.in_base_units().get_value_unsafe() ==
           rhs.in_base_units().get_value_unsafe();
  }

  /// \brief Multiplies the units of a \c quantity_value by another unit
  ///
  /// Multiplies the units of a \c quantity_value by another unit, returning a
  /// new \c quantity_value with the resulting units. Does not change the
  /// numerical value of the \c quantity_value.
  ///
  /// \tparam U1 The units of the left-hand side \c quantity_value.
  /// \tparam Q1 The quantity of the left-hand side \c quantity_value.
  /// \tparam T The type of the numerical value of the left-hand side \c
  /// quantity_value.
  /// \tparam U2 The units of the right-hand side unit.
  /// \param value The left-hand side \c quantity_value.
  /// \param U2 The right-hand side unit.
  /// \return A new \c quantity_value with the resulting units and the same
  /// numerical value.
  template <unit U2> friend constexpr auto operator*(const Derived& value, U2) {
    constexpr unit auto new_units = Derived::units * U2{};
    constexpr quantity auto new_quantity = new_units.quantity;
    return quantity_value<new_units, new_quantity,
                          typename Derived::value_type>(
        value.get_value_unsafe());
  }

  /// \brief Multiplies the units of a \c quantity_value by another unit
  ///
  /// Multiplies the units of a \c quantity_value by another unit, returning a
  /// new \c quantity_value with the resulting units. The numerical value is
  /// moved into the new \c quantity_value.
  ///
  /// \tparam U1 The units of the left-hand side \c quantity_value.
  /// \tparam Q1 The quantity of the left-hand side \c quantity_value.
  /// \tparam T The type of the numerical value of the left-hand side \c
  /// quantity_value.
  /// \tparam U2 The units of the right-hand side unit.
  /// \param value The left-hand side \c quantity_value.
  /// \param U2 The right-hand side unit.
  /// \return A new \c quantity_value with the resulting units and the same
  /// numerical value.
  template <unit U2> friend constexpr auto operator*(Derived&& value, U2) {
    constexpr unit auto new_units = Derived::units * U2{};
    constexpr quantity auto new_quantity = new_units.quantity;
    return quantity_value<new_units, new_quantity,
                          typename Derived::value_type>(
        std::move(value).get_value_unsafe());
  }

  template <unit U2> friend constexpr auto operator/(const Derived& value, U2) {
    constexpr unit auto new_units = Derived::units / U2{};
    constexpr quantity auto new_quantity = new_units.quantity;
    return quantity_value<new_units, new_quantity,
                          typename Derived::value_type>(
        value.get_value_unsafe());
  }

  template <unit U2> friend constexpr auto operator/(Derived&& value, U2) {
    constexpr unit auto new_units = Derived::units / U2{};
    constexpr quantity auto new_quantity = new_units.quantity;
    return quantity_value<new_units, new_quantity,
                          typename Derived::value_type>(
        std::move(value).get_value_unsafe());
  }
};

template <typename Derived> struct quantity_value_output {
  friend std::ostream& operator<<(std::ostream& os, const Derived& q) {
#ifdef MAXWELL_HAS_PRINT
    std::print(os, q);
#else
    os << std::format("{}", q);
#endif
    return os;
  }
};

/// \cond
template <utility::ratio From, unit To>
constexpr double chrono_conversion_factor(From, To) {
  const double from_value =
      static_cast<double>(From::num) / static_cast<double>(From::den);
  double ret_val = from_value * To::multiplier;
  return ret_val;
}
/// \endcond
} // namespace _detail

/// \brief Class template representing the value of a quantity with a particular
/// unit.
///
/// Class template \c quantity_value represents the value of a quantity
/// expressed in a particular unit. Both the quantity and unit of the \c
/// quantity_value are specified at compile-time and are part of the type of the
/// \c quantity_value. By making the quantity and unit part of the type of the
/// \c quantity_value, unit coherence can be verified at compile-time. It is not
/// permited to mix instances of \c quantity_value representing different
/// quantities. However, two \c quantity_value instances may have the same unit,
/// but represent different quantities, e.g. length and wavelength can both be
/// expressed in nanometers, but may represent different quantities. This allows
/// for an additional level of type safety. <br>
///
/// By making the unit part of the type, unit conversions can be performed
/// automatically at compile-time rather than run-time. This makes the library
/// extremely efficient. <br>
///
/// Both the units and the quantity of the \c quantity_value are specified as
/// non-type template parameters (NTTPs).
/// Using NTTPs allows for more natural definitions of custom units and
/// definitions.
///
/// \warning Using an integral type with \c quantity_value will perform
/// truncation when converting units and integer division when performing
/// division.
///
/// \pre \c Q is convertible to the quantity of the specified units \c U.
///       The program is ill-formed if this is violated.
///
/// \tparam U The units of the \c quantity_value.
/// \tparam Q The quantity of the \c quantity_value. Default: the quantity of
/// the units.
/// \tparam T The type of the \c quantity_value. Default: \c double.
MODULE_EXPORT template <auto U, auto Q, typename T>
  requires unit<decltype(U)> && quantity<decltype(Q)>
class quantity_value
    : _detail::_quantity_value_operators<quantity_value<U, Q, T>>,
      _detail::quantity_value_output<quantity_value<U, Q, T>> {
  static_assert(
      quantity_convertible_to<Q, U.quantity>,
      "Attempting to instantiate quantity value with incompatible units");

public:
  /// The type of the numerical value of the \c quantity_value.
  using value_type = T;
  /// The type of the quantity of the \c quantity_value.
  using quantity_type = std::remove_cv_t<decltype(U)>;
  /// The type of the units of the \c quantity_value.
  using units_type = std::remove_cv_t<decltype(U)>;
  /// The units of the \c quantity_value.
  static constexpr unit auto units = U;
  /// The quantity of the \c quantity_value.
  static constexpr ::maxwell::quantity auto quantity = Q;

  // --- Constructors ---

  /// \brief Default constructor
  ///
  /// Value initializes the numerical value of the \c quantity_value.
  /// This constructor only participates in overload resolution if
  /// \c std::is_default_constructible_v<T> is \c true.
  ///
  /// This constructor is a \c constexpr constructor if the default constructor
  /// of \c T is a constexpr constructor.
  ///
  /// \pre \c T is default constructible.
  /// \throw Any exceptions thrown by the default constructor of \c T.
  constexpr quantity_value()
    requires std::is_default_constructible_v<T>
  = default;

  /// \brief Constructor
  ///
  /// Direct initializes the numerical value of the \c quantity_value from
  /// \c std::forward<Up>(value).
  /// This constructor only participates in overload resolution if
  ///   1. <tt>std::is_constructible_from_v<T, Up></tt> is \c true.
  ///   2. \c Up is not an instantiation of \c quantity_value.
  ///
  /// This constructor is explicit if either of the following is \c false
  ///   1. <tt>std::convertible_to<Up, T></tt>
  ///   2. \c unitless<U> is true
  ///
  /// This constructor is a \c constexpr constructor if the selected constructor
  /// of \c T is a constexpr constructor.
  ///
  /// \pre <tt>std::is_constructible_from_v<T, Up></tt> is \c true and
  ///      \c Up is not an instantiation of \c quantity_value.
  ////
  /// \param value The value used to initialize the \c quantity_value.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_value_v<Up> &&
              !_detail::is_quantity_holder_v<Up>)
  constexpr explicit(!std::convertible_to<Up, T> || !unitless<U>)
      quantity_value(Up&& value);

  /// \brief Constructor
  ///
  /// Constructs the numerical value of the \c quantity_value in place
  /// from \c std::forward<Args>(args)...
  /// This constructor only participates in overload resolution if
  /// 1. <tt>std::is_constructible_from_v<T, Args...></tt> is \c true.
  ///
  /// This constructor is a \c constexpr constructor if the selected constructor
  /// of \c T is a constexpr constructor.
  ///
  /// \pre <tt>std::is_constructible_from_v<T, Args...></tt> is \c true.
  ///
  /// \tparam Args The types of the arguments used to construct the numerical
  /// value of the quantity.
  /// \param args The arguments used to construct the numerical value of the
  /// quantity.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr explicit quantity_value(std::in_place_t, Args&&... args);

  /// \brief Constructor
  ///
  /// Constructs the numerical value of the \c quantity_value in place
  /// from \c std::forward<Args>(args)...
  /// This constructor only participates in overload resolution if
  /// 1. <tt>std::is_constructible_from_v<T, std::initializer_list<Up>&,
  /// Args...></tt> is \c true.
  ///
  /// This constructor is a \c constexpr constructor if the selected constructor
  /// of \c T is a constexpr constructor.
  ///
  /// \pre <tt>std::is_constructible_from_v<T, std::initializer_list<Up>&,
  /// Args...></tt> is \c true.
  ///
  /// \tparam Up The type of the values in the initializer list
  /// \tparam Args The types of the arguments used to construct the numerical
  /// value of the quantity.
  /// \param il The initializer list used to construct the numerical value of
  /// the quantity.
  /// \param args The arguments used to construct the numerical value of the
  /// quantity.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename Up, typename... Args>
    requires std::constructible_from<T, std::initializer_list<Up>&, Args...>
  constexpr explicit quantity_value(std::in_place_t,
                                    std::initializer_list<Up> il,
                                    Args&&... args);

  /// \brief Constructor
  ///
  /// Constructs an instance of \c quantity_value from an instance of \c
  /// std::chrono::duration. Automatically converts the numerical value of the
  /// \c std::chrono::duration instance to the corresponding value in the units
  /// of the \c quantity_value instance being constructed.
  ///
  /// This constructor only participates in overload resolution if the following
  /// are true:
  /// 1. <tt>std::is_constructible_from_v<T, Rep></tt> is \c true.
  /// 2. \c Period is a valid \c std::ratio.
  ///
  /// The program is ill formed if \c enable_chrono_conversions_v<Q>` is \c
  /// false.
  ///
  ///
  /// \pre \c T is constructible from \c \Rep and \c Period is an instance of \c
  /// std::ratio and \c Q represents time.
  ///
  /// \tparam Rep The representation type of the \c std::chrono::duration
  /// instance \tparam Period The \c std::ratio type representing the period of
  /// the \c std::chrono::duration instance
  /// \param d The duration used to initialize the numerical value of the
  /// quantity.
  /// \throws Any exceptions thrown by the selected constructor \c T or
  /// <tt>std::chrono::duration<Rep, Period>::count()</tt>
  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && utility::ratio<Period>
  MAXWELL_CONSTEXPR23
  quantity_value(const std::chrono::duration<Rep, Period>& d);

  /// \brief Converting constructor
  ///
  /// Constructs an instance of \c quantity_value from another instance of \c
  /// quantity_value with different units, automatically converting from \c
  /// FromUnit to \c ToUnit. The conversion factor is calculated at
  /// compile-time. In this constructor, an lvalue-reference to the numerical
  /// value of \c other is passed to the conversion function.
  ///
  /// This function only participates in overload resolution if the following
  /// are true:
  /// 1. <tt>std::is_constructible_from_v<T, Up></tt> is \c true.
  /// 2. \c unit<decltype(FromUnit)> is \c true.
  /// 3. \c quantity<decltype(FromQuantity)> is \c true.
  ///
  /// The program is ill-formed if \c other represents a quantity that cannot be
  /// converted to the \c quantity_value being constructed from.
  ///
  /// \tparam FromQuantity The quantity of the other \c quantity_value instance.
  /// \tparam FromUnit The units of the other \c quantity_value instance.
  /// \tparam Up The type of the numerical value of the other \c quantity_value
  /// instance.
  /// \param other The other \c quantity_value instance to convert from.
  /// \throws Any exceptions thrown by the selected constructor of \c T
  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && unit<decltype(FromUnit)> &&
             ::maxwell::quantity<decltype(FromQuantity)>
  constexpr quantity_value(
      const quantity_value<FromUnit, FromQuantity, Up>& other);

  /// \brief Converting constructor
  ///
  /// Constructs an instance of \c quantity_value from another instance of \c
  /// quantity_value with different units, automatically converting from \c
  /// FromUnit to \c ToUnit. The conversion factor is calculated at
  /// compile-time. In this constructor, an rvalue-reference to the numerical
  /// value of \c other is passed to the conversion function.
  ///
  /// This function only participates in overload resolution if the following
  /// are true:
  /// 1. <tt>std::is_constructible_from_v<T, Up></tt> is \c true.
  /// 2. \c unit<decltype(FromUnit)> is \c true.
  /// 3. \c quantity<decltype(FromQuantity)> is \c true.
  ///
  /// The program is ill-formed if \c other represents a quantity that cannot be
  /// converted to the \c quantity_value being constructed from.
  ///
  /// \tparam FromQuantity The quantity of the other \c quantity_value instance.
  /// \tparam FromUnit The units of the other \c quantity_value instance.
  /// \tparam Up The type of the numerical value of the other \c quantity_value
  /// instance.
  /// \param other The other \c quantity_value instance to convert from.
  /// \throws Any exceptions thrown by the selected constructor of \c T
  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && unit<decltype(FromUnit)> &&
             ::maxwell::quantity<decltype(FromQuantity)>
  constexpr quantity_value(quantity_value<FromUnit, FromQuantity, Up>&& other);

  /// \brief Converting constructor
  ///
  /// Constructs an instance of \c quantity_value from an instance of \c
  /// quantity_holder, automatically converting from the quantity and units of
  /// the \c quantity_holder to the quantity and units of the \c quantity_value.
  /// The conversion factor is calculated at compile-time. In this constructor,
  /// an lvalue-reference to the numerical value of \c other is passed to the
  /// conversion function.
  ///
  /// This function only participates in overload resolution if the following
  /// are true:
  /// 1. <tt>std::is_constructible_from_v<T, Up></tt> is \c true.
  /// 2. \c quantity<decltype(FromQuantity)> is \c true.
  ///
  /// \tparam FromQuantity The quantity of the \c quantity_holder used to
  /// construct the \c quantity_value.
  /// \tparam Up The type of the numerical value of the \c quantity_holder being
  /// used to construct the \c quantity_value.
  /// \param other The \c quantity_holder instance used to construct the \c
  /// quantity_value.
  /// \throws Any exceptions thrown by the selected constructor of \c T.
  template <auto FromQuantity, typename Up = T>
    requires std::constructible_from<T, Up> &&
             ::maxwell::quantity<decltype(FromQuantity)>
  constexpr quantity_value(const quantity_holder<FromQuantity, T>& other);

  /// \brief Converting constructor
  ///
  /// Constructs an instance of \c quantity_value from an instance of \c
  /// quantity_holder, automatically converting from the quantity and units of
  /// the \c quantity_holder to the quantity and units of the \c quantity_value.
  /// The conversion factor is calculated at compile-time. In this constructor,
  /// an rvalue-reference to the numerical value of \c other is passed to the
  /// conversion function.
  ///
  /// This function only participates in overload resolution if the following
  /// are true:
  /// 1. <tt>std::is_constructible_from_v<T, Up></tt> is \c true.
  /// 2. \c quantity<decltype(FromQuantity)> is \c true.
  ///
  /// \tparam FromQuantity The quantity of the \c quantity_holder used to
  /// construct the \c quantity_value.
  /// \tparam Up The type of the numerical value of the \c quantity_holder being
  /// used to construct the \c quantity_value.
  /// \param other The \c quantity_holder instance used to construct the \c
  /// quantity_value.
  /// \throws Any exceptions thrown by the selected constructor of \c T.
  template <auto FromQuantity, typename Up = T>
    requires std::constructible_from<T, Up> &&
             ::maxwell::quantity<decltype(FromQuantity)>
  constexpr quantity_value(quantity_holder<FromQuantity, T>&& other);

  // --- Assignment Operators ---

  /// \brief Assigns the value of the specified \c quantity_value to the value
  /// of \c *this.
  ///
  /// Assigns the value of the specififed \c quantity_value to the value oc \c
  /// *this, automatically converting units where necessary. This function only
  /// participates in overload resolution if.
  ///   1. <tt>std::constructible_from<T, Rep></tt> is modeled
  ///   2. \c std::swappable<T> is modeled
  ///
  /// The program is ill-formed if <tt>quantity_convertible_to<FromQuantity,
  /// Q></tt> is not modeled.
  ///
  /// \tparam FromQuantity The quantity of the from \c quantity_value
  /// \tparam FromUnit The units being converted from
  /// \tparam Up The type of the numerical value being assigned to \c *this.
  /// \param other The \c quantity_value to assign to \c *this.
  /// \return A reference to \c *this.
  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && std::swappable<T>
  constexpr auto operator=(quantity_value<FromUnit, FromQuantity, Up> other)
      -> quantity_value&;

  /// \brief Assigns the value of the specified \c quantity_holder to the value
  /// of \c *this.
  ///
  /// Assigns the value of the specififed \c quantity_ho,der to the value oc \c
  /// *this, automatically converting units where necessary. This function only
  /// participates in overload resolution if.
  ///   1. <tt>std::constructible_from<T, Rep></tt> is modeled
  ///   2. \c std::swappable<T> is modeled
  ///
  /// The program is ill-formed if <tt>quantity_convertible_to<FromQuantity,
  /// Q></tt> is not modeled.
  ///
  /// \tparam FromQuantity The quantity of the from \c quantity_holder
  /// \tparam Up The type of the numerical value being assigned to \c *this.
  /// \param other The \c quantity_holder to assign to \c *this.
  /// \return A reference to \c *this.
  template <auto FromQuantity, typename Up = T>
    requires std::constructible_from<T, Up> && std::swappable<T>
  constexpr auto operator=(quantity_holder<FromQuantity, Up> other)
      -> quantity_value&;

  /// \brief Assigns the specified \c std::chrono::duration to the value of \c
  /// *this.
  ///
  /// Assigns the specified \c std::chrono::duration to the value of \c *this,
  /// converting from the units represented by the specified \c
  /// std::chrono::duration to the units of \c *this if necessary. This function
  /// only participates in overload resolution if
  ///   1. <tt>std::constructible_from<T, Rep></tt> is modeled
  ///   2. \c std::swappable<T> is modeled
  ///   3. \c enabe_chrono_conversions_v<Q> is true
  ///
  /// \tparam Rep The representation type of the \c std::chrono::duration
  /// \tparam Period The period type of the \c std::chrono::duration
  /// \param d The \c std::chrono::duraation to assign to \c *this.
  /// \return A reference to \c *this.
  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && std::swappable<T> &&
             enable_chrono_conversions_v<Q>
  constexpr auto operator=(const std::chrono::duration<Rep, Period>& d)
      -> quantity_value&;

  /// \brief Assigns the specified value to to the value of \c *this.
  ///
  /// Assigns the specififed value to the numerical value of \c *this. This
  /// function only participates in overload resolution if
  ///   1. \c Up is not an instantiation of \c quantity_value
  ///   2. <tt>std::is_assignable_v<T&, Up></tt> is true
  ///   3. \c unitless<U> is modeled.
  ///
  /// \tparam Up The type of the value to assign to the numerical value of \c
  /// *this.
  /// \param other The value to assign to \c *this.
  /// \return A reference to \c *this.
  template <typename Up = T>
    requires(!_detail::is_quantity_value_v<Up> &&
             !_detail::is_quantity_holder_v<Up> &&
             std::is_assignable_v<T&, Up> && unitless<U>)
  constexpr auto operator=(Up&& other) -> quantity_value&;

  // --- Accessor Methods ---

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns a constant lvalue-reference to the raw numerical value of the \c
  /// quantity_value instance. This method is unsafe because it exposes the raw
  /// numerical value, allowing for modification without regard to units.
  ///
  ///
  /// \return A constant lvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  constexpr auto get_value_unsafe() const& noexcept -> const T&;

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns an rvalue-reference to the numerical value of the \c
  /// quantity_value instance. This method is unsafe because it exposes the raw
  /// numerical value, allowing for modification without regard to units.
  ///
  /// \return An rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  constexpr auto get_value_unsafe() && noexcept -> T&&;

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns a constant rvalue-reference to the numerical value of the \c
  /// quantity_value instance. This method is unsafe because it exposes the raw
  /// numerical value, allowing for modification without regard to units.
  ///
  /// \return A constant rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  constexpr auto get_value_unsafe() const&& noexcept -> const T&&;

  /// \brief Conversion operator to the numerical value type
  ///
  /// Converts the \c quantity_value instance to its numerical value type.
  /// This operator is explicit if the \c quantity_value is not unitless.
  ///
  /// \return The numerical value of the \c quantity_value instance.
  constexpr explicit(!unitless<U>) operator value_type() const;

  /// \brief Returns the units of the quantity.
  ///
  /// \return The units of the quantity.
  constexpr auto get_units() const noexcept -> units_type;

  /// \brief Returns a quantity with the same value expressed in base units.
  ///
  /// Returns a quantity representing the same value but expressed in the
  /// unit system's base units.
  ///
  /// \return A quantity with the same value expressed in base units
  constexpr auto in_base_units() const -> quantity_value<U.base_units(), Q, T>;

  template <unit ToUnit>
  constexpr auto in(ToUnit) const -> quantity_value<ToUnit{}, Q, T>;

private:
  friend class _detail::_quantity_value_operators<quantity_value<U, Q, T>>;

  T value_{};
};

// --- Class Template Argument Deduction Guides ---
template <auto Q, auto U, typename T>
quantity_value(const quantity_value<Q, U, T>&) -> quantity_value<Q, U, T>;

template <auto Q, auto U, typename T>
quantity_value(quantity_value<Q, U, T>&&) -> quantity_value<Q, U, T>;
} // namespace maxwell