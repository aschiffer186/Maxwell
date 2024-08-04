#include "QuantityInstantiations.hpp"

namespace Maxwell {
#define TEMPLATE(U) template class BasicQuantity<double, U##Unit>;

TEMPLATE(Mole)
TEMPLATE(Ampere)
TEMPLATE(Meter)
TEMPLATE(Candela)
TEMPLATE(Gram)
TEMPLATE(Kelvin)
TEMPLATE(Second)
TEMPLATE(Radian)
#ifndef NO_PREDIFNED_DERIVED_UNITS
TEMPLATE(Degree)
TEMPLATE(MeterPerSecond)
TEMPLATE(Joule)
TEMPLATE(Hertz)
TEMPLATE(Newton)
TEMPLATE(Pascal)
TEMPLATE(Watt)
TEMPLATE(Coulomb)
TEMPLATE(Volt)
TEMPLATE(Farad)
TEMPLATE(Ohm)
TEMPLATE(Becquerel)

TEMPLATE(Minute)
TEMPLATE(Hour)
TEMPLATE(Day)
TEMPLATE(Year)
TEMPLATE(Decade)
TEMPLATE(Century)
TEMPLATE(Millenium)
TEMPLATE(Inch)
TEMPLATE(Foot)
TEMPLATE(Yard)
TEMPLATE(Mile)

TEMPLATE(PoundMass)
#endif
}   // namespace Maxwell