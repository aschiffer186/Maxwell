/// \file quantityRepo.hpp
/// \author Alex Schiffer
/// \brief Aliases for common quantities and quantity literals
/// \version 0.1
/// \date 2024-11-02
///
/// \copyright Copyright (c) 2024
#ifndef QUANTITY_REPO_HPP
#define QUANTITY_REPO_HPP

#include "Quantity.hpp"
#include "Unit.hpp"
#include "UnitRepo.hpp"

namespace maxwell
{
using mole  = quantity<mole_unit>;
using imole = iquantity<mole_unit>;

using ampere  = quantity<ampere_unit>;
using iampere = iquantity<ampere_unit>;

using meter  = quantity<meter_unit>;
using imeter = iquantity<meter_unit>;

using candela  = quantity<candela_unit>;
using icandela = iquantity<candela_unit>;

using gram  = quantity<gram_unit>;
using igram = iquantity<gram_unit>;

using kelvin  = quantity<kelvin_unit>;
using ikelvin = iquantity<kelvin_unit>;

using second  = quantity<second_unit>;
using isecond = iquantity<second_unit>;

#define QUANTITY_LITERAL(name, symbol)                                                                                 \
    consteval name operator""_##symbol(long double x) noexcept                                                         \
    {                                                                                                                  \
        return name{x};                                                                                                \
    }                                                                                                                  \
    consteval i##name operator""_##symbol(unsigned long long x) noexcept                                               \
    {                                                                                                                  \
        return i##name{x};                                                                                             \
    }

namespace metric_literals
{
QUANTITY_LITERAL(mole, mol)
QUANTITY_LITERAL(ampere, A)
QUANTITY_LITERAL(gram, g)
QUANTITY_LITERAL(meter, m)
QUANTITY_LITERAL(kelvin, K)
QUANTITY_LITERAL(second, s)
} // namespace metric_literals

using unitless_quantity  = quantity<unitless_unit_type{}>;
using iunitless_quantity = iquantity<unitless_unit_type{}>;

#define QUANTITY_PREFIXES(base_quantity)                                                                               \
    using quetta##base_quantity  = quantity<quetta##base_quantity##_unit>;                                             \
    using iquetta##base_quantity = iquantity<quetta##base_quantity##_unit>;                                            \
    using ronna##base_quantity   = quantity<ronna##base_quantity##_unit>;                                              \
    using ironna##base_quantity  = iquantity<ronna##base_quantity##_unit>;                                             \
    using yotta##base_quantity   = quantity<yotta##base_quantity##_unit>;                                              \
    using iyotta##base_quantity  = iquantity<yotta##base_quantity##_unit>;                                             \
    using zetta##base_quantity   = quantity<zetta##base_quantity##_unit>;                                              \
    using izetta##base_quantity  = iquantity<zetta##base_quantity##_unit>;                                             \
    using exa##base_quantity     = quantity<exa##base_quantity##_unit>;                                                \
    using iexa##base_quantity    = iquantity<exa##base_quantity##_unit>;                                               \
    using peta##base_quantity    = quantity<peta##base_quantity##_unit>;                                               \
    using ipeta##base_quantity   = iquantity<peta##base_quantity##_unit>;                                              \
    using tera##base_quantity    = quantity<tera##base_quantity##_unit>;                                               \
    using itera##base_quantity   = iquantity<tera##base_quantity##_unit>;                                              \
    using giga##base_quantity    = quantity<giga##base_quantity##_unit>;                                               \
    using igiga##base_quantity   = iquantity<giga##base_quantity##_unit>;                                              \
    using mega##base_quantity    = quantity<mega##base_quantity##_unit>;                                               \
    using imega##base_quantity   = iquantity<mega##base_quantity##_unit>;                                              \
    using kilo##base_quantity    = quantity<kilo##base_quantity##_unit>;                                               \
    using ikilo##base_quantity   = iquantity<kilo##base_quantity##_unit>;                                              \
    using hecto##base_quantity   = quantity<hecto##base_quantity##_unit>;                                              \
    using ihecto##base_quantity  = iquantity<hecto##base_quantity##_unit>;                                             \
    using deca##base_quantity    = quantity<deca##base_quantity##_unit>;                                               \
    using ideca##base_quantity   = iquantity<deca##base_quantity##_unit>;                                              \
    using deci##base_quantity    = quantity<deci##base_quantity##_unit>;                                               \
    using ideci##base_quantity   = iquantity<deci##base_quantity##_unit>;                                              \
    using centi##base_quantity   = quantity<centi##base_quantity##_unit>;                                              \
    using icenti##base_quantity  = iquantity<centi##base_quantity##_unit>;                                             \
    using milli##base_quantity   = quantity<milli##base_quantity##_unit>;                                              \
    using imilli##base_quantity  = iquantity<milli##base_quantity##_unit>;                                             \
    using micro##base_quantity   = quantity<micro##base_quantity##_unit>;                                              \
    using imicro##base_quantity  = iquantity<micro##base_quantity##_unit>;                                             \
    using nano##base_quantity    = quantity<nano##base_quantity##_unit>;                                               \
    using inano##base_quantity   = iquantity<nano##base_quantity##_unit>;                                              \
    using pico##base_quantity    = quantity<pico##base_quantity##_unit>;                                               \
    using ipico##base_quantity   = iquantity<pico##base_quantity##_unit>;                                              \
    using femto##base_quantity   = quantity<femto##base_quantity##_unit>;                                              \
    using ifemto##base_quantity  = iquantity<femto##base_quantity##_unit>;                                             \
    using atto##base_quantity    = quantity<atto##base_quantity##_unit>;                                               \
    using iatto##base_quantity   = iquantity<atto##base_quantity##_unit>;                                              \
    using zepto##base_quantity   = quantity<zepto##base_quantity##_unit>;                                              \
    using izepto##base_quantity  = iquantity<zepto##base_quantity##_unit>;                                             \
    using yocto##base_quantity   = quantity<yocto##base_quantity##_unit>;                                              \
    using iyocto##base_quantity  = iquantity<yocto##base_quantity##_unit>;                                             \
    using ronto##base_quantity   = quantity<ronto##base_quantity##_unit>;                                              \
    using ironto##base_quantity  = iquantity<ronto##base_quantity##_unit>;                                             \
    using quecto##base_quantity  = quantity<quecto##base_quantity##_unit>;                                             \
    using iquecto##base_quantity = iquantity<quecto##base_quantity##_unit>;

#ifndef NO_PREDEFINED_DERIVED_UNITS
QUANTITY_PREFIXES(mole)
QUANTITY_PREFIXES(ampere)
QUANTITY_PREFIXES(meter)
QUANTITY_PREFIXES(gram)
QUANTITY_PREFIXES(candela)
QUANTITY_PREFIXES(second)
QUANTITY_PREFIXES(kelvin)

using radian = quantity<radian_unit>;
QUANTITY_PREFIXES(radian)
using iradian = iquantity<radian_unit>;

using degree  = quantity<degree_unit>;
using idegree = iquantity<degree_unit>;

/// \brief Specifies a quantity has dimensions of angle
///
/// \tparam quantityType The quantity to check
template <typename QuantityType>
concept angle = angle_unit<QuantityType::units>;

using newton = quantity<newton_unit>;
QUANTITY_PREFIXES(newton)

using pascal = quantity<pascal_unit>;
QUANTITY_PREFIXES(pascal)
using ipascal = iquantity<pascal_unit>;

using celsius  = quantity<celsius_unit>;
using icelsius = iquantity<celsius_unit>;

using atm  = quantity<atm_unit>;
using iatm = iquantity<atm_unit>;

// --- Additional convenience quantities ---
template <typename QuantityType>
concept force = force_unit<QuantityType::units>;

template <typename QuantityType>
concept pressure = pressure_unit<QuantityType::units>;

template <typename QuantityType>
concept energy = energy_unit<QuantityType::units>;

template <typename QuantityType>
concept power = power_unit<QuantityType::units>;

namespace metric_literals
{
QUANTITY_LITERAL(pascal, Pa)
QUANTITY_LITERAL(celsius, degC)
QUANTITY_LITERAL(atm, atm)
} // namespace metric_literals

namespace imperial_literals
{

}
#endif
} // namespace maxwell

#endif