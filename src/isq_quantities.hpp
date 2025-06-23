#ifndef ISQ_QUANTITIES_HPP
#define ISQ_QUANTITIES_HPP

#include "isq_units.hpp"
#include "quantity.hpp"

namespace maxwell::isq {
template <typename Magnitude = double>
using meter_type = quantity_type<meter_unit, length_kind, Magnitude>;
using meter = meter_type<>;

template <typename Magnitude = double>
using kilogram_type = quantity_type<kilogram_unit, mass_kind, Magnitude>;
using kilogram = kilogram_type<>;

template <typename Magnitude = double>
using second_type = quantity_type<second_unit, time_kind, Magnitude>;
using second = second_type<>;

template <typename Magnitude = double>
using ampere_type = quantity_type<ampere_unit, current_kind, Magnitude>;
using ampere = ampere_type<>;

template <typename Magnitude = double>
using kelvin_type = quantity_type<kelvin_unit, temperature_kind, Magnitude>;
using kelvin = kelvin_type<>;

template <typename Magnitude = double>
using mole_type = quantity_type<mole_unit, amount_kind, Magnitude>;
using mole = mole_type<>;

template <typename Magnitude = double>
using candela_type = quantity_type<candela_unit, luminosity_kind, Magnitude>;
using candela = candela_type<>;

template <typename Magnitude = double>
using square_meter_type =
    quantity_type<square_meter_unit, area_kind, Magnitude>;
using square_meter = square_meter_type<>;

namespace isq_literals {
consteval meter operator""_m(unsigned long long value) noexcept {
  return meter(value);
}

consteval meter operator""_m(long double value) noexcept {
  return meter(value);
}

consteval kilogram operator""_kg(unsigned long long value) noexcept {
  return kilogram(value);
}

consteval kilogram operator""_kg(long double value) noexcept {
  return kilogram(value);
}

consteval second operator""_s(unsigned long long value) noexcept {
  return second(value);
}

consteval second operator""_s(long double value) noexcept {
  return second(value);
}

consteval ampere operator""_A(unsigned long long value) noexcept {
  return ampere(value);
}

consteval ampere operator""_A(long double value) noexcept {
  return ampere(value);
}

consteval mole operator""_mol(unsigned long long value) noexcept {
  return mole(value);
}

consteval mole operator""_mol(long double value) noexcept {
  return mole(value);
}

consteval candela operator""_cd(unsigned long long value) noexcept {
  return candela(value);
}

consteval candela operator""_cd(long double value) noexcept {
  return candela(value);
}
} // namespace isq_literals
} // namespace maxwell::isq

#endif