#include "Maxwell.hpp"

#include <gtest/gtest.h>

using namespace maxwell;
using namespace maxwell::si;

TEST(TestUnits, TestUnitConversionFactor) {
  double factor = conversion_factor(meter_unit, kilo_unit<meter_unit>);
  EXPECT_FLOAT_EQ(factor, 1e-3);

  factor = conversion_factor(kilo_unit<meter_unit>, meter_unit);
  EXPECT_FLOAT_EQ(factor, 1e3);

  factor = conversion_factor(square_meter_unit, centi_unit<square_meter_unit>);
  EXPECT_FLOAT_EQ(factor, 1e4);

  factor = conversion_factor(centi_unit<square_meter_unit>, square_meter_unit);
  EXPECT_FLOAT_EQ(factor, 1e-4);

  factor = conversion_factor(radian_unit, degree_unit);
  EXPECT_FLOAT_EQ(factor, 180.0 / std::numbers::pi);

  factor = conversion_factor(degree_unit, radian_unit);
  EXPECT_FLOAT_EQ(factor, std::numbers::pi / 180.0);
}