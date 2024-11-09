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

/// \namespace Maxwell
namespace Maxwell
{
template <typename T, Unit auto U>
class BasicQuantity;

/// \cond
namespace Internal::_detail
{
template <typename Period, RatioLike Scale>
constexpr bool implicitUnitConversion() noexcept
{
    return true;
}

template <typename D, typename M, auto U>
concept EnableImplicitFromChrono = Unit<decltype(U)> && ChronoDuration<D> &&
                                   implicitUnitConversion<typename D::period, typename decltype(U.time())::Scale>() &&
                                   std::convertible_to<typename D::rep, M>;

template <typename D, Unit auto U>
    requires TimeUnit<U>
constexpr double chronoConversionFactor()
{
    // using Period = D::period;
    if (std::ratio_equal_v<typename decltype(U.time())::Scale, Internal::_detail::one>)
    {
        // Detailing with second, potentially scaled by a metric prefix
        [[maybe_unused]] constexpr std::intmax_t multiplier = U.time().multiplier();
        return 1.0;
    }
    else
    {
        return 1.0;
    }
}

template <typename D, Unit auto U>
    requires TimeUnit<U>
constexpr double toChronoConversionFactor()
{
    // using Period = D::period;
    if (std::ratio_equal_v<typename decltype(U.time())::Scale, Internal::_detail::one>)
    {
        // Detailing with second, potentially scaled by a metric prefix
        [[maybe_unused]] constexpr std::intmax_t multiplier = U.time().multiplier();
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

template <typename M, Unit auto U>
struct is_basic_quantity<BasicQuantity<M, U>> : std::true_type
{
};
} // namespace Internal::_detail
/// \endcond

/// \class BasicQuantity
/// \brief A dimensioned quantity
///
/// <tt>class BasicQuantity</tt> represents a value that has both a magnitude
/// and units. The units are part of the quantity's type, allowing for
/// verification of unit coherency and conversion of units at compile-time.
/// A program is ill-formed if an invalid operation is performed between
/// two quantities with incompatability types.
///
/// \sa UnitType
///
/// \tparam T the type of the quantity's magnitude
/// \tparam U the quantity's units
template <typename T, Unit auto U>
class BasicQuantity
{
    static_assert(!std::is_const_v<T>, "Magnitude cannot be const");
    static_assert(!std::is_volatile_v<T>, "Magnitude cannot be volatile");
    static_assert(!Internal::Similar<T, std::in_place_t>, "Magnitude cannot be std::in_place_t");
    static_assert(!Internal::_detail::ChronoDuration<T>, "Magnitude cannot be a std::chrono::duration");
    static_assert(!Internal::_detail::is_basic_quantity<T>::value, "Magnitude cannot be a basic quantity!");

    template <typename D>
    static constexpr bool implicitFromChrono = Internal::_detail::EnableImplicitFromChrono<D, T, U>;

  public:
    /// The type of the quantity's magnitude
    using MagnitudeType = T;
    /// The type of the quantity's units
    using UnitsType     = decltype(U);

    /// The quantity's units
    static constexpr auto Units = U;

    // --- Constructors ---

    /// \brief Default constructor
    ///
    /// Creates a \c Quantity whose magnitude is default constructed.
    ///
    /// \pre \c MagnitudeType is default constructible
    /// \post <tt> this->magnitude()</tt> is equal to \c MagnitudeType{}
    ///
    /// \throws any exceptions thrown by the default constructor of \c MagnitudeType
    constexpr BasicQuantity()
        requires std::constructible_from<MagnitudeType>
    = default;

    /// \brief Constructor
    ///
    /// Constructs a \c Quantity whose magnitude is equal to the specified value.
    /// This constructor is explicit if the \c Quantity is not unitless.
    ///
    /// \pre \c MagnitudeType is move constructible
    /// \post <tt>this->magnitude()</tt> is equal to \c magnitude
    ///
    /// \throws Any exceptions thrown by the move constructor of \c MagnitudeType
    constexpr explicit(UnitlessUnit<Units>)
        BasicQuantity(MagnitudeType magnitude) noexcept(std::is_nothrow_move_constructible_v<MagnitudeType>)
        requires std::move_constructible<MagnitudeType>
        : magnitude_(std::move(magnitude))
    {
    }

    /// \brief Constructor
    ///
    /// Constructs a \c BasicQuantity whose magnitude is constructed using the value \c std::forward<Up>(magnitude)
    ///
    /// \pre \c MagnitudeType is constructible from \c Up&&
    /// \post \c this->magnitude() is equal to \c MagnitudeType(std::forward<Up>(magnitude))
    ///
    /// \throws Any exceptions thrown by the constructor of \c MagnitudeType
    template <typename Up>
        requires(!Internal::_detail::ChronoDuration<Up>) && std::constructible_from<MagnitudeType, Up> &&
                (!Internal::_detail::is_basic_quantity<std::remove_cvref_t<Up>>::value)
    constexpr explicit(UnitlessUnit<Units>)
        BasicQuantity(Up&& magnitude) noexcept(std::is_nothrow_constructible_v<MagnitudeType, Up>)
        : magnitude_(std::forward<Up>(magnitude))
    {
        std::cout << "Single argument template constructor\n";
    }

    /// \c Constructor
    ///
    /// Constructs a \c BasicQuantity whose magnitude s constructed in place from the specified arguments using
    /// the expression \c std::forward<Args>(args)...
    ///
    /// \pre \c MagnitudeType is constructible from \c Args
    /// \post \c this->magnitude() is equal to \c MagnitudeType(std::forward<Args>(args)...)
    ///
    /// \throws Any exceptions thrown by the constructor of \c MagnitudeType
    template <typename... Args>
        requires(std::constructible_from<MagnitudeType, Args && ...>)
    constexpr explicit BasicQuantity(std::in_place_t,
                                     Args&&... args) noexcept(std::is_nothrow_constructible_v<MagnitudeType, Args&&...>)
        : magnitude_(std::forward<Args>(args)...)
    {
    }

    /// \c Constructor
    ///
    /// Constructs a \c BasicQuantity whose magnitude s constructed in place from the specified initializer list
    /// and arguments arguments using
    /// the expression <tt>MagnitudeType(il, std::forward<Args>(args)...)</tt>
    ///
    /// \pre \c MagnitudeType is constructible from \c Args
    /// \post \c this->magnitude() is equal to <tt>MagnitudeType(il, std::forward<Args>(args)...)</t>>
    ///
    /// \throws Any exceptions thrown by the constructor of \c MagnitudeType
    template <typename Up, typename... Args>
        requires std::constructible_from<std::initializer_list<Up>, Args...>
    constexpr explicit BasicQuantity(std::in_place_t, std::initializer_list<Up> il, Args&&... args) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, std::initializer_list<Up>, Args...>)
        : magnitude_(il, std::forward<Args>(args)...)
    {
    }

    /// \brief Constructor
    ///
    /// Constructs a \c BasicQuantity from a \c std::chrono::duration type, allowing for integration with the
    /// standard library. This constructor is implicit if there would be a loss of information converting from the
    /// specified \c std::chrono::duration type.
    ///
    /// \pre \c MagnitudeType is constructible from \c Rep
    /// \pre \c Units has dimensions of time
    /// \post \c this->magnitude() is equal to \c dur.count()
    ///
    /// \throw any exceptions thrown by \c dur or by the constructor of \c MagnitudeType
    template <typename Rep, typename Period>
        requires TimeUnit<Units> && std::constructible_from<MagnitudeType, Rep>
    constexpr explicit(!implicitFromChrono<std::chrono::duration<Rep, Period>>)
        BasicQuantity(std::chrono::duration<Rep, Period> dur)
        : magnitude_(dur.count() *
                     Internal::_detail::chronoConversionFactor<std::chrono::duration<Rep, Period>, Units>())
    {
    }

    template <typename Up, Unit auto Other>
        requires(std::constructible_from<MagnitudeType, std::add_rvalue_reference_t<Up>>) &&
                (!(std::same_as<Up, MagnitudeType> && Other == U)) && UnitConvertibleTo<Other, Units>
    constexpr BasicQuantity(BasicQuantity<Up, Other> q) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, std::add_rvalue_reference_t<Up>>)
        : magnitude_(q.magnitude() * conversionFactor(Other, Units))
    {
    }

