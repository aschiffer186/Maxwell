#ifndef ISQ_QUANTITIES_HPP
#define ISQ_QUANTITIES_HPP

#include "isq_units.hpp"
#include "quantity.hpp"

namespace maxwell::isq {
template <typename Magnitude = double>
using meter = quantity_type<meter_unit, length_kind, Magnitude>;

template <typename Magnitude = double>
using kilogram = quantity_type<kilogram_unit, mass_kind, Magnitude>;

template <typename Magnitude = double>
using second = quantity_type<second_unit, time_kind, Magnitude>;

template <typename Magnitude = double>
using ampere = quantity_type<ampere_unit, current_kind, Magnitude>;

template <typename Magnitude = double>
using kelvin = quantity_type<kelvin_unit, temperature_kind, Magnitude>;

template <typename Magnitude = double>
using mole = quantity_type<mole_unit, amount_kind, Magnitude>;

template <typename Magnitude = double>
using candela = quantity_type<candela_unit, luminosity_kind, Magnitude>;

template <typename Magnitude = double>
using square_meter = quantity_type<square_meter_unit, area_kind, Magnitude>;
} // namespace maxwell::isq

#endif