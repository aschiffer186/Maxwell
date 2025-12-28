#include "core/quantity.hpp"
#include "core/unit.hpp"
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
  EXPECT_EQ(gram_unit.multiplier, 1e3);
  EXPECT_EQ(gram_unit.reference, 0.0);

  const std::string rep = std::format("{}", gram_unit);
  EXPECT_STREQ(rep.c_str(), "g");

  const auto u = milli_unit<gram_unit>;
  EXPECT_EQ(u.multiplier, 1e6);
  EXPECT_EQ(u.reference, 0.0);
}

TEST(TestSIUnits, TestKilogramUnit) {
  EXPECT_TRUE((quantity_convertible_to<kilogram_unit.quantity, mass>));
  EXPECT_EQ(kilogram_unit.multiplier, 1.0);
  EXPECT_EQ(kilogram_unit.reference, 0.0);

  const std::string rep = std::format("{}", kilogram_unit);
  EXPECT_STREQ(rep.c_str(), "kg");
}

TEST(TestSIUnits, TestSecondUnit) {
  EXPECT_TRUE((quantity_convertible_to<second_unit.quantity, isq::time>));
  EXPECT_EQ(second_unit.multiplier, 1.0);
  EXPECT_EQ(second_unit.reference, 0.0);

  const std::string rep = std::format("{}", second_unit);
  EXPECT_STREQ(rep.c_str(), "s");
}

TEST(TestSIUnits, TestAmpereUnit) {
  EXPECT_TRUE((quantity_convertible_to<ampere_unit.quantity, isq::current>));
  EXPECT_EQ(ampere_unit.multiplier, 1.0);
  EXPECT_EQ(ampere_unit.reference, 0.0);

  const std::string rep = std::format("{}", ampere_unit);
  EXPECT_STREQ(rep.c_str(), "A");
}

TEST(TestSIUnits, TestKelvinUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<kelvin_unit.quantity, isq::temperature>));
  EXPECT_EQ(kelvin_unit.multiplier, 1.0);
  EXPECT_EQ(kelvin_unit.reference, 0.0);

  const std::string rep = std::format("{}", kelvin_unit);
  EXPECT_STREQ(rep.c_str(), "K");
}

TEST(TestSIUnits, TestMoleUnit) {
  EXPECT_TRUE((quantity_convertible_to<mole_unit.quantity, isq::amount>));
  EXPECT_EQ(mole_unit.multiplier, 1.0);
  EXPECT_EQ(mole_unit.reference, 0.0);

  const std::string rep = std::format("{}", mole_unit);
  EXPECT_STREQ(rep.c_str(), "mol");
}

TEST(TestSIUnits, TestCandelaUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<candela_unit.quantity, isq::luminosity>));
  EXPECT_EQ(candela_unit.multiplier, 1.0);
  EXPECT_EQ(candela_unit.reference, 0.0);

  const std::string rep = std::format("{}", candela_unit);
  EXPECT_STREQ(rep.c_str(), "cd");
}

TEST(TestSIUnits, TestNumberUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<number_unit.quantity, isq::dimensionless>));
  EXPECT_EQ(number_unit.multiplier, 1.0);
  EXPECT_EQ(number_unit.reference, 0.0);

  const std::string rep = std::format("{}", number_unit);
  EXPECT_STREQ(rep.c_str(), "[]");
}

TEST(TestSIUnits, TestRadianUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<radian_unit.quantity, isq::plane_angle>));
  EXPECT_EQ(radian_unit.multiplier, 1.0);
  EXPECT_EQ(radian_unit.reference, 0.0);

  const std::string rep = std::format("{}", radian_unit);
  EXPECT_STREQ(rep.c_str(), "rad");
}

TEST(TestSIUnits, TestCelsiusUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<celsius_unit.quantity, isq::temperature>));
  EXPECT_EQ(celsius_unit.multiplier, 1.0);
  EXPECT_EQ(celsius_unit.reference, -273.15);

  const std::string rep = std::format("{}", celsius_unit);
  EXPECT_STREQ(rep.c_str(), "°C");
}

