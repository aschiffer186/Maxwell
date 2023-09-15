#pragma once 

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

    template<Arithmetic Tp_, UnitLike Unit_>
    struct Quantity : Unit_
    {
        using Rep = Tp_;
        using Unit = Unit_;

        constexpr Quantity() noexcept = default;

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

        template<typename Up_, typename Other_>
        constexpr Quantity& operator=(Quantity<Up_, Other_> o) noexcept 
        {
            val_ = static_cast<Rep>(o.val_);
            return *this;
        }

        constexpr Rep value() const noexcept 
        {
            return val_;
        }

        constexpr Unit units() const noexcept 
        {
            return *this;
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

        friend constexpr auto operator<=>(Quantity lhs, Quantity rhs) noexcept = default;
    private:
        Rep val_;
    };

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
            valUsedRhs = rhs.toCoherentUnits().valu();
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
        using ProductUnits = decltype(lhs.units() * rhs.units());
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
        using ProductUnits = decltype(lhs.units() / rhs.units());
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
        using ProductUnits = decltype(lhs.units() / rhs.units());
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