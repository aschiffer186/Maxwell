/// \file QuantityMath.hpp
/// \author Alex Schiffer
/// \brief Wrapper around cmath functions for quantities
/// \version 0.1
/// \date 2024-11-02
///
/// \copyright Copyright (c) 2024
#ifndef QUANTITY_MATH_HPP
#define QUANTITY_MATH_HPP

#include <cmath>

#include "Quantity.hpp"
#include "QuantityRepo.hpp"
#include "Unit.hpp"
#include "UnitRepo.hpp"
#include "internal/Config.hpp"

namespace maxwell::math
{
/// \brief Computes the absolute value of a \c basic_quantity
///
/// \tparam M The type of the magnitude of \c x
/// \tparam U The units of \c xs
/// \return The absolute value of x
template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR23 basic_quantity<M, U> abs(basic_quantity<M, U> x)
{
    return std::abs(x.magnitude());
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR23 basic_quantity<M, U> fmod(basic_quantity<M, U> x, basic_quantity<M, U> y)
{
    return std::fmod(x.magnitude(), y.magnitude());
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR23 basic_quantity<M, U> remainder(basic_quantity<M, U> x, basic_quantity<M, U> y)
{
    return std::remainder(x.magnitude(), y.magnitude());
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR23 basic_quantity<M, U> remquo(basic_quantity<M, U> x, basic_quantity<M, U> y)
{
    return std::remquo(x.magnitude(), y.magnitude());
}

template <typename M, typename M2, unit auto U, unit auto U2>
    requires unit_convertible_to<U * U, U2>
MAXWELL_MATH_CONSTEXPR23 auto fma(basic_quantity<M, U> x, basic_quantity<M, U> y, basic_quantity<M2, U2> z)
    -> basic_quantity<decltype(x.magnitude() * y.magnitude()), U * U>
{
    return {std::fma(x.magnitude(), y.magnitude(), z.magnitude())};
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR23 basic_quantity<M, U> fmax(basic_quantity<M, U> x, basic_quantity<M, U> y)
{
    return {std::fmax(x.magnitude(), y.magnitude())};
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR23 basic_quantity<M, U> fmin(basic_quantity<M, U> x, basic_quantity<M, U> y)
{
    return {std::fmin(x.magnitude(), y.magnitude())};
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR23 basic_quantity<M, U> fdmin(basic_quantity<M, U> x, basic_quantity<M, U> y)
{
    return {std::fdim(x.magnitude(), y.magnitude())};
}

template <typename M, unit auto U>
basic_quantity<M, U> nan(const char* arg)
{
    return {std::nan(arg)};
}

template <typename M, unit auto U, typename M2, unit auto U2>
constexpr auto lerp(basic_quantity<M, U> a, basic_quantity<M2, U2> b, basic_quantity<M2, U2> t) noexcept
    -> basic_quantity<decltype(a.magnitude() * t.magnitude()), U * U2>
{
    return {std::lerp(a.magnitude(), b.magnitude(), t.magnitude())};
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR26 basic_quantity<M, U> exp(basic_quantity<M, U> x)
{
    return {std::exp(x.magnitude())};
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR26 basic_quantity<M, U> exp2(basic_quantity<M, U> x)
{
    return basic_quantity<M, U>(std::exp2(x.magnitude()));
}

template <typename M, unit auto U>
MAXWELL_MATH_CONSTEXPR26 basic_quantity<M, U> expm1(basic_quantity<M, U> x)
{
    return basic_quantity<M, U>(std::expm1(x.magnitude()));
}

MAXWELL_MATH_CONSTEXPR26 double cos(angle auto q)
{
    if constexpr (decltype(q)::units == radian_unit)
    {
        return std::cos(q.magnitude());
    }
    else
    {
        return std::cos(radian{q}.magnitude());
    }
}

MAXWELL_MATH_CONSTEXPR26 double sin(angle auto q)
{
    if constexpr (decltype(q)::units == radian_unit)
    {
        return std::sin(q.magnitude());
    }
    else
    {
        return std::sin(radian{q}.magnitude());
    }
}

MAXWELL_MATH_CONSTEXPR26 double tan(angle auto q)
{
    if constexpr (decltype(q)::units == radian_unit)
    {
        return std::tan(q.magnitude());
    }
    else
    {
        return std::tan(radian{q}.magnitude());
    }
}

MAXWELL_MATH_CONSTEXPR26 double cot(angle auto q)
{
    if constexpr (decltype(q)::units == radian_unit)
    {
        return std::tan(std::numbers::pi / 2.0 - q.magnitude());
    }
    else
    {
        return std::tan(std::numbers::pi / 2.0 - radian{q}.magnitude());
    }
}

MAXWELL_MATH_CONSTEXPR26 double csc(angle auto q)
{
    if constexpr (decltype(q)::units == radian_unit)
    {
        return 1.0 / std::sin(q.magnitude());
    }
    else
    {
        return 1.0 / std::sin(radian{q}.magnitude());
    }
}

MAXWELL_MATH_CONSTEXPR26 double sec(angle auto q)
{
    if constexpr (decltype(q)::units == radian_unit)
    {
        return 1.0 / std::cos(q.magnitude());
    }
    else
    {
        return 1.0 / std::cos(radian{q}.magnitude());
    }
}

MAXWELL_MATH_CONSTEXPR26 inline radian asin(double x)
{
    return radian(std::asin(x));
}

MAXWELL_MATH_CONSTEXPR26 inline degree asind(double x)
{
    const double radianMagnitude(std::asin(x));
    return degree{radianMagnitude * conversion_factor(radian_unit, degree_unit)};
}

MAXWELL_MATH_CONSTEXPR26 inline radian cos(double x)
{
    return radian(std::cos(x));
}

MAXWELL_MATH_CONSTEXPR26 inline degree cosd(double x)
{
    const double radianMagnitude(std::cos(x));
    return degree{radianMagnitude * conversion_factor(radian_unit, degree_unit)};
}

MAXWELL_MATH_CONSTEXPR26 inline radian atan(double x)
{
    return radian(std::atan(x));
}

MAXWELL_MATH_CONSTEXPR26 inline degree atand(double x)
{
    const double radianMagnitude(std::atan(x));
    return degree{radianMagnitude * conversion_factor(radian_unit, degree_unit)};
}

MAXWELL_MATH_CONSTEXPR26 inline radian atan2(double y, double x)
{
    return radian(std::atan2(y, x));
}

MAXWELL_MATH_CONSTEXPR26 inline degree atan2d(double y, double x)
{
    const double radianMagnitude(std::atan2(y, x));
    return degree{radianMagnitude * conversion_factor(radian_unit, degree_unit)};
}
} // namespace maxwell::math

#endif