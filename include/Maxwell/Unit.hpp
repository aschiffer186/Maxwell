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
#include <ratio>
#include <string>
#include <type_traits>

#include "internal/Concepts.hpp"
#include "internal/Measure.hpp"

/// \namespace Maxwell
/// \brief Public declarations to be used by consumers of the library
namespace maxwell
{
/// \class unit_type
/// \brief Definition of unit of measurement
///
/// A \c unit_type represents a unit of measurement. It represents a unit derived from the SI base units
/// (with the exception of the base unit of mass is the gram, not te kilogram). A \c unit_type is the product
/// of the base units specified by the template parameters, possibly scaled by an appropriate power of exponentiation.
/// The base units are part of the units type, so two instantiations of \c unit_type are the same type if and only if
/// they represent the same unit. If a unit does not have a particular dimension, use \c null_measure as the template
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
/// \tparam amount_ The amount dimension of the unit
/// \tparam Currrent_ The current dimension of the unit
/// \tparam length_ The length dimension of the unit
/// \tparam luminosity_ The luminosity dimension of the unit
/// \tparam mass_ The mass dimension of the unit
/// \tparam Temerature_ The temperature dimension of the unit
/// \tparam time_ The time dimension of the unit
/// \tparam tag_ The tag of the unit (see above)
/// \tparam ExtraMultiplier_ Extra multiplier for dimensionless units that are not unitless
template <internal::measure auto amount_, internal::measure auto current_, internal::measure auto length_,
          internal::measure auto luminosity_, internal::measure auto mass_, internal::measure auto temperature_,
          internal::measure auto time_, typename tag_ = void, std::intmax_t ExtraMultiplier_ = 0>
struct unit_type
{
    /// The amount dimension of the unit
    static constexpr internal::measure auto amount      = amount_;
    /// The current dimension of the unit
    static constexpr internal::measure auto current     = current_;
    /// The length dimension of the unit
    static constexpr internal::measure auto length      = length_;
    /// The luminosity dimension of the unit
    static constexpr internal::measure auto luminosity  = luminosity_;
    /// The mass dimension of the unit
    static constexpr internal::measure auto mass        = mass_;
    /// The temperature dimension of the unit
    static constexpr internal::measure auto temperature = temperature_;
    /// The time dimension of the unit
    static constexpr internal::measure auto time        = time_;
    /// The tag of the unit
    using tag                                           = tag_;

    /// \brief Return the amount dimension of the unit
    /// \return The amount dimension of the unit
    consteval internal::measure auto get_amount() const noexcept
    {
        return amount;
    }

    /// \brief Return the current dimension of the unit
    /// \return The current dimension of the unit
    consteval internal::measure auto get_current() const noexcept
    {
        return current;
    }

    /// \brief Return the length dimension of the unit
    /// \return The length dimension of the unit
    consteval internal::measure auto get_length() const noexcept
    {
        return length;
    }

    /// \brief Return the luminosity dimension of the unit
    /// \return The luminosity dimension of the unit
    consteval internal::measure auto get_luminosity() const noexcept
    {
        return luminosity;
    }

    /// \brief Return the mass dimension of the unit
    /// \return The mass dimension of the unit
    consteval internal::measure auto get_mass() const noexcept
    {
        return mass;
    }

    /// \brief Return the temperature dimension of the unit
    /// \return The temperature dimension of the unit
    consteval internal::measure auto get_temperature() const noexcept
    {
        return temperature;
    }

    /// \brief Return the time dimension of the unit
    /// \return The time dimension of the unit
    consteval internal::measure auto get_time() const noexcept
    {
        return time;
    }

    /// \brief Return the extra multiplier
    /// \return The extra multiplier of the unit
    consteval std::intmax_t get_multiplier() const noexcept
    {
        return ExtraMultiplier_;
    }

    /// \brief Adds a tag to the unit
    ///
    /// Creates a new \c unit_type with the same dimensions as \c *this,
    /// but with the specified tag.
    ///
    /// \tparam T the tag to add
    /// \return the modified unit
    template <typename T>
    consteval auto add_tag() const noexcept
    {
        return unit_type<amount, current, length, luminosity, mass, temperature, time, T>{};
    }

