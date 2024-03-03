#include <gtest/gtest.h>

#include "Maxwell.hh"

#include "QuantityConcepts.hh"
#include "UDArithmeticTypes.hpp"
#include "UnitTypes.hh"

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

TEST(TestQuantity, TestValueConstructor)
{
    BasicQuantity<double, MeterUnit> q1{10};
    EXPECT_EQ(q1.value(), 10.0);
    EXPECT_EQ(q1.units(), MeterUnit);

    int baseCopyCtorCols = Type1::copyCtorCalls;
    int baseMoveCtorCols = Type1::moveCtorCalls;

    Type1                           t{10.0};
    BasicQuantity<Type1, MeterUnit> q2{t};
    EXPECT_EQ(q2.value().val(), 10.0);
    EXPECT_EQ(q2.units(), MeterUnit);
    EXPECT_EQ(Type1::copyCtorCalls, baseCopyCtorCols + 1);
    EXPECT_EQ(Type1::moveCtorCalls, baseMoveCtorCols);

    baseCopyCtorCols = Type1::copyCtorCalls;

    BasicQuantity<Type1, MeterUnit> q3{Type1{10.0}};
    EXPECT_EQ(q3.value().val(), 10.0);
    EXPECT_EQ(q3.units(), MeterUnit);
    EXPECT_TRUE((noexcept(BasicQuantity<Type1, MeterUnit>{Type1{10.0}})));
    EXPECT_EQ(Type1::copyCtorCalls, baseCopyCtorCols);
    EXPECT_EQ(Type1::moveCtorCalls, baseMoveCtorCols + 1);

    EXPECT_FALSE((noexcept(BasicQuantity<Type2, MeterUnit>{Type2{10.0}})));
}

TEST(TestQuantity, TestConveringConstructorPrefix) {}