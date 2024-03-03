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

TEST(TestQuantity, TestConveringConstructorPrefix)
{
    BasicQuantity<double, MoleUnit>      mol1{1};
    BasicQuantity<double, CentimoleUnit> cmol1{mol1};
    EXPECT_FLOAT_EQ(cmol1.value(), 100.0);
    BasicQuantity<double, KilomoleUnit> kmol1(std::move(cmol1));
    EXPECT_FLOAT_EQ(kmol1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CentimoleUnit>(mol1)));

    BasicQuantity<double, AmpereUnit>      amp1{1};
    BasicQuantity<double, CentiampereUnit> camp1{amp1};
    EXPECT_FLOAT_EQ(camp1.value(), 100.0);
    BasicQuantity<double, KiloampereUnit> kamp1(std::move(camp1));
    EXPECT_FLOAT_EQ(kamp1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CentiampereUnit>(amp1)));

    BasicQuantity<double, MeterUnit>      m1{1};
    BasicQuantity<double, CentimeterUnit> cm1{m1};
    EXPECT_FLOAT_EQ(cm1.value(), 100.0);
    BasicQuantity<double, KilometerUnit> km1(std::move(cm1));
    EXPECT_FLOAT_EQ(km1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CentimeterUnit>(m1)));

    BasicQuantity<double, CandelaUnit>      cd1{1};
    BasicQuantity<double, CenticandelaUnit> ccd1{cd1};
    EXPECT_FLOAT_EQ(ccd1.value(), 100.0);
    BasicQuantity<double, KilocandelaUnit> kcd1(std::move(ccd1));
    EXPECT_FLOAT_EQ(kcd1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CenticandelaUnit>(cd1)));

    BasicQuantity<double, GramUnit>      g1{1};
    BasicQuantity<double, CentigramUnit> cg1{g1};
    EXPECT_FLOAT_EQ(cg1.value(), 100.0);
    BasicQuantity<double, KilogramUnit> kg1(std::move(cg1));
    EXPECT_FLOAT_EQ(kg1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CentigramUnit>(g1)));

    BasicQuantity<double, KelvinUnit>      K1{1};
    BasicQuantity<double, CentikelvinUnit> cK1{K1};
    EXPECT_FLOAT_EQ(cK1.value(), 100.0);
    BasicQuantity<double, KilokelvinUnit> kK1(std::move(cK1));
    EXPECT_FLOAT_EQ(kK1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CentikelvinUnit>(K1)));

    BasicQuantity<double, SecondUnit>      s1{1};
    BasicQuantity<double, CentisecondUnit> cs1{s1};
    EXPECT_FLOAT_EQ(cs1.value(), 100.0);
    BasicQuantity<double, KilosecondUnit> ks1(std::move(cs1));
    EXPECT_FLOAT_EQ(ks1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CentisecondUnit>(s1)));

    BasicQuantity<double, RadianUnit>      rad1{1};
    BasicQuantity<double, CentiradianUnit> crad1{rad1};
    EXPECT_FLOAT_EQ(crad1.value(), 100.0);
    BasicQuantity<double, KiloradianUnit> krad1(std::move(crad1));
    EXPECT_FLOAT_EQ(krad1.value(), 1e-3);
    EXPECT_TRUE(noexcept(BasicQuantity<double, CentiradianUnit>(rad1)));
}