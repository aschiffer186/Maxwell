#ifndef UNIT_TYPES_HPP
#define UNIT_TYPES_HPP

#include "UnitCore.hpp"
#include "BaseUnitScales.hpp"

namespace Maxwell
{
    // Derived time units
    using MinuteUnit = Unit<UnitBase<0, 1, 60, 1>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using HourUnit = Unit<UnitBase<0, 1, 3'600, 1>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;

    // Derived length units
    using InchUnit = Unit<NullUnit, UnitBase<-2, 1, 254, 100>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using FootUnit = Unit<NullUnit, UnitBase<0, 1, 3'048, 10'000>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using YardUnit = Unit<NullUnit, UnitBase<0, 1, 9'144, 10'000>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;

    // Area units
    using SqInchUnit  = decltype(InchUnit{}*InchUnit{});
    using SqFootUnit  = decltype(FootUnit{}*FootUnit{});

    using SqCentimeterUnit = decltype(CentimeterUnit{}*CentimeterUnit{});
    using SqMeterUnit      = decltype(MeterUnit{}*MeterUnit{});

    // Volume units
    using CuInchUnit  = decltype(InchUnit{}*SqInchUnit{});
    using CuFootUnit  = decltype(FootUnit{}*SqFootUnit{});

    using CuCentimeterUnit = decltype(CentimeterUnit{}*SqCentimeterUnit{});
    using CuMeterUnit      = decltype(MeterUnit{}*SqMeterUnit{});

    // Velocity units
    using MeterPerSecondUnit = decltype(MeterUnit{}/SecondUnit{});
    
    // Acceleration units
    using MeterPerSecondPerSecondUnit = decltype(MeterPerSecondUnit{}/SecondUnit{});

    //Derived mass units 
    using PoundMassUnit = Unit<NullUnit, NullUnit, UnitBase<3, 1, 45'359'237, 100'000'000>, NullUnit, NullUnit, NullUnit, NullUnit>;

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

    using CelsiusUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1, 1, 1, 27'315, 100>, NullUnit, NullUnit>;
    using FarenheitUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1, 5, 9, 459'670, 1'000>, NullUnit, NullUnit>;

    using DegreeUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1, 10'000'000'000, 572'957'795'131>>;
}

#endif