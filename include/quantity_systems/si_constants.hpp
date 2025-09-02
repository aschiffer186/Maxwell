#ifndef SI_CONSTANTS_HPP
#define SI_CONSTANTS_HPP

#include "si.hpp"
#include "utility/config.hpp"

namespace maxwell::si::constants {
MODULE_EXPORT constexpr meter_per_second<> c =
    299'792'458.0 * meter_unit / second_unit;
}

#endif