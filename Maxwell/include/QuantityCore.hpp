#pragma once 

#include <compare>
#include <concepts> 
#include <ostream>
#include <type_traits>

#include "UnitCore.hpp"

namespace Maxwell 
{
    template<typename Tp> 
    concept Arithmetic = (std::integral<Tp> || std::floating_point<Tp>) && 
                         !(std::same_as<Tp, bool>     || 
                           std::same_as<Tp, char>     ||
                           std::same_as<Tp, char8_t>  ||
                           std::same_as<Tp, char16_t> ||
                           std::same_as<Tp, char32_t> || 
                           std::same_as<Tp, wchar_t>);

    /**
     * @brief A construct with a value and a unit
     * 
     * A Quantity represents a physical quantity that has both 
     * a value and units. Quantities behave like the built in 
     * arithmetic types except that they are strongly typed based 
     * on their units. All operatins between Quantities are checked at 
     * compile-time to ensure that the units are coherent. If an invalid 
     * operation is performed because the units are incompatible, the 
     * program is ill-formed.
     *
     * Where appropriate, it is possible to convert a Quantity with units A 
     * to a quantity with units B. The conversion factor is calculated at 
     * compile time. If a conversin that is not possible is request, the 
     * program is ill-formed. 
     *
     * Thus, a program that works with Quantities is well-formed if the 
     * units are all correct.
     *
     * Class template Quantity satisfies the following: TriviallyCopyableType, StandardLayoutType,
     * ImplicitLifetimeType, LiteralType, Swappable, Regular, TotallyOrdered
     *
     * @tparam Tp_ the type representing the value of the Quantity.
     * @tparam Unit_ the type representing the units of the Quantity.
     */
    template<Arithmetic Tp_, UnitLike Unit_>
    struct Quantity
    {
        /// The type used to represent the value of the Quantity.
        using Rep = Tp_;
        /// The type used to represent the units of the Quantity.
        using Unit = Unit_;

        /**
         * @brief Default constructor
         * 
         * Value initalizes the underlying value of the 
         * Quantity.
         */
        constexpr Quantity() noexcept = default;

        /**
         * @brief Constructor
         * 
         * Constructs a Quantity whose value will be the same 
         * as the specified value.
         * 
         * @param val the value of the Quatity
         */
        constexpr explicit Quantity(Rep val) noexcept
        : val_{val}
        {

        }

        template<Arithmetic Up_>
        constexpr explicit Quantity(Up_ o) noexcept 
        : val_{static_cast<Rep>(o)}
        {

        }

        template<Arithmetic Up_, UnitLike OtherUnit>
            requires UnitAssignable<OtherUnit, Unit>
        constexpr Quantity(Quantity<Up_, OtherUnit> o) noexcept 
        : val_{static_cast<Rep>(o.value())}
        {

        }

        template<typename Up_, typename OtherUnit>
            requires UnitAssignable<OtherUnit, Unit>
        constexpr Quantity& operator=(Quantity<Up_, OtherUnit> o) noexcept 
        {
            val_ = static_cast<Rep>(o.val_);
            return *this;
        }

        /**
         * @brief Returns the value of the Quantity
         * 
         * @return the value of the Quantity
         */
        constexpr Rep value() const noexcept 
        {
            return val_;
        }

        /**
         * @brief Returns the units of the Quantity
         * 
         * @return the units of the Quantity
         */
        constexpr Unit units() const noexcept 
        {
            return Unit{};
        }

        constexpr Quantity<Rep, coherent_unit_t<Unit>> toCoherentUnits() const noexcept 
        {
            return Quantity<Rep, coherent_unit_t<Unit>>{val_};
        }

        static constexpr bool isInCoherentUnits() noexcept 
        {
            return is_unit_equal_v<Unit, coherent_unit_t<Unit>>;
        }

        constexpr void swap(Quantity& other) noexcept
        {
            using std::swap; 
            swap(val_, other.val_);
        }

        // Arithmetic operators
        constexpr Quantity& operator+=(Quantity q) noexcept 
        {
            val_ += q.val_;
            return *this;
        }

        constexpr Quantity& operator-=(Quantity q) noexcept 
        {
            val_ -= q.val_;
        }

        constexpr Quantity& operator*=(Quantity q) noexcept 
        {
            val_ *= q.val_;
        }

        constexpr Quantity& operator/=(Quantity q) noexcept 
        {
            val_ /= q.val_;
        }

        constexpr Quantity& operator%=(Quantity q) noexcept requires std::integral<Rep>
        {
            return val_ %= q.val_;
        }

        //Dimensioness value arithmetic operators
        constexpr Quantity& operator+=(Arithmetic auto val) noexcept 
            requires DimensionlessUnit<Unit>
        {
            val_ += val;
            return *this;
        }

        constexpr Quantity& operator-=(Arithmetic auto val) noexcept 
            requires DimensionlessUnit<Unit>
        {
            val_ -= val;
            return *this;
        }

        constexpr Quantity& operator*=(Arithmetic auto val) noexcept 
        {
            val_ *= val; 
            return *this;
        }

        constexpr Quantity& operator/=(Arithmetic auto val) noexcept 
        {
            val_ /= val; 
            return *this;
        }

        constexpr Quantity& operator%=(Arithmetic auto val) noexcept 
            requires std::integral<Rep> 
        {
            val_ %= val; 
            return *this;
        }

        //Converting arithmetic operators
        template<Arithmetic Up, UnitLike OtherUnit> 
            requires UnitAssignable<Unit, OtherUnit>
        constexpr Quantity& operator+=(Quantity<Up, OtherUnit> other) noexcept 
        {
            return *this;
        }

