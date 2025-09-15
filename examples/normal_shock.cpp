#include "core/quantity.hpp"
#include "quantity_systems/isq.hpp"
#include <Maxwell.hpp>

// using Mach = maxwell::quantity_value<
//     maxwell::si::number_unit,
//     maxwell::make_derived_quantity_t<"Mach", maxwell::isq::dimensionless>{}>;

using namespace maxwell;

using Mach =
    quantity_value<si::number_unit, sub_quantity<isq::dimensionless, "Mach">{}>;

auto normal_shock(Mach M,
                  [[maybe_unused]] quantity_of<isq::temperature> auto T0,
                  [[maybe_unused]] quantity_of<isq::pressure> auto p0)
    -> std::tuple<Mach, maxwell::si::kelvin<>, maxwell::si::pascal<>> {
  constexpr maxwell::si::number<> gamma{1.4};

  const Mach M2_num = (gamma - 1) * M * M + 2.0;
  const Mach M2_den = 2.0 * gamma * M * M - (gamma - 1);
  const Mach M2 = std::sqrt(M2_num / M2_den);
}

int main() {
  // Mach M{2.0};
  // maxwell::si::kelvin<> T0{300.0};
  // maxwell::si::pascal<> p0{101325.0};

  // normal_shock(M, T0, p0);
}