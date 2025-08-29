#include "quantity_value.hpp"
#include "si.hpp"
#include <Maxwell.hpp>
#include <iostream>

constexpr struct wavelength_quantity_type
    : maxwell::make_derived_quantity_t<"W", maxwell::isq::length_quantity> {
} wavelength_quantity;

template <auto U>
using wavelength = maxwell::quantity_value<U, wavelength_quantity, double>;

int main() {
  maxwell::si::meter<> q1;

  // maxwell::si::meter<> area = q1 * q1;

  // maxwell::si::newton_meter<> nm =
  //     maxwell::si::newton<>{} * maxwell::si::meter<>{};

  // maxwell::si::newton_meter<> nm2 = maxwell::si::joule<>{};

  // maxwell::kilo<maxwell::si::meter<>> k{maxwell::si::ampere<>{}};

  // std::cout << k << "\n";
  // maxwell::quantity_value<maxwell::si::ampere_unit> a{k};

  // wavelength<maxwell::si::meter_unit> q2{q1};
  // [[maybe_unused]] wavelength<maxwell::si::meter_unit> w{k};
  //   maxwell::si::meter<> q3{q2};
  // wavelength w{1_nm};
  // wavelength w2 = 1_nm;
  using namespace maxwell::si::symbols;
  // constexpr maxwell::si::meter_per_second<> c = 300'000'00 * m / s;
  constexpr maxwell::si::meter<> q = 100.0 * m;

  return 0;
}