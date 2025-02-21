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

TEST(TestQuantity, TestInPlaceConstructor)
{
    using test_type = basic_quantity<in_place, meter_unit>;

    test_type q(std::in_place, 1.0, 2.0);
    EXPECT_FLOAT_EQ(q.magnitude().value, 3.0);

    test_type q2(std::in_place, {1.0, 2.0, 3.0}, 4.0);
    EXPECT_FLOAT_EQ(q2.magnitude().value, 10);
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

TEST(TestQuantity, TestUnitConvertingConstructor)
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

    using FromType4 = basic_quantity<double, kelvin_unit>;
    using ToType4   = basic_quantity<double, celsius_unit>;
    using ToType6   = basic_quantity<double, fahrenheit_unit>;

    const FromType4 k{1.0};
    const ToType4   c{k};

    EXPECT_FLOAT_EQ(c.magnitude(), -272.15);
    const ToType6 far{k};
    EXPECT_FLOAT_EQ(far.magnitude(), -457.87);
}

TEST(TestQuantity, TestConvertingAssignmentOperator)
{
    using FromType = basic_quantity<double, kilometer_unit / second_unit>;
    using ToType   = basic_quantity<double, mile_unit / hour_unit>;

    const FromType f{1.0};
    ToType         t;
    t = f;

    EXPECT_FLOAT_EQ(t.magnitude(), 2'236.9363);

    const ToType t2{1.0};
    FromType     f2;
    f2 = t2;
    EXPECT_NEAR(f2.magnitude(), 0.000447, 1e-6);

    using FromType2 = basic_quantity<double, cubic_meter_unit>;
    using ToType2   = basic_quantity<double, foot_unit * foot_unit * foot_unit>;

    const FromType2 f3{1};
    ToType2         t3;
    t3 = f3;
    EXPECT_NEAR(t3.magnitude(), 35.3147, 1e-4);

    const ToType2 t4{1};
    FromType2     f4;
    f4 = t4;
    EXPECT_NEAR(f4.magnitude(), 0.0283168, 1e-7);

    using FromType3 = basic_quantity<double, radian_unit / hour_unit>;
    using ToType3   = basic_quantity<double, degree_unit / second_unit>;

    const FromType3 r{std::numbers::pi};
    ToType3         d;
    d = r;
    EXPECT_FLOAT_EQ(d.magnitude(), 180.0 / 3'600.0);
}

TEST(TestQuantity, TestChronoAssignmentOperator)
{
    using namespace std::chrono;
    using namespace std::chrono_literals;

    basic_quantity<double, second_unit> q;
    q = 1min;
    EXPECT_FLOAT_EQ(q.magnitude(), 60.0);

    basic_quantity<double, minute_unit> q2;
    q2 = 1s;
    EXPECT_FLOAT_EQ(q2.magnitude(), 1.0 / 60.0);

    using from_duration = std::chrono::duration<double, std::ratio<12, 50>>;
    const from_duration               from{12};
    basic_quantity<double, hour_unit> q3;
    q3 = from;

    using to_duration   = std::chrono::duration<double, std::ratio<3'600, 1>>;
    const auto test_val = std::chrono::duration_cast<to_duration>(from);
    EXPECT_FLOAT_EQ(q3.magnitude(), test_val.count());
}

TEST(TestQuantity, TestUnitlessAssignmentOperator)
{
    EXPECT_FALSE((std::is_assignable_v<meter, double>));

    basic_quantity<double, unitless_unit_type{}> q{1.0};
    q = 2.0;
    EXPECT_EQ(q.magnitude(), 2.0);
}

TEST(TestQuantity, TestQuantityEquality)
{
    const int_meter     m1{1};
    const int_meter     m2{1};
    const int_meter     m3{1'000};
    const int_kilometer m4{1};

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_TRUE(m1 != m3);
    EXPECT_TRUE(m4 == m3);
    EXPECT_TRUE(m4 != m1);
}

TEST(TestQuantity, TestQuantityThreeWayComparison)
{
    const meter     m1{1.0};
    const meter     m2{2.0};
    const meter     m3{2'000.0};
    const kilometer k1{1.0};

    EXPECT_TRUE(m1 < m2);
    EXPECT_FALSE(m2 < m1);
    EXPECT_TRUE(m2 > m1);
    EXPECT_FALSE(m1 > m2);

    EXPECT_TRUE(m3 > k1);
    EXPECT_FALSE(k1 > m3);
    EXPECT_TRUE(k1 < m3);
    EXPECT_FALSE(m3 < k1);
}

TEST(TestQuantity, TestChronoConversion)
{
    int_nanosecond                        s{10.0};
    [[maybe_unused]] std::chrono::seconds s2(s);

    static_assert(internal::_detail::enable_implicit_to_chrono<std::chrono::seconds, int, maxwell::nanosecond_unit>);

    std::cout << s << "\n";
}

TEST(TestQuantity, TestMagnitude)
{
    using test_type = basic_quantity<std::vector<double>, meter_unit>;

    test_type q(std::in_place, {1.0, 2.0, 3.0});

    decltype(auto) m1 = q.magnitude();
    decltype(auto) m2 = std::move(q).magnitude();
    decltype(auto) m3 = std::move(std::as_const(q)).magnitude();

    EXPECT_TRUE((std::same_as<decltype(m1), const std::vector<double>&>));
    EXPECT_TRUE((std::same_as<decltype(m2), std::vector<double>&&>));
    EXPECT_TRUE((std::same_as<decltype(m3), const std::vector<double>&&>));
}