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
/// Type alias used to set up the ISQ system of quantities.
MODULE_EXPORT using isq_system =
    quantity_system<"L", "M", "T", "I", "Θ", "N", "J">;

// --- ISQ Base Quantities ---

/// Quantity representing ISQ quantity of length.
MODULE_EXPORT constexpr struct length_quantity_type
    : isq_system::base_quantity<"L"> {
} length;

/// Quantity representing ISQ quantity of mass.
MODULE_EXPORT constexpr struct mass_quantity_type
    : isq_system::base_quantity<"M"> {
} mass;

/// Quantity representing ISQ quantity of time.
MODULE_EXPORT constexpr struct time_quantity_type
    : isq_system::base_quantity<"T"> {
} time;

/// Quantity representing ISQ quantity of electrical crrent.
MODULE_EXPORT constexpr struct current_quantity_type
    : isq_system::base_quantity<"I"> {
} current;

/// Quantity representing ISQ quantity of temperature.
MODULE_EXPORT constexpr struct temperature_quantity_type
    : isq_system::base_quantity<"Θ"> {
} temperature;

/// Quantity representing ISQ quantity of amount of substance.
MODULE_EXPORT constexpr struct amount_quantity_type
    : isq_system::base_quantity<"N"> {
} amount;

/// Quantity representing ISQ quantity of luminous intensity.
MODULE_EXPORT constexpr struct luminosity_quantity_type
    : isq_system::base_quantity<"J"> {
} luminosity;

/// Quantity representing ISQ quantity of number/dimensionless.
MODULE_EXPORT constexpr struct dimensionless_quantity_type
    : isq_system::dimensionless_quantity {
} dimensionless;

// --- ISQ Named Derived Quantities ---

/// Quantity representing quantity of plane angle.
MODULE_EXPORT constexpr struct plane_angle_quantity_type
    : sub_quantity<dimensionless, "Plane Angle"> {
} plane_angle;

/// Quantity representing quantity of solid angle.
MODULE_EXPORT constexpr struct solid_angle_quantity_type
    : sub_quantity<dimensionless, "Solid Angle"> {
} solid_angle;

/// Quantity representing quantity of frequency.
MODULE_EXPORT constexpr struct frequency_quantity_type
    : derived_quantity<dimensionless / time, "Frequency"> {
} frequency;

/// Quantity representing quantity of force.
MODULE_EXPORT constexpr struct force_quantity_type
    : derived_quantity<mass * length / (time * time), "Force"> {
} force;

/// Quantity representing quantity of pressure.
MODULE_EXPORT constexpr struct pressure_quantity_type
    : derived_quantity<force / (length * length), "Pressure"> {
} pressure;

namespace _detail {
constexpr struct work_base_quantity_type
    : derived_quantity<force * length, "W"> {
} work_base;
} // namespace _detail

/// Quantity represeting quantity of work.
MODULE_EXPORT constexpr struct work_quantity_type
    : sub_quantity<_detail::work_base, "Work"> {
} work;

/// Quantity representing quantity of power.
MODULE_EXPORT constexpr struct power_quantity_type
    : derived_quantity<work / time, "Power"> {
} power;

/// Quantity representing quantity of electrical charge.
MODULE_EXPORT constexpr struct charge_quantity_type
    : derived_quantity<current * time, "Charge"> {
} charge;

/// Quantity representing quantity of electrical power.
MODULE_EXPORT constexpr struct potential_quantity_type
    : derived_quantity<power / current, "Potential"> {
} potential;

MODULE_EXPORT constexpr struct capacitance_quantity_type
    : derived_quantity<charge / potential, "Capacitance"> {
} capacitance;

MODULE_EXPORT constexpr struct resistance_quantity_type
    : derived_quantity<potential / current, "Resistance"> {
} resistance;

MODULE_EXPORT constexpr struct conductance_quantity_type
    : derived_quantity<current / potential, "Conductance"> {
} conductance;

MODULE_EXPORT constexpr struct magnetic_flux_quantity
    : derived_quantity<time * potential, "Magnetic Flux"> {
} magnetic_flux;

// --- Other Derived Quantities ---
MODULE_EXPORT constexpr struct area_quantity_type
    : derived_quantity<length * length, "Area"> {
} area;

MODULE_EXPORT constexpr struct volume_quantity_type
    : derived_quantity<area * length, "Volume"> {
} volume;

MODULE_EXPORT constexpr struct torque_quantity_type
    : sub_quantity<_detail::work_base, "Torque"> {
} torque;

MODULE_EXPORT constexpr struct velocity_quantity_type
    : derived_quantity<length / time, "Velocity"> {
} velocity;

MODULE_EXPORT constexpr struct acceleration_quantity_type
    : derived_quantity<velocity / time, "Acceleration"> {
} acceleration;
} // namespace maxwell::isq

namespace maxwell {
MODULE_EXPORT template <>
struct enable_chrono_conversions<isq::time> : std::true_type {};

MODULE_EXPORT template <>
struct is_angle_like<isq::plane_angle> : std::true_type {};
} // namespace maxwell
#endif