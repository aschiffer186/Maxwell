#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "quantity_repo.hpp"

namespace maxwell::constants {
namespace _detail {
consteval meter_per_second compute_c() noexcept {
  using namespace metric_literals;
  return 299792458.0_m_s;
}

consteval quantity<joule_unit * second_unit> compute_h() noexcept {
  using namespace metric_literals;
  return 6.62607015e-34_J * 1.0_s;
}

consteval coulumb compute_e() noexcept {
  using namespace metric_literals;
  return 1.602176634e-19_C;
}

consteval quantity<joule_unit / kelvin_unit> compute_k() noexcept {
  using namespace metric_literals;
  return 1.380649e-23_J / 1.0_K;
}

} // namespace _detail

constexpr meter_per_second c = 299792458.0 * meter_per_second_unit;
constexpr quantity<joule_unit * second_unit> h = 6.62607015e-34 * joule_unit * second_unit;
constexpr coulumb e = 1.602176634e-19 * coulomb_unit;
constexpr quantity<joule_unit / kelvin_unit> k = 1.380649e-23 * joule_unit / kelvin_unit;
constexpr mole N_a = 6.02214076e23 * mole_unit;
constexpr meter_per_second_per_second g = 9.81 * meter_per_second_per_second_unit;
// constexpr auto h = 6.62607015e-34_J * 1.0_s;
} // namespace maxwell::constants

#endif