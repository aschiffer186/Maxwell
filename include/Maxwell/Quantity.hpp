/// \file Quantity.hpp
/// \author Alex Schiffer
/// \brief Definition of Quantity class template
/// \version 0.1
/// \date 2024-11-02
///
/// \copyright Copyright (c) 2024
#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <cctype>
#include <chrono>
#include <compare>
#include <concepts>
#include <format>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <ostream>
#include <string_view>
#include <type_traits>
#include <utility>

#include "Unit.hpp"
#include "internal/Concepts.hpp"
#include "internal/Config.hpp"
#include "internal/Measure.hpp"

/// \namespace maxwell
namespace maxwell
{
template <typename T, unit auto U>
class basic_quantity;

/// \cond
namespace internal::_detail
{
template <ratio_like FromPeriod, ratio_like ToPeriod>
consteval bool implicit_from_chrono_conversion() noexcept
{
    // TODO: Implement
    return std::ratio_divide<FromPeriod, ToPeriod>::den == 1;
}

template <ratio_like FromPeriod, ratio_like ToPeriod>
consteval bool implicit_to_chrono_conversion() noexcept
{
    return std::ratio_divide<FromPeriod, ToPeriod>::den == 1;
}

template <typename D, typename M, auto U>
concept enable_implicit_from_chrono =
    unit<decltype(U)> && chrono_duration<D> &&
    (std::chrono::treat_as_floating_point_v<M> ||
     implicit_from_chrono_conversion<typename D::period, typename decltype(decltype(U)::time)::scale>()) &&
    std::constructible_from<M, typename D::rep>;

template <typename D, typename M, auto U>
concept enable_implicit_to_chrono =
    unit<decltype(U)> && chrono_duration<D> &&
    (std::chrono::treat_as_floating_point_v<typename D::rep> ||
     implicit_to_chrono_conversion<typename decltype(decltype(U)::time)::scale, typename D::period>()) &&
    std::constructible_from<typename D::rep, M>;

template <typename D, unit auto U>
    requires time_unit<U>
constexpr double from_chrono_conversion_factor()
{
    // TODO: Finish
    using Period = D::period;
    const unit_type<null_measure, null_measure, null_measure, null_measure, null_measure, null_measure,
                    measure_type<1, 0, Period>{}>
        as_maxwell_unit;
    return conversion_factor(as_maxwell_unit, U);
}

template <typename D, unit auto U>
    requires time_unit<U>
constexpr double to_chrono_conversion_factor()
{
    using Period = D::period;
    const unit_type<null_measure, null_measure, null_measure, null_measure, null_measure, null_measure,
                    measure_type<1, 0, Period>{}>
        as_maxwell_unit;
    return conversion_factor(U, as_maxwell_unit);
}

template <typename>
struct is_basic_quantity : std::false_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<basic_quantity<M, U>> : std::true_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<const basic_quantity<M, U>> : std::true_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<volatile basic_quantity<M, U>> : std::true_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<const volatile basic_quantity<M, U>> : std::true_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<basic_quantity<M, U>&> : std::true_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<const basic_quantity<M, U>&> : std::true_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<basic_quantity<M, U>&&> : std::true_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<const basic_quantity<M, U>&&> : std::true_type
{
};

} // namespace internal::_detail
/// \endcond

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
/// Mandates: \c T is not cv-qualified, not \c std::in_place_t, not a specialization of \c std::chrono::duration,
/// not a specialization of \c basic_quantity, and is not a specialization of \c unit_type or \c measure_type
///
/// \tparam T the type of the quantity's magnitude
/// \tparam U the quantity's units
template <typename T, unit auto U>
class basic_quantity
{
    static_assert(!std::is_void_v<T>, "Magnitude cannot be void");
    static_assert(!internal::similar<T, std::in_place_t>, "Magnitude cannot be std::in_place_t");
    static_assert(!internal::_detail::chrono_duration<T>, "Magnitude cannot be a std::chrono::duration");
    static_assert(!internal::_detail::is_basic_quantity<T>::value, "Magnitude cannot be a basic quantity!");
    static_assert(!unit<T>, "Magnitude cannot be a unit");
    static_assert(!internal::measure<T>, "Magnitude cannot be a measure");

    template <typename D>
    static constexpr bool implicit_from_chrono = internal::_detail::enable_implicit_from_chrono<D, T, U>;

  public:
    /// The type of the quantity's magnitude
    using magnitude_type        = T;
    /// The type of the quantity's units
    using units_type            = decltype(U);
    /// The quantity's units
    static constexpr auto units = U;

    // --- Constructors ---

    /// \brief Default constructor
    ///
    /// Creates a \c Quantity whose magnitude is default constructed.
    ///
    /// \pre \c magnitude_type is default constructible
    /// \post <tt> this->magnitude()</tt> is equal to \c magnitude_type{}
    ///
    /// \throws any exceptions thrown by the default constructor of \c magnitude_type
    constexpr basic_quantity()
        requires std::constructible_from<magnitude_type>
    = default;

