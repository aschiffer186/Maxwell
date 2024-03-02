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
#include "UnitTypes.hh"

namespace Maxwell
{
    template <UnitLike auto Unit>
    using Quantity = BasicQuantity<double, Unit>;

    using Mole     = Quantity<MoleUnit>;
    using Ampere   = Quantity<AmpereUnit>;
    using Meter    = Quantity<MeterUnit>;
    using Candela  = Quantity<CandelaUnit>;
    using Kilogram = Quantity<KilogramUnit>;
    using Second   = Quantity<SecondUnit>;
    using Kelvin   = Quantity<KelvinUnit>;
} // namespace Maxwell

#endif