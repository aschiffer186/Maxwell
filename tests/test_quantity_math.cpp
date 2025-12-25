#include "Maxwell.hpp"
#include "quantity_systems/isq.hpp"
#include "quantity_systems/si.hpp"

#include <gtest/gtest.h>

#include <numbers>

using namespace maxwell;
using namespace maxwell::math;

TEST(TestQuantityMath, TestAbs) {
  const si::meter<> m{-5.0};
  const si::meter<> abs_m = abs(m);

  EXPECT_FLOAT_EQ(abs_m.get_value(), 5.0);

  const isq::length_holder<> l{si::meter_unit, -5.0};
  const isq::length_holder<> abs_l = abs(l);
  EXPECT_FLOAT_EQ(abs_l.get_value(), 5.0);
}

TEST(TestQuantityMath, TestSin) {
  const si::radian<> angle1{std::numbers::pi / 2.0};
  const double sin1 = sin(angle1);
  EXPECT_FLOAT_EQ(sin1, 1.0);

  const si::degree<> angle2{90.0};
  const double sin2 = sin(angle2);
  EXPECT_FLOAT_EQ(sin2, 1.0);

  const isq::plane_angle_holder<> angle3{si::degree_unit, 90.0};
  const double sin3 = sin(angle3);
  EXPECT_FLOAT_EQ(sin3, 1.0);

  const isq::plane_angle_holder<> angle4{si::radian_unit,
                                         std::numbers::pi / 2.0};
  const double sin4 = sin(angle4);
  EXPECT_FLOAT_EQ(sin4, 1.0);
}

TEST(TestQuantityMath, TestCos) {
  const si::radian<> angle1{std::numbers::pi};
  const double cos1 = cos(angle1);
  EXPECT_FLOAT_EQ(cos1, -1.0);

  const si::degree<> angle2{180.0};
  const double cos2 = cos(angle2);
  EXPECT_FLOAT_EQ(cos2, -1.0);

  const isq::plane_angle_holder<> angle3{si::degree_unit, 180.0};
  const double cos3 = cos(angle3);
  EXPECT_FLOAT_EQ(cos3, -1.0);

  const isq::plane_angle_holder<> angle4{si::radian_unit, std::numbers::pi};
  const double cos4 = cos(angle4);
  EXPECT_FLOAT_EQ(cos4, -1.0);
}

TEST(TestQuantityMath, TestTan) {
  const si::radian<> angle1{std::numbers::pi / 4.0};
  const double tan1 = tan(angle1);
  EXPECT_FLOAT_EQ(tan1, 1.0);

  const si::degree<> angle2{45.0};
  const double tan2 = tan(angle2);
  EXPECT_FLOAT_EQ(tan2, 1.0);

  const isq::plane_angle_holder<> angle3{si::degree_unit, 45.0};
  const double tan3 = tan(angle3);
  EXPECT_FLOAT_EQ(tan3, 1.0);

  const isq::plane_angle_holder<> angle4{si::radian_unit,
                                         std::numbers::pi / 4.0};
  const double tan4 = tan(angle4);
  EXPECT_FLOAT_EQ(tan4, 1.0);
}

TEST(TestQuantityMath, TestSec) {
  const si::radian<> angle1{std::numbers::pi / 3.0};
  const double sec1 = sec(angle1);
  EXPECT_FLOAT_EQ(sec1, 2.0);

  const si::degree<> angle2{60.0};
  const double sec2 = sec(angle2);
  EXPECT_FLOAT_EQ(sec2, 2.0);

  const isq::plane_angle_holder<> angle3{si::degree_unit, 60.0};
  const double sec3 = sec(angle3);
  EXPECT_FLOAT_EQ(sec3, 2.0);

  const isq::plane_angle_holder<> angle4{si::radian_unit,
                                         std::numbers::pi / 3.0};
  const double sec4 = sec(angle4);
  EXPECT_FLOAT_EQ(sec4, 2.0);
}

