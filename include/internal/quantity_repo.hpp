#ifndef QUANTITY_REPO_HPP
#define QUANTITY_REPO_HPP

#include "internal/quantity.hpp"
#include "internal/unit.hpp"
#include "internal/unit_repo.hpp"

namespace maxwell {
using mole = quantity<mole_unit>;
using int_mole = int_quantity<mole_unit>;

using ampere = quantity<ampere_unit>;
using int_ampere = int_quantity<ampere_unit>;

using meter = quantity<meter_unit>;
using int_meter = int_quantity<meter_unit>;

using candela = quantity<candela_unit>;
using int_candela = int_quantity<candela_unit>;

using kilogram = quantity<kilogram_unit>;
using int_kilogram = int_quantity<kilogram_unit>;

using kelvin = quantity<kelvin_unit>;
using int_kelvin = int_quantity<kelvin_unit>;

using second = quantity<second_unit>;
using int_second = int_quantity<second_unit>;

using radian = quantity<radian_unit>;
using int_radian = int_quantity<radian_unit>;

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
  constexpr auto operator""_##name(unsigned long long x) noexcept { return int_quantity<unit>(x); }                    \
  constexpr auto operator""_##name(long double x) noexcept { return quantity<unit>(x); }

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
} // namespace maxwell

#endif