    /// \brief Adjusts the multiplier of the amount dimension
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the amount dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().amount().magnitude() == this->amount().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the amount dimension by
    /// \return A new unit with the amount multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_amount() const noexcept
    {
        return unit_type<amount.template adjust_multiplier<Adjustment>(), current, length, luminosity, mass,
                         temperature, time, tag>{};
    }

    /// \brief Adjusts the multiplier of the current dimension
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the current dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().current().magnitude() == this->current().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the current dimension by
    /// \return A new unit with the current multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_current() const noexcept
    {
        return unit_type<amount, current.template adjust_multiplier<Adjustment>(), length, luminosity, mass,
                         temperature, time, tag>{};
    }

    /// \brief Adjusts the multiplier of the length dimension
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the length dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().length().magnitude() == this->length().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the length dimension by
    /// \return A new unit with the length multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_length() const noexcept
    {
        return unit_type<amount, current, length.template adjust_multiplier<Adjustment>(), luminosity, mass,
                         temperature, time, tag>{};
    }

    /// \brief Adjusts the multiplier of the luminosity dimension
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the luminosity dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().luminosity().magnitude() == this->luminosity().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the luminosity dimension by
    /// \return A new unit with the luminosity multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_luminosity() const noexcept
    {
        return unit_type<amount, current, length, luminosity.template adjust_multiplier<Adjustment>(), mass,
                         temperature, time, tag>{};
    }

    /// \brief Adjusts the multiplier of the mass dimension
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the mass dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().mass().magnitude() == this->mass().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the mass dimension by
    /// \return A new unit with the mass multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_mass() const noexcept
    {
        return unit_type<amount, current, length, luminosity, mass.template adjust_multiplier<Adjustment>(),
                         temperature, time, tag>{};
    }

    /// \brief Adjusts the multiplier of the temperatue dimension
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the temperatue dimension
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().temperatue().magnitude() == this->temperatue().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the temperatue dimension by
    /// \return A new unit with the temperatue multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_temperature() const noexcept
    {
        return unit_type<amount, current, length, luminosity, mass,
                         temperature.template adjust_multiplier<Adjustment>(), time, tag>{};
    }

    /// \brief Adjusts the multiplier of the time dimension
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the time dimension
    ///
    /// \post The unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().time().magnitude() == this->time().magnitude() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the multiplier of the time dimension by
    /// \return A new unit with the time multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_time() const noexcept
    {
        return unit_type<amount, current, length, luminosity, mass, temperature,
                         time.template adjust_multiplier<Adjustment>(), tag>{};
    }

    /// \brief Adjusts the extra multiplier of the unit
    ///
    /// Returns a new \c unit_type with the same dimensions as \c *this, but
    /// with an adjusted multiplier for the extra multiplier
    ///
    /// \post the unit returned has the same dimensions as \c *this and
    /// \code
    ///     adjust_multiplier<Adjustment().get_multiplier() == this->multiplier() + Adjstument
    /// \endcode
    /// \post \c *this is unmodified
    ///
    /// \tparam The amount to adjust the extra multiplier by
    /// \return A new unit with the extra multiplier adjusted
    template <std::intmax_t Adjustment>
    consteval auto adjust_multiplier_extra() const noexcept
    {
        return unit_type<amount, current, length, luminosity, mass, temperature, time, tag,
                         ExtraMultiplier_ + Adjustment>{};
    }

    /// \brief Adjusts the scale factor of the amount dimension of the unit
    ///
    /// Returns a new \c unit_type with the same dimension, but whose amount dimension
    /// has been multiplied by the specified value.
    ///
    /// \post The scale factor of the amount dimension of the return unit is equal to the scale factor
    ///       of \c *this time adjustment
    ///
    /// \tparam Adjustment The amount to multiply the scale factor of the amount dimension by
    /// \return A \c unit_type whose amount dimension has been scaled
    template <internal::_detail::ratio_like Adjustment>
    consteval auto adjust_scale_amount() const noexcept
    {
        constexpr internal::measure auto oldMeasure = amount;
        using OldScale                              = decltype(oldMeasure)::scale;
        using NewScale                              = std::ratio_multiply<OldScale, Adjustment>;
        constexpr internal::measure_type<oldMeasure.get_power(), oldMeasure.get_multiplier(), NewScale,
                                         typename decltype(oldMeasure)::offset>
            newMeasure{};
        return unit_type<newMeasure, current, length, luminosity, mass, temperature, time, tag, ExtraMultiplier_>{};
    }

