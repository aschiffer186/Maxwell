#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include <compare>
#include <concepts>
#include <type_traits>
#include <utility>

#include "Unit.hpp"

/// @file Quantity.hpp Contains the definition of BasicQuantity

namespace Maxwell {
namespace _detail {
template <typename T>
concept Arithmetic = requires(T t, double d) {
    { t + t } -> std::same_as<T>;
    { t - t } -> std::same_as<T>;
    { t *t } -> std::same_as<T>;
    { t / t } -> std::same_as<T>;
    d *t;
};
}   // namespace _detail

template <_detail::Arithmetic Tp, Unit auto U> class BasicQuantity;

namespace _detail {
template <typename> struct is_quantity : std::false_type {};

template <typename T> struct is_quantity<const T> : is_quantity<T> {};

template <typename T> struct is_quantity<T&> : is_quantity<T> {};

template <typename T> struct is_quantity<T&&> : is_quantity<T> {};

template <Arithmetic Tp, Unit auto U>
struct is_quantity<BasicQuantity<Tp, U>> : std::true_type {};
}   // namespace _detail

/**
 * @brief Type respenting a quantity with a magnitude and a unit.
 *
 * BasicQuantity is the central type in Maxwell. It is a type representing
 * a quantity with a magnitude and unit and whose units are strongly typed.
 * This means it is only possible to construct BasicQuantity's whose units
 * are convertible to the specified units of the BasicQuantity. The conversion
 * is performed automatically during the construction process and the conversion
 * factor is calculated at compile-time rather than runtime for maximum
 * efficiency.
 *
 * The magnitude of the BasicQuantity may be any type that implements
 * the 4 basic arithmetic operators (+, -, *, and /) and that can be
 * multipled by a double.
 *
 * @tparam Tp the type of the BasicQuantity's magnitude
 * @tparam U the BasicQuantity's unit
 */
template <_detail::Arithmetic Tp, Unit auto U> class BasicQuantity {
  public:
    /// Type alias for the type of the BasicQuantity's magnitude
    using MagnitudeType = Tp;
    /// Type alias for the type of the BasicQuantity's units
    using UnitsType = decltype(U);
    /// The units of the BasicQuantity
    static constexpr UnitsType Units = U;

    /**
     * @brief Default constructor
     *
     * Constructs a BasicQuantity whose magnitude is default constructed.
     *
     * @pre MagnitudeType is default construcible
     * @post magnitude() is equal to MagnitudeType()
     *
     * @throws any exceptions thrown by the default consturctor of MagnitudeType
     */
    constexpr BasicQuantity() noexcept(
        std::is_nothrow_default_constructible_v<MagnitudeType>)
        requires std::constructible_from<MagnitudeType>
    = default;

    /**
     * @brief Constructor
     *
     * Constructs a BasicQuantiy whose magnitude is initialized from the
     * specified argument as if by MagnitudeType(std::forward<Up>(mag)).
     *
     * @pre MagnitudeType is constructible from Up && Up is not std::in_place_t
     * @post magnitude() is equal to MagnitudeType(std::forward<Up>(mag))
     *
     * @param mag the value used to construct the BasicQuantity's magnitude
     * @throws any exceptions thrown by the constructor of MagnitudeType
     */
    template <typename Up = MagnitudeType>
        requires std::constructible_from<MagnitudeType, Up&&> &&
                 (!std::same_as<Up, std::in_place_t>)
    constexpr explicit BasicQuantity(Up&& mag) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, Up&&>)
        : mag_(std::forward<Up>(mag)) {}

