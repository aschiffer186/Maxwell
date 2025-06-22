#include "dimensional_product.hpp"
#include "unit.hpp"

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