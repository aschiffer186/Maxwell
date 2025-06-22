#include "dimensional_product.hpp"
#include "isq_units.hpp"
#include "unit.hpp"
#include "utility.hpp"

#include <gtest/gtest.h>

using namespace maxwell;

TEST(TestUnit, TestUnitConcept) {
  using test_type =
      unit_type<dimension_product_type<dimension_type<std::ratio<1>, "A">>{},
                one, "Foo">;

  EXPECT_TRUE(unit<test_type>);
  EXPECT_TRUE(unit<const test_type>);
  EXPECT_TRUE(unit<test_type&>);
  EXPECT_TRUE(unit<const test_type&>);
  EXPECT_TRUE(unit<test_type&&>);
  EXPECT_TRUE(unit<const test_type&&>);
}

TEST(TestUnit, TestUnitMultiplication) {
  const unit auto prod1 = isq::meter_unit * isq::second_unit;

  const auto p1 = prod1.dimensional_product.tuple_form();

  EXPECT_EQ(prod1.unit_name(), string_literal{"m*s"});
  EXPECT_EQ(std::tuple_size_v<decltype(p1)>, 2);
  EXPECT_EQ(std::get<0>(p1), (dimension_type<std::ratio<1>, "L">{}));
  EXPECT_EQ(std::get<1>(p1), (dimension_type<std::ratio<1>, "T">{}));
  EXPECT_EQ(prod1.multiplier, one);

  const unit auto prod2 = rational_type<1, 1, 2>{} * isq::meter_unit;

  const auto p2 = prod2.dimensional_product.tuple_form();
  EXPECT_EQ(std::tuple_size_v<decltype(p2)>, 1);
  EXPECT_EQ(std::get<0>(p2), (dimension_type<std::ratio<1>, "L">{}));
  EXPECT_EQ(prod2.multiplier, (rational_type<1, 1, 2>{}));

  const unit auto prod3 = prod2 * prod2;
  const auto p3 = prod3.dimensional_product.tuple_form();
  EXPECT_EQ(std::tuple_size_v<decltype(p2)>, 1);
  EXPECT_EQ(std::get<0>(p3), (dimension_type<std::ratio<2>, "L">{}));
  EXPECT_EQ(prod3.multiplier, (rational_type<1, 1, 4>{}));
}