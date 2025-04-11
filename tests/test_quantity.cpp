#include "test_quantity.hpp"
#include "internal/quantity_repo.hpp"

#include <concepts>
#include <type_traits>
#include <vector>

#include <Maxwell.hpp>
#include <gtest/gtest.h>

using namespace maxwell;

TEST(TestQuantity, TestSizeAndAlignment) {
  EXPECT_EQ(sizeof(quantity<meter_unit, double>), sizeof(double));
  EXPECT_EQ(alignof(quantity<meter_unit, double>), alignof(double));
}

TEST(TestQuantity, TestDefaultConstructor) {
  quantity<meter_unit, double> q;
  EXPECT_EQ(q.get_magnitude(), double());
  EXPECT_TRUE((std::is_nothrow_default_constructible_v<quantity<meter_unit, double>>));

  EXPECT_TRUE((is_constant_expression([] { quantity<meter_unit, double>{}; })));

  quantity<meter_unit, throwing_noisy> q2;
  EXPECT_EQ(q2.get_magnitude().value, 0.0);
  EXPECT_FALSE((std::is_nothrow_default_constructible_v<quantity<meter_unit, throwing_noisy>>));
}

TEST(TestQuantity, TestForwardingConstructor) {
  const nothrow_noisy nn{1.0};
  quantity<meter_unit, nothrow_noisy> q1{nn};

  EXPECT_FALSE((std::convertible_to<nothrow_noisy, quantity<meter_unit, nothrow_noisy>>));
  EXPECT_TRUE((std::convertible_to<nothrow_noisy, quantity<scalar_unit, nothrow_noisy>>));

  EXPECT_TRUE((is_constant_expression([] { quantity<meter_unit, literal>{literal{}}; })));
  EXPECT_TRUE((is_constant_expression([] {
    literal l;
    quantity<meter_unit, literal>{l};
  })));

  EXPECT_EQ(q1.get_magnitude().value, 1.0);
  EXPECT_EQ(nothrow_noisy::num_default_ctor_calls, 0);
  EXPECT_EQ(nothrow_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(nothrow_noisy::num_move_ctor_calls, 0);
  EXPECT_TRUE((std::is_nothrow_constructible_v<quantity<meter_unit, nothrow_noisy>, const nothrow_noisy&>));

  quantity<meter_unit, nothrow_noisy> q2(nothrow_noisy{2.0});
  EXPECT_EQ(q2.get_magnitude().value, 2.0);
  EXPECT_EQ(nothrow_noisy::num_default_ctor_calls, 0);
  EXPECT_EQ(nothrow_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(nothrow_noisy::num_move_ctor_calls, 1);
  EXPECT_TRUE((std::is_nothrow_constructible_v<quantity<meter_unit, nothrow_noisy>, nothrow_noisy&&>));

  const throwing_noisy tn{1.0};
  quantity<meter_unit, throwing_noisy> q3{tn};
  EXPECT_EQ(q3.get_magnitude().value, 1.0);
  EXPECT_EQ(throwing_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(throwing_noisy::num_move_ctor_calls, 0);
  EXPECT_FALSE((std::is_nothrow_constructible_v<quantity<meter_unit, throwing_noisy>, const throwing_noisy&>));

  quantity<meter_unit, throwing_noisy> q4(throwing_noisy{2.0});
  EXPECT_EQ(q4.get_magnitude().value, 2.0);
  EXPECT_EQ(throwing_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(throwing_noisy::num_move_ctor_calls, 1);
  EXPECT_FALSE((std::is_nothrow_constructible_v<quantity<meter_unit, throwing_noisy>, throwing_noisy&&>));

  quantity<meter_unit, std::vector<double>> q5{{1.0, 2.0, 3.0, 4.0}};
  std::vector<double> test{1.0, 2.0, 3.0, 4.0};
  EXPECT_EQ(q5.get_magnitude(), test);
}

TEST(TestQuantity, TestInPlaceConstructor) {
  quantity<meter_unit, nothrow_noisy> q1{std::in_place, 1.0, 2.0};
  EXPECT_EQ(q1.get_magnitude().value, 3.0);
  EXPECT_TRUE((std::is_nothrow_constructible_v<quantity<meter_unit, nothrow_noisy>, std::in_place_t, double, double>));
  EXPECT_TRUE(is_constant_expression([] { quantity<meter_unit, nothrow_noisy>{std::in_place, 1.0, 2.0}; }));

  quantity<meter_unit, throwing_noisy> q2{std::in_place, 1.0, 2.0};
  EXPECT_EQ(q2.get_magnitude().value, 3.0);
  EXPECT_FALSE(
      (std::is_nothrow_constructible_v<quantity<meter_unit, throwing_noisy>, std::in_place_t, double, double>));

  quantity<meter_unit, nothrow_noisy> q3{std::in_place, {1.0, 2.0, 3.0, 4.0}, 5.0};
  EXPECT_EQ(q3.get_magnitude().value, 15.0);
  EXPECT_TRUE((std::is_nothrow_constructible_v<quantity<meter_unit, nothrow_noisy>, std::in_place_t,
                                               std::initializer_list<double>, double>));
  EXPECT_TRUE(
      is_constant_expression([] { quantity<meter_unit, nothrow_noisy>{std::in_place, {1.0, 2.0, 3.0, 4.0}, 5.0}; }));

  quantity<meter_unit, throwing_noisy> q4{std::in_place, {1.0, 2.0, 3.0, 4.0}, 5.0};
  EXPECT_EQ(q4.get_magnitude().value, 15.0);
  EXPECT_FALSE((std::is_nothrow_constructible_v<quantity<meter_unit, throwing_noisy>, std::in_place_t,
                                                std::initializer_list<double>, double>));
}

TEST(TestQuantity, TestUnitConvertingConstructorAmount) {
  mole m{1.0};
  nanomole nm{m};
  EXPECT_FLOAT_EQ(nm.get_magnitude(), 1.0e9);
  kilomole km{m};
  EXPECT_FLOAT_EQ(km.get_magnitude(), 1e-3);

  mole m2{nm};
  EXPECT_FLOAT_EQ(m2.get_magnitude(), 1.0);

  mole m3{km};
  EXPECT_FLOAT_EQ(m3.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestUnitConvertingConstructorCurrent) {
  ampere a{1.0};
  nanoampere na{a};
  EXPECT_FLOAT_EQ(na.get_magnitude(), 1.0e9);
  kiloampere ka{a};
  EXPECT_FLOAT_EQ(ka.get_magnitude(), 1e-3);

  ampere a2{na};
  EXPECT_FLOAT_EQ(a2.get_magnitude(), 1.0);

  ampere a3{ka};
  EXPECT_FLOAT_EQ(a3.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestUnitConvertingConstructorLength) {
  meter m{1.0};
  nanometer nm{m};
  EXPECT_FLOAT_EQ(nm.get_magnitude(), 1.0e9);
  kilometer km{m};
  EXPECT_FLOAT_EQ(km.get_magnitude(), 1e-3);

  meter m2{nm};
  EXPECT_FLOAT_EQ(m2.get_magnitude(), 1.0);

  meter m3{km};
  EXPECT_FLOAT_EQ(m3.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestUnitConvertingConstructorArea) {
  square_meter sm{1.0};
  square_centimeter cm{sm};

  EXPECT_FLOAT_EQ(cm.get_magnitude(), 1e4);

  square_meter m2{cm};
  EXPECT_FLOAT_EQ(m2.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestGetMagnitude) {
  quantity<meter_unit, double> q1{3.0};

  EXPECT_TRUE((std::same_as<decltype(q1.get_magnitude()), const double&>));
  EXPECT_TRUE((std::same_as<decltype(std::move(q1).get_magnitude()), double&&>));
  EXPECT_TRUE((std::same_as<decltype(std::move(std::as_const(q1)).get_magnitude()), const double&&>));
}

TEST(TestQuantity, TestMagnitudeTypeOperator) {
  EXPECT_FALSE((std::convertible_to<quantity<meter_unit, double>, double>));
  EXPECT_TRUE((std::convertible_to<quantity<scalar_unit, double>, double>));

  quantity<meter_unit, double> q1{3.0};
  double d1 = static_cast<double>(q1);

  quantity<scalar_unit, double> q2{3.0};
  double s2 = q2;

  EXPECT_FLOAT_EQ(d1, 3.0);
  EXPECT_FLOAT_EQ(s2, 3.0);
}

TEST(TestQuantity, TestAmountConcept) {
  EXPECT_TRUE(amount<mole>);
  EXPECT_TRUE(amount<quettamole>);
}

TEST(TestQuantity, TestCurrentConcept) {
  EXPECT_TRUE(current<ampere>);
  EXPECT_TRUE(current<quettaampere>);
}

TEST(TestQuantity, TestLengthConcept) {
  EXPECT_TRUE(length<meter>);
  EXPECT_TRUE(length<quettameter>);
}

TEST(TestQuantity, TestLuminosityConcept) {
  EXPECT_TRUE(luminosity<candela>);
  EXPECT_TRUE(luminosity<quettacandela>);
}

TEST(TestQuantity, TestMassConcept) {
  EXPECT_TRUE(mass<gram>);
  EXPECT_TRUE(mass<quettagram>);
}

TEST(TestQuantity, TestTemperatureConcept) {
  EXPECT_TRUE(temperature<kelvin>);
  EXPECT_TRUE(temperature<quettakelvin>);
}

TEST(TestQuantity, TestTimeConcept) { EXPECT_TRUE(maxwell::time<second>); }

TEST(TestQuantity, TestAngleConcept) {
  EXPECT_TRUE(angle<radian>);
  EXPECT_TRUE(angle<degree>);
}