#include "test_quantity.hpp"
#include "internal/quantity_repo.hpp"
#include "internal/unit_repo.hpp"

#include "gtest/gtest.h"
#include <concepts> // convertible_to
#include <format>
#include <limits> // numeric_limits
#include <list>   // list
#include <memory> // unique_ptr
#include <string>
#include <type_traits> // constructible traits, assignable traits, destructible traits

#include <Maxwell.hpp>

#include <gtest/gtest.h>

using namespace maxwell;

TEST(TestQuantity, TestSizeAndAlignment) {
  EXPECT_EQ(sizeof(quantity<meter_unit, double>), sizeof(double));
  EXPECT_EQ(alignof(quantity<meter_unit, double>), alignof(double));
}

TEST(TestQuantity, TestCppProperties) {
  using copyable = quantity<meter_unit, double>;
  using movable = quantity<meter_unit, std::unique_ptr<double>>;

  EXPECT_TRUE(std::is_default_constructible_v<copyable>);
  EXPECT_TRUE(std::is_copy_constructible_v<copyable>);
  EXPECT_TRUE(std::is_copy_assignable_v<copyable>);
  EXPECT_TRUE(std::is_move_constructible_v<copyable>);
  EXPECT_TRUE(std::is_move_assignable_v<copyable>);
  EXPECT_TRUE(std::is_destructible_v<copyable>);
  EXPECT_TRUE(std::is_trivially_destructible_v<copyable>);

  EXPECT_TRUE(std::is_default_constructible_v<movable>);
  EXPECT_FALSE(std::is_copy_constructible_v<movable>);
  EXPECT_FALSE(std::is_copy_assignable_v<movable>);
  EXPECT_TRUE(std::is_move_constructible_v<movable>);
  EXPECT_TRUE(std::is_move_assignable_v<movable>);
  EXPECT_TRUE(std::is_destructible_v<movable>);
  EXPECT_FALSE(std::is_trivially_destructible_v<movable>);
}

TEST(TestQuantity, TestDefaultConstructor) {
  quantity<meter_unit, double> q;
  EXPECT_EQ(q.get_magnitude(), double());
  EXPECT_TRUE((std::is_nothrow_default_constructible_v<quantity<meter_unit, double>>));

  quantity<meter_unit, throwing_noisy> q2;
  EXPECT_EQ(q2.get_magnitude().value, 0.0);
  EXPECT_FALSE((std::is_nothrow_default_constructible_v<quantity<meter_unit, throwing_noisy>>));
}

