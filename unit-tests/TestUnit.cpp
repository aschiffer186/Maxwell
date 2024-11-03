#include "Unit.hpp"
#include "UnitRepo.hpp"
#include "internal/Concepts.hpp"
#include "internal/Measure.hpp"

#include <gtest/gtest.h>
#include <type_traits>
#include <utility>

using namespace Maxwell;

TEST(TestUnit, TestGetterConstantExpression)
{
    constexpr Internal::Measure auto amount      = meterUnit.amount();
    constexpr Internal::Measure auto current     = meterUnit.current();
    constexpr Internal::Measure auto length      = meterUnit.length();
    constexpr Internal::Measure auto luminosity  = meterUnit.luminosity();
    constexpr Internal::Measure auto mass        = meterUnit.mass();
    constexpr Internal::Measure auto temperature = meterUnit.temperature();
    constexpr Internal::Measure auto time        = meterUnit.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length, Internal::baseMeasure);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time, Internal::nullMeasure);
}

TEST(TestUnit, TestAddTag)
{
    constexpr Unit auto u = meterUnit.addTag<int>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length, Internal::baseMeasure);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time, Internal::nullMeasure);
    EXPECT_TRUE((std::same_as<decltype(u)::Tag, int>));
}

TEST(TestUnit, TestAdjustAmount)
{
    constexpr Unit auto u = moleUnit.adjustMultiplierAmount<3>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount.power(), 1);
    EXPECT_EQ(amount.multiplier(), 3);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length, Internal::nullMeasure);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time, Internal::nullMeasure);
}

TEST(TestUnit, TestAdjustCurrent)
{
    constexpr Unit auto u = ampereUnit.adjustMultiplierCurrent<3>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current.power(), 1);
    EXPECT_EQ(current.multiplier(), 3);
    EXPECT_EQ(length, Internal::nullMeasure);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time, Internal::nullMeasure);
}

TEST(TestUnit, TestAdjustLength)
{
    constexpr Unit auto u = meterUnit.adjustMultiplierLength<3>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length.power(), 1);
    EXPECT_EQ(length.multiplier(), 3);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time, Internal::nullMeasure);
}

TEST(TestUnit, TestAdjustLuminosity)
{
    constexpr Unit auto u = candelaUnit.adjustMultiplierLuminosity<3>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length, Internal::nullMeasure);

    EXPECT_EQ(luminosity.power(), 1);
    EXPECT_EQ(luminosity.multiplier(), 3);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time, Internal::nullMeasure);
}

TEST(TestUnit, TestAdjustMass)
{
    constexpr Unit auto u = gramUnit.adjustMultiplierMass<3>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length, Internal::nullMeasure);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass.power(), 1);
    EXPECT_EQ(mass.multiplier(), 3);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time, Internal::nullMeasure);
}

TEST(TestUnit, TestAdjustTemperature)
{
    constexpr Unit auto u = kelvinUnit.adjustMultiplierTemperature<3>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length, Internal::nullMeasure);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature.power(), 1);
    EXPECT_EQ(temperature.multiplier(), 3);
    EXPECT_EQ(time, Internal::nullMeasure);
}

TEST(TestUnit, TestAdjustTime)
{
    constexpr Unit auto u = secondUnit.adjustMultiplierTime<3>();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount, Internal::nullMeasure);
    EXPECT_EQ(current, Internal::nullMeasure);
    EXPECT_EQ(length, Internal::nullMeasure);
    EXPECT_EQ(luminosity, Internal::nullMeasure);
    EXPECT_EQ(mass, Internal::nullMeasure);
    EXPECT_EQ(temperature, Internal::nullMeasure);
    EXPECT_EQ(time.power(), 1);
    EXPECT_EQ(time.multiplier(), 3);
}

