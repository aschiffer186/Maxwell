#include "Maxwell.hpp"

#include <gtest/gtest.h>

using namespace maxwell;
using namespace maxwell::si;

TEST(TestUnits, TestUnitConcept) {
  EXPECT_TRUE(unit<meter_unit_type>);
  EXPECT_TRUE(unit<const meter_unit_type>);
  EXPECT_TRUE(unit<meter_unit_type&>);
  EXPECT_TRUE(unit<const meter_unit_type&>);
  EXPECT_TRUE(unit<meter_unit_type&&>);
  EXPECT_TRUE(unit<const meter_unit_type&&>);

  EXPECT_FALSE(unit<int>);
  EXPECT_FALSE(unit<isq::length_quantity_type>);
}

TEST(TestUnits, TestUnitEquality) {
  EXPECT_EQ(meter_unit, meter_unit);
  EXPECT_NE(meter_unit, ampere_unit);
  EXPECT_NE(meter_unit, kilo_unit<meter_unit>);
  EXPECT_EQ(meter_unit, meter_unit_type{});
}

TEST(TestUnits, TestBaseUnits) {
  EXPECT_EQ(meter_unit.base_units(), meter_unit);
  EXPECT_EQ(kilo_unit<meter_unit>.base_units(), meter_unit);
  EXPECT_EQ(degree_unit.base_units(), radian_unit);
}

TEST(TestUnits, TestUnitProduct) {
  const unit auto u1 = meter_unit * meter_unit;

  EXPECT_EQ(u1.quantity, square_meter_unit.quantity);
  EXPECT_EQ(u1.multiplier, 1.0);

  const unit auto u2 = meter_unit * kilo_unit<meter_unit>;
  EXPECT_EQ(u2.quantity, square_meter_unit.quantity);
  EXPECT_EQ(u2.multiplier, 1e-3);

  const unit auto u3 = kilo_unit<meter_unit> * kilo_unit<meter_unit>;
  EXPECT_EQ(u3.quantity, square_meter_unit.quantity);
  EXPECT_EQ(u3.multiplier, 1e-6);

  const unit auto u4 = meter_unit * centi_unit<second_unit>;
  EXPECT_EQ(u4.quantity, isq::length * isq::time);
  EXPECT_EQ(u4.multiplier, 1e2);

  const unit auto u5 = value<180.0 / std::numbers::pi> * radian_unit;
  EXPECT_EQ(u5.quantity, radian_unit.quantity);
  EXPECT_EQ(u5.multiplier, 180.0 / std::numbers::pi);
  EXPECT_EQ(u5.reference, 0.0);
}

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