TEST(TestQuantity, TestForwardingConstructor) {
  const nothrow_noisy nn{1.0};
  quantity<meter_unit, nothrow_noisy> q1{nn};

  EXPECT_FALSE((std::convertible_to<nothrow_noisy, quantity<meter_unit, nothrow_noisy>>));
  EXPECT_TRUE((std::convertible_to<nothrow_noisy, quantity<scalar_unit, nothrow_noisy>>));

  EXPECT_EQ(q1.get_magnitude().value, 1.0);
  EXPECT_EQ(nothrow_noisy::num_default_ctor_calls, 0);
  EXPECT_EQ(nothrow_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(nothrow_noisy::num_move_ctor_calls, 0);

  quantity<meter_unit, nothrow_noisy> q2(nothrow_noisy{2.0});
  EXPECT_EQ(q2.get_magnitude().value, 2.0);
  EXPECT_EQ(nothrow_noisy::num_default_ctor_calls, 0);
  EXPECT_EQ(nothrow_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(nothrow_noisy::num_move_ctor_calls, 1);

  const throwing_noisy tn{1.0};
  quantity<meter_unit, throwing_noisy> q3{tn};
  EXPECT_EQ(q3.get_magnitude().value, 1.0);
  EXPECT_EQ(throwing_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(throwing_noisy::num_move_ctor_calls, 0);

  quantity<meter_unit, throwing_noisy> q4(throwing_noisy{2.0});
  EXPECT_EQ(q4.get_magnitude().value, 2.0);
  EXPECT_EQ(throwing_noisy::num_copy_ctor_calls, 1);
  EXPECT_EQ(throwing_noisy::num_move_ctor_calls, 1);
}

TEST(TestQuantity, TestInPlaceConstructor) {
  quantity<meter_unit, nothrow_noisy> q1{std::in_place, 1.0, 2.0};
  EXPECT_EQ(q1.get_magnitude().value, 3.0);

  quantity<meter_unit, throwing_noisy> q2{std::in_place, 1.0, 2.0};
  EXPECT_EQ(q2.get_magnitude().value, 3.0);

  quantity<meter_unit, nothrow_noisy> q3{std::in_place, {1.0, 2.0, 3.0, 4.0}, 5.0};
  EXPECT_EQ(q3.get_magnitude().value, 15.0);

  quantity<meter_unit, throwing_noisy> q4{std::in_place, {1.0, 2.0, 3.0, 4.0}, 5.0};
  EXPECT_EQ(q4.get_magnitude().value, 15.0);
}

TEST(TestQuantity, TestChronoConstructor) {
  using namespace std::chrono;
  using namespace std::chrono_literals;

  quantity<second_unit, double> q{1min};
  EXPECT_FLOAT_EQ(q.get_magnitude(), 60.0);

  quantity<minute_unit, double> q2{1s};
  EXPECT_FLOAT_EQ(q2.get_magnitude(), 1.0 / 60.0);

  using from_duration = std::chrono::duration<double, std::ratio<12, 50>>;
  const from_duration from{12};
  quantity<hour_unit, double> q3{from};

  using to_duration = std::chrono::duration<double, std::ratio<3'600, 1>>;
  const auto test_val = std::chrono::duration_cast<to_duration>(from);
  EXPECT_FLOAT_EQ(q3.get_magnitude(), test_val.count());
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

  foot ft{m};
  EXPECT_FLOAT_EQ(ft.get_magnitude(), 3.28084);

  meter mr4{ft};
  EXPECT_FLOAT_EQ(mr4.get_magnitude(), 1.0);

  inch in{nm};
  EXPECT_FLOAT_EQ(in.get_magnitude(), 39.370079);
}

TEST(TestQuantity, TestUnitConvertingConstructorLuminosity) {
  candela cd{1.0};
  nanocandela ncd{cd};
  EXPECT_FLOAT_EQ(ncd.get_magnitude(), 1.0e9);
  kilocandela kcd{cd};
  EXPECT_FLOAT_EQ(kcd.get_magnitude(), 1e-3);

  candela cd2{ncd};
  EXPECT_FLOAT_EQ(cd2.get_magnitude(), 1.0);

  candela cd3{kcd};
  EXPECT_FLOAT_EQ(cd3.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestUnitConvertingConstructorMass) {
  gram g{1.0};
  nanogram ng{g};
  EXPECT_FLOAT_EQ(ng.get_magnitude(), 1.0e9);

  kilogram kg{g};
  EXPECT_FLOAT_EQ(kg.get_magnitude(), 1e-3);

  gram g2{ng};
  EXPECT_FLOAT_EQ(g2.get_magnitude(), 1.0);

  gram g3{kg};
  EXPECT_FLOAT_EQ(g3.get_magnitude(), 1.0);

  ounce oz{kg};
  EXPECT_FLOAT_EQ(oz.get_magnitude(), 0.03527396);
}

TEST(TestQuantity, TestUnitConvertingConstructorTemperature) {
  kelvin k{1.0};
  nanokelvin nk{k};
  EXPECT_FLOAT_EQ(nk.get_magnitude(), 1.0e9);

  kilokelvin kk{k};
  EXPECT_FLOAT_EQ(kk.get_magnitude(), 1e-3);

  kelvin k2{nk};
  EXPECT_FLOAT_EQ(k2.get_magnitude(), 1.0);

  kelvin k3{kk};
  EXPECT_FLOAT_EQ(k3.get_magnitude(), 1.0);

  Celsisus c{k};
  EXPECT_FLOAT_EQ(c.get_magnitude(), -272.15);

  Fahrenheit f{k};
  EXPECT_FLOAT_EQ(f.get_magnitude(), -457.87);

  Celsisus c2{f};
  EXPECT_FLOAT_EQ(c2.get_magnitude(), -272.15);

  kelvin k4{c};
  EXPECT_FLOAT_EQ(k4.get_magnitude(), 1.0);

  kelvin k5{f};
  EXPECT_FLOAT_EQ(k5.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestUnitconvertingConstructorTime) {
  second s{1.0};
  nanosecond ns{s};
  EXPECT_FLOAT_EQ(ns.get_magnitude(), 1e9);

  kilosecond ks{s};
  EXPECT_FLOAT_EQ(ks.get_magnitude(), 1e-3);

  second s2{ns};
  EXPECT_FLOAT_EQ(s2.get_magnitude(), 1.0);

  second s3{ks};
  EXPECT_FLOAT_EQ(s3.get_magnitude(), 1.0);

  hour hr{ns};
  EXPECT_FLOAT_EQ(hr.get_magnitude(), 0.0002777778);

  second s4{hr};
  EXPECT_FLOAT_EQ(s4.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestUnitConvertinConstructorAngle) {
  radian r{1.0};
  degree d{r};

  EXPECT_FLOAT_EQ(d.get_magnitude(), 57.2957795);

  radian r2{d};
  EXPECT_FLOAT_EQ(r2.get_magnitude(), 1.0);
}

TEST(TestQuantity, TestUnitConvertingConstructorArea) {
  square_meter sm{1.0};
  square_centimeter cm{sm};

  EXPECT_FLOAT_EQ(cm.get_magnitude(), 1e4);

  square_meter m2{cm};
  EXPECT_FLOAT_EQ(m2.get_magnitude(), 1.0);
}

template <typename T> class UnitIncompatabilityTest : public ::testing::Test {
public:
  T value_;
};

using test_quantity_types = ::testing::Types<
    std::pair<mole, meter>, std::pair<mole, ampere>, std::pair<mole, meter>, std::pair<mole, candela>,
    std::pair<mole, gram>, std::pair<mole, kelvin>, std::pair<mole, second>, std::pair<mole, radian>,
    std::pair<mole, quantity<scalar_unit, double>>, std::pair<ampere, meter>, std::pair<ampere, candela>,
    std::pair<ampere, gram>, std::pair<ampere, kelvin>, std::pair<ampere, second>, std::pair<ampere, radian>,
    std::pair<ampere, quantity<scalar_unit, double>>, std::pair<meter, candela>, std::pair<meter, gram>,
    std::pair<meter, kelvin>, std::pair<meter, second>, std::pair<meter, radian>,
    std::pair<meter, quantity<scalar_unit, double>>, std::pair<gram, kelvin>, std::pair<gram, second>,
    std::pair<gram, radian>, std::pair<gram, quantity<scalar_unit, double>>, std::pair<kelvin, second>,
    std::pair<kelvin, radian>, std::pair<kelvin, quantity<scalar_unit, double>>, std::pair<second, radian>,
    std::pair<second, quantity<scalar_unit, double>>, std::pair<hertz, quantity<becquerel_unit, double>>,
    std::pair<radian, quantity<steradian_unit, double>>, std::pair<degree, quantity<steradian_unit, double>>>;
TYPED_TEST_SUITE(UnitIncompatabilityTest, test_quantity_types);

TYPED_TEST(UnitIncompatabilityTest, TestUnitIncompatability) {
  using first_type = typename TypeParam::first_type;
  using second_type = typename TypeParam::second_type;

  EXPECT_FALSE((unit_convertible_to<first_type::units, second_type::units>));
  EXPECT_FALSE((unit_convertible_to<second_type::units, first_type::units>));
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

TEST(TestQuantity, TestConstantExpressionUsage) {
  using constant_expression = quantity<meter_unit, double>;
  using constant_expression2 = quantity<meter_unit, nothrow_noisy>;
  using non_constant_expression = quantity<meter_unit, std::list<double>>;

  EXPECT_TRUE(is_constant_expression([] { constant_expression{}; }));
  EXPECT_TRUE(is_constant_expression([] { constant_expression{1.0}; }));
  EXPECT_TRUE(is_constant_expression([] { constant_expression2{std::in_place, 1.0}; }));
  EXPECT_TRUE(is_constant_expression([] { constant_expression2{std::in_place, {1.0, 2.0}, 3.0}; }));
  EXPECT_TRUE(is_constant_expression([] {
    foot f{1.0};
    constant_expression{f};
  }));
  EXPECT_TRUE(is_constant_expression([] { constant_expression{foot{1.0}}; }));

  EXPECT_TRUE(is_constant_expression([] { constant_expression{1.0}.get_magnitude(); }));
  EXPECT_TRUE(is_constant_expression([] {
    constant_expression ce{1.0};
    ce.get_magnitude();
  }));
  EXPECT_TRUE(is_constant_expression([] { constant_expression{1.0}.get_units(); }));

  EXPECT_FALSE(is_constant_expression([] { non_constant_expression{}; }));
}

TEST(TestQuantity, TestUnitAddition) {
  quantity<meter_unit, double> q1{1.0};
  quantity<meter_unit, double> q2{2.0};
  quantity<meter_unit, double> q3{3.0};

  EXPECT_EQ(q1 + q2, q3);
  EXPECT_EQ(q1 += q2, q3);

  quantity<centimeter_unit, double> q4 = q3 + q3;
  EXPECT_FLOAT_EQ(q4.get_magnitude(), 600.0);

  q4 += q2;
  EXPECT_FLOAT_EQ(q4.get_magnitude(), 800.0);

  quantity<mile_unit, double> q5 = quantity<meter_unit, double>{1.0} + quantity<foot_unit, double>{1.0};
  EXPECT_NEAR(q5.get_magnitude(), 0.00081, 1e-5);

  q5 += quantity<meter_unit, double>{1.0} + quantity<foot_unit, double>{1.0};
  EXPECT_NEAR(q5.get_magnitude(), 0.00162, 1e-5);

  quantity<kelvin_unit, double> q7{1.0};
  quantity<Celsisus_unit, double> q8 = q7 + q7;
  EXPECT_FLOAT_EQ(q8.get_magnitude(), -271.15);
}

TEST(TestQuantity, TestUnitSubtraction) {
  quantity<meter_unit, double> q1{1.0};
  quantity<meter_unit, double> q2{2.0};
  quantity<meter_unit, double> q3{3.0};

  EXPECT_EQ(q3 - q2, q1);
  EXPECT_EQ(q3 -= q2, q1);

  quantity<centimeter_unit, double> q4 = q3 - q3;
  EXPECT_FLOAT_EQ(q4.get_magnitude(), 0.0);

  q4 -= q2;
  EXPECT_FLOAT_EQ(q4.get_magnitude(), -200.0);

  quantity<mile_unit, double> q5 = quantity<meter_unit, double>{1.0} - quantity<foot_unit, double>{1.0};
  EXPECT_NEAR(q5.get_magnitude(), 0.00043197725284339455, 1e-5);

  q5 -= quantity<meter_unit, double>{2.0} - quantity<foot_unit, double>{.0};
  EXPECT_NEAR(q5.get_magnitude(), -0.0008107651316312733, 1e-5);

  quantity<kelvin_unit, double> q7{1.0};
  quantity<Celsisus_unit, double> q8 = q7 - q7;
  EXPECT_FLOAT_EQ(q8.get_magnitude(), -273.15);
}

TEST(TestQuantity, TestPrinting) {
  quantity<meter_unit, int> q1{1};
  std::string str = std::format("{}", q1);
  EXPECT_STREQ(str.c_str(), "1 m");

  quantity<square_meter_unit, int> q2 = q1 * q1;
  str = std::format("{}", q2);
  EXPECT_STREQ(str.c_str(), "1 m^2");

  quantity<joule_unit * second_unit, int> q3{100};
  str = std::format("{}", q3);
  EXPECT_STREQ(str.c_str(), "100 J*s");

  quantity<degree_unit / second_unit, int> q4{100};
  str = std::format("{}", q4);
  EXPECT_STREQ(str.c_str(), "100 deg/s");
}

TEST(TestQuantity, TestAmountConcept) {
  EXPECT_TRUE(amount<mole>);
  EXPECT_TRUE(amount<quettamole>);

  EXPECT_FALSE(amount<ampere>);
  EXPECT_FALSE(amount<meter>);
  EXPECT_FALSE(amount<candela>);
  EXPECT_FALSE(amount<gram>);
  EXPECT_FALSE(amount<kelvin>);
  EXPECT_FALSE(amount<second>);
  EXPECT_FALSE(amount<radian>);
  EXPECT_FALSE((amount<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestCurrentConcept) {
  EXPECT_TRUE(current<ampere>);
  EXPECT_TRUE(current<quettaampere>);

  EXPECT_FALSE(current<mole>);
  EXPECT_FALSE(current<meter>);
  EXPECT_FALSE(current<candela>);
  EXPECT_FALSE(current<gram>);
  EXPECT_FALSE(current<kelvin>);
  EXPECT_FALSE(current<second>);
  EXPECT_FALSE(current<radian>);
  EXPECT_FALSE((current<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestLengthConcept) {
  EXPECT_TRUE(length<meter>);
  EXPECT_TRUE(length<quettameter>);
  EXPECT_TRUE(length<inch>);
  EXPECT_TRUE(length<foot>);
  EXPECT_TRUE(length<yard>);
  EXPECT_TRUE(length<mile>);

  EXPECT_FALSE(length<mole>);
  EXPECT_FALSE(length<ampere>);
  EXPECT_FALSE(length<candela>);
  EXPECT_FALSE(length<gram>);
  EXPECT_FALSE(length<kelvin>);
  EXPECT_FALSE(length<second>);
  EXPECT_FALSE(length<radian>);
  EXPECT_FALSE((length<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestLuminosityConcept) {
  EXPECT_TRUE(luminosity<candela>);
  EXPECT_TRUE(luminosity<quettacandela>);

  EXPECT_FALSE(luminosity<mole>);
  EXPECT_FALSE(luminosity<ampere>);
  EXPECT_FALSE(luminosity<meter>);
  EXPECT_FALSE(luminosity<gram>);
  EXPECT_FALSE(luminosity<kelvin>);
  EXPECT_FALSE(luminosity<second>);
  EXPECT_FALSE(luminosity<radian>);
  EXPECT_FALSE((luminosity<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestMassConcept) {
  EXPECT_TRUE(mass<gram>);
  EXPECT_TRUE(mass<quettagram>);

  EXPECT_TRUE(mass<ounce>);
  EXPECT_TRUE(mass<pound>);

  EXPECT_FALSE(mass<mole>);
  EXPECT_FALSE(mass<ampere>);
  EXPECT_FALSE(mass<meter>);
  EXPECT_FALSE(mass<candela>);
  EXPECT_FALSE(mass<kelvin>);
  EXPECT_FALSE(mass<second>);
  EXPECT_FALSE(mass<radian>);
  EXPECT_FALSE((mass<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestTemperatureConcept) {
  EXPECT_TRUE(temperature<kelvin>);
  EXPECT_TRUE(temperature<quettakelvin>);
  EXPECT_TRUE(temperature<Celsisus>);
  EXPECT_TRUE(temperature<Fahrenheit>);

  EXPECT_FALSE(temperature<mole>);
  EXPECT_FALSE(temperature<ampere>);
  EXPECT_FALSE(temperature<meter>);
  EXPECT_FALSE(temperature<candela>);
  EXPECT_FALSE(temperature<gram>);
  EXPECT_FALSE(temperature<second>);
  EXPECT_FALSE(temperature<radian>);
  EXPECT_FALSE((temperature<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestTimeConcept) {
  EXPECT_TRUE(maxwell::time<second>);
  EXPECT_TRUE(maxwell::time<minute>);
  EXPECT_TRUE(maxwell::time<hour>);
  EXPECT_TRUE(maxwell::time<day>);
  EXPECT_TRUE(maxwell::time<year>);

  EXPECT_FALSE(maxwell::time<mole>);
  EXPECT_FALSE(maxwell::time<ampere>);
  EXPECT_FALSE(maxwell::time<meter>);
  EXPECT_FALSE(maxwell::time<candela>);
  EXPECT_FALSE(maxwell::time<gram>);
  EXPECT_FALSE(maxwell::time<kelvin>);
  EXPECT_FALSE(maxwell::time<radian>);
  EXPECT_FALSE((maxwell::time<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestAngleConcept) {
  EXPECT_TRUE(angle<radian>);
  EXPECT_TRUE(angle<degree>);

  EXPECT_FALSE(angle<mole>);
  EXPECT_FALSE(angle<ampere>);
  EXPECT_FALSE(angle<meter>);
  EXPECT_FALSE(angle<candela>);
  EXPECT_FALSE(angle<gram>);
  EXPECT_FALSE(angle<kelvin>);
  EXPECT_FALSE(angle<second>);
  EXPECT_FALSE((angle<quantity<scalar_unit, double>>));
}

TEST(TestQuantity, TestScalarConcept) {
  EXPECT_TRUE((scalar<quantity<scalar_unit, double>>));
  EXPECT_FALSE(scalar<mole>);
  EXPECT_FALSE(scalar<ampere>);
  EXPECT_FALSE(scalar<meter>);
  EXPECT_FALSE(scalar<candela>);
  EXPECT_FALSE(scalar<gram>);
  EXPECT_FALSE(scalar<kelvin>);
  EXPECT_FALSE(scalar<second>);
  EXPECT_FALSE(scalar<radian>);
}

TEST(TestQuantity, TestQuantityNumericLimits) {
  using test_type_1 = quantity<meter_unit, double>;
  using test_type_2 = quantity<meter_unit, const double>;
  using test_type_3 = quantity<meter_unit, std::vector<int>>;

  EXPECT_FALSE(std::numeric_limits<test_type_3>::is_specialized);

  EXPECT_TRUE(std::numeric_limits<test_type_1>::is_specialized);
  EXPECT_TRUE(std::numeric_limits<test_type_2>::is_specialized);
  EXPECT_EQ(std::numeric_limits<test_type_1>::is_signed, std::numeric_limits<double>::is_signed);
  EXPECT_EQ(std::numeric_limits<test_type_1>::is_integer, std::numeric_limits<double>::is_integer);
  EXPECT_EQ(std::numeric_limits<test_type_1>::is_exact, std::numeric_limits<double>::is_exact);
  EXPECT_EQ(std::numeric_limits<test_type_1>::digits, std::numeric_limits<double>::digits);
  EXPECT_EQ(std::numeric_limits<test_type_1>::digits10, std::numeric_limits<double>::digits10);
  EXPECT_EQ(std::numeric_limits<test_type_1>::max_digits10, std::numeric_limits<double>::max_digits10);
  EXPECT_EQ(std::numeric_limits<test_type_1>::has_infinity, std::numeric_limits<double>::has_infinity);
  EXPECT_EQ(std::numeric_limits<test_type_1>::has_quiet_NaN, std::numeric_limits<double>::has_quiet_NaN);
  EXPECT_EQ(std::numeric_limits<test_type_1>::has_signaling_NaN, std::numeric_limits<double>::has_signaling_NaN);
  EXPECT_EQ(std::numeric_limits<test_type_1>::has_denorm, std::numeric_limits<double>::has_denorm);
  EXPECT_EQ(std::numeric_limits<test_type_1>::has_denorm_loss, std::numeric_limits<double>::has_denorm_loss);
  EXPECT_EQ(std::numeric_limits<test_type_1>::round_style, std::numeric_limits<double>::round_style);
  EXPECT_EQ(std::numeric_limits<test_type_1>::is_iec559, std::numeric_limits<double>::is_iec559);
  EXPECT_EQ(std::numeric_limits<test_type_1>::is_bounded, std::numeric_limits<double>::is_bounded);
  EXPECT_EQ(std::numeric_limits<test_type_1>::is_modulo, std::numeric_limits<double>::is_modulo);
  EXPECT_EQ(std::numeric_limits<test_type_1>::traps, std::numeric_limits<double>::traps);
  EXPECT_EQ(std::numeric_limits<test_type_1>::tinyness_before, std::numeric_limits<double>::tinyness_before);
}