/**
 * @file Unit.hpp
 * @author Alex Schiffer
 * @brief Definition of units and associated functions
 * @version 0.1
 * @date 2024-11-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef UNIT_HPP
#define UNIT_HPP

#include <array>
#include <concepts>
#include <string>

#include "internal/Measure.hpp"

/// \namespace Maxwell
/// \brief Public declarations to be used by consumers of the library
namespace Maxwell
{
/// \class UnitType
/// \brief Definition of unit of measurement
///
/// A \c UnitType represents a unit of measurement. It represents a unit derived from the SI base units
/// (with the exception of the base unit of mass is the gram, not te kilogram). A \c UnitType is the product
/// of the base units specified by the template parameters, possibly scaled by an appropriate power of exponentiation.
/// The base units are part of the units type, so two instantiations of \c UnitType are the same type if and only if
/// they represent the same unit. If a unit does not have a particular dimension, use \c nullMeasure as the template
/// parameter for that dimension.
///
/// Some units have identical dimensions; different units with the same dimensions can be distinguinshed by providing
/// a tag type. This allows for different units with the same dimensionality to compare unequal.
///
/// Because a units dimension is part of its type, unit errors can be caught at compile-time rather than run-time.
/// Additionally, unit conversion can be performed automatically at compile-time rather than at runtime.
///
/// Using C++ 20 features, units can generally be manipulated as values rather than requiring metaprogramming on the
/// units type (e.g. it is possible to perform transformations on values of units or compare two units with \c ==
/// instead of needing to use metafunctions).
///
/// \tparam Amount_ the amount dimension of the unit
/// \tparam Currrent_ the current dimension of the unit
/// \tparam Length_ the length dimension of the unit
/// \tparam Luminosity_ the luminosity dimension of the unit
/// \tparam Mass_ the mass dimension of the unit
/// \tparam Temerature_ the temperature dimension of the unit
/// \tparam Time_ the time dimension of the unit
/// \tparam Tag_ the tag of the unit (see above)
template <Internal::Measure auto Amount_, Internal::Measure auto Current_, Internal::Measure auto Length_,
          Internal::Measure auto Luminosity_, Internal::Measure auto Mass_, Internal::Measure auto Temperature_,
          Internal::Measure auto Time_, typename Tag_ = void>
struct UnitType
{
    /// The amount dimension of the unit
    static constexpr Internal::Measure auto Amount      = Amount_;
    /// The current dimension of the unit
    static constexpr Internal::Measure auto Current     = Current_;
    /// The length dimension of the unit
    static constexpr Internal::Measure auto Length      = Length_;
    /// The luminosity dimension of the unit
    static constexpr Internal::Measure auto Luminosity  = Luminosity_;
    /// The mass dimension of the unit
    static constexpr Internal::Measure auto Mass        = Mass_;
    /// The temperature dimension of the unit
    static constexpr Internal::Measure auto Temperature = Temperature_;
    /// The time dimension of the unit
    static constexpr Internal::Measure auto Time        = Time_;
    /// The tag of the unit
    using Tag                                           = Tag_;

    /// \brief Return the amount dimension of the unit
    /// \return The amount dimension of the unit
    consteval Internal::Measure auto amount() const noexcept
    {
        return Amount;
    }

    /// \brief Return the current dimension of the unit
    /// \return The current dimension of the unit
    consteval Internal::Measure auto current() const noexcept
    {
        return Current;
    }

    /// \brief Return the length dimension of the unit
    /// \return The length dimension of the unit
    consteval Internal::Measure auto length() const noexcept
    {
        return Length;
    }

    /// \brief Return the luminosity dimension of the unit
    /// \return The luminosity dimension of the unit
    consteval Internal::Measure auto luminosity() const noexcept
    {
        return Luminosity;
    }

    /// \brief Return the mass dimension of the unit
    /// \return The mass dimension of the unit
    consteval Internal::Measure auto mass() const noexcept
    {
        return Mass;
    }

    /// \brief Return the temperature dimension of the unit
    /// \return The temperature dimension of the unit
    consteval Internal::Measure auto temperature() const noexcept
    {
        return Temperature;
    }

    /// \brief Return the time dimension of the unit
    /// \return The time dimension of the unit
    consteval Internal::Measure auto time() const noexcept
    {
        return Time;
    }

    /// \brief Adds a tag to the unit
    ///
    /// Creates a new \c UnitType with the same dimensions as \c *this,
    /// but with the specified tag.
    ///
    /// \tparam T the tag to add
    /// \return the modified unit
    template <typename T>
    consteval auto addTag() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature, Time, T>{};
    }

    /// \brief Adjusts the mulltiplier of the amount dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the amount dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().amount().magnitude() == this->amount().magnitude() + Adjstument
    /// \endcode
    ///
    /// \tparam The amount to adjust the multiplier of the amount dimension by
    /// \return A new unit with the amount multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierAmount() const noexcept
    {
        return UnitType<Amount.template adjustMultiplier<Adjustment>(), Current, Length, Luminosity, Mass, Temperature,
                        Time, Tag>{};
    }

    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierCurrent() const noexcept
    {
        return UnitType<Amount, Current.template adjustMultiplier<Adjustment>(), Length, Luminosity, Mass, Temperature,
                        Time, Tag>{};
    }

    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierLength() const noexcept
    {
        return UnitType<Amount, Current, Length.template adjustMultiplier<Adjustment>(), Luminosity, Mass, Temperature,
                        Time, Tag>{};
    }

    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierLuminosity() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity.template adjustMultiplier<Adjustment>(), Mass, Temperature,
                        Time, Tag>{};
    }

    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierMass() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass.template adjustMultiplier<Adjustment>(), Temperature,
                        Time, Tag>{};
    }

    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierTemperature() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature.template adjustMultiplier<Adjustment>(),
                        Time, Tag>{};
    }

    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierTime() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature,
                        Time.template adjustMultiplier<Adjustment>(), Tag>{};
    }
};

// --- Base units ---

/// SI unit "mole"
constexpr UnitType<Internal::baseMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    moleUnit;
/// Type alias for SI unit "mole"
using MoleUnitType = decltype(moleUnit);

/// SI unit "ampere"
constexpr UnitType<Internal::nullMeasure, Internal::baseMeasure, Internal::nullMeasure, Internal::baseMeasure,
                   Internal::nullMeasure, Internal::baseMeasure, Internal::nullMeasure>
    ampereUnit;
/// Type alias for SI unit "ampere"
using AmpereUnitType = decltype(ampereUnit);

/// SI unit "meter"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::baseMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    meterUnit;
/// Type alias for SI unit "meter"
using MeterUnitType = decltype(meterUnit);

/// SI unit "candela"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::baseMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    candelaUnit;
/// Type alias for SI unit "candela"
using CandelaUnitType = decltype(candelaUnit);

/// SI unit "gram"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::baseMeasure, Internal::nullMeasure, Internal::nullMeasure>
    gramUnit;
/// Type alias for SI unit "gram"
using GramUnitType = decltype(gramUnit);

/// SI unit "Kelvin"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::baseMeasure, Internal::nullMeasure>
    kelvinUnit;
/// Type alias for SI unit Kelvin
using KelvinUnitType = decltype(kelvinUnit);

/// SI unit "second"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::baseMeasure>
    secondUnit;
/// Type alias for SI unit "second"
using SecondUnitType = decltype(secondUnit);

/// Constant indidcating lack of units
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    unitlessUnit;
/// Type alias for constant indicating lack of units
using UnitlessUnitType = decltype(unitlessUnit);

// -- Unit Concepts ---

/// \cond
namespace _detail
{
template <typename>
struct _is_unit : std::false_type
{
};

template <Internal::Measure auto Amount_, Internal::Measure auto Current_, Internal::Measure auto Length_,
          Internal::Measure auto Luminosity_, Internal::Measure auto Mass_, Internal::Measure auto Temperature_,
          Internal::Measure auto Time_, typename Tag_>
struct _is_unit<UnitType<Amount_, Current_, Length_, Luminosity_, Mass_, Temperature_, Time_, Tag_>> : std::true_type
{
};
} // namespace _detail
/// \endcond

/// \brief Specifies a type is an instantiation of the \c UnitType class template
///
/// \tparam U the type to check
template <typename U>
concept Unit = _detail::_is_unit<std::remove_cvref_t<U>>::value;

/// \brief Specifies a unit is unitless
///
/// \tparam U the unit to check
template <auto U>
concept Unitless = std::same_as<std::remove_cvref_t<decltype(U)>, UnitlessUnitType>;

template <auto From, auto To>
concept AmountConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

template <auto From, auto To>
concept CurrentConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

template <auto From, auto To>
concept LengthConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

template <auto From, auto To>
concept LuminosityConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

template <auto From, auto To>
concept MassConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

template <auto From, auto To>
concept TemperatureConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

template <auto From, auto To>
concept TimeConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

template <typename, typename>
struct is_tag_convertible : std::false_type
{
};

template <typename From, typename To>
concept TagConvertibleTo = is_tag_convertible<From, To>::value;

template <auto From, auto To>
concept UnitConvertibleTo =
    Unit<decltype(From)> && Unit<decltype(To)> && AmountConvertibleTo<From.amount(), To.amount()> &&
    CurrentConvertibleTo<From.current(), To.current()> && LengthConvertibleTo<From.length(), To.length()> &&
    LuminosityConvertibleTo<From.luminosity(), To.luminosity()> && MassConvertibleTo<From.mass(), To.mass()> &&
    TemperatureConvertibleTo<From.temperature(), To.temperature()> && TimeConvertibleTo<From.time(), To.time()> &&
    TagConvertibleTo<typename decltype(From)::Tag, typename decltype(To)::Tag>;

// --- Unit Traits ---
template <auto U>
concept Amount = Unit<decltype(U)> && U.amount().power() == 1 && U.current() == Internal::nullMeasure &&
                 U.length() == Internal::nullMeasure && U.luminosity() == Internal::nullMeasure &&
                 U.mass() == Internal::nullMeasure && U.temperature() == Internal::nullMeasure &&
                 U.time() == Internal::nullMeasure && std::same_as<std::remove_cv_t<typename decltype(U)::Tag>, void>;

template <auto U>
concept Current = Unit<decltype(U)>;

template <auto U>
concept Length = Unit<decltype(U)>;

template <auto U>
concept Luminosity = Unit<decltype(U)>;

template <auto U>
concept Time = Unit<decltype(U)> && U.amount() == Internal::nullMeasure && U.current() == Internal::nullMeasure &&
               U.length() == Internal::nullMeasure && U.luminosity() == Internal::nullMeasure &&
               U.mass() == Internal::nullMeasure && U.temperature() == Internal::nullMeasure && U.time().power() == 1 &&
               std::same_as<std::remove_cv_t<typename decltype(U)::Tag>, void>;
// --- Unit comparisons ---

consteval bool operator==(Unit auto lhs, Unit auto rhs) noexcept
{
    return std::same_as<std::remove_cvref_t<decltype(lhs)>, std::remove_cvref_t<decltype(rhs)>>;
}

// --- Unit conversions ---

/// \cond
namespace _detail
{
template <typename>
constexpr bool dependentFalse = false;

constexpr std::array pow10{1e30,  1e29,  1e28,  1e27,  1e26,  1e25,  1e24,  1e23,  1e22,  1e21,  1e20,  1e19,  1e18,
                           1e17,  1e16,  1e15,  1e14,  1e13,  1e12,  1e11,  1e10,  1e9,   1e8,   1e7,   1e6,   1e5,
                           1e4,   1e3,   1e2,   1e1,   1e0,   1e-1,  1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,  1e-8,
                           1e-9,  1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, 1e-16, 1e-17, 1e-18, 1e-19, 1e-20, 1e-21,
                           1e-22, 1e-23, 1e-24, 1e-25, 1e-26, 1e-27, 1e-28, 1e-29, 1e-30};

consteval double conversionFactorPrefix(std::intmax_t from, std::intmax_t to) noexcept
{
    if (from - to < pow10.size())
    {
        return pow10[(from - to) + 30];
    }
    return 0.0;
}
} // namespace _detail
/// \endcond

consteval double tagConversionFactor(Unit auto lhs, Unit auto rhs) noexcept
{
    if constexpr (std::same_as<typename decltype(lhs)::Tag, typename decltype(rhs)::Tag>)
    {
        return 1.0;
    }
    else
    {
        static_assert(_detail::dependentFalse<typename decltype(lhs)::Tag>,
                      "Attempting to convert between units with inconvertible tags!");
    }
}

consteval double conversionFactor(Unit auto, Unit auto) noexcept
{
    return 0.0;
}

consteval Unit auto operator*(Unit auto lhs, Unit auto rhs) noexcept
{
    return UnitType<lhs.amount() * rhs.amount(), lhs.current() * rhs.current(), lhs.length() * rhs.length(),
                    lhs.luminosity() * rhs.luminosity(), lhs.mass() * rhs.mass(), lhs.temperature() * rhs.temperature(),
                    lhs.time() * rhs.time(), typename decltype(lhs)::Tag>{};
}

consteval Unit auto operator/(Unit auto lhs, Unit auto rhs) noexcept
{
    return UnitType<lhs.amount() / rhs.amount(), lhs.current() / rhs.current(), lhs.length() / rhs.length(),
                    lhs.luminosity() / rhs.luminosity(), lhs.mass() / rhs.mass(), lhs.temperature() / rhs.temperature(),
                    lhs.time() / rhs.time(), typename decltype(lhs)::Tag>{};
}

// --- Formatting ---
/// \cond
namespace _detail
{
std::string defaultUnitName(Unit auto)
{
    return "";
}
} // namespace _detail
/// \endcond

template <Unit auto U>
inline const std::string unitString = _detail::defaultUnitName(U);
} // namespace Maxwell

#endif