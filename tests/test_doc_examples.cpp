#include "Maxwell.hpp"

#include <complex>

#include <gtest/gtest.h>

TEST(TestDocExamples, TestExample1) {
  using namespace maxwell;
  using namespace maxwell::si::symbols;

  const si::square_meter<> area1 = 5 * m * 5 * m + 10 * m * 10 * m;
  EXPECT_EQ(area1.get_value_unsafe(), 125.0);

  isq::area_holder<> area2 = 5 * m * 5 * m + 10 * m * 10 * m;
  EXPECT_EQ(area2.get_value_unsafe(), 125.0);
  EXPECT_EQ(area2.get_multiplier(), 1.0);
  EXPECT_EQ(area2.get_reference(), 0.0);
}

TEST(TestDocExamples, TestExample2) {
  using namespace maxwell::si;

  const maxwell::si::meter<> length1{5.0};
  EXPECT_EQ(length1.get_value_unsafe(), 5.0);
}

TEST(TestDocExamples, TestExample3) {
  using namespace maxwell;

  const si::meter<std::complex<double>> length2{std::in_place, 3.0, 4.0};
  EXPECT_EQ(length2.get_value_unsafe(), (std::complex<double>{3.0, 4.0}));
}

TEST(TestDocExamples, TestExample4) {
  using namespace maxwell;
  using namespace maxwell::si::symbols;

  const quantity_value Kb = 5.670'374e-8 * W / (m2 * K * K * K * K);
  EXPECT_FLOAT_EQ(Kb.get_value_unsafe(), 5.670374e-8);
}