/// \file isq.hpp
/// \brief Definition of International System of Quantities (ISQ) dimensions and
/// units.
#ifndef ISQ_HPP
#define ISQ_HPP

#include <concepts>

#include "kind.hpp"
#include "unit.hpp"
#include "unit_system.hpp"

namespace maxwell::isq {
using isq_system = unit_system<"L", "M", "T", "I", "Θ", "N", "J">;

struct length_kind : isq_system::base_kind<"L"> {};
struct mass_kind : isq_system::base_kind<"M"> {};
struct time_kind : isq_system::base_kind<"T"> {};
struct current_kind : isq_system::base_kind<"I"> {};
struct temperature_kind : isq_system::base_kind<"Θ"> {};
struct amount_kind : isq_system::base_kind<"N"> {};
struct luminosity_kind : isq_system::base_kind<"J"> {};

constexpr struct meter_unit_type : isq_system::base_unit<"L", "m"> {
} meter_unit;

constexpr struct kilogram_unit_type : isq_system::base_unit<"M", "kg"> {
} kilogram_unit;

constexpr struct second_unit_type : isq_system::base_unit<"T", "s"> {
} second_unit;

constexpr struct ampere_unit_type : isq_system::base_unit<"L", "A"> {
} ampere_unit;

constexpr struct kelvin_unit_type : isq_system::base_unit<"Θ", "K"> {
} kelvin_unit;

constexpr struct mole_unit_type : isq_system::base_unit<"N", "mol"> {
} mole_unit;

constexpr struct candela_unit_type : isq_system::base_unit<"J", "cd"> {
} candela_unit;

constexpr struct one_unit_type : isq_system::one_unit<"[]"> {
} one_unit;

// SI derived units with special names
constexpr struct radian_unit_type
    : make_derived_unit_t<meter_unit / meter_unit, "rad"> {
} radian_unit;
constexpr struct steradian_unit_type
    : make_derived_unit_t<meter_unit / meter_unit, "sr"> {
} steradin_unit;
constexpr struct hertz_unit_type
    : make_derived_unit_t<one_unit / second_unit, "Hz"> {
} hertz_unit;
constexpr struct newton_unit_type
    : make_derived_unit_t<
          kilogram_unit * meter_unit / second_unit / second_unit, "N"> {
} newton_unit;
constexpr struct pascal_unit_type
    : make_derived_unit_t<newton_unit / meter_unit / meter_unit, "Pa"> {
} pascal_unit;
constexpr struct joule_unit_type
    : make_derived_unit_t<newton_unit * meter_unit, "J"> {
} joule_unit;
constexpr struct watt_unit_type
    : make_derived_unit_t<joule_unit / second_unit, "W"> {
} watt_unit;
constexpr struct coulomb_unit_type
    : make_derived_unit_t<ampere_unit * second_unit, "C"> {
} coulomb_unit;
constexpr struct volt_unit_type
    : make_derived_unit_t<watt_unit / ampere_unit, "V"> {
} volt_unit;
constexpr struct farad_unit_type
    : make_derived_unit_t<coulomb_unit / volt_unit, "F"> {
} farad_unit;
constexpr struct ohm_unit_type
    : make_derived_unit_t<volt_unit / ampere_unit, "Ω"> {
} ohm_unit;
constexpr struct siemens_unit_type
    : make_derived_unit_t<ampere_unit / volt_unit, "S"> {
} siemens_unit;
constexpr struct weber_unit_type
    : make_derived_unit_t<volt_unit * second_unit, "Wb"> {
} weber_unit;
constexpr struct tesla_unit_type
    : make_derived_unit_t<weber_unit / meter_unit / meter_unit, "T"> {
} tesla_unit;
constexpr struct henry_unit_type
    : make_derived_unit_t<weber_unit / ampere_unit, "H"> {
} henry_unit;
constexpr struct lumen_unit_type
    : make_derived_unit_t<candela_unit * steradin_unit, "lm"> {
} lumen_unit;
constexpr struct lux_unit_type
    : make_derived_unit_t<lumen_unit / meter_unit / meter_unit, "lx"> {
} lux_unit;
constexpr struct becquerel_unit_type
    : make_derived_unit_t<one_unit / second_unit, "Bq"> {
} becquerel_unit;
constexpr struct gray_unit_type
    : make_derived_unit_t<joule_unit / kilogram_unit, "Gy"> {
} gray_unit;
constexpr struct sievert_unit_type : make_derived_unit_t<gray_unit, "Sv"> {
} sievert_unit;
constexpr struct katal_unit_type
    : make_derived_unit_t<mole_unit / second_unit, "kat"> {
} katal_unit;
constexpr struct square_meter_unit_type
    : make_derived_unit_t<meter_unit * meter_unit, "m^2"> {
} square_meter_unit;

using area_kind = kind_product_t<length_kind, length_kind>;

// Concepts
template <typename Q>
concept length = std::derived_from<typename Q::kind, length_kind>;

template <typename Q>
concept mass = std::derived_from<typename Q::kind, mass_kind>;

template <typename Q>
concept time = std::derived_from<typename Q::kind, time_kind>;

template <typename Q>
concept current = std::derived_from<typename Q::kind, current_kind>;

template <typename Q>
concept temperature = std::derived_from<typename Q::kind, temperature_kind>;

template <typename Q>
concept amount = std::derived_from<typename Q::kind, amount_kind>;

template <typename Q>
concept luminosity = std::derived_from<typename Q::kind, luminosity_kind>;

template <typename Q>
concept area = std::derived_from<typename Q::kind, area_kind>;
} // namespace maxwell::isq

#endif