#include <gtest/gtest.h>

#include "Dimension.hpp"
#include "Unit.hpp"
#include "UnitRepo.hpp"

using namespace Maxwell;

TEST(TestUnitRepo, TestSIBaseUnits) {
    EXPECT_EQ(MoleUnit.amount(), CoherentDimension);
    EXPECT_EQ(MoleUnit.current(), NullDimension);
    EXPECT_EQ(MoleUnit.length(), NullDimension);
    EXPECT_EQ(MoleUnit.luminosity(), NullDimension);
    EXPECT_EQ(MoleUnit.mass(), NullDimension);
    EXPECT_EQ(MoleUnit.temperature(), NullDimension);
    EXPECT_EQ(MoleUnit.time(), NullDimension);
    EXPECT_EQ(MoleUnit.angle(), NullDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );

    EXPECT_EQ(AmpereUnit.amount(), NullDimension);
    EXPECT_EQ(AmpereUnit.current(), CoherentDimension);
    EXPECT_EQ(AmpereUnit.length(), NullDimension);
    EXPECT_EQ(AmpereUnit.luminosity(), NullDimension);
    EXPECT_EQ(AmpereUnit.mass(), NullDimension);
    EXPECT_EQ(AmpereUnit.temperature(), NullDimension);
    EXPECT_EQ(AmpereUnit.time(), NullDimension);
    EXPECT_EQ(AmpereUnit.angle(), NullDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );

    EXPECT_EQ(MeterUnit.amount(), NullDimension);
    EXPECT_EQ(MeterUnit.current(), NullDimension);
    EXPECT_EQ(MeterUnit.length(), CoherentDimension);
    EXPECT_EQ(MeterUnit.luminosity(), NullDimension);
    EXPECT_EQ(MeterUnit.mass(), NullDimension);
    EXPECT_EQ(MeterUnit.temperature(), NullDimension);
    EXPECT_EQ(MeterUnit.time(), NullDimension);
    EXPECT_EQ(MeterUnit.angle(), NullDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );

    EXPECT_EQ(CandelaUnit.amount(), NullDimension);
    EXPECT_EQ(CandelaUnit.current(), NullDimension);
    EXPECT_EQ(CandelaUnit.length(), NullDimension);
    EXPECT_EQ(CandelaUnit.luminosity(), CoherentDimension);
    EXPECT_EQ(CandelaUnit.mass(), NullDimension);
    EXPECT_EQ(CandelaUnit.temperature(), NullDimension);
    EXPECT_EQ(CandelaUnit.time(), NullDimension);
    EXPECT_EQ(CandelaUnit.angle(), NullDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );

    EXPECT_EQ(GramUnit.amount(), NullDimension);
    EXPECT_EQ(GramUnit.current(), NullDimension);
    EXPECT_EQ(GramUnit.length(), NullDimension);
    EXPECT_EQ(GramUnit.luminosity(), NullDimension);
    EXPECT_EQ(GramUnit.mass(), CoherentDimension);
    EXPECT_EQ(GramUnit.temperature(), NullDimension);
    EXPECT_EQ(GramUnit.time(), NullDimension);
    EXPECT_EQ(GramUnit.angle(), NullDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );

    EXPECT_EQ(KelvinUnit.amount(), NullDimension);
    EXPECT_EQ(KelvinUnit.current(), NullDimension);
    EXPECT_EQ(KelvinUnit.length(), NullDimension);
    EXPECT_EQ(KelvinUnit.luminosity(), NullDimension);
    EXPECT_EQ(KelvinUnit.mass(), NullDimension);
    EXPECT_EQ(KelvinUnit.temperature(), CoherentDimension);
    EXPECT_EQ(KelvinUnit.time(), NullDimension);
    EXPECT_EQ(KelvinUnit.angle(), NullDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );

    EXPECT_EQ(SecondUnit.amount(), NullDimension);
    EXPECT_EQ(SecondUnit.current(), NullDimension);
    EXPECT_EQ(SecondUnit.length(), NullDimension);
    EXPECT_EQ(SecondUnit.luminosity(), NullDimension);
    EXPECT_EQ(SecondUnit.mass(), NullDimension);
    EXPECT_EQ(SecondUnit.temperature(), NullDimension);
    EXPECT_EQ(SecondUnit.time(), CoherentDimension);
    EXPECT_EQ(SecondUnit.angle(), NullDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );

    EXPECT_EQ(RadianUnit.amount(), NullDimension);
    EXPECT_EQ(RadianUnit.current(), NullDimension);
    EXPECT_EQ(RadianUnit.length(), NullDimension);
    EXPECT_EQ(RadianUnit.luminosity(), NullDimension);
    EXPECT_EQ(RadianUnit.mass(), NullDimension);
    EXPECT_EQ(RadianUnit.temperature(), NullDimension);
    EXPECT_EQ(RadianUnit.time(), NullDimension);
    EXPECT_EQ(RadianUnit.angle(), CoherentDimension);
    EXPECT_TRUE((std::same_as<decltype(MoleUnit)::Tag, NullTag>) );
}

TEST(TestUnitRepo, TestMeterPerSecond) {
    EXPECT_EQ(MeterPerSecondUnit.length().power(), 1);
    EXPECT_EQ(MeterPerSecondUnit.time().power(), -1);
}

TEST(TestUnitRepo, TestMeterPerSecondPerSecond) {
    EXPECT_EQ(MeterPerSecondPerSecondUnit.length().power(), 1);
    EXPECT_EQ(MeterPerSecondPerSecondUnit.time().power(), -2);
}

TEST(TestUnitRepo, TestSquareMeterUnit) { EXPECT_EQ(SqMeterUnit.length().power(), 2); }

TEST(TestUnitRepo, TestCubicMeterUnit) { EXPECT_EQ(CuMeterUnit.length().power(), 3); }