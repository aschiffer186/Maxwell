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

// constexpr auto length_quantity = isq_system::base_quantity<"L", "L">{};
// constexpr auto mass_quantity = isq_system::base_quantity<"M", "M">{};
// constexpr auto time_quantity = isq_system::base_quantity<"T", "T">{};
// constexpr auto current_quantity = isq_system::base_quantity<"I", "I">{};
// constexpr auto temperature_quantity = isq_system::base_quantity<"Θ", "Θ">{};
// constexpr auto amount_quantity = isq_system::base_quantity<"N", "N">{};
// constexpr auto luminosity_quantity = isq_system::base_quantity<"J", "J">{};
// constexpr auto dimensoinless_quantity = isq_system::dimensionless_quantity{};

// --- ISQ Base Quantities ---

constexpr struct length_quantity_type : isq_system::base_quantity<"L"> {
} length_quantity;

constexpr struct mass_quantity_type : isq_system::base_quantity<"M"> {
} mass_quantity;

constexpr struct time_quantity_type : isq_system::base_quantity<"T"> {
} time_quantity;

constexpr struct current_quantity_type : isq_system::base_quantity<"I"> {
} current_quantity;

constexpr struct temperature_quantity_type : isq_system::base_quantity<"Θ"> {
} temperature_quantity;

constexpr struct amount_quantity_type : isq_system::base_quantity<"N"> {
} amount_quantity;

constexpr struct luminosity_quantity_type : isq_system::base_quantity<"J"> {
} luminosity_quantity;

constexpr struct dimensionless_quantity_type
    : isq_system::dimensionless_quantity {
} dimensionless_quantity;

// --- ISQ Named Derived Quantities ---
constexpr struct plane_angle_quantity_type
    : make_derived_quantity_t<"P", dimensionless_quantity> {
} plane_angle_quantity;

constexpr struct solid_angle_quantity_type
    : make_derived_quantity_t<"S", dimensionless_quantity> {
} solid_angle_quantity;

constexpr struct frequency_quantity_type
    : make_derived_quantity_t<"Freq", dimensionless_quantity / time_quantity> {
} frequency_quantity;

constexpr struct force_quantity_type
    : make_derived_quantity_t<"Force", mass_quantity * length_quantity /
                                           (time_quantity * time_quantity)> {
} force_quantity;

} // namespace maxwell::isq

namespace maxwell {
template <>
struct enable_chrono_conversions<isq::time_quantity> : std::true_type {};
} // namespace maxwell
#endif