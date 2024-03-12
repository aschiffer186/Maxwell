#include <gtest/gtest.h>

#include "Maxwell.hh"
#include "UnitBase.hh"
#include "UnitTypes.hh"

using namespace Maxwell;

#include <array>

TEST(TestUnits, TestUnitBase)
{
    EXPECT_TRUE(UnitBaseLike<decltype(NullUnitBase)>);
    EXPECT_TRUE(UnitBaseLike<decltype(CoherentUnitBase)>);
    EXPECT_FALSE(UnitBaseLike<int>);

    EXPECT_TRUE(isCoherentUnitBase(CoherentUnitBase));
    EXPECT_TRUE(isCoherentUnitBase(UnitBase<2, 0, 1, 1, 1, 1>{}));
    EXPECT_FALSE(isCoherentUnitBase(UnitBase<2, 2, 1, 1, 1, 1>{}));

    EXPECT_TRUE(CoherentUnitBase == CoherentUnitBase);
    EXPECT_TRUE(CoherentUnitBase != NullUnitBase);
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