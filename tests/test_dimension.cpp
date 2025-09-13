#include "Maxwell.hpp"
#include "core/dimension.hpp"
#include "core/quantity.hpp"
#include "utility/compile_time_math.hpp"

#include <gtest/gtest.h>
#include <tuple>

using namespace maxwell;

TEST(TestDimensions, TestDimensionEquality) {
  constexpr dimension_type<"A", utility::rational_type<1, 1>{}> d1;
  constexpr dimension_type<"A", utility::rational_type<2, 1>{}> d2;
  constexpr dimension_type<"B", utility::rational_type<1, 1>{}> d3;

  EXPECT_EQ(d1, d1);
  EXPECT_NE(d1, d2);
  EXPECT_NE(d1, d3);
  EXPECT_EQ(d2, d2);
  EXPECT_NE(d2, d3);
}

TEST(TestDimensions, TestDimensionConcept) {
  using test_type = dimension_type<"A", utility::rational_type<1, 1>{}>;

  EXPECT_FALSE(dimension<int>);
  EXPECT_TRUE(dimension<test_type>);
  EXPECT_TRUE(dimension<const test_type>);
  EXPECT_TRUE(dimension<test_type&>);
  EXPECT_TRUE(dimension<const test_type&>);
  EXPECT_TRUE(dimension<test_type&&>);
  EXPECT_TRUE(dimension<const test_type&&>);
}

TEST(TestDimensions, TestDimensionInverse) {
  using test_type = dimension_type<"A", utility::rational_type<1, 1>{}>;
  using inverse = dimension_inverse_t<test_type>;

  EXPECT_EQ(inverse::name, utility::template_string{"A"});
  EXPECT_EQ(inverse::power, (utility::rational_type<-1, 1>{}));

  using inverse_inverse = dimension_inverse_t<inverse>;

  EXPECT_EQ(inverse_inverse{}, test_type{});
}

TEST(TestDimensions, TestDimensionMultiplication) {
  constexpr dimension_type<"A", utility::rational_type<1, 1>{}> d1;
  constexpr dimension_type<"B", utility::rational_type<2, 1>{}> d2;
  constexpr dimension_type<"A", utility::rational_type<-1, 1>{}> d3;

  constexpr dimension_product auto prod1 = d1 * d2;
  constexpr dimension_product auto prod2 = d1 * d1;
  constexpr dimension_product auto prod3 = d1 * d3;

  const auto prod1_tuple = prod1.as_tuple();
  const auto prod2_tuple = prod2.as_tuple();
  const auto prod3_tuple = prod3.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod1_tuple)>, 2);
  EXPECT_EQ(std::get<0>(prod1_tuple),
            (dimension_type<"A", utility::rational_type<1, 1>{}>{}));
  EXPECT_EQ(std::get<1>(prod1_tuple),
            (dimension_type<"B", utility::rational_type<2, 1>{}>{}));

  EXPECT_EQ(std::tuple_size_v<decltype(prod2_tuple)>, 1);
  EXPECT_EQ(std::get<0>(prod2_tuple),
            (dimension_type<"A", utility::rational_type<2, 1>{}>{}));

  EXPECT_EQ(std::tuple_size_v<decltype(prod3_tuple)>, 0);

  constexpr dimension_product auto prod4 = dimension_one * dimension_one;

  const auto prod4_tuple = prod4.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod4_tuple)>, 1);
  EXPECT_EQ(std::get<0>(prod4_tuple),
            (dimension_type<number_kind, utility::rational_type<2, 1>{}>{}));
}

