import Maxwell;

#include <cmath>
#include <tuple>

using namespace maxwell;

using Mach =
    quantity_value<si::number_unit, sub_quantity<isq::dimensionless, "Mach">{}>;

auto normal_shock(Mach M, quantity_of<isq::temperature> auto T0,
                  quantity_of<isq::pressure> auto p0)
    -> std::tuple<Mach, maxwell::si::kelvin<>, maxwell::si::pascal<>> {
  constexpr maxwell::si::number<> gamma{1.4};

  const Mach M2_num = (gamma - 1) * M * M + 2.0;
  const Mach M2_den = 2.0 * gamma * M * M - (gamma - 1);
  const Mach M2 = std::sqrt(M2_num / M2_den);

  const double p2_num = 2.0 * gamma * M * M - (gamma - 1);
  const double p2_den = gamma + 1.0;
  const maxwell::si::pascal<> p2 = p0 * (p2_num / p2_den);

  const double T2_num = p2_num * ((gamma - 1) * M * M + 2.0);
  const double T2_den = p2_den * (2.0 * gamma * M * M - (gamma - 1));
  const maxwell::si::kelvin<> T2 = T0 * (T2_num / T2_den);

  return {M2, T2, p2};
}

int main() {
  Mach M{2.0};
  maxwell::si::kelvin<> T0{300.0};
  maxwell::si::pascal<> p0{101325.0};

  normal_shock(M, T0, p0);
}