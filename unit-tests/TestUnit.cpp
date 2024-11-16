#include "Unit.hpp"
#include "UnitRepo.hpp"
#include "internal/Concepts.hpp"
#include "internal/Measure.hpp"

#include <gtest/gtest.h>
#include <type_traits>
#include <utility>

using namespace maxwell;

TEST(TestUnit, TestGetterConstantExpression)
{
    constexpr internal::measure auto amount      = meter_unit.get_amount();
    constexpr internal::measure auto current     = meter_unit.get_current();
    constexpr internal::measure auto length      = meter_unit.get_length();
    constexpr internal::measure auto luminosity  = meter_unit.get_luminosity();
    constexpr internal::measure auto mass        = meter_unit.get_mass();
    constexpr internal::measure auto temperature = meter_unit.get_temperature();
    constexpr internal::measure auto time        = meter_unit.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length, internal::base_measure);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time, internal::null_measure);
}

TEST(TestUnit, TestAddTag)
{
    constexpr unit auto u = meter_unit.add_tag<int>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length, internal::base_measure);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time, internal::null_measure);
    EXPECT_TRUE((std::same_as<decltype(u)::tag, int>));
}

TEST(TestUnit, TestAdjustAmount)
{
    constexpr unit auto u = mole_unit.adjust_multiplier_amount<3>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount.get_power(), 1);
    EXPECT_EQ(amount.get_multiplier(), 3);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length, internal::null_measure);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time, internal::null_measure);
}

TEST(TestUnit, TestAdjustCurrent)
{
    constexpr unit auto u = ampere_unit.adjust_multiplier_current<3>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current.get_power(), 1);
    EXPECT_EQ(current.get_multiplier(), 3);
    EXPECT_EQ(length, internal::null_measure);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time, internal::null_measure);
}

TEST(TestUnit, TestAdjustLength)
{
    constexpr unit auto u = meter_unit.adjust_multiplier_length<3>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length.get_power(), 1);
    EXPECT_EQ(length.get_multiplier(), 3);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time, internal::null_measure);
}

TEST(TestUnit, TestAdjustLuminosity)
{
    constexpr unit auto u = candela_unit.adjust_multiplier_luminosity<3>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length, internal::null_measure);

    EXPECT_EQ(luminosity.get_power(), 1);
    EXPECT_EQ(luminosity.get_multiplier(), 3);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time, internal::null_measure);
}

TEST(TestUnit, TestAdjustMass)
{
    constexpr unit auto u = gram_unit.adjust_multiplier_mass<3>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length, internal::null_measure);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass.get_power(), 1);
    EXPECT_EQ(mass.get_multiplier(), 3);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time, internal::null_measure);
}

TEST(TestUnit, TestAdjustTemperature)
{
    constexpr unit auto u = kelvin_unit.adjust_multiplier_temperature<3>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length, internal::null_measure);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature.get_power(), 1);
    EXPECT_EQ(temperature.get_multiplier(), 3);
    EXPECT_EQ(time, internal::null_measure);
}

TEST(TestUnit, TestAdjustTime)
{
    constexpr unit auto u = second_unit.adjust_multiplier_time<3>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount, internal::null_measure);
    EXPECT_EQ(current, internal::null_measure);
    EXPECT_EQ(length, internal::null_measure);
    EXPECT_EQ(luminosity, internal::null_measure);
    EXPECT_EQ(mass, internal::null_measure);
    EXPECT_EQ(temperature, internal::null_measure);
    EXPECT_EQ(time.get_power(), 1);
    EXPECT_EQ(time.get_multiplier(), 3);
}

TEST(TestUnit, TestAmountScale)
{
    constexpr unit auto u = mole_unit.adjust_scale_amount<std::ratio<12, 1>>().adjust_scale_amount<std::ratio<12, 1>>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, std::ratio<144, 1>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, internal::_detail::one>));
}

TEST(TestUnit, TestCurrentScale)
{
    constexpr unit auto u =
        ampere_unit.adjust_scale_current<std::ratio<12, 1>>().adjust_scale_current<std::ratio<12, 1>>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, std::ratio<144, 1>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, internal::_detail::one>));
}

TEST(TestUnit, TestLengthScale)
{
    constexpr unit auto u =
        meter_unit.adjust_scale_length<std::ratio<12, 1>>().adjust_scale_length<std::ratio<12, 1>>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, std::ratio<144, 1>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, internal::_detail::one>));
}

TEST(TestUnit, TestLuminosityScale)
{
    constexpr unit auto u =
        candela_unit.adjust_scale_luminosity<std::ratio<12, 1>>().adjust_scale_luminosity<std::ratio<12, 1>>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, std::ratio<144, 1>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, internal::_detail::one>));
}