    /// \brief Constructor
    ///
    /// Constructs a \c Quantity whose magnitude is equal to the specified value.
    /// This constructor is explicit if the \c Quantity is not unitless.
    ///
    /// \pre \c magnitude_type is move constructible
    /// \post <tt>this->magnitude()</tt> is equal to \c magnitude
    ///
    /// \throws Any exceptions thrown by the move constructor of \c magnitude_type
    constexpr explicit(!unitless_unit<units>)
        basic_quantity(magnitude_type magnitude) noexcept(std::is_nothrow_move_constructible_v<magnitude_type>)
        requires std::move_constructible<magnitude_type>
        : magnitude_(std::move(magnitude))
    {
    }

    /// \brief Constructor
    ///
    /// Constructs a \c basic_quantity whose magnitude is constructed using the value \c std::forward<Up>(magnitude)
    ///
    /// \pre \c magnitude_type is constructible from \c Up&&
    /// \post \c this->magnitude() is equal to \c magnitude_type(std::forward<Up>(magnitude))
    ///
    /// \throws Any exceptions thrown by the constructor of \c magnitude_type
    template <typename Up>
        requires(!internal::_detail::chrono_duration<Up>) && std::constructible_from<magnitude_type, Up> &&
                (!internal::_detail::is_basic_quantity<std::remove_cvref_t<Up>>::value)
    constexpr explicit(!unitless_unit<units>)
        basic_quantity(Up&& magnitude) noexcept(std::is_nothrow_constructible_v<magnitude_type, Up>)
        : magnitude_(std::forward<Up>(magnitude))
    {
    }

    /// \c Constructor
    ///
    /// Constructs a \c basic_quantity whose magnitude s constructed in place from the specified arguments using
    /// the expression \c std::forward<Args>(args)...
    ///
    /// \pre \c magnitude_type is constructible from \c Args
    /// \post \c this->magnitude() is equal to \c magnitude_type(std::forward<Args>(args)...)
    ///
    /// \throws Any exceptions thrown by the constructor of \c magnitude_type
    template <typename... Args>
        requires(std::constructible_from<magnitude_type, Args && ...>)
    constexpr explicit basic_quantity(std::in_place_t, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<magnitude_type, Args&&...>)
        : magnitude_(std::forward<Args>(args)...)
    {
    }

    /// \c Constructor
    ///
    /// Constructs a \c basic_quantity whose magnitude s constructed in place from the specified initializer list
    /// and arguments arguments using
    /// the expression <tt>magnitude_type(il, std::forward<Args>(args)...)</tt>
    ///
    /// \pre \c magnitude_type is constructible from \c Args
    /// \post \c this->magnitude() is equal to <tt>magnitude_type(il, std::forward<Args>(args)...)</t>>
    ///
    /// \throws Any exceptions thrown by the constructor of \c magnitude_type
    template <typename Up, typename... Args>
        requires std::constructible_from<magnitude_type, std::initializer_list<Up>, Args...>
    constexpr explicit basic_quantity(std::in_place_t, std::initializer_list<Up> il, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<magnitude_type, std::initializer_list<Up>, Args...>)
        : magnitude_(il, std::forward<Args>(args)...)
    {
    }

    /// \brief Constructor
    ///
    /// Constructs a \c basic_quantity from a \c std::chrono::duration type, allowing for integration with the
    /// standard library. This constructor is implicit if there would be no loss of information converting from the
    /// specified \c std::chrono::duration type.
    ///
    /// \pre \c magnitude_type is constructible from \c Rep
    /// \pre \c units has dimensions of time
    /// \post \c this->magnitude() is equal to \c dur.count()
    ///
    /// \param dur the \c std::chrono::duration value to construct from
    ///
    /// \throw any exceptions thrown by \c dur or by the constructor of \c magnitude_type
    template <typename Rep, typename Period>
        requires time_unit<units> && std::constructible_from<magnitude_type, Rep>
    MAXWELL_CONSTEXPR23 explicit(!implicit_from_chrono<std::chrono::duration<Rep, Period>>)
        basic_quantity(std::chrono::duration<Rep, Period> dur)
        : magnitude_(dur.count() *
                     internal::_detail::from_chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>())
    {
    }

