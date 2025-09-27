/// \file quantity_value.hpp
/// \brief Definition of class template \c quantity_value.hpp

#ifndef QUANTITY_VALUE_HPP
#define QUANTITY_VALUE_HPP

#include <chrono>           // duration
#include <compare>          // spaceship operator
#include <concepts>         // constructible_from, convertible_to, swappable
#include <format>           // formatter
#include <functional>       // hash
#include <initializer_list> // initializer_list
#include <iterator>         // back_inserter
#include <ostream>          // ostream
#include <string_view>      // string_view
#include <type_traits> // false_type, is_assignable_v, remove_cvref_t, true_type
#include <utility>     // forward, in_place_t, move

#include "quantity.hpp"
#include "unit.hpp"
#include "utility/compile_time_math.hpp"
#include "utility/config.hpp"
#include "utility/type_traits.hpp"

namespace maxwell {
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
MODULE_EXPORT template <auto U, auto Q = U.quantity, typename T = double>
  requires unit<decltype(U)> && quantity<decltype(Q)>
class quantity_value;
} // namespace maxwell

template <auto U, auto Q, typename T>
struct std::formatter<maxwell::quantity_value<U, Q, T>>
    : formatter<string_view> {
  auto constexpr parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto constexpr format(const maxwell::quantity_value<U, Q, T>& q,
                        format_context& ctx) const {
    string str;
    format_to(back_inserter(str), "{} {}", q.get_value(), q.get_units());
    return formatter<string_view>::format(str, ctx);
  }
};

namespace maxwell {
/// \cond
namespace _detail {
template <typename> struct is_quantity_value : std::false_type {};

template <auto U, auto Q, typename T>
struct is_quantity_value<quantity_value<U, Q, T>> : std::true_type {};

template <typename T>
constexpr bool is_quantity_value_v =
    is_quantity_value<std::remove_cvref_t<T>>::value;

template <typename T>
concept quantity_value_like = is_quantity_value_v<T>;
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
  MODULE_EXPORT friend constexpr auto operator-(const Derived& q) -> Derived {
    return std::remove_cvref_t<decltype(q)>(-q.get_value());
  }

