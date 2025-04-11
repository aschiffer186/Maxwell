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

using radian = quantity<radian_unit>;
using int_radian = int_quantity<radian_unit>;

using degree = quantity<degree_unit>;
using int_degree = int_quantity<degree_unit>;

using hertz = quantity<hertz_unit>;
using int_hertz = int_quantity<hertz_unit>;

using joule = quantity<joule_unit>;
using int_joule = int_quantity<joule_unit>;

using coulumb = quantity<coulomb_unit>;
using int_coulumb = int_quantity<coulomb_unit>;

using square_meter = quantity<square_meter_unit>;
using int_square_meter = int_quantity<square_meter_unit>;
using square_decimeter = quantity<square_decimeter_unit>;
using square_centimeter = quantity<square_centimeter_unit>;
using int_square_centimeter = int_quantity<square_centimeter_unit>;

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

template <typename T>
concept angle = angle_unit<typename T::units_type{}>;
} // namespace maxwell

#endif