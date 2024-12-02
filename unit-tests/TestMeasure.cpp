#include "internal/Measure.hpp"

#include <gtest/gtest.h>

using namespace maxwell::internal;

TEST(TestMeasure, TestCXXProperties)
{
    using TestType = std::remove_cvref_t<decltype(null_measure)>;

    EXPECT_EQ(sizeof(TestType), 1);
    EXPECT_EQ(alignof(TestType), 1);
    EXPECT_TRUE(std::default_initializable<TestType>);
    EXPECT_TRUE(std::equality_comparable<TestType>);
    EXPECT_TRUE(std::regular<TestType>);
}

TEST(TestMeasure, TestGetterConstantExpression)
{
    const measure_type<1, 2> m;

    constexpr std::intmax_t power      = m.get_power();
    constexpr std::intmax_t multiplier = m.get_multiplier();

    EXPECT_EQ(power, 1);
    EXPECT_EQ(multiplier, 2);
}

TEST(TestMeasure, TestAdjustMultiplier)
{
    const measure_type<1, 1> m;
    const measure auto       newMeasure = m.adjust_multiplier<2>();

    EXPECT_EQ(newMeasure.get_power(), 1);
    EXPECT_EQ(newMeasure.get_multiplier(), 3);

    constexpr measure_type<1, 1> measure2;
    constexpr measure auto       newMeasure2 = measure2.adjust_multiplier<2>();

    EXPECT_EQ(newMeasure2.get_power(), 1);
    EXPECT_EQ(newMeasure2.get_multiplier(), 3);

    constexpr measure auto newMeasure3 = m.adjust_multiplier<2>();
    EXPECT_EQ(newMeasure3.get_power(), 1);
    EXPECT_EQ(newMeasure3.get_multiplier(), 3);
}

TEST(TestMeasure, TestIsBaseMeasure)
{
    const measure_type<0, 0>                                 measure1;
    const measure_type<1, 0>                                 measure2;
    const measure_type<1, 1>                                 measure3;
    const measure_type<0, 0, std::ratio<1, 2>>               measure4;
    const measure_type<0, 0, _detail::one, std::ratio<1, 2>> measure5;

    constexpr bool res1 = measure1.is_base_measure();
    constexpr bool res2 = measure2.is_base_measure();
    constexpr bool res3 = measure3.is_base_measure();
    constexpr bool res4 = measure4.is_base_measure();
    constexpr bool res5 = measure5.is_base_measure();

    EXPECT_FALSE(res1);
    EXPECT_TRUE(res2);
    EXPECT_FALSE(res3);
    EXPECT_FALSE(res4);
    EXPECT_FALSE(res5);
}

TEST(TestMeasure, TestIsCoherentMeasure)
{
    const measure_type<2, 0> measure1;
    const measure_type<2, 1> measure2;

    constexpr bool res1 = measure1.is_coherent_measure();
    constexpr bool res2 = measure2.is_coherent_measure();

    EXPECT_TRUE(res1);
    EXPECT_FALSE(res2);
}

TEST(TestMeasure, TestToCoherentMeasure)
{
    const measure_type<2, 2> m;
    constexpr measure auto   modified = m.to_coherent_measure();

    EXPECT_EQ(modified.get_power(), 2);
    EXPECT_EQ(modified.get_multiplier(), 0);
}

TEST(TestMeasure, TestMeasureConcept)
{
    using BaseType  = measure_type<0, 0>;
    using ConstType = std::add_const_t<BaseType>;
    using LRefType  = std::add_lvalue_reference_t<BaseType>;
    using RRefType  = std::add_rvalue_reference_t<BaseType>;
    using CLRefType = std::add_const_t<LRefType>;
    using CRRefType = std::add_const_t<RRefType>;

    EXPECT_FALSE(measure<int>);
    EXPECT_TRUE(measure<BaseType>);
    EXPECT_TRUE(measure<ConstType>);
    EXPECT_TRUE(measure<LRefType>);
    EXPECT_TRUE(measure<RRefType>);
    EXPECT_TRUE(measure<CLRefType>);
    EXPECT_TRUE(measure<CRRefType>);
}

