/**
 * @file QuantityLiterals.hh
 * @author Alex Schiffer
 * @brief
 * @version 0.1
 * @date 2024-02-29
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef QUANTITY_LITERALS_HH
#define QUANTITY_LITERALS_HH

#include "QuantityTypes.hh"

namespace Maxwell::QuantityLiterals
{
#define MAKE_QUANTITY_LITERAL(type, name)                                      \
    consteval type operator""_##name(long double x) noexcept                   \
    {                                                                          \
        return type{static_cast<double>(x)};                                   \
    }                                                                          \
    consteval type operator""_##name(unsigned long long x) noexcept            \
    {                                                                          \
        return type{x};                                                        \
    }

    MAKE_QUANTITY_LITERAL(Mole, mol)
    MAKE_QUANTITY_LITERAL(Ampere, A)
    MAKE_QUANTITY_LITERAL(Meter, m)
    MAKE_QUANTITY_LITERAL(Candela, cd)
    MAKE_QUANTITY_LITERAL(Kilogram, kg)
    MAKE_QUANTITY_LITERAL(Kelvin, K)
    MAKE_QUANTITY_LITERAL(Second, s)
    MAKE_QUANTITY_LITERAL(Radians, rad)

    MAKE_QUANTITY_LITERAL(Hertz, hz)
    MAKE_QUANTITY_LITERAL(Newton, N)
    MAKE_QUANTITY_LITERAL(Pascal, Pa)
    MAKE_QUANTITY_LITERAL(Joule, J)
    MAKE_QUANTITY_LITERAL(Watt, W)
    MAKE_QUANTITY_LITERAL(Coulomb, C)
    MAKE_QUANTITY_LITERAL(Volt, V)
    MAKE_QUANTITY_LITERAL(Farad, F)
    MAKE_QUANTITY_LITERAL(Ohm, Omega)
    MAKE_QUANTITY_LITERAL(Weber, Wb)

    MAKE_QUANTITY_LITERAL(Degrees, deg)

    MAKE_QUANTITY_LITERAL(MeterPerSecond, m_s)

    MAKE_QUANTITY_LITERAL(Pound, lb)

    MAKE_QUANTITY_LITERAL(Foot, ft)
    MAKE_QUANTITY_LITERAL(Inch, in)
    MAKE_QUANTITY_LITERAL(Yard, yd)
    MAKE_QUANTITY_LITERAL(Mile, mi)

    MAKE_QUANTITY_LITERAL(Minute, min)
    MAKE_QUANTITY_LITERAL(Hour, hr)
    MAKE_QUANTITY_LITERAL(Day, d)
    MAKE_QUANTITY_LITERAL(Year, yr)

#undef MAKE_QUANTITY_LITERAL
} // namespace Maxwell::QuantityLiterals

#endif