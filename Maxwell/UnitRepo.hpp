#ifndef UNIT_REPO_HPP
#define UNIT_REPO_HPP

#include "Dimension.hpp"
#include "Unit.hpp"

/// @file UnitRepo.hpp Contians the definition of common units

namespace Maxwell {
// --- SI Base Units ---

/// Unit reprsenting Mole, SI base unit for amount.
constexpr UnitType<CoherentDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension>
    MoleUnit{};
/// Type alias for MoleUnit's type
using MoleUnitType = decltype(MoleUnit);

/// Unit representing Ampere, SI base unit for current.
constexpr UnitType<NullDimension, CoherentDimension, NullDimension,
                   NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension>
    AmpereUnit{};
/// Type alias for AmpereUnit's type
using AmpereUnitType = decltype(AmpereUnit);

/// Unit reprsenting Meter, SI base unit for length.
constexpr UnitType<NullDimension, NullDimension, CoherentDimension,
                   NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension>
    MeterUnit{};
/// Type alias for MeterUnit's type
using MeterUnitType = decltype(MeterUnit);

/// Unit reprsenting Candela, SI base unit for luminosity.
constexpr UnitType<NullDimension, NullDimension, NullDimension,
                   CoherentDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension>
    CandelaUnit{};
using CandelaUnitType = decltype(CandelaUnit);

/// Unit representin Gram. This is not the SI base unit for mass,
/// but for ease of implementation, is Maxwell's base unit for mass.
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension,
                   CoherentDimension, NullDimension, NullDimension,
                   NullDimension>
    GramUnit{};
/// Type alias for GramUnit's type
using GramUnitType = decltype(GramUnit);

/// Unit representing Kelvin, SI base unit for temperature.
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, CoherentDimension, NullDimension,
                   NullDimension>
    KelvinUnit{};
/// Type alias for KelvinUnit's type
using KelvinUnitType = decltype(KelvinUnit);

/// Unit representing Second, SI base unit for time.
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension, CoherentDimension,
                   NullDimension>
    SecondUnit{};
/// Type alias for SecondUnit's type
using SecondUnitType = decltype(SecondUnit);

/// Unit representing Radian. This is not an SI base unit,
/// but for ease of implementation, is a base unit in Maxwell
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension, NullDimension,
                   CoherentDimension>
    RadianUnit{};
/// Type alias for RadianUnit's type
using RadianUnitType = decltype(RadianUnit);

/// Unit representing the abscence of a unit. Exists to distinguish between
/// dimensionless quantities like Radians and quantities with no unit, e.g. m/m
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension, NullDimension, NullDimension>
    UnitlessUnit{};
/// Type alias for UnitlessUnit's type.
using UnitlessUnitType = decltype(UnitlessUnit);

/// --- SI derive units ---

constexpr std::intmax_t Quetta = 30;
constexpr std::intmax_t Ronna  = 27;
constexpr std::intmax_t Yotta  = 24;
constexpr std::intmax_t Zetta  = 21;
constexpr std::intmax_t Exa    = 18;
constexpr std::intmax_t Peta   = 15;
constexpr std::intmax_t Tera   = 12;
constexpr std::intmax_t Giga   = 9;
constexpr std::intmax_t Mega   = 6;
constexpr std::intmax_t Kilo   = 3;
constexpr std::intmax_t Hecto  = 2;
constexpr std::intmax_t Deca   = 1;
constexpr std::intmax_t Deci   = -1;
constexpr std::intmax_t Centi  = -2;
constexpr std::intmax_t Milli  = -3;
constexpr std::intmax_t Micro  = -6;
constexpr std::intmax_t Nano   = -9;
constexpr std::intmax_t Pico   = -12;
constexpr std::intmax_t Femto  = -15;
constexpr std::intmax_t Atto   = -18;
constexpr std::intmax_t Zepto  = -21;
constexpr std::intmax_t Yocto  = -24;
constexpr std::intmax_t Ronto  = -27;
constexpr std::intmax_t Quecto = -30;

/// Given a unit, makes derived units with all metrix prefixes where
/// the prefix is applied to the specified dimension. This should be a dimension
/// used by the unit (e.g. mass for Newton not current). The units have the name
/// PrefixUnitName.
///
/// Example:
///     MAKE_UNIT_PREFIXES(MeterUnit, Length) generates the units
///     QuettaMeterUnit,
///         RonnaMeterUnit, etc.
#define MAKE_UNIT_PREFIXES(UnitName, Dimension)                                \
    constexpr auto Quetta##UnitName =                                          \
        UnitName.adjustPrefix##Dimension<Quetta>();                            \
    using Quetta##UnitName##Type = decltype(Quetta##UnitName);                 \
    constexpr auto Ronna##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Ronna>();                             \
    using Ronna##UnitName##Type = decltype(Ronna##UnitName);                   \
    constexpr auto Yotta##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Yotta>();                             \
    using Yotta##UnitName##Type = decltype(Yotta##UnitName);                   \
    constexpr auto Zetta##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Zetta>();                             \
    using Zetta##UnitName##Type   = decltype(Zetta##UnitName);                 \
    constexpr auto Exa##UnitName  = UnitName.adjustPrefix##Dimension<Exa>();   \
    using Exa##UnitName##Type     = decltype(Exa##UnitName);                   \
    constexpr auto Peta##UnitName = UnitName.adjustPrefix##Dimension<Peta>();  \
    using Peta##UnitName##Type    = decltype(Peta##UnitName);                  \
    constexpr auto Tera##UnitName = UnitName.adjustPrefix##Dimension<Tera>();  \
    using Tera##UnitName##Type    = decltype(Tera##UnitName);                  \
    constexpr auto Giga##UnitName = UnitName.adjustPrefix##Dimension<Giga>();  \
    using Giga##UnitName##Type    = decltype(Giga##UnitName);                  \
    constexpr auto Mega##UnitName = UnitName.adjustPrefix##Dimension<Mega>();  \
    using Mega##UnitName##Type    = decltype(Mega##UnitName);                  \
    constexpr auto Kilo##UnitName = UnitName.adjustPrefix##Dimension<Kilo>();  \
    using Kilo##UnitName##Type    = decltype(Kilo##UnitName);                  \
    constexpr auto Hecto##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Hecto>();                             \
    using Hecto##UnitName##Type   = decltype(Hecto##UnitName);                 \
    constexpr auto Deca##UnitName = UnitName.adjustPrefix##Dimension<Deca>();  \
    using Deca##UnitName##Type    = decltype(Deca##UnitName);                  \
    constexpr auto Deci##UnitName = UnitName.adjustPrefix##Dimension<Deci>();  \
    using Deci##UnitName##Type    = decltype(Deci##UnitName);                  \
    constexpr auto Centi##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Centi>();                             \
    using Centi##UnitName##Type = decltype(Centi##UnitName);                   \
    constexpr auto Milli##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Milli>();                             \
    using Milli##UnitName##Type = decltype(Milli##UnitName);                   \
    constexpr auto Micro##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Micro>();                             \
    using Micro##UnitName##Type   = decltype(Micro##UnitName);                 \
    constexpr auto Nano##UnitName = UnitName.adjustPrefix##Dimension<Nano>();  \
    using Nano##UnitName##Type    = decltype(Nano##UnitName);                  \
    constexpr auto Pico##UnitName = UnitName.adjustPrefix##Dimension<Pico>();  \
    using Pico##UnitName##Type    = decltype(Pico##UnitName);                  \
    constexpr auto Femto##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Femto>();                             \
    using Femto##UnitName##Type   = decltype(Femto##UnitName);                 \
    constexpr auto Atto##UnitName = UnitName.adjustPrefix##Dimension<Atto>();  \
    using Atto##UnitName##Type    = decltype(Atto##UnitName);                  \
    constexpr auto Zepto##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Zepto>();                             \
    using Zepto##UnitName##Type = decltype(Zepto##UnitName);                   \
    constexpr auto Yocto##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Yocto>();                             \
    using Yocto##UnitName##Type = decltype(Yocto##UnitName);                   \
    constexpr auto Ronto##UnitName =                                           \
        UnitName.adjustPrefix##Dimension<Ronto>();                             \
    using Ronto##UnitName##Type = decltype(Ronto##UnitName);                   \
    constexpr auto Quecto##UnitName =                                          \
        UnitName.adjustPrefix##Dimension<Quecto>();                            \
    using Quecto##UnitName##Type = decltype(Quecto##UnitName)

/// Unit representing Degrees. Constructed from Radians.
constexpr auto DegreeUnit =
    RadianUnit.adjustScaleAngle<std::ratio<5'729'577'913, 100'000'000>>();
using DegreeUnitType = decltype(DegreeUnit);

MAKE_UNIT_PREFIXES(MoleUnit, Amount);
MAKE_UNIT_PREFIXES(AmpereUnit, Current);
MAKE_UNIT_PREFIXES(MeterUnit, Length);
MAKE_UNIT_PREFIXES(CandelaUnit, Luminosity);
MAKE_UNIT_PREFIXES(GramUnit, Mass);
MAKE_UNIT_PREFIXES(KelvinUnit, Temperature);
MAKE_UNIT_PREFIXES(SecondUnit, Time);
MAKE_UNIT_PREFIXES(RadianUnit, Angle);

/// Creates a new unit from the multiple and division of existing units.
#define MAKE_UNIT(UnitName, UnitDef)                                           \
    constexpr auto UnitName = UnitDef;                                         \
    using UnitName##Type    = decltype(UnitName)

// clang format off
MAKE_UNIT(SqMeterUnit, MeterUnit *MeterUnit);
MAKE_UNIT(CuMeterUnit, SqMeterUnit *MeterUnit);
MAKE_UNIT(MeterPerSecondUnit, MeterUnit / SecondUnit);
MAKE_UNIT(MeterPerSecondPerSecondUnit, MeterPerSecondUnit / SecondUnit);

MAKE_UNIT(HertzUnit, UnitlessUnit / SecondUnit);
MAKE_UNIT(NewtonUnit, KiloGramUnit *MeterUnit / SecondUnit / SecondUnit);
MAKE_UNIT(PascalUnit, NewtonUnit / SqMeterUnit);
MAKE_UNIT(JouleUnit, NewtonUnit *MeterUnit);
MAKE_UNIT(WattUnit, JouleUnit / SecondUnit);
MAKE_UNIT(CoulombUnit, SecondUnit *AmpereUnit);
MAKE_UNIT(VoltUnit, WattUnit / AmpereUnit);
MAKE_UNIT(FaradUnit, CoulombUnit / VoltUnit);
MAKE_UNIT(OhmUnit, VoltUnit / AmpereUnit);
MAKE_UNIT(SiemensUnit, UnitlessUnit / OhmUnit);
MAKE_UNIT(WeberUnit, VoltUnit *SecondUnit);
MAKE_UNIT(TeslaUnit, WeberUnit / SqMeterUnit);
MAKE_UNIT(HenryUnit, WeberUnit / AmpereUnit);

constexpr auto BecquerelUnit = HertzUnit.addTag<UnitTagType{1}>();
using BecquerelUnitType      = decltype(BecquerelUnit);

MAKE_UNIT(GrayUnit, JouleUnit / KiloGramUnit);

constexpr auto SievertUnit = GrayUnit.addTag<UnitTagType{1}>();
using SievertUnitType      = decltype(SievertUnit);

MAKE_UNIT(KatalUnit, MoleUnit / SecondUnit);

constexpr auto SteradianUnit = RadianUnit.addTag<UnitTagType{1}>();
static_assert(SteradianUnit != RadianUnit);

MAKE_UNIT(LumenUnit, CandelaUnit *SteradianUnit);
MAKE_UNIT(LuxUnit, LumenUnit / SqMeterUnit);
// clang format on

MAKE_UNIT_PREFIXES(HertzUnit, Time);
MAKE_UNIT_PREFIXES(NewtonUnit, Mass);
MAKE_UNIT_PREFIXES(PascalUnit, Mass);
MAKE_UNIT_PREFIXES(JouleUnit, Mass);
MAKE_UNIT_PREFIXES(WattUnit, Mass);
MAKE_UNIT_PREFIXES(CoulombUnit, Time);
MAKE_UNIT_PREFIXES(VoltUnit, Mass);
MAKE_UNIT_PREFIXES(FaradUnit, Time);
MAKE_UNIT_PREFIXES(OhmUnit, Mass);
MAKE_UNIT_PREFIXES(SiemensUnit, Time);
MAKE_UNIT_PREFIXES(WeberUnit, Mass);
MAKE_UNIT_PREFIXES(TeslaUnit, Mass);
MAKE_UNIT_PREFIXES(HenryUnit, Mass);
MAKE_UNIT_PREFIXES(LumenUnit, Luminosity);
MAKE_UNIT_PREFIXES(LuxUnit, Luminosity);
MAKE_UNIT_PREFIXES(BecquerelUnit, Time);
MAKE_UNIT_PREFIXES(GrayUnit, Mass);
MAKE_UNIT_PREFIXES(SievertUnit, Mass);
MAKE_UNIT_PREFIXES(KatalUnit, Amount);

/// -- Derived Time Units ---
constexpr auto MinuteUnit = SecondUnit.adjustScaleTime<std::ratio<60>>();
using MinuteUnitType      = decltype(MinuteUnit);

constexpr auto HourUnit = MinuteUnit.adjustScaleTime<std::ratio<60>>();
using HourUnitType      = decltype(HourUnit);

constexpr auto DayUnit = HourUnit.adjustScaleTime<std::ratio<24>>();
using DayUnitType      = decltype(DayUnit);

constexpr auto YearUnit = DayUnit.adjustScaleTime<std::ratio<365>>();
using YearUnitType      = decltype(YearUnit);

constexpr auto DecadeUnit = YearUnit.adjustScaleTime<std::ratio<10>>();
using DecadeUnitType      = decltype(DecadeUnit);

constexpr auto CenturyUnit = YearUnit.adjustScaleTime<std::ratio<100>>();
using CenturyUnitType      = decltype(CenturyUnit);

constexpr auto MilleniumUnit = YearUnit.adjustScaleTime<std::ratio<1'000>>();
using MilleniumUnitType      = decltype(MilleniumUnit);

// -- Derived Length Units
constexpr auto FootUnit =
    MeterUnit.adjustScaleLength<std::ratio<328'084, 100'000>>();
using FootUnitType = decltype(FootUnit);

constexpr auto InchUnit = FootUnit.adjustScaleLength<std::ratio<12>>();
using InchUnitType      = decltype(InchUnit);

constexpr auto YardUnit = FootUnit.adjustScaleLength<std::ratio<1, 3>>();
using YardUnitType      = decltype(YardUnit);

constexpr auto MileUnit = FootUnit.adjustScaleLength<std::ratio<1, 5'280>>();
using MileUnitType      = decltype(MileUnit);

// -- Derived Mass Units
constexpr auto PoundMassUnit =
    KiloGramUnit.adjustScaleMass<std::ratio<220'462, 100'000>>();
}   // namespace Maxwell

#endif