    /// \brief Converting constructor
    ///
    /// Constructs a \c basic_quantity from the specified \c basic_quantity with different units, automatically
    /// converting the units of the specified quantity to \c Units. The magnitude of \c *this is copy constructed
    /// from \c q.magnitude() then multiplied by the appropriate conversion factor between \c Other and \c Units.
    ///
    /// \pre \c magnitude_type is constructible from \c Up
    /// \pre \c Other is convertible to \c Units.
    /// \post <tt>this->magnitude() == conversion_factor(Other, Units)*q.magnitude()</tt>
    ///
    /// \tparam Up the type of the magnitude of the \c basic_quantity to construct from
    /// \tparam Other the units ofthe \c basic_quantity to construct from
    /// \param q the \c basic_quantity to construct from
    ///
    /// \throw any exceptions thrown by the move constructor of \c magnitude_type
    template <typename Up, unit auto Other>
        requires std::constructible_from<magnitude_type, Up> &&
                 (!(std::same_as<Up, magnitude_type> && Other == U)) && unit_convertible_to<Other, units>
    constexpr basic_quantity(const basic_quantity<Up, Other>& q) noexcept(
        std::is_nothrow_constructible_v<magnitude_type, Up>)
        : magnitude_(q.magnitude() * conversion_factor(Other, units))
    {
    }

    /// \brief Converting constructor
    ///
    /// Constructs a \c basic_quantity from the specified \c basic_quantity with different units, automatically
    /// converting the units of the specified quantity to \c Units. The magnitude of \c *this is move constructed
    /// from \c q.magnitude() then multiplied by the appropriate conversion factor between \c Other and \c Units.
    ///
    /// \pre \c magnitude_type is constructible from \c Up
    /// \pre \c Other is convertible to \c Units.
    /// \post <tt>this->magnitude() == conversion_factor(Other, Units)*q.magnitude()</tt>
    ///
    /// \tparam Up the type of the magnitude of the \c basic_quantity to construct from
    /// \tparam Other the units ofthe \c basic_quantity to construct from
    /// \param q the \c basic_quantity to construct from
    ///
    /// \throw any exceptions thrown by the move constructor of \c magnitude_type
    template <typename Up, unit auto Other>
        requires std::constructible_from<magnitude_type, std::add_rvalue_reference_t<Up>> &&
                 (!(std::same_as<Up, magnitude_type> && Other == U)) && unit_convertible_to<Other, units>
    constexpr basic_quantity(basic_quantity<Up, Other>&& q) noexcept(
        std::is_nothrow_constructible_v<magnitude_type, std::add_rvalue_reference_t<Up>>)
        : magnitude_(std::move(q).magnitude() * conversion_factor(Other, units))
    {
    }

    /// \brief Converting assignment operator
    ///
    /// Converts the magitude of \c q from \c Other to \c Units then assigns the value to the magnitude
    /// of \c *this.
    ///
    /// \pre \c Up is swappable with \c magnitude_type
    /// \pre \c Other is convertible to \c units
    /// \post <tt>this->magnitude() == conversion_factor(Other, Units)*q.magnitude()</tt>
    ///
    /// \tparam Up the type of the magnitude of the \c basic_quantity being assigned from
    /// \tparam Other the units of the \c basic_quantity being assigned from
    /// \param q the \c basic_quantity to assign to \c *this
    ///
    /// \return a reference to \c *this
    ///
    /// \throws any exceptions through by swapping \c Up and \c magnitude_type
    template <std::swappable_with<magnitude_type> Up, unit auto Other>
        requires unit_convertible_to<Other, units> &&
                 std::constructible_from<magnitude_type, std::add_rvalue_reference_t<Up>>
    constexpr basic_quantity& operator=(basic_quantity<Up, Other> q)
    {
        basic_quantity temp(std::move(q));
        (*this).swap(temp);
        return *this;
    }

    /// \brief Converting assignment operator
    ///
    /// Converts the magitude of \c dur from the period specified by the \c std::chrono::duration to
    /// \c Units
    ///
    /// \pre \c magnitude_type is constructible from \c Rep
    /// \pre \c time_unit<units>
    /// \post <tt>this->magnitude() == conversion_factor(Other, Units)*dur.coun</tt>
    ///
    /// \tparam Rep the representation type of the \c std::chrono::duration
    /// \tparam Period the period of the \c std::chrono::duration
    /// \param dur the \c std::chrono::duration to assign to \c *this
    ///
    /// \return a refeence to \c *this
    ///
    /// \throws any exceptions thrown by the copy constructor of \c dur
    template <typename Rep, typename Period>
        requires time_unit<units> && std::constructible_from<magnitude_type, Rep>
    MAXWELL_CONSTEXPR23 basic_quantity& operator=(std::chrono::duration<Rep, Period> dur)
    {
        basic_quantity temp{dur};
        (*this).swap(temp);
        return *this;
    }

    /// \brief Assignment operator
    ///
    /// Assigns the specified value to the magnitude of \c *this. This operator only applies to
    /// unitless quantities.
    ///
    /// \pre \c magnitude_type is assignable from \c Up
    /// \pre \c *this is unitless
    ///
    /// \tparam Up the the type of the value to assign to \c *this
    /// \param up the value to assign to \c *this
    ///
    /// \return a reference to \c *this
    template <typename Up>
        requires std::assignable_from<std::add_rvalue_reference_t<Up>, magnitude_type> && unitless_unit<units>
    constexpr basic_quantity& operator=(Up&& up) noexcept(
        std::is_nothrow_assignable_v<std::add_rvalue_reference_t<Up>, magnitude_type>)
    {
        magnitude_ = std::forward(up);
        return *this;
    }

