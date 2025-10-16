#include "Maxwell.hpp"

#include <concepts>
#include <gtest/gtest.h>
#include <type_traits>

#include "core/quantity.hpp"
#include "quantity_systems/isq.hpp"
#include "quantity_systems/si.hpp"
#include "quantity_systems/us.hpp"
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

  EXPECT_TRUE((std::convertible_to<double, si::number<>>));
  EXPECT_FALSE((std::convertible_to<double, si::meter<>>));
  EXPECT_FALSE((std::convertible_to<double, si::radian<>>));
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

  using from_type = std::chrono::duration<double, std::ratio<3600>>;
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

  si::kelvin<> k{300.0};
  si::celsius<> c{k};
  si::kelvin<> k2{c};

  EXPECT_FLOAT_EQ(c.get_value(), 26.85);
  EXPECT_FLOAT_EQ(k2.get_value(), 300.0);

  us::fahrenheit<> f{k};
  us::fahrenheit<> f2{c};

  EXPECT_FLOAT_EQ(f.get_value(), 80.33);
  EXPECT_FLOAT_EQ(f2.get_value(), 80.33);

  si::celsius<> c2{f};
  si::kelvin<> k3{f};
  EXPECT_FLOAT_EQ(c2.get_value(), 26.85);
  EXPECT_FLOAT_EQ(k3.get_value(), 300.0);
}

