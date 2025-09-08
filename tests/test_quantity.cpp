#include "Maxwell.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using namespace maxwell;

TEST(TestQuantity, TestQuantityConcept) {
  EXPECT_TRUE((quantity<decltype(isq::length)>));
  EXPECT_TRUE((quantity<std::remove_cv_t<decltype(isq::area)>>));
  EXPECT_TRUE((quantity<std::add_lvalue_reference_t<decltype(isq::volume)>>));
  EXPECT_TRUE(
      (quantity<std::add_rvalue_reference_t<decltype(isq::plane_angle)>>));
  EXPECT_TRUE((quantity<std::add_lvalue_reference_t<
                   std::remove_cv_t<decltype(isq::solid_angle)>>>));

  EXPECT_FALSE((quantity<int>));
  EXPECT_FALSE((quantity<double>));
  EXPECT_FALSE((quantity<std::string>));
}

TEST(TestQuantity, TestQuantityProduct) {
  const auto product1 = isq::length * isq::length;
  EXPECT_EQ(product1.dimensions, isq::area.dimensions);
  EXPECT_EQ(product1.kind, utility::template_string("L*L"));
}

TEST(TestQuantity, TestQuantityQuotient) {
  const auto quotient1 = isq::length / isq::time;
  EXPECT_EQ(quotient1.dimensions,
            (dimension_product_type<
                dimension_type<"L", utility::rational_type<1, 1, 0>{}>,
                dimension_type<"T", utility::rational_type<-1, 1, 0>{}>>{}));
  EXPECT_EQ(quotient1.kind, utility::template_string("L/T"));
}

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

TEST(TestQuantity, TestQuantiyFactories) {
  constexpr auto derived1 =
      derived_quantity<isq::length / isq::time, "Speed">{};
  constexpr quantity auto sub = sub_quantity<derived1, "CarSpeed">{};

  EXPECT_FALSE(derived1.derived);
  EXPECT_TRUE(sub.derived);

  const auto derived_tupl = derived1.dimensions.as_tuple();

  EXPECT_EQ(std::get<0>(derived_tupl),
            (dimension_type<"L", utility::rational_type<1, 1, 0>{}>{}));
  EXPECT_EQ(std::get<1>(derived_tupl),
            (dimension_type<"T", utility::rational_type<-1, 1, 0>{}>{}));
  EXPECT_EQ(std::tuple_size<decltype(derived_tupl)>::value, 2);

  EXPECT_EQ(sub.dimensions, derived1.dimensions);
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

  constexpr auto derived_quantity1 = sub_quantity<quantity_prod, "C">{};
  constexpr auto super_derived_quantity1 =
      sub_quantity<derived_quantity1, "D">{};

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

  constexpr auto derived_quantity2 = sub_quantity<quantity_quot, "C">{};
  constexpr auto super_derived_quantity2 =
      sub_quantity<derived_quantity2, "D">{};

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