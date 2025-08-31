/// \file isq.hpp
/// \brief Definition of quantities in the ISQ system

#ifndef ISQ_HPP
#define ISQ_HPP

#include <type_traits> // true_type

#include "core/quantity.hpp"
#include "core/quantity_system.hpp"
#include "utility/type_traits.hpp"

namespace maxwell::isq {
using isq_system = quantity_system<"L", "M", "T", "I", "Θ", "N", "J">;

// --- ISQ Base Quantities ---

constexpr struct length_quantity_type : isq_system::base_quantity<"L"> {
} length;

constexpr struct mass_quantity_type : isq_system::base_quantity<"M"> {
} mass;

constexpr struct time_quantity_type : isq_system::base_quantity<"T"> {
} time;

constexpr struct current_quantity_type : isq_system::base_quantity<"I"> {
} current;

constexpr struct temperature_quantity_type : isq_system::base_quantity<"Θ"> {
} temperature;

constexpr struct amount_quantity_type : isq_system::base_quantity<"N"> {
} amount;

constexpr struct luminosity_quantity_type : isq_system::base_quantity<"J"> {
} luminosity;

constexpr struct dimensionless_quantity_type
    : isq_system::dimensionless_quantity {
} dimensionless;

// --- ISQ Named Derived Quantities ---
constexpr struct plane_angle_quantity_type
    : make_derived_quantity_t<"Plane Angle", dimensionless> {
} plane_angle;

constexpr struct solid_angle_quantity_type
    : make_derived_quantity_t<"Solid Angle", dimensionless> {
} solid_angle;

constexpr struct frequency_quantity_type
    : make_derived_quantity_t<"Frequency", dimensionless / time, false> {
} frequency;

constexpr struct force_quantity_type
    : make_derived_quantity_t<"Force", mass * length / (time * time), false> {
} force;

constexpr struct pressure_quantity_type
    : make_derived_quantity_t<"Pressure", force / (length * length), false> {
} pressure;

constexpr struct work_quantity_type
    : make_derived_quantity_t<"Work", force * length, false> {
} work;

constexpr struct power_quantity_type
    : make_derived_quantity_t<"Power", work / time, false> {
} power;

constexpr struct charge_quantity_type
    : make_derived_quantity_t<"Charge", current * time, false> {
} charge;

constexpr struct potential_quantity_type
    : make_derived_quantity_t<"Potential", power / current, false> {
} potential;

constexpr struct capacitance_quantity_type
    : make_derived_quantity_t<"Capacitance", charge / potential, false> {
} capacitance;

constexpr struct resistance_quantity_type
    : make_derived_quantity_t<"Resistance", potential / current, false> {
} resistance;

constexpr struct conductance_quantity_type
    : make_derived_quantity_t<"Conductance", current / potential, false> {
} conductance;

constexpr struct magnetic_flux_quantity
    : make_derived_quantity_t<"Magnetic Flux", potential * time, false> {
} magnetic_flux;

// --- Other Derived Quantities ---
constexpr struct area_quantity_type
    : make_derived_quantity_t<"Area", length * length, false> {
} area;

constexpr struct volume_quantity_type
    : make_derived_quantity_t<"Volume", area * length, false> {
} volume;

constexpr struct torque_quantity_type
    : make_derived_quantity_t<"Torque", work> {
} torque;

constexpr struct velocity_quantity_type
    : make_derived_quantity_t<"Velocity", length / time, false> {
} velocity;

// --- Convenience Concepts ---
// template <typename Q>
// concept length = quantity_convertible_to<Q::quantity_kind, length_quantity>;

// template <typename Q>
// concept mass = quantity_convertible_to<Q::quantity_kind, mass_quantity>;

// template <typename Q>
// concept time = quantity_convertible_to<Q::quantity_kind, time_quantity>;

// template <typename Q>
// concept temperature =
//     quantity_convertible_to<Q::quantity_kind, temperature_quantity>;

// template <typename Q>
// concept amount = quantity_convertible_to<Q::quantity_kind, amount_quantity>;

// template <typename Q>
// concept luminosity =
//     quantity_convertible_to<Q::quantity_kind, luminosity_quantity>;

// template <typename Q>
// concept pressure = quantity_convertible_to<Q::quantity_kind,
// pressure_quantity>;

} // namespace maxwell::isq

namespace maxwell {
template <> struct enable_chrono_conversions<isq::time> : std::true_type {};

template <> struct is_angle_like<isq::plane_angle> : std::true_type {};
} // namespace maxwell
#endif