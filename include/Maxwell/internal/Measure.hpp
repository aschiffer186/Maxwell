///
/// \file Measure.hpp
/// \author Alex Schiffer
/// \brief Definition of measure type and associated functions
/// \version 0.1
/// \date 2024-11-02
///
/// \copyright Copyright (c) 2024
///
///
#ifndef MEASURE_HPP
#define MEASURE_HPP

#include <concepts>
#include <cstdint>
#include <ratio>
#include <type_traits>

/// \namespace Maxwell::Internal
/// \brief Declarations internal to Maxwell
namespace maxwell::internal
{
// --- Measure Type Defintion ---

/// \cond
namespace _detail
{
using zero = std::ratio<0, 1>;
using one  = std::ratio<1, 1>;

template <typename>
struct is_ratio : std::false_type
{
};

template <std::intmax_t Num, std::intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> : std::true_type
{
};

template <typename Tp>
concept ratio_like = is_ratio<Tp>::value;
} // namespace _detail
/// \endcond

/// \class measure_type
/// \brief Represents one fundamental quantity in the SI system
///
/// <tt>class measure_type</tt> represents one dimension used
/// in the SI system. It consists of a base unit and the transformations
/// applied to the base unit to convert it to a derived unit repersenting
/// that dimension. The unit represented by this class is given by
///
/// \f$Unit = (10^{multiplier} * scale::num/scale::den * Measure^{power}) + offset::num/offset::den\f$
///
/// The transformations applied to the measure are part of the
/// measure's type; different measres are different types.
///
/// \tparam power_ the power the unit is raised to
/// \tparam multiplier_ the metric prefix applied to the unit
/// \tparam scale_ the scale factor of the unit
/// \tparam offset_ the offset of the unit
template <std::intmax_t power_, std::intmax_t multiplier_, _detail::ratio_like scale_ = _detail::one,
          _detail::ratio_like offset_ = _detail::zero>
struct measure_type
{
    /// The power the unit is raised to
    static constexpr std::intmax_t power      = power_;
    /// The metric prefix of the unit
    static constexpr std::intmax_t multiplier = multiplier_;
    /// The scale factor of the unit
    using scale                               = scale_;
    /// The offset of the unit
    using offset                              = offset_;

    /// \fn get_power
    /// \brief Returns the power the measure is raised to
    ///
    /// \return The power the measure is raised to
    consteval std::intmax_t get_power() const noexcept
    {
        return power;
    }

    /// \fn get_multiplier
    /// \brief Returns the metric prefix of the measure
    /// \return The metric prefix of the measure
    std::intmax_t consteval get_multiplier() const noexcept
    {
        return multiplier;
    }

    /// \fn adjustmultiplier
    /// \brief Adjusts the multiplier by the specified amount
    ///
    /// Creates a new measure whose multiplier is equivalent to
    /// \code multiplier + adjustment \endcode
    ///
    /// \post The returned measure has a power of \c power and a
    ///       multiplier equal to <tt>multiplier + adjustment</tt>
    ///
    /// \tparam adjustment the amount to adjust the measure by
    /// \return A new masure with the adjusted prefix
    template <std::intmax_t adjustment>
    consteval auto adjust_multiplier() const noexcept
    {
        return measure_type<power, multiplier + adjustment, scale, offset>{};
    }

    /// \fn is_base_measure
    /// \brief Returns true if the measure is a base measure
    ///
    /// Returns true if the measure is a base measure, i.e. if the
    /// following condition is true
    /// \code
    /// power == 1 && multiplier == 0
    /// \endcode
    /// In general, this corresponds with an SI base unit except
    /// for mass, where this measure returns true for gram, not kilogram.
    /// \return true if the measure is a base measure
    consteval bool is_base_measure() const noexcept
    {
        return power_ == 1 && multiplier_ == 0 && std::ratio_equal_v<scale, _detail::one> &&
               std::ratio_equal_v<offset, _detail::zero>;
    }

    /// \fn is_coherent_measure
    /// \brief Returns true if the measure is a coherent measure
    ///
    /// Returns true if the measure is a coherent measure, i.e.
    /// if it has no multiplier.
    ///
    /// \return true if the measuer is a coherent measure
    consteval bool is_coherent_measure() const noexcept
    {
        return multiplier_ == 0 && std::ratio_equal_v<scale, _detail::one> && std::ratio_equal_v<offset, _detail::zero>;
    }

