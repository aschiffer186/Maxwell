/// \file si.hpp
/// \brief Definition of units in the SI system.

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

constexpr struct number_unit_type
    : make_base_unit_t<isq::dimensionless_quantity, "[]"> {
} number_unit;

// --- SI derived unit types ---
constexpr struct radian_unit_type
    : make_derived_unit_t<isq::plane_angle_quantity, "rad"> {
} radian_unit;

constexpr struct steradian_unit_type
    : make_derived_unit_t<isq::solid_angle_quantity, "sr"> {
} steradian_unit;

constexpr struct hertz_unit_type
    : make_derived_unit_t<isq::frequency_quantity, "Hz"> {
} hertz_unit;

constexpr struct newton_unit_type
    : make_derived_unit_t<isq::force_quantity, "N"> {
} newton_unit;

constexpr struct pascal_unit_type
    : make_derived_unit_t<isq::pressure_quantity, "Pa"> {
} pascal_unit;

constexpr struct joule_unit_type
    : make_derived_unit_t<isq::work_quantity, "J"> {
} joule_unit;

constexpr struct newton_meter_unit_type
    : make_derived_unit_t<isq::torque_quantity, "N·m"> {
} newton_meter_unit;

constexpr struct watt_unit_type
    : make_derived_unit_t<isq::power_quantity, "W"> {
} watt_unit;

constexpr struct coulumb_unit_type
    : make_derived_unit_t<isq::charge_quantity, "C"> {
} coulumb_unit;

constexpr struct square_meter_unit_type
    : make_derived_unit_t<isq::area_quantity, "m^2"> {
} square_meter_unit;

constexpr struct meter_per_second_unit_type
    : make_derived_unit_t<isq::velocity_quantity, "m/s"> {
} meter_per_second_unit;

template <typename T = double>
using meter = quantity_value<meter_unit, isq::length_quantity, T>;

template <typename T = double>
using ampere = quantity_value<ampere_unit, isq::current_quantity, T>;

template <typename T = double>
using second = quantity_value<second_unit, isq::time_quantity, T>;

template <typename T = double>
using kelvin = quantity_value<kelvin_unit, isq::temperature_quantity, T>;

template <typename T = double>
using mole = quantity_value<mole_unit, isq::amount_quantity, T>;

template <typename T = double>
using candela = quantity_value<candela_unit, isq::luminosity_quantity, T>;

template <typename T = double>
using number = quantity_value<number_unit, isq::dimensionless_quantity, T>;

template <typename T = double>
using newton = quantity_value<newton_unit, isq::force_quantity, T>;

template <typename T = double>
using pascal = quantity_value<pascal_unit, isq::pressure_quantity, T>;

template <typename T = double>
using joule = quantity_value<joule_unit, isq::work_quantity, T>;

template <typename T = double>
using square_meter = quantity_value<square_meter_unit, isq::area_quantity, T>;

template <typename T = double>
using newton_meter = quantity_value<newton_meter_unit, isq::torque_quantity, T>;

template <typename T = double>
using meter_per_second =
    quantity_value<meter_per_second_unit, isq::velocity_quantity, T>;

/// \namespace maxwell::si::symbols
/// \brief Convenience symbols for creating units
namespace symbols {
constexpr unit auto m = meter_unit;
constexpr unit auto kg = kilogram_unit;
constexpr unit auto s = second_unit;
constexpr unit auto A = ampere_unit;
constexpr unit auto K = kelvin_unit;
constexpr unit auto mol = mole_unit;
constexpr unit auto cd = candela_unit;
constexpr unit auto one = number_unit;
constexpr unit auto rad = radian_unit;
constexpr unit auto sr = steradian_unit;
constexpr unit auto Hz = hertz_unit;
constexpr unit auto N = newton_unit;
constexpr unit auto Pa = pascal_unit;
constexpr unit auto J = joule_unit;
constexpr unit auto N_m = newton_meter_unit;
constexpr unit auto W = watt_unit;
constexpr unit auto C = coulumb_unit;
constexpr unit auto m2 = square_meter_unit;
} // namespace symbols
} // namespace maxwell::si

#endif