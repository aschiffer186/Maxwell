/**
 * @file QuantityBase.hh
 * @author Alex Schiffer (aschiffer186)
 * @brief
 * @version 0.1
 * @date 2024-02-29
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef QUANTITY_BASE_HH
#define QUANTITY_BASE_HH

#include <compare>
#include <type_traits>
#include <utility>

#include "QuantityConcepts.hh"
#include "UnitBase.hh"

namespace Maxwell
{
    template <Arithmetic Scalar, UnitLike auto Units>
    class BasicQuantity
    {
      public:
        using ScalarType = Scalar;
        using UnitsType  = decltype(Units);

        /**
         * @brief Default constructor
         *
         * @post this->value() == ScalarType()
         *
         */
        constexpr BasicQuantity()
            noexcept(std::is_nothrow_default_constructible_v<ScalarType>)
            requires std::constructible_from<Scalar>
        = default;

        /**
         * @brief Constructor
         *
         * Constructs a BasicQuantity object from the specified value.
         * The value will be initialized by std::forward<U>(u). This
         * constructor can only be called if std::constructible_from<ScalarType,
         * U> is true.
         *
         * @post this->value() == u
         *
         * @tparam U the type of the value used to construct
         * @param u the value used to construct the basic quantity
         */
        template <typename U>
            requires std::constructible_from<ScalarType, U>
        constexpr explicit(Unitless<UnitsType>) BasicQuantity(U&& u)
            noexcept(std::is_nothrow_constructible_v<ScalarType, U>)
            : scalar_(std::forward<U>(u))
        {
        }

        /**
         * @brief Converting constructor
         *
         * Constructs a BasicQuantity from the specified quantity,
         * converting the value expressed in Unit2 its equivalent
         * expresssed in Units. This constructor can only be
         * called if Unit2 is convertible to Units and Scalar2
         * can be constructed from ScalarType. The value is copy
         * constructed other.scalar_ prior to the conversion.
         *
         * @tparam Scalar2
         * @tparam Unit2
         */
        template <Arithmetic Scalar2, auto Unit2>
            requires std::constructible_from<ScalarType, const Scalar2&> &&
                     UnitConvertibleTo<decltype(Unit2), UnitsType>
        constexpr BasicQuantity(const BasicQuantity<Scalar2, Unit2>& other)
            noexcept(std::is_nothrow_constructible_v<ScalarType, Scalar2>)
            : scalar_(other.value())
        {
            constexpr double prefixConversion =
                unitPrefixConversion(Unit2, Units);
            scalar_ = prefixConversion * scalar_;
        }

        /**
         * @brief Converting constructor
         *
         * Constructs a BasicQuantity from the specified quantity,
         * converting the value expressed in Unit2 its equivalent
         * expresssed in Units. This constructor can only be
         * called if Unit2 is convertible to Units and Scalar2
         * can be constructed from ScalarType. The value is move
         * constructed from other.scalar_ prior to conversion.
         *
         * @tparam Scalar2
         * @tparam Unit2
         */
        template <Arithmetic Scalar2, auto Unit2>
            requires std::constructible_from<ScalarType, Scalar2&&> &&
                     UnitConvertibleTo<decltype(Unit2), UnitsType>
        constexpr BasicQuantity(BasicQuantity<Scalar2, Unit2>&& other)
            noexcept(std::is_nothrow_constructible_v<ScalarType, Scalar2&&>)
            : scalar_(std::move(other.value()))
        {
            constexpr double prefixConversion =
                unitPrefixConversion(Unit2, Units);
            scalar_ = prefixConversion * scalar_;
        }

        constexpr BasicQuantity(const BasicQuantity&)
            requires(!std::copy_constructible<ScalarType>)
        = delete;

        constexpr BasicQuantity(const BasicQuantity&)
            requires std::copy_constructible<ScalarType>
        = default;

        constexpr BasicQuantity(BasicQuantity&&) noexcept(
            std::is_nothrow_move_constructible_v<ScalarType>) = default;

        /**
         * @brief Assignment operator
         *
         * Assigns the specified scalar to the quantity. This
         * function can only be called if the BasicQuantity is unitless
         * and std::constructible_from<ScalarType, S> is true.
         *
         * @tparam S the type of the scalar
         * @param s the new scalar value of the quantity
         * @return a reference to *this
         */
        template <Arithmetic S>
            requires std::constructible_from<ScalarType, S> &&
                         Unitless<UnitsType>
        constexpr auto operator=(S&& s)
            noexcept(std::is_nothrow_constructible_v<ScalarType, S>)
                -> BasicQuantity&
        {
            scalar_ = std::forward<S>(s);
        }

        template <Arithmetic Scalar2, UnitLike auto Unit2>
            requires std::constructible_from<ScalarType, Scalar2&&> &&
                         UnitConvertibleTo<decltype(Unit2), UnitsType>
        constexpr auto
        operator=(const BasicQuantity<Scalar2, Unit2>& other) -> BasicQuantity&
        {
            BasicQuantity{other}.swap(*this);
        }

        template <Arithmetic Scalar2, UnitLike auto Unit2>
            requires std::constructible_from<ScalarType, Scalar2&&> &&
                         UnitConvertibleTo<decltype(Unit2), UnitsType>
        constexpr auto
        operator=(BasicQuantity<Scalar2, Unit2>&& other) -> BasicQuantity&
        {
            BasicQuantity{std::move(other)}.swap(*this);
        }

        constexpr auto operator=(const BasicQuantity&)
            -> BasicQuantity& requires(!std::is_copy_assignable_v<ScalarType>) =
                   delete;

        constexpr auto operator=(const BasicQuantity&) -> BasicQuantity&
            requires std::is_copy_assignable_v<ScalarType>
        = default;

        constexpr auto operator=(BasicQuantity&&)
            noexcept(std::is_nothrow_move_assignable_v<ScalarType>)
                -> BasicQuantity& = default;

        /**
         * @brief Return the value of the BasicQuanity
         *
         * Returns the value of the BasicQuantity.
         *
         * @return the value of the basic quantity
         */
        constexpr auto value() const& noexcept -> const ScalarType&
        {
            return scalar_;
        }

        constexpr auto value() && noexcept -> ScalarType&&
        {
            return std::move(scalar_);
        }

        /**
         * @brief Return the units of the BasicQuantity
         *
         * Returns the units of the BasicQuantity. For two BasicQuantities
         * q1 and q2, q1.units() == q2.units().
         *
         * @return the value of the basic quantity
         */
        constexpr auto units() const noexcept -> UnitsType { return Units; }

        /**
         * @brief Conversion operator to Scalar
         *
         * Conversion operator to convert the value of *this
         * to the value of the underyling scalar type, ScalarType.
         * This conversion operator can only be called if the
         * the BasicQuantity is unitless.
         *
         * @return the underlying value
         */
        constexpr operator Scalar() const
            requires Unitless<UnitsType>
        {
            return scalar_;
        }

        constexpr auto inCoherentUnits() const noexcept -> auto
        {
            return BasicQuantity<ScalarType, toCoherentUnit(Units)>{scalar_};
        }

        // Arithmetic operators
        // Start with the easy cases: no conversion necessary

        constexpr auto operator+=(const BasicQuantity& other)
            noexcept(NothrowArithmetic<ScalarType>) -> BasicQuantity&
        {
            scalar_ = scalar_ + other.scalar_;
            return *this;
        }

        constexpr auto operator-=(const BasicQuantity& other)
            noexcept(NothrowArithmetic<ScalarType>) -> BasicQuantity&
        {
            scalar_ = scalar_ - other.scalar_;
            return *this;
        }

        constexpr auto operator*=(const ScalarType& scalar)
            noexcept(NothrowArithmetic<ScalarType>) -> BasicQuantity&
        {
            scalar_ = scalar_ * scalar;
            return *this;
        }

        constexpr auto operator/=(const ScalarType& scalar)
            noexcept(NothrowArithmetic<ScalarType>) -> BasicQuantity&
        {
            scalar_ = scalar_ / scalar;
            return *this;
        }

        constexpr auto operator%=(const ScalarType& scalar)
            noexcept(NothrowArithmetic<ScalarType>) -> BasicQuantity&
            requires std::integral<ScalarType>
        {
            scalar_ %= scalar;
            return *this;
        }

        /**
         * @brief Swaps the values of two quantities
         *
         * Swaps the value of *this with the value of other.
         *
         * @param other the quantity to swap with *this
         */
        constexpr auto swap(BasicQuantity& other)
            noexcept(std::is_nothrow_swappable_v<ScalarType>) -> void
            requires std::swappable<ScalarType>
        {
            using std::swap;
            swap(other.scalar_, scalar_);
        }

        friend auto operator<=>(const BasicQuantity& lhs,
                                const BasicQuantity& rhs)
            requires std::three_way_comparable<ScalarType>
        = default;

      private:
        ScalarType scalar_{};
    };

    template <Arithmetic S, UnitLike auto U>
    constexpr auto swap(BasicQuantity<S, U>& lhs, BasicQuantity<S, U>& rhs)
        noexcept(noexcept(lhs.swap(rhs))) -> void
        requires std::swappable<S>
    {
        lhs.swap(rhs);
    }

    // Arithmetic operators
    // Start with the easy overloads: no conversion necessary
    template <Arithmetic S, UnitLike auto U>
    constexpr auto operator+(BasicQuantity<S, U>        lhs,
                             const BasicQuantity<S, U>& rhs)
        noexcept(NothrowArithmetic<S>) -> BasicQuantity<S, U>
    {
        lhs += rhs;
        return lhs;
    }

    template <Arithmetic S, UnitLike auto U>
    constexpr auto operator-(BasicQuantity<S, U>        lhs,
                             const BasicQuantity<S, U>& rhs)
        noexcept(NothrowArithmetic<S>) -> BasicQuantity<S, U>
    {
        lhs -= rhs;
        return lhs;
    }

    template <Arithmetic S, UnitLike auto U1, UnitLike auto U2>
    constexpr auto operator*(const BasicQuantity<S, U1>& lhs,
                             const BasicQuantity<S, U2>& rhs)
        noexcept(NothrowArithmetic<S>) -> auto
    {
        constexpr auto outputUnit = toCoherentUnit(U1) * toCoherentUnit(U2);
        return BasicQuantity<S, outputUnit>(lhs.inCoherentUnits().value() *
                                            rhs.inCoherentUnits().value());
    }

    template <Arithmetic S, UnitLike auto U1, UnitLike auto U2>
    constexpr auto operator/(const BasicQuantity<S, U1>& lhs,
                             const BasicQuantity<S, U2>& rhs)
        noexcept(NothrowArithmetic<S>) -> auto
    {
        constexpr auto outputUnit = toCoherentUnit(U1) / toCoherentUnit(U2);
        return BasicQuantity<S, outputUnit>(lhs.inCoherentUnits().value() /
                                            rhs.inCoherentUnits().value());
    }

    template <Arithmetic S, UnitLike auto U>
    constexpr auto operator*(BasicQuantity<S, U> lhs, const S& rhs)
        noexcept(NothrowArithmetic<S>) -> BasicQuantity<S, U>
    {
        lhs *= rhs;
        return lhs;
    }

    template <Arithmetic S, UnitLike auto U>
    constexpr auto operator*(const S& lhs, BasicQuantity<S, U> rhs)
        noexcept(NothrowArithmetic<S>) -> BasicQuantity<S, U>
    {
        rhs *= lhs;
        return lhs;
    }

    template <Arithmetic S, UnitLike auto U>
    constexpr auto operator/(BasicQuantity<S, U> lhs, const S& rhs)
        noexcept(NothrowArithmetic<S>) -> BasicQuantity<S, U>
    {
        lhs /= rhs;
        return lhs;
    }

    template <Arithmetic S, UnitLike auto U>
    constexpr auto operator/(const S& lhs, BasicQuantity<S, U> rhs)
        noexcept(NothrowArithmetic<S>) -> BasicQuantity<S, U>
    {
        rhs /= lhs;
        return lhs;
    }
} // namespace Maxwell

#endif