    template <typename... Args>
    constexpr BasicQuantity(std::in_place_t, Args&&...args) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, Args&&...>)
        requires std::constructible_from<MagnitudeType, Args&&...>
        : mag_(std::forward<Args>(args)...) {}

    template <typename Up, typename... Args>
    constexpr BasicQuantity(
        std::in_place_t, std::initializer_list<Up> il,
        Args&&...args) noexcept(std::
                                    is_nothrow_constructible_v<
                                        MagnitudeType,
                                        std::initializer_list<Up>, Args&&...>)
        requires std::constructible_from<MagnitudeType,
                                         std::initializer_list<Up>, Args&&...>
        : mag_(il, std::forward<Args>(args)...) {}

    template <_detail::Arithmetic Up, Unit auto From>
        requires UnitConvertibleTo<From, Units> &&
                 std::constructible_from<MagnitudeType, const Up&>
    constexpr BasicQuantity(const BasicQuantity<Up, From>& other) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, Up>)
        : mag_(other.magnitude()) {
        constexpr double factor = conversionFactor(From, Units);
        mag_ *= factor;
    }

    template <_detail::Arithmetic Up, Unit auto From>
        requires UnitConvertibleTo<From, Units> &&
                 std::constructible_from<MagnitudeType,
                                         std::add_rvalue_reference_t<Up>>
    constexpr BasicQuantity(BasicQuantity<Up, From>&& other) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, Up>)
        : mag_(std::move(other.magnitude())) {
        constexpr double factor = conversionFactor(From, Units);
        mag_ *= factor;
    }

    template <_detail::Arithmetic Up, Unit auto From>
        requires UnitConvertibleTo<From, Units> &&
                     std::constructible_from<MagnitudeType,
                                             std::add_rvalue_reference_t<Up>>
    auto constexpr operator=(BasicQuantity<Up, From> other) noexcept(
        std::is_nothrow_constructible_v<
            MagnitudeType, std::add_rvalue_reference_t<Up>>) -> BasicQuantity& {
        BasicQuantity(other).swap(*this);
        return *this;
    }

    auto constexpr magnitude() const noexcept -> const MagnitudeType& {
        return mag_;
    }

    // constexpr explicit(Unitless<Units>) operator MagnitudeType() const {
    //     return mag_;
    // }

    auto constexpr units() const noexcept -> UnitsType { return Units; }

    auto constexpr toCoherentQuantity() const noexcept -> auto {
        constexpr Unit auto toUnit = Units.toCoherentUnit();
        return BasicQuantity<Tp, toUnit>(mag_);
    }

    auto constexpr swap(BasicQuantity& other) noexcept(
        std::is_nothrow_swappable_v<MagnitudeType>) -> void
        requires std::swappable<MagnitudeType>
    {
        using std::swap;
        swap(mag_, other.mag_);
    }

    // --- Comparison Operators ---
    friend auto constexpr
    operator<=>(const BasicQuantity& lhs,
                const BasicQuantity& rhs) noexcept(noexcept(lhs <= rhs)) -> auto
        requires std::three_way_comparable<MagnitudeType>
    = default;

    friend auto constexpr
    operator==(const BasicQuantity& lhs,
               const BasicQuantity& rhs) noexcept(noexcept(lhs == rhs)) -> bool
        requires std::equality_comparable<MagnitudeType>
    = default;

    /// --- Arithmetic Operators ---
    auto constexpr operator+=(const BasicQuantity& other) noexcept(
        noexcept(magnitude() + magnitude())) -> BasicQuantity& {
        mag_ += other.mag_;
        return *this;
    }

    auto constexpr operator-=(const BasicQuantity& other) noexcept(
        noexcept(magnitude() - magnitude())) -> BasicQuantity& {
        mag_ -= other.mag_;
        return *this;
    }

    template <_detail::Arithmetic S, Unit auto O>
        requires UnitConvertibleTo<O, Units>
    auto constexpr operator+=(const BasicQuantity<S, O>& other) noexcept(
        noexcept(magnitude() + magnitude())) {
        mag_ += conversionFactor(O, Units) * other.magnitude();
    }

    template <_detail::Arithmetic S, Unit auto O>
        requires UnitConvertibleTo<O, Units>
    auto constexpr operator-=(const BasicQuantity<S, O>& other) noexcept(
        noexcept(magnitude() - magnitude())) {
        mag_ -= conversionFactor(O, Units) * other.magnitude();
    }

  private:
    MagnitudeType mag_{};
};

template <Unit auto U> using Quantity = BasicQuantity<double, U>;

// --- Arithmetic Operators ---
template <_detail::Arithmetic S1, Unit auto U1, _detail::Arithmetic S2,
          Unit auto U2>
    requires UnitConvertibleTo<U2, U1>
auto constexpr
operator+(BasicQuantity<S1, U1> lhs,
          const BasicQuantity<S2, U2>& rhs) noexcept(noexcept(lhs +=
                                                              rhs)) -> auto {
    return lhs += rhs;
}

template <_detail::Arithmetic S1, Unit auto U1, _detail::Arithmetic S2,
          Unit auto U2>
    requires UnitConvertibleTo<U2, U1>
auto constexpr
operator-(BasicQuantity<S1, U1> lhs,
          const BasicQuantity<S2, U2>& rhs) noexcept(noexcept(lhs -=
                                                              rhs)) -> auto {
    return lhs -= rhs;
}

template <_detail::Arithmetic S1, Unit auto U1, _detail::Arithmetic S2,
          Unit auto U2>
    requires UnitConvertibleTo<U2, U1>
auto constexpr
operator*(BasicQuantity<S1, U1> lhs, const BasicQuantity<S2, U2>& rhs) noexcept(
    noexcept(lhs.magnitude() * rhs.magnitude())) -> auto {
    constexpr Unit auto resUnit = lhs.units() * rhs.units();
    return BasicQuantity(lhs.toCoherentQuantity() * rhs.toCoherentQuantity(),
                         resUnit);
}

template <_detail::Arithmetic S1, Unit auto U1, _detail::Arithmetic S2,
          Unit auto U2>
    requires UnitConvertibleTo<U2, U1>
auto constexpr
operator/(BasicQuantity<S1, U1> lhs, const BasicQuantity<S2, U2>& rhs) noexcept(
    noexcept(lhs.magnitude() / rhs.magnitude())) -> auto {
    constexpr Unit auto resUnit = lhs.units() / rhs.units();
    return BasicQuantity(lhs.toCoherentQuantity() * rhs.toCoherentQuantity(),
                         resUnit);
}

// --- Comparison Operators ---
template <_detail::Arithmetic S1, Unit auto U1, _detail::Arithmetic S2,
          Unit auto U2>
    requires UnitConvertibleTo<U2, U1> && std::three_way_comparable_with<S1, S2>
auto constexpr
operator<=>(const BasicQuantity<S1, U1>& lhs,
            const BasicQuantity<S2, U2>&
                rhs) noexcept(noexcept(lhs.magnitude() <=>
                                       rhs.magnitude())) -> auto {
    return lhs.toCoherentQuantity() <=> rhs.toCoherentQuantity();
}

template <_detail::Arithmetic S1, Unit auto U1, _detail::Arithmetic S2,
          Unit auto U2>
    requires UnitConvertibleTo<U2, U1> && std::equality_comparable_with<S1, S2>
auto constexpr
operator==(const BasicQuantity<S1, U1>& lhs, const BasicQuantity<S2, U2>& rhs) {
    return lhs.toCoherentQuantity() == rhs.toCoherentQuantity();
}

template <_detail::Arithmetic S, Unit auto U>
auto constexpr swap(BasicQuantity<S, U>& lhs,
                    BasicQuantity<S, U>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    -> void {
    lhs.swap(rhs);
}
}   // namespace Maxwell

#endif