TEST(TestSIUnits, TestDegreeUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<degree_unit.quantity, isq::plane_angle>));
  EXPECT_EQ(degree_unit.multiplier, 180.0 / std::numbers::pi);
  EXPECT_EQ(degree_unit.reference, 0.0);

  const std::string rep = std::format("{}", degree_unit);
  EXPECT_STREQ(rep.c_str(), "deg");
}

TEST(TestSIUnits, TestSteradianUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<steradian_unit.quantity, isq::solid_angle>));
  EXPECT_EQ(steradian_unit.multiplier, 1.0);
  EXPECT_EQ(steradian_unit.reference, 0.0);

  const std::string rep = std::format("{}", steradian_unit);
  EXPECT_STREQ(rep.c_str(), "sr");
}

TEST(TestSIUnits, TestHertzUnit) {
  EXPECT_TRUE((quantity_convertible_to<hertz_unit.quantity, isq::frequency>));
  EXPECT_EQ(hertz_unit.multiplier, 1.0);
  EXPECT_EQ(hertz_unit.reference, 0.0);

  const std::string rep = std::format("{}", hertz_unit);
  EXPECT_STREQ(rep.c_str(), "Hz");
}

TEST(TestSIUnits, TestNewtonUnit) {
  EXPECT_TRUE((quantity_convertible_to<newton_unit.quantity, isq::force>));
  EXPECT_EQ(newton_unit.multiplier, 1.0);
  EXPECT_EQ(newton_unit.reference, 0.0);

  const std::string rep = std::format("{}", newton_unit);
  EXPECT_STREQ(rep.c_str(), "N");
}

TEST(TestSIUnits, TestPascalUnit) {
  EXPECT_TRUE((quantity_convertible_to<pascal_unit.quantity, isq::pressure>));
  EXPECT_EQ(pascal_unit.multiplier, 1.0);
  EXPECT_EQ(pascal_unit.reference, 0.0);

  const std::string rep = std::format("{}", pascal_unit);
  EXPECT_STREQ(rep.c_str(), "Pa");
}

TEST(TestSIUnits, TestJouleUnit) {
  EXPECT_TRUE((quantity_convertible_to<joule_unit.quantity, isq::work>));
  EXPECT_EQ(joule_unit.multiplier, 1.0);
  EXPECT_EQ(joule_unit.reference, 0.0);

  const std::string rep = std::format("{}", joule_unit);
  EXPECT_STREQ(rep.c_str(), "J");
}

TEST(TestSIUnits, TestNewtonMeterUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<newton_meter_unit.quantity, isq::torque>));
  EXPECT_EQ(newton_meter_unit.multiplier, 1.0);
  EXPECT_EQ(newton_meter_unit.reference, 0.0);

  const std::string rep = std::format("{}", newton_meter_unit);
  EXPECT_STREQ(rep.c_str(), "N*m");
}

TEST(TestSIUnits, TestWattUnit) {
  EXPECT_TRUE((quantity_convertible_to<watt_unit.quantity, isq::power>));
  EXPECT_EQ(watt_unit.multiplier, 1.0);
  EXPECT_EQ(watt_unit.reference, 0.0);

  const std::string rep = std::format("{}", watt_unit);
  EXPECT_STREQ(rep.c_str(), "W");

  EXPECT_TRUE(
      (quantity_convertible_to<decibel_watt_unit.quantity, isq::power>));
  EXPECT_EQ(decibel_watt_unit.multiplier, 1.0);
  EXPECT_EQ(decibel_watt_unit.reference, 0.0);
  EXPECT_TRUE((std::same_as<decltype(decibel_watt_unit.scale),
                            const decibel_scale_type>));

  const std::string rep_dbw = std::format("{}", decibel_watt_unit);
  EXPECT_STREQ(rep_dbw.c_str(), "dB");

  EXPECT_TRUE(
      (quantity_convertible_to<decibel_milliwatt_unit.quantity, isq::power>));
  EXPECT_EQ(decibel_milliwatt_unit.multiplier, 1e3);
  EXPECT_EQ(decibel_milliwatt_unit.reference, 0.0);
  EXPECT_TRUE((std::same_as<decltype(decibel_milliwatt_unit.scale),
                            const decibel_scale_type>));

  const std::string rep_dbm = std::format("{}", decibel_milliwatt_unit);
  EXPECT_STREQ(rep_dbm.c_str(), "dBm");
}

