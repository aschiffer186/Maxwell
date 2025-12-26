
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
/// \brief Exception thrown when attempting to perform an operation on
/// incompatible \c quantity_holders
///
/// Class \c incompatible_quantity_holder is an exception thrown when an
/// arithmetic operation is performed on two instaces of \c quantity_holder with
/// difference references. Because the units of a \c quantity_holder are
/// determined at run-time, this error cannot be caught at compile-time.
class incompatible_quantity_holder : public std::runtime_error {
public:
  /// \brief Constructor
  ///
  /// \param message The error message associated with the exception.
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
    static_assert(quantity_convertible_to<Q2, Derived::quantity> &&
                      quantity_convertible_to<Derived::quantity, Q2>,
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
    static_assert(quantity_convertible_to<Q2, Derived::quantity> &&
                      quantity_convertible_to<Derived::quantity, Q2>,
                  "Cannot add quantities of different kinds");
    if (lhs.reference_ != U2.reference) {
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
    static_assert(quantity_convertible_to<Q2, Derived::quantity> &&
                      quantity_convertible_to<Derived::quantity, Q2>,
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
    static_assert(quantity_convertible_to<Q2, Derived::quantity> &&
                      quantity_convertible_to<Derived::quantity, Q2>,
                  "Cannot subtract quantities of different kinds");
    if (lhs.reference_ != U2.reference) {
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
    return quantity_holder<Derived::quantity * Q2, result_type>(
        lhs.get_value() * rhs.get_value(),
        lhs.multiplier_ * rhs.get_multiplier());
  }

  template <auto U2, auto Q2, typename T2>
  friend constexpr auto operator*(const Derived& lhs,
                                  const quantity_value<U2, Q2, T2>& rhs) {
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity * Q2, result_type>(
        lhs.get_value() * rhs.get_value(),
        lhs.multiplier_ * U2.get_multiplier());
  }

  template <typename T2>
    requires requires(Derived lhs, T2 rhs) { lhs.get_value() * rhs; } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator*(const Derived& lhs, const T2& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs.get_value() * rhs)>;
    return quantity_holder<Derived::quantity, result_type>(
        lhs.get_value() * rhs, lhs.multiplier_);
  }

  template <typename T2>
    requires requires(T2 lhs, Derived rhs) { lhs * rhs.get_value(); } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator*(const T2& lhs, const Derived& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs * rhs.get_value())>;
    return quantity_holder<Derived::quantity, result_type>(
        lhs * rhs.get_value(), rhs.multiplier_);
  }

  template <auto Q2, typename T2>
  friend constexpr auto operator/(const Derived& lhs,
                                  const quantity_holder<Q2, T2>& rhs) {
    using result_type =
        std::remove_cvref_t<decltype(lhs.get_value() * rhs.get_value())>;
    return quantity_holder<Derived::quantity / Q2, result_type>(
        lhs.get_value() / rhs.get_value(),
        lhs.multiplier_ / rhs.get_multiplier());
  }

  template <typename T2>
    requires requires(Derived lhs, T2 rhs) { lhs.get_value() / rhs; } &&
             (!is_quantity_holder_v<T2> && !quantity_value_like<T2>)
  friend constexpr auto operator/(const Derived& lhs, const T2& rhs) {
    using result_type = std::remove_cvref_t<decltype(lhs.get_value() / rhs)>;
    return quantity_holder<Derived::quantity, result_type>(
        lhs.get_value() / rhs, lhs.multiplier_);
  }
};
} // namespace _detail
/// \endcond

/// \brief Holds a numerical value associated with a physical quantity.
MODULE_EXPORT template <auto Q, typename T>
  requires quantity<decltype(Q)>
