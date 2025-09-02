#include "Maxwell.hpp"

#include <concepts>
#include <gtest/gtest.h>
#include <type_traits>

#include "quantity_systems/other.hpp"
#include "quantity_systems/si.hpp"
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

TEST(TestQuantityValue, TestConvertingConstructor) {
  kilo<si::meter<>> km{1.0};
  si::meter<> m{km};
  kilo<si::meter<>> km2{m};
  nano<si::meter<>> nm{km};
  si::meter<> m2{nm};

  EXPECT_FLOAT_EQ(m.get_value(), 1000.0);
  EXPECT_FLOAT_EQ(km2.get_value(), 1.0);
  EXPECT_FLOAT_EQ(nm.get_value(), 1e9 * 1e3);
  EXPECT_FLOAT_EQ(m2.get_value(), 1000.0);

  milli<si::radian<>> mr{500.0};
  si::degree<> deg{mr};
  milli<si::radian<>> mr2{deg};

  EXPECT_NEAR(deg.get_value(), 28.64788975654116, 1e-5);
  EXPECT_FLOAT_EQ(mr2.get_value(), 500.0);

  const quantity_value mr3 = mr * mr;
  const quantity_value<si::degree_unit * si::degree_unit> deg2{mr3};
  const quantity_value mr4{mr3};

  EXPECT_FLOAT_EQ(deg2.get_value(), .5 * .5 * (180.0 * 180.0) /
                                        (std::numbers::pi * std::numbers::pi));
  EXPECT_FLOAT_EQ(mr4.get_value(), 500.0 * 500.0);

  si::square_meter<> sm{1.0};
  centi<si::square_meter<>> sq_cm{sm};
  si::square_meter<> sm2{sm};
  kilo<si::square_meter<>> sq_km{sq_cm};

  EXPECT_FLOAT_EQ(sq_cm.get_value(), 1e4);
  EXPECT_FLOAT_EQ(sm2.get_value(), 1.0);
  EXPECT_FLOAT_EQ(sq_km.get_value(), 1e-6);

  other::time::minute<> min{1.0};
  si::second<> s{min};

  EXPECT_FLOAT_EQ(s.get_value(), 60.0);
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