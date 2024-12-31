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

#ifndef NO_PREDEFINED_DERIVED_UNITS
using radian  = quantity<radian_unit>;
using iradian = iquantity<radian_unit>;

using degree  = quantity<degree_unit>;
using idegree = iquantity<degree_unit>;

/// \brief Specifies a quantity has dimensions of angle
///
/// \tparam quantityType The quantity to check
template <typename QuantityType>
concept angle = angle_unit<QuantityType::units>;

template <typename QuantityType>
concept pressure = pressure_unit<QuantityType::units>;

using pascal  = quantity<pascal_unit>;
using ipascal = quantity<pascal_unit>;
#endif
} // namespace maxwell

#endif