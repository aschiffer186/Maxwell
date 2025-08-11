/// \file isq.hpp
/// \brief Definition of quantities in the ISQ system

#ifndef ISQ_HPP
#define ISQ_HPP

#include <type_traits> // true_type

#include "quantity.hpp"
#include "quantity_system.hpp"
#include "type_traits.hpp"

namespace maxwell::isq {
using isq_system = quantity_system<"L", "M", "T", "I", "Θ", "N", "J">;

constexpr auto length_quantity = isq_system::base_quantity<"L", "L">{};
constexpr auto mass_quantity = isq_system::base_quantity<"M", "M">{};
constexpr auto time_quantity = isq_system::base_quantity<"T", "T">{};
constexpr auto current_quantity = isq_system::base_quantity<"I", "I">{};
constexpr auto temperature_quantity = isq_system::base_quantity<"Θ", "Θ">{};
constexpr auto amount_quantity = isq_system::base_quantity<"N", "N">{};
constexpr auto luminosity_quantity = isq_system::base_quantity<"J", "J">{};

} // namespace maxwell::isq

namespace maxwell {
template <>
struct enable_chrono_conversions<isq::time_quantity> : std::true_type {};
} // namespace maxwell
#endif