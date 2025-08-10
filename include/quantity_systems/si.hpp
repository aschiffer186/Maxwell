#ifndef SI_HPP
#define SI_HPP

#include "isq.hpp"
#include "quantity_value.hpp"
#include "unit.hpp"

namespace maxwell::si {
constexpr struct meter_unit_type : make_base_unit_t<isq::length_quantity, "m"> {
} meter_unit;

constexpr struct kilogram_unit_type
    : make_base_unit_t<isq::mass_quantity, "kg"> {
} kilogram_unit;

constexpr struct second_unit_type : make_base_unit_t<isq::time_quantity, "s"> {
} second_unit;

constexpr struct ampere_unit_type
    : make_base_unit_t<isq::current_quantity, "A"> {
} ampere_unit;

constexpr struct kelvin_unit_type
    : make_base_unit_t<isq::temperature_quantity, "K"> {
} kelvin_unit;

constexpr struct mole_unit_type
    : make_base_unit_t<isq::amount_quantity, "mol"> {
} mole_unit;

constexpr struct candela_unit_type
    : make_base_unit_t<isq::luminosity_quantity, "cd"> {
} candela_unit;

template <typename T = double>
using meter = quantity_value<isq::length_quantity, meter_unit, T>;

template <typename T = double>
using ampere = quantity_value<isq::current_quantity, ampere_unit, T>;
} // namespace maxwell::si

#endif