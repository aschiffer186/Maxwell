#include "Maxwell.hpp"

using namespace maxwell;
using namespace maxwell::isq::isq_literals;

struct wavelength_kind : make_derived_kind_t<"Wavelength", isq::length_kind> {};

using wavelength = maxwell::quantity_type<isq::meter_unit, wavelength_kind>;

int main() {
  //   isq::square_meter<double> sm(isq::meter<double>(1));
  wavelength w(1.0);
  wavelength w2 = wavelength::from(1.0_m);
  wavelength w3 = 1.0_m .as<wavelength>();

  return 0;
}