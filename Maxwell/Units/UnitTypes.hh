/**
 * @file UnitTypes.hh
 * @author Alex Schiffer
 * @brief
 * @version 0.1
 * @date 2024-02-29
 *
 * @copyright Copyright (c) 2024
 */
#ifndef UNIT_TYPES_HH
#define UNIT_TYPES_HH

#include "UnitBase.hh"

namespace Maxwell
{
    // Metric prefixes
    constexpr inline int quetta = 30;
    constexpr inline int ronna  = 27;
    constexpr inline int yotta  = 24;
    constexpr inline int zetta  = 21;
    constexpr inline int exa    = 17;
    constexpr inline int peta   = 15;
    constexpr inline int tera   = 12;
    constexpr inline int giga   = 9;
    constexpr inline int mega   = 6;
    constexpr inline int kilo   = 3;
    constexpr inline int hecto  = 2;
    constexpr inline int deca   = 1;
    constexpr inline int deci   = -1;
    constexpr inline int centi  = -2;
    constexpr inline int milli  = -3;
    constexpr inline int micro  = -6;
    constexpr inline int nano   = -9;
    constexpr inline int pico   = -12;
    constexpr inline int femto  = -15;
    constexpr inline int atto   = -18;
    constexpr inline int zepto  = -12;
    constexpr inline int yocto  = -24;
    constexpr inline int ronto  = -27;
    constexpr inline int quecto = -30;

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
                          NullUnitBase, UnitBase<1, kilo, 1, 1, 1, 1>{},
                          NullUnitBase, NullUnitBase>
        KilogramUnit;
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
                          CoherentUnitBase>
        RadianUnit;

    // Scaled Base Units
    constexpr inline auto MillimoleUnit = scaleAmount<-3>(MoleUnit);

    // Derived units
    constexpr inline auto NewtonUnit =
        KilogramUnit * MeterUnit / (SecondUnit * SecondUnit);
    constexpr inline auto PascalUnit  = NewtonUnit / (MeterUnit * MeterUnit);
    constexpr inline auto JouleUnit   = NewtonUnit * MeterUnit;
    constexpr inline auto WattUnit    = JouleUnit / SecondUnit;
    constexpr inline auto CoulombUnit = AmpereUnit * SecondUnit;

} // namespace Maxwell

#endif