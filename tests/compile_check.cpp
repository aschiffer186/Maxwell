#include "isq.hpp"
#include "quantity.hpp"
#include "quantity_holder.hpp"
#include "quantity_value.hpp"
#include "si.hpp"

constexpr struct wavelength_quantity_type
    : maxwell::make_derived_quantity_t<"W", maxwell::isq::length_quantity> {
} wavelength_quantity;

template <auto U>
using wavelength = maxwell::quantity_value<wavelength_quantity, U, double>;

int main() {
  maxwell::si::meter<> q1;
  wavelength<maxwell::si::meter_unit> q2{q1};
  maxwell::quantity_holder<wavelength_quantity> w;
  //   maxwell::si::meter<> q3{q2};
  // wavelength w{1_nm};
  // wavelength w2 = 1_nm;
  return 0;
}