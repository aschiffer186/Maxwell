#ifndef QUANTITY_INSTANTIATIONS_HPP
#define QUANTITY_INSTANTIATIONS_HPP

#include "QuantityRepo.hpp"

namespace Maxwell {
#define EXTERN_TEMPLATE(U) extern template class BasicQuantity<double, U##Unit>;

EXTERN_TEMPLATE(Mole)
EXTERN_TEMPLATE(Ampere)
EXTERN_TEMPLATE(Meter)
EXTERN_TEMPLATE(Candela)
EXTERN_TEMPLATE(Gram)
EXTERN_TEMPLATE(Second)
EXTERN_TEMPLATE(Radian)

#ifndef NO_PREDFINED_DERIVED_UNITS
EXTERN_TEMPLATE(Degree)
EXTERN_TEMPLATE(MeterPerSecond)
#endif

#undef EXTERN_TEMPLATE
}   // namespace Maxwell

#endif