#ifndef QUANTITY_REPO_HPP
#define QUANTITY_REPO_HPP

#include "Quantity.hpp"
#include "UnitRepo.hpp"

namespace Maxwell {
using Mole    = Quantity<MoleUnit>;
using Ampere  = Quantity<AmpereUnit>;
using Meter   = Quantity<MeterUnit>;
using Candela = Quantity<CandelaUnit>;
using Gram    = Quantity<GramUnit>;
using Kelvin  = Quantity<KelvinUnit>;
using Second  = Quantity<SecondUnit>;
using Radian  = Quantity<RadianUnit>;

using Degree = Quantity<DegreeUnit>;

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

#define MAKE_QUANTITY_LITERAL(UnitName, Symbol)                                \
    auto consteval operator""_##Symbol(long double x) noexcept -> UnitName {   \
        return UnitName(x);                                                    \
    }                                                                          \
    auto consteval operator""_##Symbol(unsigned long long x) noexcept          \
        -> BasicQuantity<unsigned long long, UnitName##Unit> {                 \
        return BasicQuantity<unsigned long long, UnitName##Unit>(x);           \
    }

inline namespace MetricLiterals
{
    MAKE_QUANTITY_LITERAL(Mole, mol)
    MAKE_QUANTITY_LITERAL(Ampere, A)
    MAKE_QUANTITY_LITERAL(Meter, m)
    MAKE_QUANTITY_LITERAL(Candela, cd)
    MAKE_QUANTITY_LITERAL(Gram, g)
    MAKE_QUANTITY_LITERAL(Kelvin, K)
    MAKE_QUANTITY_LITERAL(Second, s)
    MAKE_QUANTITY_LITERAL(Radian, rad)

    MAKE_QUANTITY_LITERAL(Degree, deg)
}

inline namespace TimeLiterals
{
    MAKE_QUANTITY_LITERAL(Day, d)
    MAKE_QUANTITY_LITERAL(Hour, hr)
    MAKE_QUANTITY_LITERAL(Year, yr)
    MAKE_QUANTITY_LITERAL(Decade, dec)
    MAKE_QUANTITY_LITERAL(Century, c)
    MAKE_QUANTITY_LITERAL(Millenium, M)
}

inline namespace ImperialUnits
{
    MAKE_QUANTITY_LITERAL(Inch, in)
    MAKE_QUANTITY_LITERAL(Foot, ft)
    MAKE_QUANTITY_LITERAL(Yard, yd)
    MAKE_QUANTITY_LITERAL(Mile, mi)
}
}   // namespace Maxwell

#endif