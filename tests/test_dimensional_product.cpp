#include "dimensional_product.hpp"

#include "gtest/gtest.h"
#include <gtest/gtest.h>

using namespace maxwell;

TEST(TestDimensionalProduct, TestDimensionEquality) {
  const dimension_type<std::ratio<1>, "A"> d1;
  const dimension_type<std::ratio<2>, "A"> d2;
  const dimension_type<std::ratio<1>, "B"> d3;

  EXPECT_EQ(d1, d1);
  EXPECT_NE(d1, d2);
  EXPECT_NE(d1, d3);
  EXPECT_NE(d2, d3);
}

TEST(TestDimensionalProduct, TestDimensionConcept) {
  EXPECT_FALSE(dimension<int>);
  EXPECT_TRUE((dimension<dimension_type<std::ratio<1>, "A">>));
  EXPECT_TRUE((dimension<const dimension_type<std::ratio<1>, "A">>));
  EXPECT_TRUE((dimension<const dimension_type<std::ratio<1>, "A">&>));
  EXPECT_TRUE((dimension<const dimension_type<std::ratio<1>, "A">&&>));
  EXPECT_TRUE((dimension<dimension_type<std::ratio<1>, "A">&&>));
}

TEST(TestDimensionProduct, TestDimensionInverse) {
  using test_type = dimension_type<std::ratio<2>, "A">;
  using inverse = dimension_inverse_t<test_type>;

  EXPECT_TRUE((std::ratio_equal_v<inverse::power, std::ratio<-2>>));
  EXPECT_EQ(inverse::name, test_type::name);

  using inverse_inverse = dimension_inverse_t<inverse>;

  EXPECT_TRUE((std::ratio_equal_v<inverse_inverse::power, std::ratio<2>>));
  EXPECT_EQ(inverse_inverse::name, test_type::name);
}

TEST(TestDimensionProduct, TestDimensionMultiplication) {
  const dimension_type<std::ratio<1>, "A"> d1;
  const dimension_type<std::ratio<2>, "B"> d2;

  const dimension_product auto prod1 = d1 * d2;
  const dimension_product auto prod2 = d2 * d1;
  const dimension_product auto prod3 = d1 * d1;

  const auto prod1_tuple = decltype(prod1)::as_tuple{};
  const auto prod2_tuple = decltype(prod2)::as_tuple{};
  const auto prod3_tuple = decltype(prod3)::as_tuple{};

  EXPECT_EQ(std::tuple_size_v<decltype(prod1_tuple)>, 2);
  EXPECT_EQ(std::get<0>(prod1_tuple), (dimension_type<std::ratio<1>, "A">{}));
  EXPECT_EQ(std::get<1>(prod1_tuple), (dimension_type<std::ratio<2>, "B">{}));

  EXPECT_EQ(std::tuple_size_v<decltype(prod2_tuple)>, 2);
  EXPECT_EQ(std::get<0>(prod2_tuple), (dimension_type<std::ratio<1>, "A">{}));
  EXPECT_EQ(std::get<1>(prod2_tuple), (dimension_type<std::ratio<2>, "B">{}));

  EXPECT_EQ(std::tuple_size_v<decltype(prod3_tuple)>, 1);
  EXPECT_EQ(std::get<0>(prod3_tuple), (dimension_type<std::ratio<2>, "A">{}));
}

TEST(TestDimensionProduct, TestDimensionDivision) {
  const dimension_type<std::ratio<1>, "A"> d1;
  const dimension_type<std::ratio<2>, "B"> d2;

  const dimension_product auto quot1 = d1 / d2;
  const dimension_product auto quot2 = d2 / d1;
  const dimension_product auto quot3 = d1 / d1;

  const auto quot1_tuple = decltype(quot1)::as_tuple{};
  const auto quot2_tuple = decltype(quot2)::as_tuple{};
  const auto quot3_tuple = decltype(quot3)::as_tuple{};

  EXPECT_EQ(std::tuple_size_v<decltype(quot1_tuple)>, 2);
  EXPECT_EQ(std::get<0>(quot1_tuple), (dimension_type<std::ratio<1>, "A">{}));
  EXPECT_EQ(std::get<1>(quot1_tuple), (dimension_type<std::ratio<-2>, "B">{}));

  EXPECT_EQ(std::tuple_size_v<decltype(quot2_tuple)>, 2);
  EXPECT_EQ(std::get<0>(quot2_tuple), (dimension_type<std::ratio<-1>, "A">{}));
  EXPECT_EQ(std::get<1>(quot2_tuple), (dimension_type<std::ratio<2>, "B">{}));

  EXPECT_EQ(std::tuple_size_v<decltype(quot3_tuple)>, 1);
  EXPECT_EQ(std::get<0>(quot3_tuple), (dimension_type<std::ratio<0>, "A">{}));
}