TEST(TestDimensions, TestDimensionDivision) {
  constexpr dimension_type<"A", utility::rational_type<1, 1>{}> d1;
  constexpr dimension_type<"B", utility::rational_type<2, 1>{}> d2;
  constexpr dimension_type<"A", utility::rational_type<-1, 1>{}> d3;

  constexpr dimension_product auto quot1 = d1 / d2;
  constexpr dimension_product auto quot2 = d1 / d1;
  constexpr dimension_product auto quot3 = d1 / d3;

  const auto quot1_tuple = quot1.as_tuple();
  const auto quot2_tuple = quot2.as_tuple();
  const auto quot3_tuple = quot3.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot1_tuple)>, 2);

  EXPECT_EQ(std::get<0>(quot1_tuple),
            (dimension_type<"A", utility::rational_type<1, 1>{}>{}));
  EXPECT_EQ(std::get<1>(quot1_tuple),
            (dimension_type<"B", utility::rational_type<-2, 1>{}>{}));

  EXPECT_EQ(std::tuple_size_v<decltype(quot2_tuple)>, 0);

  EXPECT_EQ(std::tuple_size_v<decltype(quot3_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot3_tuple),
            (dimension_type<"A", utility::rational_type<2, 1>{}>{}));

  constexpr dimension_type<number_kind, utility::rational_type<2, 1>{}> d4;

  const auto quot4 = d4 / dimension_one;
  const auto quot4_tuple = quot4.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot4_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot4_tuple),
            (dimension_type<number_kind, utility::one>{}));
}

TEST(TestDimensions, TestDimensionProductConcept) {
  using test_type = dimension_product_type<>;

  EXPECT_FALSE(dimension_product<int>);
  EXPECT_TRUE(dimension_product<test_type>);
  EXPECT_TRUE(dimension_product<const test_type>);
  EXPECT_TRUE(dimension_product<test_type&>);
  EXPECT_TRUE(dimension_product<const test_type&>);
  EXPECT_TRUE(dimension_product<test_type&&>);
  EXPECT_TRUE(dimension_product<const test_type&&>);
}

TEST(TestDimensions, TestDimensionProductSquareRoot) {
  constexpr dimension_product_type<
      dimension_type<"A", utility::rational_type<2, 1>{}>,
      dimension_type<"B", utility::rational_type<3, 1>{}>>
      dim_prod;

  constexpr dimension_product auto sqrt = dim_prod.sqrt();
  const auto sqrt_tuple = sqrt.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(sqrt_tuple)>, 2);
  EXPECT_EQ(std::get<0>(sqrt_tuple), (dimension_type<"A", utility::one>{}));
  EXPECT_EQ(std::get<1>(sqrt_tuple),
            (dimension_type<"B", utility::rational_type<3, 2>{}>{}));
}

TEST(TestDimensions, TestDimensionProductPower) {
  constexpr dimension_product_type<dimension_type<"A", rational<1, 2>>,
                                   dimension_type<"B", rational<3, 2>>>
      dim_prod;

  constexpr dimension_product auto pow2 =
      dim_prod.template pow<utility::rational_type<2, 1>{}>();
  const auto pow2_tuple = pow2.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(pow2_tuple)>, 2);
  EXPECT_EQ(std::get<0>(pow2_tuple), (dimension_type<"A", utility::one>{}));
  EXPECT_EQ(std::get<1>(pow2_tuple),
            (dimension_type<"B", utility::rational_type<3, 1>{}>{}));

  constexpr dimension_product auto pow3 =
      dim_prod.template pow<utility::rational_type<3, 1>{}>();
  const auto pow3_tuple = pow3.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(pow3_tuple)>, 2);
  EXPECT_EQ(std::get<0>(pow3_tuple),
            (dimension_type<"A", utility::rational_type<3, 2>{}>{}));
  EXPECT_EQ(std::get<1>(pow3_tuple),
            (dimension_type<"B", utility::rational_type<9, 2>{}>{}));
}

