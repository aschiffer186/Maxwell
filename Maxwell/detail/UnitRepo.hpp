#ifndef UNIT_REPO_HPP
#define UNIT_REPO_HPP

#include "Dimension.hpp"
#include "Unit.hpp"

/// @file UnitRepo.hpp Contians the definition of common units

namespace Maxwell {
// --- SI Base Units ---

/// Unit reprsenting Mole, SI base unit for amount.
constexpr UnitType<CoherentDimension, NullDimension, NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension>
    MoleUnit{};
/// Type alias for MoleUnit's type
using MoleUnitType                                        = decltype(MoleUnit);
template <> inline const std::string unitString<MoleUnit> = "mol";

/// Unit representing Ampere, SI base unit for current.
constexpr UnitType<NullDimension, CoherentDimension, NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension>
    AmpereUnit{};
/// Type alias for AmpereUnit's type
using AmpereUnitType                                        = decltype(AmpereUnit);
template <> inline const std::string unitString<AmpereUnit> = "mol";
/// Unit reprsenting Meter, SI base unit for length.
constexpr UnitType<NullDimension, NullDimension, CoherentDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension>
    MeterUnit{};
/// Type alias for MeterUnit's type
using MeterUnitType                                        = decltype(MeterUnit);
template <> inline const std::string unitString<MeterUnit> = "m";

/// Unit reprsenting Candela, SI base unit for luminosity.
constexpr UnitType<NullDimension, NullDimension, NullDimension, CoherentDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension>
    CandelaUnit{};
using CandelaUnitType                                        = decltype(CandelaUnit);
template <> inline const std::string unitString<CandelaUnit> = "cd";

/// Unit representin Gram. This is not the SI base unit for mass,
/// but for ease of implementation, is Maxwell's base unit for mass.
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension, CoherentDimension, NullDimension,
                   NullDimension, NullDimension>
    GramUnit{};
/// Type alias for GramUnit's type
using GramUnitType                                        = decltype(GramUnit);
template <> inline const std::string unitString<GramUnit> = "g";

/// Unit representing Kelvin, SI base unit for temperature.
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension, NullDimension, CoherentDimension,
                   NullDimension, NullDimension>
    KelvinUnit{};
/// Type alias for KelvinUnit's type
using KelvinUnitType                                        = decltype(KelvinUnit);
template <> inline const std::string unitString<KelvinUnit> = "K";

/// Unit representing Second, SI base unit for time.
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension, NullDimension, NullDimension,
                   CoherentDimension, NullDimension>
    SecondUnit{};
/// Type alias for SecondUnit's type
using SecondUnitType                                        = decltype(SecondUnit);
template <> inline const std::string unitString<SecondUnit> = "s";

/// Unit representing Radian. This is not an SI base unit,
/// but for ease of implementation, is a base unit in Maxwell
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, CoherentDimension>
    RadianUnit{};
/// Type alias for RadianUnit's type
using RadianUnitType                                        = decltype(RadianUnit);
template <> inline const std::string unitString<RadianUnit> = "rad";

/// Unit representing the abscence of a unit. Exists to distinguish between
/// dimensionless quantities like Radians and quantities with no unit, e.g. m/m
constexpr UnitType<NullDimension, NullDimension, NullDimension, NullDimension, NullDimension, NullDimension,
                   NullDimension, NullDimension>
    UnitlessUnit{};
/// Type alias for UnitlessUnit's type.
using UnitlessUnitType                                        = decltype(UnitlessUnit);
template <> inline const std::string unitString<UnitlessUnit> = "";

/// Creates a new unit from the multiple and division of existing units.
/// This macro creates a constexpr variable intialized from the specified
/// expression and a type alias for the new unit.
///
/// @param UnitName the name of the new unit
/// @param UnitDef an expression for the definition of th new unit
#define MAKE_UNIT(UnitName, UnitDef)                                                                                   \
    constexpr auto UnitName = UnitDef;                                                                                 \
    using UnitName##Type    = decltype(UnitName);

/// Creates a new by scaling an existing unit. This macro creates a
/// constexpr variable initialized by scaling the specified unit
/// and a type alias for the new unit
///
/// @param UnitName the name of the new unit
/// @param BaseUnit the unit to scale
/// @param Dim the dimension to scale
/// @param Num the numerator of the scale factor
/// @param Den the denominator of the scale factor
#define MAKE_UNIT_SCALE(UnitName, BaseUnit, Dim, Num, Den)                                                             \
    constexpr auto UnitName = BaseUnit.adjustScale##Dim<std::ratio<Num, Den>>();                                       \
    using UnitName##Type    = decltype(UnitName);

/// Creates a new by scaling an existing unit. This macro creates a
/// constexpr variable initialized by scaling the specified unit
/// and a type alias for the new unit. Also specifies a string
/// representation for the new unit.
///
/// @param UnitName the name of the new unit
/// @param BaseUnit the unit to scale
/// @param Dim the dimension to scale
/// @param Num the numerator of the scale factor
/// @param Den the denominator of the scale factor
/// @param UnitString the string represenation of the unit
#define MAKE_UNIT_SCALE_STRING(UnitName, BaseUnit, Dim, Num, Den, UnitString)                                          \
    MAKE_UNIT_SCALE(UnitName, BaseUnit, Dim, Num, Den)                                                                 \
    template <> inline const std::string unitString<UnitName> = UnitString;

/// Creates a new unit from the multiple and division of existing units.
/// This macro creates a constexpr variable intialized from the specified
/// expression and a type alias for the new unit. Also specializes
/// unitString so the new unit has the given representation.
///
/// @param UnitName the name of the new unit
/// @param UnitDef an expression for the definition of th new unit
/// @param UnitRep the representation of the new unit (provided as a string
/// literal).
#define MAKE_UNIT_STRING(UnitName, UnitDef, UnitRep)                                                                   \
    MAKE_UNIT(UnitName, UnitDef)                                                                                       \
    template <> inline const std::string unitString<UnitName> = UnitRep;

/// Creates a new unit type that has the same dimensions as an existing unit.
/// This can be used for units that are distinct but have the same dimensionality
/// e.g. hertz and becquerel.
///
/// @param UnitName the name of the new unit
/// @param BaseUnit the unit being used to create the new unit
#define MAKE_UNIT_TAG(UnitName, BaseUnit)                                                                              \
    struct UnitName##Tag {};                                                                                           \
    constexpr auto UnitName = BaseUnit.addTag<UnitName##Tag>();                                                        \
    using UnitName##Type    = decltype(UnitName);

/// Creates a new unit type that has the same dimensions as an existing unit.
/// This can be used for units that are distinct but have the same dimensionality
/// e.g. hertz and becquerel. Additionally specifies a string representation for
/// the new unit.
///
/// @param UnitName the name of the new unit
/// @param BaseUnit the unit being used to create the new unit
/// @param UnitRep the string representation of the unit
#define MAKE_UNIT_TAG_STRING(UnitName, BaseUnit, UnitRep)                                                              \
    MAKE_UNIT_TAG(UnitName, BaseUnit);                                                                                 \
    template <> inline const std::string unitString<UnitName> = UnitRep;

#ifndef NO_PREDIFNED_DERIVED_UNITS
/// Quetta prefix(10^30)
constexpr std::intmax_t Quetta = 30;
/// Ronna prefix (10^27)
constexpr std::intmax_t Ronna = 27;
/// Yotta prefix (10^24)
constexpr std::intmax_t Yotta = 24;
/// Zetta prefix (10^21)
constexpr std::intmax_t Zetta = 21;
/// Exa prefix (10^18)
constexpr std::intmax_t Exa = 18;
/// Peta prefix (10^15)
constexpr std::intmax_t Peta = 15;
/// Tera prefix (10^12)
constexpr std::intmax_t Tera = 12;
/// Giga prefix (10^9)
constexpr std::intmax_t Giga = 9;
/// Mega prefix (10^6)
constexpr std::intmax_t Mega = 6;
/// Kilo prefix (10^3)
constexpr std::intmax_t Kilo = 3;
/// Hecto prefix (10^2)
constexpr std::intmax_t Hecto = 2;
/// Deca/Deka prefix (10^1)
constexpr std::intmax_t Deca = 1;
/// Deci prefix (10^-1)
constexpr std::intmax_t Deci = -1;
/// Centi prefix (10^-2)
constexpr std::intmax_t Centi = -2;
/// Milli prefix (10^-3)
constexpr std::intmax_t Milli = -3;
/// Micro prefix (10^-6)
constexpr std::intmax_t Micro = -6;
/// Nano prefix (10^-9)
constexpr std::intmax_t Nano = -9;
/// Pico prefix (10^-12)
constexpr std::intmax_t Pico = -12;
/// Femto prefix (10^-15)
constexpr std::intmax_t Femto = -15;
/// Atto prefix (10^-18)
constexpr std::intmax_t Atto = -18;
/// Zepto prefix (10^-21)
constexpr std::intmax_t Zepto = -21;
/// Yocto prefix (10^-24)
constexpr std::intmax_t Yocto = -24;
/// Ronto prefix (10^-27)
constexpr std::intmax_t Ronto = -27;
/// Quecto prefix (10^-30)
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
/// @param UnitName the unit to generate prefixes for
/// @param Dimension the dimensoion that will be prefixed
#define MAKE_UNIT_PREFIXES(UnitName, Dimension)                                                                        \
    constexpr auto Quetta##UnitName                                   = UnitName.adjustPrefix##Dimension<Quetta>();    \
    using Quetta##UnitName##Type                                      = decltype(Quetta##UnitName);                    \
    template <> inline const std::string unitString<Quetta##UnitName> = std::string{"Q"}.append(unitString<UnitName>); \
    constexpr auto Ronna##UnitName                                    = UnitName.adjustPrefix##Dimension<Ronna>();     \
    using Ronna##UnitName##Type                                       = decltype(Ronna##UnitName);                     \
    template <> inline const std::string unitString<Ronna##UnitName>  = std::string{"R"}.append(unitString<UnitName>); \
    constexpr auto Yotta##UnitName                                    = UnitName.adjustPrefix##Dimension<Yotta>();     \
    using Yotta##UnitName##Type                                       = decltype(Yotta##UnitName);                     \
    template <> inline const std::string unitString<Yotta##UnitName>  = std::string{"Y"}.append(unitString<UnitName>); \
    constexpr auto Zetta##UnitName                                    = UnitName.adjustPrefix##Dimension<Zetta>();     \
    using Zetta##UnitName##Type                                       = decltype(Zetta##UnitName);                     \
    template <> inline const std::string unitString<Zetta##UnitName>  = std::string{"Z"}.append(unitString<UnitName>); \
    constexpr auto Exa##UnitName                                      = UnitName.adjustPrefix##Dimension<Exa>();       \
    using Exa##UnitName##Type                                         = decltype(Exa##UnitName);                       \
    template <> inline const std::string unitString<Exa##UnitName>    = std::string{"E"}.append(unitString<UnitName>); \
    constexpr auto Peta##UnitName                                     = UnitName.adjustPrefix##Dimension<Peta>();      \
    using Peta##UnitName##Type                                        = decltype(Peta##UnitName);                      \
    template <> inline const std::string unitString<Peta##UnitName>   = std::string{"P"}.append(unitString<UnitName>); \
    constexpr auto Tera##UnitName                                     = UnitName.adjustPrefix##Dimension<Tera>();      \
    using Tera##UnitName##Type                                        = decltype(Tera##UnitName);                      \
    template <> inline const std::string unitString<Tera##UnitName>   = std::string{"T"}.append(unitString<UnitName>); \
    constexpr auto Giga##UnitName                                     = UnitName.adjustPrefix##Dimension<Giga>();      \
    using Giga##UnitName##Type                                        = decltype(Giga##UnitName);                      \
    template <> inline const std::string unitString<Giga##UnitName>   = std::string{"G"}.append(unitString<UnitName>); \
    constexpr auto Mega##UnitName                                     = UnitName.adjustPrefix##Dimension<Mega>();      \
    using Mega##UnitName##Type                                        = decltype(Mega##UnitName);                      \
    template <> inline const std::string unitString<Mega##UnitName>   = std::string{"M"}.append(unitString<UnitName>); \
    constexpr auto Kilo##UnitName                                     = UnitName.adjustPrefix##Dimension<Kilo>();      \
    using Kilo##UnitName##Type                                        = decltype(Kilo##UnitName);                      \
    template <> inline const std::string unitString<Kilo##UnitName>   = std::string{"k"}.append(unitString<UnitName>); \
    constexpr auto Hecto##UnitName                                    = UnitName.adjustPrefix##Dimension<Hecto>();     \
    using Hecto##UnitName##Type                                       = decltype(Hecto##UnitName);                     \
    template <> inline const std::string unitString<Hecto##UnitName>  = std::string{"h"}.append(unitString<UnitName>); \
    constexpr auto Deca##UnitName                                     = UnitName.adjustPrefix##Dimension<Deca>();      \
    using Deca##UnitName##Type                                        = decltype(Deca##UnitName);                      \
    template <> inline const std::string unitString<Deca##UnitName>   = std::string{"D"}.append(unitString<UnitName>); \
    constexpr auto Deci##UnitName                                     = UnitName.adjustPrefix##Dimension<Deci>();      \
    using Deci##UnitName##Type                                        = decltype(Deci##UnitName);                      \
    template <> inline const std::string unitString<Deci##UnitName>   = std::string{"d"}.append(unitString<UnitName>); \
    constexpr auto Centi##UnitName                                    = UnitName.adjustPrefix##Dimension<Centi>();     \
    using Centi##UnitName##Type                                       = decltype(Centi##UnitName);                     \
    template <> inline const std::string unitString<Centi##UnitName>  = std::string{"c"}.append(unitString<UnitName>); \
    constexpr auto Milli##UnitName                                    = UnitName.adjustPrefix##Dimension<Milli>();     \
    using Milli##UnitName##Type                                       = decltype(Milli##UnitName);                     \
    template <> inline const std::string unitString<Milli##UnitName>  = std::string{"m"}.append(unitString<UnitName>); \
    constexpr auto Micro##UnitName                                    = UnitName.adjustPrefix##Dimension<Micro>();     \
    template <> inline const std::string unitString<Micro##UnitName>  = std::string{"μ"}.append(unitString<UnitName>); \
    using Micro##UnitName##Type                                       = decltype(Micro##UnitName);                     \
    constexpr auto Nano##UnitName                                     = UnitName.adjustPrefix##Dimension<Nano>();      \
    using Nano##UnitName##Type                                        = decltype(Nano##UnitName);                      \
    template <> inline const std::string unitString<Nano##UnitName>   = std::string{"n"}.append(unitString<UnitName>); \
    constexpr auto Pico##UnitName                                     = UnitName.adjustPrefix##Dimension<Pico>();      \
    using Pico##UnitName##Type                                        = decltype(Pico##UnitName);                      \
    template <>                                                                                                        \
    inline const std::string unitString<Pico##UnitName> = std::string{"p"}.append(unitString<Pico##UnitName>);         \
    constexpr auto Femto##UnitName                      = UnitName.adjustPrefix##Dimension<Femto>();                   \
    using Femto##UnitName##Type                         = decltype(Femto##UnitName);                                   \
    template <> inline const std::string unitString<Femto##UnitName>  = std::string{"f"}.append(unitString<UnitName>); \
    constexpr auto Atto##UnitName                                     = UnitName.adjustPrefix##Dimension<Atto>();      \
    using Atto##UnitName##Type                                        = decltype(Atto##UnitName);                      \
    template <> inline const std::string unitString<Atto##UnitName>   = std::string{"a"}.append(unitString<UnitName>); \
    constexpr auto Zepto##UnitName                                    = UnitName.adjustPrefix##Dimension<Zepto>();     \
    using Zepto##UnitName##Type                                       = decltype(Zepto##UnitName);                     \
    template <> inline const std::string unitString<Zepto##UnitName>  = std::string{"z"}.append(unitString<UnitName>); \
    constexpr auto Yocto##UnitName                                    = UnitName.adjustPrefix##Dimension<Yocto>();     \
    using Yocto##UnitName##Type                                       = decltype(Yocto##UnitName);                     \
    template <> inline const std::string unitString<Yocto##UnitName>  = std::string{"y"}.append(unitString<UnitName>); \
    constexpr auto Ronto##UnitName                                    = UnitName.adjustPrefix##Dimension<Ronto>();     \
    using Ronto##UnitName##Type                                       = decltype(Ronto##UnitName);                     \
    template <> inline const std::string unitString<Ronto##UnitName>  = std::string{"r"}.append(unitString<UnitName>); \
    constexpr auto Quecto##UnitName                                   = UnitName.adjustPrefix##Dimension<Quecto>();    \
    using Quecto##UnitName##Type                                      = decltype(Quecto##UnitName);                    \
    template <> inline const std::string unitString<Quecto##UnitName> = std::string{"q"}.append(unitString<UnitName>);

MAKE_UNIT_SCALE_STRING(DegreeUnit, RadianUnit, Angle, 5'729'577'913, 100'000'000, "deg")

MAKE_UNIT_PREFIXES(MoleUnit, Amount)
MAKE_UNIT_PREFIXES(AmpereUnit, Current)
MAKE_UNIT_PREFIXES(MeterUnit, Length)
MAKE_UNIT_PREFIXES(CandelaUnit, Luminosity)
MAKE_UNIT_PREFIXES(GramUnit, Mass)
MAKE_UNIT_PREFIXES(KelvinUnit, Temperature)
MAKE_UNIT_PREFIXES(SecondUnit, Time)
MAKE_UNIT_PREFIXES(RadianUnit, Angle)

// clang format off
MAKE_UNIT(SqMeterUnit, MeterUnit* MeterUnit)
MAKE_UNIT(CuMeterUnit, SqMeterUnit* MeterUnit)
MAKE_UNIT(MeterPerSecondUnit, MeterUnit / SecondUnit)
MAKE_UNIT(MeterPerSecondPerSecondUnit, MeterPerSecondUnit / SecondUnit)

MAKE_UNIT_STRING(HertzUnit, UnitlessUnit / SecondUnit, "Hz")
MAKE_UNIT_STRING(NewtonUnit, KiloGramUnit* MeterUnit / SecondUnit / SecondUnit, "N")
MAKE_UNIT_STRING(PascalUnit, NewtonUnit / SqMeterUnit, "Pa")
MAKE_UNIT_STRING(JouleUnit, NewtonUnit* MeterUnit, "J")
MAKE_UNIT_STRING(WattUnit, JouleUnit / SecondUnit, "W")
MAKE_UNIT_STRING(CoulombUnit, SecondUnit* AmpereUnit, "C")
MAKE_UNIT_STRING(VoltUnit, WattUnit / AmpereUnit, "V")
MAKE_UNIT_STRING(FaradUnit, CoulombUnit / VoltUnit, "F")
MAKE_UNIT_STRING(OhmUnit, VoltUnit / AmpereUnit, "O")
MAKE_UNIT_STRING(SiemensUnit, UnitlessUnit / OhmUnit, "sv")
MAKE_UNIT_STRING(WeberUnit, VoltUnit* SecondUnit, "Wb")
MAKE_UNIT_STRING(TeslaUnit, WeberUnit / SqMeterUnit, "T")
MAKE_UNIT_STRING(HenryUnit, WeberUnit / AmpereUnit, "L")

MAKE_UNIT_TAG(BecquerelUnit, HertzUnit)

MAKE_UNIT_STRING(GrayUnit, JouleUnit / KiloGramUnit, "Gy")
MAKE_UNIT_TAG_STRING(SievertUnit, GrayUnit, "sv")
MAKE_UNIT(KatalUnit, MoleUnit / SecondUnit);
MAKE_UNIT_TAG_STRING(SteradianUnit, RadianUnit, "sd")

MAKE_UNIT(LumenUnit, CandelaUnit* SteradianUnit)
MAKE_UNIT(LuxUnit, LumenUnit / SqMeterUnit)

MAKE_UNIT_PREFIXES(HertzUnit, Time)
MAKE_UNIT_PREFIXES(NewtonUnit, Mass)
MAKE_UNIT_PREFIXES(PascalUnit, Mass)
MAKE_UNIT_PREFIXES(JouleUnit, Mass)
MAKE_UNIT_PREFIXES(WattUnit, Mass)
MAKE_UNIT_PREFIXES(CoulombUnit, Time)
MAKE_UNIT_PREFIXES(VoltUnit, Mass)
MAKE_UNIT_PREFIXES(FaradUnit, Time)
MAKE_UNIT_PREFIXES(OhmUnit, Mass)
MAKE_UNIT_PREFIXES(SiemensUnit, Time)
MAKE_UNIT_PREFIXES(WeberUnit, Mass)
MAKE_UNIT_PREFIXES(TeslaUnit, Mass)
MAKE_UNIT_PREFIXES(HenryUnit, Mass)
MAKE_UNIT_PREFIXES(LumenUnit, Luminosity)
MAKE_UNIT_PREFIXES(LuxUnit, Luminosity)
MAKE_UNIT_PREFIXES(BecquerelUnit, Time)
MAKE_UNIT_PREFIXES(GrayUnit, Mass)
MAKE_UNIT_PREFIXES(SievertUnit, Mass)
MAKE_UNIT_PREFIXES(KatalUnit, Amount)
MAKE_UNIT_PREFIXES(SteradianUnit, Angle)

MAKE_UNIT_SCALE_STRING(MinuteUnit, SecondUnit, Time, 60, 1, "min")
MAKE_UNIT_SCALE_STRING(HourUnit, MinuteUnit, Time, 60, 1, "hr")
MAKE_UNIT_SCALE_STRING(DayUnit, HourUnit, Time, 24, 1, "day")
MAKE_UNIT_SCALE(YearUnit, DayUnit, Time, 365, 1)
MAKE_UNIT_SCALE(DecadeUnit, YearUnit, Time, 10, 1)
MAKE_UNIT_SCALE(CenturyUnit, YearUnit, Time, 100, 1)
MAKE_UNIT_SCALE(MilleniumUnit, YearUnit, Time, 1000, 1)

// -- Derived Length Units

MAKE_UNIT_SCALE_STRING(FootUnit, MeterUnit, Length, 328'084, 100'000, "ft")
MAKE_UNIT_SCALE_STRING(InchUnit, FootUnit, Length, 12, 1, "in")
MAKE_UNIT_SCALE_STRING(YardUnit, FootUnit, Length, 3, 1, "yd")
MAKE_UNIT_SCALE_STRING(MileUnit, FootUnit, Length, 5280, 1, "mi")

// -- Derived Mass Units
MAKE_UNIT_SCALE_STRING(PoundMassUnit, KiloGramUnit, Mass, 220'462, 100'000, "lbm")
/// \cond
#else
// Macros for individual units
#define MAKE_DEGREE_UNIT MAKE_UNIT_SCALE_STRING(DegreeUnit, RadianUnit, Angle, 5'729'577'913,
                       100'000'000, "deg")

#define MAKE_MOLE_UNIT_PREFIXES    MAKE_UNIT_PREFXIES(MoleUnit, Amount)
#define MAKE_AMPERE_UNIT_PREFIXES  MAKE_UNIT_PREFIXES(AmpereUnit, Current)
#define MAKE_METER_UNIT_PREFIXES   MAKE_UNIT_PREFIXES(MeterUnit, Length)
#define MAKE_CANDELA_UNIT_PREFIXES MAKE_UNIT_PREFIXES(CandelaUnit, Luminosity)
#define MAKE_GRAM_UNIT_PREFIXES    MAKE_UNIT_PREFIXES(GramUnit, Mass)
#define MAKE_KELVIN_UNIT_PREFIXES  MAKE_UNIT_PREFIXES(KelvinUnit, Temperature)
#define MAKE_SECOND_UNIT_PREFIXES  MAKE_UNIT_PREFIXES(SecondUnit, Time)
#define MAKE_RADIAN_UNIT_PREFIXES  MAKE_UNIT_PREFIXES(RadianUnit, Angle)

#define MAKE_SQ_METER_UNIT                    MAKE_UNIT(SqMeterUnit, MeterUnit* MeterUnit)
#define MAKE_CU_METER_UNIT                    MAKE_UNIT(CuMeterUnit, SqMeterUnit* MeterUnit)
#define MAKE_METER_PER_SECOND_UNIT            MAKE_UNIT(MeterPerSecondUnit, MeterUnit / SecondUnit)
#define MAKE_METER_PER_SECOND_PER_SECOND_UNIT MAKE_UNIT(MeterPerSecondPerSecondUnit, MeterPerSecondUnit / SecondUnit)
#define MAKE_HERTZ_UNIT                       MAKE_UNIT_STRING(HertzUnit, UnitlessUnit / SecondUnit, "Hz")
#define MAKE_NEWTON_UNIT                      MAKE_UNIT_STRING(NewtonUnit, KiloGramUnit* MeterUnit / SecondUnit / SecondUnit, "N")
#define MAKE_PASCAL_UNIT                      MAKE_UNIT_STRING(PascalUnit, NewtonUnit / SqMeterUnit, "Pa")
#define MAKE_JOULE_UNIT                       MAKE_UNIT_STRING(JouleUnit, NewtonUnit* MeterUnit, "J")
#define MAKE_WATT_UNIT                        MAKE_UNIT_STRING(WattUnit, JouleUnit / SecondUnit, "W")
#define MAKE_COULOMB_UNIT                     MAKE_UNIT_STRING(CoulombUnit, SecondUnit* AmpereUnit, "C")
#define MAKE_VOLT_UNIT                        MAKE_UNIT_STRING(VoltUnit, WattUnit / AmpereUnit, "V")
#define MAKE_FARAD_UNIT                       MAKE_UNIT_STRING(FaradUnit, CoulombUnit / VoltUnit, "F")
#define MAKE_OHM_UNIT                         MAKE_UNIT_STRING(OhmUnit, VoltUnit / AmpereUnit, "O")
#define MAKE_SIEMENS_UNIT                     MAKE_UNIT_STRING(SiemensUnit, UnitlessUnit / OhmUnit, "sv")
#define MAKE_WEBER_UNIT                       MAKE_UNIT_STRING(WeberUnit, VoltUnit* SecondUnit, "Wb")
#define MAKE_TESLA_UNIT                       MAKE_UNIT_STRING(TeslaUnit, WeberUnit / SqMeterUnit, "T")
#define MAKE_HENRY_UNIT                       MAKE_UNIT_STRING(HenryUnit, WeberUnit / AmpereUnit, "L")
#define MAKE_BECQUEREL_UNIT                   MAKE_UNIT_TAG(BecquerelUnit, HertzUnit)

#define MAKE_GRAY_UNIT      MAKE_UNIT_STRING(GrayUnit, JouleUnit / KiloGramUnit, "Gy")
#define MAKE_SIEVERT_UNIT   MAKE_UNIT_TAG_STRING(SievertUnit, GrayUnit, "sv")
#define MAKE_KATAL_UNIT     MAKE_UNIT(KatalUnit, MoleUnit / SecondUnit);
#define MAKE_STERADIAN_UNIT MAKE_UNIT_TAG_STRING(SteradianUnit, RadianUnit, "sd")

#define MAKE_LUMEN_UNIT MAKE_UNIT(LumenUnit, CandelaUnit* SteradianUnit)
#define MAKE_LUX_UNIT   MAKE_UNIT(LuxUnit, LumenUnit / SqMeterUnit)

#define MAKE_MINUTE_UNIT    MAKE_UNIT_SCALE_STRING(MinuteUnit, SecondUnit, Time, 60, 1, "min")
#define MAKE_HOUR_UNIT      MAKE_UNIT_SCALE_STRING(HourUnit, MinuteUnit, Time, 60, 1, "hr")
#define MAKE_DAY_UNIT       MAKE_UNIT_SCALE_STRING(DayUnit, HourUnit, Time, 24, 1, "day")
#define MAKE_YEAR_UNIT      MAKE_UNIT_SCALE(YearUnit, DayUnit, Time, 365, 1)
#define MAKE_DECADE_UNIT    MAKE_UNIT_SCALE(DecadeUnit, YearUnit, Time, 10, 1)
#define MAKE_CENTURY_UNIT   MAKE_UNIT_SCALE(CenturyUnit, YearUnit, Time, 100, 1)
#define MAKE_MILLENIUM_UNIT MAKE_UNIT_SCALE(MilleniumUnit, YearUnit, Time, 1000, 1)

#define MAKE_FOOT_UNIT MAKE_UNIT_SCALE_STRING(FootUnit, MeterUnit, Length, 328'084, 100'000, "ft")
#define MAKE_INCH_UNIT MAKE_UNIT_SCALE_STRING(InchUnit, FootUnit, Length, 12, 1, "in")
#define MAKE_YARD_UNIT MAKE_UNIT_SCALE_STRING(YardUnit, FootUnit, Length, 3, 1, "yd")
#define MAKE_MILE_UNIT MAKE_UNIT_SCALE_STRING(MileUnit, FootUnit, Length, 5280, 1, "mi")

#endif
/// \endcond
}   // namespace Maxwell

#endif