    // --- Core Language Functions ---
    /// \brief Swaps two quantities
    ///
    /// Swaps the magnitude of two quantities.
    ///
    /// \post \c other has the old magnitude of \c *this and \c *this has the old magnitude of \c other
    ///
    /// \throws Any exceptions thrown by swapping the magnitudes of the quantities
    constexpr void swap(basic_quantity& other) noexcept(std::is_nothrow_swappable_v<magnitude_type>)
    {
        using std::swap;
        swap(other.magnitude_, magnitude_);
    }

    /// \brief Compares two quantities for equality
    ///
    /// Checks if two \c basic_quantities are equal. Two quantities are equal if and only if their
    /// magnitudes are equal. Note, if the magnitude type is a floating-point type,
    /// this operator will use floating-point equality, which is most likely undesired
    /// and can lead to false negatives.
    ///
    /// \pre the magitude types of the quantities are equality comparable
    ///
    /// \param a one quantity to check for equality
    /// \param b the other quantity to check for equality
    /// \return \c true if the magnitudes of the quantities are the same
    friend bool operator==(const basic_quantity& a, const basic_quantity& b)
        requires std::equality_comparable<magnitude_type>
    = default;

    /// \brief Quantity three way comparison operator
    ///
    /// Three way comparison operator for \c basic_quantities.
    ///
    /// \pre The magnitude type is three_way_comparable
    ///
    /// \param a one quantity to use in the three way comparison
    /// \param b the other quantity to use in the three way comparison
    /// \return <tt>a.magnitude() <=> b.magnitude()</tt>
    friend auto operator<=>(const basic_quantity& a, const basic_quantity& b)
        requires std::three_way_comparable<magnitude_type>
    = default;

    // --- Accessor Functions ---

    /// \brief Returns the magnitude of the \c *this
    ///
    /// \return A \c const reference to the magnitude of \c *this
    constexpr const magnitude_type& magnitude() const& noexcept
    {
        return magnitude_;
    }

    /// \brief Returns the magnitude of the \c *this
    ///
    /// \return An rvalue reference to the magnitude of \c *this
    constexpr magnitude_type&& magnitude() && noexcept
    {
        return std::move(magnitude_);
    }

    /// \brief Returns the magnitude of the \c *this
    ///
    /// \return A \c const rvalue reference to the magnitude of \c *this
    constexpr const magnitude_type&& magnitude() const&& noexcept
    {
        return std::move(magnitude_);
    }

    /// \brief Returns the units of \c *this
    ///
    /// \return The units of \c *this
    constexpr unit auto get_units() const noexcept
    {
        return units;
    }

    // --- Transformation Functions ---
    /// \brief Returns a new \c basic_quantity with the same dimensions of \c *this but in SI base units
    ///
    /// Returns a new \c basic_quantity with the same dimension of \c *this, but in SI base units. The magnitude of
    /// the returned quantity is equivalent to \c this->magnitude() converted to SI base units
    ///
    /// \return A new \c basic_quantity with the same dimension of \c *this in SI base units.
    constexpr auto to_SI_base_units() const noexcept
    {
        basic_quantity<magnitude_type, units.to_SI_base_units()> temp(*this);
        return temp;
    }

    // --- Conversion Functions ---
    /// \brief Conversion operator to underlying magnitude type
    ///
    /// Converts the quantity to the underlying type. This operation is implicit
    /// if the quantity is unitless. It is highly recommended to only call this operator
    /// if the quantity is unitless.
    ///
    /// \return the underlying magnitude of the \c basic_quantity
    constexpr explicit(!unitless_unit<units>) operator magnitude_type() const
    {
        return magnitude_;
    }

    template <typename Rep, typename Period>
        requires internal::multiply_enabled_with<magnitude_type, Rep>
    MAXWELL_CONSTEXPR23 explicit(
        !internal::_detail::enable_implicit_to_chrono<std::chrono::duration<Rep, Period>, magnitude_type,
                                                      units>) operator std::chrono::duration<Rep, Period>() const
        requires time_unit<units>
    {
        return std::chrono::duration<Rep, Period>{static_cast<Rep>(
            magnitude_ * internal::_detail::to_chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>())};
    }

    // --- Arithmetic Functions ---
    constexpr basic_quantity& operator+=(const basic_quantity& other) noexcept(
        internal::nothrow_add_enabled<magnitude_type>)
        requires internal::add_enabled<magnitude_type>
    {
        magnitude_ = magnitude_ + other.magnitude();
        return *this;
    }

