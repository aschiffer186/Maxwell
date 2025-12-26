#include "Maxwell.hpp"

#include <gtest/gtest.h>
#include <type_traits>

#include "quantity_systems/si.hpp"
#include "quantity_systems/us.hpp"
#include "test_types.hpp"

using namespace maxwell;
using namespace maxwell::isq;

TEST(TestQuantityHolder, TestCXXProperties) {
  using test_type = length_holder<>;

  EXPECT_EQ(sizeof(test_type), 3 * sizeof(double));
  EXPECT_EQ(alignof(test_type), alignof(double));

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
  EXPECT_EQ(l.get_value(), 0.0);
  EXPECT_EQ(l.get_multiplier(), 1e-3);
  EXPECT_EQ(l.get_reference(), 0.0);

  temperature_holder<> t{si::celsius_unit};
  EXPECT_EQ(t.get_value(), 0.0);
  EXPECT_EQ(t.get_multiplier(), 1.0);
  EXPECT_EQ(t.get_reference(), -273.15);

  EXPECT_TRUE(
      (std::is_nothrow_constructible_v<length_holder<>, si::meter_unit_type>));
  EXPECT_FALSE((std::is_nothrow_constructible_v<length_holder<throwing_tattle>,
                                                si::meter_unit_type>));
}

TEST(TestQuantityHolder, TestValueConstructors) {
  length_holder<> l1{kilo_unit<si::meter_unit>, 5.0};
  EXPECT_EQ(l1.get_value(), 5.0);
  EXPECT_EQ(l1.get_multiplier(), 1e-3);
  EXPECT_EQ(l1.get_reference(), 0.0);

  length_holder<> l2{si::meter_unit, 5000.0};
  EXPECT_EQ(l2.get_value(), 5000.0);
  EXPECT_EQ(l2.get_multiplier(), 1.0);
  EXPECT_EQ(l2.get_reference(), 0.0);

  temperature_holder<> t1{si::celsius_unit, 100.0};
  EXPECT_EQ(t1.get_value(), 100.0);
  EXPECT_EQ(t1.get_multiplier(), 1.0);
  EXPECT_EQ(t1.get_reference(), -273.15);

  temperature_holder<> t2{si::kelvin_unit, 373.15};
  EXPECT_EQ(t2.get_value(), 373.15);
  EXPECT_EQ(t2.get_multiplier(), 1.0);
  EXPECT_EQ(t2.get_reference(), 0.0);
}

TEST(TestQuantityHolder, TestUnitPieceConstructor) {
  quantity_holder<isq::temperature, double> t{10.0, 9.0 / 5.0, -459.67};
  EXPECT_EQ(t.get_value(), 10.0);
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

  EXPECT_FLOAT_EQ(t.get_value().value, 3.0);
  EXPECT_FLOAT_EQ(t.get_multiplier(), us::fahrenheit_unit.multiplier);
  EXPECT_FLOAT_EQ(t.get_reference(), us::fahrenheit_unit.reference);
  EXPECT_EQ(nothrow_tattle::value_ctor_count, value_ctor_count + 1);
  EXPECT_EQ(nothrow_tattle::move_ctor_count, move_ctor_count);
  EXPECT_EQ(nothrow_tattle::copy_ctor_count, copy_ctor_count);
  EXPECT_EQ(nothrow_tattle::il_ctor_count, il_ctor_count);

  test_type t2{us::fahrenheit_unit, std::in_place, {1.0, 2.0, 3.0}};
  EXPECT_FLOAT_EQ(t2.get_value().value, 6.0);
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
  EXPECT_EQ(t1.get_value(), 10.0);
  EXPECT_EQ(t1.get_multiplier(), 1.0);

  time_holder<> t2{std::chrono::nanoseconds{1}};
  EXPECT_EQ(t2.get_value(), 1.0);
  EXPECT_EQ(t2.get_multiplier(), nano_unit<si::second_unit>.multiplier);
}

TEST(TestQuatityHolder, TestQuantityValueConstructor) {
  temperature_holder<> t1{us::fahrenheit<>{32.0}};
  EXPECT_FLOAT_EQ(t1.get_value(), 32.0);
  EXPECT_FLOAT_EQ(t1.get_multiplier(), us::fahrenheit_unit.multiplier);
  EXPECT_FLOAT_EQ(t1.get_reference(), us::fahrenheit_unit.reference);
}

TEST(TestQuantityHolder, TestMixedAddition) {
  // length_holder<> l1{si::meter_unit, 5000.0};
  // l1 += si::kilometer<>{2.0};
  // EXPECT_FLOAT_EQ(l1.get_value(), 7000.0);

  temperature_holder<> t1{si::celsius_unit, 100.0};
  EXPECT_THROW(t1 += us::fahrenheit<>{212.0}, incompatible_quantity_holder);
}

TEST(TestQuantityHolder, TestAsMethod) {
  length_holder<> l{si::meter_unit, 5000.0};
  const kilo<si::meter<>> km = l.as<kilo_unit<si::meter_unit>>();
  EXPECT_FLOAT_EQ(km.get_value(), 5.0);

  temperature_holder<> t{si::kelvin_unit, 300.0};
  const si::celsius<> c = t.as<si::celsius_unit>();
  EXPECT_FLOAT_EQ(c.get_value(), 26.85);

  const us::fahrenheit<> f = t.as<us::fahrenheit_unit>();
  EXPECT_FLOAT_EQ(f.get_value(), 80.33);
}