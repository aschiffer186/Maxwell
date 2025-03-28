#include "Maxwell.hpp"
#include "internal/quantity.hpp"
#include "internal/unit.hpp"
#include <vector>

struct res_unit : decltype(maxwell::mole_unit * maxwell::ampere_unit) {
  using base_type = decltype(maxwell::mole_unit * maxwell::ampere_unit);

  constexpr res_unit() = default;
  constexpr res_unit(const base_type&) noexcept {}
};

void foo(maxwell::amount auto) {}

int main() {
  [[maybe_unused]] const maxwell::mole m;

  //   using res = maxwell::basic_quantity<double, res_unit{}>;
  //   [[maybe_unused]] const maxwell::mole m2(maxwell::mole{} /
  //   maxwell::ampere{} *
  //                                           maxell::newton{});
}