    template <typename Up, Unit auto Other>
        requires(std::constructible_from<MagnitudeType, std::add_rvalue_reference_t<Up>>) &&
                UnitConvertibleTo<Other, Units>
    BasicQuantity& operator=(BasicQuantity<Up, Other> q) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, std::add_rvalue_reference_t<Up>>)
    {
        (*this).swap(q);
        return *this;
    }

    template <typename Rep, typename Period>
        requires TimeUnit<Units> && std::constructible_from<MagnitudeType, Rep>
    BasicQuantity& operator=(std::chrono::duration<Rep, Period> dur)
    {
        (*this).swap(BasicQuantity{dur});
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
    void swap(BasicQuantity& other) noexcept(std::is_nothrow_swappable_v<MagnitudeType>)
    {
        using std::swap;
        swap(other.magnitude_, magnitude_);
    }

    friend auto operator==(const BasicQuantity& a, const BasicQuantity& b) -> bool  = default;
    friend auto operator<=>(const BasicQuantity& a, const BasicQuantity& b) -> auto = default;

    // --- Accessor Functions ---

    /// \brief Returns the magnitude of the \c *this
    ///
    /// \return A \c const reference to the magnitude of \c *this
    constexpr const MagnitudeType& magnitude() const noexcept
    {
        return magnitude_;
    }

    /// \brief Returns the units of \c *this
    ///
    /// \return The units of \c *this
    constexpr Unit auto units() const noexcept
    {
        return Units;
    }

    // --- Transformation Functions ---
    /// \brief Returns a new \c BasicQuantity with the same dimensions of \c *this but in SI base units
    ///
    /// Returns a new \c BasicQuantity with the same dimension of \c *this, but in SI base units. The magnitude of
    /// the returned quantity is equivalent to \c this->magnitude() converted to SI base units
    ///
    /// \return A new \c BasicQuantity with the same dimension of \c *this in SI base units.
    constexpr auto toSIBaseUnits() const noexcept
    {
        return *this;
    }

    // --- Conversion Functions ---

    constexpr explicit(UnitlessUnit<Units>) operator MagnitudeType() const noexcept
    {
        return magnitude_;
    }

    template <typename Rep, typename Period>
    constexpr explicit(
        !Internal::_detail::EnableImplicitFromChrono<std::chrono::duration<Rep, Period>, MagnitudeType,
                                                     Units>) operator std::chrono::duration<Rep, Period>() const
        requires TimeUnit<Units>
    {
        return std::chrono::duration<Rep, Period>{
            magnitude_ * Internal::_detail::toChronoConversionFactor<std::chrono::duration<Rep, Period>, Units>()};
    }

    // --- Arithmetic Functions ---
    constexpr BasicQuantity& operator+=(const BasicQuantity& other) noexcept(Internal::NothrowAddEnabled<MagnitudeType>)
        requires Internal::AddEnabled<MagnitudeType>
    {
        magnitude_ = magnitude_ + other.magnitude();
        return *this;
    }

    constexpr BasicQuantity& operator-=(const BasicQuantity& other) noexcept(
        Internal::NothrowSubtractEnabled<MagnitudeType>)
        requires Internal::SubtractEnabled<MagnitudeType>
    {
        magnitude_ = magnitude_ - other.magnitude();
        return *this;
    }

    template <typename Up, Unit auto Other>
        requires UnitConvertibleTo<Other, Units> && Internal::AddEnabledWith<MagnitudeType, Up>
    constexpr BasicQuantity& operator+=(const BasicQuantity<Up, Other>& other) noexcept(
        Internal::NothrowAddEnabledWith<MagnitudeType, Up>)
    {
        constexpr double conversion = conversionFactor(Other, Units);
        magnitude_                  = magnitude_ + other.magnitude() * conversion;
        return *this;
    }

    template <typename Up, Unit auto Other>
        requires UnitConvertibleTo<Other, Units> && Internal::SubtractEnabledWith<MagnitudeType, Up>
    constexpr BasicQuantity& operator-=(const BasicQuantity<Up, Other>& other) noexcept(
        Internal::NothrowSubtractEnabledWith<MagnitudeType, Up>)
    {
        constexpr double conversion = conversionFactor(Other, Units);
        magnitude_                  = magnitude_ - other.magnitude() * conversion;
        return *this;
    }

    template <typename Up>
        requires Internal::MultiplyEnabledWith<MagnitudeType, Up>
    constexpr BasicQuantity& operator*=(const Up& value) noexcept(
        Internal::NothrowMultiplyEnabledWith<MagnitudeType, Up>)
    {
        magnitude_ = magnitude_ * value;
        return *this;
    }

    template <typename Up>
        requires Internal::DivideEnabledWith<MagnitudeType, Up>
    constexpr BasicQuantity& operator/=(const Up& value) noexcept(Internal::NothrowDivideEnabledWith<MagnitudeType, Up>)
    {
        magnitude_ = magnitude_ / value;
        return *this;
    }

    template <typename Up>
        requires Internal::ModuloEnabledWith<MagnitudeType, Up>
    constexpr BasicQuantity& operator%=(const Up& value) noexcept(Internal::NothrowModuloEnabledWith<MagnitudeType, Up>)
    {
        magnitude_ = magnitude_ % value;
        return *this;
    }

  private:
    MagnitudeType magnitude_{};
};

// --- CTAD for construction from std::chrono::duration
template <typename Rep, typename Period>
BasicQuantity(std::chrono::duration<Rep, Period>) -> BasicQuantity<Rep, secondUnit>;

/// \brief Convenience type alias for common case where \c MagnitudeType is \c double
template <Unit auto U>
using Quantity = BasicQuantity<double, U>;

/// \brief Convenience type alias for \c BasicQuantity with \c MagnitudeType of \c int
template <Unit auto U>
using IQuantity = BasicQuantity<int, U>;

template <typename M, Unit auto U>
constexpr void swap(BasicQuantity<M, U>& lhs, BasicQuantity<M, U>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

// --- Arithmetic Operators ---
template <typename M1, Unit auto U1, typename M2, Unit auto U2>
    requires UnitConvertibleTo<U1, U2> && Internal::AddEnabledWith<M1, M2>
constexpr auto operator+(BasicQuantity<M1, U1>        lhs,
                         const BasicQuantity<M2, U2>& rhs) noexcept(Internal::NothrowAddEnabledWith<M1, M2>)
{
    return lhs += rhs;
}

template <typename M1, Unit auto U1, typename M2, Unit auto U2>
    requires UnitConvertibleTo<U1, U2> && Internal::SubtractEnabledWith<M1, M2>
constexpr auto operator-(BasicQuantity<M1, U1>        lhs,
                         const BasicQuantity<M2, U2>& rhs) noexcept(Internal::SubtractEnabledWith<M1, M2>)
{
    return lhs -= rhs;
}

// --- Formatting ---
template <typename M, Unit auto U>
std::ostream& operator<<(std::ostream& os, const BasicQuantity<M, U>& q)
{
    os << std::format("{}", q);
    return os;
}

// --- Quantity Type Traits ---
/// \brief Specifies a quantity has dimensions of amount
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Amount = AmountUnit<QuantityType::Units>;

/// \brief Specifies a quantity has dimensions of current
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Current = CurrentUnit<QuantityType::Units>;

/// \brief Specifies a quantity has dimensions of length
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Length = LengthUnit<QuantityType::Units>;

/// \brief Specifies a quantity has dimensions of time
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Luminosity = LuminosityUnit<QuantityType::Units>;

/// \brief Specifies a quantity has dimensions of mass
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Mass = MassUnit<QuantityType::Units>;

/// \brief Specifies a quantity has dimensions of temperature
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Temperature = TemperatureUnit<QuantityType::Units>;

/// \brief Specifies a quantity has dimensions of time
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Time = TimeUnit<QuantityType::Units>;

/// \brief Specifies a quantity is unitless
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Unitless = UnitlessUnit<QuantityType::Units>;
} // namespace Maxwell

// --- Specialization of standard library templates ---

/// \namespace std
/// \brief Specialization of standard library templates
namespace std
{
/// \brief Specialization of \c std::formatter for \c BasicQuantity
///
/// \tparam M The type of the magnitude of the \c BasicQuantity
/// \tparam U The units of the \c BasicQuantity
template <typename M, Maxwell::Unit auto U>
struct formatter<Maxwell::BasicQuantity<M, U>>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        auto pos = ctx.begin();
        while (pos != ctx.end() && *pos != '}')
        {
            if (std::tolower(*pos) == 'b')
            {
                inSIBaseUnits_ = true;
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

    auto format(const Maxwell::BasicQuantity<M, U>& q, std::format_context& ctx) const
    {
        std::string temp;
        const auto  actual = (inSIBaseUnits_) ? q.toSIBaseUnits() : q;
        if (unitsOnly_)
        {
            std::format_to(std::back_inserter(temp), "{}", Maxwell::unitString<decltype(actual)::Units>);
        }
        else if (dimensionOnly_)
        {
            if constexpr (Maxwell::Unitless<decltype(q)>)
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
                           Maxwell::unitString<decltype(actual)::Units>);
        }
        return std::formatter<std::string_view>{}.format(temp, ctx);
    }

  private:
    bool inSIBaseUnits_{false};
    bool unitsOnly_{false};
    bool dimensionOnly_{false};
};

/// \brief Specialization of \c std::hash for \c BasicQuantity
///
/// Specialization of \c std::hash for \c BasicQuantity. Returns the hash value
/// of the magnitude of a quantity after converting to SI base units. This ensures
/// that equal instances of \c BasicQuantity have equal hash values.
template <typename M, Maxwell::Unit auto U>
struct hash<Maxwell::BasicQuantity<M, U>>
{
    size_t operator()(const Maxwell::BasicQuantity<M, U>& q) noexcept(noexcept(hash<M>{q.magnitude()}))
    {
        hash<M>{}(q.toSIBaseUnits().magnitude());
    }
};

/// \brief Specialization of \c std::numeric_limits for \c BasicQuantity
///
/// Specialization of \c std::numeric_limits for \c BasicQuantity. Provides
/// values equivalent to \c std::numeric_limits<M>.
template <typename M, Maxwell::Unit auto U>
struct numeric_limits<Maxwell::BasicQuantity<M, U>> : numeric_limits<M>
{
    static constexpr bool is_specialized = true;
};
} // namespace std
#endif