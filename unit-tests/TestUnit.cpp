#include "Unit.hpp"
#include "UnitRepo.hpp"
#include "internal/Measure.hpp"

#include <gtest/gtest.h>
#include <type_traits>

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
    using BaseType  = std::remove_cvref_t<MeterUnitType>;
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
    EXPECT_FALSE(Unitless<moleUnit>);
    EXPECT_FALSE(Unitless<ampereUnit>);
    EXPECT_FALSE(Unitless<meterUnit>);
    EXPECT_FALSE(Unitless<candelaUnit>);
    EXPECT_FALSE(Unitless<gramUnit>);
    EXPECT_FALSE(Unitless<kelvinUnit>);
    EXPECT_FALSE(Unitless<secondUnit>);
    EXPECT_FALSE(Unitless<radianUnit>);
    EXPECT_TRUE(Unitless<unitlessUnit>);
}