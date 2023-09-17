#ifndef QUANTITY_TYPES_HPP
#define QUANTITY_TYPES_HPP

#include "QuantityCore.hpp"
#include "UnitTypes.hpp"

namespace Maxwell 
{
    using Second   = Quantity<SecondUnit>;
    using Meter    = Quantity<MeterUnit>; 
    using Kilogram = Quantity<KilogramUnit>; 
    using Kelvin   = Quantity<KelvinUnit>;
    using Ampere   = Quantity<AmpereUnit>; 
    using Mole     = Quantity<MoleUnit>; 
    using Candela  = Quantity<CandelaUnit>;

    //Area 
    using SqMeter = Quantity<decltype(MeterUnit{}*MeterUnit{})>;

    //Volume 
    using CuMeter = Quantity<decltype(SqMeter::Unit{}*MeterUnit{})>;

    using Newton   = Quantity<NewtonUnit>;
    using Joule    = Quantity<JouleUnit>; 
    using Watt     = Quantity<WattUnit>; 
    using Hertz    = Quantity<HertzUnit>; 
    using Pascal   = Quantity<PascalUnit>;
}

#endif