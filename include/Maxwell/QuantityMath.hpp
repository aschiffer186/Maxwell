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

namespace Maxwell
{
/// \brief Computes the absolute value of a \c BasicQuantity
///
/// \tparam M The type of the magnitude of \c x
/// \tparam U The units of \c xs
/// \return The absolute value of x
template <typename M, Unit auto U>
BasicQuantity<M, U> abs(BasicQuantity<M, U> x)
{
    return std::abs(x.magnitude());
}

template <typename M, Unit auto U>
BasicQuantity<M, U> fmod(BasicQuantity<M, U> x, BasicQuantity<M, U> y)
{
    return std::fmod(x.magnitude(), y.magnitude());
}

template <typename M, Unit auto U>
BasicQuantity<M, U> remainder(BasicQuantity<M, U> x, BasicQuantity<M, U> y)
{
    return std::remainder(x.magnitude(), y.magnitude());
}

template <typename M, Unit auto U>
BasicQuantity<M, U> remquo(BasicQuantity<M, U> x, BasicQuantity<M, U> y)
{
    return std::remquo(x.magnitude(), y.magnitude());
}

template <typename M, typename M2, Unit auto U, Unit auto U2>
    requires UnitConvertibleTo<U * U, U2>
BasicQuantity<M, U> fma(BasicQuantity<M, U> x, BasicQuantity<M, U> y, BasicQuantity<M2, U2> z)
{
    return std::fma(x.magnitude(), y.magnitude(), z.magnitude());
}

template <typename M, Unit auto U>
BasicQuantity<M, U> fmax(BasicQuantity<M, U> x, BasicQuantity<M, U> y)
{
    return std::fmax(x.magnitude(), y.magnitude());
}

template <typename M, Unit auto U>
BasicQuantity<M, U> fmin(BasicQuantity<M, U> x, BasicQuantity<M, U> y)
{
    return std::fmin(x.magnitude(), y.magnitude());
}

double cos(Angle auto q)
{
    if constexpr (decltype(q)::Units == radianUnit)
    {
        return std::cos(q.magnitude());
    }
    else
    {
        return std::cos(Radian{q}.magnitude());
    }
}

double sin(Angle auto q)
{
    if constexpr (decltype(q)::Units == radianUnit)
    {
        return std::sin(q.magnitude());
    }
    else
    {
        return std::sin(Radian{q}.magnitude());
    }
}

double tan(Angle auto q)
{
    if constexpr (decltype(q)::Units == radianUnit)
    {
        return std::tan(q.magnitude());
    }
    else
    {
        return std::tan(Radian{q}.magnitude());
    }
}

double cot(Angle auto q)
{
    if constexpr (decltype(q)::Units == radianUnit)
    {
        return std::tan(std::numbers::pi / 2.0 - q.magnitude());
    }
    else
    {
        return std::tan(std::numbers::pi / 2.0 - Radian{q}.magnitude());
    }
}

double csc(Angle auto q)
{
    if constexpr (decltype(q)::Units == radianUnit)
    {
        return 1.0 / std::sin(q.magnitude());
    }
    else
    {
        return 1.0 / std::sin(Radian{q}.magnitude());
    }
}

double sec(Angle auto q)
{
    if constexpr (decltype(q)::Units == radianUnit)
    {
        return 1.0 / std::cos(q.magnitude());
    }
    else
    {
        return 1.0 / std::cos(Radian{q}.magnitude());
    }
}

inline Radian asin(double x)
{
    return Radian(std::asin(x));
}

inline Degree asind(double x)
{
    const double radianMagnitude(std::asin(x));
    return Degree{radianMagnitude * conversionFactor(radianUnit, degreeUnit)};
}

inline Radian cos(double x)
{
    return Radian(std::cos(x));
}

inline Degree cosd(double x)
{
    const double radianMagnitude(std::cos(x));
    return Degree{radianMagnitude * conversionFactor(radianUnit, degreeUnit)};
}

inline Radian atan(double x)
{
    return Radian(std::atan(x));
}

inline Degree atand(double x)
{
    const double radianMagnitude(std::atan(x));
    return Degree{radianMagnitude * conversionFactor(radianUnit, degreeUnit)};
}

inline Radian atan2(double y, double x)
{
    return Radian(std::atan2(y, x));
}

inline Degree atan2d(double y, double x)
{
    const double radianMagnitude(std::atan2(y, x));
    return Degree{radianMagnitude * conversionFactor(radianUnit, degreeUnit)};
}
} // namespace Maxwell

#endif