    constexpr basic_quantity& operator-=(const basic_quantity& other) noexcept(
        internal::nothrow_subtract_enabled<magnitude_type>)
        requires internal::subtract_enabled<magnitude_type>
    {
        magnitude_ = magnitude_ - other.magnitude();
        return *this;
    }

    template <typename Up, unit auto Other>
        requires unit_convertible_to<Other, units> && internal::add_enabled_with<magnitude_type, Up>
    constexpr basic_quantity& operator+=(const basic_quantity<Up, Other>& other) noexcept(
        internal::nothrow_add_enabled_with<magnitude_type, Up>)
    {
        constexpr double conversion = conversion_factor(Other, units);
        magnitude_                  = magnitude_ + other.magnitude() * conversion;
        return *this;
    }

    template <typename Up, unit auto Other>
        requires unit_convertible_to<Other, units> && internal::subtract_enabled_with<magnitude_type, Up>
    constexpr basic_quantity& operator-=(const basic_quantity<Up, Other>& other) noexcept(
        internal::nothrow_subtract_enabled_with<magnitude_type, Up>)
    {
        constexpr double conversion = conversion_factor(Other, units);
        magnitude_                  = magnitude_ - other.magnitude() * conversion;
        return *this;
    }

    template <typename Up>
        requires internal::multiply_enabled_with<magnitude_type, Up>
    constexpr basic_quantity& operator*=(const Up& value) noexcept(
        internal::nothrow_multiply_enabled_with<magnitude_type, Up>)
    {
        magnitude_ = magnitude_ * value;
        return *this;
    }

    template <typename Up>
        requires internal::divide_enabled_with<magnitude_type, Up>
    constexpr basic_quantity& operator/=(const Up& value) noexcept(
        internal::nothrow_divide_enabled_with<magnitude_type, Up>)
    {
        magnitude_ = magnitude_ / value;
        return *this;
    }

    template <typename Up>
        requires internal::modulo_enabled_with<magnitude_type, Up>
    constexpr basic_quantity& operator%=(const Up& value) noexcept(
        internal::nothrow_modulo_enabled_with<magnitude_type, Up>)
    {
        magnitude_ = magnitude_ % value;
        return *this;
    }

    /// \brief Pre-increment operator
    ///
    /// Increments the underlying magnitude by 1 and
    /// returns a reference to the modified \c basic_quantity
    ///
    /// \return a reference to \c *this after the modification
    constexpr basic_quantity& operator++()
    {
        ++magnitude_;
        return *this;
    }

    /// \brief Post-increment operator
    ///
    /// Increments the underlying magnitude by 1 and
    /// returns a copy of the \c basic_quantity before the
    /// modification
    ///
    /// \return a copy of \c *this prior to the modification
    constexpr basic_quantity operator++(int)
    {
        basic_quantity temp(*this);
        ++(*this);
        return temp;
    }

    constexpr basic_quantity& operator--() noexcept
    {
        --magnitude_;
        return *this;
    }

    constexpr basic_quantity operator--(int) noexcept
    {
        basic_quantity temp(*this);
        --(*this);
        return temp;
    }