TEST(TestDimensionProduct, TestDimensionProductMultiplication) {
  const dimension_type<std::ratio<1>, "A"> d1;
  const dimension_type<std::ratio<1>, "B"> d2;
  const dimension_type<std::ratio<1>, "C"> d3;

  const dimension_product auto prod1 = d1 * d2; // A*B
  const dimension_product auto prod2 = d1 * d3; // A*C

  const dimension_product auto prod3 = prod1 * d3; // A*B*C
  const dimension_product auto prod4 = d3 * prod1; // A*B*C
  const dimension_product auto prod5 = d1 * prod4; // A^2*B*C
  const dimension_product auto prod6 = prod4 * d1; // A^2*B*C

  const auto prod3_tuple = decltype(prod3)::as_tuple{};
  const auto prod4_tuple = decltype(prod4)::as_tuple{};
  const auto prod5_tuple = decltype(prod5)::as_tuple{};
  const auto prod6_tuple = decltype(prod6)::as_tuple{};

  EXPECT_EQ(std::tuple_size_v<decltype(prod3_tuple)>, 3);
  EXPECT_EQ(std::get<0>(prod3_tuple), d1);
  EXPECT_EQ(std::get<1>(prod3_tuple), d2);
  EXPECT_EQ(std::get<2>(prod3_tuple), d3);

  EXPECT_EQ(std::tuple_size_v<decltype(prod4_tuple)>, 3);
  EXPECT_EQ(std::get<0>(prod4_tuple), d1);
  EXPECT_EQ(std::get<1>(prod4_tuple), d2);
  EXPECT_EQ(std::get<2>(prod4_tuple), d3);

  EXPECT_EQ(std::tuple_size_v<decltype(prod5_tuple)>, 3);
  EXPECT_EQ(std::get<0>(prod5_tuple), (dimension_type<std::ratio<2>, "A">{}));
  EXPECT_EQ(std::get<1>(prod5_tuple), d2);
  EXPECT_EQ(std::get<2>(prod5_tuple), d3);

  EXPECT_EQ(std::tuple_size_v<decltype(prod6_tuple)>, 3);
  EXPECT_EQ(std::get<0>(prod6_tuple), (dimension_type<std::ratio<2>, "A">{}));
  EXPECT_EQ(std::get<1>(prod6_tuple), d2);
  EXPECT_EQ(std::get<2>(prod6_tuple), d3);

  const dimension_product auto prod7 = d2 * d3;       // B*C
  const dimension_product auto prod8 = prod7 * prod2; // A * B * C^2
  const auto prod8_tuple = decltype(prod8)::as_tuple{};

  EXPECT_EQ(std::tuple_size_v<decltype(prod8_tuple)>, 3);
  EXPECT_EQ(std::get<0>(prod8_tuple), d1);
  EXPECT_EQ(std::get<1>(prod8_tuple), (dimension_type<std::ratio<1>, "B">{}));
  EXPECT_EQ(std::get<2>(prod8_tuple), (dimension_type<std::ratio<2>, "C">{}));

  const dimension_product auto prod9 = prod2 * prod7;
  const auto prod9_tuple = decltype(prod9)::as_tuple{};
  EXPECT_EQ(std::tuple_size_v<decltype(prod9_tuple)>, 3);
  EXPECT_EQ(std::get<0>(prod9_tuple), d1);
  EXPECT_EQ(std::get<1>(prod9_tuple), (dimension_type<std::ratio<1>, "B">{}));
  EXPECT_EQ(std::get<2>(prod9_tuple), (dimension_type<std::ratio<2>, "C">{}));
}