TEST(TestQuantityValue, TestQuantityHolderConstructor) {
  const isq::length_holder<> l{1.0, si::meter_unit};
  const si::kilometer<> km{l};

  EXPECT_FLOAT_EQ(km.get_value(), 1e-3);

  const isq::temperature_holder<> t{300.0, si::kelvin_unit};
  si::celsius<> c{t};
  us::fahrenheit<> f{t};

  EXPECT_FLOAT_EQ(c.get_value(), 26.85);
  EXPECT_FLOAT_EQ(f.get_value(), 80.33);
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

TEST(TestQuantityValue, TestInBaseUnits) {
  const kilo<si::meter<>> km{1.0};
  const auto m = km.in_base_units();

  EXPECT_FLOAT_EQ(m.get_value(), 1'000);
  EXPECT_EQ(m.get_units(), si::meter_unit);
}

TEST(TestQuantityValue, TestNegation) {
  si::meter<> m1{10.0};
  auto m2 = -m1;

  EXPECT_EQ(m2.get_value(), -10.0);
  EXPECT_EQ(m2.get_units(), si::meter_unit);
}

TEST(TestQuantityValue, TestIncrement) {
  si::meter<> m1{10.0};
  auto m2 = ++m1;

  EXPECT_EQ(m2.get_value(), 11.0);
  EXPECT_EQ(m1.get_value(), 11.0);
  EXPECT_EQ(m2.get_units(), si::meter_unit);

  auto m3 = m2++;
  EXPECT_EQ(m3.get_value(), 11.0);
  EXPECT_EQ(m2.get_value(), 12.0);
  EXPECT_EQ(m3.get_units(), si::meter_unit);
}

TEST(TestQuantityValue, TestDecrement) {
  si::meter<> m1{10.0};
  auto m2 = --m1;

  EXPECT_EQ(m2.get_value(), 9.0);
  EXPECT_EQ(m1.get_value(), 9.0);
  EXPECT_EQ(m2.get_units(), si::meter_unit);

  auto m3 = m2--;
  EXPECT_EQ(m3.get_value(), 9.0);
  EXPECT_EQ(m2.get_value(), 8.0);
  EXPECT_EQ(m3.get_units(), si::meter_unit);
}

TEST(TestQuantityValue, TestAddition) {
  si::meter<> m1{10.0};
  auto m2 = m1 += si::meter<>{5.0};

  EXPECT_EQ(m1.get_value(), 15.0);
  EXPECT_EQ(m2.get_value(), 15.0);
  EXPECT_EQ(m1.get_units(), si::meter_unit);

  m1 += kilo<si::meter<>>{1};
  EXPECT_EQ(m1.get_value(), 1015.0);
  EXPECT_EQ(m1.get_units(), si::meter_unit);

  m1 += us::foot<>{1.0};
  EXPECT_NEAR(m1.get_value(), 1015.3048, 1e-4);
  EXPECT_EQ(m1.get_units(), si::meter_unit);

  si::number<> n1{5.0};
  n1 += 2.0;

  EXPECT_EQ(n1.get_value(), 7.0);
  EXPECT_EQ(n1.get_units(), si::number_unit);

  const si::meter<> m3 = m2 + m2;
  EXPECT_EQ(m3.get_value(), 30.0);
  EXPECT_EQ(m3.get_units(), si::meter_unit);

  const us::foot<> ft = m3 + kilo<si::meter<>>{1.0} + us::inch<>{12.0};
  EXPECT_NEAR(ft.get_value(), 3380.2651, 1e-4);
  EXPECT_EQ(ft.get_units(), us::foot_unit);

  const auto n2 = n1 + 3.0;
  EXPECT_EQ(n2.get_value(), 10.0);
  EXPECT_EQ(n2.get_units(), si::number_unit);
}

TEST(TestQuantityValue, TestSubtraction) {
  si::meter<> m2{10.0};
  auto m1 = m2 -= si::meter<>{5.0};

  EXPECT_EQ(m1.get_value(), 5.0);
  EXPECT_EQ(m2.get_value(), 5.0);
  EXPECT_EQ(m1.get_units(), si::meter_unit);

  m2 -= kilo<si::meter<>>{1};
  EXPECT_EQ(m2.get_value(), -995.0);
  EXPECT_EQ(m2.get_units(), si::meter_unit);

  m2 -= us::foot<>{1.0};
  EXPECT_NEAR(m2.get_value(), -995.3048, 1e-4);
  EXPECT_EQ(m2.get_units(), si::meter_unit);

  si::number<> n1{5.0};
  n1 -= 2.0;

  EXPECT_EQ(n1.get_value(), 3.0);
  EXPECT_EQ(n1.get_units(), si::number_unit);

  const si::meter<> m3 = m1 - si::meter<>{2.0};
  EXPECT_EQ(m3.get_value(), 3.0);
  EXPECT_EQ(m3.get_units(), si::meter_unit);

  const us::foot<> ft =
      si::meter<>{1'100} - kilo<si::meter<>>{1.0} - us::inch<>{12.0};
  EXPECT_NEAR(ft.get_value(), 327.0840, 1e-4);
  EXPECT_EQ(ft.get_units(), us::foot_unit);
}

TEST(TestQuantityValue, TestMultiplication) {
  const si::square_meter<> m2 = si::meter<>{10.0} * si::meter<>{10.0};
  EXPECT_FLOAT_EQ(m2.get_value(), 100.0);

  const quantity_value q = si::meter<>{100.0} * us::foot<>{20.0};
  EXPECT_FLOAT_EQ(q.get_value(), 2000);
  EXPECT_EQ(q.get_units(), si::meter_unit * us::foot_unit);

  const quantity_value q2 = 10.0 * si::meter<>{10.0};
  EXPECT_FLOAT_EQ(q2.get_value(), 100.0);
  EXPECT_EQ(q2.get_units(), si::meter_unit);

  const quantity_value q3 = si::meter<>{10.0} * 10.0;
  EXPECT_FLOAT_EQ(q3.get_value(), 100.0);
  EXPECT_EQ(q3.get_units(), si::meter_unit);
  const quantity_value q4 =
      si::radian<>{10.0} / si::second<>{1.0} * si::meter<>{10.0};
  EXPECT_TRUE((quantity_convertible_to<q4.quantity, isq::velocity>));
}

TEST(TestQuantityValue, TestDivision) {
  const si::meter_per_second<> m3 = si::meter<>{10.0} / si::second<>{1.0};
  EXPECT_FLOAT_EQ(m3.get_value(), 10.0);

  const quantity_value q = si::meter<>{10.0} / si::meter<>{10.0};
  EXPECT_EQ(q.get_value(), 1.0);
  EXPECT_TRUE(unitless<q.get_units()>);
  EXPECT_TRUE((std::convertible_to<decltype(q), double>));

  const si::hertz<> hz = 1.0 / si::second<>{10.0};
  EXPECT_EQ(hz.get_value(), 0.1);
}

TEST(TestQuantityValue, TestQuantityComparison) {
  si::meter<> m1{10.0};
  si::meter<> m2{15.0};
  si::meter<> m3{10.0};
  kilo<si::meter<>> km{0.01};

  EXPECT_TRUE(m1 < m2);
  EXPECT_FALSE(m2 < m1);
  EXPECT_FALSE(m1 < m3);
  EXPECT_FALSE(m1 < km);

  EXPECT_TRUE(m2 > m1);
  EXPECT_FALSE(m1 > m2);
  EXPECT_FALSE(m1 > m3);
  EXPECT_FALSE(m1 > km);

  EXPECT_TRUE(m1 <= m2);
  EXPECT_FALSE(m2 <= m1);
  EXPECT_TRUE(m1 <= m3);
  EXPECT_TRUE(m1 <= km);

  EXPECT_TRUE(m2 >= m1);
  EXPECT_FALSE(m1 >= m2);
  EXPECT_TRUE(m1 >= m3);
  EXPECT_TRUE(m1 >= km);

  EXPECT_TRUE(m1 == m3);
  EXPECT_FALSE(m1 == m2);
  EXPECT_TRUE(m1 == km);

  EXPECT_FALSE(m1 != m3);
  EXPECT_TRUE(m1 != m2);
  EXPECT_FALSE(m1 != km);
}

TEST(TestQuantityVary, TestAbbreviatedConstruction) {
  using namespace maxwell::si::symbols;

  const quantity_value q1 = 1.0 * m;
  EXPECT_FLOAT_EQ(q1.get_value(), 1.0);
  EXPECT_EQ(q1.get_units(), si::meter_unit);

  const quantity_value q2 = 1.0 * kilo_unit<m>;
  EXPECT_FLOAT_EQ(q2.get_value(), 1.0);
  EXPECT_EQ(q2.get_units(), kilo_unit<si::meter_unit>);

  const quantity_value q3 = 1.0 * m / s;
  EXPECT_FLOAT_EQ(q3.get_value(), 1.0);
  EXPECT_EQ(q3.get_units(), si::meter_unit / si::second_unit);

  const quantity_value q4 = 1.0 * J / (kg * K);
  EXPECT_FLOAT_EQ(q4.get_value(), 1.0);
  EXPECT_EQ(q4.get_units(),
            si::joule_unit / (si::kilogram_unit * si::kelvin_unit));
}