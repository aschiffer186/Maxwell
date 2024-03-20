/**
 * @file QuantityTypes.hh
 * @author Alex Schiffer
 * @brief
 * @version 0.1
 * @date 2024-02-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef QUANTITY_TYPES_HH
#define QUANTITY_TYPES_HH

#include "QuantityBase.hh"
#include "UnitBase.hh"
#include "UnitTypes.hh"

namespace Maxwell
{
    template <UnitLike auto Unit>
    using Quantity = BasicQuantity<double, Unit>;

    using Mole          = Quantity<MoleUnit>;
    using Ampere        = Quantity<AmpereUnit>;
    using Meter         = Quantity<MeterUnit>;
    using Candela       = Quantity<CandelaUnit>;
    using Kilogram      = Quantity<KilogramUnit>;
    using Second        = Quantity<SecondUnit>;
    using Kelvin        = Quantity<KelvinUnit>;
    using Dimensionless = Quantity<DimensionlessUnit>;
    using Radians       = Quantity<RadianUnit>;

    using Hertz   = Quantity<HertzUnit>;
    using Newton  = Quantity<NewtonUnit>;
    using Pascal  = Quantity<PascalUnit>;
    using Joule   = Quantity<JouleUnit>;
    using Watt    = Quantity<WattUnit>;
    using Coulomb = Quantity<CoulombUnit>;
    using Volt    = Quantity<VoltUnit>;
    using Farad   = Quantity<FaradUnit>;
    using Ohm     = Quantity<OhmUnit>;
    using Siemens = Quantity<SiemensUnit>;
    using Weber   = Quantity<WeberUnit>;

    using Degrees = Quantity<DegreesUnit>;

    using MeterPerSecond = Quantity<MeterPerSecondUnit>;

    using Pound = Quantity<PoundUnit>;

    using Foot = Quantity<FootUnit>;
    using Inch = Quantity<InchUnit>;
    using Yard = Quantity<YardUnit>;
    using Mile = Quantity<MileUnit>;

    using Minute = Quantity<MinuteUnit>;
    using Hour   = Quantity<HourUnit>;
    using Day    = Quantity<DayUnit>;
    using Year   = Quantity<YearUnit>;
} // namespace Maxwell

#endif