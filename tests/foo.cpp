#include "Maxwell.hpp"

using namespace maxwell;

struct wavelength_kind : make_derived_kind_t<"Wavelength", isq::length_kind> {};

using wavelength = maxwell::quantity_type<isq::meter_unit, wavelength_kind>;

int main() {
  //   isq::square_meter<double> sm(isq::meter<double>(1));
  return 0;
}