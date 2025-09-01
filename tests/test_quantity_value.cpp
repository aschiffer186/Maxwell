#include "Maxwell.hpp"

#include <concepts>
#include <gtest/gtest.h>
#include <type_traits>

#include "test_types.hpp"

using namespace maxwell;

TEST(TestQuantityValue, CXXProperties) {
  using test_type = si::meter<>;

  // Should match underlying properties of double
  EXPECT_EQ(sizeof(test_type), sizeof(double));
  EXPECT_EQ(alignof(test_type), alignof(double));

  EXPECT_TRUE(std::is_default_constructible_v<test_type>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<test_type>);
  EXPECT_TRUE(std::is_copy_constructible_v<test_type>);
  EXPECT_TRUE(std::is_nothrow_copy_constructible_v<test_type>);
  EXPECT_TRUE(std::is_move_constructible_v<test_type>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<test_type>);
  EXPECT_TRUE(std::is_copy_assignable_v<test_type>);
  EXPECT_TRUE(std::is_nothrow_copy_assignable_v<test_type>);
  EXPECT_TRUE(std::is_move_assignable_v<test_type>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<test_type>);
  EXPECT_TRUE(std::is_destructible_v<test_type>);
  EXPECT_TRUE(std::is_nothrow_destructible_v<test_type>);
  EXPECT_TRUE(std::is_trivially_destructible_v<test_type>);
  EXPECT_TRUE(std::is_trivially_copyable_v<test_type>);
  EXPECT_TRUE(std::is_standard_layout_v<test_type>);
}

TEST(TestQuantityValue, TestDefaultConstructor) {
  si::meter<> m;

  EXPECT_EQ(m.get_value(), double{});
  EXPECT_EQ(m.get_units(), si::meter_unit);

  constexpr si::meter<> m2;
  constexpr double value = m2.get_value();
  constexpr unit auto units = m2.get_units();

  EXPECT_EQ(m2.get_value(), double{});
  EXPECT_EQ(m2.get_units(), si::meter_unit);

  using throwing_test_type =
      quantity_value<si::meter_unit, isq::length, throwing_tattle>;

  throwing_test_type m3;

  EXPECT_EQ(m3.get_value(), throwing_tattle{});
  EXPECT_FALSE(noexcept(throwing_test_type{}));
}

TEST(TestQuantityValue, TestValueConstructor) {
  using nothrow_test_type =
      quantity_value<si::meter_unit, isq::length, nothrow_tattle>;
  using throwing_test_type =
      quantity_value<si::meter_unit, isq::length, throwing_tattle>;

  int start_copy_ctor_count = nothrow_tattle::copy_ctor_count;
  int start_move_ctor_count = nothrow_tattle::move_ctor_count;

  nothrow_tattle t1{1.0, 1.0};
  nothrow_test_type q1(t1);

  EXPECT_EQ(q1.get_value(), t1);
  EXPECT_EQ(q1.get_units(), si::meter_unit);
  EXPECT_EQ(nothrow_tattle::copy_ctor_count, start_copy_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::move_ctor_count, start_move_ctor_count);

  nothrow_test_type q2(nothrow_tattle{2.0, 2.0});
  EXPECT_EQ(q2.get_value(), (nothrow_tattle{2.0, 2.0}));
  EXPECT_EQ(q2.get_units(), si::meter_unit);
  EXPECT_EQ(nothrow_tattle::copy_ctor_count, start_copy_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::move_ctor_count, start_move_ctor_count + 1);
}

TEST(TestQuantityValue, TestConversionOperator) {
  using test_type1 = si::meter<>;
  using test_type2 = si::number<>;
  using test_type3 = si::radian<>;

  EXPECT_FALSE((std::convertible_to<test_type1, double>));
  EXPECT_TRUE((std::convertible_to<test_type2, double>));
  EXPECT_FALSE((std::convertible_to<test_type3, double>));
}