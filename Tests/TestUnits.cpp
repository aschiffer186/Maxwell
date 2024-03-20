#include <gtest/gtest.h>
#include <type_traits>

#include "Maxwell.hh"
#include "UnitBase.hh"
#include "UnitTypes.hh"

using namespace Maxwell;

#include <array>

TEST(TestUnits, TestUnitBaseBasics)
{

    UnitBase<1, 2, 3, 4, 5, 6> ub;

    EXPECT_TRUE(std::is_nothrow_default_constructible_v<decltype(ub)>);
    EXPECT_TRUE(std::is_trivially_default_constructible_v<decltype(ub)>);

    EXPECT_TRUE(std::is_empty_v<decltype(ub)>);

    EXPECT_EQ(decltype(ub)::Power, 1);
    EXPECT_EQ(decltype(ub)::Prefix, 2);
    EXPECT_EQ(decltype(ub)::OffsetNum, 3);
    EXPECT_EQ(decltype(ub)::OffsetDen, 4);
    EXPECT_EQ(decltype(ub)::ScaleNum, 5);
    EXPECT_EQ(decltype(ub)::ScaleDen, 6);

    EXPECT_EQ(ub.power(), 1);
    EXPECT_EQ(ub.prefix(), 2);
    EXPECT_EQ(ub.offsetNum(), 3);
    EXPECT_EQ(ub.offsetDen(), 4);
    EXPECT_EQ(ub.scaleNum(), 5);
    EXPECT_EQ(ub.scaleDen(), 6);

    constexpr UnitBase<1, 2, 3, 4, 5, 6> ub2;
    [[maybe_unused]] constexpr int       pow2       = ub2.power();
    [[maybe_unused]] constexpr int       prefix2    = ub2.prefix();
    [[maybe_unused]] constexpr int       offsetNum2 = ub2.offsetNum();
    [[maybe_unused]] constexpr int       offsetDen2 = ub2.offsetDen();
    [[maybe_unused]] constexpr int       scaleNum2  = ub2.scaleNum();
    [[maybe_unused]] constexpr int       scaleDen2  = ub2.scaleDen();
}

TEST(TestUnits, TestUnitBaseConcepts)
{
    UnitBase<1, 2, 3, 4, 5, 6> ub;
    EXPECT_TRUE(UnitBaseLike<decltype(ub)>);
    EXPECT_FALSE(UnitBaseLike<double>);
}

TEST(TestUnits, TestUnitBaseCoherency)
{
    UnitBase<2, 2, 3, 4, 5, 6>  ub;
    UnitBase<12, 0, 0, 1, 1, 1> ub2;

    EXPECT_FALSE(isCoherentUnitBase(ub));
    EXPECT_TRUE(isCoherentUnitBase(ub2));
    EXPECT_TRUE(isCoherentUnitBase(CoherentUnitBase));
    EXPECT_TRUE(isCoherentUnitBase(NullUnitBase));

    const auto cub = toCoherentUnitBase(ub);
    EXPECT_TRUE(isCoherentUnitBase(cub));
    EXPECT_EQ(cub.power(), ub.power());
}

TEST(TestUnits, TestUnitBaseOperators)
{
    decltype(CoherentUnitBase) ub;
    EXPECT_EQ(ub, CoherentUnitBase);
    EXPECT_NE(ub, NullUnitBase);

    UnitBase<1, 2, 3, 4, 5, 6> ub2;
    const auto                 cub = toCoherentUnitBase(ub2);
    EXPECT_EQ(cub, CoherentUnitBase);
    EXPECT_NE(ub2, cub);

    const auto prod = ub * ub2;
    EXPECT_EQ(prod.power(), ub.power() + ub2.power());
    const auto quot = ub / ub2;
    EXPECT_EQ(quot.power(), ub.power() - ub2.power());

    UnitBase<2, 2, 3, 4, 5, 6> ub3;
    const auto                 inv = unitBaseInverse(ub3);
    EXPECT_EQ(inv.power(), -1 * ub3.power());
}

TEST(TestUnits, TestUnitBaseScaleConversion)
{
    UnitBase<1, 0, 0, 1, 3'048, 1'000> ftBase;
    UnitBase<1, 0, 0, 1, 254, 1'000>   inBase;

    double conversion = unitBaseScaleConversion(ftBase, inBase);
    EXPECT_FLOAT_EQ(conversion, 12);

    conversion = unitBaseScaleConversion(inBase, ftBase);
    EXPECT_FLOAT_EQ(conversion, 1.0 / 12.0);
}

TEST(TestUnits, TestUnit)
{
    // Ensure units are convertible/not convertible as expected
    EXPECT_TRUE((UnitConvertibleTo<MeterUnitType, CentimeterUnitType>));
    EXPECT_FALSE((UnitConvertibleTo<MeterUnitType, MoleUnitType>));

    // Test Scaling units
    constexpr auto centimole = adjustPrefixAmount<-2>(MoleUnit);
    EXPECT_EQ(centimole.amount().prefix() - MoleUnit.amount().prefix(), -2);

    constexpr auto centiamp = adjustPrefixCurrent<-2>(AmpereUnit);
    EXPECT_EQ(centiamp.current().prefix() - AmpereUnit.current().prefix(), -2);

    constexpr auto centimeter = adjustPrefixLength<-2>(MeterUnit);
    EXPECT_EQ(centimeter.length().prefix() - MeterUnit.length().prefix(), -2);

    constexpr auto centicandela = adjustPrefixLuminosity<-2>(CandelaUnit);
    EXPECT_EQ(centicandela.luminosity().prefix() -
                  CandelaUnit.luminosity().prefix(),
              -2);

    constexpr auto gram = adjustPrefixMass<-3>(KilogramUnit);
    EXPECT_EQ(gram.mass().prefix() - KilogramUnit.mass().prefix(), -3);

    constexpr auto centiKelvin = adjustPrefixTemperature<-2>(KelvinUnit);
    EXPECT_EQ(centiKelvin.temperature().prefix() -
                  KelvinUnit.temperature().prefix(),
              -2);

    constexpr auto centisecond = adjustPrefixTime<-2>(SecondUnit);
    EXPECT_EQ(centisecond.time().prefix() - SecondUnit.time().prefix(), -2);

    constexpr auto centiradian = adjustPrefixAngle<-2>(RadianUnit);
    EXPECT_EQ(centiradian.angle().prefix() - RadianUnit.angle().prefix(), -2);
}

TEST(TestUnits, TestUnitConversionPrefix) {}