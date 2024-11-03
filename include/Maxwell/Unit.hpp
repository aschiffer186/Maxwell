/// \file Unit.hpp
/// \author Alex Schiffer
/// \brief Definition of units and associated functions
/// \version 0.1
/// \date 2024-11-02
///
/// \copyright Copyright (c) 2024
#ifndef UNIT_HPP
#define UNIT_HPP

#include <array>
#include <concepts>
#include <string>
#include <type_traits>

#include "internal/Concepts.hpp"
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
/// \tparam Amount_ The amount dimension of the unit
/// \tparam Currrent_ The current dimension of the unit
/// \tparam Length_ The length dimension of the unit
/// \tparam Luminosity_ The luminosity dimension of the unit
/// \tparam Mass_ The mass dimension of the unit
/// \tparam Temerature_ The temperature dimension of the unit
/// \tparam Time_ The time dimension of the unit
/// \tparam Tag_ The tag of the unit (see above)
/// \tparam ExtraMultiplier_ Extra multiplier for dimensionless units that are not unitless
template <Internal::Measure auto Amount_, Internal::Measure auto Current_, Internal::Measure auto Length_,
          Internal::Measure auto Luminosity_, Internal::Measure auto Mass_, Internal::Measure auto Temperature_,
          Internal::Measure auto Time_, typename Tag_ = void, std::intmax_t ExtraMultiplier_ = 0>
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

    /// \brief Return the extra multiplier
    /// \return The extra multiplier of the unit
    consteval std::intmax_t multiplier() const noexcept
    {
        return ExtraMultiplier_;
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

    /// \brief Adjusts the multiplier of the amount dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the amount dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().amount().magnitude() == this->amount().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the amount dimension by
    /// \return A new unit with the amount multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierAmount() const noexcept
    {
        return UnitType<Amount.template adjustMultiplier<Adjustment>(), Current, Length, Luminosity, Mass, Temperature,
                        Time, Tag>{};
    }

    /// \brief Adjusts the multiplier of the current dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the current dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().current().magnitude() == this->current().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the current dimension by
    /// \return A new unit with the current multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierCurrent() const noexcept
    {
        return UnitType<Amount, Current.template adjustMultiplier<Adjustment>(), Length, Luminosity, Mass, Temperature,
                        Time, Tag>{};
    }

    /// \brief Adjusts the multiplier of the length dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the length dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().length().magnitude() == this->length().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the length dimension by
    /// \return A new unit with the length multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierLength() const noexcept
    {
        return UnitType<Amount, Current, Length.template adjustMultiplier<Adjustment>(), Luminosity, Mass, Temperature,
                        Time, Tag>{};
    }

    /// \brief Adjusts the multiplier of the luminosity dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the luminosity dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().luminosity().magnitude() == this->luminosity().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the luminosity dimension by
    /// \return A new unit with the luminosity multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierLuminosity() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity.template adjustMultiplier<Adjustment>(), Mass, Temperature,
                        Time, Tag>{};
    }

    /// \brief Adjusts the multiplier of the mass dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the mass dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().mass().magnitude() == this->mass().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the mass dimension by
    /// \return A new unit with the mass multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierMass() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass.template adjustMultiplier<Adjustment>(), Temperature,
                        Time, Tag>{};
    }

    /// \brief Adjusts the multiplier of the temperatue dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the temperatue dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().temperatue().magnitude() == this->temperatue().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the temperatue dimension by
    /// \return A new unit with the temperatue multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierTemperature() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature.template adjustMultiplier<Adjustment>(),
                        Time, Tag>{};
    }

    /// \brief Adjusts the multiplier of the time dimension
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the time dimension
    ///
    /// \post The unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().time().magnitude() == this->time().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the time dimension by
    /// \return A new unit with the time multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierTime() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature,
                        Time.template adjustMultiplier<Adjustment>(), Tag>{};
    }

    /// \brief Adjusts the extra multiplier of the unit
    ///
    /// Returns a new \c UnitType with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the xtra multiplier
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjustMultiplier<Adjustment().multiplier() == this->multiplier() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the extra multiplier by
    /// \return A new unit with the extra multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplierExtra() const noexcept
    {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature, Time, Tag,
                        ExtraMultiplier_ + Adjustment>{};
    }

    /// \brief Converts a unit to SI base units
    ///
    /// Returns a new \c UnitType with the same dimesions as \c *this, but expressed
    /// entirely in SI base units.
    ///
    /// \post The retuned unit is in SI base units.
    ///
    /// \return The equivalent unit in SI base units
    consteval auto toSIBaseUnits() const noexcept
    {
        const UnitType<Amount.toCoherentMeasure(), Current.toCoherentMeasure(), Length.toCoherentMeasure(),
                       Luminosity.toCoherentMeasure(), Mass.toCoherentMeasure(), Temperature.toCoherentMeasure(),
                       Time.toCoherentMeasure(), void>
            u{};
        return u.template adjustMultiplierMass<3>();
    }
};

