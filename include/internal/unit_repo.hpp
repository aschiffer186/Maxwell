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
  struct quetta_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<quetta>()) {       \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<quetta>());                                \
    constexpr static std::string unit_string() { return "Q" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr quetta_##unit_name##_type quetta_##unit_name;                                                              \
  struct ronna_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<ronna>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<ronna>());                                 \
    constexpr static std::string unit_string() { return "R" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr ronna_##unit_name##_type ronna_##unit_name;                                                                \
  struct yotta_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<yotta>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<yotta>());                                 \
    constexpr static std::string unit_string() { return "Y" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr yotta_##unit_name##_type yotta_##unit_name;                                                                \
  struct zetta_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<zetta>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<zetta>());                                 \
    constexpr static std::string unit_string() { return "Z" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr zetta_##unit_name##_type zetta_##unit_name;                                                                \
  struct exa_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<exa>()) {             \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<exa>());                                   \
    constexpr static std::string unit_string() { return "E" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr exa_##unit_name##_type exa_##unit_name;                                                                    \
  struct peta_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<peta>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<peta>());                                  \
    constexpr static std::string unit_string() { return "P" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr peta_##unit_name##_type peta_##unit_name;                                                                  \
  struct tera_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<tera>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<tera>());                                  \
    constexpr static std::string unit_string() { return "T" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr tera_##unit_name##_type tera_##unit_name;                                                                  \
  struct giga_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<giga>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<giga>());                                  \
    constexpr static std::string unit_string() { return "G" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr giga_##unit_name##_type giga_##unit_name;                                                                  \
  struct mega_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<mega>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<mega>());                                  \
    constexpr static std::string unit_string() { return "M" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr mega_##unit_name##_type mega_##unit_name;                                                                  \
  struct kilo_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<kilo>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<kilo>());                                  \
    constexpr static std::string unit_string() { return "k" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr kilo_##unit_name##_type kilo_##unit_name;                                                                  \
  struct hecto_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<hecto>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<hecto>());                                 \
    constexpr static std::string unit_string() { return "h" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr hecto_##unit_name##_type hecto_##unit_name;                                                                \
  struct deca_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<deca>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<deca>());                                  \
    constexpr static std::string unit_string() { return "da" + unit_name##_type::unit_string(); }                      \
  };                                                                                                                   \
  constexpr deca_##unit_name##_type deca_##unit_name;                                                                  \
  struct deci_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<deci>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<deci>());                                  \
    constexpr static std::string unit_string() { return "d" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr deci_##unit_name##_type deci_##unit_name;                                                                  \
  struct centi_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<centi>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<centi>());                                 \
    constexpr static std::string unit_string() { return "c" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr centi_##unit_name##_type centi_##unit_name;                                                                \
  struct milli_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<milli>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<milli>());                                 \
    constexpr static std::string unit_string() { return "m" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr milli_##unit_name##_type milli_##unit_name;                                                                \
  struct micro_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<micro>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<micro>());                                 \
    constexpr static std::string unit_string() { return "µ" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr micro_##unit_name##_type micro_##unit_name;                                                                \
  struct nano_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<nano>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<nano>());                                  \
    constexpr static std::string unit_string() { return "n" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr nano_##unit_name##_type nano_##unit_name;                                                                  \
  struct pico_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<pico>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<pico>());                                  \
    constexpr static std::string unit_string() { return "p" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr pico_##unit_name##_type pico_##unit_name;                                                                  \
  struct femto_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<femto>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<femto>());                                 \
    constexpr static std::string unit_string() { return "f" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr femto_##unit_name##_type femto_##unit_name;                                                                \
  struct atto_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<atto>()) {           \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<atto>());                                  \
    constexpr static std::string unit_string() { return "a" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr atto_##unit_name##_type atto_##unit_name;                                                                  \
  struct zepto_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<zepto>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<zepto>());                                 \
    constexpr static std::string unit_string() { return "z" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr zepto_##unit_name##_type zepto_##unit_name;                                                                \
  struct yocto_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<yocto>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<yocto>());                                 \
    constexpr static std::string unit_string() { return "y" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr yocto_##unit_name##_type yocto_##unit_name;                                                                \
  struct ronto_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<ronto>()) {         \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<ronto>());                                 \
    constexpr static std::string unit_string() { return "r" + unit_name##_type::unit_string(); }                       \
  };                                                                                                                   \
  constexpr ronto_##unit_name##_type ronto_##unit_name;                                                                \
  struct quecto_##unit_name##_type : decltype(unit_name##_type{}.template adjust_prefix_##dimension<quecto>()) {       \
    using base_type = decltype(unit_name##_type{}.adjust_prefix_##dimension<quecto>());                                \
    constexpr static std::string unit_string() { return "q" + unit_name##_type::unit_string(); }                       \
  };

#define MAKE_UNIT_WITH_DESC(unit_name, definition, desc)                                                               \
  struct unit_name##_unit_type : decltype(definition) {                                                                \
    using base_type = decltype(definition);                                                                            \
    constexpr static std::string unit_string() { return desc; }                                                        \
  };                                                                                                                   \
  constexpr unit_name##_unit_type unit_name##_unit;

#define MAKE_UNIT_PREFXIES_WITH_DESC(unit_name, definition, desc, dimension)                                           \
  MAKE_UNIT_WITH_DESC(unit_name, definition, desc)                                                                     \
  MAKE_UNIT_PREFIXES(unit_name##_unit, dimension)

MAKE_UNIT_WITH_DESC(radian, scalar_unit, "rad");
MAKE_UNIT_WITH_DESC(steradian, scalar_unit, "sr");
MAKE_UNIT_WITH_DESC(degree, scalar_unit, "deg");

MAKE_UNIT_PREFXIES_WITH_DESC(hertz, scalar_unit / second_unit, "Hz", time);

template <> struct is_tag_convertible<degree_unit_type, radian_unit_type> : std::true_type {};

template <> struct is_tag_convertible<radian_unit_type, degree_unit_type> : std::true_type {};

template <> struct tag_conversion_factor<radian_unit_type, degree_unit_type> {
  static constexpr double factor = 180.0 / std::numbers::pi;
};

template <> struct tag_conversion_factor<degree_unit_type, radian_unit_type> {
  static constexpr double factor = std::numbers::pi / 180.0;
};

MAKE_UNIT_PREFXIES_WITH_DESC(newton, kilogram_unit* meter_unit / second_unit, "N", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(pascal, newton_unit / meter_unit / meter_unit, "Pa", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(joule, newton_unit* meter_unit, "J", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(watt, joule_unit / second_unit, "W", mass)
MAKE_UNIT_PREFXIES_WITH_DESC(coulomb, ampere_unit* second_unit, "C", current)
MAKE_UNIT_PREFXIES_WITH_DESC(volt, watt_unit / ampere_unit, "V", mass)

MAKE_UNIT_WITH_DESC(meter_per_second, meter_unit / second_unit, "m/s")
MAKE_UNIT_WITH_DESC(meter_per_second_per_second, meter_per_second_unit / second_unit, "m/s^2")
} // namespace maxwell

#endif