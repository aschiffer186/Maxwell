#include "dimension.hpp"

#include <gtest/gtest.h>

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