TEST(TestDimensions, TestDimensionProductEquality) {
  constexpr dimension_product_type<> d1;
  dimension_product_type<> d2;
  constexpr dimension_product_type<
      dimension_type<"A", utility::rational_type<1, 1>{}>>
      d3;
  constexpr dimension_product_type<
      dimension_type<"A", utility::rational_type<2, 1>{}>>
      d4;
  constexpr dimension_product_type<
      dimension_type<"B", utility::rational_type<1, 1>{}>>
      d5;
  constexpr dimension_product_type<
      dimension_type<"A", utility::rational_type<1, 1>{}>,
      dimension_type<"B", utility::rational_type<1, 1>{}>>
      d6;

  EXPECT_EQ(d1, d1);
  EXPECT_EQ(d1, d2);
  EXPECT_NE(d1, d3);
  EXPECT_NE(d1, d4);
  EXPECT_NE(d1, d5);
  EXPECT_NE(d1, d6);
  EXPECT_EQ(d2, d2);
  EXPECT_NE(d2, d3);
  EXPECT_NE(d2, d4);
  EXPECT_NE(d2, d5);
  EXPECT_NE(d2, d6);
  EXPECT_EQ(d3, d3);
  EXPECT_NE(d3, d4);
  EXPECT_NE(d3, d5);
  EXPECT_NE(d3, d6);
  EXPECT_EQ(d4, d4);
  EXPECT_NE(d4, d5);
  EXPECT_NE(d4, d6);
  EXPECT_EQ(d5, d5);
  EXPECT_NE(d5, d6);
  EXPECT_EQ(d6, d6);
}

TEST(TestDimensions, TestDimensionProductMultiplication) {
  const dimension_product_type<> dim_prod1;

  const auto prod1 = dim_prod1 * dim_prod1;
  const auto prod1_tuple = prod1.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod1_tuple)>, 0);

  const dimension_type<"A", utility::one> dim;
  const dimension_product_type<dimension_type<"A", utility::one>,
                               dimension_type<"B", utility::one>>
      dim_prod2;

  const auto prod2 = dim * dim_prod2;
  const auto prod2_tuple = prod2.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod2_tuple)>, 2);
  EXPECT_EQ(std::get<0>(prod2_tuple),
            (dimension_type<"A", utility::rational_type<2, 1>{}>{}));
  EXPECT_EQ(std::get<1>(prod2_tuple), (dimension_type<"B", utility::one>{}));

  const auto prod3 = dim_prod2 * dim;
  const auto prod3_tuple = prod3.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod3_tuple)>, 2);
  EXPECT_EQ(std::get<0>(prod3_tuple),
            (dimension_type<"A", utility::rational_type<2, 1>{}>{}));
  EXPECT_EQ(std::get<1>(prod3_tuple), (dimension_type<"B", utility::one>{}));

  const auto prod4 = dim * dim_prod1;
  const auto prod4_tuple = prod4.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod4_tuple)>, 1);
  EXPECT_EQ(std::get<0>(prod4_tuple), (dimension_type<"A", utility::one>{}));

  const auto prod5 = dim_prod1 * dim;
  const auto prod5_tuple = prod5.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod5_tuple)>, 1);
  EXPECT_EQ(std::get<0>(prod5_tuple), (dimension_type<"A", utility::one>{}));

  const dimension_product_type<
      dimension_type<"A", utility::rational_type<-1, 1>{}>>
      dim_prod3;

  const auto prod6 = dim_prod2 * dim_prod3;
  const auto prod6_tuple = prod6.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod6_tuple)>, 1);
  EXPECT_EQ(std::get<0>(prod6_tuple), (dimension_type<"B", utility::one>{}));

  const dimension_product_type<
      dimension_type<"A", utility::rational_type<1, 1>{}>,
      dimension_type<"C", utility::rational_type<1, 1>{}>>
      dim_prod4;
  const dimension_product_type<
      dimension_type<"B", utility::rational_type<1, 1>{}>,
      dimension_type<"D", utility::rational_type<1, 1>{}>>
      dim_prod5;

  const auto prod7 = dim_prod4 * dim_prod5;
  const auto prod7_tuple = prod7.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod7_tuple)>, 4);
  EXPECT_EQ(std::get<0>(prod7_tuple), (dimension_type<"A", utility::one>{}));
  EXPECT_EQ(std::get<1>(prod7_tuple), (dimension_type<"B", utility::one>{}));
  EXPECT_EQ(std::get<2>(prod7_tuple), (dimension_type<"C", utility::one>{}));
  EXPECT_EQ(std::get<3>(prod7_tuple), (dimension_type<"D", utility::one>{}));

  const dimension_product_type<dimension_type<"A", utility::one>,
                               dimension_type<"E", utility::one>>
      dim_prod6;

  const auto prod8 = prod7 * dim_prod6;
  const auto prod8_tuple = prod8.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(prod8_tuple)>, 5);
  EXPECT_EQ(std::get<0>(prod8_tuple),
            (dimension_type<"A", utility::rational_type<2, 1>{}>{}));
  EXPECT_EQ(std::get<1>(prod8_tuple), (dimension_type<"B", utility::one>{}));
  EXPECT_EQ(std::get<2>(prod8_tuple), (dimension_type<"C", utility::one>{}));
  EXPECT_EQ(std::get<3>(prod8_tuple), (dimension_type<"D", utility::one>{}));
  EXPECT_EQ(std::get<4>(prod8_tuple), (dimension_type<"E", utility::one>{}));

  const dimension_product_type<dimension_type<"A", utility::zero>> dim_prod7;
  const auto prod9 = dim_prod7 * dim_prod7;

  const auto prod9_tuple = prod9.as_tuple();
  EXPECT_EQ(std::tuple_size_v<decltype(prod9_tuple)>, 0);
}

