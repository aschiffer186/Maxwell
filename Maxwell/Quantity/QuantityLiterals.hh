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
    }

    MAKE_QUANTITY_LITERAL(Mole, mol)
    MAKE_QUANTITY_LITERAL(Ampere, A)
    MAKE_QUANTITY_LITERAL(Meter, m)
    MAKE_QUANTITY_LITERAL(Candela, cd)
    MAKE_QUANTITY_LITERAL(Kilogram, kg)
    MAKE_QUANTITY_LITERAL(Kelvin, K)
    MAKE_QUANTITY_LITERAL(Second, s)

#undef MAKE_QUANTITY_LITERAL
} // namespace Maxwell::QuantityLiterals

#endif