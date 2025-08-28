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
    : make_derived_quantity_t<"Plane Angle", dimensionless_quantity> {
} plane_angle_quantity;

constexpr struct solid_angle_quantity_type
    : make_derived_quantity_t<"Solid Angle", dimensionless_quantity> {
} solid_angle_quantity;

constexpr struct frequency_quantity_type
    : make_derived_quantity_t<"Frequency",
                              dimensionless_quantity / time_quantity, false> {
} frequency_quantity;

constexpr struct force_quantity_type
    : make_derived_quantity_t<"Force",
                              mass_quantity * length_quantity /
                                  (time_quantity * time_quantity),
                              false> {
} force_quantity;

constexpr struct pressure_quantity_type
    : make_derived_quantity_t<
          "Pressure", force_quantity / (length_quantity * length_quantity),
          false> {
} pressure_quantity;

constexpr struct work_quantity_type
    : make_derived_quantity_t<"Work", force_quantity * length_quantity, false> {
} work_quantity;

constexpr struct power_quantity_type
    : make_derived_quantity_t<"Power", work_quantity / time_quantity, false> {
} power_quantity;

constexpr struct charge_quantity_type
    : make_derived_quantity_t<"Charge", current_quantity * time_quantity,
                              false> {
} charge_quantity;

// --- Other Derived Quantities ---
constexpr struct area_quantity_type
    : make_derived_quantity_t<"Area", length_quantity * length_quantity,
                              false> {
} area_quantity;

constexpr struct volume_quantity_type
    : make_derived_quantity_t<"Volume", area_quantity * length_quantity,
                              false> {
} volume_quantity;

constexpr struct torque_quantity_type
    : make_derived_quantity_t<"Torque", work_quantity> {
} torque_quantity;

// --- Convenience Concepts ---
template <typename Q>
concept length = quantity_convertible_to<Q::quantity_kind, length_quantity>;

template <typename Q>
concept mass = quantity_convertible_to<Q::quantity_kind, mass_quantity>;

template <typename Q>
concept time = quantity_convertible_to<Q::quantity_kind, time_quantity>;

template <typename Q>
concept temperature =
    quantity_convertible_to<Q::quantity_kind, temperature_quantity>;

template <typename Q>
concept amount = quantity_convertible_to<Q::quantity_kind, amount_quantity>;

template <typename Q>
concept luminosity =
    quantity_convertible_to<Q::quantity_kind, luminosity_quantity>;

template <typename Q>
concept pressure = quantity_convertible_to<Q::quantity_kind, pressure_quantity>;

} // namespace maxwell::isq

namespace maxwell {
template <>
struct enable_chrono_conversions<isq::time_quantity> : std::true_type {};
} // namespace maxwell
#endif