TEST(TestQuantityMath, TestCsc) {
  const si::radian<> angle1{std::numbers::pi / 6.0};
  const double csc1 = csc(angle1);
  EXPECT_FLOAT_EQ(csc1, 2.0);

  const si::degree<> angle2{30.0};
  const double csc2 = csc(angle2);
  EXPECT_FLOAT_EQ(csc2, 2.0);

  const isq::plane_angle_holder<> angle3{si::degree_unit, 30.0};
  const double csc3 = csc(angle3);
  EXPECT_FLOAT_EQ(csc3, 2.0);

  const isq::plane_angle_holder<> angle4{si::radian_unit,
                                         std::numbers::pi / 6.0};
  const double csc4 = csc(angle4);
  EXPECT_FLOAT_EQ(csc4, 2.0);
}

TEST(TestQuantityMath, TestCot) {
  const si::radian<> angle1{std::numbers::pi / 3.0};
  const double cot1 = cot(angle1);
  EXPECT_FLOAT_EQ(cot1, 1.0 / std::sqrt(3.0));

  const si::degree<> angle2{60.0};
  const double cot2 = cot(angle2);
  EXPECT_FLOAT_EQ(cot2, 1.0 / std::sqrt(3.0));

  const isq::plane_angle_holder<> angle3{si::degree_unit, 60.0};
  const double cot3 = cot(angle3);
  EXPECT_FLOAT_EQ(cot3, 1.0 / std::sqrt(3.0));

  const isq::plane_angle_holder<> angle4{si::radian_unit,
                                         std::numbers::pi / 3.0};
  const double cot4 = cot(angle4);
  EXPECT_FLOAT_EQ(cot4, 1.0 / std::sqrt(3.0));
}

TEST(TestQuantityMath, TestAsin) {
  const double value = 0.5;
  const si::radian<> angle = math::asin(value);

  EXPECT_FLOAT_EQ(angle.get_value(), std::numbers::pi / 6.0);

  const si::degree<> angle_deg = math::asind(value);
  EXPECT_FLOAT_EQ(angle_deg.get_value(), 30.0);
}

TEST(TestQuantityMath, TestAcos) {
  const double value = 0.5;
  const si::radian<> angle = math::acos(value);

  EXPECT_FLOAT_EQ(angle.get_value(), std::numbers::pi / 3.0);

  const si::degree<> angle_deg = math::acosd(value);
  EXPECT_FLOAT_EQ(angle_deg.get_value(), 60.0);
}

TEST(TestQuantityMath, TestAtan) {
  const double value = 1.0;
  const si::radian<> angle = math::atan(value);

  EXPECT_FLOAT_EQ(angle.get_value(), std::numbers::pi / 4.0);

  const si::degree<> angle_deg = math::atand(value);
  EXPECT_FLOAT_EQ(angle_deg.get_value(), 45.0);
}

TEST(TestQuantityMath, TestAtan2) {
  const double y = 1.0;
  const double x = 1.0;
  const si::radian<> angle = math::atan2(y, x);

  EXPECT_FLOAT_EQ(angle.get_value(), std::numbers::pi / 4.0);

  const si::degree<> angle_deg = math::atan2d(y, x);
  EXPECT_FLOAT_EQ(angle_deg.get_value(), 45.0);
}

TEST(TestQuantityMath, TestAcsc) {
  const double value = 2.0;
  const si::radian<> angle = math::acsc(value);

  EXPECT_FLOAT_EQ(angle.get_value(), std::numbers::pi / 6.0);

  const si::degree<> angle_deg = math::acscd(value);
  EXPECT_FLOAT_EQ(angle_deg.get_value(), 30.0);
}

TEST(TestQuantityMath, TestAsec) {
  const double value = 2.0;
  const si::radian<> angle = math::asec(value);

  EXPECT_FLOAT_EQ(angle.get_value(), std::numbers::pi / 3.0);

  const si::degree<> angle_deg = math::asecd(value);
  EXPECT_FLOAT_EQ(angle_deg.get_value(), 60.0);
}

