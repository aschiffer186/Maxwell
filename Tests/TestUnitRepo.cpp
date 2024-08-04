#include <gtest/gtest.h>

#include "Maxwell.hpp"
#include "detail/UnitRepo.hpp"

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

TEST(TestUnitRepo, TestPrefixes) {
    EXPECT_EQ(QuettaAmpereUnit.current().prefix(), Quetta);
    EXPECT_EQ(RonnaAmpereUnit.current().prefix(), Ronna);
    EXPECT_EQ(YottaAmpereUnit.current().prefix(), Yotta);
    EXPECT_EQ(ZettaAmpereUnit.current().prefix(), Zetta);
    EXPECT_EQ(ExaAmpereUnit.current().prefix(), Exa);
    EXPECT_EQ(PetaAmpereUnit.current().prefix(), Peta);
    EXPECT_EQ(TeraAmpereUnit.current().prefix(), Tera);
    EXPECT_EQ(GigaAmpereUnit.current().prefix(), Giga);
    EXPECT_EQ(MegaAmpereUnit.current().prefix(), Mega);
    EXPECT_EQ(KiloAmpereUnit.current().prefix(), Kilo);
    EXPECT_EQ(HectoAmpereUnit.current().prefix(), Hecto);
    EXPECT_EQ(DecaAmpereUnit.current().prefix(), Deca);
    EXPECT_EQ(DeciAmpereUnit.current().prefix(), Deci);
    EXPECT_EQ(CentiAmpereUnit.current().prefix(), Centi);
    EXPECT_EQ(MilliAmpereUnit.current().prefix(), Milli);
    EXPECT_EQ(MicroAmpereUnit.current().prefix(), Micro);
    EXPECT_EQ(NanoAmpereUnit.current().prefix(), Nano);
    EXPECT_EQ(PicoAmpereUnit.current().prefix(), Pico);
    EXPECT_EQ(FemtoAmpereUnit.current().prefix(), Femto);
    EXPECT_EQ(AttoAmpereUnit.current().prefix(), Atto);
    EXPECT_EQ(ZeptoAmpereUnit.current().prefix(), Zepto);
    EXPECT_EQ(YoctoAmpereUnit.current().prefix(), Yocto);
    EXPECT_EQ(RontoAmpereUnit.current().prefix(), Ronto);
    EXPECT_EQ(QuectoAmpereUnit.current().prefix(), Quecto);
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

TEST(TestUnitRepo, TestHertzUnit) { EXPECT_EQ(HertzUnit.time().power(), -1); }

TEST(TestUnitRepo, TestNewtonUnit) {
    EXPECT_EQ(NewtonUnit.mass().prefix(), Kilo);
    EXPECT_EQ(NewtonUnit.mass().power(), 1);
    EXPECT_EQ(NewtonUnit.length().power(), 1);
    EXPECT_EQ(NewtonUnit.time().power(), -2);
}

TEST(TestUnitRepo, TestPascalUnit) {
    EXPECT_EQ(PascalUnit.mass().power(), 1);
    EXPECT_EQ(PascalUnit.mass().prefix(), Kilo);
    EXPECT_EQ(PascalUnit.length().power(), -1);
    EXPECT_EQ(PascalUnit.time().power(), -2);
}

TEST(TestUnitRepo, TestJouleUnit) {
    EXPECT_EQ(JouleUnit.mass().power(), 1);
    EXPECT_EQ(JouleUnit.mass().prefix(), Kilo);
    EXPECT_EQ(JouleUnit.length().power(), 2);
    EXPECT_EQ(JouleUnit.time().power(), -2);
}

TEST(TestUnitRepo, TestWattUnit) {
    EXPECT_EQ(JouleUnit.mass().power(), 1);
    EXPECT_EQ(JouleUnit.mass().prefix(), Kilo);
    EXPECT_EQ(JouleUnit.length().power(), 2);
    EXPECT_EQ(JouleUnit.time().power(), -2);
}
