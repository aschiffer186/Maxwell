/// \file QuantityRepo.hpp
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
using mole     = quantity<mole_unit>;
using int_mole = int_quantity<mole_unit>;

using ampere     = quantity<ampere_unit>;
using int_ampere = int_quantity<ampere_unit>;

using meter     = quantity<meter_unit>;
using int_meter = int_quantity<meter_unit>;

using candela     = quantity<candela_unit>;
using int_candela = int_quantity<candela_unit>;

using gram     = quantity<gram_unit>;
using int_gram = int_quantity<gram_unit>;

using kelvin     = quantity<kelvin_unit>;
using int_kelvin = int_quantity<kelvin_unit>;

using second     = quantity<second_unit>;
using int_second = int_quantity<second_unit>;

using unitless_quantity     = quantity<unitless_unit_type{}>;
using int_unitless_quantity = int_quantity<unitless_unit_type{}>;

#define QUANTITY_LITERAL(name, symbol)                                                                                 \
    consteval name operator""_##symbol(long double x) noexcept                                                         \
    {                                                                                                                  \
        return name{x};                                                                                                \
    }                                                                                                                  \
    consteval int_##name operator""_##symbol(unsigned long long x) noexcept                                            \
    {                                                                                                                  \
        return int_##name{x};                                                                                          \
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

using unitless_quantity     = quantity<unitless_unit_type{}>;
using int_unitless_quantity = int_quantity<unitless_unit_type{}>;

#define QUANTITY_PREFIXES(base_quantity)                                                                               \
    using quetta##base_quantity     = quantity<quetta##base_quantity##_unit>;                                          \
    using int_quetta##base_quantity = int_quantity<quetta##base_quantity##_unit>;                                      \
    using ronna##base_quantity      = quantity<ronna##base_quantity##_unit>;                                           \
    using int_ronna##base_quantity  = int_quantity<ronna##base_quantity##_unit>;                                       \
    using yotta##base_quantity      = quantity<yotta##base_quantity##_unit>;                                           \
    using int_yotta##base_quantity  = int_quantity<yotta##base_quantity##_unit>;                                       \
    using zetta##base_quantity      = quantity<zetta##base_quantity##_unit>;                                           \
    using int_zetta##base_quantity  = int_quantity<zetta##base_quantity##_unit>;                                       \
    using exa##base_quantity        = quantity<exa##base_quantity##_unit>;                                             \
    using int_exa##base_quantity    = int_quantity<exa##base_quantity##_unit>;                                         \
    using peta##base_quantity       = quantity<peta##base_quantity##_unit>;                                            \
    using int_peta##base_quantity   = int_quantity<peta##base_quantity##_unit>;                                        \
    using tera##base_quantity       = quantity<tera##base_quantity##_unit>;                                            \
    using int_tera##base_quantity   = int_quantity<tera##base_quantity##_unit>;                                        \
    using giga##base_quantity       = quantity<giga##base_quantity##_unit>;                                            \
    using int_giga##base_quantity   = int_quantity<giga##base_quantity##_unit>;                                        \
    using mega##base_quantity       = quantity<mega##base_quantity##_unit>;                                            \
    using int_mega##base_quantity   = int_quantity<mega##base_quantity##_unit>;                                        \
    using kilo##base_quantity       = quantity<kilo##base_quantity##_unit>;                                            \
    using int_kilo##base_quantity   = int_quantity<kilo##base_quantity##_unit>;                                        \
    using hecto##base_quantity      = quantity<hecto##base_quantity##_unit>;                                           \
    using int_hecto##base_quantity  = int_quantity<hecto##base_quantity##_unit>;                                       \
    using deca##base_quantity       = quantity<deca##base_quantity##_unit>;                                            \
    using int_deca##base_quantity   = int_quantity<deca##base_quantity##_unit>;                                        \
    using deci##base_quantity       = quantity<deci##base_quantity##_unit>;                                            \
    using int_deci##base_quantity   = int_quantity<deci##base_quantity##_unit>;                                        \
    using centi##base_quantity      = quantity<centi##base_quantity##_unit>;                                           \
    using int_centi##base_quantity  = int_quantity<centi##base_quantity##_unit>;                                       \
    using milli##base_quantity      = quantity<milli##base_quantity##_unit>;                                           \
    using int_milli##base_quantity  = int_quantity<milli##base_quantity##_unit>;                                       \
    using micro##base_quantity      = quantity<micro##base_quantity##_unit>;                                           \
    using int_micro##base_quantity  = int_quantity<micro##base_quantity##_unit>;                                       \
    using nano##base_quantity       = quantity<nano##base_quantity##_unit>;                                            \
    using int_nano##base_quantity   = int_quantity<nano##base_quantity##_unit>;                                        \
    using pico##base_quantity       = quantity<pico##base_quantity##_unit>;                                            \
    using int_pico##base_quantity   = int_quantity<pico##base_quantity##_unit>;                                        \
    using femto##base_quantity      = quantity<femto##base_quantity##_unit>;                                           \
    using int_femto##base_quantity  = int_quantity<femto##base_quantity##_unit>;                                       \
    using atto##base_quantity       = quantity<atto##base_quantity##_unit>;                                            \
    using int_atto##base_quantity   = int_quantity<atto##base_quantity##_unit>;                                        \
    using zepto##base_quantity      = quantity<zepto##base_quantity##_unit>;                                           \
    using int_zepto##base_quantity  = int_quantity<zepto##base_quantity##_unit>;                                       \
    using yocto##base_quantity      = quantity<yocto##base_quantity##_unit>;                                           \
    using int_yocto##base_quantity  = int_quantity<yocto##base_quantity##_unit>;                                       \
    using ronto##base_quantity      = quantity<ronto##base_quantity##_unit>;                                           \
    using int_ronto##base_quantity  = int_quantity<ronto##base_quantity##_unit>;                                       \
    using quecto##base_quantity     = quantity<quecto##base_quantity##_unit>;                                          \
    using int_quecto##base_quantity = int_quantity<quecto##base_quantity##_unit>;

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
using int_radian = int_quantity<radian_unit>;

using degree     = quantity<degree_unit>;
using int_degree = int_quantity<degree_unit>;

/// \brief Specifies a quantity has dimensions of angle
///
/// \tparam quantityType The quantity to check
template <typename QuantityType>
concept angle = angle_unit<QuantityType::units>;

using steradian     = quantity<steradian_unit>;
using int_steradian = int_quantity<steradian_unit>;
QUANTITY_PREFIXES(steradian)

using newton     = quantity<newton_unit>;
using int_newton = int_quantity<newton_unit>;
QUANTITY_PREFIXES(newton)
using hertz     = quantity<hertz_unit>;
using int_hertz = int_quantity<hertz_unit>;
QUANTITY_PREFIXES(hertz)
using pascal     = quantity<pascal_unit>;
using int_pascal = int_quantity<pascal_unit>;
QUANTITY_PREFIXES(pascal)
using joule     = quantity<joule_unit>;
using int_joule = int_quantity<joule_unit>;
QUANTITY_PREFIXES(joule)
using watt     = quantity<watt_unit>;
using int_watt = int_quantity<watt_unit>;
QUANTITY_PREFIXES(watt)
using coulomb     = quantity<coulomb_unit>;
using int_coulomb = int_quantity<coulomb_unit>;
QUANTITY_PREFIXES(coulomb)
using volt     = quantity<volt_unit>;
using int_volt = int_quantity<volt_unit>;
QUANTITY_PREFIXES(volt)
using farad     = quantity<farad_unit>;
using int_farad = int_quantity<farad_unit>;
QUANTITY_PREFIXES(farad)
using ohm     = quantity<ohm_unit>;
using int_ohm = int_quantity<ohm_unit>;
QUANTITY_PREFIXES(ohm)
using siemens     = quantity<siemens_unit>;
using int_siemens = int_quantity<siemens_unit>;
QUANTITY_PREFIXES(siemens)
using weber     = quantity<weber_unit>;
using int_weber = int_quantity<weber_unit>;
QUANTITY_PREFIXES(weber)
using tesla     = quantity<tesla_unit>;
using int_tesla = int_quantity<tesla_unit>;
QUANTITY_PREFIXES(tesla)
using henry     = quantity<henry_unit>;
using int_henry = int_quantity<henry_unit>;
QUANTITY_PREFIXES(henry)
using lumen     = quantity<lumen_unit>;
using int_lumen = int_quantity<lumen_unit>;
QUANTITY_PREFIXES(lumen)
using lux     = quantity<lux_unit>;
using int_lux = int_quantity<lux_unit>;
QUANTITY_PREFIXES(lux)
using gray     = quantity<gray_unit>;
using int_gray = int_quantity<gray_unit>;
QUANTITY_PREFIXES(gray)
using becquerel     = quantity<becquerel_unit>;
using int_becquerel = int_quantity<becquerel_unit>;
QUANTITY_PREFIXES(becquerel)
using sievert     = quantity<sievert_unit>;
using int_sievert = int_quantity<sievert_unit>;
QUANTITY_PREFIXES(sievert)

using celsius     = quantity<celsius_unit>;
using int_celsius = int_quantity<celsius_unit>;

using atm     = quantity<atm_unit>;
using int_atm = int_quantity<atm_unit>;

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
QUANTITY_LITERAL(newton, N)
QUANTITY_LITERAL(pascal, Pa)
QUANTITY_LITERAL(joule, J)
QUANTITY_LITERAL(watt, W)
QUANTITY_LITERAL(coulomb, C)
QUANTITY_LITERAL(volt, V)
QUANTITY_LITERAL(farad, F)
QUANTITY_LITERAL(ohm, Ohm)
QUANTITY_LITERAL(siemens, S)
QUANTITY_LITERAL(weber, Wb)
QUANTITY_LITERAL(tesla, T)
QUANTITY_LITERAL(hertz, Hz)
QUANTITY_LITERAL(lumen, lm)
QUANTITY_LITERAL(lux, lx)
QUANTITY_LITERAL(gray, Gy)
QUANTITY_LITERAL(becquerel, Bq)
QUANTITY_LITERAL(sievert, Sv)
QUANTITY_LITERAL(celsius, degC)
QUANTITY_LITERAL(atm, atm)
} // namespace metric_literals

