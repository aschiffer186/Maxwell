/// \file currency.cpp
/// \brief Brief example showing how to set up a custom quantity system to
/// represent the US currency system.

#include "Maxwell.hpp"

using namespace maxwell;

using currency_system = quantity_system<"currency">;

constexpr struct dollar_unit_type
    : base_unit<currency_system::base_quantity<"currency">{}, "dollar"> {
} dollar_unit;

constexpr struct cent_unit_type
    : derived_unit<value<100.0> * dollar_unit, "cent"> {
} cent_unit;

constexpr struct nickel_unit_type
    : derived_unit<value<0.2> * cent_unit, "nickel"> {
} nickel_unit;

constexpr struct dime_unit_type
    : derived_unit<value<0.1> * cent_unit, "dime"> {
} dime_unit;

constexpr struct quarter_unit_type
    : derived_unit<value<1.0 / 25.0> * cent_unit, "quarter"> {
} quarter_unit;

using dollar = quantity_value<dollar_unit>;
using cent = quantity_value<cent_unit>;
using nickel = quantity_value<nickel_unit>;
using dime = quantity_value<dime_unit>;
using quarter = quantity_value<quarter_unit>;

int main() { return 0; }