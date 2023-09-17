#include <gtest/gtest.h> 
#include <type_traits>

#include "Quantity.hpp"
#include "QuantityCore.hpp"
#include "UnitCore.hpp"
#include "UnitTypes.hpp"

TEST(TestQuantity, TestLanguageProperties)
{
    using Q1 = Maxwell::Basic_Quantity<int, Maxwell::MeterUnit>;
    using Q2 = Maxwell::Basic_Quantity<double, Maxwell::MeterUnit>;

    EXPECT_EQ(sizeof(Q1), sizeof(int));
    EXPECT_EQ(sizeof(Q2), sizeof(double));
    EXPECT_TRUE(std::is_class_v<Q1>);
    EXPECT_TRUE(std::is_class_v<Q2>);
    EXPECT_TRUE(std::is_nothrow_default_constructible_v<Q1>);
    EXPECT_TRUE(std::is_nothrow_default_constructible_v<Q2>);
    EXPECT_TRUE(std::is_trivially_copy_constructible_v<Q1>);
    EXPECT_TRUE(std::is_trivially_copy_constructible_v<Q2>);
    EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Q1>);
    EXPECT_TRUE(std::is_nothrow_copy_constructible_v<Q2>);
    EXPECT_TRUE(std::is_trivially_move_constructible_v<Q1>);
    EXPECT_TRUE(std::is_trivially_move_constructible_v<Q2>);
    EXPECT_TRUE(std::is_nothrow_move_constructible_v<Q1>);
    EXPECT_TRUE(std::is_nothrow_move_constructible_v<Q2>);
    EXPECT_TRUE(std::is_trivially_copy_assignable_v<Q1>);
    EXPECT_TRUE(std::is_trivially_copy_assignable_v<Q2>);
    EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Q1>);
    EXPECT_TRUE(std::is_nothrow_copy_assignable_v<Q2>);
    EXPECT_TRUE(std::is_trivially_move_assignable_v<Q1>);
    EXPECT_TRUE(std::is_trivially_move_assignable_v<Q2>);
    EXPECT_TRUE(std::is_nothrow_move_assignable_v<Q1>);
    EXPECT_TRUE(std::is_nothrow_move_assignable_v<Q2>);
    EXPECT_TRUE(std::is_trivially_destructible_v<Q1>);
    EXPECT_TRUE(std::is_trivially_destructible_v<Q2>);
    EXPECT_TRUE(std::is_nothrow_destructible_v<Q1>);
    EXPECT_TRUE(std::is_nothrow_destructible_v<Q2>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Q1>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Q2>);
    EXPECT_TRUE(std::is_standard_layout_v<Q1>);
    EXPECT_TRUE(std::is_standard_layout_v<Q2>);
    EXPECT_TRUE(std::is_nothrow_swappable_v<Q1>);
    EXPECT_TRUE(std::is_nothrow_swappable_v<Q2>);
    EXPECT_TRUE(std::regular<Q1>);
    EXPECT_TRUE(std::regular<Q2>);
    EXPECT_TRUE(std::equality_comparable<Q1>);
    EXPECT_TRUE(std::equality_comparable<Q2>);
    EXPECT_TRUE(std::three_way_comparable<Q1>);
    EXPECT_TRUE(std::three_way_comparable<Q2>);
    EXPECT_TRUE(std::totally_ordered<Q1>);
    EXPECT_TRUE(std::totally_ordered<Q2>);
}

TEST(TestQuantity, TestQuantityConstructors)
{
    using Q1 = Maxwell::Basic_Quantity<int, Maxwell::MeterUnit>;
    using Q2 = Maxwell::Basic_Quantity<double, Maxwell::MeterUnit>; 

    Q1 q1; 
    Q2 q2; 

    EXPECT_EQ(q1.value(), 0);
    EXPECT_EQ(q1.units(), Maxwell::MeterUnit{});
    EXPECT_FLOAT_EQ(q2.value(), 0.0);
    EXPECT_EQ(q2.units(), Maxwell::MeterUnit{});

    Q1 q3{1};
    Q2 q4{1.0};

    EXPECT_EQ(q3.value(), 1);
    EXPECT_EQ(q3.units(), Maxwell::MeterUnit{});
    EXPECT_FLOAT_EQ(q4.value(), 1.0);
    EXPECT_EQ(q4.units(), Maxwell::MeterUnit{});

    //Test copy constructor
    Maxwell::Basic_Quantity<int, Maxwell::MeterUnit> q5{q3};
    EXPECT_EQ(q5.value(), 1);
    EXPECT_EQ(q5.units(), Maxwell::MeterUnit{});
    EXPECT_TRUE((std::same_as<decltype(q5)::Rep, int>));

    // Test converting constructor
    Maxwell::Basic_Quantity<double, Maxwell::KilometerUnit> q6{q5};
    EXPECT_FLOAT_EQ(q6.value(), 1e-3);
    EXPECT_EQ(q6.units(), Maxwell::KilometerUnit{});

    Maxwell::Basic_Quantity<double, decltype(Maxwell::MeterUnit{}*Maxwell::SecondUnit{})> q7{10.0};
    Maxwell::Basic_Quantity<double, decltype(Maxwell::KilometerUnit{}*Maxwell::NanosecondUnit{})> q8{q7};
    EXPECT_FLOAT_EQ(q8.value(), 10.0*1e-3*1e9);
    EXPECT_EQ(q8.units(), Maxwell::KilometerUnit{}*Maxwell::NanosecondUnit{});

    Maxwell::Quantity<decltype(Maxwell::KilometerUnit{}*Maxwell::NanosecondUnit{})> q;
}