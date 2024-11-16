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
#include <concepts>
#include <format>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <ostream>
#include <string_view>
#include <type_traits>
#include <utility>

#include "Unit.hpp"
#include "internal/Concepts.hpp"
#include "internal/Measure.hpp"

/// \namespace maxwell
namespace maxwell
{
template <typename T, unit auto U>
class basic_quantity;

/// \cond
namespace internal::_detail
{
template <typename Period, ratio_like Scale>
constexpr bool implicit_unit_conversion() noexcept
{
    return true;
}

template <typename D, typename M, auto U>
concept EnableImplicitFromChrono =
    unit<decltype(U)> && chrono_duration<D> &&
    implicit_unit_conversion<typename D::period, typename decltype(U.get_time())::scale>() &&
    std::convertible_to<typename D::rep, M>;

template <typename D, unit auto U>
    requires time_unit<U>
constexpr double chrono_conversion_factor()
{
    // using Period = D::period;
    if (std::ratio_equal_v<typename decltype(U.get_time())::scale, internal::_detail::one>)
    {
        // Detailing with second, potentially scaled by a metric prefix
        [[maybe_unused]] constexpr std::intmax_t multiplier = U.get_time().get_multiplier();
        return 1.0;
    }
    else
    {
        return 1.0;
    }
}

template <typename D, unit auto U>
    requires time_unit<U>
constexpr double to_chrono_conversion_factor()
{
    // using Period = D::period;
    if (std::ratio_equal_v<typename decltype(U.get_time())::scale, internal::_detail::one>)
    {
        // Detailing with second, potentially scaled by a metric prefix
        [[maybe_unused]] constexpr std::intmax_t multiplier = U.get_time().get_multiplier();
        return 1.0;
    }
    else
    {
        return 1.0;
    }
}

template <typename>
struct is_basic_quantity : std::false_type
{
};

template <typename M, unit auto U>
struct is_basic_quantity<basic_quantity<M, U>> : std::true_type
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
/// \tparam T the type of the quantity's magnitude
/// \tparam U the quantity's units
template <typename T, unit auto U>
class basic_quantity
{
    static_assert(!std::is_const_v<T>, "Magnitude cannot be const");
    static_assert(!std::is_volatile_v<T>, "Magnitude cannot be volatile");
    static_assert(!internal::similar<T, std::in_place_t>, "Magnitude cannot be std::in_place_t");
    static_assert(!internal::_detail::chrono_duration<T>, "Magnitude cannot be a std::chrono::duration");
    static_assert(!internal::_detail::is_basic_quantity<T>::value, "Magnitude cannot be a basic quantity!");

    template <typename D>
    static constexpr bool implicitFromChrono = internal::_detail::EnableImplicitFromChrono<D, T, U>;