  /// \brief Pre-increment operator
  ///
  /// Increments the numerical value of a \c quantity_value instance and returns
  /// a reference to the modified \c quantity_value.
  ///
  /// \param q The quantity value instance to increment.
  /// \return A reference to the modified \c quantity_value instance.
  MODULE_EXPORT friend constexpr auto operator++(Derived& q) -> Derived& {
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
  MODULE_EXPORT friend constexpr auto operator++(Derived& q, int) -> Derived {
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
  MODULE_EXPORT friend constexpr auto operator--(Derived& q) -> Derived& {
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
  MODULE_EXPORT friend constexpr auto operator--(Derived& q, int) -> Derived {
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
  /// \tparam Q2 The quantity of the other \c quantity_value instance.
  /// \tparam T2 The type of the other \c quantity_value instance.
  /// \param lhs The \c quantity_value instance to modify.
  /// \param rhs The other \c quantity_value instance to add.
  /// \return A reference to the modified \c quantity_value instance.
  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
    requires requires(typename Derived::value_type lhs, T2 rhs) { lhs += rhs; }
  friend constexpr auto operator+=(Derived& lhs,
                                   const quantity_value<U2, Q2, T2>& rhs)
      -> Derived& {
    static_assert(unit_addable_with<Derived::units, U2>,
                  "Cannot add quantities of different kinds or quantities "
                  "whose units have different reference points.");
    if constexpr (U2.multiplier == Derived::units.multiplier) {
      lhs.value_ += rhs.get_value();
    } else {
      lhs.value_ += Derived(rhs).get_value();
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
  /// \tparam Q2 The quantity of the other \c quantity_value instance.
  /// \tparam T2 The type of the other \c quantity_value instance.
  /// \param lhs The \c quantity_value instance to modify.
  /// \param rhs The other \c quantity_value instance to subtract.
  /// \return A reference to the modified \c quantity_value instance.
  template <auto U2, auto Q2, typename T2>
    requires requires(typename Derived::value_type lhs, T2 rhs) { lhs -= rhs; }
  MODULE_EXPORT friend constexpr auto
  operator-=(Derived& lhs, const quantity_value<U2, Q2, T2>& rhs) -> Derived& {
    static_assert(unit_subtractable_from<Derived::units, U2>,
                  "Cannot subtract quantities of different kinds or quantities "
                  "whose units have different reference points.");
    if constexpr (U2.multiplier == Derived::units.multiplier) {
      lhs.value_ -= rhs.get_value();
    } else {
      lhs.value_ -= Derived(rhs).get_value();
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
  MODULE_EXPORT template <typename T2>
    requires(!quantity_value_like<T2>) &&
            requires(typename Derived::value_type lhs, T2 rhs) {
              lhs += rhs;
            } && unitless<Derived::units>
  friend constexpr auto operator+=(Derived& lhs, T2&& rhs) -> Derived {
    lhs.value_ += std::forward<T2>(rhs);
    return lhs;
  }

  MODULE_EXPORT template <typename T2>
    requires(!quantity_value_like<T2>) &&
            requires(typename Derived::value_type lhs, T2 rhs) { lhs -= rhs; }
            friend constexpr auto operator-=(Derived& lhs, T2&& rhs) -> Derived&
              requires unitless<Derived::units>
  {
    lhs.value_ -= std::forward<T2>(rhs);
    return lhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr quantity_value_like auto
  operator+(Derived lhs, const quantity_value<U2, Q2, T2>& rhs) {
    static_assert(unit_addable_with<Derived::units, U2>,
                  "Cannot add quantities of different kinds or quantities "
                  "whose units have different reference points.");
    return lhs += rhs;
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr quantity_value_like auto
  operator-(Derived lhs, const quantity_value<U2, Q2, T2>& rhs) {
    static_assert(unit_subtractable_from<Derived::units, U2>,
                  "Cannot subtract quantities of different kinds or quantities "
                  "whose units have different reference points.");
    return lhs -= rhs;
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T>)
  friend constexpr quantity_value_like auto operator+(Derived lhs, T&& rhs) {
    using result_number_type =
        std::remove_cvref_t<decltype(lhs.get_value() + std::forward<T>(rhs))>;
    return quantity_value<Derived::units, Derived::quantity_kind,
                          result_number_type>(lhs.get_value() +
                                              std::forward<T>(rhs));
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T>)
  friend constexpr quantity_value_like auto operator+(T&& lhs, Derived rhs) {
    using result_number_type =
        std::remove_cvref_t<decltype(std::forward<T>(lhs) + rhs.get_value())>;
    return quantity_value<Derived::units, Derived::quantity_kind,
                          result_number_type>(std::forward<T>(lhs) +
                                              rhs.get_value());
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T>)
  friend constexpr quantity_value_like auto operator-(Derived lhs, T&& rhs) {
    return lhs -= std::forward<T>(rhs);
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T>)
  friend constexpr quantity_value_like auto operator-(T&& lhs, Derived rhs)
    requires unitless<decltype(rhs)::units>
  {
    return rhs -= std::forward<T>(lhs);
  }

  MODULE_EXPORT friend constexpr quantity_value_like auto
  operator*(const Derived& lhs, const quantity_value_like auto& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    constexpr unit auto result_units = lhs_type::units * rhs_type::units;
    constexpr quantity auto result_quantity =
        lhs_type::quantity_kind * rhs_type::quantity_kind;
    return quantity_value<result_units, result_quantity, result_type>(
        lhs.get_value() * rhs.get_value());
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T> && !unit<T> &&
             !utility::_detail::is_value_type<T>::value)
  friend constexpr quantity_value_like auto operator*(const Derived& lhs,
                                                      const T& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using product_type = std::remove_cvref_t<decltype(lhs.get_value() * rhs)>;
    return quantity_value<lhs_type::units, lhs_type::quantity_kind,
                          product_type>(lhs.get_value() * rhs);
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T> && !unit<T> &&
             !utility::_detail::is_value_type<T>::value)
  friend constexpr quantity_value_like auto operator*(const T& lhs,
                                                      const Derived& rhs) {
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using product_type = std::remove_cvref_t<decltype(lhs * rhs.get_value())>;
    return quantity_value<rhs_type::units, rhs_type::quantity_kind,
                          product_type>(lhs * rhs.get_value());
  }

  MODULE_EXPORT friend constexpr quantity_value_like auto
  operator/(const Derived& lhs, const quantity_value_like auto& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() / rhs.get_value())>;
    constexpr unit auto result_units = lhs_type::units / rhs_type::units;
    constexpr quantity auto result_quantity =
        lhs_type::quantity_kind / rhs_type::quantity_kind;
    return quantity_value<result_units, result_quantity, result_type>(
        lhs.get_value() / rhs.get_value());
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T> && !unit<T>)
  friend constexpr quantity_value_like auto operator/(const Derived& lhs,
                                                      const T& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using product_type = std::remove_cvref_t<decltype(lhs.get_value() / rhs)>;
    return quantity_value<lhs_type::units, lhs_type::quantity_kind,
                          product_type>(lhs.get_value() / rhs);
  }

  MODULE_EXPORT template <typename T>
    requires(!quantity_value_like<T> && !unit<T> &&
             !utility::_detail::is_value_type<T>::value)
  friend constexpr quantity_value_like auto operator/(const T& lhs,
                                                      const Derived& rhs) {
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using product_type = std::remove_cvref_t<decltype(lhs / rhs.get_value())>;
    return quantity_value<rhs_type::units, rhs_type::quantity_kind,
                          product_type>(lhs / rhs.get_value());
  }

  MODULE_EXPORT friend constexpr quantity_value_like auto
  operator%(const Derived& lhs, const quantity_value_like auto& rhs) {
    using lhs_type = std::remove_cvref_t<decltype(lhs)>;
    using rhs_type = std::remove_cvref_t<decltype(rhs)>;
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() % rhs.get_value())>;
    constexpr unit auto result_units = lhs_type::units / rhs_type::units;
    constexpr quantity auto result_quantity =
        lhs_type::quantity_kind / rhs_type::quantity_kind;
    return quantity_value<result_units, result_quantity, result_type>(
        lhs.get_value() % rhs.get_value());
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator<=>(const Derived& lhs,
                                    const quantity_value<U2, Q2, T2>& rhs)
    requires std::three_way_comparable_with<
        std::remove_cvref_t<decltype(lhs.get_value())>,
        std::remove_cvref_t<decltype(rhs.get_value())>>
  {
    static_assert(unit_comparable_with<Derived::units, U2>,
                  "Cannot compare quantities of different kinds");
    return lhs.in_base_units().get_value() <=> rhs.in_base_units().get_value();
  }

  MODULE_EXPORT template <auto U2, auto Q2, typename T2>
  friend auto operator==(const Derived& lhs,
                         const quantity_value<U2, Q2, T2>& rhs) -> bool
    requires std::equality_comparable_with<
        std::remove_cvref_t<decltype(lhs.get_value())>,
        std::remove_cvref_t<decltype(rhs.get_value())>>
  {
    static_assert(unit_comparable_with<Derived::units, U2>,
                  "Cannot compare quantities of different kinds");
    return lhs.in_base_units().get_value() == rhs.in_base_units().get_value();
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

template <auto U, auto Q, typename T>
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
  using quantity_kind_type = std::remove_cv_t<decltype(U)>;
  /// The type of the units of the \c quantity_value.
  using units_type = std::remove_cv_t<decltype(U)>;
  /// The units of the \c quantity_value.
  static constexpr unit auto units = U;
  /// The quantity of the \c quantity_value.
  static constexpr quantity auto quantity_kind = Q;

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
             (!_detail::is_quantity_value_v<Up>)
  constexpr explicit(!std::convertible_to<Up, T> || !unitless<U>)
      quantity_value(Up&& value)
      : value_(std::forward<Up>(value)) {}

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
  constexpr explicit quantity_value(std::in_place_t, Args&&... args)
      : value_(std::forward<Args>(args)...) {}

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
                                    Args&&... args)
      : value_(il, std::forward<Args>(args)...) {}

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
  quantity_value(const std::chrono::duration<Rep, Period>& d)
      : value_(utility::as_constant<_detail::chrono_conversion_factor(Period{},
                                                                      U)> *
               d.count()) {
    static_assert(enable_chrono_conversions_v<Q>,
                  "Attempting to construct a quantity_value that does not "
                  "represent time from a std::chrono::duration instance");
  }

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
             quantity<decltype(FromQuantity)>
  constexpr quantity_value(
      const quantity_value<FromUnit, FromQuantity, Up>& other)
      : value_(U.scale.from_scale(
            other.get_value() *
                    utility::as_constant<conversion_factor(FromUnit, U)> +
                utility::as_constant<conversion_offset(FromUnit, U)>,
            FromUnit.scale)) {
    static_assert(
        quantity_convertible_to<FromQuantity, Q>,
        "Attempting to construct value from incompatible quantity. Note, "
        "quantities can be incompatible even if they have te same units.");
  }

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
             quantity<decltype(FromQuantity)>
  constexpr quantity_value(
      quantity_value<FromUnit, FromQuantity, Up>&& other) noexcept
      : value_(U.scale.from_scale(
            std::move(other).get_value() *
                    utility::as_constant<conversion_factor(FromUnit, U)> +
                utility::as_constant<conversion_offset(FromUnit, U)>,
            FromUnit.scale)) {
    static_assert(
        quantity_convertible_to<FromQuantity, Q>,
        "Attempting to construct value from incompatible quantity. Note, "
        "quantities can be incompatible even if they have te same units.");
  }

  // --- Assignment Operators ---

  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up> && std::swappable<T>
  constexpr auto operator=(quantity_value<FromUnit, FromQuantity, Up> other)
      -> quantity_value& {

    static_assert(
        quantity_convertible_to<FromQuantity, Q>,
        "Attempting to construct value from incompatible quantity. Note, "
        "quantities can be incompatible even if they have te same units.");

    using std::swap;
    quantity_value temp(std::move(other));
    swap(temp.value_, value_);
    return *this;
  }

  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && std::swappable<T> &&
             enable_chrono_conversions_v<Q>
  constexpr auto operator=(const std::chrono::duration<Rep, Period>& d)
      -> quantity_value& {
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

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns a constant lvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  ///
  /// \return A constant lvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  constexpr auto get_value() const& noexcept -> const T& { return value_; }

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns an rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  ///
  /// \return An rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  constexpr auto get_value() && noexcept -> T&& { return std::move(value_); }

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns a constant rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  ///
  /// \return A constant rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  constexpr auto get_value() const&& noexcept -> const T&& {
    return std::move(value_);
  }

  constexpr explicit(!unitless<U>) operator value_type() const {
    return value_;
  }

  constexpr auto get_units() const noexcept -> units_type { return units; }

  constexpr auto in_base_units() const -> quantity_value<U.base_units(), Q, T> {
    constexpr unit auto base_units = U.base_units();
    constexpr double factor = conversion_factor(U, base_units);
    return quantity_value<base_units, Q, T>{value_ * factor};
  }

private:
  friend class _detail::_quantity_value_operators<quantity_value<U, Q, T>>;

  T value_{};
}; // namespace maxwell

// --- Class Template Argument Deduction Guides ---

template <auto Q, auto U, typename T>
quantity_value(const quantity_value<Q, U, T>&) -> quantity_value<Q, U, T>;

template <auto Q, auto U, typename T>
quantity_value(quantity_value<Q, U, T>&&) -> quantity_value<Q, U, T>;

MODULE_EXPORT template <typename Q>
using quetta = quantity_value<quetta_unit<Q::units>, Q::quantity_kind,
                              typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using ronna = quantity_value<ronna_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using yotta = quantity_value<yotta_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using zetta = quantity_value<zetta_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using exa = quantity_value<exa_unit<Q::units>, Q::quantity_kind,
                           typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using peta = quantity_value<peta_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using tera = quantity_value<tera_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using giga = quantity_value<giga_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using mega = quantity_value<mega_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using kilo = quantity_value<kilo_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using hecto = quantity_value<hecto_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using deca = quantity_value<deca_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using deci = quantity_value<deci_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using centi = quantity_value<centi_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using milli = quantity_value<milli_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using micro = quantity_value<micro_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using nano = quantity_value<nano_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using pico = quantity_value<pico_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using femto = quantity_value<femto_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using atto = quantity_value<atto_unit<Q::units>, Q::quantity_kind,
                            typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using zepto = quantity_value<zepto_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using yocto = quantity_value<yocto_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using ronto = quantity_value<ronto_unit<Q::units>, Q::quantity_kind,
                             typename Q::value_type>;
MODULE_EXPORT template <typename Q>
using quecto = quantity_value<quecto_unit<Q::units>, Q::quantity_kind,
                              typename Q::value_type>;

/// \brief Convets between two different quantities
///
/// Explicit cast between two different quantities. This function only checks
/// that the quantity being converted to has the same dimensions as the quantity
/// being converted from. It allows for an explicit conversion between
/// quantities with different kinds. Automatically converts the numerical value
/// of the input quantity from the input units to the destination units.
///
/// \tparam ToUnits The units of the quantity being converted to.
/// \tparam ToQuantity The quantity of the quantity being converted to. Defaults
/// to \c ToUnits.quantity.
/// \tparam FromUnits The units of the quantity being converted from.
/// \tparam FromQuantity The quantity of the quantity being converted from.
/// \tparam T The type of the numerical value of the quantity being converted
/// from.
/// \param value The quantity being converted from.
/// \return A \c quantity_value with the specified units and quantity, and the
/// converted numerical value.
MODULE_EXPORT template <auto ToUnits, auto ToQuantity = ToUnits.quantity,
                        auto FromUnits, auto FromQuantity, typename T>
constexpr auto
quantity_cast(const quantity_value<FromUnits, FromQuantity, T>& value)
    -> quantity_value<ToUnits, ToQuantity, T> {
  static_assert(ToQuantity.dimensions == FromQuantity.dimensions,
                "Cannot convert between quantities with different dimensions");

  constexpr double multiplier = conversion_factor(FromUnits, ToUnits);
  constexpr double offset = conversion_offset(FromUnits, ToUnits);
  return quantity_value<ToUnits, ToQuantity, T>(
      (value.get_value() * multiplier) + offset);
}

/// \brief Creates a \c quantity_value from a number and a unit
///
/// Creates a quantity_value from the specified numer and unit. The resulting
/// \c quantity_value has the specified number and units.
///
/// \tparam T The type of the numerical value of the quantity.
/// \tparam U The units of the quantity
/// \param value The numerical value of the quantity
/// \param U The units of the quantity.
/// \return A \c quantity_value with the specified value and units.
template <typename T, unit U>
  requires(!_detail::is_quantity_value_v<T> && !unit<T>)
constexpr auto operator*(T&& value, U) -> quantity_value<U{}, U::quantity, T> {
  return quantity_value<U{}, U::quantity, T>(std::forward<T>(value));
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
template <auto U1, auto Q1, typename T, unit U2>
constexpr auto operator*(const quantity_value<U1, Q1, T>& value, U2) {
  constexpr unit auto new_units = U1 * U2{};
  constexpr quantity auto new_quantity = new_units.quantity;
  return quantity_value<new_units, new_quantity, T>(value.get_value());
}

/// \brief Multiplies the units of a \c quantity_value by another unit
///
/// Multiplies the units of a \c quantity_value by another unit, returning a
/// new \c quantity_value with the resulting units. The numerical value is moved
/// into the new \c quantity_value.
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
template <auto U1, auto Q1, typename T, unit U2>
constexpr auto operator*(quantity_value<U1, Q1, T>&& value, U2) {
  constexpr unit auto new_units = U1 * U2{};
  constexpr quantity auto new_quantity = new_units.quantity;
  return quantity_value<new_units, new_quantity, T>(
      std::move(value).get_value());
}

template <auto U1, auto Q1, typename T, unit U2>
constexpr auto operator/(const quantity_value<U1, Q1, T>& value, U2) {
  constexpr unit auto new_units = U1 / U2{};
  constexpr quantity auto new_quantity = new_units.quantity;
  return quantity_value<new_units, new_quantity, T>(value.get_value());
}

template <auto U1, auto Q1, typename T, unit U2>
constexpr auto operator/(quantity_value<U1, Q1, T>&& value, U2) {
  constexpr unit auto new_units = U1 / U2{};
  constexpr quantity auto new_quantity = new_units.quantity;
  return quantity_value<new_units, new_quantity, T>(
      std::move(value).get_value());
}

} // namespace maxwell

/// \brief Specialization of \c std::hash
///
/// Specialization of \c std::hash for \c quantity_value.
/// Two \c quantity_value instances have the same hash code if they represent
/// the same value after being converted to base units.
/// Two \c quantity_values instances will have difference hash codes if they
/// have the same value and same units, but represent different quantities.
///
/// \note The specialization of \c std::hash is not suitable as cryptographic
/// hash function.
///
/// \tparam Q The quantity of the \c quantity_value
/// \tparam U The units of the \c quantity_value
/// \tparam T The underlying type of the \c quantity_value
template <auto Q, auto U, typename T>
struct std::hash<maxwell::quantity_value<Q, U, T>> {
  auto operator()(const maxwell::quantity_value<Q, U, T>& q) noexcept
      -> std::size_t {
    const auto in_base_units = q.in_base_units();
    std::size_t hash_code =
        std::hash<typename decltype(q)::value_type>{}(q.get_value());
    hash_code ^= std::hash<decltype(q.get_units().name)>{}(q.get_units().name) +
                 0x9e377b9b + (hash_code << 6) + (hash_code >> 2);
    hash_code ^=
        std::hash<decltype(q.quantity_kind::name)>{q.quantity_kind.name} +
        0x9e377b9b + (hash_code << 6) + (hash_code >> 2);
    return hash_code;
  }
};

#endif