/**
 * @file UnitTypes.hh
 * @author Alex Schiffer
 * @brief Constants used to represent common units, both SI
 *        and imperial and common multiples of those units.
 * @version 0.1
 * @date 2024-03-02
 *
 * @copyright Copyright (c) 2024
 */
#ifndef UNIT_TYPES_HH
#define UNIT_TYPES_HH

#include "UnitBase.hh"

namespace Maxwell
{
#define MAKE_SCALED_UNITS(Unit, BaseUnit, dim)                                 \
    using BaseUnit##Type               = decltype(BaseUnit);                   \
    constexpr inline auto Quetta##Unit = adjustPrefix##dim<30>(BaseUnit);      \
    constexpr inline auto Ronna##Unit  = adjustPrefix##dim<27>(BaseUnit);      \
    constexpr inline auto Yotta##Unit  = adjustPrefix##dim<24>(BaseUnit);      \
    constexpr inline auto Zeta##Unit   = adjustPrefix##dim<21>(BaseUnit);      \
    constexpr inline auto Exa##Unit    = adjustPrefix##dim<18>(BaseUnit);      \
    constexpr inline auto Peta##Unit   = adjustPrefix##dim<15>(BaseUnit);      \
    constexpr inline auto Tera##Unit   = adjustPrefix##dim<12>(BaseUnit);      \
    constexpr inline auto Giga##Unit   = adjustPrefix##dim<9>(BaseUnit);       \
    constexpr inline auto Mega##Unit   = adjustPrefix##dim<6>(BaseUnit);       \
    constexpr inline auto Kilo##Unit   = adjustPrefix##dim<3>(BaseUnit);       \
    constexpr inline auto Hecto##Unit  = adjustPrefix##dim<2>(BaseUnit);       \
    constexpr inline auto Deca##Unit   = adjustPrefix##dim<1>(BaseUnit);       \
    constexpr inline auto Deci##Unit   = adjustPrefix##dim<-1>(BaseUnit);      \
    constexpr inline auto Centi##Unit  = adjustPrefix##dim<-2>(BaseUnit);      \
    constexpr inline auto Milli##Unit  = adjustPrefix##dim<-3>(BaseUnit);      \
    constexpr inline auto Micro##Unit  = adjustPrefix##dim<-6>(BaseUnit);      \
    constexpr inline auto Nano##Unit   = adjustPrefix##dim<-9>(BaseUnit);      \
    constexpr inline auto Pico##Unit   = adjustPrefix##dim<-12>(BaseUnit);     \
    constexpr inline auto Femto##Unit  = adjustPrefix##dim<-15>(BaseUnit);     \
    constexpr inline auto Atto##Unit   = adjustPrefix##dim<-18>(BaseUnit);     \
    constexpr inline auto Zepto##Unit  = adjustPrefix##dim<-21>(BaseUnit);     \
    constexpr inline auto Yocto##Unit  = adjustPrefix##dim<-24>(BaseUnit);     \
    constexpr inline auto Ronto##Unit  = adjustPrefix##dim<-27>(BaseUnit);     \
    constexpr inline auto Quecto##Unit = adjustPrefix##dim<-30>(BaseUnit);     \
    using Quetta##Unit##Type           = decltype(Quetta##Unit);               \
    using Ronna##Unit##Type            = decltype(Ronna##Unit);                \
    using Yotta##Unit##Type            = decltype(Yotta##Unit);                \
    using Zeta##Unit##Type             = decltype(Zeta##Unit);                 \
    using Exa##Unit##Type              = decltype(Exa##Unit);                  \
    using Peta##Unit##Type             = decltype(Peta##Unit);                 \
    using Tera##Unit##Type             = decltype(Tera##Unit);                 \
    using Giga##Unit##Type             = decltype(Giga##Unit);                 \
    using Mega##Unit##Type             = decltype(Mega##Unit);                 \
    using Kilo##Unit##Type             = decltype(Kilo##Unit);                 \
    using Hecto##Unit##Type            = decltype(Hecto##Unit);                \
    using Deca##Unit##Type             = decltype(Deca##Unit);                 \
    using Deci##Unit##Type             = decltype(Deci##Unit);                 \
    using Centi##Unit##Type            = decltype(Centi##Unit);                \
    using Milli##Unit##Type            = decltype(Milli##Unit);                \
    using Micro##Unit##Type            = decltype(Micro##Unit);                \
    using Nano##Unit##Type             = decltype(Nano##Unit);                 \
    using Pico##Unit##Type             = decltype(Pico##Unit);                 \
    using Femto##Unit##Type            = decltype(Femto##Unit);                \
    using Atto##Unit##Type             = decltype(Atto##Unit);                 \
    using Zepto##Unit##Type            = decltype(Zepto##Unit);                \
    using Yocto##Unit##Type            = decltype(Yocto##Unit);                \
    using Ronto##Unit##Type            = decltype(Ronto##Unit);                \
    using Quecto##Unit##Type           = decltype(Quecto##Unit);