  public:
    /// The type of the quantity's magnitude
    using magnitude_type = T;
    /// The type of the quantity's units
    using units_type     = decltype(U);

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
    constexpr explicit(unitless_unit<units>)
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
    constexpr explicit(unitless_unit<units>)
        basic_quantity(Up&& magnitude) noexcept(std::is_nothrow_constructible_v<magnitude_type, Up>)
        : magnitude_(std::forward<Up>(magnitude))
    {
        std::cout << "Single argument template constructor\n";
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
        requires std::constructible_from<std::initializer_list<Up>, Args...>
    constexpr explicit basic_quantity(std::in_place_t, std::initializer_list<Up> il, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<magnitude_type, std::initializer_list<Up>, Args...>)
        : magnitude_(il, std::forward<Args>(args)...)
    {
    }

    /// \brief Constructor
    ///
    /// Constructs a \c basic_quantity from a \c std::chrono::duration type, allowing for integration with the
    /// standard library. This constructor is implicit if there would be a loss of information converting from the
    /// specified \c std::chrono::duration type.
    ///
    /// \pre \c magnitude_type is constructible from \c Rep
    /// \pre \c units has dimensions of time
    /// \post \c this->magnitude() is equal to \c dur.count()
    ///
    /// \throw any exceptions thrown by \c dur or by the constructor of \c magnitude_type
    template <typename Rep, typename Period>
        requires time_unit<units> && std::constructible_from<magnitude_type, Rep>
    constexpr explicit(!implicitFromChrono<std::chrono::duration<Rep, Period>>)
        basic_quantity(std::chrono::duration<Rep, Period> dur)
        : magnitude_(dur.count() *
                     internal::_detail::chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>())
    {
    }

    template <typename Up, unit auto Other>
        requires(std::constructible_from<magnitude_type, std::add_rvalue_reference_t<Up>>) &&
                (!(std::same_as<Up, magnitude_type> && Other == U)) && unit_convertible_to<Other, units>
    constexpr basic_quantity(basic_quantity<Up, Other> q) noexcept(
        std::is_nothrow_constructible_v<magnitude_type, std::add_rvalue_reference_t<Up>>)
        : magnitude_(q.magnitude() * conversion_factor(Other, units))
    {
    }

    template <typename Up, unit auto Other>
        requires(std::constructible_from<magnitude_type, std::add_rvalue_reference_t<Up>>) &&
                unit_convertible_to<Other, units>
    constexpr basic_quantity& operator=(basic_quantity<Up, Other> q) noexcept(
        std::is_nothrow_constructible_v<magnitude_type, std::add_rvalue_reference_t<Up>>)
    {
        (*this).swap(q);
        return *this;
    }

    template <typename Rep, typename Period>
        requires time_unit<units> && std::constructible_from<magnitude_type, Rep>
    basic_quantity& operator=(std::chrono::duration<Rep, Period> dur)
    {
        (*this).swap(basic_quantity{dur});
        return *this;
    }

    template <typename Up>
        requires std::constructible_from<magnitude_type, Up> && unitless_unit<units>
    constexpr basic_quantity& operator=(Up up) noexcept(std::is_nothrow_constructible_v<magnitude_type, Up>)
    {
        (*this).swap(basic_quantity(std::move(up)));
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
    void swap(basic_quantity& other) noexcept(std::is_nothrow_swappable_v<magnitude_type>)
    {
        using std::swap;
        swap(other.magnitude_, magnitude_);
    }

    friend auto operator==(const basic_quantity& a, const basic_quantity& b) -> bool  = default;
    friend auto operator<=>(const basic_quantity& a, const basic_quantity& b) -> auto = default;

    // --- Accessor Functions ---

    /// \brief Returns the magnitude of the \c *this
    ///
    /// \return A \c const reference to the magnitude of \c *this
    constexpr const magnitude_type& magnitude() const noexcept
    {
        return magnitude_;
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

    constexpr explicit(unitless_unit<units>) operator magnitude_type() const noexcept
    {
        return magnitude_;
    }

    template <typename Rep, typename Period>
    constexpr explicit(
        !internal::_detail::EnableImplicitFromChrono<std::chrono::duration<Rep, Period>, magnitude_type,
                                                     units>) operator std::chrono::duration<Rep, Period>() const
        requires time_unit<units>
    {
        return std::chrono::duration<Rep, Period>{
            magnitude_ * internal::_detail::to_chrono_conversion_factor<std::chrono::duration<Rep, Period>, units>()};
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
    const auto lhsSIBase = lhs.toSIBaseunits();
    const auto rhsSIBase = rhs.toSIBaseunits();

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
constexpr auto operator/(const basic_quantity<M1, U1>  lhs,
                         const basic_quantity<M2, U2>& rhs) noexcept(internal::nothrow_multiply_enabled_with<M1, M2>)
    -> basic_quantity<decltype(lhs.magnitude() / rhs.magnitude()), U1.toSIBaseunits() / U2.toSIBaseunits()>
{
    const auto lhsSIBase = lhs.toSIBaseunits();
    const auto rhsSIBase = rhs.toSIBaseunits();

    constexpr unit auto outputunits = lhsSIBase.units() / rhsSIBase.units();
    return basic_quantity<decltype(lhs.magnitude() / rhs.magnitude()), outputunits>(lhsSIBase.magnitude() *
                                                                                    rhsSIBase.magnitude());
}

template <typename M1, unit auto U1, typename M2>
    requires internal::multiply_enabled_with<M1, M2>
constexpr auto operator*(const basic_quantity<M1, U1>& lhs, const M2& rhs) noexcept(
    internal::nothrow_multiply_enabled_with<M1, M2>) -> basic_quantity<decltype(lhs.magnitude() * rhs), U1>
{
    return basic_quantity<decltype(lhs.magnitude() * rhs), U1>(lhs.magnitude() * rhs);
}

template <typename M1, unit auto U1, typename M2>
    requires internal::multiply_enabled_with<M1, M2>
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

// --- Formatting ---
template <typename M, unit auto U>
std::ostream& operator<<(std::ostream& os, const basic_quantity<M, U>& q)
{
    os << std::format("{}", q);
    return os;
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
concept Temperature = temperature_unit<QuantityType::units>;

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
        auto pos = ctx.begin();
        while (pos != ctx.end() && *pos != '}')
        {
            if (std::tolower(*pos) == 'b')
            {
                inSIBaseunits_ = true;
            }
            else if (std::tolower(*pos) == 'u')
            {
                unitsOnly_ = true;
            }
            else if (std::tolower(*pos) == 'd')
            {
                dimensionOnly_ = true;
            }
            ++pos;
        }
        if (dimensionOnly_ && unitsOnly_)
        {
            throw std::format_error{"Cannot specify both dimension only and units only!"};
        }
    }

    auto format(const maxwell::basic_quantity<M, U>& q, std::format_context& ctx) const
    {
        std::string temp;
        const auto  actual = (inSIBaseunits_) ? q.toSIBaseunits() : q;
        if (unitsOnly_)
        {
            std::format_to(std::back_inserter(temp), "{}", maxwell::unit_string<decltype(actual)::units>);
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
            std::format_to(std::back_inserter(temp), "{} {}", actual.magnitude(),
                           maxwell::unit_string<decltype(actual)::units>);
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

/// \cond
namespace internal::_detail
{
template <typename M>
constexpr bool supports_limits = std::numeric_limits<M>::is_specialized;
}
/// \endcond

/// \brief Specialization of \c std::numeric_limits for \c basic_quantity
///
/// Specialization of \c std::numeric_limits for \c basic_quantity. Provides
/// values equivalent to \c std::numeric_limits<M>.
template <typename M, maxwell::unit auto U>
    requires internal::_detail::supports_limits<M>
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
    requires internal::_detail::supports_limits<M>
struct std::numeric_limits<const maxwell::basic_quantity<M, U>> : std::numeric_limits<maxwell::basic_quantity<M, U>>
{
};
#endif