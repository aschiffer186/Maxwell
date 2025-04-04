#include <gtest/gtest.h>

#include "Maxwell.hpp"

TEST(TestUtilities, TestRationalAddition) {
  maxwell::rational r1(1, 2); // 1/2
  maxwell::rational r2(1, 3); // 1/3
  r1 += r2;                   // 1/2 + 1/3 = 5/6
  EXPECT_EQ(r1, maxwell::rational(5, 6));

  maxwell::rational r4(1, 4);
  maxwell::rational r5(1, 4);
  r4 += r5; // 1/4 + 1/4 = 1/2
  EXPECT_EQ(r4, maxwell::rational(1, 2));

  const maxwell::rational r6(1, 2);
  const maxwell::rational r7(1, 3);
  EXPECT_EQ(r6 + r7, maxwell::rational(5, 6));

  const maxwell::rational r8(1, 4);
  const maxwell::rational r9(1, 4);
  EXPECT_EQ(r8 + r9, maxwell::rational(1, 2));
}

TEST(TestUtilities, TestRationalSubtraction) {
  maxwell::rational r1(1, 2); // 1/2
  maxwell::rational r2(1, 3); // 1/3
  r1 -= r2;                   // 1/2 - 1/3 = 1/6
  EXPECT_EQ(r1.numerator, 1);
  EXPECT_EQ(r1.denominator, 6);

  maxwell::rational r4(3, 6);
  maxwell::rational r5(1, 6);
  r4 -= r5; // 3/6 - 1/6 = 1/3
  EXPECT_EQ(r4.numerator, 1);
  EXPECT_EQ(r4.denominator, 3);

  const maxwell::rational r6(1, 2);
  const maxwell::rational r7(1, 3);
  EXPECT_EQ(r6 - r7, maxwell::rational(1, 6));

  const maxwell::rational r8(3, 6);
  const maxwell::rational r9(1, 6);
  EXPECT_EQ(r8 - r9, maxwell::rational(1, 3));
}

TEST(TestUtilities, TestRationalMultiplication) {
  maxwell::rational r1(1, 2); // 1/2
  maxwell::rational r2(2, 3); // 1/3
  r1 *= r2;                   // 1/2 * 2/3 = 1/3
  EXPECT_EQ(r1.numerator, 1);
  EXPECT_EQ(r1.denominator, 3);

  maxwell::rational r3(1, 2);
  EXPECT_EQ(r3 * r2, maxwell::rational(1, 3));
}

TEST(TestUtilities, TestRationalDivision) {
  maxwell::rational r1(1, 2); // 1/2
  maxwell::rational r2(2, 3); // 1/3
  r1 /= r2;                   // 1/2 / 2/3 = 3/4
  EXPECT_EQ(r1.numerator, 3);
  EXPECT_EQ(r1.denominator, 4);

  maxwell::rational r3(1, 2);
  EXPECT_EQ(r3 / r2, maxwell::rational(3, 4));
}

TEST(TestUtilities, TestRationalReduce) {
  maxwell::rational r(8, 16);
  r.reduce();
  EXPECT_EQ(r.numerator, 1);
  EXPECT_EQ(r.denominator, 2);
}

TEST(TestUtilities, TestRationalComparsion) { EXPECT_EQ(maxwell::rational(1, 2), maxwell::rational(2, 4)); }