TEST(TestUnit, TestToSIBaseUnits)
{
    constexpr Internal::MeasureType<2, 3, std::ratio<2, 1>, std::ratio<3, 2>> m;

    const UnitType<m, m, m, m, m, m, m, int> startUnit;
    constexpr Unit auto                      u = startUnit.toSIBaseUnits();

    constexpr Internal::Measure auto amount      = u.amount();
    constexpr Internal::Measure auto current     = u.current();
    constexpr Internal::Measure auto length      = u.length();
    constexpr Internal::Measure auto luminosity  = u.luminosity();
    constexpr Internal::Measure auto mass        = u.mass();
    constexpr Internal::Measure auto temperature = u.temperature();
    constexpr Internal::Measure auto time        = u.time();

    EXPECT_EQ(amount.power(), 2);
    EXPECT_EQ(amount.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::Scale, Internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::Offset, Internal::_detail::zero>));

    EXPECT_EQ(current.power(), 2);
    EXPECT_EQ(current.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::Scale, Internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::Offset, Internal::_detail::zero>));

    EXPECT_EQ(length.power(), 2);
    EXPECT_EQ(length.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::Scale, Internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::Offset, Internal::_detail::zero>));

    EXPECT_EQ(luminosity.power(), 2);
    EXPECT_EQ(luminosity.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::Scale, Internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::Offset, Internal::_detail::zero>));

    EXPECT_EQ(mass.power(), 2);
    EXPECT_EQ(mass.multiplier(), 3);
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::Scale, Internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::Offset, Internal::_detail::zero>));

    EXPECT_EQ(temperature.power(), 2);
    EXPECT_EQ(temperature.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::Scale, Internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::Offset, Internal::_detail::zero>));

    EXPECT_EQ(time.power(), 2);
    EXPECT_EQ(time.multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::Scale, Internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::Offset, Internal::_detail::zero>));

    EXPECT_TRUE((std::same_as<decltype(u)::Tag, void>));
}

TEST(TestUnit, TestUnitConcept)
{
    using BaseType  = std::remove_cvref_t<meterUnitType>;
    using ConstType = std::add_const_t<BaseType>;
    using LRefType  = std::add_lvalue_reference_t<BaseType>;
    using RRefType  = std::add_rvalue_reference_t<BaseType>;
    using CLRefType = std::add_const_t<LRefType>;
    using CRRefType = std::add_const_t<RRefType>;

    EXPECT_TRUE(Unit<BaseType>);
    EXPECT_TRUE(Unit<ConstType>);
    EXPECT_TRUE(Unit<LRefType>);
    EXPECT_TRUE(Unit<RRefType>);
    EXPECT_TRUE(Unit<CLRefType>);
    EXPECT_TRUE(Unit<CRRefType>);
}

TEST(TestUnit, TestUnitless)
{
    EXPECT_FALSE(UnitlessUnit<moleUnit>);
    EXPECT_FALSE(UnitlessUnit<ampereUnit>);
    EXPECT_FALSE(UnitlessUnit<meterUnit>);
    EXPECT_FALSE(UnitlessUnit<candelaUnit>);
    EXPECT_FALSE(UnitlessUnit<gramUnit>);
    EXPECT_FALSE(UnitlessUnit<kelvinUnit>);
    EXPECT_FALSE(UnitlessUnit<secondUnit>);
    EXPECT_FALSE(UnitlessUnit<radianUnit>);
    EXPECT_TRUE(UnitlessUnit<unitlessUnit>);
}