TEST(TestDimensionProduct, TestDimensionProductDivision) {
  const dimension_type<std::ratio<1>, "A"> d1;
  const dimension_type<std::ratio<1>, "B"> d2;
  const dimension_type<std::ratio<1>, "C"> d3;

  const dimension_product auto quot1 = d1 / d2; // A*B^-1
  const dimension_product auto quot2 = d1 / d3; // A*C^-1

  const dimension_product auto quot3 = quot1 / d3; // A*B^-1*C^-1
  const dimension_product auto quot4 = d3 / quot1; // A^-1*B*C
  const dimension_product auto quot5 = d1 / quot4; // A^2*B^-1*C^-1
  const dimension_product auto quot6 = quot4 / d1; // A^-2*B*C

  const auto quot3_tuple = decltype(quot3)::as_tuple{};
  const auto quot4_tuple = decltype(quot4)::as_tuple{};
  const auto quot5_tuple = decltype(quot5)::as_tuple{};
  const auto quot6_tuple = decltype(quot6)::as_tuple{};

  EXPECT_EQ(std::tuple_size_v<decltype(quot3_tuple)>, 3);
  EXPECT_EQ(std::get<0>(quot3_tuple), d1);
  EXPECT_EQ(std::get<1>(quot3_tuple), (dimension_type<std::ratio<-1>, "B">{}));
  EXPECT_EQ(std::get<2>(quot3_tuple), (dimension_type<std::ratio<-1>, "C">{}));

  EXPECT_EQ(std::tuple_size_v<decltype(quot4_tuple)>, 3);
  EXPECT_EQ(std::get<0>(quot4_tuple), (dimension_type<std::ratio<-1>, "A">{}));
  EXPECT_EQ(std::get<1>(quot4_tuple), d2);
  EXPECT_EQ(std::get<2>(quot4_tuple), d3);

  EXPECT_EQ(std::tuple_size_v<decltype(quot5_tuple)>, 3);
  EXPECT_EQ(std::get<0>(quot5_tuple), (dimension_type<std::ratio<2>, "A">{}));
  EXPECT_EQ(std::get<1>(quot5_tuple), (dimension_type<std::ratio<-1>, "B">{}));
  EXPECT_EQ(std::get<2>(quot5_tuple), (dimension_type<std::ratio<-1>, "C">{}));

  EXPECT_EQ(std::tuple_size_v<decltype(quot6_tuple)>, 3);
  EXPECT_EQ(std::get<0>(quot6_tuple), (dimension_type<std::ratio<-2>, "A">{}));
  EXPECT_EQ(std::get<1>(quot6_tuple), d2);
  EXPECT_EQ(std::get<2>(quot6_tuple), d3);

  const dimension_product auto quot7 = d2 / d3; // B*C^-1
  const auto quot7_tuple = decltype(quot7)::as_tuple{};
  EXPECT_EQ(std::tuple_size_v<decltype(quot7_tuple)>, 2);
  EXPECT_EQ(std::get<0>(quot7_tuple), d2);
  EXPECT_EQ(std::get<1>(quot7_tuple), (dimension_type<std::ratio<-1>, "C">{}));

  const dimension_product auto quot8 = quot7 / quot2; // (B*C^-1) / (A*C^-1)
  const auto quot8_tuple = decltype(quot8)::as_tuple{};

  EXPECT_EQ(std::tuple_size_v<decltype(quot8_tuple)>, 3);
  EXPECT_EQ(std::get<0>(quot8_tuple), (dimension_type<std::ratio<-1>, "A">{}));
  EXPECT_EQ(std::get<1>(quot8_tuple), d2);
  EXPECT_EQ(std::get<2>(quot8_tuple), (dimension_type<std::ratio<0>, "C">{}));
}

TEST(TestDimensionProduct, TestDimensionProductEquality) {
  const dimension_product_type<dimension_type<std::ratio<1>, "A">,
                               dimension_type<std::ratio<0>, "B">,
                               dimension_type<std::ratio<2>, "C">>
      d1;

  constexpr dimension_product_type<dimension_type<std::ratio<1>, "A">,
                                   dimension_type<std::ratio<2>, "C">>
      d2;
  const dimension_product_type<dimension_type<std::ratio<1>, "A">,
                               dimension_type<std::ratio<2>, "D">>
      d3;

  constexpr dimension_product_type<dimension_type<std::ratio<2>, "A">,
                                   dimension_type<std::ratio<2>, "C">>
      d4;

  EXPECT_EQ(d1, d2);
  EXPECT_EQ(d2, d1);
  EXPECT_NE(d1, d3);
  EXPECT_NE(d1, d4);
  EXPECT_EQ(d2, d2);
  EXPECT_NE(d2, d3);
  EXPECT_NE(d2, d4);
  EXPECT_EQ(d3, d3);
  EXPECT_NE(d3, d4);
  EXPECT_EQ(d4, d4);
}

TEST(TestDimensionProduct, TestDimensionProductConcept) {
  
}
