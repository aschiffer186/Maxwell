#include "Maxwell.hpp"

#include <gtest/gtest.h>
#include <type_traits>

#include "quantity_systems/isq.hpp"
#include "test_types.hpp"

using namespace maxwell;
using namespace maxwell::isq;

TEST(TestQuantityHolder, TestCXXProperties) {
  using test_type = length_holder<>;

  EXPECT_EQ(sizeof(test_type), 3 * sizeof(double));
  EXPECT_EQ(alignof(test_type), alignof(double));

  EXPECT_FALSE(std::is_default_constructible_v<test_type>);
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

TEST(TestQuantityHolder, TestUnitConstructor) {
  length_holder<> l{kilo_unit<si::meter_unit>};
  EXPECT_EQ(l.get_value_unsafe(), 0.0);
  EXPECT_EQ(l.get_multiplier(), 1e-3);
  EXPECT_EQ(l.get_reference(), 0.0);

  temperature_holder<> t{si::celsius_unit};
  EXPECT_EQ(t.get_value_unsafe(), 0.0);
  EXPECT_EQ(t.get_multiplier(), 1.0);
  EXPECT_EQ(t.get_reference(), -273.15);

  EXPECT_TRUE(
      (std::is_nothrow_constructible_v<length_holder<>, si::meter_unit_type>));
  EXPECT_FALSE((std::is_nothrow_constructible_v<length_holder<throwing_tattle>,
                                                si::meter_unit_type>));
}

TEST(TestQuantityHolder, TestValueConstructors) {
  length_holder<> l1{kilo_unit<si::meter_unit>, 5.0};
  EXPECT_EQ(l1.get_value_unsafe(), 5.0);
  EXPECT_EQ(l1.get_multiplier(), 1e-3);
  EXPECT_EQ(l1.get_reference(), 0.0);

  length_holder<> l2{si::meter_unit, 5000.0};
  EXPECT_EQ(l2.get_value_unsafe(), 5000.0);
  EXPECT_EQ(l2.get_multiplier(), 1.0);
  EXPECT_EQ(l2.get_reference(), 0.0);

  temperature_holder<> t1{si::celsius_unit, 100.0};
  EXPECT_EQ(t1.get_value_unsafe(), 100.0);
  EXPECT_EQ(t1.get_multiplier(), 1.0);
  EXPECT_EQ(t1.get_reference(), -273.15);

  temperature_holder<> t2{si::kelvin_unit, 373.15};
  EXPECT_EQ(t2.get_value_unsafe(), 373.15);
  EXPECT_EQ(t2.get_multiplier(), 1.0);
  EXPECT_EQ(t2.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestUnitPieceConstructor) {
  quantity_holder<isq::temperature, double> t{10.0, 9.0 / 5.0, -459.67};
  EXPECT_EQ(t.get_value_unsafe(), 10.0);
  EXPECT_EQ(t.get_multiplier(), 9.0 / 5.0);
  EXPECT_EQ(t.get_reference(), -459.67);
}

TEST(TestQuantityHolder, TestInPlaceConstructor) {
  using test_type = temperature_holder<nothrow_tattle>;

  int value_ctor_count = nothrow_tattle::value_ctor_count;
  int move_ctor_count = nothrow_tattle::move_ctor_count;
  int copy_ctor_count = nothrow_tattle::copy_ctor_count;
  int il_ctor_count = nothrow_tattle::il_ctor_count;

  test_type t{us::fahrenheit_unit, std::in_place, 1.0, 2.0};

  EXPECT_FLOAT_EQ(t.get_value_unsafe().value, 3.0);
  EXPECT_FLOAT_EQ(t.get_multiplier(), us::fahrenheit_unit.multiplier);
  EXPECT_FLOAT_EQ(t.get_reference(), us::fahrenheit_unit.reference);
  EXPECT_EQ(nothrow_tattle::value_ctor_count, value_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::move_ctor_count, move_ctor_count);
  EXPECT_EQ(nothrow_tattle::copy_ctor_count, copy_ctor_count);
  EXPECT_EQ(nothrow_tattle::il_ctor_count, il_ctor_count);

  test_type t2{us::fahrenheit_unit, std::in_place, {1.0, 2.0, 3.0}};
  EXPECT_FLOAT_EQ(t2.get_value_unsafe().value, 6.0);
  EXPECT_FLOAT_EQ(t2.get_multiplier(), us::fahrenheit_unit.multiplier);
  EXPECT_FLOAT_EQ(t2.get_reference(), us::fahrenheit_unit.reference);
  EXPECT_EQ(nothrow_tattle::value_ctor_count, value_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::il_ctor_count, il_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::move_ctor_count, move_ctor_count);
  EXPECT_EQ(nothrow_tattle::copy_ctor_count, copy_ctor_count);
}

TEST(TestQuantityHolder, TestChronoConstructor) {
  using namespace std::chrono_literals;

  time_holder<> t1{std::chrono::seconds{10}};
  EXPECT_EQ(t1.get_value_unsafe(), 10.0);
  EXPECT_EQ(t1.get_multiplier(), 1.0);

  time_holder<> t2{std::chrono::nanoseconds{1}};
  EXPECT_EQ(t2.get_value_unsafe(), 1.0);
  EXPECT_EQ(t2.get_multiplier(), nano_unit<si::second_unit>.multiplier);
}

TEST(TestQuatityHolder, TestQuantityValueConstructor) {
  temperature_holder<> t1{us::fahrenheit<>{32.0}};
  EXPECT_FLOAT_EQ(t1.get_value_unsafe(), 32.0);
  EXPECT_FLOAT_EQ(t1.get_multiplier(), us::fahrenheit_unit.multiplier);
  EXPECT_FLOAT_EQ(t1.get_reference(), us::fahrenheit_unit.reference);
}

TEST(TestQuantityHolder, TestCTAD) {
  const quantity_holder l1{si::kilometer_unit, 5.0};
  EXPECT_TRUE((utility::similar<decltype(l1.quantity), decltype(isq::length)>));

  const quantity_holder t2{si::kelvin{}};
  EXPECT_TRUE(
      (utility::similar<decltype(t2.quantity), decltype(isq::temperature)>));

  const quantity_holder t3{isq::temperature_holder<>{si::celsius_unit}};
  EXPECT_TRUE(
      (utility::similar<decltype(t3.quantity), decltype(isq::temperature)>));
}

TEST(TestQuantityHolder, TestAsMethod) {
  length_holder<> l{si::meter_unit, 5000.0};
  const kilo<si::meter<>> km = l.as(kilo_unit<si::meter_unit>);
  EXPECT_FLOAT_EQ(km.get_value_unsafe(), 5.0);

  temperature_holder<> t{si::kelvin_unit, 300.0};
  const si::celsius<> c = t.as(si::celsius_unit);
  EXPECT_FLOAT_EQ(c.get_value_unsafe(), 26.85);

  const us::fahrenheit<> f = t.as(us::fahrenheit_unit);
  EXPECT_FLOAT_EQ(f.get_value_unsafe(), 80.33);
}

TEST(TestQuantityHolder, TestInMethod) {
  temperature_holder<> t{si::kelvin_unit, 300.0};
  const double f = t.in(us::fahrenheit_unit);
  EXPECT_FLOAT_EQ(f, 80.33);
}

TEST(TestQuantityHolder, TestNegation) {
  length_holder<> l1{si::meter_unit, 5000.0};
  const length_holder<> l2 = -l1;
  EXPECT_FLOAT_EQ(l2.get_value_unsafe(), -5000.0);
  EXPECT_FLOAT_EQ(l2.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l2.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestIncrement) {
  length_holder<> l1{si::meter_unit, 500.0};
  const length_holder<> l2 = ++l1;
  EXPECT_FLOAT_EQ(l1.get_value_unsafe(), 501.0);
  EXPECT_FLOAT_EQ(l2.get_value_unsafe(), 501.0);
  EXPECT_FLOAT_EQ(l1.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l1.get_reference(), 0.0);
  EXPECT_FLOAT_EQ(l2.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l2.get_reference(), 0.0);

  length_holder<> l3{si::meter_unit, 500.0};
  const length_holder<> l4 = l3++;
  EXPECT_FLOAT_EQ(l3.get_value_unsafe(), 501.0);
  EXPECT_FLOAT_EQ(l4.get_value_unsafe(), 500.0);
  EXPECT_FLOAT_EQ(l3.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l3.get_reference(), 0.0);
  EXPECT_FLOAT_EQ(l4.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l4.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestDecrement) {
  length_holder<> l1{si::meter_unit, 500.0};
  const length_holder<> l2 = --l1;
  EXPECT_FLOAT_EQ(l1.get_value_unsafe(), 499.0);
  EXPECT_FLOAT_EQ(l2.get_value_unsafe(), 499.0);
  EXPECT_FLOAT_EQ(l1.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l1.get_reference(), 0.0);
  EXPECT_FLOAT_EQ(l2.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l2.get_reference(), 0.0);

  length_holder<> l3{si::meter_unit, 500.0};
  const length_holder<> l4 = l3--;
  EXPECT_FLOAT_EQ(l3.get_value_unsafe(), 499.0);
  EXPECT_FLOAT_EQ(l4.get_value_unsafe(), 500.0);
  EXPECT_FLOAT_EQ(l3.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l3.get_reference(), 0.0);
  EXPECT_FLOAT_EQ(l4.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l4.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestAddition) {
  length_holder<> l1{si::meter_unit, 5000.0};
  l1 += si::kilometer<>{2.0};
  EXPECT_FLOAT_EQ(l1.get_value_unsafe(), 7000.0);
  EXPECT_FLOAT_EQ(l1.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l1.get_reference(), 0.0);

  area_holder<> a1{centi_unit<si::square_meter_unit>, 10'000.0};
  a1 += si::square_meter<>{1.0};
  EXPECT_FLOAT_EQ(a1.get_value_unsafe(), 20'000.0);
  EXPECT_FLOAT_EQ(a1.get_multiplier(), 1e4);
  EXPECT_FLOAT_EQ(a1.get_reference(), 0.0);

  const length_holder l2{si::meter_unit, 3000.0};
  const length_holder l3 = l2 + length_holder<>{si::kilometer_unit, 3.0};
  EXPECT_FLOAT_EQ(l3.get_value_unsafe(), 6000.0);
  EXPECT_FLOAT_EQ(l3.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l3.get_reference(), 0.0);

  area_holder<> a2{centi_unit<si::square_meter_unit>, 10'000.0};
  const area_holder<> a3 = a2 + area_holder<>{si::square_meter_unit, 1.0};
  EXPECT_FLOAT_EQ(a3.get_value_unsafe(), 20'000.0);
  EXPECT_FLOAT_EQ(a3.get_multiplier(), 1e4);

  temperature_holder<> t1{si::celsius_unit, 100.0};
  EXPECT_THROW(t1 += si::kelvin<>{173.15}, incompatible_quantity_holder);

  const dimensionless_holder<> d1{si::number_unit, 10.0};
  const dimensionless_holder<> d2 = d1 + 20.0;
  EXPECT_FLOAT_EQ(d2.get_value_unsafe(), 30.0);
  EXPECT_FLOAT_EQ(d2.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(d2.get_reference(), 0.0);

  dimensionless_holder<> d3{si::number_unit, 10.0};
  d3 += 20.0;
  EXPECT_FLOAT_EQ(d3.get_value_unsafe(), 30.0);
  EXPECT_FLOAT_EQ(d3.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(d3.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestSubtraction) {
  length_holder<> l1{si::meter_unit, 5000.0};
  l1 -= si::kilometer<>{2.0};
  EXPECT_FLOAT_EQ(l1.get_value_unsafe(), 3000.0);
  EXPECT_FLOAT_EQ(l1.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l1.get_reference(), 0.0);

  area_holder<> a1{centi_unit<si::square_meter_unit>, 20'000.0};
  a1 -= si::square_meter<>{1.0};
  EXPECT_FLOAT_EQ(a1.get_value_unsafe(), 10'000.0);
  EXPECT_FLOAT_EQ(a1.get_multiplier(), 1e4);
  EXPECT_FLOAT_EQ(a1.get_reference(), 0.0);

  const length_holder l2{si::meter_unit, 5000.0};
  const length_holder l3 = l2 - length_holder<>{si::kilometer_unit, 3.0};
  EXPECT_FLOAT_EQ(l3.get_value_unsafe(), 2000.0);
  EXPECT_FLOAT_EQ(l3.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l3.get_reference(), 0.0);

  area_holder<> a2{centi_unit<si::square_meter_unit>, 20'000.0};
  const area_holder<> a3 = a2 - area_holder<>{si::square_meter_unit, 1.0};
  EXPECT_FLOAT_EQ(a3.get_value_unsafe(), 10'000.0);
  EXPECT_FLOAT_EQ(a3.get_multiplier(), 1e4);
  EXPECT_FLOAT_EQ(a3.get_reference(), 0.0);

  temperature_holder<> t1{si::celsius_unit, 100.0};
  EXPECT_THROW(t1 -= si::kelvin<>{173.15}, incompatible_quantity_holder);

  dimensionless_holder<> d1{si::number_unit, 10.0};
  d1 -= 20.0;
  EXPECT_FLOAT_EQ(d1.get_value_unsafe(), -10.0);
  EXPECT_FLOAT_EQ(d1.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(d1.get_reference(), 0.0);

  const dimensionless_holder<> d2{si::number_unit, 30.0};
  const dimensionless_holder<> d3 = d2 - 15.0;
  EXPECT_FLOAT_EQ(d3.get_value_unsafe(), 15.0);
  EXPECT_FLOAT_EQ(d3.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(d3.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestMultiplication) {
  const length_holder<> l1{si::centimeter_unit, 5.0};
  const area_holder<> a1 = l1 * l1;
  EXPECT_FLOAT_EQ(a1.get_value_unsafe(), 25.0);
  EXPECT_FLOAT_EQ(a1.get_multiplier(), 1e4);
  EXPECT_FLOAT_EQ(a1.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestDivision) {
  const area_holder<> a1{si::square_meter_unit, 20.0};
  const length_holder<> l1 = a1 / length_holder<>{si::meter_unit, 4.0};
  EXPECT_FLOAT_EQ(l1.get_value_unsafe(), 5.0);
  EXPECT_FLOAT_EQ(l1.get_multiplier(), 1.0);
  EXPECT_FLOAT_EQ(l1.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestMixedAddition) {
  length_holder<> l1{si::meter_unit, 5000.0};
  l1 += si::kilometer<>{2.0};
  EXPECT_FLOAT_EQ(l1.get_value_unsafe(), 7000.0);

  const length_holder<> l2{si::meter_unit, 5000.0};
  const length_holder<> l3 = l2 + si::kilometer<>{3.0};
  EXPECT_FLOAT_EQ(l3.get_value_unsafe(), 8000.0);

  si::meter<> m4{3000.0};
  m4 += length_holder<>{si::kilometer_unit, 2.0};
  EXPECT_FLOAT_EQ(m4.get_value_unsafe(), 5000.0);

  const si::kilometer<> m5{5.0};
  const si::meter<> m6 = m5 + length_holder<>{si::meter_unit, 3000.0};
  EXPECT_FLOAT_EQ(m6.get_value_unsafe(), 8000.0);

  temperature_holder<> t1{si::celsius_unit, 100.0};
  EXPECT_THROW(t1 += us::fahrenheit<>{212.0}, incompatible_quantity_holder);
}

TEST(TestQuantityHolder, TestMixedSubtraction) {
  length_holder<> l1{si::meter_unit, 5000.0};
  l1 -= si::kilometer<>{2.0};
  EXPECT_FLOAT_EQ(l1.get_value_unsafe(), 3000.0);

  const length_holder<> l2{si::meter_unit, 5000.0};
  const length_holder<> l3 = l2 - si::kilometer<>{3.0};
  EXPECT_FLOAT_EQ(l3.get_value_unsafe(), 2000.0);

  si::meter<> m4{5000.0};
  m4 -= length_holder<>{si::kilometer_unit, 2.0};
  EXPECT_FLOAT_EQ(m4.get_value_unsafe(), 3000.0);

  const si::kilometer<> m5{5.0};
  const si::meter<> m6 = m5 - length_holder<>{si::meter_unit, 3000.0};
  EXPECT_FLOAT_EQ(m6.get_value_unsafe(), 2000.0);

  temperature_holder<> t1{si::celsius_unit, 100.0};
  EXPECT_THROW(t1 -= us::fahrenheit<>{32.0}, incompatible_quantity_holder);
}

TEST(TestQuantityHolder, TestMixedMultiplication) {
  const length_holder<> l1{si::centimeter_unit, 5.0};
  const area_holder<> a1 = l1 * si::meter<>{2.0};
  EXPECT_FLOAT_EQ(a1.get_value_unsafe(), 10.0);
  EXPECT_FLOAT_EQ(a1.get_multiplier(), 1e2);
  EXPECT_FLOAT_EQ(a1.get_reference(), 0.0);

  const area_holder<> a2 = si::meter<>{2.0} * l1;
  EXPECT_FLOAT_EQ(a2.get_value_unsafe(), 10.0);
  EXPECT_FLOAT_EQ(a2.get_multiplier(), 1e2);
  EXPECT_FLOAT_EQ(a2.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestComparisonOperators) {
  length_holder<> l1{si::meter_unit, 5000.0};
  length_holder<> l2{si::kilometer_unit, 5.0};
  length_holder<> l3{si::meter_unit, 3000.0};

  EXPECT_TRUE(l1 == l2);
  EXPECT_FALSE(l1 != l2);
  EXPECT_TRUE(l3 < l1);
  EXPECT_TRUE(l3 <= l1);
  EXPECT_FALSE(l3 > l1);
  EXPECT_FALSE(l3 >= l1);
}