TEST(TestUnit, TestUnitTraits)
{
    EXPECT_TRUE(AmountUnit<moleUnit>);
    EXPECT_TRUE(AmountUnit<kilomoleUnit>);
    EXPECT_FALSE(AmountUnit<ampereUnit>);
    EXPECT_FALSE(AmountUnit<meterUnit>);
    EXPECT_FALSE(AmountUnit<candelaUnit>);
    EXPECT_FALSE(AmountUnit<gramUnit>);
    EXPECT_FALSE(AmountUnit<kelvinUnit>);
    EXPECT_FALSE(AmountUnit<secondUnit>);
    EXPECT_FALSE(AmountUnit<radianUnit>);
    EXPECT_FALSE(AmountUnit<unitlessUnit>);

    EXPECT_FALSE(CurrentUnit<moleUnit>);
    EXPECT_TRUE(CurrentUnit<ampereUnit>);
    EXPECT_TRUE(CurrentUnit<kiloampereUnit>);
    EXPECT_FALSE(CurrentUnit<meterUnit>);
    EXPECT_FALSE(CurrentUnit<candelaUnit>);
    EXPECT_FALSE(CurrentUnit<gramUnit>);
    EXPECT_FALSE(CurrentUnit<kelvinUnit>);
    EXPECT_FALSE(CurrentUnit<secondUnit>);
    EXPECT_FALSE(CurrentUnit<radianUnit>);
    EXPECT_FALSE(CurrentUnit<unitlessUnit>);

    EXPECT_FALSE(LengthUnit<moleUnit>);
    EXPECT_FALSE(LengthUnit<ampereUnit>);
    EXPECT_TRUE(LengthUnit<meterUnit>);
    EXPECT_TRUE(LengthUnit<kilometerUnit>);
    EXPECT_FALSE(LengthUnit<candelaUnit>);
    EXPECT_FALSE(LengthUnit<gramUnit>);
    EXPECT_FALSE(LengthUnit<kelvinUnit>);
    EXPECT_FALSE(LengthUnit<secondUnit>);
    EXPECT_FALSE(LengthUnit<radianUnit>);
    EXPECT_FALSE(LengthUnit<unitlessUnit>);

    EXPECT_FALSE(LuminosityUnit<moleUnit>);
    EXPECT_FALSE(LuminosityUnit<ampereUnit>);
    EXPECT_FALSE(LuminosityUnit<meterUnit>);
    EXPECT_TRUE(LuminosityUnit<candelaUnit>);
    EXPECT_TRUE(LuminosityUnit<kilocandelaUnit>);
    EXPECT_FALSE(LuminosityUnit<gramUnit>);
    EXPECT_FALSE(LuminosityUnit<kelvinUnit>);
    EXPECT_FALSE(LuminosityUnit<secondUnit>);
    EXPECT_FALSE(LuminosityUnit<radianUnit>);
    EXPECT_FALSE(LuminosityUnit<unitlessUnit>);

    EXPECT_FALSE(MassUnit<moleUnit>);
    EXPECT_FALSE(MassUnit<ampereUnit>);
    EXPECT_FALSE(MassUnit<meterUnit>);
    EXPECT_FALSE(MassUnit<candelaUnit>);
    EXPECT_TRUE(MassUnit<gramUnit>);
    EXPECT_TRUE(MassUnit<kilogramUnit>);
    EXPECT_FALSE(MassUnit<kelvinUnit>);
    EXPECT_FALSE(MassUnit<secondUnit>);
    EXPECT_FALSE(MassUnit<radianUnit>);
    EXPECT_FALSE(MassUnit<unitlessUnit>);

    EXPECT_FALSE(TemperatureUnit<moleUnit>);
    EXPECT_FALSE(TemperatureUnit<ampereUnit>);
    EXPECT_FALSE(TemperatureUnit<meterUnit>);
    EXPECT_FALSE(TemperatureUnit<candelaUnit>);
    EXPECT_FALSE(TemperatureUnit<gramUnit>);
    EXPECT_TRUE(TemperatureUnit<kelvinUnit>);
    EXPECT_TRUE(TemperatureUnit<kilokelvinUnit>);
    EXPECT_FALSE(TemperatureUnit<secondUnit>);
    EXPECT_FALSE(TemperatureUnit<radianUnit>);
    EXPECT_FALSE(TemperatureUnit<unitlessUnit>);

    EXPECT_FALSE(TimeUnit<moleUnit>);
    EXPECT_FALSE(TimeUnit<ampereUnit>);
    EXPECT_FALSE(TimeUnit<meterUnit>);
    EXPECT_FALSE(TimeUnit<candelaUnit>);
    EXPECT_FALSE(TimeUnit<gramUnit>);
    EXPECT_FALSE(TimeUnit<kelvinUnit>);
    EXPECT_TRUE(TimeUnit<secondUnit>);
    EXPECT_TRUE(TimeUnit<kilosecondUnit>);
    EXPECT_FALSE(TimeUnit<radianUnit>);
    EXPECT_FALSE(TimeUnit<unitlessUnit>);

    EXPECT_FALSE(AngleUnit<moleUnit>);
    EXPECT_FALSE(AngleUnit<ampereUnit>);
    EXPECT_FALSE(AngleUnit<meterUnit>);
    EXPECT_FALSE(AngleUnit<candelaUnit>);
    EXPECT_FALSE(AngleUnit<gramUnit>);
    EXPECT_FALSE(AngleUnit<kelvinUnit>);
    EXPECT_FALSE(AngleUnit<secondUnit>);
    EXPECT_TRUE(AngleUnit<radianUnit>);
    static_assert(AngleUnit<kiloradianUnit>);
    EXPECT_TRUE(AngleUnit<degreeUnit>);
    EXPECT_FALSE(AngleUnit<unitlessUnit>);
}