TEST(TestQuantityMath, TestAcot) {
  const double value = 1.0 / std::sqrt(3.0);
  const si::radian<> angle = math::acot(value);

  EXPECT_FLOAT_EQ(angle.get_value(), std::numbers::pi / 3.0);

  const si::degree<> angle_deg = math::acotd(value);
  EXPECT_FLOAT_EQ(angle_deg.get_value(), 60.0);
}

TEST(TestQuantityMath, TestExp) {
  const si::number<> n{1.0};
  const double result = exp(n);

  EXPECT_FLOAT_EQ(result, std::exp(1.0));

  isq::dimensionless_holder<> nh{si::number_unit, 1.0};
  const double result2 = exp(nh);

  EXPECT_FLOAT_EQ(result2, std::exp(1.0));
}

TEST(TestQuantityMath, TestExp2) {
  const si::number<> n{3.0};
  const double result = exp2(n);
  EXPECT_FLOAT_EQ(result, std::exp2(3.0));

  isq::dimensionless_holder<> nh{si::number_unit, 3.0};
  const double result2 = exp2(nh);
  EXPECT_FLOAT_EQ(result2, std::exp2(3.0));
}

TEST(TestQuantityMatth, TestExpm1) {
  const si::number<> n{1.0};
  const double result = expm1(n);

  EXPECT_FLOAT_EQ(result, std::expm1(1.0));

  isq::dimensionless_holder<> nh{si::number_unit, 1.0};
  const double result2 = expm1(nh);

  EXPECT_FLOAT_EQ(result2, std::expm1(1.0));
}

TEST(TestQuantityMath, TestLog) {
  const si::number<> n{std::numbers::e};
  const double result = log(n);

  EXPECT_FLOAT_EQ(result, 1.0);

  isq::dimensionless_holder<> nh{si::number_unit, std::numbers::e};
  const double result2 = log(nh);

  EXPECT_FLOAT_EQ(result2, 1.0);
}

TEST(TestQuantityMath, TestLog2) {
  const si::number<> n{8.0};
  const double result = log2(n);

  EXPECT_FLOAT_EQ(result, 3.0);

  isq::dimensionless_holder<> nh{si::number_unit, 8.0};
  const double result2 = log2(nh);

  EXPECT_FLOAT_EQ(result2, 3.0);
}

TEST(TestQuantityMath, TestLog10) {
  const si::number<> n{1000.0};
  const double result = log10(n);

  EXPECT_FLOAT_EQ(result, 3.0);

  isq::dimensionless_holder<> nh{si::number_unit, 1000.0};
  const double result2 = log10(nh);

  EXPECT_FLOAT_EQ(result2, 3.0);
}

TEST(TestQuantityMath, TestLog1p) {
  const si::number<> n{0.0};
  const double result = log1p(n);

  EXPECT_FLOAT_EQ(result, 0.0);

  isq::dimensionless_holder<> nh{si::number_unit, 0.0};
  const double result2 = log1p(nh);

  EXPECT_FLOAT_EQ(result2, 0.0);
}

TEST(TestQuantityMath, TestPow) {
  const si::meter<> length{2.0};
  const si::square_meter<> area = pow<2>(length);

  EXPECT_FLOAT_EQ(area.get_value(), 4.0);

  const si::meter<> length2{8.0};
  const si::cubic_meter<> volume = pow<rational<3, 1>>(length2);
  EXPECT_FLOAT_EQ(volume.get_value(), 512.0);

  const isq::length_holder<> length3{si::centimeter_unit, 4.0};
  const isq::volume_holder<> volume2 = pow<rational<3, 1>>(length3);

  EXPECT_FLOAT_EQ(volume2.get_value(), 64.0);
  EXPECT_EQ(volume2.get_multiplier(), 1e6);
}