        template<Arithmetic Up, UnitLike OtherUnit>
            requires UnitAssignable<Unit, OtherUnit>
        constexpr Quantity& operator-=(Quantity<Up, OtherUnit> other) noexcept 
        {
            return *this;
        }

        friend constexpr std::compare_three_way_result_t<Rep> operator<=>(const Quantity& lhs, const Quantity& rhs) noexcept = default;
    private:
        Rep val_{};
    };

    template<Arithmetic Tp, UnitLike Unit> 
    Quantity(Quantity<Tp, Unit>) -> Quantity<Tp, Unit>;

    template<typename Tp_, typename Unit_> 
    std::ostream& operator<<(std::ostream& os, Quantity<Tp_, Unit_> q)
    {
        os << q.value();
        return os;
    }

    template<typename Tp_, typename Unit_> 
    constexpr void swap(Quantity<Tp_, Unit_>& q1, Quantity<Tp_, Unit_>& q2) noexcept 
    {
        q1.swap(q2);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
        requires UnitAddable<Unit1, Unit2>
    constexpr auto operator+(Quantity<Rep1, Unit1> lhs, Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using SumRep = decltype(lhs.value() + rhs.value());
        using LHSType = Quantity<Rep1, Unit1>;
        using RHSType = Quantity<Rep2, Unit2>;

        Rep1 valUsedLhs{};
        Rep2 valUsedRhs{};

        if constexpr (!LHSType::isInCoherentUnits())
            valUsedLhs = lhs.toCoherentUnits().value();
        else 
            valUsedRhs = lhs.value();

        if constexpr(!RHSType::isInCoherentUnits())
            valUsedRhs = rhs.toCoherentUnits().value();
        else  
            valUsedRhs = rhs.value();

        return Quantity<SumRep, coherent_unit_t<Unit1>>(valUsedLhs + valUsedRhs);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
        requires UnitAddable<Unit1, Unit2>
    constexpr auto operator-(Quantity<Rep1, Unit1> lhs, Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using MinusRep = decltype(lhs.value() - rhs.value());
        using LHSType = Quantity<Rep1, Unit1>;
        using RHSType = Quantity<Rep2, Unit2>;

        Rep1 valUsedLhs{};
        Rep2 valUsedRhs{};

        if constexpr (!LHSType::isInCoherentUnits())
            valUsedLhs = lhs.toCoherentUnits().value();
        else 
            valUsedRhs = lhs.value();

        if constexpr(!RHSType::isInCoherentUnits())
            valUsedRhs = rhs.toCoherentUnits().valu();
        else  
            valUsedRhs = rhs.value();

        return Quantity<MinusRep, coherent_unit_t<Unit1>>(valUsedLhs - valUsedRhs);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
    constexpr auto operator*(Quantity<Rep1, Unit1> lhs, Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using LHSType = Quantity<Rep1, Unit1>;
        using RHSType = Quantity<Rep2, Unit2>;
        using ProductUnits = decltype(lhs.toCoherentUnits().units() * rhs.toCoherentUnits().units());
        using ProductRep = decltype(lhs.value() * rhs.value());

        Rep1 valUsedLhs{};
        Rep2 valUsedRhs{};

        if constexpr (!LHSType::isInCoherentUnits())
            valUsedLhs = lhs.toCoherentUnits().value();
        else 
            valUsedRhs = lhs.value();

        if constexpr(!RHSType::isInCoherentUnits())
            valUsedRhs = rhs.toCoherentUnits().valu();
        else  
            valUsedRhs = rhs.value();

        return Quantity<ProductRep, ProductUnits>(valUsedLhs * valUsedRhs);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
    constexpr auto operator/(Quantity<Rep1, Unit1> lhs, Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using LHSType = Quantity<Rep1, Unit1>;
        using RHSType = Quantity<Rep2, Unit2>;
        using ProductUnits = decltype(lhs.toCoherentUnits().units() / rhs.toCoherentUnits().units());
        using ProductRep = decltype(lhs.value() / rhs.value());

        Rep1 valUsedLhs{};
        Rep2 valUsedRhs{};

        if constexpr (!LHSType::isInCoherentUnits())
            valUsedLhs = lhs.toCoherentUnits().value();
        else 
            valUsedRhs = lhs.value();

        if constexpr(!RHSType::isInCoherentUnits())
            valUsedRhs = rhs.toCoherentUnits().valu();
        else  
            valUsedRhs = rhs.value();

        return Quantity<ProductRep, ProductUnits>(valUsedLhs / valUsedRhs);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2>
        requires std::integral<Rep1> && std::integral<Rep2> 
    constexpr auto operator%(Quantity<Rep1, Unit1> lhs, Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using LHSType = Quantity<Rep1, Unit1>;
        using RHSType = Quantity<Rep2, Unit2>;
        using ProductUnits = decltype(lhs.toCoherentUnits().units() / rhs.toCoherentUnits().units());
        using ProductRep = decltype(lhs.value() / rhs.value());

        Rep1 valUsedLhs{};
        Rep2 valUsedRhs{};

        if constexpr (!LHSType::isInCoherentUnits())
            valUsedLhs = lhs.toCoherentUnits().value();
        else 
            valUsedRhs = lhs.value();

        if constexpr(!RHSType::isInCoherentUnits())
            valUsedRhs = rhs.toCoherentUnits().value();
        else  
            valUsedRhs = rhs.value();
        
        return Quantity<ProductRep, ProductUnits>(valUsedLhs % valUsedRhs);
    }
}