#include "core/quantity.hpp"
#include "quantity_systems/si.hpp"
#include <Maxwell.hpp>

#include <gtest/gtest.h>
#include <string>

using namespace maxwell;
using namespace maxwell::si;
using namespace maxwell::isq;

TEST(TestSIUnits, TestMeterUnit) {
  EXPECT_TRUE((quantity_convertible_to<meter_unit.quantity, isq::length>));
  EXPECT_EQ(meter_unit.multiplier, 1.0);
  EXPECT_EQ(meter_unit.reference, 0.0);

  const std::string rep = std::format("{}", meter_unit);
  EXPECT_STREQ(rep.c_str(), "m");
}

TEST(TestSIUnits, TestKilometerUnit) {
  EXPECT_TRUE((quantity_convertible_to<kilometer_unit.quantity, isq::length>));
  EXPECT_EQ(kilometer_unit.multiplier, 1e-3);
  EXPECT_EQ(kilometer_unit.reference, 0.0);

  const std::string rep = std::format("{}", kilometer_unit);
  EXPECT_STREQ(rep.c_str(), "km");
}

TEST(TestSIUnits, TestCentimeterUnit) {
  EXPECT_TRUE((quantity_convertible_to<centimeter_unit.quantity, isq::length>));
  EXPECT_EQ(centimeter_unit.multiplier, 1e2);
  EXPECT_EQ(centimeter_unit.reference, 0.0);

  const std::string rep = std::format("{}", centimeter_unit);
  EXPECT_STREQ(rep.c_str(), "cm");
}

TEST(TestSIUnits, TestMilimeterUnit) {
  EXPECT_TRUE((quantity_convertible_to<millimeter_unit.quantity, length>));
  EXPECT_EQ(millimeter_unit.multiplier, 1e3);
  EXPECT_EQ(centimeter_unit.reference, 0.0);

  const std::string rep = std::format("{}", millimeter_unit);
  EXPECT_STREQ(rep.c_str(), "mm");
}

TEST(TestSIUnits, TestGramUnit) {
  EXPECT_TRUE((quantity_convertible_to<gram_unit.quantity, mass>));
  EXPECT_EQ(gram_unit.multiplier, 1.0);
  EXPECT_EQ(gram_unit.reference, 0.0);

  const std::string rep = std::format("{}", gram_unit);
  EXPECT_STREQ(rep.c_str(), "g");
}

TEST(TestSIUnits, TestKilogramUnit) {
  EXPECT_TRUE((quantity_convertible_to<kilogram_unit.quantity, mass>));
  EXPECT_EQ(kilogram_unit.multiplier, 1e-3);
  EXPECT_EQ(kilogram_unit.reference, 0.0);

  const std::string rep = std::format("{}", kilogram_unit);
  EXPECT_STREQ(rep.c_str(), "kg");
}