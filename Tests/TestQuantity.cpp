#include <gtest/gtest.h>

#include "Maxwell.hh"

#include "QuantityConcepts.hh"
#include "UDArithmeticTypes.hpp"

using namespace Maxwell;
using namespace Maxwell::QuantityLiterals;

TEST(TestQuantity, TestQuantityDefaultConstructor)
{
    // Test using built-in type
    EXPECT_TRUE((std::is_nothrow_default_constructible_v<
                 BasicQuantity<double, MeterUnit>>));
    BasicQuantity<double, MeterUnit> q;
    EXPECT_FLOAT_EQ(q.value(), double());
    EXPECT_EQ(q.units(), MeterUnit);

    // Test using custom NothrowArithmetic type
    EXPECT_TRUE(NothrowArithmetic<Type1>);
    EXPECT_TRUE((std::is_nothrow_default_constructible_v<
                 BasicQuantity<Type1, MeterUnit>>));
    BasicQuantity<Type1, MeterUnit> q2;
    EXPECT_EQ(q2.value(), Type1{});
    EXPECT_EQ(q2.units(), MeterUnit);

    // Test using custom ArithmeticType
    EXPECT_TRUE(Arithmetic<Type2> && NothrowArithmetic<Type2>);
    EXPECT_TRUE(
        (std::is_default_constructible_v<BasicQuantity<Type2, MeterUnit>>));
    EXPECT_FALSE((std::is_nothrow_default_constructible_v<
                  BasicQuantity<Type2, MeterUnit>>));
    BasicQuantity<Type2, MeterUnit> q3;
    EXPECT_EQ(q3.value(), Type2{});
    EXPECT_EQ(q3.units(), MeterUnit);

    // Test using quantity in constant expressions
    constexpr BasicQuantity<double, MeterUnit> cq1;
    EXPECT_EQ(cq1.value(), double());
    EXPECT_EQ(cq1.units(), MeterUnit);
}

TEST(TestQuantity, TestValueConstructor) {}