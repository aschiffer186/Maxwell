#include "Maxwell.hpp"
#include "utility/type_traits.hpp"

#include <vector>

int main() {
  using namespace maxwell;
  using namespace maxwell::si;

#ifdef TEST_INVALID_QUANTITY_VALUE_CONSTRUCTOR_1
  [[maybe_unused]] meter<> m{std::vector{1.0, 2.0, 3.0}};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONSTRUCTOR_2
  [[maybe_unused]] meter<> m{std::in_place, "invalid"};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONSTRUCTOR_3
  [[maybe_unused]] meter<> m{
      std::in_place, {1.0, 2.0, 3.0}, std::allocator<double>{}};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CHRONO_CONSTRUCTOR
  [[maybe_unused]] meter<> m{std::chrono::seconds(2)};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_1
  [[maybe_unused]] meter<> m{ampere<>{5.0}};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_2
  [[maybe_unused]] radian<> angle{number<>{std::numbers::pi}};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_3
  using Mach_quantity = sub_quantity<isq::dimensionless, "Mach">;
  using Mach = quantity_value<si::number_unit, Mach_quantity{}>;
  [[maybe_unused]] Mach mach{si::number<>{0.85}};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_4
  const ampere<> a{5.0};
  [[maybe_unused]] const meter<> m{a};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_5
  using Mach_quantity = sub_quantity<isq::dimensionless, "Mach">;
  using Mach = quantity_value<si::number_unit, Mach_quantity{}>;
  const si::number<> n{0.85};
  [[maybe_unused]] Mach mach{n};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_6
  si::number<> n{std::numbers::pi};
  [[maybe_unused]] const si::radian<> angle{n};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_QUANTITY_HOLDER_1
  [[maybe_unused]] si::meter<> m{isq::time_holder<>{si::second_unit, 5.0}};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_QUANTITY_HOLDER_2
  const isq::time_holder<> t{si::second_unit, 300.0};
  [[maybe_unused]] const si::meter<> m{t};
#endif

#ifdef TEST_INVALID_QUANTIY_VALUE_QUANTITY_HOLDER_3
  const isq::dimensionless_holder<> d{si::number_unit, 10.0};
  [[maybe_unused]] const si::radian<> angle{d};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_QUANTITY_HOLDER_4
  constexpr quantity auto wavelength =
      sub_quantity<isq::length, "wavelength">{};
  isq::length_holder<> l{si::meter_unit, 500.0};
  [[maybe_unused]] const quantity_value<si::meter_unit, wavelength, double> wl{
      l};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_OPERATOR_1
  const si::meter<> m{5.0};
  [[maybe_unused]] const double d = m;
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_ASSIGNMENT_1
  si::meter<> m{5.0};
  m = 10.0;
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_ASSIGNMENT_2
  si::meter<> m{5.0};
  m = std::chrono::nanoseconds{100};
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_CONVERSION_OPERATOR_2
  const si::radian<> angle{std::numbers::pi};
  [[maybe_unused]] const double d = angle;
#endif

#ifdef TEST_INVALID_QUANTITY_VALUE_IN_1
  meter<> m{5.0};
  const si::second<> q2 = m.in(si::second_unit);
#endif

  return 0;
}