    /// \brief Adjusts the scale factor of the current dimension of the unit
    ///
    /// Returns a new \c unit_type with the same dimension, but whose current dimension
    /// has been multiplied by the specified value.
    ///
    /// \post The scale factor of the current dimension of the return unit is equal to the scale factor
    ///       of \c *this time adjustment
    ///
    /// \tparam Adjustment The amount to multiply the scale factor of the current dimension by
    /// \return A \c unit_type whose current dimension has been scaled
    template <internal::_detail::ratio_like Adjustment>
    consteval auto adjust_scale_current() const noexcept
    {
        constexpr internal::measure auto oldMeasure = current;
        using OldScale                              = decltype(oldMeasure)::scale;
        using NewScale                              = std::ratio_multiply<OldScale, Adjustment>;
        constexpr internal::measure_type<oldMeasure.get_power(), oldMeasure.get_multiplier(), NewScale,
                                         typename decltype(oldMeasure)::offset>
            newMeasure{};
        return unit_type<amount, newMeasure, length, luminosity, mass, temperature, time, tag, ExtraMultiplier_>{};
    }

    /// \brief Adjusts the scale factor of the length dimension of the unit
    ///
    /// Returns a new \c unit_type with the same dimension, but whose length dimension
    /// has been multiplied by the specified value.
    ///
    /// \post The scale factor of the length dimension of the return unit is equal to the scale factor
    ///       of \c *this time adjustment
    ///
    /// \tparam Adjustment The amount to multiply the scale factor of the length dimension by
    /// \return A \c unit_type whose length dimension has been scaled
    template <internal::_detail::ratio_like Adjustment>
    consteval auto adjust_scale_length() const noexcept
    {
        constexpr internal::measure auto oldMeasure = length;
        using OldScale                              = decltype(oldMeasure)::scale;
        using NewScale                              = std::ratio_multiply<OldScale, Adjustment>;
        constexpr internal::measure_type<oldMeasure.get_power(), oldMeasure.get_multiplier(), NewScale,
                                         typename decltype(oldMeasure)::offset>
            newMeasure{};
        return unit_type<amount, current, newMeasure, luminosity, mass, temperature, time, tag, ExtraMultiplier_>{};
    }

    /// \brief Adjusts the scale factor of the luminosity dimension of the unit
    ///
    /// Returns a new \c unit_type with the same dimension, but whose luminosity dimension
    /// has been multiplied by the specified value.
    ///
    /// \post The scale factor of the luminosity dimension of the return unit is equal to the scale factor
    ///       of \c *this time adjustment
    ///
    /// \tparam Adjustment The amount to multiply the scale factor of the luminosity dimension by
    /// \return A \c unit_type whose amount dimension has been scaled
    template <internal::_detail::ratio_like Adjustment>
    consteval auto adjust_scale_luminosity() const noexcept
    {
        constexpr internal::measure auto oldMeasure = luminosity;
        using OldScale                              = decltype(oldMeasure)::scale;
        using NewScale                              = std::ratio_multiply<OldScale, Adjustment>;
        constexpr internal::measure_type<oldMeasure.get_power(), oldMeasure.get_multiplier(), NewScale,
                                         typename decltype(oldMeasure)::offset>
            newMeasure{};
        return unit_type<amount, current, length, newMeasure, mass, temperature, time, tag, ExtraMultiplier_>{};
    }

