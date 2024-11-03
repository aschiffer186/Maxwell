#ifndef QUANTITY_REPO_HPP
#define QUANTITY_REPO_HPP

#include "Quantity.hpp"
#include "Unit.hpp"
#include "UnitRepo.hpp"

namespace Maxwell
{
using Mole  = Quantity<moleUnit>;
using IMole = IQuantity<moleUnit>;

using Ampere  = Quantity<ampereUnit>;
using IAmpere = IQuantity<ampereUnit>;

using Meter  = Quantity<meterUnit>;
using IMeter = IQuantity<meterUnit>;

using Candela  = Quantity<candelaUnit>;
using ICandela = IQuantity<candelaUnit>;

using Gram  = Quantity<gramUnit>;
using IGram = IQuantity<gramUnit>;

using Kelvin  = Quantity<kelvinUnit>;
using IKelvin = IQuantity<kelvinUnit>;

using Second  = Quantity<secondUnit>;
using ISecond = IQuantity<secondUnit>;

#define QUANTITY_LITERAL(name, symbol)                                                                                 \
    consteval name operator""_##symbol(long double x) noexcept                                                         \
    {                                                                                                                  \
        return name{x};                                                                                                \
    }                                                                                                                  \
    consteval I##name operator""_##symbol(unsigned long long x) noexcept                                               \
    {                                                                                                                  \
        return I##name{x};                                                                                             \
    }

namespace MetricLiterals
{
QUANTITY_LITERAL(Mole, mol)
QUANTITY_LITERAL(Ampere, A)
QUANTITY_LITERAL(Gram, g)
QUANTITY_LITERAL(Meter, m)
QUANTITY_LITERAL(Kelvin, K)
QUANTITY_LITERAL(Second, s)
} // namespace MetricLiterals

#ifndef NO_PREDEFINED_DERIVED_UNITS
using Radian  = Quantity<radianUnit>;
using IRadian = IQuantity<radianUnit>;

using Degree  = Quantity<degreeUnit>;
using IDegree = IQuantity<degreeUnit>;

/// \brief Specifies a quantity has dimensions of angle
///
/// \tparam QuantityType The quantity to check
template <typename QuantityType>
concept Angle = AngleUnit<QuantityType::Units>;
#endif
} // namespace Maxwell

#endif