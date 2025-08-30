#include "Maxwell.hpp"

#include <gtest/gtest.h>

using namespace maxwell::utility;

TEST(TestUtilities, TestRationalTypeMultiplication) {
  const rational_type<2, 4, 2> lhs;
  const rational_type<2, 4, 2> rhs;

  const rational auto prod = lhs * rhs;

  EXPECT_EQ(prod.numerator, 1);
  EXPECT_EQ(prod.denominator, 4);
  EXPECT_EQ(prod.exponent, 4);
}

TEST(TestUtilities, TestRationalTypeDivision) {
  const rational_type<3, 6, 4> lhs;
  const rational_type<4, 8, 2> rhs;

  const rational auto quot = lhs / rhs;
  EXPECT_EQ(quot.numerator, 1);
  EXPECT_EQ(quot.denominator, 1);
  EXPECT_EQ(quot.exponent, 2);
}

TEST(TestUtilities, TestRationalNumberAddition) {
  const rational_type<3, 6, 2> lhs;
  const rational_type<8, 12, 2> rhs;
  const rational auto sum = lhs + rhs;
  EXPECT_EQ(sum.numerator, 7);
  EXPECT_EQ(sum.denominator, 6);
  EXPECT_EQ(sum.exponent, 2);
}

TEST(TestUtilities, TestRationalNumberSubtraction) {
  const rational_type<3, 6, 2> lhs;
  const rational_type<8, 12, 2> rhs;
  const rational auto diff = lhs - rhs;

  EXPECT_EQ(diff.numerator, -1);
  EXPECT_EQ(diff.denominator, 6);
  EXPECT_EQ(diff.exponent, 2);
}