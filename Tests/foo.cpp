#include "Dimension.hpp"
#include "Quantity.hpp"
#include "Unit.hpp"

constexpr Maxwell::UnitType<Maxwell::NullDimension, Maxwell::NullDimension,
                            Maxwell::NullDimension, Maxwell::NullDimension,
                            Maxwell::NullDimension, Maxwell::NullDimension,
                            Maxwell::NullDimension, Maxwell::NullDimension>
    u{};
constexpr Maxwell::UnitType<Maxwell::NullDimension, Maxwell::CoherentDimension,
                            Maxwell::CoherentDimension, Maxwell::NullDimension,
                            Maxwell::NullDimension, Maxwell::NullDimension,
                            Maxwell::NullDimension, Maxwell::NullDimension>
    u2{};

int
main() {}