  private:
    magnitude_type magnitude_{};
};
// --- CTAD for construction from std::chrono::duration
template <typename Rep, typename Period>
basic_quantity(std::chrono::duration<Rep, Period>) -> basic_quantity<Rep, second_unit>;

/// \brief Convenience type alias for common case where \c magnitude_type is \c double
template <unit auto U>
using quantity = basic_quantity<double, U>;

/// \brief Convenience type alias for \c basic_quantity with \c magnitude_type of \c int
template <unit auto U>
using iquantity = basic_quantity<int, U>;

template <typename M, unit auto U>
constexpr void swap(basic_quantity<M, U>& lhs, basic_quantity<M, U>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

// --- Arithmetic Operators ---
template <typename M1, unit auto U1, typename M2, unit auto U2>
    requires unit_convertible_to<U1, U2> && internal::add_enabled_with<M1, M2>
constexpr auto operator+(basic_quantity<M1, U1>        lhs,
                         const basic_quantity<M2, U2>& rhs) noexcept(internal::nothrow_add_enabled_with<M1, M2>)
{
    return lhs += rhs;
}

template <typename M1, unit auto U1, typename M2, unit auto U2>
    requires unit_convertible_to<U1, U2> && internal::subtract_enabled_with<M1, M2>
constexpr auto operator-(basic_quantity<M1, U1>        lhs,
                         const basic_quantity<M2, U2>& rhs) noexcept(internal::subtract_enabled_with<M1, M2>)
{
    return lhs -= rhs;
}

template <typename M, unit auto U>
constexpr basic_quantity<M, U> operator-(const basic_quantity<M, U>& x)
{
    return basic_quantity<M, U>(-x.magnitude());
}

/// \brief Calculates the product between two \c BasicQuantities
///
/// Calculates the product between two \c BasicQuantities. Both quantities are converted
/// to SI base units prior to the multiplication being performed and the product of the
/// units will be in SI base units
///
/// \pre The mangitude of \c lhs can be multiplied with the magnitude of \c rhs
///
/// \tparam M1 The type of the magnitude of the left hand side of the multiplication
/// \tparam U1 The units of the left hand side of the multiplication
/// \tparam M2 The type of the magnitude of the left hand side of the multiplication
/// \tparam U2 The units of the left hand side of the multiplication
/// \param lhs The left hand side of the multiplication
/// \param rhs The right hand side of the multiplication
/// \return The product of \c lhs and \c rhs
/// \throw Any exception thrown by multiplying \c lhs.magnitude() and \c rhs.magnitude()
template <typename M1, unit auto U1, typename M2, unit auto U2>
    requires internal::multiply_enabled_with<M1, M2>
constexpr auto operator*(const basic_quantity<M1, U1>  lhs,
                         const basic_quantity<M2, U2>& rhs) noexcept(internal::nothrow_multiply_enabled_with<M1, M2>)
    -> basic_quantity<decltype(lhs.get_magnitude() * rhs.get_magnitude()),
                      U1.to_SI_base_units() * U2.to_SI_base_units()>
{
    const auto lhsSIBase = lhs.to_SI_base_units();
    const auto rhsSIBase = rhs.to_SI_base_units();

    constexpr unit auto outputunits = lhsSIBase.units() * rhsSIBase.units();
    return basic_quantity<decltype(lhs.magnitude() * rhs.magnitude()), outputunits>(lhsSIBase.magnitude() *
                                                                                    rhsSIBase.magnitude());
}

/// \brief Calculates the quotient between two \c BasicQuantities
///
/// Calculates the quotient between two \c BasicQuantities. Both quantities are converted
/// to SI base units prior to the divison being performed and the quotient of the
/// units will be in SI base units
///
/// \pre The mangitude of \c lhs can be divided by the magnitude of \c rhs
///
/// \tparam M1 The type of the magnitude of the left hand side of the division
/// \tparam U1 The units of the left hand side of the division
/// \tparam M2 The type of the magnitude of the left hand side of the division
/// \tparam U2 The units of the left hand side of the division
/// \param lhs The left hand side of the division
/// \param rhs The right hand side of the division
/// \return The quotient of \c lhs and \c rhs
/// \throw Any exception thrown by dividing \c lhs.magnitude() by \c rhs.magnitude()
template <typename M1, unit auto U1, typename M2, unit auto U2>
    requires internal::multiply_enabled_with<M1, M2>
constexpr auto operator/(const basic_quantity<M1, U1>& lhs,
                         const basic_quantity<M2, U2>& rhs) noexcept(internal::nothrow_multiply_enabled_with<M1, M2>)
    -> basic_quantity<decltype(lhs.magnitude() / rhs.magnitude()), U1.toSIBaseunits() / U2.toSIBaseunits()>
{
    const auto lhsSIBase = lhs.to_SI_base_units();
    const auto rhsSIBase = rhs.to_SI_base_units();

    constexpr unit auto outputunits = lhsSIBase.units() / rhsSIBase.units();
    return basic_quantity<decltype(lhs.magnitude() / rhs.magnitude()), outputunits>(lhsSIBase.magnitude() *
                                                                                    rhsSIBase.magnitude());
}

template <typename M1, unit auto U1, typename M2>
    requires internal::multiply_enabled_with<M1, M2> && (!unitless_unit<U1>)
constexpr auto operator*(const basic_quantity<M1, U1>& lhs, const M2& rhs) noexcept(
    internal::nothrow_multiply_enabled_with<M1, M2>) -> basic_quantity<decltype(lhs.magnitude() * rhs), U1>
{
    return basic_quantity<decltype(lhs.magnitude() * rhs), U1>(lhs.magnitude() * rhs);
}

template <typename M1, unit auto U1, typename M2>
    requires internal::multiply_enabled_with<M1, M2> && (!unitless_unit<U1>)
constexpr auto operator*(const M2& lhs, const basic_quantity<M1, U1>& rhs) noexcept(
    internal::nothrow_multiply_enabled_with<M1, M2>) -> basic_quantity<decltype(lhs * rhs.magnitude()), U1>
{
    return rhs * lhs;
}

template <typename M1, unit auto U1, typename M2>
    requires internal::multiply_enabled_with<M1, M2>
constexpr auto operator/(const basic_quantity<M1, U1>& lhs, const M2& rhs) noexcept(
    internal::nothrow_multiply_enabled_with<M1, M2>) -> basic_quantity<decltype(lhs.magnitude() / rhs), U1>
{
    return basic_quantity<decltype(lhs.magnitude() / rhs), U1>(lhs.magnitude() * rhs);
}

template <typename M1, unit auto U1, typename M2>
    requires internal::multiply_enabled_with<M1, M2>
constexpr auto operator/(const M2& lhs, const basic_quantity<M1, U1>& rhs) noexcept(
    internal::nothrow_multiply_enabled_with<M1, M2>) -> basic_quantity<decltype(lhs / rhs.magnitude()), U1>
{
    return basic_quantity<decltype(lhs.magnitude() / rhs), U1>(lhs.magnitude() * rhs);
}

// --- Comparison ---
/// \brief Compares two quantities for equality
///
/// Checks if two \c basic_quantities are equal. Two quantities are equal if and only if their
/// magnitudes, when converted to SI base units, are equal. Note, if the magnitude type is a floating-point type,
/// this operator will use floating-point equality, which is most likely undesired
/// and can lead to false negatives.
///
/// \pre the magitude types of the quantities are equality comparable
///
/// \param lhs one quantity to check for equality
/// \param rhs the other quantity to check for equality
/// \return \c true if the magnitudes of the quantities are the same
template <typename M, unit auto U1, unit auto U2>
    requires std::equality_comparable<M>
constexpr bool operator==(const basic_quantity<M, U1>& lhs,
                          const basic_quantity<M, U2>& rhs) noexcept(noexcept(lhs.magnitude() == rhs.magnitude()))
{
    return lhs.to_SI_base_units().magnitude() == rhs.to_SI_base_units().magnitude();
}

/// \brief Quantity three way comparison operator
///
/// Three way comparison operator for \c basic_quantities. Behaves as if
/// the quantities were converted to SI base units prior to the comparison.
///
/// \pre The magnitude type is three_way_comparable
///
/// \param rhs one quantity to use in the three way comparison
/// \param lhs the other quantity to use in the three way comparison
/// \return <tt>lhs.to_SI_base_units().magnitude() <=> rhs.to_SI_base_units().magnitude()</tt>
template <typename M, unit auto U1, unit auto U2>
    requires std::three_way_comparable<M>
constexpr std::compare_three_way_result_t<M> operator<=>(
    const basic_quantity<M, U1>& lhs,
    const basic_quantity<M, U2>& rhs) noexcept(noexcept(lhs.magnitude() <=> rhs.magnitude()))
{
    return lhs.to_SI_base_units().magnitude() <=> rhs.to_SI_base_units().magnitude();
}

// --- Formatting ---
template <typename M, unit auto U>
std::ostream& operator<<(std::ostream& os, const basic_quantity<M, U>& q)
{
#ifdef MAXWELL_USE_PRINT
    std::print(os, q);
#else
    os << std::format("{}", q);
#endif
    return os;
}

/// \brief Converts a quantity to another quantity
///
/// Converts a quantity to a quantity in different units as
/// long as \c From can be converted a quantity with the
/// units of \c Target.
///
/// \pre \c From is convertible to \c To
///
/// \tparam Target the type of the quantity to convert to
/// \tparam From the type of the quantity to convert from
/// \param from the quantity to convert from
/// \return the converted quantity
template <typename Target, typename From>
    requires internal::_detail::is_basic_quantity<Target>::value && internal::_detail::is_basic_quantity<From>::value
constexpr Target quantity_cast(const From& from)
{
    return Target(from);
}

// --- Quantity Type Traits ---
/// \brief Specifies a quantity has dimensions of amount
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept amount = amount_unit<QuantityType::units>;

/// \brief Specifies a quantity has dimensions of current
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept current = current_unit<QuantityType::units>;

/// \brief Specifies a quantity has dimensions of length
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept length = length_unit<QuantityType::units>;

/// \brief Specifies a quantity has dimensions of time
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept luminosity = luminosity_unit<QuantityType::units>;

/// \brief Specifies a quantity has dimensions of mass
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept mass = mass_unit<QuantityType::units>;

/// \brief Specifies a quantity has dimensions of temperature
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept temperature = temperature_unit<QuantityType::units>;

/// \brief Specifies a quantity has dimensions of time
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept time = time_unit<QuantityType::units>;

/// \brief Specifies a quantity is unitless
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept unitless = unitless_unit<QuantityType::units>;
} // namespace maxwell

// --- Specialization of standard library templates ---

/// \brief Specialization of \c std::formatter for \c basic_quantity
///
/// \tparam M The type of the magnitude of the \c basic_quantity
/// \tparam U The units of the \c basic_quantity
template <typename M, maxwell::unit auto U>
struct std::formatter<maxwell::basic_quantity<M, U>>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const maxwell::basic_quantity<M, U>& q, std::format_context& ctx) const
    {
        std::string              temp;
        const maxwell::unit auto actual = q.get_units();
        if (unitsOnly_)
        {
            std::format_to(std::back_inserter(temp), "{}", maxwell::unit_string<actual>);
        }
        else if (dimensionOnly_)
        {
            if constexpr (maxwell::unitless<decltype(q)>)
            {
                std::format_to(std::back_inserter(temp), "{}", "[]");
            }
            else
            {
                std::string dimensionString;
            }
        }
        else
        {
            std::format_to(std::back_inserter(temp), "{} {}", q.magnitude(),
                           maxwell::unit_string<std::remove_cvref_t<decltype(q)>::units>);
        }
        return std::formatter<std::string_view>{}.format(temp, ctx);
    }

  private:
    bool inSIBaseunits_{false};
    bool unitsOnly_{false};
    bool dimensionOnly_{false};
};

