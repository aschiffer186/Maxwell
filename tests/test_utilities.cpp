#include "utility.hpp"

#include <gtest/gtest.h>

using namespace maxwell;

TEST(TestUtilities, TestSimilar) {
  EXPECT_TRUE((similar<int, int>));
  EXPECT_TRUE((similar<const int, int>));
  EXPECT_TRUE((similar<int&, int>));
  EXPECT_TRUE((similar<const int&, int>));
  EXPECT_TRUE((similar<int&&, int>));
  EXPECT_TRUE((similar<const int&&, int>));
  EXPECT_FALSE((similar<int, double>));
}

TEST(TestUtilities, TestStringLiteral) {
  EXPECT_TRUE(std::ranges::range<string_literal<2>>);
  EXPECT_TRUE(std::ranges::sized_range<string_literal<2>>);

  const string_literal foo{"foo"};
  EXPECT_EQ(foo.size(), 3);

  const string_literal bar{"bar"};
  EXPECT_EQ(foo + bar, string_literal{"foobar"});
}

TEST(TestUtilities, TestRatioLike) {
  EXPECT_TRUE((ratio_like<std::ratio<1, 1>>));
  EXPECT_FALSE(ratio_like<int>);
}

TEST(TestUtilities, TestRationalTypeMultiplication) {
  const rational_type<2, 4, 2> lhs;
  const rational_type<2, 4, 2> rhs;

  const rational auto prod = lhs * rhs;

  EXPECT_EQ(prod.num, 1);
  EXPECT_EQ(prod.den, 4);
  EXPECT_EQ(prod.exp, 4);
}

TEST(TestUtilities, TestRationalTypeDivision) {
  const rational_type<3, 6, 4> lhs;
  const rational_type<4, 8, 2> rhs;

  const rational auto quot = lhs / rhs;
  EXPECT_EQ(quot.num, 1);
  EXPECT_EQ(quot.den, 1);
  EXPECT_EQ(quot.exp, 2);
}

TEST(TestUtilities, TestRationalNumberAddition) {
  const rational_type<3, 6, 2> lhs;
  const rational_type<8, 12, 2> rhs;
  const rational auto sum = lhs + rhs;
  EXPECT_EQ(sum.num, 7);
  EXPECT_EQ(sum.den, 6);
  EXPECT_EQ(sum.exp, 2);
}

TEST(TestUtilities, TestRationalNumberSubtraction) {
  const rational_type<3, 6, 2> lhs;
  const rational_type<8, 12, 2> rhs;
  const rational auto diff = lhs - rhs;
  EXPECT_EQ(diff.num, -1);
  EXPECT_EQ(diff.den, 6);
  EXPECT_EQ(diff.exp, 2);
}

TEST(TestUtilties, TestFromRatio) {
  const rational auto r = from_ratio<std::ratio<10, 20>>();

  EXPECT_EQ(r.num, 1);
  EXPECT_EQ(r.den, 2);
  EXPECT_EQ(r.exp, 0);
}