#include "test_quantity.hpp"

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
}