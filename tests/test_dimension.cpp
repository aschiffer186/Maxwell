#include "Maxwell.hpp"

#include <gtest/gtest.h>
#include <tuple>

using namespace maxwell;

TEST(TestDimensions, TestDimensionEquality) {
  constexpr dimension_type<"A", utility::rational_type<1, 1, 0>{}> d1;
  constexpr dimension_type<"A", utility::rational_type<2, 1, 0>{}> d2;
  constexpr dimension_type<"B", utility::rational_type<1, 1, 0>{}> d3;

  EXPECT_EQ(d1, d1);
  EXPECT_NE(d1, d2);
  EXPECT_NE(d1, d3);
  EXPECT_EQ(d2, d2);
  EXPECT_NE(d2, d3);
}

TEST(TestDimensions, TestDimensionProduct) {
  using test_type = dimension_type<"A", utility::rational_type<1, 1, 0>{}>;

  EXPECT_FALSE(dimension<int>);
  EXPECT_TRUE(dimension<test_type>);
  EXPECT_TRUE(dimension<const test_type>);
  EXPECT_TRUE(dimension<test_type&>);
  EXPECT_TRUE(dimension<const test_type&>);
  EXPECT_TRUE(dimension<test_type&&>);
  EXPECT_TRUE(dimension<const test_type&&>);
}

TEST(TestDimensions, TestDimensionInverse) {
  using test_type = dimension_type<"A", utility::rational_type<1, 1, 0>{}>;
  using inverse = dimension_inverse_t<test_type>;

  EXPECT_EQ(inverse::name, utility::template_string{"A"});
  EXPECT_EQ(inverse::power, (utility::rational_type<-1, 1, 0>{}));

  using inverse_inverse = dimension_inverse_t<inverse>;

  EXPECT_EQ(inverse_inverse{}, test_type{});
}

TEST(TestDimensions, TestDimensionMultiplication) {
  constexpr dimension_type<"A", utility::rational_type<1, 1, 0>{}> d1;
  constexpr dimension_type<"B", utility::rational_type<2, 1, 0>{}> d2;
  constexpr dimension_type<"A", utility::rational_type<-1, 1, 0>{}> d3;

  constexpr dimension_product auto prod1 = d1 * d2;
  constexpr dimension_product auto prod2 = d1 * d1;
  constexpr dimension_product auto prod3 = d1 * d3;

  const auto prod1_tuple = prod1.as_tuple();
  const auto prod2_tuple = prod2.as_tuple();
  const auto prod3_tuple = prod3.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod1_tuple)>, 2);
  EXPECT_EQ(std::get<0>(prod1_tuple),
            (dimension_type<"A", utility::rational_type<1, 1, 0>{}>{}));
  EXPECT_EQ(std::get<1>(prod1_tuple),
            (dimension_type<"B", utility::rational_type<2, 1, 0>{}>{}));

  EXPECT_EQ(std::tuple_size_v<decltype(prod2_tuple)>, 1);
  EXPECT_EQ(std::get<0>(prod2_tuple),
            (dimension_type<"A", utility::rational_type<2, 1, 0>{}>{}));

  EXPECT_EQ(std::tuple_size_v<decltype(prod3_tuple)>, 0);
}

TEST(TestDimension, TestDimensionDivision) {
  constexpr dimension_type<"A", utility::rational_type<1, 1, 0>{}> d1;
  constexpr dimension_type<"B", utility::rational_type<2, 1, 0>{}> d2;
  constexpr dimension_type<"A", utility::rational_type<-1, 1, 0>{}> d3;

  constexpr dimension_product auto quot1 = d1 / d2;
  constexpr dimension_product auto quot2 = d1 / d1;
  constexpr dimension_product auto quot3 = d1 / d3;

  const auto quot1_tuple = quot1.as_tuple();
  const auto quot2_tuple = quot2.as_tuple();
  const auto quot3_tuple = quot3.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot1_tuple)>, 2);

  EXPECT_EQ(std::get<0>(quot1_tuple),
            (dimension_type<"A", utility::rational_type<1, 1, 0>{}>{}));
  EXPECT_EQ(std::get<1>(quot1_tuple),
            (dimension_type<"B", utility::rational_type<-2, 1, 0>{}>{}));

  EXPECT_EQ(std::tuple_size_v<decltype(quot2_tuple)>, 0);

  EXPECT_EQ(std::tuple_size_v<decltype(quot3_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot3_tuple),
            (dimension_type<"A", utility::rational_type<2, 1, 0>{}>{}));
}