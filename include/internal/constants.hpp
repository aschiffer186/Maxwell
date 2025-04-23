/**
 * @file constants.hpp
 * @author Alex Schiffer
 * @brief Definition of commonly used scientific constants
 * @version 1.0
 * @date 2025-04-15
 *
 * @copyright Copyright (c) 2025
 */
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "quantity_repo.hpp"

namespace maxwell::constants {
// Note: not using quantity literals to avoid using statement in header file

constexpr meter_per_second c = 299792458.0 * meter_per_second_unit;
constexpr quantity h = 6.62607015e-34 * joule_unit * second_unit;
constexpr coulumb e = 1.602176634e-19 * coulomb_unit;
constexpr quantity k = 1.380649e-23 * joule_unit / kelvin_unit;
constexpr mole N_a = 6.02214076e23 * mole_unit;
constexpr meter_per_second_per_second g = 9.81 * meter_per_second_per_second_unit;
// constexpr auto h = 6.62607015e-34_J * 1.0_s;
} // namespace maxwell::constants

#endif