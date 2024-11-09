#include "Quantity.hpp"
#include "QuantityRepo.hpp"
#include "Unit.hpp"

#include <concepts>
#include <gtest/gtest.h>
#include <type_traits>

using namespace Maxwell;

TEST(TestQuantity, TestQuantityCXXProperties)
{
    using Type = Maxwell::Meter;

    EXPECT_EQ(sizeof(Type), sizeof(double));
    EXPECT_EQ(alignof(Type), alignof(double));
    EXPECT_TRUE(std::regular<Type>);

    EXPECT_TRUE(std::is_standard_layout_v<Type>);
    EXPECT_TRUE(std::destructible<Type>);
    EXPECT_TRUE(std::is_trivially_destructible_v<Type>);
}

TEST(TestQuantity, TestDefaultConstructor)
{
    EXPECT_TRUE(std::default_initializable<Meter>);
    EXPECT_TRUE(std::is_nothrow_default_constructible_v<Meter>);

    const Meter m{};
    EXPECT_FLOAT_EQ(m.magnitude(), double{});

    constexpr Meter m2{};
    EXPECT_FLOAT_EQ(m2.magnitude(), double{});
}

TEST(TestQuantity, TestMagnitudeTypeConstructor)
{
    EXPECT_TRUE((std::constructible_from<Meter, double>));
    EXPECT_TRUE((std::is_nothrow_constructible_v<Meter, double>));

    const Meter m{1.0};
    EXPECT_FLOAT_EQ(m.magnitude(), double(1.0));

    constexpr Meter m2{1.0};
    EXPECT_FLOAT_EQ(m2.magnitude(), double(1.0));
}