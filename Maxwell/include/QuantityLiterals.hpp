#ifndef QUANTITY_LITERALS_HPP
#define QUANTITY_LITERALS_HPP

#include "QuantityCore.hpp"
#include "Unit.hpp"

#define MAKE_QUANTITY_LITERAL(unit, literal)\
    consteval Basic_Quantity<unsigned long long, unit##Unit> operator ""_##literal(unsigned long long x) noexcept\
    {\
        return Basic_Quantity<unsigned long long, unit##Unit>(x);\
    }\
    consteval Basic_Quantity<long double, unit##Unit> operator ""_##literal(long double x) noexcept\
    {\
        return Basic_Quantity<double, unit##Unit>(x);\
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
    MAKE_QUANTITY_LITERAL(Quettasecond, Qs)

    MAKE_QUANTITY_LITERAL(Quectometer, qm)
    MAKE_QUANTITY_LITERAL(Rontometer, rm)
    MAKE_QUANTITY_LITERAL(Yoctometer, ym)
    MAKE_QUANTITY_LITERAL(Zeptometer, zm)
    MAKE_QUANTITY_LITERAL(Attometer, am)
    MAKE_QUANTITY_LITERAL(Femtometer, fm)
    MAKE_QUANTITY_LITERAL(Picometer, pm)
    MAKE_QUANTITY_LITERAL(Nanometer, nm)
    MAKE_QUANTITY_LITERAL(Micrometer, um)
    MAKE_QUANTITY_LITERAL(Millimeter, mm)
    MAKE_QUANTITY_LITERAL(Centimeter, cm)
    MAKE_QUANTITY_LITERAL(Decimeter, dm)
    MAKE_QUANTITY_LITERAL(Meter, m)
    MAKE_QUANTITY_LITERAL(Decameter, dam)
    MAKE_QUANTITY_LITERAL(Hectometer, hm)
    MAKE_QUANTITY_LITERAL(Kilometer, km)
    MAKE_QUANTITY_LITERAL(Megameter, Mm)
    MAKE_QUANTITY_LITERAL(Gigameter, Gm)
    MAKE_QUANTITY_LITERAL(Terameter, Tm)
    MAKE_QUANTITY_LITERAL(Petameter, Pm)
    MAKE_QUANTITY_LITERAL(Exameter, Em)
    MAKE_QUANTITY_LITERAL(Zettameter, Zm)
    MAKE_QUANTITY_LITERAL(Yottameter, Ym)
    MAKE_QUANTITY_LITERAL(Ronnameter, Rm)
    MAKE_QUANTITY_LITERAL(Quettameter, Qm)

    MAKE_QUANTITY_LITERAL(Liter, L)

    MAKE_QUANTITY_LITERAL(MeterPerSecond, m_s)
    MAKE_QUANTITY_LITERAL(MeterPerSecondPerSecond, m_s2)

    MAKE_QUANTITY_LITERAL(SqMeter, m2)

    MAKE_QUANTITY_LITERAL(CuMeter, m3)

    MAKE_QUANTITY_LITERAL(Quectogram, qg)
    MAKE_QUANTITY_LITERAL(Rontogram, rg)
    MAKE_QUANTITY_LITERAL(Yoctogram, yg)
    MAKE_QUANTITY_LITERAL(Zeptogram, zg)
    MAKE_QUANTITY_LITERAL(Attogram, ag)
    MAKE_QUANTITY_LITERAL(Femtogram, fg)
    MAKE_QUANTITY_LITERAL(Picogram, pg)
    MAKE_QUANTITY_LITERAL(Nanogram, ng)
    MAKE_QUANTITY_LITERAL(Microgram, ug)
    MAKE_QUANTITY_LITERAL(Milligram, mg)
    MAKE_QUANTITY_LITERAL(Centigram, cg)
    MAKE_QUANTITY_LITERAL(Decigram, dg)
    MAKE_QUANTITY_LITERAL(Gram, g)
    MAKE_QUANTITY_LITERAL(Decagram, dag)
    MAKE_QUANTITY_LITERAL(Hectogram, hg)
    MAKE_QUANTITY_LITERAL(Kilogram, kg)
    MAKE_QUANTITY_LITERAL(Megagram, Mg)
    MAKE_QUANTITY_LITERAL(Gigagram, Gg)
    MAKE_QUANTITY_LITERAL(Teragram, Tg)
    MAKE_QUANTITY_LITERAL(Petagram, Pg)
    MAKE_QUANTITY_LITERAL(Exagram, Eg)
    MAKE_QUANTITY_LITERAL(Zettagram, Zg)
    MAKE_QUANTITY_LITERAL(Yottagram, Yg)
    MAKE_QUANTITY_LITERAL(Ronnagram, Rg)
    MAKE_QUANTITY_LITERAL(Quettagram, Qg)

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
    MAKE_QUANTITY_LITERAL(Degree, deg)
}

#endif