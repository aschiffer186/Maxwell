#ifndef QUANTITY_REPO_HPP
#define QUANTITY_REPO_HPP

#include "internal/quantity.hpp"
#include "internal/unit.hpp"
#include "internal/unit_repo.hpp"

namespace maxwell {
#define MAKE_QUANTITY_PREFIXES(base_quantity)                                                                          \
  using quetta##base_quantity = quantity<quetta##base_quantity##_unit>;                                                \
  using int_quetta##base_quantity = int_quantity<quetta##base_quantity##_unit>;                                        \
  using ronna##base_quantity = quantity<ronna##base_quantity##_unit>;                                                  \
  using int_ronna##base_quantity = int_quantity<ronna##base_quantity##_unit>;                                          \
  using yotta##base_quantity = quantity<yotta##base_quantity##_unit>;                                                  \
  using int_yotta##base_quantity = int_quantity<yotta##base_quantity##_unit>;                                          \
  using zetta##base_quantity = quantity<zetta##base_quantity##_unit>;                                                  \
  using int_zetta##base_quantity = quantity<zetta##base_quantity##_unit>;

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