/// \brief Specialization of \c std::hash for \c basic_quantity
///
/// Specialization of \c std::hash for \c basic_quantity. Returns the hash value
/// of the magnitude of a quantity after converting to SI base units. This ensures
/// that equal instances of \c basic_quantity have equal hash values.
template <typename M, maxwell::unit auto U>
struct std::hash<maxwell::basic_quantity<M, U>>
{
    size_t operator()(const maxwell::basic_quantity<M, U>& q) noexcept(noexcept(hash<M>{q.magnitude()}))
    {
        hash<M>{}(q.toSIBaseunits().magnitude());
    }
};

/// \brief Specialization of \c std::numeric_limits for \c basic_quantity
///
/// Specialization of \c std::numeric_limits for \c basic_quantity. Provides
/// values equivalent to \c std::numeric_limits<M>.
template <typename M, maxwell::unit auto U>
struct std::numeric_limits<maxwell::basic_quantity<M, U>>
{
  private:
    using BaseType   = std::remove_cv_t<M>;
    using BaseLimits = std::numeric_limits<BaseType>;

  public:
    static constexpr bool is_specialized = true;

    static constexpr bool is_signed = BaseLimits::is_signed;

    static constexpr bool is_integer = BaseLimits::is_integer;

    static constexpr bool is_exact = BaseLimits::is_exact;