// --- Base units ---

/// SI unit "mole"
constexpr UnitType<Internal::baseMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    moleUnit;
/// Type alias for SI unit "mole"
using moleUnitType = std::remove_const_t<decltype(moleUnit)>;

/// SI unit "ampere"
constexpr UnitType<Internal::nullMeasure, Internal::baseMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    ampereUnit;
/// Type alias for SI unit "ampere"
using ampereUnitType = std::remove_const_t<decltype(ampereUnit)>;

/// SI unit "meter"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::baseMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    meterUnit;
/// Type alias for SI unit "meter"
using meterUnitType = std::remove_const_t<decltype(meterUnit)>;

/// SI unit "candela"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::baseMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    candelaUnit;
/// Type alias for SI unit "candela"
using candelaUnitType = std::remove_const_t<decltype(candelaUnit)>;

/// SI unit "gram"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::baseMeasure, Internal::nullMeasure, Internal::nullMeasure>
    gramUnit;
/// Type alias for SI unit "gram"
using gramUnitType = std::remove_const_t<decltype(gramUnit)>;

/// SI unit "Kelvin"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::baseMeasure, Internal::nullMeasure>
    kelvinUnit;
/// Type alias for SI unit Kelvin
using kelvinUnitType = std::remove_const_t<decltype(kelvinUnit)>;

/// SI unit "second"
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::baseMeasure>
    secondUnit;
/// Type alias for SI unit "second"
using secondUnitType = std::remove_const_t<decltype(secondUnit)>;

/// Constant indidcating lack of units
constexpr UnitType<Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure,
                   Internal::nullMeasure, Internal::nullMeasure, Internal::nullMeasure>
    unitlessUnit;
/// Type alias for constant indicating lack of units
using unitlessUnitType = std::remove_const_t<decltype(unitlessUnit)>;

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
          Internal::Measure auto Time_, typename Tag_, std::intmax_t Extra_>
struct _is_unit<UnitType<Amount_, Current_, Length_, Luminosity_, Mass_, Temperature_, Time_, Tag_, Extra_>>
    : std::true_type
{
};
} // namespace _detail
/// \endcond

/// \brief Specifies a type is an instantiation of the \c UnitType class template
///
/// Specifies a type is an instantiation of the \c UnitType class template, ignoring
/// cv-qualifiers and references.
///
/// \tparam U The type to check
template <typename U>
concept Unit = _detail::_is_unit<std::remove_cvref_t<U>>::value;

/// \brief Specifies a unit is unitless
///
/// \tparam U The unit to check
template <auto U>
concept UnitlessUnit = Internal::Similar<decltype(U), unitlessUnitType>;

/// \brief Specfies amount dimension \c From can be converted to amount dimension \c To
///
/// Specifies amount dimension \c From can be converted to amount dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept AmountConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

/// \brief Specfies current dimension \c From can be converted to current dimension \c To
///
/// Specifies current dimension \c From can be converted to current dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept CurrentConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

/// \brief Specfies length dimension \c From can be converted to length dimension \c To
///
/// Specifies length dimension \c From can be converted to length dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept LengthConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

/// \brief Specfies luminosity dimension \c From can be converted to luminosity dimension \c To
///
/// Specifies luminosity dimension \c From can be converted to luminosity dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept LuminosityConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

/// \brief Specfies mass dimension \c From can be converted to mass dimension \c To
///
/// Specifies mass dimension \c From can be converted to mass dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept MassConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

/// \brief Specfies temperature dimension \c From can be converted to temperature dimension \c To
///
/// Specifies temperature dimension \c From can be converted to temperature dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept TemperatureConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

/// \brief Specfies time dimension \c From can be converted to time dimension \c To
///
/// Specifies time dimension \c From can be converted to time dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept TimeConvertibleTo =
    Internal::Measure<decltype(From)> && Internal::Measure<decltype(To)> && Internal::isMeasureConvertible(From, To);

/// \brief Type trait indicating two tags are convertible
///
/// Type trait indicating two tags are convertible. To indicate
/// two tags are convertible, specialize this trait for the tags
/// and inherit from \c std::true_type. By default, two different
/// tags are not convertible. Note, this trait should be specialized
/// for both \c From to \c To and \c To to \c From.
///
/// \sa \c TagConvertibleTo
/// \tparam From The starting tag
/// \tparam To The target tag
template <typename From, typename To>
struct is_tag_convertible : std::is_same<From, To>
{
};

