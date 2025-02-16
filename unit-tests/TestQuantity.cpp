#include "Quantity.hpp"
#include "QuantityRepo.hpp"
#include "Unit.hpp"

#include <chrono>
#include <concepts>
#include <gtest/gtest.h>
#include <numbers>
#include <type_traits>

#include "TestTypes.hpp"
#include "UnitRepo.hpp"

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

    EXPECT_TRUE(std::is_copy_constructible_v<Type>);
    EXPECT_TRUE(std::is_move_constructible_v<Type>);
    EXPECT_TRUE(std::is_copy_assignable_v<Type>);
    EXPECT_TRUE(std::is_move_assignable_v<Type>);
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
    // using type1 = basic_quantity<Noisy<true>, meter_unit>;
    // using type2 = basic_quantity<Noisy<false>, meter_unit>;
}

TEST(TestQuantity, TestChronoConstructor)
{
    using namespace std::chrono;
    using namespace std::chrono_literals;

    basic_quantity<double, second_unit> q{1min};
    EXPECT_FLOAT_EQ(q.magnitude(), 60.0);

    basic_quantity<double, minute_unit> q2{1s};
    EXPECT_FLOAT_EQ(q2.magnitude(), 1.0 / 60.0);

    using from_duration = std::chrono::duration<double, std::ratio<12, 50>>;
    const from_duration                     from{12};
    const basic_quantity<double, hour_unit> q3{from};

    using to_duration   = std::chrono::duration<double, std::ratio<3'600, 1>>;
    const auto test_val = std::chrono::duration_cast<to_duration>(from);
    EXPECT_FLOAT_EQ(q3.magnitude(), test_val.count());
}

TEST(TestQuantity, TestConvertingConstructor)
{
    using FromType = basic_quantity<double, kilometer_unit / second_unit>;
    using ToType   = basic_quantity<double, mile_unit / hour_unit>;

    const FromType f{1.0};
    const ToType   t{f};

    EXPECT_FLOAT_EQ(t.magnitude(), 2'236.9363);

    const ToType   t2{1.0};
    const FromType f2{t2};
    EXPECT_NEAR(f2.magnitude(), 0.000447, 1e-6);

    using FromType2 = basic_quantity<double, cubic_meter_unit>;
    using ToType2   = basic_quantity<double, foot_unit * foot_unit * foot_unit>;

    const FromType2 f3{1};
    const ToType2   t3{f3};
    EXPECT_NEAR(t3.magnitude(), 35.3147, 1e-4);

    const ToType2   t4{1};
    const FromType2 f4{t4};
    EXPECT_NEAR(f4.magnitude(), 0.0283168, 1e-7);

    using FromType3 = basic_quantity<double, radian_unit / hour_unit>;
    using ToType3   = basic_quantity<double, degree_unit / second_unit>;

    const FromType3 r{std::numbers::pi};
    const ToType3   d{r};

    EXPECT_FLOAT_EQ(d.magnitude(), 180.0 / 3'600.0);
}

TEST(TestQuantity, TestChronoConversion)
{
    inanosecond                           s{10.0};
    [[maybe_unused]] std::chrono::seconds s2(s);

    static_assert(internal::_detail::enable_implicit_to_chrono<std::chrono::seconds, int, maxwell::nanosecond_unit>);

    std::cout << s << "\n";
}