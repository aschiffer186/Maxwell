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

TEST(TestQuantityHolder, TestValueConstructors) {
  length_holder<> l1{5.0, kilo_unit<si::meter_unit>};
  EXPECT_EQ(l1.get_value(), 5.0);
  EXPECT_EQ(l1.get_multiplier(), 1e-3);
  EXPECT_EQ(l1.get_reference(), 0.0);

  length_holder<> l2{5000.0, si::meter_unit};
  EXPECT_EQ(l2.get_value(), 5000.0);
  EXPECT_EQ(l2.get_multiplier(), 1.0);
  EXPECT_EQ(l2.get_reference(), 0.0);

  length_holder<> l3{3.0, 1.0, 0.0};
  EXPECT_EQ(l3.get_value(), 3.0);
  EXPECT_EQ(l3.get_multiplier(), 1.0);
  EXPECT_EQ(l3.get_reference(), 0.0);

  temperature_holder<> t1{100.0, si::celsius_unit};
  EXPECT_EQ(t1.get_value(), 100.0);
  EXPECT_EQ(t1.get_multiplier(), 1.0);
  EXPECT_EQ(t1.get_reference(), -273.15);

  temperature_holder<> t2{373.15, si::kelvin_unit};
  EXPECT_EQ(t2.get_value(), 373.15);
  EXPECT_EQ(t2.get_multiplier(), 1.0);
  EXPECT_EQ(t2.get_reference(), 0.0);
}