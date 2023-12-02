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
     * A Basic_Quantity represents a physical quantity that has both 
     * a value and units. Quantities behave like the built in 
     * arithmetic types except that they are strongly typed based 
     * on their units. All operatins between Quantities are checked at 
     * compile-time to ensure that the units are coherent. If an invalid 
     * operation is performed because the units are incompatible, the 
     * program is ill-formed.
     *
     * Where appropriate, it is possible to convert a Basic_Quantity with units A 
     * to a quantity with units B. The conversion factor is calculated at 
     * compile time. If a conversin that is not possible is request, the 
     * program is ill-formed. 
     *
     * Thus, a program that works with Basic_Quantities is well-formed if the 
     * units are all correct.
     *
     * Class template Basic_Quantity satisfies the following: TriviallyCopyableType, StandardLayoutType,
     * ImplicitLifetimeType, LiteralType, Swappable, Regular, TotallyOrdered
     *
     * @tparam Tp_ the type representing the value of the Quantity.
     * @tparam Unit_ the type representing the units of the Quantity.
     */
    template<Arithmetic Tp_, UnitLike Unit_>
    struct Basic_Quantity
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
        constexpr Basic_Quantity() noexcept = default;

        /**
         * @brief Constructor
         * 
         * Constructs a Quantity whose value will be the same 
         * as the specified value.
         * 
         * @param val the value of the Quatity
         */
        constexpr explicit Basic_Quantity(Rep val) noexcept
        : val_{val}
        {

        }

        /**
         * @brief Converting Constructor
         * 
         * Constructs a Quantity whose value will be the same as the specified value 
         * after converting to Rep. The value of the Quantity is intialized as if by 
         * Rep{static_cast<Rep>(o)}
         *
         * @tparam Up_ the type of the value 
         * @param o the value of the quantity
         */
        template<Arithmetic Up_>
        constexpr explicit Basic_Quantity(Up_ o) noexcept 
        : val_{static_cast<Rep>(o)}
        {

        }

        /**
         * @brief Converting constructor
         * 
         * Constructs a Quantity from the specified Quantity with a different value representation 
         * or different units. The value of the quantity is automatically adjusted so that the value 
         * of *this is the same if it were expressed in the units of the specified quantity. The conversion 
         * factor is calculated at compile-time.
         *
         * This constructor can only be called if the units of specified quantity are convertible to 
         * Unit. This requires that the powers of the base-units of Unit and OtherUnit be the same. 
         * This function does not participate in overload resolution if this is not the case.
         * 
         * @tparam Up_ the value representation of the specified quantity
         * @tparam OtherUnit the units of the specified quantity
         * @param o the specified Quantity
         */
        template<Arithmetic Up_, UnitLike OtherUnit>
            requires UnitAssignable<OtherUnit, Unit>
        constexpr Basic_Quantity(Basic_Quantity<Up_, OtherUnit> o) noexcept 
        : val_{static_cast<Rep>(o.value())}
        {
            val_ *= static_cast<Rep>(conversionPrefix(OtherUnit{}, Unit{}));
            val_ *= static_cast<Rep>(conversionScale(OtherUnit{}, Unit{}));
        }

        template<typename Up_, typename OtherUnit>
            requires UnitAssignable<OtherUnit, Unit>
        constexpr Basic_Quantity& operator=(Basic_Quantity<Up_, OtherUnit> o) noexcept 
        {
            val_ = static_cast<Rep>(o.value());
            val_ *= static_cast<Rep>(conversionPrefix(OtherUnit{}, Unit{}));
            val_ *= static_cast<Rep>(conversionScale(OtherUnit{}, Unit{}));

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

        /**
         * @brief Returns a Basic_Quantity whose value is the same as *this expressed in coherent units.
         * 
         * Returns a Basic_Quantity whose value is the same as *this expressed in coherent units. Coherent units 
         * are units are SI units with no prefixes. 
         *
         * @return constexpr Basic_Quantity<Rep, coherent_unit_t<Unit>> 
         */
        constexpr Basic_Quantity<Rep, coherent_unit_t<Unit>> toCoherentUnits() const noexcept 
        {
            double scale = conversionPrefix(Unit{}, coherent_unit_t<Unit>{});
            return Basic_Quantity<Rep, coherent_unit_t<Unit>>{val_*scale};
        }

        /**
         * @brief Returns true if the units of the Basic_Quantity are coherent units.
         * 
         * @return trrue if the units of the Basic_Quantity are coherent units.
         */
        static constexpr bool isInCoherentUnits() noexcept 
        {
            return is_unit_equal_v<Unit, coherent_unit_t<Unit>>;
        }

        /**
         * @brief Swaps the values of two Basic_Quantities.
         * 
         * @param other the Basici_Quantity to swap with
         */
        constexpr void swap(Basic_Quantity& other) noexcept
        {
            using std::swap; 
            swap(val_, other.val_);
        }

        /**
         * @brief Conversion function
         * 
         * Converts the value of *this to Rep{this->value()}. This function  
         * only participle in overload resolution if the Basic_Quantity is 
         * dimensionless. A common use of this conversion is invoking standard 
         * math library functions on ratios of units e.g. std::pow(1_m/1_m, 2);
         *
         * @return Rep{this->value()}
         */
        constexpr operator Rep() const noexcept
            requires DimensionlessUnit<Unit>
        {
            return val_;
        }

        // Arithmetic operators
        constexpr Basic_Quantity& operator+=(Basic_Quantity q) noexcept 
        {
            val_ += q.val_;
            return *this;
        }

        constexpr Basic_Quantity& operator-=(Basic_Quantity q) noexcept 
        {
            val_ -= q.val_;
        }

        constexpr Basic_Quantity& operator*=(Basic_Quantity q) noexcept 
        {
            val_ *= q.val_;
        }

        constexpr Basic_Quantity& operator/=(Basic_Quantity q) noexcept 
        {
            val_ /= q.val_;
        }

        constexpr Basic_Quantity& operator%=(Basic_Quantity q) noexcept requires std::integral<Rep>
        {
            return val_ %= q.val_;
        }

        //Dimensioness value arithmetic operators
        constexpr Basic_Quantity& operator+=(Arithmetic auto val) noexcept 
            requires DimensionlessUnit<Unit>
        {
            val_ += val;
            return *this;
        }

        constexpr Basic_Quantity& operator-=(Arithmetic auto val) noexcept 
            requires DimensionlessUnit<Unit>
        {
            val_ -= val;
            return *this;
        }

        constexpr Basic_Quantity& operator*=(Arithmetic auto val) noexcept 
        {
            val_ *= val; 
            return *this;
        }

        constexpr Basic_Quantity& operator/=(Arithmetic auto val) noexcept 
        {
            val_ /= val; 
            return *this;
        }

        constexpr Basic_Quantity& operator%=(Arithmetic auto val) noexcept 
            requires std::integral<Rep> 
        {
            val_ %= val; 
            return *this;
        }

        //Converting arithmetic operators
        template<Arithmetic Up, UnitLike OtherUnit> 
            requires UnitAssignable<Unit, OtherUnit>
        constexpr Basic_Quantity& operator+=(Basic_Quantity<Up, OtherUnit> other) noexcept 
        {
            return *this;
        }

        template<Arithmetic Up, UnitLike OtherUnit>
            requires UnitAssignable<Unit, OtherUnit>
        constexpr Basic_Quantity& operator-=(Basic_Quantity<Up, OtherUnit> other) noexcept 
        {
            return *this;
        }

        friend constexpr std::compare_three_way_result_t<Rep> operator<=>(const Basic_Quantity& lhs, const Basic_Quantity& rhs) noexcept = default;
    private:
        Rep val_{};
    };

    template<Arithmetic Tp, UnitLike Unit> 
    Basic_Quantity(Basic_Quantity<Tp, Unit>) -> Basic_Quantity<Tp, Unit>;

    template<typename Tp_, typename Unit_> 
    std::ostream& operator<<(std::ostream& os, Basic_Quantity<Tp_, Unit_> q)
    {
        os << q.value();
        return os;
    }

    template<typename Tp_, typename Unit_> 
    constexpr void swap(Basic_Quantity<Tp_, Unit_>& q1, Basic_Quantity<Tp_, Unit_>& q2) noexcept 
    {
        q1.swap(q2);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
        requires UnitAddable<Unit1, Unit2>
    constexpr auto operator+(Basic_Quantity<Rep1, Unit1> lhs, Basic_Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using SumRep = decltype(lhs.value() + rhs.value());
        using LHSType = Basic_Quantity<Rep1, Unit1>;
        using RHSType = Basic_Quantity<Rep2, Unit2>;

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

        return Basic_Quantity<SumRep, coherent_unit_t<Unit1>>(valUsedLhs + valUsedRhs);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
        requires UnitAddable<Unit1, Unit2>
    constexpr auto operator-(Basic_Quantity<Rep1, Unit1> lhs, Basic_Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using MinusRep = decltype(lhs.value() - rhs.value());
        using LHSType = Basic_Quantity<Rep1, Unit1>;
        using RHSType = Basic_Quantity<Rep2, Unit2>;

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

        return Basic_Quantity<MinusRep, coherent_unit_t<Unit1>>(valUsedLhs - valUsedRhs);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
    constexpr auto operator*(Basic_Quantity<Rep1, Unit1> lhs, Basic_Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using LHSType = Basic_Quantity<Rep1, Unit1>;
        using RHSType = Basic_Quantity<Rep2, Unit2>;
        using ProductUnits = decltype(lhs.toCoherentUnits().units() * rhs.toCoherentUnits().units());
        using ProductRep = decltype(lhs.value() * rhs.value());

        Rep1 valUsedLhs{};
        Rep2 valUsedRhs{};

        if constexpr (!LHSType::isInCoherentUnits())
            valUsedLhs = lhs.toCoherentUnits().value();
        else 
            valUsedLhs = lhs.value();

        if constexpr(!RHSType::isInCoherentUnits())
            valUsedRhs = rhs.toCoherentUnits().value();
        else  
            valUsedRhs = rhs.value();

        return Basic_Quantity<ProductRep, ProductUnits>(valUsedLhs * valUsedRhs);
    }

    template<Arithmetic Rep, UnitLike Unit> 
    constexpr auto operator*(Arithmetic auto lhs, Basic_Quantity<Rep, Unit> rhs) noexcept 
    {
        rhs *= lhs; 
        return rhs;
    }
    
    template<Arithmetic Rep, UnitLike Unit> 
    constexpr auto operator*(Basic_Quantity<Rep, Unit> lhs, Arithmetic auto rhs) noexcept 
    {
        lhs *= rhs; 
        return lhs;
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
    constexpr auto operator/(Basic_Quantity<Rep1, Unit1> lhs, Basic_Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using LHSType = Basic_Quantity<Rep1, Unit1>;
        using RHSType = Basic_Quantity<Rep2, Unit2>;
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

        return Basic_Quantity<ProductRep, ProductUnits>(valUsedLhs / valUsedRhs);
    }

    template<Arithmetic Rep, UnitLike Unit> 
    constexpr auto operator/(Arithmetic auto lhs, Basic_Quantity<Rep, Unit> rhs) noexcept 
    {
        using OutputUnits = unit_inverse_t<Unit>;
        return Basic_Quantity<Rep, OutputUnits>(1/rhs.value());
    }
    
    template<Arithmetic Rep, UnitLike Unit> 
    constexpr auto operator/(Basic_Quantity<Rep, Unit> lhs, Arithmetic auto rhs) noexcept 
    {
        lhs /= rhs; 
        return lhs;
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2>
        requires std::integral<Rep1> && std::integral<Rep2> 
    constexpr auto operator%(Basic_Quantity<Rep1, Unit1> lhs, Basic_Quantity<Rep2, Unit2> rhs) noexcept 
    {
        using LHSType = Basic_Quantity<Rep1, Unit1>;
        using RHSType = Basic_Quantity<Rep2, Unit2>;
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
        
        return Basic_Quantity<ProductRep, ProductUnits>(valUsedLhs % valUsedRhs);
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
        requires UnitAssignable<Unit1, Unit2>
    constexpr auto operator<=>(Basic_Quantity<Rep1, Unit1> lhs, Basic_Quantity<Rep2, Unit2> rhs) noexcept 
    {
        return lhs.toCoherentUnits().value() <=> rhs.toCoherentUnits().value();
    }

    template<Arithmetic Rep1, UnitLike Unit1, Arithmetic Rep2, UnitLike Unit2> 
        requires UnitAssignable<Unit1, Unit2>
    constexpr bool operator==(Basic_Quantity<Rep1, Unit1> lhs, Basic_Quantity<Rep2, Unit2> rhs) noexcept 
    {
        return lhs.toCoherentUnits().value() == rhs.toCoherentUnits().value();
    }

    /// Type alias where the value representation is double
    template<UnitLike U> 
    using Quantity = Basic_Quantity<double, U>;
}