TEST(TestUnit, TestMassScale)
{
    constexpr unit auto u = gram_unit.adjust_scale_mass<std::ratio<12, 1>>().adjust_scale_mass<std::ratio<12, 1>>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, std::ratio<144, 1>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, internal::_detail::one>));
}

TEST(TestUnit, TestTemperatureScale)
{
    constexpr unit auto u =
        kelvin_unit.adjust_scale_temperature<std::ratio<12, 1>>().adjust_scale_temperature<std::ratio<12, 1>>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, std::ratio<144, 1>>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, internal::_detail::one>));
}

TEST(TestUnit, TestTimeScale)
{
    constexpr unit auto u = second_unit.adjust_scale_time<std::ratio<12, 1>>().adjust_scale_time<std::ratio<12, 1>>();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, std::ratio<144, 1>>));
}

TEST(TestUnit, TestToSIBaseUnits)
{
    constexpr internal::measure_type<2, 3, std::ratio<2, 1>, std::ratio<3, 2>> m;

    const unit_type<m, m, m, m, m, m, m, int> start_unit;
    constexpr unit auto                       u = start_unit.to_SI_base_units();

    constexpr internal::measure auto amount      = u.get_amount();
    constexpr internal::measure auto current     = u.get_current();
    constexpr internal::measure auto length      = u.get_length();
    constexpr internal::measure auto luminosity  = u.get_luminosity();
    constexpr internal::measure auto mass        = u.get_mass();
    constexpr internal::measure auto temperature = u.get_temperature();
    constexpr internal::measure auto time        = u.get_time();

    EXPECT_EQ(amount.get_power(), 2);
    EXPECT_EQ(amount.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(amount)::offset, internal::_detail::zero>));

    EXPECT_EQ(current.get_power(), 2);
    EXPECT_EQ(current.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(current)::offset, internal::_detail::zero>));

    EXPECT_EQ(length.get_power(), 2);
    EXPECT_EQ(length.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(length)::offset, internal::_detail::zero>));

    EXPECT_EQ(luminosity.get_power(), 2);
    EXPECT_EQ(luminosity.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(luminosity)::offset, internal::_detail::zero>));

    EXPECT_EQ(mass.get_power(), 2);
    EXPECT_EQ(mass.get_multiplier(), 3);
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(mass)::offset, internal::_detail::zero>));

    EXPECT_EQ(temperature.get_power(), 2);
    EXPECT_EQ(temperature.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(temperature)::offset, internal::_detail::zero>));

    EXPECT_EQ(time.get_power(), 2);
    EXPECT_EQ(time.get_multiplier(), 0);
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::scale, internal::_detail::one>));
    EXPECT_TRUE((std::ratio_equal_v<decltype(time)::offset, internal::_detail::zero>));

    EXPECT_TRUE((std::same_as<decltype(u)::tag, void>));
}

TEST(TestUnit, TestUnitConcept)
{
    using BaseType  = std::remove_cvref_t<meter_unit_type>;
    using ConstType = std::add_const_t<BaseType>;
    using LRefType  = std::add_lvalue_reference_t<BaseType>;
    using RRefType  = std::add_rvalue_reference_t<BaseType>;
    using CLRefType = std::add_const_t<LRefType>;
    using CRRefType = std::add_const_t<RRefType>;

    EXPECT_TRUE(unit<BaseType>);
    EXPECT_TRUE(unit<ConstType>);
    EXPECT_TRUE(unit<LRefType>);
    EXPECT_TRUE(unit<RRefType>);
    EXPECT_TRUE(unit<CLRefType>);
    EXPECT_TRUE(unit<CRRefType>);
}

TEST(TestUnit, TestUnitless)
{
    EXPECT_FALSE(unitless_unit<mole_unit>);
    EXPECT_FALSE(unitless_unit<ampere_unit>);
    EXPECT_FALSE(unitless_unit<meter_unit>);
    EXPECT_FALSE(unitless_unit<candela_unit>);
    EXPECT_FALSE(unitless_unit<gram_unit>);
    EXPECT_FALSE(unitless_unit<kelvin_unit>);
    EXPECT_FALSE(unitless_unit<second_unit>);
    EXPECT_FALSE(unitless_unit<radian_unit>);
    EXPECT_TRUE(unitless_unit<unitless_unit_type{}>);
}

