#include "internal/Measure.hpp"

#include <gtest/gtest.h>

using namespace Maxwell::Internal;

TEST(TestMeasure, TestCXXProperties)
{
    using TestType = std::remove_cvref_t<decltype(nullMeasure)>;

    EXPECT_EQ(sizeof(TestType), 1);
    EXPECT_EQ(alignof(TestType), 1);
    EXPECT_TRUE(std::default_initializable<TestType>);
    EXPECT_TRUE(std::equality_comparable<TestType>);
    EXPECT_TRUE(std::regular<TestType>);
}

TEST(TestMeasure, TestGetterConstantExpression)
{
    const MeasureType<1, 2> measure;

    constexpr std::intmax_t power      = measure.power();
    constexpr std::intmax_t multiplier = measure.multiplier();

    EXPECT_EQ(power, 1);
    EXPECT_EQ(multiplier, 2);
}

TEST(TestMeasure, TestAdjustMultiplier)
{
    const MeasureType<1, 1> measure;
    const Measure auto      newMeasure = measure.adjustMultiplier<2>();

    EXPECT_EQ(newMeasure.power(), 1);
    EXPECT_EQ(newMeasure.multiplier(), 3);

    constexpr MeasureType<1, 1> measure2;
    constexpr Measure auto      newMeasure2 = measure2.adjustMultiplier<2>();

    EXPECT_EQ(newMeasure2.power(), 1);
    EXPECT_EQ(newMeasure2.multiplier(), 3);

    constexpr Measure auto newMeasure3 = measure.adjustMultiplier<2>();
    EXPECT_EQ(newMeasure3.power(), 1);
    EXPECT_EQ(newMeasure3.multiplier(), 3);
}

TEST(TestMeasure, TestIsBaseMeasure)
{
    const MeasureType<0, 0>                                 measure1;
    const MeasureType<1, 0>                                 measure2;
    const MeasureType<1, 1>                                 measure3;
    const MeasureType<0, 0, std::ratio<1, 2>>               measure4;
    const MeasureType<0, 0, _detail::one, std::ratio<1, 2>> measure5;

    constexpr bool res1 = measure1.isBaseMeasure();
    constexpr bool res2 = measure2.isBaseMeasure();
    constexpr bool res3 = measure3.isBaseMeasure();
    constexpr bool res4 = measure4.isBaseMeasure();
    constexpr bool res5 = measure5.isBaseMeasure();

    EXPECT_FALSE(res1);
    EXPECT_TRUE(res2);
    EXPECT_FALSE(res3);
    EXPECT_FALSE(res4);
    EXPECT_FALSE(res5);
}

TEST(TestMeasure, TestIsCoherentMeasure)
{
    const MeasureType<2, 0> measure1;
    const MeasureType<2, 1> measure2;

    constexpr bool res1 = measure1.isCoherentMeasure();
    constexpr bool res2 = measure2.isCoherentMeasure();

    EXPECT_TRUE(res1);
    EXPECT_FALSE(res2);
}

TEST(TestMeasure, TestToCoherentMeasure)
{
    const MeasureType<2, 2> measure;
    constexpr Measure auto  modified = measure.toCoherentMeasure();

    EXPECT_EQ(modified.power(), 2);
    EXPECT_EQ(modified.multiplier(), 0);
}

TEST(TestMeasure, TestMeasureConcept)
{
    using BaseType  = MeasureType<0, 0>;
    using ConstType = std::add_const_t<BaseType>;
    using LRefType  = std::add_lvalue_reference_t<BaseType>;
    using RRefType  = std::add_rvalue_reference_t<BaseType>;
    using CLRefType = std::add_const_t<LRefType>;
    using CRRefType = std::add_const_t<RRefType>;

    EXPECT_FALSE(Measure<int>);
    EXPECT_TRUE(Measure<BaseType>);
    EXPECT_TRUE(Measure<ConstType>);
    EXPECT_TRUE(Measure<LRefType>);
    EXPECT_TRUE(Measure<RRefType>);
    EXPECT_TRUE(Measure<CLRefType>);
    EXPECT_TRUE(Measure<CRRefType>);
}