    // SI Base Units
    constexpr inline Unit<CoherentUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase>
        MoleUnit;
    constexpr inline Unit<NullUnitBase, CoherentUnitBase, NullUnitBase,
                          NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase>
        AmpereUnit;
    constexpr inline Unit<NullUnitBase, NullUnitBase, CoherentUnitBase,
                          NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase>
        MeterUnit;
    constexpr inline Unit<NullUnitBase, NullUnitBase, NullUnitBase,
                          CoherentUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase>
        CandelaUnit;
    constexpr inline Unit<NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, CoherentUnitBase, NullUnitBase,
                          NullUnitBase>
        GramUnit;
    constexpr inline Unit<NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, NullUnitBase, CoherentUnitBase,
                          NullUnitBase>
        KelvinUnit;
    constexpr inline Unit<NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, NullUnitBase, NullUnitBase,
                          CoherentUnitBase>
        SecondUnit;
    constexpr inline Unit<NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, CoherentUnitBase>
        RadianUnit;
    constexpr inline Unit<NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, NullUnitBase, NullUnitBase,
                          NullUnitBase, NullUnitBase>
        DimensionlessUnit;
    using DimensionlessUnitType = decltype(DimensionlessUnit);

    // Scaled Base Units
    // Amount
    MAKE_SCALED_UNITS(moleUnit, MoleUnit, Amount)
    MAKE_SCALED_UNITS(ampereUnit, AmpereUnit, Current)
    MAKE_SCALED_UNITS(meterUnit, MeterUnit, Length)
    MAKE_SCALED_UNITS(candelaUnit, CandelaUnit, Luminosity)
    MAKE_SCALED_UNITS(gramUnit, GramUnit, Mass)
    MAKE_SCALED_UNITS(kelvinUnit, KelvinUnit, Temperature)
    MAKE_SCALED_UNITS(secondUnit, SecondUnit, Time)
    MAKE_SCALED_UNITS(radianUnit, RadianUnit, Angle)

    // Special Derived units
    constexpr inline auto HertzUnit = unitInverse(SecondUnit);
    MAKE_SCALED_UNITS(hertzUnit, HertzUnit, Time)

    constexpr inline auto NewtonUnit =
        KilogramUnit * MeterUnit / (SecondUnit * SecondUnit);
    MAKE_SCALED_UNITS(newtonUnit, NewtonUnit, Mass)

    constexpr inline auto PascalUnit = NewtonUnit / (MeterUnit * MeterUnit);
    MAKE_SCALED_UNITS(pascalUnit, PascalUnit, Mass)

    constexpr inline auto JouleUnit = NewtonUnit * MeterUnit;
    MAKE_SCALED_UNITS(jouleUnit, JouleUnit, Mass)

    constexpr inline auto WattUnit = JouleUnit / SecondUnit;
    MAKE_SCALED_UNITS(wattUnit, WattUnit, Mass)

    constexpr inline auto CoulombUnit = AmpereUnit * SecondUnit;
    MAKE_SCALED_UNITS(coulombUnit, CoulombUnit, Current)

    constexpr inline auto VoltUnit = WattUnit / AmpereUnit;
    MAKE_SCALED_UNITS(voltUnit, VoltUnit, Mass)

    constexpr inline auto FaradUnit = CoulombUnit / VoltUnit;
    MAKE_SCALED_UNITS(faradUnit, FaradUnit, Current)

    constexpr inline auto OhmUnit = VoltUnit / AmpereUnit;
    MAKE_SCALED_UNITS(ohmUnit, OhmUnit, Mass)

    constexpr inline auto SiemensUnit = AmpereUnit / VoltUnit;
    MAKE_SCALED_UNITS(siemensUnit, SiemensUnit, Current)

    constexpr inline auto WeberUnit = VoltUnit * SecondUnit;
    MAKE_SCALED_UNITS(weberUnit, WeberUnit, Mass)

    constexpr inline auto DegreesUnit =
        adjustScaleAngle<5'729'578, 100'000>(RadianUnit);
    using DegreesUnitType = decltype(DegreesUnit);

    // Imperial units

    // Length measurements
    constexpr inline auto FootUnit =
        adjustScaleLength<328'084, 100'000>(MeterUnit);
    using FootUnitType = decltype(FootUnit);

    constexpr inline auto InchUnit = adjustScaleLength<1, 12>(FootUnit);
    using InchUnitType             = decltype(InchUnit);

    constexpr inline auto YardUnit = adjustScaleLength<3, 1>(FootUnit);
    using YardUnitType             = decltype(YardUnit);

    constexpr inline auto MileUnit = adjustScaleLength<5'280, 1>(FootUnit);
    using MileUnitType             = decltype(MileUnit);

    // Mass measurements
    constexpr inline auto PoundUnit =
        adjustScaleMass<220'462, 100'000>(KilogramUnit);
    using PoundUnitType = decltype(PoundUnit);

    // Time measurements
    constexpr inline auto MinuteUnit = adjustScaleTime<60, 1>(SecondUnit);
    using MinuteUnitType             = decltype(DegreesUnit);

    constexpr inline auto HourUnit = adjustScaleTime<60, 1>(MinuteUnit);
    using HourUnitType             = decltype(HourUnit);

    constexpr inline auto DayUnit = adjustScaleTime<24, 1>(HourUnit);
    using DayUnitType             = decltype(DayUnit);

    constexpr inline auto YearUnit = adjustScaleTime<365, 1>(DayUnit);
    using YearUnitType             = decltype(YearUnit);
#undef MAKE_SCALED_UNITS
} // namespace Maxwell

#endif