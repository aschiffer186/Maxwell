#include <Maxwell.hpp>

using Mach = maxwell::quantity_value<
    maxwell::si::number_unit,
    maxwell::make_derived_quantity_t<"Mach", maxwell::isq::dimensionless>{}>;

using namespace maxwell;

auto normal_shock(Mach M, quantity_of<isq::temperature> auto T0,
                  quantity_of<isq::pressure> auto p0)
    -> std::tuple<Mach, maxwell::si::kelvin<>, maxwell::si::pascal<>> {
  constexpr maxwell::si::number<> gamma{1.4};

  // const Mach M1_num = (gamma - 1) * M * M;
}

int main() {
  Mach M{2.0};
  maxwell::si::kelvin<> T0{300.0};
  maxwell::si::pascal<> p0{101325.0};

  normal_shock(M, T0, p0);
}