    /// \brief Adjusts the scale factor of the mass dimension of the unit
    ///
    /// Returns a new \c unit_type with the same dimension, but whose mass dimension
    /// has been multiplied by the specified value.
    ///
    /// \post The scale factor of the mass dimension of the return unit is equal to the scale factor
    ///       of \c *this time adjustment
    ///
    /// \tparam Adjustment The amount to multiply the scale factor of the mass dimension by
    /// \return A \c unit_type whose mass dimension has been scaled
    template <internal::_detail::ratio_like Adjustment>
    consteval auto adjust_scale_mass() const noexcept
    {
        constexpr internal::measure auto oldMeasure = mass;
        using OldScale                              = decltype(oldMeasure)::scale;
        using NewScale                              = std::ratio_multiply<OldScale, Adjustment>;
        constexpr internal::measure_type<oldMeasure.get_power(), oldMeasure.get_multiplier(), NewScale,
                                         typename decltype(oldMeasure)::offset>
            newMeasure{};
        return unit_type<amount, current, length, luminosity, newMeasure, temperature, time, tag, ExtraMultiplier_>{};
    }

    /// \brief Adjusts the scale factor of the temperature dimension of the unit
    ///
    /// Returns a new \c unit_type with the same dimension, but whose temperature dimension
    /// has been multiplied by the specified value.
    ///
    /// \post The scale factor of the temperature dimension of the return unit is equal to the scale factor
    ///       of \c *this time adjustment
    ///
    /// \tparam Adjustment The amount to multiply the scale factor of the temperature dimension by
    /// \return A \c unit_type whose temperature dimension has been scaled
    template <internal::_detail::ratio_like Adjustment>
    consteval auto adjust_scale_temperature() const noexcept
    {
        constexpr internal::measure auto oldMeasure = temperature;
        using OldScale                              = decltype(oldMeasure)::scale;
        using NewScale                              = std::ratio_multiply<OldScale, Adjustment>;
        constexpr internal::measure_type<oldMeasure.get_power(), oldMeasure.get_multiplier(), NewScale,
                                         typename decltype(oldMeasure)::offset>
            newMeasure{};
        return unit_type<amount, current, length, luminosity, mass, newMeasure, time, tag, ExtraMultiplier_>{};
    }

    /// \brief Adjusts the scale factor of the time dimension of the unit
    ///
    /// Returns a new \c unit_type with the same dimension, but whose time dimension
    /// has been multiplied by the specified value.
    ///
    /// \post The scale factor of the time dimension of the return unit is equal to the scale factor
    ///       of \c *this time adjustment
    ///
    /// \tparam Adjustment The amount to multiply the scale factor of the time dimension by
    /// \return A \c unit_type whose time dimension has been scaled
    template <internal::_detail::ratio_like Adjustment>
    consteval auto adjust_scale_time() const noexcept
    {
        constexpr internal::measure auto oldMeasure = time;
        using OldScale                              = decltype(oldMeasure)::scale;
        using NewScale                              = std::ratio_multiply<OldScale, Adjustment>;
        constexpr internal::measure_type<oldMeasure.get_power(), oldMeasure.get_multiplier(), NewScale,
                                         typename decltype(oldMeasure)::offset>
            newMeasure{};
        return unit_type<amount, current, length, luminosity, mass, temperature, newMeasure, tag, ExtraMultiplier_>{};
    }

