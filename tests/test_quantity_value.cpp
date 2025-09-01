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

  const si::meter<> m2;
  const double value = m2.get_value();
  const unit auto units = m2.get_units();

  EXPECT_EQ(value, double{});
  EXPECT_EQ(units, si::meter_unit);

  using throwing_test_type =
      quantity_value<si::meter_unit, isq::length, throwing_tattle>;

  throwing_test_type m3;

  EXPECT_EQ(m3.get_value(), throwing_tattle{});
  EXPECT_FALSE(noexcept(throwing_test_type{}));
}

TEST(TestQuantityValue, TestValueConstructor) {
  using nothrow_test_type =
      quantity_value<si::meter_unit, isq::length, nothrow_tattle>;

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

TEST(TestQuantityValue, TestInPlaceConstructor) {
  using test_type = si::meter<nothrow_tattle>;

  int value_ctor_count = nothrow_tattle::value_ctor_count;
  int move_ctor_count = nothrow_tattle::move_ctor_count;
  int copy_ctor_count = nothrow_tattle::copy_ctor_count;
  int il_ctor_count = nothrow_tattle::il_ctor_count;

  test_type q{std::in_place, 1.0, 2.0};

  EXPECT_FLOAT_EQ(q.get_value().value, 3.0);
  EXPECT_EQ(nothrow_tattle::value_ctor_count, value_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::move_ctor_count, move_ctor_count);
  EXPECT_EQ(nothrow_tattle::copy_ctor_count, copy_ctor_count);
  EXPECT_EQ(nothrow_tattle::il_ctor_count, il_ctor_count);

  test_type q2{std::in_place, {1.0, 2.0, 3.0}};

  EXPECT_FLOAT_EQ(q2.get_value().value, 6.0);
  EXPECT_EQ(nothrow_tattle::value_ctor_count, value_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::il_ctor_count, il_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::move_ctor_count, move_ctor_count);
  EXPECT_EQ(nothrow_tattle::copy_ctor_count, copy_ctor_count);
}

TEST(TestQuantityValue, TestChronoConstructor) {
  using namespace std::chrono_literals;

  milli<si::second<>> q1{1s};

  using from_type = std::chrono::duration<double, std::ratio<27, 36>>;
  using to_type = std::chrono::duration<double, std::nano>;

  to_type expected_value = from_type(1s);
  nano<si::second<>> q2{from_type(1s)};

  EXPECT_FLOAT_EQ(q1.get_value(), 1000.0);
  EXPECT_FLOAT_EQ(q2.get_value(), expected_value.count());
}

TEST(TestQuantityValue, TestConversionOperator) {
  using test_type1 = si::meter<>;
  using test_type2 = si::number<>;
  using test_type3 = si::radian<>;

  EXPECT_FALSE((std::convertible_to<test_type1, double>));
  EXPECT_TRUE((std::convertible_to<test_type2, double>));
  EXPECT_FALSE((std::convertible_to<test_type3, double>));

  const si::meter<> m{10.0};
  const si::number<> n{5.0};
  const si::radian<> r{3.0};

  const double d1 = static_cast<double>(m);
  const double d2 = n;
  const double d3 = static_cast<double>(r);

  EXPECT_FLOAT_EQ(d1, 10.0);
  EXPECT_FLOAT_EQ(d2, 5.0);
  EXPECT_FLOAT_EQ(d3, 3.0);
}