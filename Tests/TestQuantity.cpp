#include <compare>
#include <gtest/gtest.h> 
#include <type_traits>

#include "Quantity.hpp"
#include "QuantityCore.hpp"
#include "QuantityTypes.hpp"
#include "UnitCore.hpp"
#include "UnitTypes.hpp"

using namespace Maxwell;
using namespace Maxwell::Literals;

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

struct Foo 
{
    double val{};

    Foo() noexcept = default; 

    int numMoveCtorCalls{};
    int numCopyCtorCalls{};

    Foo(const Foo& other) noexcept
    : val{other.val}
    {
        ++numCopyCtorCalls;
    }

    Foo(Foo&& other) noexcept
    : val{std::move(other.val)}
    {
        ++numMoveCtorCalls;
    }

    explicit Foo(double d) noexcept
    : val{d}
    {

    }

    Foo operator+(const Foo&) const
    {
        return Foo{};
    }

    Foo operator-(const Foo&) const 
    {
        return Foo{};
    }

    Foo operator*(const Foo&) const
    {
        return Foo{};
    }

    Foo operator/(const Foo&) const 
    {
        return Foo{};
    }

    Foo operator*(double d) const 
    {
        return Foo{};
    }

    friend auto operator<=>(const Foo& lhs, const Foo& rhs) = default;
};

struct Bar 
{
    double val{};

    int numMoveCtorCalls{};
    int numCopyCtorCalls{};

    Bar() noexcept(false) = default; 

    Bar(const Bar& other) 
    : val{other.val}
    {
        ++numCopyCtorCalls;
    }

    Bar(Bar&& other) 
    : val{std::move(other.val)}
    {
        ++numMoveCtorCalls;
    }

    explicit Bar(double d) noexcept
    : val{d}
    {

    }

    Bar operator+(const Bar&) const
    {
        return Bar{};
    }

    Bar operator-(const Bar&) const 
    {
        return Bar{};
    }

    Bar operator*(const Bar&) const
    {
        return Bar{};
    }

    Bar operator/(const Bar&) const 
    {
        return Bar{};
    }

    Bar operator*(double d) const 
    {
        return Bar{};
    }

    friend auto operator<=>(const Bar& lhs, const Bar& rhs) = default;
};

TEST(TestQuantity, TestDefaultConstructor)
{
    Basic_Quantity<double, MeterUnit> q;

    EXPECT_FLOAT_EQ(q.value(), 0.0);
    EXPECT_EQ(q.units(), MeterUnit{});
    EXPECT_TRUE(std::is_nothrow_default_constructible_v<decltype(q)>);

    Basic_Quantity<Foo, MeterUnit> q2;
    EXPECT_FLOAT_EQ(q2.value().val, 0.0);
    EXPECT_EQ(q2.units(), MeterUnit{});
    EXPECT_TRUE(std::is_nothrow_default_constructible_v<decltype(q2)>);

    Basic_Quantity<Bar, MeterUnit> q3;
    EXPECT_FLOAT_EQ(q3.value().val, 0.0);
    EXPECT_EQ(q3.units(), MeterUnit{});
    EXPECT_FALSE(std::is_nothrow_default_constructible_v<decltype(q3)>);
}

TEST(TestQuantity, TestSingleArgumentConstructor)
{
    Basic_Quantity<double, MeterUnit> q{1.0};
    EXPECT_FLOAT_EQ(q.value(), 1.0);
    EXPECT_EQ(q.units(), MeterUnit{});
    bool isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q), double>;
    EXPECT_TRUE(isNothrowConstructible);

    Basic_Quantity<Foo, MeterUnit> q2{Foo{1.0}};
    EXPECT_FLOAT_EQ(q2.value().val, 1.0);
    EXPECT_EQ(q2.units(), MeterUnit{});
    EXPECT_EQ(q2.value().numMoveCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q2), Foo&&>;
    EXPECT_TRUE(isNothrowConstructible);

    Foo f{2.0};
    Basic_Quantity<Foo, MeterUnit> q3{f};
    EXPECT_FLOAT_EQ(q3.value().val, 2.0);
    EXPECT_EQ(q3.units(), MeterUnit{});
    EXPECT_EQ(q3.value().numCopyCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q2),const Foo&>;
    EXPECT_TRUE(isNothrowConstructible);

    Basic_Quantity<Bar, MeterUnit> q4{Bar{1.0}};
    EXPECT_FLOAT_EQ(q4.value().val, 1.0);
    EXPECT_EQ(q4.units(), MeterUnit{});
    EXPECT_EQ(q4.value().numMoveCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q4), Bar&&>;
    EXPECT_FALSE(isNothrowConstructible);

    Bar b{2.0};
    Basic_Quantity<Bar, MeterUnit> q5{b};
    EXPECT_FLOAT_EQ(q5.value().val, 2.0);
    EXPECT_EQ(q5.units(), MeterUnit{});
    EXPECT_EQ(q5.value().numCopyCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q5),const Bar&>;
    EXPECT_FALSE(isNothrowConstructible);
}

TEST(TestQuantity, TestSingleArgumentUnitConstructr)
{
    Basic_Quantity q{1.0, MeterUnit{}};
    EXPECT_FLOAT_EQ(q.value(), 1.0);
    EXPECT_EQ(q.units(), MeterUnit{});
    bool isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q), double, MeterUnit>;
    EXPECT_TRUE(isNothrowConstructible);

    Basic_Quantity q2{Foo{1.0}, MeterUnit{}};
    EXPECT_TRUE((std::is_same_v<decltype(q2)::Rep, Foo>));
    EXPECT_FLOAT_EQ(q2.value().val, 1.0);
    EXPECT_EQ(q2.units(), MeterUnit{});
    EXPECT_EQ(q2.value().numMoveCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q2), Foo&&>;
    EXPECT_TRUE(isNothrowConstructible);

    Foo f{2.0};
    Basic_Quantity q3{f, MeterUnit{}};
    EXPECT_TRUE((std::is_same_v<decltype(q3)::Rep, Foo>));
    EXPECT_FLOAT_EQ(q3.value().val, 2.0);
    EXPECT_EQ(q3.units(), MeterUnit{});
    EXPECT_EQ(q3.value().numCopyCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q2),const Foo&>;
    EXPECT_TRUE(isNothrowConstructible);

    Basic_Quantity q4{Bar{1.0}, MeterUnit{}};
    EXPECT_TRUE((std::is_same_v<decltype(q4)::Rep, Bar>));
    EXPECT_FLOAT_EQ(q4.value().val, 1.0);
    EXPECT_EQ(q4.units(), MeterUnit{});
    EXPECT_EQ(q4.value().numMoveCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q4), Bar&&>;
    EXPECT_FALSE(isNothrowConstructible);

    Bar b{2.0};
    Basic_Quantity q5{b, MeterUnit{}};
    EXPECT_TRUE((std::is_same_v<decltype(q5)::Rep, Bar>));
    EXPECT_FLOAT_EQ(q5.value().val, 2.0);
    EXPECT_EQ(q5.units(), MeterUnit{});
    EXPECT_EQ(q5.value().numCopyCtorCalls, 1);
    isNothrowConstructible = std::is_nothrow_constructible_v<decltype(q5),const Bar&>;
    EXPECT_FALSE(isNothrowConstructible);
}