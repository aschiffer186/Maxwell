#ifndef UNIT_TYPES_HPP
#define UNIT_TYPES_HPP

#include "UnitCore.hpp"

namespace Maxwell
{
    // Derived length units
    using FootUnit = Unit<NullUnit, UnitBase<0, 1, 3'048, 10'000>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;

    // Area units
    using SqMeterUnit = decltype(MeterUnit{}*MeterUnit{});

    // Volume units
    using CuMeterUnit = decltype(MeterUnit{}*MeterUnit{}*MeterUnit{});

    // Velocity units
    using MeterPerSecondUnit = decltype(MeterUnit{}/SecondUnit{});
    
    // Acceleration units
    using MeterPerSecondPerSecondUnit = decltype(MeterPerSecondUnit{}/SecondUnit{});

    //Derived mass units 
    using PoundMassUnit = Unit<NullUnit, NullUnit, UnitBase<3, 1, 453'592, 1'000'000>, NullUnit, NullUnit, NullUnit, NullUnit>;

    using NewtonUnit = decltype(KilogramUnit{}*MeterUnit{}/(SecondUnit{}*SecondUnit{}));
    using JouleUnit = decltype(NewtonUnit{}*MeterUnit{});
    using WattUnit = decltype(JouleUnit{}/SecondUnit{});
    using HertzUnit = decltype(Dimensionless_Unit{}/SecondUnit{});
    using PascalUnit = decltype(NewtonUnit{}/(MeterUnit{}*MeterUnit{}));
    using CoulumbUnit = decltype(SecondUnit{}*AmpereUnit{});
    using VoltUnit = decltype(JouleUnit{}/CoulumbUnit{});
    using FaradUnit = decltype(CoulumbUnit{}/VoltUnit{});
    using OhmUnit = decltype(VoltUnit{}/AmpereUnit{});
    using SiemensUnit = decltype(AmpereUnit{}/VoltUnit{});
    using WeberUnit = decltype(VoltUnit{}*SecondUnit{});
    using TeslaUnit = decltype(WeberUnit{}/(MeterUnit{}*MeterUnit{}));
    using HenryUnit = decltype(WeberUnit{}/AmpereUnit{}); 

    //using CelsiusUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit

    using DegreeUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1, 572'957'795'131, 10'000'000'000>>;
}

#endif