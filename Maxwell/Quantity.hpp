#ifndef QUANTITY_HPP
#define QUANTITY_HPP

#include "Unit.hpp"
#include <compare>
#include <concepts>
#include <type_traits>
#include <utility>

namespace Maxwell {
namespace _detail {
template <typename T>
concept Arithmetic = requires(T t, double d) {
    { t + t } -> std::same_as<T>;
    { t - t } -> std::same_as<T>;
    { t *t } -> std::same_as<T>;
    { t / t } -> std::same_as<T>;
    { d *t } -> std::same_as<T>;
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

template <typename T>
concept Quantity = _detail::is_quantity<T>::value;

template <_detail::Arithmetic Tp, Unit auto U> class BasicQuantity {
  public:
    using MagnitudeType = Tp;
    using UnitsType = decltype(U);
    static constexpr UnitsType Units = U;

    constexpr BasicQuantity() noexcept(
        std::is_nothrow_default_constructible_v<MagnitudeType>)
        requires std::constructible_from<MagnitudeType>
    = default;

    template <typename Up = MagnitudeType>
    constexpr explicit BasicQuantity(Up&& mag) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, Up&&>)
        requires std::constructible_from<MagnitudeType, Up&&>
        : mag_(std::forward<Up>(mag)) {}

    template <typename Up = MagnitudeType>
    constexpr explicit BasicQuantity(Up&& mag, UnitsType) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, Up&&>)
        requires std::constructible_from<MagnitudeType, Up&&>
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
        : mag_(other.magnitude()) {}

    template <_detail::Arithmetic Up, Unit auto From>
        requires UnitConvertibleTo<From, Units> &&
                 std::constructible_from<MagnitudeType,
                                         std::add_rvalue_reference_t<Up>>
    constexpr BasicQuantity(BasicQuantity<Up, From>&& other) noexcept(
        std::is_nothrow_constructible_v<MagnitudeType, Up>)
        : mag_(std::move(other.magnitude())) {}

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

    constexpr explicit(Unitless<Units>) operator MagnitudeType() const {
        return mag_;
    }

    auto constexpr units() const noexcept -> UnitsType { return Units; }

    auto constexpr toCoherentQuantity() const noexcept -> Quantity auto {
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

  private:
    MagnitudeType mag_{};
};

template <_detail::Arithmetic Tp, Unit U>
BasicQuantity(Tp t, U u) -> BasicQuantity<Tp, std::declval<U>()>;
}   // namespace Maxwell

#endif