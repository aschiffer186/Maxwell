/**
 * @file quantity_repo.hpp
 * @author Alex Schiffer
 * @brief Definition of common instntiations of \c quantity
 * @version 1.0
 * @date 2025-04-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef QUANTITY_REPO_HPP
#define QUANTITY_REPO_HPP

#include "dimension.hpp"
#include "quantity.hpp"
#include "unit.hpp"
#include "unit_repo.hpp"

namespace maxwell {
#define MAKE_QUANTITY_PREFIXES(base_quantity)                                                                          \
  using quetta##base_quantity = quantity<quetta##base_quantity##_unit>;                                                \
  using int_quetta##base_quantity = int_quantity<quetta##base_quantity##_unit>;                                        \
  using ronna##base_quantity = quantity<ronna##base_quantity##_unit>;                                                  \
  using int_ronna##base_quantity = int_quantity<ronna##base_quantity##_unit>;                                          \
  using yotta##base_quantity = quantity<yotta##base_quantity##_unit>;                                                  \
  using int_yotta##base_quantity = int_quantity<yotta##base_quantity##_unit>;                                          \
  using zetta##base_quantity = quantity<zetta##base_quantity##_unit>;                                                  \
  using int_zetta##base_quantity = quantity<zetta##base_quantity##_unit>;                                              \
  using exa##base_quantity = quantity<exa##base_quantity##_unit>;                                                      \
  using int_exa##base_quantity = int_quantity<exa##base_quantity##_unit>;                                              \
  using peta##base_quantity = quantity<peta##base_quantity##_unit>;                                                    \
  using int_peta##base_quantity = int_quantity<peta##base_quantity##_unit>;                                            \
  using tera##base_quantity = quantity<tera##base_quantity##_unit>;                                                    \
  using int_tera##base_quantity = int_quantity<tera##base_quantity##_unit>;                                            \
  using giga##base_quantity = quantity<giga##base_quantity##_unit>;                                                    \
  using int_giga##base_quantity = int_quantity<giga##base_quantity##_unit>;                                            \
  using mega##base_quantity = quantity<mega##base_quantity##_unit>;                                                    \
  using int_mega##base_quantity = int_quantity<mega##base_quantity##_unit>;                                            \
  using kilo##base_quantity = quantity<kilo##base_quantity##_unit>;                                                    \
  using int_kilo##base_quantity = int_quantity<kilo##base_quantity##_unit>;                                            \
  using hecto##base_quantity = quantity<hecto##base_quantity##_unit>;                                                  \
  using int_hecto##base_quantity = int_quantity<hecto##base_quantity##_unit>;                                          \
  using deca##base_quantity = quantity<deca##base_quantity##_unit>;                                                    \
  using int_deca##base_quantity = int_quantity<deca##base_quantity##_unit>;                                            \
  using deci##base_quantity = quantity<deci##base_quantity##_unit>;                                                    \
  using int_deci##base_quantity = int_quantity<deci##base_quantity##_unit>;                                            \
  using centi##base_quantity = quantity<centi##base_quantity##_unit>;                                                  \
  using int_centi##base_quantity = int_quantity<centi##base_quantity##_unit>;                                          \
  using milli##base_quantity = quantity<milli##base_quantity##_unit>;                                                  \
  using int_milli##base_quantity = int_quantity<milli##base_quantity##_unit>;                                          \
  using micro##base_quantity = quantity<micro##base_quantity##_unit>;                                                  \
  using int_micro##base_quantity = int_quantity<micro##base_quantity##_unit>;                                          \
  using nano##base_quantity = quantity<nano##base_quantity##_unit>;                                                    \
  using int_nano##base_quantity = int_quantity<nano##base_quantity##_unit>;                                            \
  using pico##base_quantity = quantity<pico##base_quantity##_unit>;                                                    \
  using int_pico##base_quantity = int_quantity<pico##base_quantity##_unit>;                                            \
  using femto##base_quantity = quantity<femto##base_quantity##_unit>;                                                  \
  using int_femto##base_quantity = int_quantity<femto##base_quantity##_unit>;                                          \
  using atto##base_quantity = quantity<atto##base_quantity##_unit>;                                                    \
  using int_atto##base_quantity = int_quantity<atto##base_quantity##_unit>;                                            \
  using zepto##base_quantity = quantity<zepto##base_quantity##_unit>;                                                  \
  using int_zepto##base_quantity = int_quantity<zepto##base_quantity##_unit>;                                          \
  using yocto##base_quantity = quantity<yocto##base_quantity##_unit>;                                                  \
  using int_yocto##base_quantity = int_quantity<yocto##base_quantity##_unit>;                                          \
  using ronto##base_quantity = quantity<ronto##base_quantity##_unit>;                                                  \
  using int_ronto##base_quantity = int_quantity<ronto##base_quantity##_unit>;                                          \
  using quecto##base_quantity = quantity<quecto##base_quantity##_unit>;                                                \
  using int_quecto##base_quantity = int_quantity<quecto##base_quantity##_unit>;

using mole = quantity<mole_unit>;
using int_mole = int_quantity<mole_unit>;
MAKE_QUANTITY_PREFIXES(mole)

using ampere = quantity<ampere_unit>;
using int_ampere = int_quantity<ampere_unit>;
MAKE_QUANTITY_PREFIXES(ampere)

using meter = quantity<meter_unit>;
using int_meter = int_quantity<meter_unit>;
MAKE_QUANTITY_PREFIXES(meter)

using candela = quantity<candela_unit>;
using int_candela = int_quantity<candela_unit>;
MAKE_QUANTITY_PREFIXES(candela)

using gram = quantity<gram_unit>;
using int_gram = int_quantity<gram_unit>;
MAKE_QUANTITY_PREFIXES(gram)

using kelvin = quantity<kelvin_unit>;
using int_kelvin = int_quantity<kelvin_unit>;
MAKE_QUANTITY_PREFIXES(kelvin)

using second = quantity<second_unit>;
using int_second = int_quantity<second_unit>;
MAKE_QUANTITY_PREFIXES(second)

template <typename T>
concept angle = angle_unit<typename T::units_type{}>;

using radian = quantity<radian_unit>;
using int_radian = int_quantity<radian_unit>;

#ifndef DISABLE_PREDEFINED_UNITS
using degree = quantity<degree_unit>;
using int_degree = int_quantity<degree_unit>;

using hertz = quantity<hertz_unit>;
using int_hertz = int_quantity<hertz_unit>;

using joule = quantity<joule_unit>;
using int_joule = int_quantity<joule_unit>;

using coulumb = quantity<coulomb_unit>;
using int_coulumb = int_quantity<coulomb_unit>;

using Celsisus = quantity<Celsisus_unit>;
using int_Celsisus = int_quantity<Celsisus_unit>;

using square_quettameter = quantity<square_quettameter_unit>;
using int_square_quettameter = int_quantity<square_quettameter_unit>;
using square_ronnameter = quantity<square_ronnameter_unit>;
using int_square_ronnameter = int_quantity<square_ronnameter_unit>;
using square_yottameter = quantity<square_yottameter_unit>;
using int_square_yottameter = int_quantity<square_yottameter_unit>;
using square_zettameter = quantity<square_zettameter_unit>;
using int_square_zettameter = int_quantity<square_zettameter_unit>;
using square_exameter = quantity<square_exameter_unit>;
using int_square_exameter = int_quantity<square_exameter_unit>;
using square_petameter = quantity<square_petameter_unit>;
using int_square_petameter = int_quantity<square_petameter_unit>;
using square_terameter = quantity<square_terameter_unit>;
using int_square_terameter = int_quantity<square_terameter_unit>;
using square_gigameter = quantity<square_gigameter_unit>;
using int_square_gigameter = int_quantity<square_gigameter_unit>;
using square_megameter = quantity<square_megameter_unit>;
using int_square_megameter = int_quantity<square_megameter_unit>;
using square_kilometer = quantity<square_kilometer_unit>;
using int_square_kilometer = int_quantity<square_kilometer_unit>;
using square_hectometer = quantity<square_hectometer_unit>;
using int_square_hectometer = int_quantity<square_hectometer_unit>;
using square_decameter = quantity<square_decameter_unit>;
using int_square_decameter = int_quantity<square_decameter_unit>;
using square_meter = quantity<square_meter_unit>;
using int_square_meter = int_quantity<square_meter_unit>;
using square_decimeter = quantity<square_decimeter_unit>;
using square_centimeter = quantity<square_centimeter_unit>;
using int_square_centimeter = int_quantity<square_centimeter_unit>;
using square_millimeter = quantity<square_millimeter_unit>;
using int_square_millimeter = int_quantity<square_millimeter_unit>;
using square_micrometer = quantity<square_micrometer_unit>;
using int_square_micrometer = int_quantity<square_micrometer_unit>;
using square_nanometer = quantity<square_nanometer_unit>;
using int_square_nanometer = int_quantity<square_nanometer_unit>;
using square_picometer = quantity<square_picometer_unit>;
using int_square_picometer = int_quantity<square_picometer_unit>;
using square_femtometer = quantity<square_femtometer_unit>;
using int_square_femtometer = int_quantity<square_femtometer_unit>;
using square_attometer = quantity<square_attometer_unit>;
using int_square_attometer = int_quantity<square_attometer_unit>;
using square_zeptometer = quantity<square_zeptometer_unit>;
using int_square_zeptometer = int_quantity<square_zeptometer_unit>;
using square_yoctometer = quantity<square_yoctometer_unit>;
using int_square_yoctometer = int_quantity<square_yoctometer_unit>;
using square_rontometer = quantity<square_rontometer_unit>;
using int_square_rontometer = int_quantity<square_rontometer_unit>;
using square_quectometer = quantity<square_quectometer_unit>;
using int_square_quectometer = int_quantity<square_quectometer_unit>;

using cubic_quettameter = quantity<cubic_quettameter_unit>;
using int_cubic_quettameter = int_quantity<cubic_quettameter_unit>;
using cubic_ronnameter = quantity<cubic_ronnameter_unit>;
using int_cubic_ronnameter = int_quantity<cubic_ronnameter_unit>;
using cubic_yottameter = quantity<cubic_yottameter_unit>;
using int_cubic_yottameter = int_quantity<cubic_yottameter_unit>;
using cubic_zettameter = quantity<cubic_zettameter_unit>;
using int_cubic_zettameter = int_quantity<cubic_zettameter_unit>;
using cubic_exameter = quantity<cubic_exameter_unit>;
using int_cubic_exameter = int_quantity<cubic_exameter_unit>;
using cubic_petameter = quantity<cubic_petameter_unit>;
using int_cubic_petameter = int_quantity<cubic_petameter_unit>;
using cubic_terameter = quantity<cubic_terameter_unit>;
using int_cubic_terameter = int_quantity<cubic_terameter_unit>;
using cubic_gigameter = quantity<cubic_gigameter_unit>;
using int_cubic_gigameter = int_quantity<cubic_gigameter_unit>;
using cubic_megameter = quantity<cubic_megameter_unit>;
using int_cubic_megameter = int_quantity<cubic_megameter_unit>;
using cubic_kilometer = quantity<cubic_kilometer_unit>;
using int_cubic_kilometer = int_quantity<cubic_kilometer_unit>;
using cubic_hectometer = quantity<cubic_hectometer_unit>;
using int_cubic_hectometer = int_quantity<cubic_hectometer_unit>;
using cubic_decameter = quantity<cubic_decameter_unit>;
using int_cubic_decameter = int_quantity<cubic_decameter_unit>;
using cubic_meter = quantity<cubic_meter_unit>;
using int_cubic_meter = int_quantity<cubic_meter_unit>;
using cubic_decimeter = quantity<cubic_decimeter_unit>;
using cubic_centimeter = quantity<cubic_centimeter_unit>;
using int_cubic_centimeter = int_quantity<cubic_centimeter_unit>;
using cubic_millimeter = quantity<cubic_millimeter_unit>;
using int_cubic_millimeter = int_quantity<cubic_millimeter_unit>;
using cubic_micrometer = quantity<cubic_micrometer_unit>;
using int_cubic_micrometer = int_quantity<cubic_micrometer_unit>;
using cubic_nanometer = quantity<cubic_nanometer_unit>;
using int_cubic_nanometer = int_quantity<cubic_nanometer_unit>;
using cubic_picometer = quantity<cubic_picometer_unit>;
using int_cubic_picometer = int_quantity<cubic_picometer_unit>;
using cubic_femtometer = quantity<cubic_femtometer_unit>;
using int_cubic_femtometer = int_quantity<cubic_femtometer_unit>;
using cubic_attometer = quantity<cubic_attometer_unit>;
using int_cubic_attometer = int_quantity<cubic_attometer_unit>;
using cubic_zeptometer = quantity<cubic_zeptometer_unit>;
using int_cubic_zeptometer = int_quantity<cubic_zeptometer_unit>;
using cubic_yoctometer = quantity<cubic_yoctometer_unit>;
using int_cubic_yoctometer = int_quantity<cubic_yoctometer_unit>;
using cubic_rontometer = quantity<cubic_rontometer_unit>;
using int_cubic_rontometer = int_quantity<cubic_rontometer_unit>;
using cubic_quectometer = quantity<cubic_quectometer_unit>;
using int_cubic_quectometer = int_quantity<cubic_quectometer_unit>;

using meter_per_second = quantity<meter_per_second_unit>;
using int_meter_per_second = int_quantity<meter_per_second_unit>;

using meter_per_second_per_second = quantity<meter_per_second_per_second_unit>;
using int_meter_per_second_per_second = int_quantity<meter_per_second_per_second_unit>;

#define QUANTITY_LITERAL(name, unit)                                                                                   \
  constexpr int_quantity<unit> operator""_##name(unsigned long long x) noexcept { return int_quantity<unit>(x); }      \
  constexpr quantity<unit> operator""_##name(long double x) noexcept { return quantity<unit>(x); }

namespace metric_literals {
QUANTITY_LITERAL(mol, mole_unit)
QUANTITY_LITERAL(A, ampere_unit)
QUANTITY_LITERAL(m, meter_unit)
QUANTITY_LITERAL(cd, candela_unit)
QUANTITY_LITERAL(kg, kilogram_unit)
QUANTITY_LITERAL(K, kelvin_unit)
QUANTITY_LITERAL(s, second_unit)
QUANTITY_LITERAL(rad, radian_unit)

QUANTITY_LITERAL(Hz, hertz_unit)
QUANTITY_LITERAL(J, joule_unit)
QUANTITY_LITERAL(C, coulomb_unit)

QUANTITY_LITERAL(m_s, meter_per_second_unit)
QUANTITY_LITERAL(m_s_s, meter_per_second_per_second_unit)
} // namespace metric_literals

using foot = quantity<foot_unit>;
using int_foot = int_quantity<foot_unit>;
using inch = quantity<inch_unit>;
using int_inch = int_quantity<inch_unit>;
using mile = quantity<mile_unit>;
using int_mile = int_quantity<mile_unit>;
using yard = quantity<yard_unit>;
using int_yard = int_quantity<yard_unit>;
using square_foot = quantity<square_foot_unit>;
using int_square_foot = int_quantity<square_foot_unit>;
using square_inch = quantity<square_inch_unit>;
using int_square_inch = int_quantity<square_inch_unit>;
using square_yard = quantity<square_yard_unit>;
using int_square_yard = int_quantity<square_yard_unit>;
using square_mile = quantity<square_mile_unit>;
using int_square_mile = int_quantity<square_mile_unit>;
using cubic_foot = quantity<cubic_foot_unit>;
using int_cubic_foot = int_quantity<cubic_foot_unit>;
using cubic_inch = quantity<cubic_inch_unit>;
using int_cubic_inch = int_quantity<cubic_inch_unit>;
using cubic_yard = quantity<cubic_yard_unit>;
using int_cubic_yard = int_quantity<cubic_yard_unit>;
using cubic_mile = quantity<cubic_mile_unit>;
using int_cubic_mile = int_quantity<cubic_mile_unit>;

using minute = quantity<minute_unit>;
using int_minute = int_quantity<minute_unit>;
using hour = quantity<hour_unit>;
using int_hour = int_quantity<hour_unit>;
using day = quantity<day_unit>;
using int_day = int_quantity<day_unit>;
using week = quantity<week_unit>;
using int_week = int_quantity<week_unit>;
using year = quantity<year_unit>;
using int_year = int_quantity<year_unit>;

using pound = quantity<pound_unit>;
using int_pound = int_quantity<pound_unit>;
using ounce = quantity<ounce_unit>;
using int_ounce = int_quantity<ounce_unit>;

using Fahrenheit = quantity<Fahrenheit_unit>;
using int_Fahrenheit = int_quantity<Fahrenheit_unit>;
#endif
} // namespace maxwell

#endif