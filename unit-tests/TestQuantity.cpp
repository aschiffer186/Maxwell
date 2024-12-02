#include "Quantity.hpp"
#include "QuantityRepo.hpp"
#include "Unit.hpp"

#include <concepts>
#include <gtest/gtest.h>
#include <type_traits>

#include "TestTypes.hpp"

using namespace maxwell;

TEST(TestQuantity, TestQuantityCXXProperties)
{
    using Type = maxwell::meter;

    EXPECT_EQ(sizeof(Type), sizeof(double));
    EXPECT_EQ(alignof(Type), alignof(double));
    EXPECT_TRUE(std::regular<Type>);

    EXPECT_TRUE(std::is_standard_layout_v<Type>);
    EXPECT_TRUE(std::destructible<Type>);
    EXPECT_TRUE(std::is_trivially_destructible_v<Type>);
}

TEST(TestQuantity, TestDefaultConstructor)
{
    EXPECT_TRUE(std::default_initializable<meter>);
    EXPECT_TRUE(std::is_nothrow_default_constructible_v<meter>);

    const meter m{};
    EXPECT_FLOAT_EQ(m.magnitude(), double{});

    const meter m2{};

    EXPECT_FLOAT_EQ(m2.magnitude(), double{});
}

template <mass M>
void foo(M)
{
}

TEST(TestQuantity, TestMagnitudeTypeConstructor)
{
    EXPECT_TRUE((std::constructible_from<meter, double>));
    EXPECT_TRUE((std::is_nothrow_constructible_v<meter, double>));

    const meter m{1.0};
    EXPECT_FLOAT_EQ(m.magnitude(), double(1.0));

    constexpr meter m2{1.0};
    EXPECT_FLOAT_EQ(m2.magnitude(), double(1.0));

    Noisy<true>                             n{};
    Noisy<false>                            n2{};
    basic_quantity<Noisy<true>, meter_unit> q{n};

    bool is_nothrow = noexcept(basic_quantity<Noisy<true>, meter_unit>{n});

    EXPECT_TRUE(is_nothrow);
    EXPECT_EQ(Noisy<true>::numCopyCtorCalls, 1);
    EXPECT_EQ(Noisy<true>::numMoveCtorCalls, 1);

    constexpr bool is_nothrow2 = noexcept(basic_quantity<Noisy<false>, meter_unit>{n2});
    EXPECT_FALSE(is_nothrow2);
}

TEST(TestQuantity, TestForwardingConstructor)
{
    using type1 = basic_quantity<Noisy<true>, meter_unit>;
    using type2 = basic_quantity<Noisy<false>, meter_unit>;
}