TEST(TestUnit, TestUnitEquality)
{
    Unit auto        u  = meterUnit;
    const Unit auto  u2 = meterUnit;
    const Unit auto& u3 = meterUnit;

    EXPECT_EQ(u, u);
    EXPECT_EQ(u, u2);
    EXPECT_EQ(u2, u);
    EXPECT_EQ(u, u3);
    EXPECT_EQ(u3, u);
    EXPECT_EQ(u2, u3);
    EXPECT_EQ(u3, u2);

    EXPECT_NE(meterUnit, kilometerUnit);
    EXPECT_FALSE(u != u);
    EXPECT_FALSE(u != u2);
    EXPECT_FALSE(u2 != u);
    EXPECT_FALSE(u != u3);
    EXPECT_FALSE(u3 != u);
    EXPECT_FALSE(u2 != u3);
    EXPECT_FALSE(u3 != u2);
}

// --- Type Parameterized Tests ---
template <typename T>
class UnitIncompatabilityTest : public testing::Test
{
  public:
    static inline constexpr T                       value{};
    static inline constexpr typename T::first_type  first  = value.first;
    static inline constexpr typename T::second_type second = value.second;
};

using IncompatibleUnits =
    ::testing::Types<std::pair<moleUnitType, ampereUnitType>, std::pair<ampereUnitType, candelaUnitType>,
                     std::pair<candelaUnitType, gramUnitType>, std::pair<gramUnitType, kelvinUnitType>,
                     std::pair<kelvinUnitType, secondUnitType>, std::pair<secondUnitType, radianUnitType>,
                     std::pair<radianUnitType, unitlessUnitType>>;
TYPED_TEST_SUITE(UnitIncompatabilityTest, IncompatibleUnits);

TYPED_TEST(UnitIncompatabilityTest, TestUnitIncompatability)
{
    constexpr auto first  = TestFixture::first;
    constexpr auto second = TestFixture::second;

    EXPECT_FALSE((UnitConvertibleTo<first, second>));
    EXPECT_FALSE((UnitConvertibleTo<second, first>));
}

template <typename T>
class UnitCompatabilityTest : public testing::Test
{
  public:
    static inline constexpr T                       value{};
    static inline constexpr typename T::first_type  first  = value.first;
    static inline constexpr typename T::second_type second = value.second;
};

using CompatibleUnits =
    ::testing::Types<std::pair<moleUnitType, kilomoleUnitType>, std::pair<ampereUnitType, kiloampereUnitType>,
                     std::pair<meterUnitType, kilometerUnitType>, std::pair<candelaUnitType, kilocandelaUnitType>,
                     std::pair<gramUnitType, kilogramUnitType>, std::pair<kelvinUnitType, kilokelvinUnitType>,
                     std::pair<secondUnitType, kilosecondUnitType>, std::pair<radianUnitType, kiloradianUnitType>,
                     std::pair<radianUnitType, degreeUnitType>>;
TYPED_TEST_SUITE(UnitCompatabilityTest, CompatibleUnits);

TYPED_TEST(UnitCompatabilityTest, TestCompatibleUnits)
{
    constexpr auto first  = TestFixture::first;
    constexpr auto second = TestFixture::second;

    EXPECT_TRUE((UnitConvertibleTo<first, second>));
    EXPECT_TRUE((UnitConvertibleTo<second, first>));
}

TYPED_TEST(UnitCompatabilityTest, TestUnitPrefixConversion)
{
    constexpr auto first  = TestFixture::first;
    constexpr auto second = TestFixture::second;

    double conversion1 = conversionFactor(first, second);
    double conversion2 = conversionFactor(second, first);

    if constexpr (!Internal::Similar<decltype(second), degreeUnitType>)
    {
        EXPECT_FLOAT_EQ(conversion1, 1e-3);
        EXPECT_FLOAT_EQ(conversion2, 1e3);
    }
    else
    {
        EXPECT_FLOAT_EQ(conversion1, 180.0 / std::numbers::pi);
        EXPECT_FLOAT_EQ(conversion2, std::numbers::pi / 180.0);
    }
}