TEST(TestMeasure, TestMeasureConvertibleConcept)
{
    const MeasureType<1, 0>                                     m1;
    const MeasureType<1, 1>                                     m2;
    const MeasureType<2, 0>                                     m3;
    const MeasureType<1, 0, std::ratio<1, 2>>                   m4;
    const MeasureType<1, 0, _detail::one, std::ratio<1, 2>>     m5;
    const MeasureType<1, 0, std::ratio<1, 2>, std::ratio<1, 2>> m6;

    constexpr bool res1  = isMeasureConvertible(m1, m2);
    constexpr bool res2  = isMeasureConvertible(m2, m1);
    constexpr bool res3  = isMeasureConvertible(m1, m3);
    constexpr bool res4  = isMeasureConvertible(m3, m1);
    constexpr bool res5  = isMeasureConvertible(m2, m3);
    constexpr bool res6  = isMeasureConvertible(m3, m2);
    constexpr bool res7  = isMeasureConvertible(m3, m3);
    constexpr bool res8  = isMeasureConvertible(m1, m4);
    constexpr bool res9  = isMeasureConvertible(m4, m1);
    constexpr bool res10 = isMeasureConvertible(m1, m5);
    constexpr bool res11 = isMeasureConvertible(m5, m1);
    constexpr bool res12 = isMeasureConvertible(m1, m6);
    constexpr bool res13 = isMeasureConvertible(m6, m1);

    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
    EXPECT_FALSE(res3);
    EXPECT_FALSE(res4);
    EXPECT_FALSE(res5);
    EXPECT_FALSE(res6);
    EXPECT_TRUE(res7);
    EXPECT_TRUE(res8);
    EXPECT_TRUE(res9);
    EXPECT_TRUE(res10);
    EXPECT_TRUE(res11);
    EXPECT_TRUE(res12);
    EXPECT_TRUE(res13);
}

TEST(TestMeasure, TestMeasureEquality)
{
    const MeasureType<1, 0> m1;
    const MeasureType<1, 1> m2;
    MeasureType<1, 0>       m3;

    constexpr bool res1 = (m1 == m1);
    constexpr bool res2 = (m1 == m2);
    constexpr bool res3 = (m2 == m1);

    constexpr bool res4 = (m1 != m1);
    constexpr bool res5 = (m1 != m2);
    constexpr bool res6 = (m2 != m1);

    constexpr bool res7  = (m1 == m3);
    constexpr bool res8  = (m3 == m1);
    constexpr bool res9  = (m1 != m3);
    constexpr bool res10 = (m3 != m1);

    EXPECT_TRUE(res1);
    EXPECT_FALSE(res2);
    EXPECT_FALSE(res3);

    EXPECT_FALSE(res4);
    EXPECT_TRUE(res5);
    EXPECT_TRUE(res6);

    EXPECT_TRUE(res7);
    EXPECT_TRUE(res8);
    EXPECT_FALSE(res9);
    EXPECT_FALSE(res10);
}

TEST(TestMeasure, TestMeasureMultiplication)
{
    const MeasureType<1, 0>                                     m1;
    const MeasureType<1, 1>                                     m2;
    const MeasureType<1, 1, std::ratio<1, 2>, std::ratio<1, 2>> m3;
    const MeasureType<2, 2, std::ratio<2, 3>, std::ratio<2, 3>> m4;

    constexpr Measure auto p1 = m1 * m1;
    constexpr Measure auto p2 = m1 * m2;
    constexpr Measure auto p3 = m3 * m3;
    constexpr Measure auto p4 = m3 * m4;

    EXPECT_EQ(p1.power(), 2);
    EXPECT_EQ(p1.multiplier(), 0);

    EXPECT_EQ(p2.power(), 2);
    EXPECT_EQ(p2.multiplier(), 0);

    EXPECT_EQ(p3.power(), 2);
    EXPECT_EQ(p3.multiplier(), 1);
    EXPECT_TRUE((std::ratio_equal_v<decltype(p3)::Scale, std::ratio<1, 2>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(p3)::Offset, std::ratio<1, 2>>));

    EXPECT_EQ(p4.power(), 3);
    EXPECT_EQ(p4.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(p4)::Scale, _detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(p4)::Offset, _detail::zero>));
}

TEST(TestMeasure, TestMeasureDivision)
{
    const MeasureType<1, 0>                                     m1;
    const MeasureType<1, 1>                                     m2;
    const MeasureType<1, 1, std::ratio<1, 2>, std::ratio<1, 2>> m3;
    const MeasureType<2, 2, std::ratio<2, 3>, std::ratio<2, 3>> m4;

    constexpr Measure auto q1 = m1 / m1;
    constexpr Measure auto q2 = m1 / m2;
    constexpr Measure auto q3 = m3 / m3;
    constexpr Measure auto q4 = m3 / m4;

    EXPECT_EQ(q1.power(), 0);
    EXPECT_EQ(q1.multiplier(), 0);

    EXPECT_EQ(q2.power(), 0);
    EXPECT_EQ(q2.multiplier(), 0);

    EXPECT_EQ(q3.power(), 0);
    EXPECT_EQ(q3.multiplier(), 1);
    EXPECT_TRUE((std::ratio_equal_v<decltype(q3)::Scale, std::ratio<1, 2>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(q3)::Offset, std::ratio<1, 2>>));

    EXPECT_EQ(q4.power(), -1);
    EXPECT_EQ(q4.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(q4)::Scale, _detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(q4)::Offset, _detail::zero>));
}