// Imperial units
using foot     = quantity<foot_unit>;
using int_foot = int_quantity<foot_unit>;
using inch     = quantity<inch_unit>;
using int_inch = int_quantity<inch_unit>;
using yard     = quantity<yard_unit>;
using int_yard = int_quantity<yard_unit>;
using mile     = quantity<mile_unit>;
using int_mile = int_quantity<mile_unit>;

using int_pound = int_quantity<pound_unit>;
using pound     = quantity<pound_unit>;

using minute     = quantity<minute_unit>;
using int_minute = int_quantity<minute_unit>;
using hour       = quantity<hour_unit>;
using int_hour   = int_quantity<hour_unit>;
using day        = quantity<day_unit>;
using int_day    = int_quantity<day_unit>;
using week       = quantity<week_unit>;
using int_week   = int_quantity<week_unit>;
using year       = quantity<year_unit>;
using int_year   = int_quantity<year_unit>;

using fahrenheit     = quantity<fahrenheit_unit>;
using int_fahrenheit = int_quantity<fahrenheit_unit>;

namespace imperial_literals
{
QUANTITY_LITERAL(foot, ft)
QUANTITY_LITERAL(inch, in)
QUANTITY_LITERAL(yard, yd)
QUANTITY_LITERAL(mile, mi)
QUANTITY_LITERAL(pound, lb)
QUANTITY_LITERAL(minute, min)
QUANTITY_LITERAL(hour, hr)
QUANTITY_LITERAL(day, day)
QUANTITY_LITERAL(week, week)
QUANTITY_LITERAL(year, yr)
QUANTITY_LITERAL(fahrenheit, degF)
} // namespace imperial_literals
#endif
} // namespace maxwell

#endif