#ifndef QUANTITY_REPO_HPP
#define QUANTITY_REPO_HPP

#include "Quantity.hpp"
#include "UnitRepo.hpp"

///@file QuantityRepo.hpp Definition of commonly used quantity types

namespace Maxwell {
using Mole    = Quantity<MoleUnit>;
using Ampere  = Quantity<AmpereUnit>;
using Meter   = Quantity<MeterUnit>;
using Candela = Quantity<CandelaUnit>;
using Gram    = Quantity<GramUnit>;
using Kelvin  = Quantity<KelvinUnit>;
using Second  = Quantity<SecondUnit>;
using Radian  = Quantity<RadianUnit>;

/// Given a quantity, makes quantities with all metrix prefixes. E
///
/// @param Q the name of the quantity
#define MAKE_PREFIXED_QUANTITIES(Q)                                                                                    \
    using Quetta##Q = Quantity<Quetta##Q##Unit>;                                                                       \
    using Ronna##Q  = Quantity<Ronna##Q##Unit>;                                                                        \
    using Yotta##Q  = Quantity<Yotta##Q##Unit>;                                                                        \
    using Zetta##Q  = Quantity<Zetta##Q##Unit>;                                                                        \
    using Exa##Q    = Quantity<Exa##Q##Unit>;                                                                          \
    using Tera##Q   = Quantity<Tera##Q##Unit>;                                                                         \
    using Peta##Q   = Quantity<Peta##Q##Unit>;                                                                         \
    using Tera##Q   = Quantity<Tera##Q##Unit>;                                                                         \
    using Giga##Q   = Quantity<Giga##Q##Unit>;                                                                         \
    using Mega##Q   = Quantity<Mega##Q##Unit>;                                                                         \
    using Kilo##Q   = Quantity<Kilo##Q##Unit>;                                                                         \
    using Hecto##Q  = Quantity<Hecto##Q##Unit>;                                                                        \
    using Deca##Q   = Quantity<Deca##Q##Unit>;                                                                         \
    using Deci##Q   = Quantity<Deci##Q##Unit>;                                                                         \
    using Centi##Q  = Quantity<Centi##Q##Unit>;                                                                        \
    using Milli##Q  = Quantity<Milli##Q##Unit>;                                                                        \
    using Micro##Q  = Quantity<Micro##Q##Unit>;                                                                        \
    using Nano##Q   = Quantity<Nano##Q##Unit>;                                                                         \
    using Pico##Q   = Quantity<Pico##Q##Unit>;                                                                         \
    using Femto##Q  = Quantity<Femto##Q##Unit>;                                                                        \
    using Atto##Q   = Quantity<Atto##Q##Unit>;                                                                         \
    using Zepto##Q  = Quantity<Zepto##Q##Unit>;                                                                        \
    using Yocto##Q  = Quantity<Yocto##Q##Unit>;                                                                        \
    using Yocto##Q  = Quantity<Yocto##Q##Unit>;                                                                        \
    using Quecto##Q = Quantity<Quecto##Q##Unit>;

#ifndef NO_PREDIFNED_DERIVED_UNITS
MAKE_PREFIXED_QUANTITIES(Mole)
MAKE_PREFIXED_QUANTITIES(Ampere)
MAKE_PREFIXED_QUANTITIES(Meter)
MAKE_PREFIXED_QUANTITIES(Candela)
MAKE_PREFIXED_QUANTITIES(Gram)
MAKE_PREFIXED_QUANTITIES(Kelvin)
MAKE_PREFIXED_QUANTITIES(Second)
MAKE_PREFIXED_QUANTITIES(Radian)

using Degree = Quantity<DegreeUnit>;

using MeterPerSecond = Quantity<MeterPerSecondUnit>;

using Joule = Quantity<JouleUnit>;
MAKE_PREFIXED_QUANTITIES(Joule)
using Hertz = Quantity<HertzUnit>;
MAKE_PREFIXED_QUANTITIES(Hertz)
using Newton = Quantity<NewtonUnit>;
MAKE_PREFIXED_QUANTITIES(Newton)
using Pascal = Quantity<PascalUnit>;
MAKE_PREFIXED_QUANTITIES(Pascal)
using Watt = Quantity<WattUnit>;
MAKE_PREFIXED_QUANTITIES(Watt)
using Coulomb = Quantity<CoulombUnit>;
MAKE_PREFIXED_QUANTITIES(Coulomb)
using Volt = Quantity<VoltUnit>;
MAKE_PREFIXED_QUANTITIES(Volt)
using Farad = Quantity<FaradUnit>;
MAKE_PREFIXED_QUANTITIES(Farad)
using Ohm = Quantity<OhmUnit>;
MAKE_PREFIXED_QUANTITIES(Ohm)
using Becquerel = Quantity<BecquerelUnit>;
MAKE_PREFIXED_QUANTITIES(Becquerel)

using Minute    = Quantity<MinuteUnit>;
using Hour      = Quantity<HourUnit>;
using Day       = Quantity<DayUnit>;
using Year      = Quantity<YearUnit>;
using Decade    = Quantity<DecadeUnit>;
using Century   = Quantity<CenturyUnit>;
using Millenium = Quantity<MilleniumUnit>;

using Inch = Quantity<InchUnit>;
using Foot = Quantity<FootUnit>;
using Yard = Quantity<YardUnit>;
using Mile = Quantity<MileUnit>;

using PoundMass = Quantity<PoundMassUnit>;

#define MAKE_QUANTITY_LITERAL(UnitName, Symbol)                                                                        \
    auto consteval operator""_##Symbol(long double x) noexcept -> UnitName { return UnitName(x); }                     \
    auto consteval operator""_##Symbol(                                                                                \
        unsigned long long x) noexcept -> BasicQuantity<unsigned long long, UnitName##Unit> {                          \
        return BasicQuantity<unsigned long long, UnitName##Unit>(x);                                                   \
    }

