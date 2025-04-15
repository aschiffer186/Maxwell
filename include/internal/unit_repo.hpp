#ifndef UNIT_REPO_HPP
#define UNIT_REPO_HPP

#include <string>

#include "dimension.hpp"
#include "unit.hpp"

namespace maxwell {
#define MAKE_UNIT(unit_name, definition)                                                                               \
  struct unit_name##_type : decltype(definition) {                                                                     \
    using base_type = decltype(definition);                                                                            \
    constexpr static std::string unit_string() { return base_type::unit_string(); }                                    \
  };                                                                                                                   \
  constexpr unit_name##_type unit_name##_unit;

#define MAKE_UNIT_PREFIXES(unit_name, dimension)                                                                       \
  struct quetta##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<quetta>()) {        \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<quetta>());                                \
    constexpr static std::string unit_string() { return "Q" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr quetta##unit_name##_type quetta##unit_name;                                                                \
  struct ronna##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<ronna>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<ronna>());                                 \
    constexpr static std::string unit_string() { return "R" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr ronna##unit_name##_type ronna##unit_name;                                                                  \
  struct yotta##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<yotta>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<yotta>());                                 \
    constexpr static std::string unit_string() { return "Y" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr yotta##unit_name##_type yotta##unit_name;                                                                  \
  struct zetta##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<zetta>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<zetta>());                                 \
    constexpr static std::string unit_string() { return "Z" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr zetta##unit_name##_type zetta##unit_name;                                                                  \
  struct exa##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<exa>()) {              \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<exa>());                                   \
    constexpr static std::string unit_string() { return "E" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr exa##unit_name##_type exa##unit_name;                                                                      \
  struct peta##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<peta>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<peta>());                                  \
    constexpr static std::string unit_string() { return "P" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr peta##unit_name##_type peta##unit_name;                                                                    \
  struct tera##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<tera>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<tera>());                                  \
    constexpr static std::string unit_string() { return "T" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr tera##unit_name##_type tera##unit_name;                                                                    \
  struct giga##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<giga>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<giga>());                                  \
    constexpr static std::string unit_string() { return "G" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr giga##unit_name##_type giga##unit_name;                                                                    \
  struct mega##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<mega>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<mega>());                                  \
    constexpr static std::string unit_string() { return "M" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr mega##unit_name##_type mega##unit_name;                                                                    \
  struct kilo##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<kilo>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<kilo>());                                  \
    constexpr static std::string unit_string() { return "k" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr kilo##unit_name##_type kilo##unit_name;                                                                    \
  struct hecto##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<hecto>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<hecto>());                                 \
    constexpr static std::string unit_string() { return "h" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr hecto##unit_name##_type hecto##unit_name;                                                                  \
  struct deca##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<deca>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<deca>());                                  \
    constexpr static std::string unit_string() { return "da" + unit_name##_type::unit_string(); }                      \
  };                                                                                                                   \
  constexpr deca##unit_name##_type deca##unit_name;                                                                    \
  struct deci##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<deci>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<deci>());                                  \
    constexpr static std::string unit_string() { return "d" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr deci##unit_name##_type deci##unit_name;                                                                    \
  struct centi##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<centi>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<centi>());                                 \
    constexpr static std::string unit_string() { return "c" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr centi##unit_name##_type centi##unit_name;                                                                  \
  struct milli##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<milli>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<milli>());                                 \
    constexpr static std::string unit_string() { return "m" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr milli##unit_name##_type milli##unit_name;                                                                  \
  struct micro##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<micro>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<micro>());                                 \
    constexpr static std::string unit_string() { return "µ" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr micro##unit_name##_type micro##unit_name;                                                                  \
  struct nano##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<nano>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<nano>());                                  \
    constexpr static std::string unit_string() { return "n" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr nano##unit_name##_type nano##unit_name;                                                                    \
  struct pico##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<pico>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<pico>());                                  \
    constexpr static std::string unit_string() { return "p" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr pico##unit_name##_type pico##unit_name;                                                                    \
  struct femto##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<femto>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<femto>());                                 \
    constexpr static std::string unit_string() { return "f" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr femto##unit_name##_type femto##unit_name;                                                                  \
  struct atto##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<atto>()) {            \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<atto>());                                  \
    constexpr static std::string unit_string() { return "a" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr atto##unit_name##_type atto##unit_name;                                                                    \
  struct zepto##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<zepto>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<zepto>());                                 \
    constexpr static std::string unit_string() { return "z" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr zepto##unit_name##_type zepto##unit_name;                                                                  \
  struct yocto##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<yocto>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<yocto>());                                 \
    constexpr static std::string unit_string() { return "y" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr yocto##unit_name##_type yocto##unit_name;                                                                  \
  struct ronto##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<ronto>()) {          \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<ronto>());                                 \
    constexpr static std::string unit_string() { return "r" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr ronto##unit_name##_type ronto##unit_name;                                                                  \
  struct quecto##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<quecto>()) {        \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<quecto>());                                \
    constexpr static std::string unit_string() { return "q" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr quecto##unit_name##_type quecto##unit_name;

#define MAKE_UNIT_WITH_DESC(unit_name, definition, desc)                                                               \
  struct unit_name##_unit_type : decltype(definition) {                                                                \
    using base_type = decltype(definition);                                                                            \
    constexpr static std::string unit_string() { return desc; }                                                        \
  };                                                                                                                   \
  constexpr unit_name##_unit_type unit_name##_unit;

#define MAKE_UNIT_PREFXIES_WITH_DESC(unit_name, definition, desc, dimension)                                           \
  MAKE_UNIT_WITH_DESC(unit_name, definition, desc)                                                                     \
  MAKE_UNIT_PREFIXES(unit_name##_unit, dimension)

MAKE_UNIT_PREFIXES(mole_unit, amount)
MAKE_UNIT_PREFIXES(ampere_unit, current)
MAKE_UNIT_PREFIXES(meter_unit, length)
MAKE_UNIT_PREFIXES(candela_unit, luminosity)
MAKE_UNIT_PREFIXES(gram_unit, mass)
MAKE_UNIT_PREFIXES(kelvin_unit, temperature)
MAKE_UNIT_PREFIXES(second_unit, time)

/// \cond
struct radian_unit_tag;
struct degree_unit_tag;
struct steradian_unit_tag;
struct becquerel_unit_tag;
struct sievert_unit_tag;
/// \endcond

MAKE_UNIT_WITH_DESC(radian, scalar_unit.add_tag<radian_unit_tag>(), "rad");
MAKE_UNIT_WITH_DESC(steradian, scalar_unit.add_tag<steradian_unit_tag>(), "sr");
MAKE_UNIT_WITH_DESC(degree, scalar_unit.add_tag<degree_unit_tag>(), "deg");

/// \cond
template <> struct is_tag_convertible<degree_unit_tag, radian_unit_tag> : std::true_type {};
template <> struct is_tag_convertible<radian_unit_tag, degree_unit_tag> : std::true_type {};
template <> struct tag_conversion_factor<radian_unit_type, degree_unit_type> {
  static constexpr double factor = 180.0 / std::numbers::pi;
};
template <> struct tag_conversion_factor<degree_unit_type, radian_unit_type> {
  static constexpr double factor = std::numbers::pi / 180.0;
};
/// \endcond

MAKE_UNIT_PREFXIES_WITH_DESC(hertz, scalar_unit / second_unit, "Hz", time);
MAKE_UNIT_PREFXIES_WITH_DESC(newton, kilogram_unit* meter_unit / second_unit, "N", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(pascal, newton_unit / meter_unit / meter_unit, "Pa", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(joule, newton_unit* meter_unit, "J", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(watt, joule_unit / second_unit, "W", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(coulomb, ampere_unit* second_unit, "C", current)
MAKE_UNIT_PREFXIES_WITH_DESC(volt, watt_unit / ampere_unit, "V", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(farad, coulomb_unit / volt_unit, "F", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(ohm, volt_unit / ampere_unit, "Ω", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(siemens, ampere_unit / volt_unit, "S", current)
MAKE_UNIT_PREFXIES_WITH_DESC(weber, volt_unit* second_unit, "Wb", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(tesla, weber_unit / (meter_unit * meter_unit), "T", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(henry, weber_unit / ampere_unit, "H", mass)
constexpr rational celsisus_offset{-27'315, 100};
MAKE_UNIT_WITH_DESC(Celsisus, kelvin_unit.adjust_offset_temperature<celsisus_offset>(), "°C")
MAKE_UNIT_PREFXIES_WITH_DESC(lumen, candela_unit* steradian_unit, "lm", luminosity)
MAKE_UNIT_PREFXIES_WITH_DESC(lux, lumen_unit / (meter_unit * meter_unit), "lx", luminosity)
MAKE_UNIT_PREFXIES_WITH_DESC(becquerel, hertz_unit.add_tag<becquerel_unit_tag>(), "Bq", time)
MAKE_UNIT_PREFXIES_WITH_DESC(gray, joule_unit / kilogram_unit, "Gy", length)
MAKE_UNIT_PREFXIES_WITH_DESC(sievert, gray_unit.add_tag<sievert_unit_tag>(), "Sv", length)
MAKE_UNIT_PREFXIES_WITH_DESC(katal, mole_unit / second_unit, "kat", amount)

MAKE_UNIT_WITH_DESC(square_quettameter, quettameter_unit* quettameter_unit, "Qm^2")
MAKE_UNIT_WITH_DESC(square_ronnameter, ronnameter_unit* ronnameter_unit, "Rm^2")
MAKE_UNIT_WITH_DESC(square_yottameter, yottameter_unit* yottameter_unit, "Ym^2")
MAKE_UNIT_WITH_DESC(square_zettameter, zettameter_unit* zettameter_unit, "Zm^2")
MAKE_UNIT_WITH_DESC(square_exameter, exameter_unit* exameter_unit, "Em^2")
MAKE_UNIT_WITH_DESC(square_petameter, petameter_unit* petameter_unit, "Pm^2")
MAKE_UNIT_WITH_DESC(square_terameter, terameter_unit* terameter_unit, "Tm^2")
MAKE_UNIT_WITH_DESC(square_gigameter, gigameter_unit* gigameter_unit, "Gm^2")
MAKE_UNIT_WITH_DESC(square_megameter, megameter_unit* megameter_unit, "Mm^2")
MAKE_UNIT_WITH_DESC(square_kilometer, kilometer_unit* kilometer_unit, "km^2")
MAKE_UNIT_WITH_DESC(square_hectometer, hectometer_unit* hectometer_unit, "hm^2")
MAKE_UNIT_WITH_DESC(square_decameter, decameter_unit* decameter_unit, "dam^2")
MAKE_UNIT_WITH_DESC(square_meter, meter_unit* meter_unit, "m^2")
MAKE_UNIT_WITH_DESC(square_decimeter, decimeter_unit* decimeter_unit, "dm^2")
MAKE_UNIT_WITH_DESC(square_centimeter, centimeter_unit* centimeter_unit, "cm^2")
MAKE_UNIT_WITH_DESC(square_millimeter, millimeter_unit* millimeter_unit, "mm^2")
MAKE_UNIT_WITH_DESC(square_micrometer, micrometer_unit* micrometer_unit, "µm^2")
MAKE_UNIT_WITH_DESC(square_nanometer, nanometer_unit* nanometer_unit, "nm^2")
MAKE_UNIT_WITH_DESC(square_picometer, picometer_unit* picometer_unit, "pm^2")
MAKE_UNIT_WITH_DESC(square_femtometer, femtometer_unit* femtometer_unit, "fm^2")
MAKE_UNIT_WITH_DESC(square_attometer, attometer_unit* attometer_unit, "am^2")
MAKE_UNIT_WITH_DESC(square_zeptometer, zeptometer_unit* zeptometer_unit, "zm^2")
MAKE_UNIT_WITH_DESC(square_yoctometer, yoctometer_unit* yoctometer_unit, "ym^2")
MAKE_UNIT_WITH_DESC(square_rontometer, rontometer_unit* rontometer_unit, "rm^2")
MAKE_UNIT_WITH_DESC(square_quectometer, quectometer_unit* quectometer_unit, "qm^2")

MAKE_UNIT_WITH_DESC(cubic_quettameter, quettameter_unit* quettameter_unit* quettameter_unit, "Qm^3")
MAKE_UNIT_WITH_DESC(cubic_ronnameter, ronnameter_unit* ronnameter_unit* ronnameter_unit, "Rm^3")
MAKE_UNIT_WITH_DESC(cubic_yottameter, yottameter_unit* yottameter_unit* yottameter_unit, "Ym^3")
MAKE_UNIT_WITH_DESC(cubic_zettameter, zettameter_unit* zettameter_unit* zettameter_unit, "Zm^3")
MAKE_UNIT_WITH_DESC(cubic_exameter, exameter_unit* exameter_unit* exameter_unit, "Em^3")
MAKE_UNIT_WITH_DESC(cubic_petameter, petameter_unit* petameter_unit* petameter_unit, "Pm^3")
MAKE_UNIT_WITH_DESC(cubic_terameter, terameter_unit* terameter_unit* terameter_unit, "Tm^3")
MAKE_UNIT_WITH_DESC(cubic_gigameter, gigameter_unit* gigameter_unit* gigameter_unit, "Gm^3")
MAKE_UNIT_WITH_DESC(cubic_megameter, megameter_unit* megameter_unit* megameter_unit, "Mm^3")
MAKE_UNIT_WITH_DESC(cubic_kilometer, kilometer_unit* kilometer_unit* kilometer_unit, "km^3")
MAKE_UNIT_WITH_DESC(cubic_hectometer, hectometer_unit* hectometer_unit* hectometer_unit, "hm^3")
MAKE_UNIT_WITH_DESC(cubic_decameter, decameter_unit* decameter_unit* decameter_unit, "dam^3")
MAKE_UNIT_WITH_DESC(cubic_meter, square_meter_unit* meter_unit, "m^3")
MAKE_UNIT_WITH_DESC(cubic_decimeter, decimeter_unit* decimeter_unit* decimeter_unit, "dm^3")
MAKE_UNIT_WITH_DESC(cubic_centimeter, centimeter_unit* centimeter_unit* centimeter_unit, "cm^3")
MAKE_UNIT_WITH_DESC(cubic_millimeter, millimeter_unit* millimeter_unit* millimeter_unit, "mm^3")
MAKE_UNIT_WITH_DESC(cubic_micrometer, micrometer_unit* micrometer_unit* micrometer_unit, "µm^3")
MAKE_UNIT_WITH_DESC(cubic_nanometer, nanometer_unit* nanometer_unit* nanometer_unit, "nm^3")
MAKE_UNIT_WITH_DESC(cubic_picometer, picometer_unit* picometer_unit* picometer_unit, "pm^3")
MAKE_UNIT_WITH_DESC(cubic_femtometer, femtometer_unit* femtometer_unit* femtometer_unit, "fm^3")
MAKE_UNIT_WITH_DESC(cubic_attometer, attometer_unit* attometer_unit* attometer_unit, "am^3")
MAKE_UNIT_WITH_DESC(cubic_zeptometer, zeptometer_unit* zeptometer_unit* zeptometer_unit, "zm^3")
MAKE_UNIT_WITH_DESC(cubic_yoctometer, yoctometer_unit* yoctometer_unit* yoctometer_unit, "ym^3")
MAKE_UNIT_WITH_DESC(cubic_rontometer, rontometer_unit* rontometer_unit* rontometer_unit, "rm^3")
MAKE_UNIT_WITH_DESC(cubic_quectometer, quectometer_unit* quectometer_unit* quectometer_unit, "qm^3")

MAKE_UNIT_WITH_DESC(meter_per_second, meter_unit / second_unit, "m/s")
MAKE_UNIT_WITH_DESC(meter_per_second_per_second, meter_per_second_unit / second_unit, "m/s^2")

template <auto U>
concept angle_unit = unit_convertible_to<U, radian_unit>;

template <auto U>
concept freqency_unit = unit_convertible_to<U, hertz_unit>;

template <auto U>
concept force_unit = unit_convertible_to<U, newton_unit>;

template <auto U>
concept pressure_unit = unit_convertible_to<U, pascal_unit>;

template <auto U>
concept energy_unit = unit_convertible_to<U, joule_unit>;

// Imperial units
constexpr rational meter_to_foot{10'000, 3'048};
constexpr rational foot_to_inch{12, 1};
constexpr rational foot_to_yard{1, 3};
constexpr rational foot_to_mile{1, 5'280};

MAKE_UNIT_WITH_DESC(foot, meter_unit.adjust_scale_length<meter_to_foot>(), "ft")
MAKE_UNIT_WITH_DESC(inch, foot_unit.adjust_scale_length<foot_to_inch>(), "in")
MAKE_UNIT_WITH_DESC(mile, foot_unit.adjust_scale_length<foot_to_mile>(), "mi")
MAKE_UNIT_WITH_DESC(yard, foot_unit.adjust_scale_length<foot_to_yard>(), "yd")

MAKE_UNIT_WITH_DESC(square_foot, foot_unit* foot_unit, "ft^2")
MAKE_UNIT_WITH_DESC(square_inch, inch_unit* inch_unit, "in^2")
MAKE_UNIT_WITH_DESC(square_yard, yard_unit* yard_unit, "yd^2")
MAKE_UNIT_WITH_DESC(square_mile, mile_unit* mile_unit, "mi^2")

MAKE_UNIT_WITH_DESC(cubic_foot, foot_unit* foot_unit* foot_unit, "ft^3")
MAKE_UNIT_WITH_DESC(cubic_inch, inch_unit* inch_unit* inch_unit, "in^3")
MAKE_UNIT_WITH_DESC(cubic_yard, yard_unit* yard_unit* yard_unit, "yd^3")
MAKE_UNIT_WITH_DESC(cubic_mile, mile_unit* mile_unit* mile_unit, "mi^3")

constexpr rational second_to_minute{1, 60};
constexpr rational minute_to_hour{1, 60};
constexpr rational hour_to_day{1, 24};
constexpr rational day_to_year{1, 365};

MAKE_UNIT_WITH_DESC(minute, second_unit.adjust_scale_time<second_to_minute>(), "min")
MAKE_UNIT_WITH_DESC(hour, minute_unit.adjust_scale_time<minute_to_hour>(), "h")
MAKE_UNIT_WITH_DESC(day, hour_unit.adjust_scale_time<hour_to_day>(), "d")
MAKE_UNIT_WITH_DESC(year, day_unit.adjust_scale_time<day_to_year>(), "y")
MAKE_UNIT_WITH_DESC(week, day_unit.adjust_scale_time<7>(), "w")

constexpr rational kilogram_to_pound{100'000'000, 45'359'237};
constexpr rational pound_to_ounce{16, 1};

MAKE_UNIT_WITH_DESC(pound, kilogram_unit.adjust_scale_mass<kilogram_to_pound>(), "lb")
MAKE_UNIT_WITH_DESC(ounce, pound_unit.adjust_scale_mass<pound_to_ounce>(), "oz")

constexpr rational celsisu_to_fahrenheit_scale{9, 5};
constexpr rational celsisus_to_fahrenheit_offset{32};

MAKE_UNIT_WITH_DESC(Fahrenheit,
                    Celsisus_unit.adjust_scale_temperature<celsisu_to_fahrenheit_scale>()
                        .adjust_offset_temperature<celsisus_to_fahrenheit_offset>(),
                    "°F")
// MAKE_UNIT_WITH_DESC(psi, pound_unit / square_inch_unit, "psi")
} // namespace maxwell

#endif