TEST(TestSIUnits, TestCoulombUnit) {
  EXPECT_TRUE((quantity_convertible_to<coulomb_unit.quantity, isq::charge>));
  EXPECT_EQ(coulomb_unit.multiplier, 1.0);
  EXPECT_EQ(coulomb_unit.reference, 0.0);

  const std::string rep = std::format("{}", coulomb_unit);
  EXPECT_STREQ(rep.c_str(), "C");
}

TEST(TestSIUnits, TestVoltUnit) {
  EXPECT_TRUE((quantity_convertible_to<volt_unit.quantity, isq::potential>));
  EXPECT_EQ(volt_unit.multiplier, 1.0);
  EXPECT_EQ(volt_unit.reference, 0.0);

  const std::string rep = std::format("{}", volt_unit);
  EXPECT_STREQ(rep.c_str(), "V");
}

TEST(TestSIUnits, TestOhmUnit) {
  EXPECT_TRUE((quantity_convertible_to<ohm_unit.quantity, isq::resistance>));
  EXPECT_EQ(ohm_unit.multiplier, 1.0);
  EXPECT_EQ(ohm_unit.reference, 0.0);

  const std::string rep = std::format("{}", ohm_unit);
  EXPECT_STREQ(rep.c_str(), "Ω");
}

TEST(TestSIUnits, TestSiemensUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<siemens_unit.quantity, isq::conductance>));
  EXPECT_EQ(siemens_unit.multiplier, 1.0);
  EXPECT_EQ(siemens_unit.reference, 0.0);

  const std::string rep = std::format("{}", siemens_unit);
  EXPECT_STREQ(rep.c_str(), "S");
}

TEST(TestSIUnits, TestFaradUnit) {
  EXPECT_TRUE((quantity_convertible_to<farad_unit.quantity, isq::capacitance>));
  EXPECT_EQ(farad_unit.multiplier, 1.0);
  EXPECT_EQ(farad_unit.reference, 0.0);

  const std::string rep = std::format("{}", farad_unit);
  EXPECT_STREQ(rep.c_str(), "F");
}

TEST(TestSIUnits, TestWeberUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<weber_unit.quantity, isq::magnetic_flux>));
  EXPECT_EQ(weber_unit.multiplier, 1.0);
  EXPECT_EQ(weber_unit.reference, 0.0);

  const std::string rep = std::format("{}", weber_unit);
  EXPECT_STREQ(rep.c_str(), "Wb");
}

TEST(TestSIUnits, TestTeslaUnit) {
  EXPECT_TRUE((quantity_convertible_to<tesla_unit.quantity,
                                       isq::magnetic_flux_density>));
  EXPECT_EQ(tesla_unit.multiplier, 1.0);
  EXPECT_EQ(tesla_unit.reference, 0.0);

  const std::string rep = std::format("{}", tesla_unit);
  EXPECT_STREQ(rep.c_str(), "T");
}

TEST(TestSIUnits, TestHenryUnit) {
  EXPECT_TRUE((quantity_convertible_to<henry_unit.quantity, isq::inductance>));
  EXPECT_EQ(henry_unit.multiplier, 1.0);
  EXPECT_EQ(henry_unit.reference, 0.0);

  const std::string rep = std::format("{}", henry_unit);
  EXPECT_STREQ(rep.c_str(), "H");
}

TEST(TestSIUnits, TestLumenUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<lumen_unit.quantity, isq::luminous_flux>));
  EXPECT_EQ(lumen_unit.multiplier, 1.0);
  EXPECT_EQ(lumen_unit.reference, 0.0);

  const std::string rep = std::format("{}", lumen_unit);
  EXPECT_STREQ(rep.c_str(), "lm");
}