class quantity_holder
    : _detail::quantity_holder_arithmetic_operators<quantity_holder<Q, T>> {
public:
  /// The type of the numerical value of the \c quantity_holder.
  using value_type = T;
  /// The type of the quantity of the \c quantity_holder.
  using quantity_type = std::remove_cv_t<decltype(Q)>;
  /// The quantity of the \c quantity_holder.
  static constexpr ::maxwell::quantity auto quantity = Q;

  /// \brief Constructor
  ///
  /// Constructs a \c quantity_holder whose underlying value is
  /// default-initialized in the specified units.
  ///
  /// \param units The units to use for the \c quantity_holder.
  /// \throw Any exceptions thrown by the default constructor of \c T.
  constexpr explicit quantity_holder(unit auto units) noexcept(
      std::is_nothrow_default_constructible_v<T>)
    requires std::is_default_constructible_v<T>;

  /// \brief Constructor
  ///
  /// Constructs a \c quantity_holder whose underlying value is direct non-list
  /// initialized from the specified value as if by \c std::forward<Up>(u). The
  /// units of value stored by the \c quantity_holder are the specified units.
  ///
  /// \tparam Up The type of the value used to initialize the \c
  /// quantity_holder.
  /// \param u The value used to initialize the \c quantity_holder.
  /// \param units The units to use for the \c quantity_holder.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_holder_v<Up>) &&
             (!_detail::quantity_value_like<Up> && !unit<Up>)
  constexpr quantity_holder(unit auto units, Up&& u);

  /// \brief Constructor
  ///
  /// Constructs a \c quantity_holder whose underlying value is direct non-list
  /// initialized from the specified value as if by \c std::forward<Up>(u). The
  /// units of value stored by the \c quantity_holder are the units represented
  /// by the specified multiplier and reference from the base units of the \c
  /// quantity_holder's quantity.
  ///
  /// \tparam Up The type of the value used to initialize the \c
  /// quantity_holder.
  /// \param u The value used to initialize the \c quantity_holder.
  /// \param multiplier The multiplier from the base units of the \c
  /// quantity_holder's quantity.
  /// \param reference The reference from the base units of the \c
  /// quantity_holder's quantity.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename Up = T>
    requires std::constructible_from<T, Up> &&
             (!_detail::is_quantity_holder_v<Up>) &&
             (!_detail::quantity_value_like<Up> && !unit<Up>)
  constexpr quantity_holder(Up&& u, double multiplier, double reference);

  /// \brief Constructor
  ///
  /// Constructs a \c quantity_holder whose underlying value is direct non-list
  /// initialized from the specified arguments as if by \c
  /// std::forward<Args>(args).... The units of value stored by the \c
  /// quantity_holder are the specified units.
  ///
  /// \pre <tt>std::constructible_from<T, Args...></tt> is modeled.
  ///
  /// \tparam Args The types of the arguments used to construct the numerical
  /// value of the quantity.
  /// \param units The units to use for the \c quantity_holder.
  /// \param args The arguments used to construct the numerical value of the
  /// quantity.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename... Args>
    requires std::constructible_from<T, Args...>
  constexpr quantity_holder(unit auto units, std::in_place_t, Args&&... args);

  /// \brief Constructor
  ///
  /// Constructs a \c quantity_holder whose underlying value is direct non-list
  /// initialized from the specified arguments. The units of value stored by the
  /// \c quantity_holder are the specified units.
  ///
  /// \pre <tt>std::constructible_from<T, std::initialize_list<U>&,
  /// Args...></tt> is modeled.
  ///
  /// \tparam Args The types of the arguments used to construct the numerical
  /// value of the quantity.
  /// \param units The units to use for the \c quantity_holder.
  /// \param args The arguments used to construct the numerical value of the
  /// quantity.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename U, typename... Args>
    requires std::constructible_from<T, std::initializer_list<U>&, Args...>
  constexpr quantity_holder(unit auto units, std::in_place_t,
                            std::initializer_list<U> il, Args&&... args);

  /// \brief Constructor
  ///
  /// Constructs a \c quantity_holder from a
  /// \c std::chrono::duration. The units of the \c quantity_holder are the same
  /// as the units of the specified duration type.
  ///
  /// \pre <tt>std::constructible_from_v<T, Rep></tt> is modeled.
  /// \pre \c enable_chrono_conversions_v<Q> is \c true.
  ///
  /// \tparam Rep The representation type of the \c std::chrono::duration
  /// \tparam Period The period type of the \c std::chrono::duration
  /// \param d The \c std::chrono::duration used to construct the \c
  /// quantity_holder.
  /// \throw Any exceptions thrown by the selected constructor of \c T.
  template <typename Rep, typename Period>
    requires std::constructible_from<T, Rep> && enable_chrono_conversions_v<Q>
  constexpr explicit quantity_holder(
      const std::chrono::duration<Rep, Period>& d);

  /// \brief Constructor
  /// Constructs a \c quantity_holder from a \c quantity_value. The units of the
  /// \c quantity_holder are the same as the units of the specified \c
  /// quantity_value.
  ///
  /// \pre <tt>quantity_convertible_to<FromQuantity, quantity></tt> is modeled.
  ///
  /// \tparam FromQuantity The quantity of the specified \c quantity_value
  template <auto FromQuantity, auto FromUnit, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(quantity_value<FromUnit, FromQuantity, Up> other);

  template <auto FromQuantity, typename Up = T>
    requires std::constructible_from<T, Up>
  constexpr quantity_holder(quantity_holder<FromQuantity, Up> other);

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns a constant lvalue-reference to the numerical value of the \c
  /// quantity_holder instance.
  ///
  /// \return A constant lvalue-reference to the numerical value of the \c
  /// quantity_holder instance.
  constexpr auto get_value() const& noexcept -> const T&;

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns an rvalue-reference to the numerical value of the \c
  /// quantity_holder instance.
  ///
  /// \return An rvalue-reference to the numerical value of the \c
  /// quantity_holder instance.
  constexpr auto get_value() && noexcept -> T&&;

  /// \brief Returns the numerical value of the quantity.
  ///
  /// Returns a constant rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  ///
  /// \return A constant rvalue-reference to the numerical value of the \c
  /// quantity_value instance.
  constexpr auto get_value() const&& noexcept -> const T&&;

  /// \brief Returns the multiplier of the quantity holder.
  ///
  /// \return The multiplier of the quantity holder.
  constexpr auto get_multiplier() const noexcept -> double;

  /// \brief Returns the reference of the quantity holder.
  ///
  /// \return The reference of the quantity holder.
  constexpr auto get_reference() const noexcept -> double;

  /// \brief Converts the quantity holder to the specified unit.
  ///
  /// Converts the \c quantity_holder to a \c quantity_value expressed in the
  /// specified units.
  ///
  /// \tparam ToUnit The unit to convert to.
  /// \return A \c quantity_value representing the quantity in the specified
  /// unit.
  template <auto ToUnit>
  constexpr auto as() const -> quantity_value<ToUnit, Q, T>;

  constexpr auto in_base_units() const -> quantity_holder<Q, T>;

  constexpr explicit(!quantity_convertible_to<Q, number> || Q.derived)
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
