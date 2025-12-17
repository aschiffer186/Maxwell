#ifndef SI_CONSTANTS_HPP
#define SI_CONSTANTS_HPP

#include "si.hpp"
#include "utility/config.hpp"
#include <numbers>

namespace maxwell::si::constants {
/// Speed of light in vacuum
MODULE_EXPORT constexpr quantity_value speed_of_light =
    299'792'458.0 * meter_unit / second_unit;

/// Planck constant
MODULE_EXPORT constexpr quantity_value planck_constant =
    6.626'070'15e-34 * joule_unit * second_unit;

/// Reduced Planck constant
MODULE_EXPORT constexpr quantity_value reduced_planck_constant =
    planck_constant / (2.0 * std::numbers::pi);
/// Boltzmann constant
MODULE_EXPORT constexpr quantity_value boltzmann_constant =
    1.380'649e-23 * joule_unit / kelvin_unit;

/// Gravitational constant
MODULE_EXPORT constexpr quantity_value gravitational_constant =
    6.67430e-11 * cubic_meter_unit /
    (kilogram_unit * second_unit * second_unit);

MODULE_EXPORT constexpr quantity_value cosmological_constant =
    1.540'596e-10 / square_meter_unit;

MODULE_EXPORT constexpr quantity_value stefan_boltzmann_constant =
    5.670'374e-8 * watt_unit /
    (square_meter_unit * kelvin_unit * kelvin_unit * kelvin_unit * kelvin_unit);

MODULE_EXPORT constexpr quantity_value first_radiation_constant =
    3.741'771'852e-16 * watt_unit * square_meter_unit;

MODULE_EXPORT constexpr quantity_value second_radiation_constant =
    1.438'776'877e-2 * meter_unit * kelvin_unit;

MODULE_EXPORT constexpr quantity_value elementary_charge =
    1.602'176'634e-19 * coulomb_unit;
} // namespace maxwell::si::constants

#endif