    static constexpr bool has_infinity = BaseLimits::has_infinity;

    static constexpr bool has_quiet_NaN = BaseLimits::has_quiet_NaN;

    static constexpr bool has_signaling_NaN = BaseLimits::has_signaling_NaN;

    static constexpr bool has_denorm = BaseLimits::has_denorm;

    static constexpr bool has_denorm_loss = BaseLimits::has_denorm_loss;

    static constexpr std::float_round_style round_style = BaseLimits::round_style;

    static constexpr bool is_iec559 = BaseLimits::is_iec559;

    static constexpr bool is_bounded = BaseLimits::is_bounded;

    static constexpr bool is_modulo = BaseLimits::is_modulo;

    static constexpr int digits = BaseLimits::digits;

    static constexpr int digits10 = BaseLimits::digits10;

    static constexpr int max_digits10 = BaseLimits::max_digits10;

    static constexpr int radix = BaseLimits::radix;

    static constexpr int min_exponent = BaseLimits::min_exponent;

    static constexpr int min_exponent10 = BaseLimits::min_exponent10;

    static constexpr int max_exponent = BaseLimits::max_exponent;

    static constexpr int max_exponent10 = BaseLimits::max_exponent10;

    static constexpr bool traps = BaseLimits::traps;

    static constexpr bool tinyness_before = BaseLimits::tinyness_before;

    static maxwell::basic_quantity<M, U> min() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::min()};
    }

    static maxwell::basic_quantity<M, U> lowest() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::lowest()};
    }

    static maxwell::basic_quantity<M, U> max() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::max()};
    }

    static maxwell::basic_quantity<M, U> epsilon() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::epsilon()};
    }

    static maxwell::basic_quantity<M, U> round_error() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::epsilon()};
    }

    static maxwell::basic_quantity<M, U> infinity() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::infinity()};
    }

    static maxwell::basic_quantity<M, U> quiet_NaN() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::quiet_NaN()};
    }

    static maxwell::basic_quantity<M, U> signaling_NaN() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::signaling_NaN()};
    }

    static maxwell::basic_quantity<M, U> denorm_min() noexcept
    {
        return maxwell::basic_quantity<M, U>{BaseLimits::denorm_min()};
    }
};

template <typename M, maxwell::unit auto U>
struct std::numeric_limits<const maxwell::basic_quantity<M, U>> : std::numeric_limits<maxwell::basic_quantity<M, U>>
{
};

template <typename M, maxwell::unit auto U>
struct std::numeric_limits<volatile maxwell::basic_quantity<M, U>> : std::numeric_limits<maxwell::basic_quantity<M, U>>
{
};

template <typename M, maxwell::unit auto U>
struct std::numeric_limits<const volatile maxwell::basic_quantity<M, U>>
    : std::numeric_limits<maxwell::basic_quantity<M, U>>
{
};
#endif