#include "Maxwell.hpp"

#include <gtest/gtest.h>

using namespace maxwell::utility;

TEST(TestUtilities, TestRationalTypeMultiplication) {
  const rational_type<2, 4> lhs;
  const rational_type<2, 4> rhs;

  const rational auto prod = lhs * rhs;

  EXPECT_EQ(prod.numerator, 1);
  EXPECT_EQ(prod.denominator, 4);
}

TEST(TestUtilities, TestRationalTypeDivision) {
  const rational_type<3, 6> lhs;
  const rational_type<4, 8> rhs;

  const rational auto quot = lhs / rhs;
  EXPECT_EQ(quot.numerator, 1);
  EXPECT_EQ(quot.denominator, 1);
}

TEST(TestUtilities, TestRationalNumberAddition) {
  const rational_type<3, 6> lhs;
  const rational_type<8, 12> rhs;
  const rational auto sum = lhs + rhs;
  EXPECT_EQ(sum.numerator, 7);
  EXPECT_EQ(sum.denominator, 6);
}

TEST(TestUtilities, TestRationalNumberSubtraction) {
  const rational_type<3, 6> lhs;
  const rational_type<8, 12> rhs;
  const rational auto diff = lhs - rhs;

  EXPECT_EQ(diff.numerator, -1);
  EXPECT_EQ(diff.denominator, 6);
}

TEST(TestUtilities, TestSqrt) {
  EXPECT_DOUBLE_EQ(maxwell::utility::sqrt(4.0), 2.0);
  EXPECT_DOUBLE_EQ(maxwell::utility::sqrt(2.0), 1.4142135623730951);
  EXPECT_DOUBLE_EQ(maxwell::utility::sqrt(0.25), 0.5);
  EXPECT_DOUBLE_EQ(maxwell::utility::sqrt(0.0), 0.0);
  EXPECT_DOUBLE_EQ(maxwell::utility::sqrt(1e6), 1e3);
}

TEST(TestUtilities, TestPow) {
  EXPECT_DOUBLE_EQ(maxwell::utility::pow(2.0, maxwell::rational<2, 1>), 4.0);
  EXPECT_DOUBLE_EQ(maxwell::utility::pow(9.0, maxwell::rational<1, 2>), 3.0);
  EXPECT_DOUBLE_EQ(maxwell::utility::pow(27.0, maxwell::rational<1, 3>), 3.0);
  EXPECT_DOUBLE_EQ(maxwell::utility::pow(-8.0, maxwell::rational<-1, 3>), -0.5);
  EXPECT_DOUBLE_EQ(maxwell::utility::pow(1e3, maxwell::rational<3, 1>), 1e9);
}