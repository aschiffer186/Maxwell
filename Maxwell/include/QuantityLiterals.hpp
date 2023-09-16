#ifndef QUANTITY_LITERALS_HPP
#define QUANTITY_LITERALS_HPP

#include "QuantityCore.hpp"
#include "UnitCore.hpp"
#include "UnitTypes.hpp"
#include <sys/types.h>

#define MAKE_QUANTITY_LITERAL(Unit, literal)\
    consteval Quantity<unsigned long long, Unit> operator ""_##literal(unsigned long long x)\
    {\
        return Quantity<unsigned long long, Unit>(x);\
    }\
    consteval Quantity<long double, Unit> operator ""_##literal(long double x)\
    {\
        return Quantity<double, Unit>(x);\
    }

namespace Maxwell::Literals 
{
    MAKE_QUANTITY_LITERAL(Quectosecond, qs)
    MAKE_QUANTITY_LITERAL(Rontosecond, rs)
    MAKE_QUANTITY_LITERAL(Yoctosecond, ys)
    MAKE_QUANTITY_LITERAL(Zeptosecond, zs)
    MAKE_QUANTITY_LITERAL(Attosecond, as)
    MAKE_QUANTITY_LITERAL(Femtosecond, fs)
    MAKE_QUANTITY_LITERAL(Picosecond, ps)
    MAKE_QUANTITY_LITERAL(Nanosecond, ns)
    MAKE_QUANTITY_LITERAL(Microsecond, us)
    MAKE_QUANTITY_LITERAL(Millisecond, ms)
    MAKE_QUANTITY_LITERAL(Centisecond, cs)
    MAKE_QUANTITY_LITERAL(Decisecond, ds)
    MAKE_QUANTITY_LITERAL(Second, s)
    MAKE_QUANTITY_LITERAL(Decasecond, das)
    MAKE_QUANTITY_LITERAL(Hectosecond, hs)
    MAKE_QUANTITY_LITERAL(Kilosecond, ks)
    MAKE_QUANTITY_LITERAL(Megasecond, Ms)
    MAKE_QUANTITY_LITERAL(Gigasecond, Gs)
    MAKE_QUANTITY_LITERAL(Terasecond, Ts)
    MAKE_QUANTITY_LITERAL(Petasecond, Ps)
    MAKE_QUANTITY_LITERAL(Exasecond, Es)
    MAKE_QUANTITY_LITERAL(Zettasecond, Zs)
    MAKE_QUANTITY_LITERAL(Yottasecond, Ys)
    MAKE_QUANTITY_LITERAL(Ronnasecond, Rs)
    MAKE_QUANTITY_LITERAL(Quettasecond, quad_t)

    MAKE_QUANTITY_LITERAL(Meter, m)
    MAKE_QUANTITY_LITERAL(Kilogram, kg)
    MAKE_QUANTITY_LITERAL(Ampere, A)
    MAKE_QUANTITY_LITERAL(Mole, mol)
    MAKE_QUANTITY_LITERAL(Kelvin, K)
    MAKE_QUANTITY_LITERAL(Candela, cd)
    MAKE_QUANTITY_LITERAL(Newton, N)
    MAKE_QUANTITY_LITERAL(Joule, J)
    MAKE_QUANTITY_LITERAL(Watt, W)
    MAKE_QUANTITY_LITERAL(Hertz, Hz)
    MAKE_QUANTITY_LITERAL(Pascal, Pa)
    MAKE_QUANTITY_LITERAL(Coulumb, C)
    MAKE_QUANTITY_LITERAL(Volt, V)
    MAKE_QUANTITY_LITERAL(Farad, F)
    MAKE_QUANTITY_LITERAL(Ohm, O)
    MAKE_QUANTITY_LITERAL(Siemens, S)
    MAKE_QUANTITY_LITERAL(Weber, Wb)
    MAKE_QUANTITY_LITERAL(Tesla, T)
    MAKE_QUANTITY_LITERAL(Henry, H)

    
}

#endif