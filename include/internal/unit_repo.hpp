#ifndef UNIT_REPO_HPP
#define UNIT_REPO_HPP

#include <string>

#include "internal/dimension.hpp"
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

struct radian_unit_tag;
struct degree_unit_tag;
struct steradian_unit_tag;

MAKE_UNIT_WITH_DESC(radian, scalar_unit.add_tag<radian_unit_tag>(), "rad");
MAKE_UNIT_WITH_DESC(steradian, scalar_unit.add_tag<degree_unit_tag>(), "sr");
MAKE_UNIT_WITH_DESC(degree, scalar_unit.add_tag<steradian_unit_tag>(), "deg");

MAKE_UNIT_PREFXIES_WITH_DESC(hertz, scalar_unit / second_unit, "Hz", time);

template <> struct is_tag_convertible<degree_unit_type, radian_unit_type> : std::true_type {};

template <> struct is_tag_convertible<radian_unit_type, degree_unit_type> : std::true_type {};

template <> struct tag_conversion_factor<radian_unit_type, degree_unit_type> {
  static constexpr double factor = 180.0 / std::numbers::pi;
};

template <> struct tag_conversion_factor<degree_unit_type, radian_unit_type> {
  static constexpr double factor = std::numbers::pi / 180.0;
};

struct steradian_tag;

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

constexpr rational celsisus_offset{27'315, 100};
MAKE_UNIT_WITH_DESC(Celsisus, kelvin_unit.adjust_offset_temperature<celsisus_offset>(), "°C")

MAKE_UNIT_PREFXIES_WITH_DESC(lumen, candela_unit* steradian_unit, "lm", luminosity)
MAKE_UNIT_PREFXIES_WITH_DESC(lux, lumen_unit / (meter_unit * meter_unit), "lx", luminosity)

struct becquerel_unit_tag;
MAKE_UNIT_PREFXIES_WITH_DESC(becquerel, hertz_unit.add_tag<becquerel_unit_tag>(), "Bq", time)

MAKE_UNIT_PREFXIES_WITH_DESC(gray, joule_unit / kilogram_unit, "Gy", length)

struct sievert_unit_tag;
MAKE_UNIT_PREFXIES_WITH_DESC(sievert, gray_unit.add_tag<sievert_unit_tag>(), "Sv", length)

MAKE_UNIT_PREFXIES_WITH_DESC(katal, mole_unit / second_unit, "kat", amount)

MAKE_UNIT_WITH_DESC(square_meter, meter_unit* meter_unit, "m^2")
MAKE_UNIT_WITH_DESC(cubic_meter, square_meter_unit* meter_unit, "m^3")

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
} // namespace maxwell

#endif