TEST(TestDimensions, TestDimensionProductDivision) {
  const dimension_product_type<> dim_prod1;

  const auto quot1 = dim_prod1 / dim_prod1;
  const auto quot1_tuple = quot1.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot1_tuple)>, 0);

  const dimension_type<"A", utility::one> dim;
  const dimension_product_type<dimension_type<"A", utility::one>,
                               dimension_type<"B", utility::one>>
      dim_prod2;

  const auto quot2 = dim / dim_prod2;
  const auto quot2_tuple = quot2.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot2_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot2_tuple),
            (dimension_type<"B", utility::rational_type<-1, 1>{}>{}));

  const auto quot3 = dim_prod2 / dim;
  const auto quot3_tuple = quot3.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot3_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot3_tuple),
            (dimension_type<"B", utility::rational_type<1, 1>{}>{}));

  const auto quot4 = dim / dim_prod1;
  const auto quot4_tuple = quot4.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot4_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot4_tuple), (dimension_type<"A", utility::one>{}));

  const auto quot5 = dim_prod1 / dim;
  const auto quot5_tuple = quot5.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot5_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot5_tuple),
            (dimension_type<"A", utility::rational_type<-1, 1>{}>{}));

  const dimension_product_type<dimension_type<"A", utility::one>,
                               dimension_type<"B", utility::one>>
      dim_prod3;

  const auto quot6 = dim_prod2 / dim_prod3;
  const auto quot6_tuple = quot6.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot6_tuple)>, 0);

  const dimension_product_type<
      dimension_type<"A", utility::rational_type<1, 1>{}>,
      dimension_type<"C", utility::rational_type<1, 1>{}>>
      dim_prod4;
  const dimension_product_type<
      dimension_type<"B", utility::rational_type<1, 1>{}>,
      dimension_type<"D", utility::rational_type<1, 1>{}>>
      dim_prod5;

  const auto quot7 = dim_prod4 / dim_prod5;
  const auto quot7_tuple = quot7.as_tuple();

  EXPECT_EQ(std::tuple_size_v<decltype(quot7_tuple)>, 4);
  EXPECT_EQ(std::get<0>(quot7_tuple), (dimension_type<"A", utility::one>{}));
  EXPECT_EQ(std::get<1>(quot7_tuple),
            (dimension_type<"B", utility::rational_type<-1, 1>{}>{}));
  EXPECT_EQ(std::get<2>(quot7_tuple), (dimension_type<"C", utility::one>{}));
  EXPECT_EQ(std::get<3>(quot7_tuple),
            (dimension_type<"D", utility::rational_type<-1, 1>{}>{}));
}