TEST(TestUnit, TestUnitTraits)
{
    EXPECT_TRUE(amount_unit<mole_unit>);
    EXPECT_TRUE(amount_unit<kilomole_unit>);
    EXPECT_FALSE(amount_unit<ampere_unit>);
    EXPECT_FALSE(amount_unit<meter_unit>);
    EXPECT_FALSE(amount_unit<candela_unit>);
    EXPECT_FALSE(amount_unit<gram_unit>);
    EXPECT_FALSE(amount_unit<kelvin_unit>);
    EXPECT_FALSE(amount_unit<second_unit>);
    EXPECT_FALSE(amount_unit<radian_unit>);
    EXPECT_FALSE(amount_unit<unitless_unit_type{}>);

    EXPECT_FALSE(current_unit<mole_unit>);
    EXPECT_TRUE(current_unit<ampere_unit>);
    EXPECT_TRUE(current_unit<kiloampere_unit>);
    EXPECT_FALSE(current_unit<meter_unit>);
    EXPECT_FALSE(current_unit<candela_unit>);
    EXPECT_FALSE(current_unit<gram_unit>);
    EXPECT_FALSE(current_unit<kelvin_unit>);
    EXPECT_FALSE(current_unit<second_unit>);
    EXPECT_FALSE(current_unit<radian_unit>);
    EXPECT_FALSE(current_unit<unitless_unit_type{}>);

    EXPECT_FALSE(length_unit<mole_unit>);
    EXPECT_FALSE(length_unit<ampere_unit>);
    EXPECT_TRUE(length_unit<meter_unit>);
    EXPECT_TRUE(length_unit<kilometer_unit>);
    EXPECT_FALSE(length_unit<candela_unit>);
    EXPECT_FALSE(length_unit<gram_unit>);
    EXPECT_FALSE(length_unit<kelvin_unit>);
    EXPECT_FALSE(length_unit<second_unit>);
    EXPECT_FALSE(length_unit<radian_unit>);
    EXPECT_FALSE(length_unit<unitless_unit_type{}>);

    EXPECT_FALSE(luminosity_unit<mole_unit>);
    EXPECT_FALSE(luminosity_unit<ampere_unit>);
    EXPECT_FALSE(luminosity_unit<meter_unit>);
    EXPECT_TRUE(luminosity_unit<candela_unit>);
    EXPECT_TRUE(luminosity_unit<kilocandela_unit>);
    EXPECT_FALSE(luminosity_unit<gram_unit>);
    EXPECT_FALSE(luminosity_unit<kelvin_unit>);
    EXPECT_FALSE(luminosity_unit<second_unit>);
    EXPECT_FALSE(luminosity_unit<radian_unit>);
    EXPECT_FALSE(luminosity_unit<unitless_unit_type{}>);

    EXPECT_FALSE(mass_unit<mole_unit>);
    EXPECT_FALSE(mass_unit<ampere_unit>);
    EXPECT_FALSE(mass_unit<meter_unit>);
    EXPECT_FALSE(mass_unit<candela_unit>);
    EXPECT_TRUE(mass_unit<gram_unit>);
    EXPECT_TRUE(mass_unit<kilogram_unit>);
    EXPECT_FALSE(mass_unit<kelvin_unit>);
    EXPECT_FALSE(mass_unit<second_unit>);
    EXPECT_FALSE(mass_unit<radian_unit>);
    EXPECT_FALSE(mass_unit<unitless_unit_type{}>);

    EXPECT_FALSE(temperature_unit<mole_unit>);
    EXPECT_FALSE(temperature_unit<ampere_unit>);
    EXPECT_FALSE(temperature_unit<meter_unit>);
    EXPECT_FALSE(temperature_unit<candela_unit>);
    EXPECT_FALSE(temperature_unit<gram_unit>);
    EXPECT_TRUE(temperature_unit<kelvin_unit>);
    EXPECT_TRUE(temperature_unit<kilokelvin_unit>);
    EXPECT_FALSE(temperature_unit<second_unit>);
    EXPECT_FALSE(temperature_unit<radian_unit>);
    EXPECT_FALSE(temperature_unit<unitless_unit_type{}>);

    EXPECT_FALSE(time_unit<mole_unit>);
    EXPECT_FALSE(time_unit<ampere_unit>);
    EXPECT_FALSE(time_unit<meter_unit>);
    EXPECT_FALSE(time_unit<candela_unit>);
    EXPECT_FALSE(time_unit<gram_unit>);
    EXPECT_FALSE(time_unit<kelvin_unit>);
    EXPECT_TRUE(time_unit<second_unit>);
    EXPECT_TRUE(time_unit<kilosecond_unit>);
    EXPECT_FALSE(time_unit<radian_unit>);
    EXPECT_FALSE(time_unit<unitless_unit_type{}>);

    EXPECT_FALSE(angle_unit<mole_unit>);
    EXPECT_FALSE(angle_unit<ampere_unit>);
    EXPECT_FALSE(angle_unit<meter_unit>);
    EXPECT_FALSE(angle_unit<candela_unit>);
    EXPECT_FALSE(angle_unit<gram_unit>);
    EXPECT_FALSE(angle_unit<kelvin_unit>);
    EXPECT_FALSE(angle_unit<second_unit>);
    EXPECT_TRUE(angle_unit<radian_unit>);
    static_assert(angle_unit<kiloradian_unit>);
    EXPECT_TRUE(angle_unit<degree_unit>);
    EXPECT_FALSE(angle_unit<unitless_unit_type{}>);
}