    /// \brief Converts a unit to SI base units
    ///
    /// Returns a new \c unit_type with the same dimesions as \c *this, but expressed
    /// entirely in SI base units.
    ///
    /// \post The retuned unit is in SI base units.
    ///
    /// \return The equivalent unit in SI base units
    consteval auto to_SI_base_units() const noexcept
    {
        const unit_type<amount.to_coherent_measure(), current.to_coherent_measure(), length.to_coherent_measure(),
                        luminosity.to_coherent_measure(), mass.to_coherent_measure(), temperature.to_coherent_measure(),
                        time.to_coherent_measure(), void>
            u{};
        return u.template adjust_multiplier_mass<3>();
    }
};

// --- Base units ---

/// SI unit "mole"
constexpr unit_type<internal::base_measure, internal::null_measure, internal::null_measure, internal::null_measure,
                    internal::null_measure, internal::null_measure, internal::null_measure>
    mole_unit;
/// Type alias for SI unit "mole"
using mole_unit_type = std::remove_const_t<decltype(mole_unit)>;

/// SI unit "ampere"
constexpr unit_type<internal::null_measure, internal::base_measure, internal::null_measure, internal::null_measure,
                    internal::null_measure, internal::null_measure, internal::null_measure>
    ampere_unit;
/// Type alias for SI unit "ampere"
using ampere_unit_type = std::remove_const_t<decltype(ampere_unit)>;

/// SI unit "meter"
constexpr unit_type<internal::null_measure, internal::null_measure, internal::base_measure, internal::null_measure,
                    internal::null_measure, internal::null_measure, internal::null_measure>
    meter_unit;
/// Type alias for SI unit "meter"
using meter_unit_type = std::remove_const_t<decltype(meter_unit)>;

/// SI unit "candela"
constexpr unit_type<internal::null_measure, internal::null_measure, internal::null_measure, internal::base_measure,
                    internal::null_measure, internal::null_measure, internal::null_measure>
    candela_unit;
/// Type alias for SI unit "candela"
using candela_unit_type = std::remove_const_t<decltype(candela_unit)>;

/// SI unit "gram"
constexpr unit_type<internal::null_measure, internal::null_measure, internal::null_measure, internal::null_measure,
                    internal::base_measure, internal::null_measure, internal::null_measure>
    gram_unit;
/// Type alias for SI unit "gram"
using gram_unit_type = std::remove_const_t<decltype(gram_unit)>;

/// SI unit "Kelvin"
constexpr unit_type<internal::null_measure, internal::null_measure, internal::null_measure, internal::null_measure,
                    internal::null_measure, internal::base_measure, internal::null_measure>
    kelvin_unit;
/// Type alias for SI unit Kelvin
using kelvin_unit_type = std::remove_const_t<decltype(kelvin_unit)>;

/// SI unit "second"
constexpr unit_type<internal::null_measure, internal::null_measure, internal::null_measure, internal::null_measure,
                    internal::null_measure, internal::null_measure, internal::base_measure>
    second_unit;
/// Type alias for SI unit "second"
using second_unit_type = std::remove_const_t<decltype(second_unit)>;

/// Constant indidcating lack of units
// constexpr unit_type<internal::null_measure, internal::null_measure, internal::null_measure, internal::null_measure,
//                     internal::null_measure, internal::null_measure, internal::null_measure>
//     unitless_unit;
// /// Type alias for constant indicating lack of units
using unitless_unit_type =
    unit_type<internal::null_measure, internal::null_measure, internal::null_measure, internal::null_measure,
              internal::null_measure, internal::null_measure, internal::null_measure>;

// -- Unit Concepts ---

/// \cond
namespace _detail
{
template <typename>
struct _is_unit : std::false_type
{
};

template <internal::measure auto amount_, internal::measure auto current_, internal::measure auto length_,
          internal::measure auto luminosity_, internal::measure auto mass_, internal::measure auto temperature_,
          internal::measure auto time_, typename tag_, std::intmax_t Extra_>
struct _is_unit<unit_type<amount_, current_, length_, luminosity_, mass_, temperature_, time_, tag_, Extra_>>
    : std::true_type
{
};
} // namespace _detail
/// \endcond

/// \brief Specifies a type is an instantiation of the \c unit_type class template
///
/// Specifies a type is an instantiation of the \c unit_type class template, ignoring
/// cv-qualifiers and references.
///
/// \tparam U The type to check
template <typename U>
concept unit = _detail::_is_unit<std::remove_cvref_t<U>>::value;

/// \brief Specifies a unit is unitless
///
/// \tparam U The unit to check
template <auto U>
concept unitless_unit = internal::similar<decltype(U), unitless_unit_type>;

/// \brief Specfies amount dimension \c From can be converted to amount dimension \c To
///
/// Specifies amount dimension \c From can be converted to amount dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept amount_convertible_to =
    internal::measure<decltype(From)> && internal::measure<decltype(To)> && internal::is_measure_convertible(From, To);

/// \brief Specfies current dimension \c From can be converted to current dimension \c To
///
/// Specifies current dimension \c From can be converted to current dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept current_convertible_to =
    internal::measure<decltype(From)> && internal::measure<decltype(To)> && internal::is_measure_convertible(From, To);

/// \brief Specfies length dimension \c From can be converted to length dimension \c To
///
/// Specifies length dimension \c From can be converted to length dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept length_convertible_to =
    internal::measure<decltype(From)> && internal::measure<decltype(To)> && internal::is_measure_convertible(From, To);

/// \brief Specfies luminosity dimension \c From can be converted to luminosity dimension \c To
///
/// Specifies luminosity dimension \c From can be converted to luminosity dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept luminosity_convertible_to =
    internal::measure<decltype(From)> && internal::measure<decltype(To)> && internal::is_measure_convertible(From, To);

/// \brief Specfies mass dimension \c From can be converted to mass dimension \c To
///
/// Specifies mass dimension \c From can be converted to mass dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept mass_convertible_to =
    internal::measure<decltype(From)> && internal::measure<decltype(To)> && internal::is_measure_convertible(From, To);

/// \brief Specfies temperature dimension \c From can be converted to temperature dimension \c To
///
/// Specifies temperature dimension \c From can be converted to temperature dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept temperature_convertible_to =
    internal::measure<decltype(From)> && internal::measure<decltype(To)> && internal::is_measure_convertible(From, To);

/// \brief Specfies time dimension \c From can be converted to time dimension \c To
///
/// Specifies time dimension \c From can be converted to time dimension \c To,
/// this conversion is possible if both dimensions have the same power.
///
/// \tparam From The starting dimension
/// \tparam To The target dimension
template <auto From, auto To>
concept time_convertible_to =
    internal::measure<decltype(From)> && internal::measure<decltype(To)> && internal::is_measure_convertible(From, To);

/// \brief Type trait indicating two tags are convertible
///
/// Type trait indicating two tags are convertible. To indicate
/// two tags are convertible, specialize this trait for the tags
/// and inherit from \c std::true_type. By default, two different
/// tags are not convertible. Note, this trait should be specialized
/// for both \c From to \c To and \c To to \c From.
///
/// \sa \c tag_convertible_to
/// \tparam From The starting tag
/// \tparam To The target tag
template <typename From, typename To>
struct is_tag_convertible : std::is_same<From, To>
{
};

/// \brief Specfies tag \c From can be converted to \c tag to.
///
/// Specfies tag \c From can be converted to \c tag to. This concept evaluates
/// to \c false unless the struct \c is_tag_convertible is specialized for
/// \c From and \c To
///
/// \sa \c is_tag_convertible
/// \tparam From The starting tag
/// \tparam To The target tag
template <typename From, typename To>
concept tag_convertible_to = is_tag_convertible<From, To>::value;

/// \brief Specifies unit \c From can be converted to unit \c To
///
/// Specifies unit \c From can be converted to unit \c To. \c From can be converted to
/// \c To if all of all the dimensions of \c From can be converted to the corresponding
/// dimensions of \c To and the tag of \c From can be converted to the \c tag of to.
///
/// Convertibility is a symmetric relation, if \c From is convertible to \c To, then \c To
/// is convertible to \c From.
///
/// \tparam From The starting unit
/// \tparam To the target unit
template <auto From, auto To>
concept unit_convertible_to =
    unit<decltype(From)> && unit<decltype(To)> && amount_convertible_to<From.get_amount(), To.get_amount()> &&
    current_convertible_to<From.get_current(), To.get_current()> &&
    length_convertible_to<From.get_length(), To.get_length()> &&
    luminosity_convertible_to<From.get_luminosity(), To.get_luminosity()> &&
    mass_convertible_to<From.get_mass(), To.get_mass()> &&
    temperature_convertible_to<From.get_temperature(), To.get_temperature()> &&
    time_convertible_to<From.get_time(), To.get_time()> &&
    tag_convertible_to<typename decltype(From)::tag, typename decltype(To)::tag>;

// --- Unit Traits ---
/// \brief Specifies a unit has dimensions of amount
///
/// Specifies a unit has dimensions of amount, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept amount_unit = unit_convertible_to<U, mole_unit>;

/// \brief Specifies a unit has dimensions of current
///
/// Specifies a unit has dimensions of current, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept current_unit = unit_convertible_to<U, ampere_unit>;

/// \brief Specifies a unit has dimensions of length
///
/// Specifies a unit has dimensions of length, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept length_unit = unit_convertible_to<U, meter_unit>;

/// \brief Specifies a unit has dimensions of luminosity
///
/// Specifies a unit has dimensions of luminosity, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept luminosity_unit = unit_convertible_to<U, candela_unit>;

/// \brief Specifies a unit has dimensions of mass
///
/// Specifies a unit has dimensions of mass, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept mass_unit = unit_convertible_to<U, gram_unit>;

/// \brief Specifies a unit has dimensions of temperature
///
/// Specifies a unit has dimensions of temperature, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept temperature_unit = unit_convertible_to<U, kelvin_unit>;

/// \brief Specifies a unit has dimensions of time
///
/// Specifies a unit has dimensions of time, regardless of scaling and offset
///
/// \tparam U The unit to check
template <auto U>
concept time_unit = unit_convertible_to<U, second_unit>;
// --- Unit comparisons ---

/// \brief Compare two units for equality
///
/// Compares two units for equality; two units are equal if and only if they
/// have the same dimension and tag. This implies the units have the same type
/// up to cvref-qualifiers.
///
/// \param lhs the first unit to compare for equality
/// \param rhs th second unit to compare for equality
constexpr bool operator==(unit auto lhs, unit auto rhs) noexcept
{
    return internal::similar<decltype(lhs), decltype(rhs)>;
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

template <internal::_detail::ratio_like From, internal::_detail::ratio_like To>
constexpr double conversionFactorOffset() noexcept
{
    using ResRatio = std::ratio_divide<To, From>;
    return static_cast<double>(ResRatio::num) / static_cast<double>(ResRatio::den);
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
constexpr double tag_conversion_factor(unit auto lhs, unit auto rhs) noexcept
{
    if constexpr (std::same_as<typename decltype(lhs)::tag, typename decltype(rhs)::tag>)
    {
        return 1.0;
    }
    else
    {
        static_assert(_detail::dependentFalse<typename decltype(lhs)::tag>,
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
constexpr double conversion_factor(unit auto from, unit auto to) noexcept
    requires unit_convertible_to<from, to>
{
    using From = decltype(from);
    using To   = decltype(to);

    double conversionFactor{1.0};
    // Convert prefixes
    conversionFactor *=
        _detail::conversionFactorPrefix(from.get_amount().get_multiplier(), to.get_amount().get_multiplier());
    conversionFactor *=
        _detail::conversionFactorPrefix(from.get_current().get_multiplier(), to.get_current().get_multiplier());
    conversionFactor *=
        _detail::conversionFactorPrefix(from.get_length().get_multiplier(), to.get_length().get_multiplier());
    conversionFactor *=
        _detail::conversionFactorPrefix(from.get_luminosity().get_multiplier(), to.get_luminosity().get_multiplier());
    conversionFactor *=
        _detail::conversionFactorPrefix(from.get_mass().get_multiplier(), to.get_mass().get_multiplier());
    conversionFactor *=
        _detail::conversionFactorPrefix(from.get_temperature().get_multiplier(), to.get_temperature().get_multiplier());
    conversionFactor *=
        _detail::conversionFactorPrefix(from.get_time().get_multiplier(), to.get_time().get_multiplier());
    conversionFactor *= _detail::conversionFactorPrefix(from.get_multiplier(), to.get_multiplier());
    conversionFactor *= tag_conversion_factor(from, to);

    // Convert ratios
    conversionFactor *=
        _detail::conversionFactorOffset<typename decltype(From::amount)::scale, typename decltype(To::amount)::scale>();
    conversionFactor *= _detail::conversionFactorOffset<typename decltype(From::current)::scale,
                                                        typename decltype(To::current)::scale>();
    conversionFactor *=
        _detail::conversionFactorOffset<typename decltype(From::length)::scale, typename decltype(To::length)::scale>();
    conversionFactor *= _detail::conversionFactorOffset<typename decltype(From::luminosity)::scale,
                                                        typename decltype(To::luminosity)::scale>();
    conversionFactor *=
        _detail::conversionFactorOffset<typename decltype(From::mass)::scale, typename decltype(To::mass)::scale>();
    conversionFactor *= _detail::conversionFactorOffset<typename decltype(From::temperature)::scale,
                                                        typename decltype(To::temperature)::scale>();
    conversionFactor *=
        _detail::conversionFactorOffset<typename decltype(From::time)::scale, typename decltype(To::time)::scale>();
    return conversionFactor;
}

/// \brief Multiplies two units
///
/// Multiplies two units; the dimensions of the resulting unit is the product of
/// the dimensions of lhs and rhs. This function carries forward the tag and the multipliler
/// of the first unit in the multiplication.
///
/// \param lhs The left hand side of the multiplication
/// \param rhs The right hand side of the multiplication
/// \return The product of two units
consteval unit auto operator*(unit auto lhs, unit auto rhs) noexcept
{
    return unit_type<lhs.get_amount() * rhs.get_amount(), lhs.get_current() * rhs.get_current(),
                     lhs.get_length() * rhs.get_length(), lhs.get_luminosity() * rhs.get_luminosity(),
                     lhs.get_mass() * rhs.get_mass(), lhs.get_temperature() * rhs.get_temperature(),
                     lhs.get_time() * rhs.get_time(), typename decltype(lhs)::tag, lhs.get_multiplier()>{};
}

/// \brief Divides two units
///
/// Divides two units; the dimensions of the resulting unit is the division of
/// the dimensions of lhs and rhs. This function carries forward the tag and the multipliler
/// of the first unit in the multiplication.
///
/// \param lhs The dividend
/// \param rhs The divisor
/// \return The quotient of two units
consteval unit auto operator/(unit auto lhs, unit auto rhs) noexcept
{
    return unit_type<lhs.get_amount() / rhs.get_amount(), lhs.get_current() / rhs.get_current(),
                     lhs.get_length() / rhs.get_length(), lhs.get_luminosity() / rhs.get_luminosity(),
                     lhs.get_mass() / rhs.get_mass(), lhs.get_temperature() / rhs.get_temperature(),
                     lhs.get_time() / rhs.get_time(), typename decltype(lhs)::tag>{};
}

// --- Metric Prefixes ---

/// Metric prefix "quetta" (10^30)
constexpr std::intmax_t quetta = 30;
/// Metric prefix ronna (10^27)
constexpr std::intmax_t ronna  = 27;
/// Metric prefix yotta (10^24)
constexpr std::intmax_t yotta  = 24;
/// Metric prefix zetta (10^21)
constexpr std::intmax_t zetta  = 21;
/// Metric prefix exa (10^18)
constexpr std::intmax_t exa    = 18;
/// Metric prefix peta (10^15)
constexpr std::intmax_t peta   = 15;
/// Metric prefix tera (10^12)
constexpr std::intmax_t tera   = 12;
/// Metric prefix giga (10^9)
constexpr std::intmax_t giga   = 9;
/// Metric prefix mega (10^6)
constexpr std::intmax_t mega   = 6;
/// Metric prefix kilo (10^3)
constexpr std::intmax_t kilo   = 3;
/// Metric prefix hecto (10^2)
constexpr std::intmax_t hecto  = 2;
/// Metric prefix deca (10^1)
constexpr std::intmax_t deca   = 1;
/// Metric prefix deci (10^-1)
constexpr std::intmax_t deci   = -1;
/// Metric prefix centi (10^-2)
constexpr std::intmax_t centi  = -2;
/// Metric prefix milli (10^-3)
constexpr std::intmax_t milli  = -3;
/// Metric prefix micro (10^-6)
constexpr std::intmax_t micro  = -6;
/// Metric prefix nano (10^-9)
constexpr std::intmax_t nano   = -9;
/// Metric prefix pico (10^-12)
constexpr std::intmax_t pico   = -12;
/// Metric prefix femto (10^-15)
constexpr std::intmax_t femto  = -15;
/// Metric prefix atto (10^-18)
constexpr std::intmax_t atto   = -18;
/// Metric prefix zepto (10^-21)
constexpr std::intmax_t zepto  = -21;
/// Metric prefix yocto (10^-24)
constexpr std::intmax_t yocto  = -24;
/// Metric prefix ronto (10^-27)
constexpr std::intmax_t ronto  = -27;
/// Metric prefix quecto (10^-30)
constexpr std::intmax_t quecto = -30;

// --- Formatting ---
/// \cond
namespace _detail
{
std::string defaultUnitName(unit auto)
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
template <unit auto U>
inline const std::string unit_string = _detail::defaultUnitName(U);
} // namespace maxwell

#endif