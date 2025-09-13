#include "Maxwell.hpp"
#include "core/dimension.hpp"
#include "quantity_systems/isq.hpp"

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