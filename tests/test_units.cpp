#include "Maxwell.hpp"
#include "core/unit.hpp"
#include "quantity_systems/isq.hpp"
#include "quantity_systems/si.hpp"

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

  const unit auto u6 = meter_unit * radian_unit;
  EXPECT_EQ(u6.quantity, isq::length);
  EXPECT_EQ(u6.multiplier, 1.0);
  EXPECT_EQ(u6.reference, 0.0);

  const unit auto u7 = meter_unit * degree_unit;
  EXPECT_EQ(u7.quantity, isq::length);
  EXPECT_EQ(u7.multiplier, 180.0 / std::numbers::pi);
  EXPECT_EQ(u7.reference, 0.0);
}

TEST(TestUnits, TestUnitQuotient) {
  const auto u1 = meter_unit / second_unit;

  EXPECT_EQ(u1.quantity, meter_unit.quantity / second_unit.quantity);
  EXPECT_EQ(u1.multiplier, 1.0);
  EXPECT_EQ(u1.reference, 0.0);

  const auto u2 = kilo_unit<meter_unit> / second_unit;
  EXPECT_EQ(u2.quantity, meter_unit.quantity / second_unit.quantity);
  EXPECT_EQ(u2.multiplier, 1e-3);
  EXPECT_EQ(u2.reference, 0.0);

  const auto u3 = meter_unit / kilo_unit<second_unit>;
  EXPECT_EQ(u3.quantity, meter_unit.quantity / second_unit.quantity);
  EXPECT_EQ(u3.multiplier, 1e3);
  EXPECT_EQ(u3.reference, 0.0);

  const auto u4 = meter_unit / meter_unit;
  EXPECT_EQ(u4.quantity.dimensions, dimension_product_type<>{});
  EXPECT_EQ(u4.multiplier, 1.0);
  EXPECT_EQ(u4.reference, 0.0);
  EXPECT_TRUE(unitless<u4>);
}

TEST(TestUnits, TestUnitSqrt) {
  const auto u1 = kilo_unit<square_meter_unit>;
  const auto u2 = sqrt(u1);

  EXPECT_EQ(u2.quantity.dimensions, meter_unit.quantity.dimensions);
  EXPECT_EQ(u2.multiplier, 1e-3);

  const auto u3 = sqrt(degree_unit);
  EXPECT_EQ(u3.quantity.dimensions, radian_unit.quantity.dimensions);
  EXPECT_FLOAT_EQ(u3.multiplier, std::sqrt(180.0 / std::numbers::pi));
  EXPECT_EQ(u3.reference, 0.0);
}

TEST(TestUnits, TestUnitPow) {
  const auto u1 = deca_unit<meter_unit>;
  const auto u2 = pow<4>(u1);

  const auto actual_dimensions = u2.quantity.dimensions;
  const auto expected_dimensions = pow<4>(isq::length).dimensions;

  EXPECT_EQ(actual_dimensions, expected_dimensions);
  EXPECT_FLOAT_EQ(u2.multiplier, 1e-4);

  const auto u3 = pow<-2>(u1);

  const auto actual_dimensions2 = u3.quantity.dimensions;
  const auto expected_dimensions2 = pow<-2>(isq::length).dimensions;
  EXPECT_EQ(actual_dimensions2, expected_dimensions2);
  EXPECT_FLOAT_EQ(u3.multiplier, 1e2);

  const auto u4 = kilo_unit<square_meter_unit>;
  const auto u5 = pow<rational<1, 2>>(u4);

  EXPECT_EQ(u5.quantity.dimensions, meter_unit.quantity.dimensions);
  EXPECT_FLOAT_EQ(u5.multiplier, 1e-3);
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

  factor = conversion_factor(meter_unit / second_unit,
                             kilo_unit<meter_unit> / other::time::hour_unit);
  EXPECT_FLOAT_EQ(factor, 3.6);

  factor = conversion_factor(sqrt(radian_unit), sqrt(degree_unit));
  EXPECT_FLOAT_EQ(factor, std::sqrt(180.0 / std::numbers::pi));
}

TEST(TestUnits, TestUnitConversionOffset) {
  double offset = conversion_offset(si::celsius_unit, si::kelvin_unit);
  EXPECT_FLOAT_EQ(offset, 273.15);

  offset = conversion_offset(si::kelvin_unit, si::celsius_unit);
  EXPECT_FLOAT_EQ(offset, -273.15);

  offset = conversion_offset(si::celsius_unit, us::fahrenheit_unit);
  EXPECT_FLOAT_EQ(offset, 32.0);

  offset = conversion_offset(us::fahrenheit_unit, si::celsius_unit);
  EXPECT_FLOAT_EQ(offset, -32.0 / 1.8);
}

TEST(TestUnits, TestUnitless) {
  EXPECT_FALSE(unitless<si::meter_unit>);
  EXPECT_TRUE(unitless<si::number_unit>);
  EXPECT_TRUE(unitless<si::meter_unit / si::meter_unit>);
  EXPECT_FALSE(unitless<si::radian_unit>);
  EXPECT_TRUE(unitless<si::radian_unit / si::radian_unit>);
}

TEST(TestUnits, TestUnitAddableSubtractableWith) {
  EXPECT_TRUE((unit_addable_with<meter_unit, meter_unit>));
  EXPECT_TRUE((unit_addable_with<kilo_unit<meter_unit>, meter_unit>));
  EXPECT_TRUE((unit_addable_with<meter_unit, kilo_unit<meter_unit>>));
  EXPECT_FALSE((unit_addable_with<meter_unit, second_unit>));
  EXPECT_FALSE((unit_addable_with<kelvin_unit, celsius_unit>));
  EXPECT_FALSE((unit_addable_with<radian_unit, number_unit>));

  EXPECT_TRUE((unit_subtractable_from<meter_unit, meter_unit>));
  EXPECT_TRUE((unit_subtractable_from<kilo_unit<meter_unit>, meter_unit>));
  EXPECT_TRUE((unit_subtractable_from<meter_unit, kilo_unit<meter_unit>>));
  EXPECT_FALSE((unit_subtractable_from<meter_unit, second_unit>));
  EXPECT_FALSE((unit_subtractable_from<kelvin_unit, celsius_unit>));
  EXPECT_FALSE((unit_subtractable_from<radian_unit, number_unit>));
}