inline namespace MetricLiterals {
MAKE_QUANTITY_LITERAL(Mole, mol)
MAKE_QUANTITY_LITERAL(Ampere, A)
MAKE_QUANTITY_LITERAL(Meter, m)
MAKE_QUANTITY_LITERAL(Candela, cd)
MAKE_QUANTITY_LITERAL(Gram, g)
MAKE_QUANTITY_LITERAL(Kelvin, K)
MAKE_QUANTITY_LITERAL(Second, s)
MAKE_QUANTITY_LITERAL(Radian, rad)

MAKE_QUANTITY_LITERAL(Degree, deg)

MAKE_QUANTITY_LITERAL(Joule, J)
MAKE_QUANTITY_LITERAL(Hertz, hz)
MAKE_QUANTITY_LITERAL(Newton, N)
MAKE_QUANTITY_LITERAL(Pascal, Pa)
}   // namespace MetricLiterals

inline namespace TimeLiterals {
MAKE_QUANTITY_LITERAL(Day, d)
MAKE_QUANTITY_LITERAL(Hour, hr)
MAKE_QUANTITY_LITERAL(Year, yr)
MAKE_QUANTITY_LITERAL(Decade, dec)
MAKE_QUANTITY_LITERAL(Century, c)
MAKE_QUANTITY_LITERAL(Millenium, M)
}   // namespace TimeLiterals

inline namespace ImperialUnits {
MAKE_QUANTITY_LITERAL(Inch, in)
MAKE_QUANTITY_LITERAL(Foot, ft)
MAKE_QUANTITY_LITERAL(Yard, yd)
MAKE_QUANTITY_LITERAL(Mile, mi)

MAKE_QUANTITY_LITERAL(PoundMass, lb)
MAKE_QUANTITY_LITERAL(PoundMass, lbm)
}   // namespace ImperialUnits
#else
#define MAKE_MOLE_PREFIXES MAKE_QUANTITY_PREFIXES(Mole)
#define MAKE_DEGREE        using Degree = Quantity<DegreeUnit>;
#endif
}   // namespace Maxwell
#endif