TEST(TestSIUnits, TestLuxUnit) {
  EXPECT_TRUE((quantity_convertible_to<lux_unit.quantity, isq::illuminance>));
  EXPECT_EQ(lux_unit.multiplier, 1.0);
  EXPECT_EQ(lux_unit.reference, 0.0);

  const std::string rep = std::format("{}", lux_unit);
  EXPECT_STREQ(rep.c_str(), "lx");
}

TEST(TestSIUnits, TestBecquerelUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<becquerel_unit.quantity, isq::radioactivity>));
  EXPECT_EQ(becquerel_unit.multiplier, 1.0);
  EXPECT_EQ(becquerel_unit.reference, 0.0);

  const std::string rep = std::format("{}", becquerel_unit);
  EXPECT_STREQ(rep.c_str(), "Bq");
}

TEST(TestSIUnits, TestGrayUnit) {
  EXPECT_TRUE((quantity_convertible_to<gray_unit.quantity, isq::dose>));
  EXPECT_EQ(gray_unit.multiplier, 1.0);
  EXPECT_EQ(gray_unit.reference, 0.0);

  const std::string rep = std::format("{}", gray_unit);
  EXPECT_STREQ(rep.c_str(), "Gy");
}

TEST(TestSIUnits, TestSievertUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<sievert_unit.quantity, isq::dose_equivalent>));
  EXPECT_EQ(sievert_unit.multiplier, 1.0);
  EXPECT_EQ(sievert_unit.reference, 0.0);

  const std::string rep = std::format("{}", sievert_unit);
  EXPECT_STREQ(rep.c_str(), "Sv");
}

TEST(TestSIUnits, TestKatalUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<katal_unit.quantity, isq::catalytic_activity>));
  EXPECT_EQ(katal_unit.multiplier, 1.0);
  EXPECT_EQ(katal_unit.reference, 0.0);

  const std::string rep = std::format("{}", katal_unit);
  EXPECT_STREQ(rep.c_str(), "kat");
}

TEST(TestSIUnits, TestSquareMeterUnit) {
  EXPECT_TRUE((quantity_convertible_to<square_meter_unit.quantity, isq::area>));
  EXPECT_EQ(square_meter_unit.multiplier, 1.0);
  EXPECT_EQ(square_meter_unit.reference, 0.0);

  const std::string rep = std::format("{}", square_meter_unit);
  EXPECT_STREQ(rep.c_str(), "m^2");
}

TEST(TestSIUnits, TestCubicMeterUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<cubic_meter_unit.quantity, isq::volume>));
  EXPECT_EQ(cubic_meter_unit.multiplier, 1.0);
  EXPECT_EQ(cubic_meter_unit.reference, 0.0);

  const std::string rep = std::format("{}", cubic_meter_unit);
  EXPECT_STREQ(rep.c_str(), "m^3");
}

TEST(TestSIUnits, TestLiterUnit) {
  EXPECT_TRUE((quantity_convertible_to<liter_unit.quantity, isq::volume>));
  EXPECT_EQ(liter_unit.multiplier, 0.001);
  EXPECT_EQ(liter_unit.reference, 0.0);

  const std::string rep = std::format("{}", liter_unit);
  EXPECT_STREQ(rep.c_str(), "L");
}

TEST(TestSIUnits, TestMeterPerSecondUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<meter_per_second_unit.quantity, isq::velocity>));
  EXPECT_EQ(meter_per_second_unit.multiplier, 1.0);
  EXPECT_EQ(meter_per_second_unit.reference, 0.0);

  const std::string rep = std::format("{}", meter_per_second_unit);
  EXPECT_STREQ(rep.c_str(), "m*s^-1");
}

TEST(TestSIUnits, TestMeterPerSecondPerSecondUnit) {
  EXPECT_TRUE(
      (quantity_convertible_to<meter_per_second_per_second_unit.quantity,
                               isq::acceleration>));
  EXPECT_EQ(meter_per_second_per_second_unit.multiplier, 1.0);
  EXPECT_EQ(meter_per_second_per_second_unit.reference, 0.0);

  const std::string rep = std::format("{}", meter_per_second_per_second_unit);
  EXPECT_STREQ(rep.c_str(), "m*s^-2");
}