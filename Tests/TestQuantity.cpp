#include <compare>
#include <gtest/gtest.h> 
#include <type_traits>

#include "BaseUnitScales.hpp"
#include "Quantity.hpp"
#include "QuantityCore.hpp"
#include "QuantityTypes.hpp"
#include "Unit.hpp"
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

TEST(TestQuantity, TestConvertingConstructorPrefix)
{
    //Test Prefix conversion
    Basic_Quantity<double, SecondUnit> s{1'000};
    Basic_Quantity<double, KilosecondUnit> ks{s};
    Basic_Quantity<double, MillisecondUnit> ms{s};

    EXPECT_FLOAT_EQ(ks.value(), 1.0);
    EXPECT_EQ(ks.units(), KilosecondUnit{});
    
    EXPECT_FLOAT_EQ(ms.value(), 1'000*1'000);
    EXPECT_EQ(ms.units(), MillisecondUnit{});

    Basic_Quantity<double, MeterUnit> m{1'000};
    Basic_Quantity<double, KilometerUnit> km{m};
    Basic_Quantity<double, MillimeterUnit> mm{m};

    EXPECT_FLOAT_EQ(km.value(), 1.0);
    EXPECT_EQ(km.units(), KilometerUnit{});

    EXPECT_FLOAT_EQ(mm.value(), 1'000*1'000);
    EXPECT_EQ(mm.units(), MillimeterUnit{});

    Basic_Quantity<double, GramUnit> g{1'000};
    Basic_Quantity<double, KilogramUnit> kg{g};
    Basic_Quantity<double, MilligramUnit> mg{g}; 

    EXPECT_FLOAT_EQ(kg.value(), 1.0);
    EXPECT_EQ(kg.units(), KilogramUnit{});

    EXPECT_FLOAT_EQ(mg.value(), 1'000*1'000);
    EXPECT_EQ(mg.units(), MilligramUnit{});

    Basic_Quantity<double, AmpereUnit> A{1'000};
    Basic_Quantity<double, KiloampereUnit> kA{A};
    Basic_Quantity<double, MilliampereUnit> mA{A};

    EXPECT_FLOAT_EQ(kA.value(), 1.0);
    EXPECT_EQ(kA.units(), KiloampereUnit{});

    EXPECT_FLOAT_EQ(mA.value(), 1'000*1'000);
    EXPECT_EQ(mA.units(), MilliampereUnit{});

    Basic_Quantity<double, KelvinUnit> K{1'000};
    Basic_Quantity<double, KilokelvinUnit> KK{K};
    Basic_Quantity<double, MillikelvinUnit> mK{K};

    EXPECT_FLOAT_EQ(KK.value(), 1.0);
    EXPECT_EQ(KK.units(), KilokelvinUnit{});

    EXPECT_FLOAT_EQ(mK.value(), 1'000*1'000);
    EXPECT_EQ(mK.units(), MillikelvinUnit{});

    Basic_Quantity<double, MoleUnit> mol{1'000};
    Basic_Quantity<double, KilomoleUnit> kMol{mol};
    Basic_Quantity<double, MillimoleUnit> mmol{mol};

    EXPECT_FLOAT_EQ(kMol.value(), 1.0);
    EXPECT_EQ(kMol.units(), KilomoleUnit{});

    EXPECT_FLOAT_EQ(mmol.value(), 1'000*1'000);
    EXPECT_EQ(mmol.units(), MillimoleUnit{});

    Basic_Quantity<double, CandelaUnit> cd{1'000};
    Basic_Quantity<double, KilocandelaUnit> kcd{cd};
    Basic_Quantity<double, MillicandelaUnit> mcd{cd};

    EXPECT_FLOAT_EQ(kcd.value(), 1.0);
    EXPECT_EQ(kcd.units(), KilocandelaUnit{});

    EXPECT_FLOAT_EQ(mcd.value(), 1'000*1'000);
    EXPECT_EQ(mcd.units(), MillicandelaUnit{});

    Basic_Quantity<double, RadianUnit> rad{1'000};
    Basic_Quantity<double, KiloradianUnit> krad{rad};
    Basic_Quantity<double, MilliradianUnit> mrad{rad};

    EXPECT_FLOAT_EQ(krad.value(), 1.0);
    EXPECT_EQ(krad.units(), KiloradianUnit{});

    EXPECT_FLOAT_EQ(mrad.value(), 1'000*1'000);
    EXPECT_EQ(mrad.units(), MilliradianUnit{});

    using InputUnit = decltype(SecondUnit{}*MeterUnit{}*GramUnit{}*AmpereUnit{}*KelvinUnit()*MoleUnit{}*CandelaUnit{}*RadianUnit{});
    using OutputUnit = decltype(PetasecondUnit{}*DecimeterUnit{}*QuettagramUnit{}*YoctoampereUnit{}*FemtokelvinUnit{}*QuectomoleUnit{}*QuettacandelaUnit{}*DecaradianUnit{});

    Basic_Quantity<double, InputUnit> in{1.0};
    Basic_Quantity<double, OutputUnit> out{in};

    EXPECT_FLOAT_EQ(out.value(), 1e-6);
    EXPECT_EQ(out.units(), OutputUnit{});

    Basic_Quantity<double, SqMeterUnit> sm{1.0};
    Basic_Quantity<double, SqCentimeterUnit> scm{sm};

    EXPECT_FLOAT_EQ(scm.value(), 100*100);
    EXPECT_EQ(scm.units(), SqCentimeterUnit{});
}

TEST(TestQuantity, TestConvertingConstructorScale)
{
    Basic_Quantity<double, SecondUnit> s1{60.0};
    Basic_Quantity<double, MinuteUnit> min{s1};
    Basic_Quantity<double, SecondUnit> s2{min};

    EXPECT_FLOAT_EQ(min.value(), 1.0);
    EXPECT_FLOAT_EQ(s2.value(), 60.0);

    Basic_Quantity<double, RadianUnit> rad1{M_PI};
    Basic_Quantity<double, DegreeUnit> deg{rad1}; 
    Basic_Quantity<double, RadianUnit> rad2{deg};

    EXPECT_FLOAT_EQ(deg.value(), 180.0);
    EXPECT_FLOAT_EQ(rad2.value(), M_PI);

    Basic_Quantity<double, MeterUnit> m1{1.0};
    Basic_Quantity<double, FootUnit> ft{m1};
    Basic_Quantity<double, MeterUnit> m2{ft};

    EXPECT_FLOAT_EQ(ft.value(), 1/0.3048);
    EXPECT_FLOAT_EQ(m2.value(), 1.0);

    Basic_Quantity<double, KilogramUnit> kg1{1.0};
    Basic_Quantity<double, PoundMassUnit> lbm{kg1};
    Basic_Quantity<double, KilogramUnit> kg2{lbm};

    EXPECT_FLOAT_EQ(lbm.value(), 2.204622);
    EXPECT_FLOAT_EQ(kg2.value(), 1.0);

    Basic_Quantity<double, FootUnit> ft1{1.0};
    Basic_Quantity<double, InchUnit> in{ft1};
    Basic_Quantity<double, FootUnit> ft2{in};

    EXPECT_FLOAT_EQ(in.value(), 12.0);
    EXPECT_FLOAT_EQ(ft2.value(), 1.0);

    Basic_Quantity<double, SqFootUnit>
}