TEST(TestMeasure, TestMeasureConvertibleConcept)
{
    const measure_type<1, 0>                                     m1;
    const measure_type<1, 1>                                     m2;
    const measure_type<2, 0>                                     m3;
    const measure_type<1, 0, std::ratio<1, 2>>                   m4;
    const measure_type<1, 0, _detail::one, std::ratio<1, 2>>     m5;
    const measure_type<1, 0, std::ratio<1, 2>, std::ratio<1, 2>> m6;

    constexpr bool res1  = is_measure_convertible(m1, m2);
    constexpr bool res2  = is_measure_convertible(m2, m1);
    constexpr bool res3  = is_measure_convertible(m1, m3);
    constexpr bool res4  = is_measure_convertible(m3, m1);
    constexpr bool res5  = is_measure_convertible(m2, m3);
    constexpr bool res6  = is_measure_convertible(m3, m2);
    constexpr bool res7  = is_measure_convertible(m3, m3);
    constexpr bool res8  = is_measure_convertible(m1, m4);
    constexpr bool res9  = is_measure_convertible(m4, m1);
    constexpr bool res10 = is_measure_convertible(m1, m5);
    constexpr bool res11 = is_measure_convertible(m5, m1);
    constexpr bool res12 = is_measure_convertible(m1, m6);
    constexpr bool res13 = is_measure_convertible(m6, m1);

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
    const measure_type<1, 0> m1;
    const measure_type<1, 1> m2;
    measure_type<1, 0>       m3;

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
    const measure_type<1, 0>                                     m1;
    const measure_type<1, 1>                                     m2;
    const measure_type<1, 1, std::ratio<1, 2>, std::ratio<1, 2>> m3;
    const measure_type<2, 2, std::ratio<2, 3>, std::ratio<2, 3>> m4;

    constexpr measure auto p1 = m1 * m1;
    constexpr measure auto p2 = m1 * m2;
    constexpr measure auto p3 = m3 * m3;
    constexpr measure auto p4 = m3 * m4;

    EXPECT_EQ(p1.get_power(), 2);
    EXPECT_EQ(p1.get_multiplier(), 0);

    EXPECT_EQ(p2.get_power(), 2);
    EXPECT_EQ(p2.get_multiplier(), 0);

    EXPECT_EQ(p3.get_power(), 2);
    EXPECT_EQ(p3.get_multiplier(), 1);
    EXPECT_TRUE((std::ratio_equal_v<decltype(p3)::scale, std::ratio<1, 4>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(p3)::offset, _detail::zero>));

    EXPECT_EQ(p4.get_power(), 3);
    EXPECT_EQ(p4.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(p4)::scale, std::ratio<2, 6>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(p4)::offset, _detail::zero>));
}

TEST(TestMeasure, TestMeasureDivision)
{
    const measure_type<1, 0>                                     m1;
    const measure_type<1, 1>                                     m2;
    const measure_type<1, 1, std::ratio<1, 2>, std::ratio<1, 2>> m3;
    const measure_type<2, 2, std::ratio<2, 3>, std::ratio<2, 3>> m4;

    constexpr measure auto q1 = m1 / m1;
    constexpr measure auto q2 = m1 / m2;
    constexpr measure auto q3 = m3 / m3;
    constexpr measure auto q4 = m3 / m4;

    EXPECT_EQ(q1.get_power(), 0);
    EXPECT_EQ(q1.get_multiplier(), 0);

    EXPECT_EQ(q2.get_power(), 0);
    EXPECT_EQ(q2.get_multiplier(), 0);

    EXPECT_EQ(q3.get_power(), 0);
    EXPECT_EQ(q3.get_multiplier(), 1);
    EXPECT_TRUE((std::ratio_equal_v<decltype(q3)::scale, std::ratio<1, 1>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(q3)::offset, _detail::zero>));

    EXPECT_EQ(q4.get_power(), -1);
    EXPECT_EQ(q4.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(q4)::scale, std::ratio<3, 4>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(q4)::offset, _detail::zero>));
}