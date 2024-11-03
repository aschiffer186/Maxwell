/**
 * @file Quantity.hpp
 * @author Alex Schiffer
 * @brief Definition of Quantity class template
 * @version 0.1
 * @date 2024-11-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <chrono>
#include <concepts>
#include <format>
#include <functional>
#include <iterator>
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
    requires Time<U>
constexpr double chronoConversionFactor()
{
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
} // namespace Internal::_detail
/// \endcond

/// \class Quantity
/// \brief A dimensioned quantity
///
/// <tt>class Quantity</tt> represents a value that has both a magnitude
/// and units. The units are part of the quantity's type, allowing for
/// verification of unit coherency and conversion of units at compile-time.
/// A program is ill-formed if an invalid operation is performed between
/// two quantities with incompatability types.
///
/// \sa UnitType
///
/// @tparam T the type of the quantity's magnitude
/// @tparam U the quantity's units
template <typename T, Unit auto U>
class BasicQuantity
{
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
    constexpr explicit(Unitless<Units>)
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
    /// \throws any exceptions thrown by the constructor of \c MagnitudeType
    template <typename Up>
        requires(!std::same_as<Up, MagnitudeType>) && std::constructible_from<MagnitudeType, Up&&>
    constexpr explicit(Unitless<Units>)
        BasicQuantity(Up&& magnitude) noexcept(std::is_nothrow_constructible_v<MagnitudeType, Up&&>)
        : magnitude_(std::forward<Up>(magnitude))
    {
    }

    /// \brief Constructor
    ///
    /// Constructs a \c BasicQuantity from a \c std::chrono::duration type, allowing for integration with the standard
    /// library. This constructor is implicit if there would be a loss of information converting from the
    /// specified \c std::chrono::duration type.
    ///
    /// \pre \c MagnitudeType is constructible from \c Rep
    /// \pre \c Units has dimensions of time
    /// \post \c this->magnitude() is equal to \c dur.count()
    ///
    /// \throw any exceptions thrown by \c dur or by the constructor of \c MagnitudeType
    template <typename Rep, typename Period>
        requires Time<Units> && std::constructible_from<MagnitudeType, Rep>
    constexpr explicit(!implicitFromChrono<std::chrono::duration<Rep, Period>>)
        BasicQuantity(std::chrono::duration<Rep, Period> dur)
        : magnitude_(dur.count() *
                     Internal::_detail::chronoConversionFactor<std::chrono::duration<Rep, Period>, Units>())
    {
    }

    template <typename Up, Unit auto Other>
        requires(std::constructible_from<MagnitudeType, std::add_rvalue_reference_t<Up>>) &&
                (!(std::same_as<Up, MagnitudeType> && Other == U)) && UnitConvertibleTo<Other, Units>
    constexpr BasicQuantity([[maybe_unused]] BasicQuantity<Up, Other> q) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, std::add_rvalue_reference_t<Up>>)
        : magnitude_(q.magnitude() * conversionFactor(Other, Units))
    {
    }

    template <typename Up, Unit auto Other>
        requires(std::constructible_from<MagnitudeType, std::add_rvalue_reference_t<Up>>) &&
                    (!(std::same_as<Up, MagnitudeType> && Other == U)) && UnitConvertibleTo<Other, Units>
    auto operator=([[maybe_unused]] BasicQuantity<Up, Other> q) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, std::add_rvalue_reference_t<Up>>) -> BasicQuantity&
    {
        (*this).swap(q);
        return *this;
    }

    // --- Core Language Functions ---

    auto swap(BasicQuantity& other) noexcept(std::is_nothrow_swappable_v<MagnitudeType>)
    {
        using std::swap;
        swap(other.magnitude_, magnitude_);
    }

    friend auto operator==(const BasicQuantity& a, const BasicQuantity& b) -> bool  = default;
    friend auto operator<=>(const BasicQuantity& a, const BasicQuantity& b) -> auto = default;

    // --- Accessor Functions ---

    constexpr const MagnitudeType& magnitude() const noexcept
    {
        return magnitude_;
    }

    constexpr Unit auto units() const noexcept
    {
        return Units;
    }

    // --- Transformation Functions ---
    constexpr auto toSIBaseUnits() const noexcept
    {
        return *this;
    }

    // --- Conversion Functions ---

    constexpr explicit(Unitless<Units>) operator MagnitudeType() const noexcept
    {
        return magnitude_;
    }

    template <typename Rep, typename Period>
    constexpr explicit(
        !Internal::_detail::EnableImplicitFromChrono<std::chrono::duration<Rep, Period>, MagnitudeType,
                                                     Units>) operator std::chrono::duration<Rep, Period>() const
        requires Time<Units>
    {
        return std::chrono::duration<Rep, Period>{};
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

  private:
    MagnitudeType magnitude_{};
};

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
} // namespace Maxwell

namespace std
{
template <typename M, Maxwell::Unit auto U>
struct formatter<Maxwell::BasicQuantity<M, U>>
{
    auto format(const Maxwell::BasicQuantity<M, U> q, std::format_context& ctx) const
    {
        std::string temp;
        std::format_to(std::back_inserter(temp), "{} {}", q.magnitude(), Maxwell::unitString<U>);
        return std::formatter<std::string_view>{}.format(temp, ctx);
    }
};

template <typename M, Maxwell::Unit auto U>
struct hash<Maxwell::BasicQuantity<M, U>>
{
    size_t operator()(const Maxwell::BasicQuantity<M, U>& q) noexcept(noexcept(hash<M>{q.magnitude()}))
    {
        hash<M>{}(q.toSIBaseUnits().magnitude());
    }
};
} // namespace std
#endif