/// \brief Specfies tag \c From can be converted to \c Tag to.
///
/// Specfies tag \c From can be converted to \c Tag to. This concept evaluates
/// to \c false unless the struct \c is_tag_convertible is specialized for
/// \c From and \c To
///
/// \sa \c is_tag_convertible
/// \tparam From The starting tag
/// \tparam To The target tag
template <typename From, typename To>
concept TagConvertibleTo = is_tag_convertible<From, To>::value;

/// \brief Specifies unit \c From can be converted to unit \c To
///
/// Specifies unit \c From can be converted to unit \c To. \c From can be converted to
/// \c To if all of all the dimensions of \c From can be converted to the corresponding
/// dimensions of \c To and the tag of \c From can be converted to the \c Tag of to.
///
/// Convertibility is a symmetric relation, if \c From is convertible to \c To, then \c To
/// is convertible to \c From.
///
/// \tparam From The starting unit
/// \tparam To the target unit
template <auto From, auto To>
concept UnitConvertibleTo =
    Unit<decltype(From)> && Unit<decltype(To)> && AmountConvertibleTo<From.amount(), To.amount()> &&
    CurrentConvertibleTo<From.current(), To.current()> && LengthConvertibleTo<From.length(), To.length()> &&
    LuminosityConvertibleTo<From.luminosity(), To.luminosity()> && MassConvertibleTo<From.mass(), To.mass()> &&
    TemperatureConvertibleTo<From.temperature(), To.temperature()> && TimeConvertibleTo<From.time(), To.time()> &&
    TagConvertibleTo<typename decltype(From)::Tag, typename decltype(To)::Tag>;

// --- Unit Traits ---
/// \brief Specifies a unit has dimensions of amount
///
/// Specifies a unit has dimensions of amount, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept AmountUnit = UnitConvertibleTo<U, moleUnit>;

/// \brief Specifies a unit has dimensions of current
///
/// Specifies a unit has dimensions of current, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept CurrentUnit = UnitConvertibleTo<U, ampereUnit>;

/// \brief Specifies a unit has dimensions of length
///
/// Specifies a unit has dimensions of length, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept LengthUnit = UnitConvertibleTo<U, meterUnit>;

/// \brief Specifies a unit has dimensions of luminosity
///
/// Specifies a unit has dimensions of luminosity, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept LuminosityUnit = UnitConvertibleTo<U, candelaUnit>;

/// \brief Specifies a unit has dimensions of mass
///
/// Specifies a unit has dimensions of mass, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept MassUnit = UnitConvertibleTo<U, gramUnit>;

/// \brief Specifies a unit has dimensions of temperature
///
/// Specifies a unit has dimensions of temperature, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept TemperatureUnit = UnitConvertibleTo<U, kelvinUnit>;

/// \brief Specifies a unit has dimensions of time
///
/// Specifies a unit has dimensions of time, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept TimeUnit = UnitConvertibleTo<U, secondUnit>;
// --- Unit comparisons ---

/// \brief Compare two units for equality
///
/// Compares two units for equality; two units are equal if and only if they
/// have the same dimension and tag. This implies the units have the same type
/// up to cvref-qualifiers.
///
/// \param lhs the first unit to compare for equality
/// \param rhs th second unit to compare for equality
constexpr bool operator==(Unit auto lhs, Unit auto rhs) noexcept
{
    return Internal::Similar<decltype(lhs), decltype(rhs)>;
}

// --- Unit conversions ---

/// \cond
namespace _detail
{
template <typename>
constexpr bool dependentFalse = false;

constexpr std::array pow10{1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22, 1e-21, 1e-20, 1e-19, 1e-18,
                           1e-17, 1e-16, 1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10, 1e-9,  1e-8,  1e-7,  1e-6,  1e-5,
                           1e-4,  1e-3,  1e-2,  1e-1,  1e-0,  1e1,   1e2,   1e3,   1e4,   1e5,   1e6,   1e7,   1e8,
                           1e9,   1e10,  1e11,  1e12,  1e13,  1e14,  1e15,  1e16,  1e17,  1e18,  1e19,  1e20,  1e21,
                           1e22,  1e23,  1e24,  1e25,  1e26,  1e27,  1e28,  1e29,  1e30};

constexpr double pow(double base, std::intmax_t power) noexcept
{
    if (power < 0)
    {
        return 1.0 / pow(base, -power);
    }

    if (power == 0)
    {
        return 1.0;
    }

    if (power == 1)
    {
        return base;
    }

    if (power % 2 == 0)
    {
        return pow(base * base, power / 2);
    }

    return pow(base * base, (power - 1) / 2);
}

constexpr double conversionFactorPrefix(std::intmax_t from, std::intmax_t to) noexcept
{
    if ((from - to) < 30)
    {
        return pow10[(from - to) + 30];
    }
    else
    {
        return pow(10, (from - to));
    }
}
} // namespace _detail
/// \endcond

