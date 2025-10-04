#include "Maxwell.hpp"

#include <gtest/gtest.h>
#include <type_traits>

#include "test_types.hpp"

using namespace maxwell;
using namespace maxwell::isq;

TEST(TestQuantityHolder, TestUnitConstructor) {
  length_holder<> l{kilo_unit<si::meter_unit>};
  EXPECT_EQ(l.get_value(), 0.0);
  EXPECT_EQ(l.get_multiplier(), 1e-3);
  EXPECT_EQ(l.get_reference(), 0.0);

  temperature_holder<> t{si::celsius_unit};
  EXPECT_EQ(t.get_value(), 0.0);
  EXPECT_EQ(t.get_multiplier(), 1.0);
  EXPECT_EQ(t.get_reference(), -273.15);

  EXPECT_TRUE(
      (std::is_nothrow_constructible_v<length_holder<>, si::meter_unit_type>));
  EXPECT_FALSE((std::is_nothrow_constructible_v<length_holder<throwing_tattle>,
                                                si::meter_unit_type>));
}