TEST(TestUnit, TestUnitEquality)
{
    unit auto        u  = meter_unit;
    const unit auto  u2 = meter_unit;
    const unit auto& u3 = meter_unit;

    EXPECT_EQ(u, u);
    EXPECT_EQ(u, u2);
    EXPECT_EQ(u2, u);
    EXPECT_EQ(u, u3);
    EXPECT_EQ(u3, u);
    EXPECT_EQ(u2, u3);
    EXPECT_EQ(u3, u2);

    EXPECT_NE(meter_unit, kilometer_unit);
    EXPECT_FALSE(u != u);
    EXPECT_FALSE(u != u2);
    EXPECT_FALSE(u2 != u);
    EXPECT_FALSE(u != u3);
    EXPECT_FALSE(u3 != u);
    EXPECT_FALSE(u2 != u3);
    EXPECT_FALSE(u3 != u2);
}

TEST(TestUnit, TestUnitConversionOffset)
{
    // Amount
    const double conversion1 = conversion_factor(foot_unit, mile_unit);
    const double conversion2 = conversion_factor(mile_unit, foot_unit);
    const double conversion3 = conversion_factor(foot_unit, meter_unit);
    const double conversion4 = conversion_factor(meter_unit, foot_unit);

    EXPECT_FLOAT_EQ(conversion1, 1.0 / 5'280.0);
    EXPECT_FLOAT_EQ(conversion2, 5'280.0);
    EXPECT_FLOAT_EQ(conversion3, 0.3048);
    EXPECT_FLOAT_EQ(conversion4, 1.0 / 0.3048);

    const double conversion5 = conversion_factor(kilogram_unit, pound_unit);
    const double conversion6 = conversion_factor(pound_unit, kilogram_unit);

    EXPECT_FLOAT_EQ(conversion5, 2.2046226);
    EXPECT_FLOAT_EQ(conversion6, 1.0 / 2.2046226);

    const double conversion7 = conversion_factor(year_unit, day_unit);
    const double conversion8 = conversion_factor(day_unit, year_unit);

    EXPECT_FLOAT_EQ(conversion7, 365.0);
    EXPECT_FLOAT_EQ(conversion8, 1.0 / 365.0);

    const unit auto from = meter_unit / second_unit;
    const unit auto to   = mile_unit / hour_unit;

    const double conversion9 = conversion_factor(from, to);

    EXPECT_FLOAT_EQ(conversion9, 2.23694);
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
    ::testing::Types<std::pair<mole_unit_type, ampere_unit_type>, std::pair<ampere_unit_type, candela_unit_type>,
                     std::pair<candela_unit_type, gram_unit_type>, std::pair<gram_unit_type, kelvin_unit_type>,
                     std::pair<kelvin_unit_type, second_unit_type>, std::pair<second_unit_type, radian_unit_type>,
                     std::pair<radian_unit_type, unitless_unit_type>>;
TYPED_TEST_SUITE(UnitIncompatabilityTest, IncompatibleUnits);

TYPED_TEST(UnitIncompatabilityTest, TestUnitIncompatability)
{
    constexpr auto first  = TestFixture::first;
    constexpr auto second = TestFixture::second;

    EXPECT_FALSE((unit_convertible_to<first, second>));
    EXPECT_FALSE((unit_convertible_to<second, first>));
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
    ::testing::Types<std::pair<mole_unit_type, kilomole_unit_type>, std::pair<ampere_unit_type, kiloampere_unit_type>,
                     std::pair<meter_unit_type, kilometer_unit_type>,
                     std::pair<candela_unit_type, kilocandela_unit_type>, std::pair<gram_unit_type, kilogram_unit_type>,
                     std::pair<kelvin_unit_type, kilokelvin_unit_type>,
                     std::pair<second_unit_type, kilosecond_unit_type>,
                     std::pair<radian_unit_type, kiloradian_unit_type>, std::pair<radian_unit_type, degree_unit_type>>;
TYPED_TEST_SUITE(UnitCompatabilityTest, CompatibleUnits);

TYPED_TEST(UnitCompatabilityTest, TestCompatibleUnits)
{
    constexpr auto first  = TestFixture::first;
    constexpr auto second = TestFixture::second;

    EXPECT_TRUE((unit_convertible_to<first, second>));
    EXPECT_TRUE((unit_convertible_to<second, first>));
}

TYPED_TEST(UnitCompatabilityTest, TestUnitPrefixConversion)
{
    constexpr auto first  = TestFixture::first;
    constexpr auto second = TestFixture::second;

    double conversion1 = conversion_factor(first, second);
    double conversion2 = conversion_factor(second, first);

    if constexpr (!internal::similar<decltype(second), degree_unit_type>)
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