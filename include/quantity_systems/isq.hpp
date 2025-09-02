/// \file isq.hpp
/// \brief Definition of quantities in the ISQ system

#ifndef ISQ_HPP
#define ISQ_HPP

#include <type_traits> // true_type

#include "core/quantity.hpp"
#include "core/quantity_system.hpp"
#include "utility/config.hpp"
#include "utility/type_traits.hpp"

namespace maxwell::isq {
MODULE_EXPORT using isq_system =
    quantity_system<"L", "M", "T", "I", "Θ", "N", "J">;

// --- ISQ Base Quantities ---

MODULE_EXPORT constexpr struct length_quantity_type
    : isq_system::base_quantity<"L"> {
} length;

MODULE_EXPORT constexpr struct mass_quantity_type
    : isq_system::base_quantity<"M"> {
} mass;

MODULE_EXPORT constexpr struct time_quantity_type
    : isq_system::base_quantity<"T"> {
} time;

MODULE_EXPORT constexpr struct current_quantity_type
    : isq_system::base_quantity<"I"> {
} current;

MODULE_EXPORT constexpr struct temperature_quantity_type
    : isq_system::base_quantity<"Θ"> {
} temperature;

MODULE_EXPORT constexpr struct amount_quantity_type
    : isq_system::base_quantity<"N"> {
} amount;

MODULE_EXPORT constexpr struct luminosity_quantity_type
    : isq_system::base_quantity<"J"> {
} luminosity;

MODULE_EXPORT constexpr struct dimensionless_quantity_type
    : isq_system::dimensionless_quantity {
} dimensionless;

// --- ISQ Named Derived Quantities ---
MODULE_EXPORT constexpr struct plane_angle_quantity_type
    : make_derived_quantity_t<"Plane Angle", dimensionless> {
} plane_angle;

MODULE_EXPORT constexpr struct solid_angle_quantity_type
    : make_derived_quantity_t<"Solid Angle", dimensionless> {
} solid_angle;

MODULE_EXPORT constexpr struct frequency_quantity_type
    : make_derived_quantity_t<"Frequency", dimensionless / time, false> {
} frequency;

MODULE_EXPORT constexpr struct force_quantity_type
    : make_derived_quantity_t<"Force", mass * length / (time * time), false> {
} force;

MODULE_EXPORT constexpr struct pressure_quantity_type
    : make_derived_quantity_t<"Pressure", force / (length * length), false> {
} pressure;

MODULE_EXPORT constexpr struct work_quantity_type
    : make_derived_quantity_t<"Work", force * length, false> {
} work;

MODULE_EXPORT constexpr struct power_quantity_type
    : make_derived_quantity_t<"Power", work / time, false> {
} power;

MODULE_EXPORT constexpr struct charge_quantity_type
    : make_derived_quantity_t<"Charge", current * time, false> {
} charge;

MODULE_EXPORT constexpr struct potential_quantity_type
    : make_derived_quantity_t<"Potential", power / current, false> {
} potential;

MODULE_EXPORT constexpr struct capacitance_quantity_type
    : make_derived_quantity_t<"Capacitance", charge / potential, false> {
} capacitance;

MODULE_EXPORT constexpr struct resistance_quantity_type
    : make_derived_quantity_t<"Resistance", potential / current, false> {
} resistance;

MODULE_EXPORT constexpr struct conductance_quantity_type
    : make_derived_quantity_t<"Conductance", current / potential, false> {
} conductance;

MODULE_EXPORT constexpr struct magnetic_flux_quantity
    : make_derived_quantity_t<"Magnetic Flux", potential * time, false> {
} magnetic_flux;

// --- Other Derived Quantities ---
MODULE_EXPORT constexpr struct area_quantity_type
    : make_derived_quantity_t<"Area", length * length, false> {
} area;

MODULE_EXPORT constexpr struct volume_quantity_type
    : make_derived_quantity_t<"Volume", area * length, false> {
} volume;

MODULE_EXPORT constexpr struct torque_quantity_type
    : make_derived_quantity_t<"Torque", work> {
} torque;

MODULE_EXPORT constexpr struct velocity_quantity_type
    : make_derived_quantity_t<"Velocity", length / time, false> {
} velocity;

MODULE_EXPORT constexpr struct acceleration_quantity_type
    : make_derived_quantity_t<"Acceleration", velocity / time, false> {
} acceleration;

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
MODULE_EXPORT template <>
struct enable_chrono_conversions<isq::time> : std::true_type {};

MODULE_EXPORT template <>
struct is_angle_like<isq::plane_angle> : std::true_type {};
} // namespace maxwell
#endif