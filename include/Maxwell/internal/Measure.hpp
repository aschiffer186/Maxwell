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
namespace Maxwell::Internal
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
concept RatioLike = is_ratio<Tp>::value;
} // namespace _detail
/// \endcond

/// \class MeasureType
/// \brief Represents one fundamental quantity in the SI system
///
/// <tt>class MeasureType</tt> represents one dimension used
/// in the SI system. It consists of a base unit and the transformations
/// applied to the base unit to convert it to a derived unit repersenting
/// that dimension. The unit represented by this class is given by
///
/// \f$Unit = (10^{Multiplier} * Scale::num/Scale::den * Measure^{Power}) + Offset::num/Offset::den\f$
///
/// The transformations applied to the measure are part of the
/// measure's type; different measres are different types.
///
/// \tparam Power_ the power the unit is raised to
/// \tparam Multiplier_ the metric prefix applied to the unit
/// \tparam Scale_ the scale factor of the unit
/// \tparam Offset_ the offset of the unit
template <std::intmax_t Power_, std::intmax_t Multiplier_, _detail::RatioLike Scale_ = _detail::one,
          _detail::RatioLike Offset_ = _detail::zero>
struct MeasureType
{
    /// The power the unit is raised to
    static constexpr std::intmax_t Power      = Power_;
    /// The metric prefix of the unit
    static constexpr std::intmax_t Multiplier = Multiplier_;
    /// The scale factor of the unit
    using Scale                               = Scale_;
    /// The offset of the unit
    using Offset                              = Offset_;

    /// \fn power
    /// \brief Returns the power the measure is raised to
    ///
    /// \return The power the measure is raised to
    consteval std::intmax_t power() const noexcept
    {
        return Power;
    }

    /// \fn multiplier
    /// \brief Returns the metric prefix of the measure
    /// \return The metric prefix of the measure
    std::intmax_t consteval multiplier() const noexcept
    {
        return Multiplier;
    }

    /// \fn adjustMultiplier
    /// \brief Adjusts the multiplier by the specified amount
    ///
    /// Creates a new measure whose multiplier is equivalent to
    /// \code Multiplier + Adjustment \endcode
    ///
    /// \post The returned measure has a power of \c Power and a
    ///       multiplier equal to <tt>Multiplier + Adjustment</tt>
    ///
    /// \tparam Adjustment the amount to adjust the measure by
    /// \return A new masure with the adjusted prefix
    template <std::intmax_t Adjustment>
    consteval auto adjustMultiplier() const noexcept
    {
        return MeasureType<Power, Multiplier + Adjustment, Scale, Offset>{};
    }

    /// \fn isBaseMeasure
    /// \brief Returns true if the measure is a base measure
    ///
    /// Returns true if the measure is a base measure, i.e. if the
    /// following condition is true
    /// \code
    /// Power == 1 && Multiplier == 0
    /// \endcode
    /// In general, this corresponds with an SI base unit except
    /// for mass, where this measure returns true for gram, not kilogram.
    /// \return true if the measure is a base measure
    consteval bool isBaseMeasure() const noexcept
    {
        return Power_ == 1 && Multiplier_ == 0 && std::ratio_equal_v<Scale, _detail::one> &&
               std::ratio_equal_v<Offset, _detail::zero>;
    }

    /// \fn isCoherentMeasure
    /// \brief Returns true if the measure is a coherent measure
    ///
    /// Returns true if the measure is a coherent measure, i.e.
    /// if it has no multiplier.
    ///
    /// \return true if the measuer is a coherent measure
    consteval bool isCoherentMeasure() const noexcept
    {
        return Multiplier_ == 0 && std::ratio_equal_v<Scale, _detail::one> && std::ratio_equal_v<Offset, _detail::zero>;
    }

    /// \fn toCoherentMeasure
    /// \brief Returns a coherent version of the measure
    ///
    /// Returns a new \c MeasureType with the same power that is a
    /// coherent version of \c *this
    ///
    /// \post The returned \c MeasureType has a multiplier of 0
    /// \return a coherent version of \c *this
    consteval auto toCoherentMeasure() const noexcept
    {
        return MeasureType<Power, 0, _detail::one, _detail::zero>{};
    }
};

/// Compile-time constant represeting base measure
constexpr MeasureType<1, 0, _detail::one, _detail::zero> baseMeasure;
/// Compile-time constant representing a null measure
/// (measure that doesn't contribute to the unit's definition)
constexpr MeasureType<0, 0, _detail::one, _detail::zero> nullMeasure;

// -- Measure Type Concepts --

/// \cond
namespace _detail
{
template <typename>
struct _is_measure : std::false_type
{
};

template <std::intmax_t Power_, std::intmax_t Multiplier, _detail::RatioLike Scale_, _detail::RatioLike Offset_>
struct _is_measure<MeasureType<Power_, Multiplier, Scale_, Offset_>> : std::true_type
{
};
} // namespace _detail
/// \endcond

/// \brief Specifies a type is an instantiation of the \c MeasureType class template
///
/// Specifies a type is an instantiation of the \c MeasureType class template, ignoring cv-qualfiers
/// and references.
///
/// \tparam M the type to check
template <typename M>
concept Measure = _detail::_is_measure<std::remove_cvref_t<M>>::value;

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
consteval bool isMeasureConvertible(Measure auto from, Measure auto to) noexcept
{
    return from.power() == to.power();
}

// --- Measure Type Comparison ---

/// \brief Compares two \c MeasureTypes for equality
///
/// Compares two \c MeasureTypes for equality.
/// Two \c MeasureTypes are equal if and only if they have the
/// same power, multiplier, scale factor, and offset. This implies they are the same type.
///
/// \param lhs one measure to compare for equality
/// \param rhs the other measure to compare for equality
///
/// \return true if the measures are equal
constexpr bool operator==(Measure auto lhs, Measure auto rhs) noexcept
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
constexpr Measure auto operator*(Measure auto lhs, Measure auto rhs) noexcept
{
    using LHSType = decltype(lhs);
    using RHSType = decltype(rhs);
    if constexpr (lhs.multiplier() == rhs.multiplier() &&
                  std::ratio_equal_v<typename LHSType::Scale, typename RHSType::Scale> &&
                  std::ratio_equal_v<typename LHSType::Offset, typename RHSType::Offset>)
    {
        return MeasureType<lhs.power() + rhs.power(), lhs.multiplier(), typename LHSType::Scale,
                           typename RHSType::Offset>{};
    }
    else
    {
        return MeasureType<lhs.power() + rhs.power(), 0, _detail::one, _detail::zero>{};
    }
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
constexpr Measure auto operator/(Measure auto lhs, Measure auto rhs) noexcept
{
    using LHSType = decltype(lhs);
    using RHSType = decltype(rhs);
    if constexpr (lhs.multiplier() == rhs.multiplier() &&
                  std::ratio_equal_v<typename LHSType::Scale, typename RHSType::Scale> &&
                  std::ratio_equal_v<typename LHSType::Offset, typename RHSType::Offset>)
    {
        return MeasureType<lhs.power() - rhs.power(), lhs.multiplier(), typename LHSType::Scale,
                           typename RHSType::Offset>{};
    }
    else
    {
        return MeasureType<lhs.power() - rhs.power(), 0, _detail::one, _detail::zero>{};
    }
}
} // namespace Maxwell::Internal

#endif