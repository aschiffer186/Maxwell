/**
 * @file QuantityMath.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-03-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef QUANTITY_MATH_HH
#define QUANTITY_MATH_HH

#include <cinttypes>
#include <cmath>
#include <concepts>

#include "QuantityTypes.hh"

namespace Maxwell::Math
{
#define MAXWELL_TRIG_FUNCTION(name)                                            \
    template <typename Tp>                                                     \
    auto name(BasicQuantity<Tp, RadianUnit> x) noexcept(                       \
        noexcept(std::name(x.value()))) -> double                              \
    {                                                                          \
        return std::name(x.value());                                           \
    }                                                                          \
    template <typename Tp>                                                     \
    auto name(BasicQuantity<Tp, DegreesUnit> x) noexcept(                      \
        noexcept(name(Radians(x)))) -> double                                  \
    {                                                                          \
        return name(Radians(x));                                               \
    }

#define MAXWELL_RECIP_TRIG_FUNCTION(name, recip)                               \
    template <typename Tp>                                                     \
    auto name(BasicQuantity<Tp, RadianUnit> x) noexcept(                       \
        noexcept(std::recip(x.value()))) -> double                             \
    {                                                                          \
        return 1.0 / std::recip(x.value());                                    \
    }                                                                          \
    template <typename Tp>                                                     \
    auto name(BasicQuantity<Tp, DegreesUnit> x) noexcept(                      \
        noexcept(name(Radians(x)))) -> double                                  \
    {                                                                          \
        return name(Radians(x));                                               \
    }

#define MAXWELL_INV_TRIG_FUNCTION(name)                                        \
    template <typename Tp>                                                     \
        requires std::floating_point<Tp> ||                                    \
                 std::integral<Tp>                                             \
                 auto name(Tp x) noexcept(noexcept(std::name(x))) -> Radians   \
    {                                                                          \
        return Radians(std::name(x));                                          \
    }                                                                          \
    template <typename Tp>                                                     \
        requires std::floating_point<Tp> ||                                    \
                 std::integral<Tp>                                             \
                 auto name##d(Tp x) noexcept(noexcept(std::name(x)))->Degrees  \
    {                                                                          \
        const double val = std::name(x);                                       \
        return Degrees(val * 180.0 / M_PI);                                    \
    }

    MAXWELL_TRIG_FUNCTION(sin)
    MAXWELL_TRIG_FUNCTION(cos)
    MAXWELL_TRIG_FUNCTION(tan)

    MAXWELL_RECIP_TRIG_FUNCTION(csc, sin)
    MAXWELL_RECIP_TRIG_FUNCTION(sec, cos)
    MAXWELL_RECIP_TRIG_FUNCTION(cot, tan)

    MAXWELL_INV_TRIG_FUNCTION(asin)
    MAXWELL_INV_TRIG_FUNCTION(acos)
    MAXWELL_INV_TRIG_FUNCTION(atan)

    template <typename Tp>
        requires std::floating_point<Tp> || std::integral<Tp>
    auto atan2(Tp y, Tp x) noexcept(noexcept(std::atan2(y, x))) -> Radians
    {
        return Radians(std::atan2(y, x));
    }

    template <typename Tp>
        requires std::floating_point<Tp> || std::integral<Tp>
    auto atan2d(Tp y, Tp x) noexcept(noexcept(std::atan2(y, x))) -> Degrees
    {
        const double val = std::atan2(y, x);
        return Degrees(val * 180.0 / M_PI);
    }

#undef MAXWELL_TRIG_FUNCTION
#undef MAXWELL_RECIP_TRIG_FUNCTION
#undef MAXWELL_INV_TRIG_FUNCTION
} // namespace Maxwell::Math

#endif