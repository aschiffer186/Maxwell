#include "Maxwell.hpp"
#include "core/quantity.hpp"
#include "quantity_systems/isq.hpp"

#include <gtest/gtest.h>

using namespace maxwell;

TEST(TestQuantity, TestDimensionSum) {
  const auto sum1 = isq::length.dimension_sum();

  EXPECT_EQ(sum1, utility::one);

  const auto sum2 = isq::area.dimension_sum();
  EXPECT_EQ(sum2, (utility::rational_type<2, 1, 0>{}));

  const auto sum3 = isq::volume.dimension_sum();
  EXPECT_EQ(sum3, (utility::rational_type<3, 1, 0>{}));

  const auto sum4 = isq::plane_angle.dimension_sum();
  EXPECT_EQ(sum4, utility::one);

  const auto square = isq::plane_angle * isq::plane_angle;
  const auto sum5 = square.dimension_sum();
  EXPECT_EQ(sum5, (utility::rational_type<2, 1, 0>{}));
}

TEST(TestQuantity, TestQuantityConvertibleTo) {
  using test_system = quantity_system<"A", "B">;

  constexpr auto quantity_A = test_system::base_quantity<"A">{};
  constexpr auto quantity_B = test_system::base_quantity<"B">{};

  EXPECT_TRUE((quantity_convertible_to<quantity_A, quantity_A>));
  EXPECT_TRUE((quantity_convertible_to<quantity_B, quantity_B>));
  EXPECT_FALSE((quantity_convertible_to<quantity_A, quantity_B>));
  EXPECT_FALSE((quantity_convertible_to<quantity_B, quantity_A>));

  constexpr auto quantity_prod = quantity_A * quantity_B;
  constexpr auto quantity_quot = quantity_A / quantity_B;

  EXPECT_TRUE(
      (quantity_convertible_to<quantity_prod, quantity_A * quantity_B>));
  EXPECT_TRUE(
      (quantity_convertible_to<quantity_A * quantity_B, quantity_prod>));
  EXPECT_FALSE((quantity_convertible_to<quantity_prod, quantity_A>));
  EXPECT_FALSE((quantity_convertible_to<quantity_prod, quantity_B>));

  EXPECT_TRUE(
      (quantity_convertible_to<quantity_quot, quantity_A / quantity_B>));
  EXPECT_TRUE(
      (quantity_convertible_to<quantity_A / quantity_B, quantity_quot>));

  constexpr auto derived_quantity1 =
      make_derived_quantity_t<"C", quantity_prod>{};
  constexpr auto super_derived_quantity1 =
      make_derived_quantity_t<"D", derived_quantity1>{};

  EXPECT_TRUE((quantity_convertible_to<quantity_prod, derived_quantity1>));
  EXPECT_TRUE((quantity_convertible_to<derived_quantity1, quantity_prod>));

  EXPECT_TRUE(
      (quantity_convertible_to<quantity_prod, super_derived_quantity1>));
  EXPECT_TRUE(
      (quantity_convertible_to<super_derived_quantity1, derived_quantity1>));
  EXPECT_TRUE(
      (quantity_convertible_to<super_derived_quantity1, quantity_prod>));

  EXPECT_FALSE(
      (quantity_convertible_to<derived_quantity1, super_derived_quantity1>));
  EXPECT_TRUE(
      (quantity_convertible_to<quantity_prod, super_derived_quantity1>));

  constexpr auto derived_quantity2 =
      make_derived_quantity_t<"C", quantity_quot>{};
  constexpr auto super_derived_quantity2 =
      make_derived_quantity_t<"D", derived_quantity2>{};

  EXPECT_TRUE((quantity_convertible_to<quantity_quot, derived_quantity2>));
  EXPECT_FALSE((quantity_convertible_to<derived_quantity1, quantity_quot>));

  EXPECT_TRUE(
      (quantity_convertible_to<quantity_quot, super_derived_quantity2>));
  EXPECT_TRUE(
      (quantity_convertible_to<super_derived_quantity2, derived_quantity2>));
  EXPECT_TRUE(
      (quantity_convertible_to<quantity_quot, super_derived_quantity2>));

  EXPECT_FALSE(
      (quantity_convertible_to<derived_quantity2, super_derived_quantity2>));
  EXPECT_TRUE(
      (quantity_convertible_to<super_derived_quantity2, quantity_quot>));
}