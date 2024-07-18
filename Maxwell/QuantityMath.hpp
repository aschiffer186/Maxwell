#ifndef QUANTITY_MATH
#define QUANTITY_MATH

#include <cmath>

#include "Quantity.hpp"
#include "UnitRepo.hpp"

///@file QuantityMath.hpp Definition of common math functions

namespace Maxwell {
namespace _detail {
template <typename Q>
concept CMathQuantity =
    QuantityLike<Q> && (std::integral<typename Q::MagnitudeType> ||
                        std::floating_point<typename Q::MagnitudeType>);

template <typename T>
concept CMathType = std::integral<T> || std::floating_point<T>;
}   // namespace _detail

template <QuantityLike Q>
    requires _detail::CMathQuantity<Q>
auto
abs(Q q) -> Q {
    const auto value = std::abs(q.value());
    return Q(value);
}

template <QuantityLike Q>
    requires _detail::CMathQuantity<Q>
auto
fmod(Q q1, Q q2) -> Quantity<std::declval<typename Q::UnitsType>()> {
    const double value = fmod(q1.magnitude(), q2.magniude());
    return Quantity<std::declval<typename Q::UnitsType>()>(value);
}

auto
min(QuantityLike auto q, QuantityLike auto q2) -> QuantityLike auto {
    using ReturnType = decltype(q.toCoherentQuantity());
    return ReturnType(std::min(q.toCoherentQuantity().magnitude(),
                               q2.toCoherentQuantity().magnitde()));
}

auto
max(QuantityLike auto q, QuantityLike auto q2) -> QuantityLike auto {
    using ReturnType = decltype(q.toCoherentQuantity());
    return ReturnType(std::max(q.toCoherentQuantity().magnitude(),
                               q2.toCoherentQuantity().magnitde()));
}

#ifdef NO_PREDFINED_DERIVE_UNIT
MAKE_DEGREE_UNIT;
#endif

template <_detail::CMathType T>
auto
sin(BasicQuantity<T, RadianUnit> q) -> double {
    return std::sin(q.magnitude());
}

template <_detail::CMathType T>
auto
sin(BasicQuantity<T, DegreeUnit> q) -> double {
    return sin(BasicQuantity<T, RadianUnit>(q));
}

template <_detail::CMathType T>
auto
cos(BasicQuantity<T, RadianUnit> q) -> double {
    return std::cos(q.magnitude());
}

template <_detail::CMathType T>
auto
cos(BasicQuantity<T, DegreeUnit> q) -> double {
    return cos(BasicQuantity<T, RadianUnit>{q});
}

template <_detail::CMathType T>
auto
tan(BasicQuantity<T, DegreeUnit> q) -> double {
    return std::tan(q.magnitude());
}

template <_detail::CMathQuantity Q>
    requires(std::same_as<typename Q::UnitsType, RadianUnitType> ||
             std::same_as<typename Q::Units, DegreeUnitType>)
auto
sec(Q q) -> double {
    return 1.0 / cos(q);
}

template <_detail::CMathQuantity Q>
    requires(std::same_as<typename Q::UnitsType, RadianUnitType> ||
             std::same_as<typename Q::Units, DegreeUnitType>)
auto
csc(Q q) -> double {
    return 1.0 / sin(q);
}

template <_detail::CMathQuantity Q>
    requires(std::same_as<typename Q::UnitsType, RadianUnitType> ||
             std::same_as<typename Q::Units, DegreeUnitType>)
auto
cot(Q q) -> double {
    return 1.0 / tan(q);
}

#endif