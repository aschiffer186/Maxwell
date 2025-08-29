#include "quantity.hpp"

#include <gtest/gtest.h>

#include "quantity_system.hpp"

using namespace maxwell;

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