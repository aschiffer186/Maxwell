#include "Maxwell.hpp"
#include "core/quantity.hpp"
#include "quantity_systems/isq.hpp"
#include "utility/compile_time_math.hpp"

#include <gtest/gtest.h>

using namespace maxwell::isq;
using namespace maxwell;

TEST(TestISQ, TestLengthQuantity) {
  const auto dim_tuple = length.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"L", utility::one>{}));
}

TEST(TestISQ, TestMassQuantity) {
  const auto dim_tuple = mass.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"M", utility::one>{}));
}

TEST(TestISQ, TestTimeQuantity) {
  const auto dim_tuple = maxwell::isq::time.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"T", utility::one>{}));
}

TEST(TestISQ, TestCurrentQuantity) {
  const auto dim_tuple = current.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"I", utility::one>{}));
}

TEST(TestISQ, TestTemperatureQuantity) {
  const auto dim_tuple = temperature.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"Θ", utility::one>{}));
}

TEST(TestISQ, TestAmountQuantity) {
  const auto dim_tuple = amount.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"N", utility::one>{}));
}

TEST(TestISQ, TestLuminosityQuantity) {
  const auto dim_tuple = luminosity.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"J", utility::one>{}));
}

TEST(TestISQ, TestDimensionlessQuantity) {
  EXPECT_EQ(dimensionless.dimensions, dimension_one);
}

TEST(TestISQ, TestPlaneAngleQuantity) {
  EXPECT_EQ(plane_angle.dimensions, dimension_one);
  EXPECT_FALSE((quantity_convertible_to<dimensionless, plane_angle>));
}

TEST(TestISQ, TestSolidAngleQuantity) {
  EXPECT_EQ(solid_angle.dimensions, dimension_one);
  EXPECT_FALSE((quantity_convertible_to<dimensionless, solid_angle>));
}

TEST(TestISQ, TestFrequencyQuantity) {
  const auto dim_tuple = frequency.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 1);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"T", rational<-1, 1>>{}));
  EXPECT_FALSE((quantity_convertible_to<frequency, radioactivity>));
}

TEST(TestISQ, TestForceQuantity) {
  const auto dim_tuple = force.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 3);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"L", utility::one>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"M", utility::one>{}));
  EXPECT_EQ(std::get<2>(dim_tuple), (dimension_type<"T", rational<-2, 1>>{}));
}

TEST(TestISQ, TestPressureQuantity) {
  const auto dim_tuple = pressure.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 3);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"L", rational<-1, 1>>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"M", utility::one>{}));
  EXPECT_EQ(std::get<2>(dim_tuple), (dimension_type<"T", rational<-2, 1>>{}));
}

TEST(TestISQ, TestWorkQuantity) {
  const auto dim_tuple = work.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 3);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"L", rational<2, 1>>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"M", utility::one>{}));
  EXPECT_EQ(std::get<2>(dim_tuple), (dimension_type<"T", rational<-2, 1>>{}));

  EXPECT_FALSE((quantity_convertible_to<work, torque>));
}

TEST(TestISQ, TestPowerQuantity) {
  const auto dim_tuple = power.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 3);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"L", rational<2, 1>>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"M", utility::one>{}));
  EXPECT_EQ(std::get<2>(dim_tuple), (dimension_type<"T", rational<-3, 1>>{}));
}

TEST(TestISQ, TestChargeQuantity) {
  const auto dim_tuple = charge.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 2);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"I", utility::one>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"T", utility::one>{}));
}

TEST(TestISQ, TestPotentialQuantity) {
  const auto dim_tuple = potential.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 4);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"I", rational<-1, 1>>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"L", rational<2, 1>>{}));
  EXPECT_EQ(std::get<2>(dim_tuple), (dimension_type<"M", utility::one>{}));
  EXPECT_EQ(std::get<3>(dim_tuple), (dimension_type<"T", rational<-3, 1>>{}));
}

TEST(TestISQ, TestCapacitanceQuantity) {
  const auto dim_tuple = capacitance.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 4);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"I", rational<2, 1>>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"L", rational<-2, 1>>{}));
  EXPECT_EQ(std::get<2>(dim_tuple), (dimension_type<"M", rational<-1, 1>>{}));
  EXPECT_EQ(std::get<3>(dim_tuple), (dimension_type<"T", rational<4, 1>>{}));
}

TEST(TestISQ, TestResistanceQuantity) {
  const auto dim_tuple = resistance.dimensions.as_tuple();
  using type = decltype(dim_tuple);

  EXPECT_EQ(std::tuple_size_v<type>, 4);
  EXPECT_EQ(std::get<0>(dim_tuple), (dimension_type<"I", rational<-2, 1>>{}));
  EXPECT_EQ(std::get<1>(dim_tuple), (dimension_type<"L", rational<2, 1>>{}));
  EXPECT_EQ(std::get<2>(dim_tuple), (dimension_type<"M", utility::one>{}));
  EXPECT_EQ(std::get<3>(dim_tuple), (dimension_type<"T", rational<-3, 1>>{}));
}