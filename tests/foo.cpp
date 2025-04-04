#include <Maxwell.hpp>
#include <iostream>
#include <limits>
#include <vector>

struct res_unit : decltype(maxwell::mole_unit * maxwell::ampere_unit) {
  using base_type = decltype(maxwell::mole_unit * maxwell::ampere_unit);

  constexpr res_unit() = default;
  constexpr res_unit(const base_type&) noexcept {}
};

void foo(maxwell::amount auto) {}

int main() {
  // [[maybe_unused]] const maxwell::mole m;

  //   using res = maxwell::basic_quantity<double, res_unit{}>;
  //   [[maybe_unused]] const maxwell::mole m2(maxwell::mole{} /
  //   maxwell::ampere{} *
  //                                           maxell::newton{});
  const auto V_unit = maxwell::newton_unit / (maxwell::meter_unit * maxwell::meter_unit);

  maxwell::quantity<V_unit> V1{3.0};
  maxwell::quantity<maxwell::pascal_unit> V2{3.0};
  // maxwell::basic_quantity<double, maxwell::meter_unit> meter{V2};
  // V1 = V2;
}