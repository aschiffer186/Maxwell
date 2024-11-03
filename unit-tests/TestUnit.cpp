#include "Unit.hpp"
#include "internal/Measure.hpp"

#include <gtest/gtest.h>

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