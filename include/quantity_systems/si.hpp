/// \file si.hpp
/// \brief Definition of units in the SI system.

#ifndef SI_HPP
#define SI_HPP

#include <numbers>

#include "core/quantity_value.hpp"
#include "core/unit.hpp"
#include "isq.hpp"
#include "utility/config.hpp"

namespace maxwell::si {
MODULE_EXPORT constexpr struct meter_unit_type : base_unit<isq::length, "m"> {
} meter_unit;

MODULE_EXPORT constexpr struct gram_unit_type : base_unit<isq::mass, "g"> {
} gram_unit;

MODULE_EXPORT constexpr struct kilogram_unit_type
    : derived_unit<kilo_unit<gram_unit>, "kg"> {
} kilogram_unit;

MODULE_EXPORT constexpr struct second_unit_type : base_unit<isq::time, "s"> {
} second_unit;

MODULE_EXPORT constexpr struct ampere_unit_type : base_unit<isq::current, "A"> {
} ampere_unit;

MODULE_EXPORT constexpr struct kelvin_unit_type
    : base_unit<isq::temperature, "K"> {
} kelvin_unit;

MODULE_EXPORT constexpr struct mole_unit_type : base_unit<isq::amount, "mol"> {
} mole_unit;

MODULE_EXPORT constexpr struct candela_unit_type
    : base_unit<isq::luminosity, "cd"> {
} candela_unit;

MODULE_EXPORT constexpr struct number_unit_type
    : base_unit<isq::dimensionless, "[]"> {
} number_unit;

// --- SI derived unit types ---
MODULE_EXPORT constexpr struct radian_unit_type
    : derived_unit<isq::plane_angle, "rad"> {
} radian_unit;

/// \cond
namespace _detail {
constexpr auto rad_to_deg = value<180.0 / std::numbers::pi>;
constexpr auto kelvin_to_celsius = value<-273.15>;
} // namespace _detail
/// \endcond

MODULE_EXPORT constexpr struct celsius_unit_type
    : derived_unit<kelvin_unit + _detail::kelvin_to_celsius, "°C"> {
} celsius_unit;

MODULE_EXPORT constexpr struct degree_unit_type
    : derived_unit<_detail::rad_to_deg * radian_unit, "deg"> {
} degree_unit;

MODULE_EXPORT constexpr struct steradian_unit_type
    : derived_unit<isq::solid_angle, "sr"> {
} steradian_unit;

MODULE_EXPORT constexpr struct hertz_unit_type
    : derived_unit<isq::frequency, "Hz"> {
} hertz_unit;

MODULE_EXPORT constexpr struct newton_unit_type
    : derived_unit<isq::force, "N"> {
} newton_unit;

MODULE_EXPORT constexpr struct pascal_unit_type
    : derived_unit<isq::pressure, "Pa"> {
} pascal_unit;

MODULE_EXPORT constexpr struct joule_unit_type : derived_unit<isq::work, "J"> {
} joule_unit;

MODULE_EXPORT constexpr struct newton_meter_unit_type
    : derived_unit<isq::torque, "N·m"> {
} newton_meter_unit;

MODULE_EXPORT constexpr struct watt_unit_type : derived_unit<isq::power, "W"> {
} watt_unit;

MODULE_EXPORT constexpr struct coulomb_unit_type
    : derived_unit<isq::charge, "C"> {
} coulomb_unit;

MODULE_EXPORT constexpr struct volt_unit_type
    : derived_unit<isq::potential, "V"> {
} volt_unit;

MODULE_EXPORT constexpr struct ohm_unit_type
    : derived_unit<isq::resistance, "Ω"> {
} ohm_unit;

MODULE_EXPORT constexpr struct siemens_unit_type
    : derived_unit<isq::conductance, "S"> {
} siemens_unit;

MODULE_EXPORT constexpr struct farad_unit_type
    : derived_unit<isq::capacitance, "F"> {
} farad_unit;

MODULE_EXPORT constexpr struct weber_unit_type
    : derived_unit<isq::magnetic_flux, "Wb"> {
} weber_unit;

MODULE_EXPORT constexpr struct tesla_unit_type
    : derived_unit<isq::magnetic_flux_density, "T"> {
} tesla_unit;

MODULE_EXPORT constexpr struct henry_unit_type
    : derived_unit<isq::inductance, "H"> {
} henry_unit;

MODULE_EXPORT constexpr struct lumen_unit_type
    : derived_unit<isq::luminous_flux, "lm"> {
} lumen_unit;

MODULE_EXPORT constexpr struct lux_unit_type
    : derived_unit<isq::illuminance, "lx"> {
} lux_unit;

MODULE_EXPORT constexpr struct becquerel_unit_type
    : derived_unit<isq::radioactivity, "Bq"> {
} becquerel_unit;

MODULE_EXPORT constexpr struct gray_unit_type : derived_unit<isq::dose, "Gy"> {
} gray_unit;

MODULE_EXPORT constexpr struct sievert_unit_type
    : derived_unit<isq::dose_equivalent, "Sv"> {
} sievert_unit;

MODULE_EXPORT constexpr struct katal_unit_type
    : derived_unit<isq::catalytic_activity, "kat"> {
} katal_unit;

MODULE_EXPORT constexpr struct square_meter_unit_type
    : derived_unit<isq::area, "m^2"> {
} square_meter_unit;

MODULE_EXPORT constexpr struct cubic_meter_unit_type
    : derived_unit<isq::volume, "m^3"> {
} cubic_meter_unit;

MODULE_EXPORT constexpr struct meter_per_second_unit_type
    : derived_unit<isq::velocity, "m/s"> {
} meter_per_second_unit;

MODULE_EXPORT constexpr struct meter_per_second_per_second_unit_type
    : derived_unit<isq::acceleration, "m/s^2"> {
} meter_per_second_per_second_unit;

template <typename T = double>
MODULE_EXPORT using meter = quantity_value<meter_unit, isq::length, T>;

template <typename T = double>
MODULE_EXPORT using ampere = quantity_value<ampere_unit, isq::current, T>;

template <typename T = double>
MODULE_EXPORT using second = quantity_value<second_unit, isq::time, T>;

template <typename T = double>
MODULE_EXPORT using kelvin = quantity_value<kelvin_unit, isq::temperature, T>;

template <typename T = double>
MODULE_EXPORT using mole = quantity_value<mole_unit, isq::amount, T>;

template <typename T = double>
MODULE_EXPORT using candela = quantity_value<candela_unit, isq::luminosity, T>;

template <typename T = double>
MODULE_EXPORT using number = quantity_value<number_unit, isq::dimensionless, T>;

template <typename T = double>
MODULE_EXPORT using radian = quantity_value<radian_unit, isq::plane_angle, T>;

template <typename T = double>
MODULE_EXPORT using degree = quantity_value<degree_unit, isq::plane_angle, T>;

template <typename T = double>
MODULE_EXPORT using celsius = quantity_value<celsius_unit, isq::temperature, T>;

template <typename T = double>
MODULE_EXPORT using newton = quantity_value<newton_unit, isq::force, T>;

template <typename T = double>
MODULE_EXPORT using pascal = quantity_value<pascal_unit, isq::pressure, T>;

template <typename T = double>
MODULE_EXPORT using joule = quantity_value<joule_unit, isq::work, T>;

MODULE_EXPORT template <typename T = double>
using watt = quantity_value<watt_unit, isq::power, T>;

MODULE_EXPORT template <typename T = double>
using coulomb = quantity_value<coulomb_unit, isq::charge, T>;

MODULE_EXPORT template <typename T = double>
using volt = quantity_value<volt_unit, isq::potential, T>;

MODULE_EXPORT template <typename T = double>
using ohm = quantity_value<ohm_unit, isq::resistance, T>;

MODULE_EXPORT template <typename T = double>
using siemens = quantity_value<siemens_unit, isq::conductance, T>;

MODULE_EXPORT template <typename T = double>
using farad = quantity_value<farad_unit, isq::capacitance, T>;

MODULE_EXPORT template <typename T = double>
using weber = quantity_value<weber_unit, isq::magnetic_flux, T>;

MODULE_EXPORT template <typename T = double>
using tesla = quantity_value<tesla_unit, isq::magnetic_flux_density, T>;

MODULE_EXPORT template <typename T = double>
using henry = quantity_value<henry_unit, isq::inductance, T>;

MODULE_EXPORT template <typename T = double>
using lumen = quantity_value<lumen_unit, isq::luminous_flux, T>;

MODULE_EXPORT template <typename T = double>
using lux = quantity_value<lux_unit, isq::illuminance, T>;

MODULE_EXPORT template <typename T = double>
using becquerel = quantity_value<becquerel_unit, isq::radioactivity, T>;

MODULE_EXPORT template <typename T = double>
using gray = quantity_value<gray_unit, isq::dose, T>;

MODULE_EXPORT template <typename T = double>
using sievert = quantity_value<sievert_unit, isq::dose_equivalent, T>;

MODULE_EXPORT template <typename T = double>
using katal = quantity_value<katal_unit, isq::catalytic_activity, T>;

template <typename T = double>
MODULE_EXPORT using square_meter =
    quantity_value<square_meter_unit, isq::area, T>;

template <typename T = double>
MODULE_EXPORT using cubic_meter =
    quantity_value<cubic_meter_unit, isq::volume, T>;

template <typename T = double>
MODULE_EXPORT using newton_meter =
    quantity_value<newton_meter_unit, isq::torque, T>;

template <typename T = double>
MODULE_EXPORT using meter_per_second =
    quantity_value<meter_per_second_unit, isq::velocity, T>;

template <typename T = double>
MODULE_EXPORT using meter_per_second_per_second =
    quantity_value<meter_per_second_per_second_unit, isq::acceleration, T>;

/// \namespace maxwell::si::symbols
/// \brief Convenience symbols for creating units
namespace symbols {
MODULE_EXPORT constexpr unit auto m = meter_unit;
MODULE_EXPORT constexpr unit auto kg = kilogram_unit;
MODULE_EXPORT constexpr unit auto s = second_unit;
MODULE_EXPORT constexpr unit auto A = ampere_unit;
MODULE_EXPORT constexpr unit auto K = kelvin_unit;
MODULE_EXPORT constexpr unit auto degC = celsius_unit;
MODULE_EXPORT constexpr unit auto mol = mole_unit;
MODULE_EXPORT constexpr unit auto cd = candela_unit;
MODULE_EXPORT constexpr unit auto one = number_unit;
MODULE_EXPORT constexpr unit auto rad = radian_unit;
MODULE_EXPORT constexpr unit auto deg = degree_unit;
MODULE_EXPORT constexpr unit auto sr = steradian_unit;
MODULE_EXPORT constexpr unit auto Hz = hertz_unit;
MODULE_EXPORT constexpr unit auto N = newton_unit;
MODULE_EXPORT constexpr unit auto Pa = pascal_unit;
MODULE_EXPORT constexpr unit auto J = joule_unit;
MODULE_EXPORT constexpr unit auto N_m = newton_meter_unit;
MODULE_EXPORT constexpr unit auto W = watt_unit;
MODULE_EXPORT constexpr unit auto C = coulomb_unit;
MODULE_EXPORT constexpr unit auto V = volt_unit;
MODULE_EXPORT constexpr unit auto O = ohm_unit;
MODULE_EXPORT constexpr unit auto S = siemens_unit;
MODULE_EXPORT constexpr unit auto F = farad_unit;
MODULE_EXPORT constexpr unit auto Wb = weber_unit;
MODULE_EXPORT constexpr unit auto T = tesla_unit;
MODULE_EXPORT constexpr unit auto H = henry_unit;
MODULE_EXPORT constexpr unit auto lm = lumen_unit;
MODULE_EXPORT constexpr unit auto lx = lux_unit;
MODULE_EXPORT constexpr unit auto Bq = becquerel_unit;
MODULE_EXPORT constexpr unit auto Gy = gray_unit;
MODULE_EXPORT constexpr unit auto Sv = sievert_unit;
MODULE_EXPORT constexpr unit auto kat = katal_unit;
MODULE_EXPORT constexpr unit auto m2 = square_meter_unit;
MODULE_EXPORT constexpr unit auto m_s = meter_per_second_unit;
} // namespace symbols
} // namespace maxwell::si

#endif