/// \brief Calculates the conversion factor due to diffrent tags
///
/// Calculates the factor the magnitude of aquantity with units \c from needs to be multiplied
/// to be converted to a quantity with units \c to due to different unit tags, By default, this
/// returns 1. To add a conversion factor for different tags, provide an overload of this
/// function
///
/// \pre \c from is convertible to \c to
///
/// \param from The starting unit
/// \param to The target unit
/// \return The factor the magnitude of a quantity with units \c from needs to be multiplied
/// to be converted to a quantity with units \c to
constexpr double tagConversionFactor(Unit auto lhs, Unit auto rhs) noexcept
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

/// \brief Calculate the conversion factor to go from \c from to \c to
///
/// Calculates the factor the magnitude of a quantity with units \c from needs to be multiplied
/// to be converted to a quantity with units \c to
///
/// \pre \c from is convertible to \c to
///
/// \param from The starting unit
/// \param to The target unit
/// \return The factor the magnitude of a quantity with units \c from needs to be multiplied
/// to be converted to a quantity with units \c to
constexpr double conversionFactor(Unit auto from, Unit auto to) noexcept
    requires UnitConvertibleTo<from, to>
{
    double conversionFactor{1.0};
    // Convert prefixes
    conversionFactor *= _detail::conversionFactorPrefix(from.amount().multiplier(), to.amount().multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.current().multiplier(), to.current().multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.length().multiplier(), to.length().multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.luminosity().multiplier(), to.luminosity().multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.mass().multiplier(), to.mass().multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.temperature().multiplier(), to.temperature().multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.time().multiplier(), to.time().multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.multiplier(), to.multiplier());
    conversionFactor *= tagConversionFactor(from, to);
    return conversionFactor;
}

/// \brief Multiplies two units
consteval Unit auto operator*(Unit auto lhs, Unit auto rhs) noexcept
{
    return UnitType<lhs.amount() * rhs.amount(), lhs.current() * rhs.current(), lhs.length() * rhs.length(),
                    lhs.luminosity() * rhs.luminosity(), lhs.mass() * rhs.mass(), lhs.temperature() * rhs.temperature(),
                    lhs.time() * rhs.time(), typename decltype(lhs)::Tag>{};
}

/// \brief Divides two units
consteval Unit auto operator/(Unit auto lhs, Unit auto rhs) noexcept
{
    return UnitType<lhs.amount() / rhs.amount(), lhs.current() / rhs.current(), lhs.length() / rhs.length(),
                    lhs.luminosity() / rhs.luminosity(), lhs.mass() / rhs.mass(), lhs.temperature() / rhs.temperature(),
                    lhs.time() / rhs.time(), typename decltype(lhs)::Tag>{};
}

// --- Metric Prefixes ---

constexpr std::intmax_t quetta = 30;
constexpr std::intmax_t ronna  = 27;
constexpr std::intmax_t yotta  = 24;
constexpr std::intmax_t zetta  = 21;
constexpr std::intmax_t exa    = 18;
constexpr std::intmax_t peta   = 15;
constexpr std::intmax_t tera   = 12;
constexpr std::intmax_t giga   = 9;
constexpr std::intmax_t mega   = 6;
constexpr std::intmax_t kilo   = 3;
constexpr std::intmax_t hecto  = 2;
constexpr std::intmax_t deca   = 1;
constexpr std::intmax_t deci   = -1;
constexpr std::intmax_t centi  = -2;
constexpr std::intmax_t milli  = -3;
constexpr std::intmax_t micro  = -6;
constexpr std::intmax_t nano   = -9;
constexpr std::intmax_t pico   = -12;
constexpr std::intmax_t femto  = -15;
constexpr std::intmax_t atto   = -18;
constexpr std::intmax_t zepto  = -21;
constexpr std::intmax_t yocto  = -24;
constexpr std::intmax_t ronto  = -27;
constexpr std::intmax_t quecto = -30;

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

/// \brief Provides a string representation of the unit
///
/// Provides a string representation of the unit (e.g. m for meter, K for kelvin).
/// The default representation of a unit is in terms of SI base units
/// (e.g. a newton would be represented as kg * m * s^-2). To provide a custom
/// representation for the unit, specialize this string for that unit.
///
/// \tparam U The unit to provide a description for
template <Unit auto U>
inline const std::string unitString = _detail::defaultUnitName(U);
} // namespace Maxwell

#endif