    /// \fn to_coherent_measure
    /// \brief Returns a coherent version of the measure
    ///
    /// Returns a new \c measure_type with the same power that is a
    /// coherent version of \c *this
    ///
    /// \post The returned \c measure_type has a multiplier of 0
    /// \return a coherent version of \c *this
    consteval auto to_coherent_measure() const noexcept
    {
        return measure_type<power, 0, _detail::one, _detail::zero>{};
    }
};

/// Compile-time constant represeting base measure
constexpr measure_type<1, 0, _detail::one, _detail::zero> base_measure;
/// Compile-time constant representing a null measure
/// (measure that doesn't contribute to the unit's definition)
constexpr measure_type<0, 0, _detail::one, _detail::zero> null_measure;

// -- Measure Type Concepts --

/// \cond
namespace _detail
{
template <typename>
struct _is_measure : std::false_type
{
};

template <std::intmax_t power_, std::intmax_t multiplier, _detail::ratio_like scale_, _detail::ratio_like offset_>
struct _is_measure<measure_type<power_, multiplier, scale_, offset_>> : std::true_type
{
};
} // namespace _detail
/// \endcond

/// \brief Specifies a type is an instantiation of the \c measure_type class template
///
/// Specifies a type is an instantiation of the \c measure_type class template, ignoring cv-qualfiers
/// and references.
///
/// \tparam M the type to check
template <typename M>
concept measure = _detail::_is_measure<std::remove_cvref_t<M>>::value;

// --- Measure Conversion ---

/// \brief Return if measure is convertible to another measure
///
/// Specifies \c From is convertible to \c To. Two measures are
/// convertible if there is a constant C such that
/// \code
/// C*From == To
/// \endcode.
/// This is true if they have the same power. Note, this concept is
/// a property of values, not types.
///
/// \param from the starting measure
/// \param to the target measure
consteval bool is_measure_convertible(measure auto from, measure auto to) noexcept
{
    return from.get_power() == to.get_power();
}

// --- Measure Type Comparison ---

/// \brief Compares two \c measure_types for equality
///
/// Compares two \c measure_types for equality.
/// Two \c measure_types are equal if and only if they have the
/// same power, multiplier, scale factor, and offset. This implies they are the same type.
///
/// \param lhs one measure to compare for equality
/// \param rhs the other measure to compare for equality
///
/// \return true if the measures are equal
constexpr bool operator==(measure auto lhs, measure auto rhs) noexcept
{
    return std::same_as<std::remove_cvref_t<decltype(lhs)>, std::remove_cvref_t<decltype(rhs)>>;
}

/// \brief Multiplies two measures
///
/// Multiplies two measures. The resulting measure has a power that is the sum of the
/// power of \c lhs and \c rhs. If the two inputs have the same multiplier, scale factor,
/// and offset, they are copied to the resulting unit. Otherwise, the returned unit is a coherent unit.
///
/// \param lhs one measure to multiply
/// \param rhs the other measure to multiply
/// \return the product of \c lhs and \c rhs
constexpr measure auto operator*(measure auto lhs, measure auto rhs) noexcept
{
    using LHSType = decltype(lhs);
    using RHSType = decltype(rhs);

    constexpr auto multiplier = (lhs.get_multiplier() == rhs.get_multiplier())
                                    ? lhs.get_multiplier()
                                    : ((rhs.get_multiplier() == 0)   ? lhs.get_multiplier()
                                       : (lhs.get_multiplier() == 0) ? rhs.get_multiplier()
                                                                     : 0);

    // if constexpr (lhs.multiplier() == rhs.multiplier() &&
    //               std::ratio_equal_v<typename LHSType::offset, typename RHSType::offset>)
    // {
    //     return measure_type<lhs.power() + rhs.power(), lhs.multiplier(), typename LHSType::scale,
    //                        typename RHSType::offset>{};
    // }
    // else
    // {
    //     return measure_type<lhs.power() + rhs.power(), 0, _detail::one, _detail::zero>{};
    // }
    return measure_type<lhs.get_power() + rhs.get_power(), multiplier,
                        std::ratio_multiply<typename LHSType::scale, typename RHSType::scale>, _detail::zero>{};
}

/// \brief Divides two measures
///
/// Dividies two measures. The resulting measure has a power that is the difference of the
/// power of \c lhs and \c rhs. If the two inputs have the same multiplier, scale factor,
/// and offset, they are copied to the resulting unit. Otherwise, the returned unit is a coherent unit.
///
/// \param lhs the divident
/// \param rhs the divisor
/// \return the quotient of \c lhs and \c rhs
constexpr measure auto operator/(measure auto lhs, measure auto rhs) noexcept
{
    using LHSType = decltype(lhs);
    using RHSType = decltype(rhs);

    constexpr auto multiplier = (lhs.get_multiplier() == rhs.get_multiplier())
                                    ? lhs.get_multiplier()
                                    : ((rhs.get_multiplier() == 0)   ? lhs.get_multiplier()
                                       : (lhs.get_multiplier() == 0) ? rhs.get_multiplier()
                                                                     : 0);

    // if constexpr (lhs.multiplier() == rhs.multiplier() &&
    //               std::ratio_equal_v<typename LHSType::offset, typename RHSType::offset>)
    // {
    //     return measure_type<lhs.power() + rhs.power(), lhs.multiplier(), typename LHSType::scale,
    //                        typename RHSType::offset>{};
    // }
    // else
    // {
    //     return measure_type<lhs.power() + rhs.power(), 0, _detail::one, _detail::zero>{};
    // }
    return measure_type<lhs.get_power() - rhs.get_power(), multiplier,
                        std::ratio_divide<typename LHSType::scale, typename RHSType::scale>, _detail::zero>{};
}
} // namespace maxwell::internal

#endif