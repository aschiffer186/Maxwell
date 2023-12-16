#pragma once 

#include <compare>
#include <concepts> 
#include <functional>
#include <initializer_list>
#include <ostream>
#include <type_traits>
#include <utility>

#include "UnitCore.hpp"

namespace Maxwell 
{
    template<typename T> 
    concept Arithmetic = requires(const T& a, const T& b, double d)
    {
        a + b;
        a - b;
        a * b;
        a / b;
        a * d;
    };

    template<Arithmetic, UnitLike> 
    class Basic_Quantity;

    template<typename> 
    struct _is_quantity : std::false_type {};

    template<Arithmetic R, UnitLike U> 
    struct _is_quantity<Basic_Quantity<R, U>> : std::true_type {};

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
        constexpr Basic_Quantity() noexcept(std::is_nothrow_default_constructible_v<Rep>)= default;


        /**
         * @brief Constructor
         * 
         * Constructs a Basic_Quantity with the specified units whose value 
         * will be constructed from o as if by direct initializing via the 
         * expression Rep(std::forward<U>(o)). This function only participates 
         * in overload resolution if std::constructible_from<Rep, Up_&&> is true.
         *
         * @tparam Up_ the type of the value 
         * @param o the value of the quantity
         */
        template<typename Up_>
            requires std::constructible_from<Rep, Up_&&>
        constexpr explicit Basic_Quantity(Up_&& o) noexcept(std::is_nothrow_constructible_v<Rep, Up_&&>)
        : val_(std::forward<Up_>(o))
        {

        }
        
        /**
         * @brief Consructor
         * 
         * Constructs a Basic_Quantity with the specified units whose value 
         * will be constructed from o as if by direct initializing via the 
         * expression Rep(std::forward<U>(o)). This function only participates 
         * in overload resolution if std::constructible_from<Rep, Up_&&> is true.
         *
         * The additional units parameter is ignored, but is used for the create of 
         * a template argument deductiion guide
         *
         * @tparam Up_ the type of the value 
         * @param o the value of the quantity
         */
        template<typename U = Rep>
            requires std::constructible_from<Rep, U&&>
        constexpr Basic_Quantity(U&& val, Unit) noexcept(std::is_nothrow_constructible_v<Rep, U&&>)
        : val_{std::forward<U>(val)}
        {

        }

        /**
         * @brief Constructor
         * 
         * Constructs whose a Basic_Quantity whose stored value is constructed from 
         * the specified arguments as if by calling Rep(std::forward<Tps>(args)...). 
         * This function only participates in in overload resolution if 
         * std::constructible_from<Rep, Tps...> is satisified.
         *
         * @tparam Tps the argument types used to construct the value
         * @param args the arguments used to construct the value of *this
         */
        template<typename... Tps> 
            requires std::constructible_from<Rep, Tps...>
        constexpr Basic_Quantity(std::in_place_t, Tps&&... args) noexcept(std::is_nothrow_constructible_v<Rep, Tps...>)
        : val_(std::forward<Tps>(args)...)
        {

        }

          /**
         * @brief Constructor
         * 
         * Constructs whose a Basic_Quantity whose stored value is constructed from 
         * the specified initializer list and arguments as if by calling
         * Rep(il, std::forward<Tps>(args)...). 
         * This function only participates in in overload resolution if 
         * std::constructible_from<Rep, std::initializer_list<U>, Tps...> is satisified.
         *
         * @tparam U the type of the elements of the initializer list
         * @tparam Tps the argument types used to construct the value
         * @param il the initializer list used to construct the value of *this
         * @param args the arguments used to construct the value of *this
         */
        template<typename U, typename... Tps> 
            requires std::constructible_from<Rep, U, Tps...>
        constexpr Basic_Quantity(std::in_place_t, std::initializer_list<U> il, Tps&&... args) 
            noexcept(std::is_nothrow_constructible_v<Rep, std::initializer_list<U>, Tps...>)
        : val_(il, std::forward<Tps>(args)...)
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
        constexpr Basic_Quantity(const Basic_Quantity<Up_, OtherUnit>& o) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
        : val_(o.value())
        {
            val_ *= conversionPrefix(OtherUnit{}, Unit{});
            val_ *= conversionScale(OtherUnit{}, Unit{});
        }

        template<Arithmetic Up_, UnitLike OtherUnit>
            requires UnitAssignable<OtherUnit, Unit>
        constexpr Basic_Quantity(Basic_Quantity<Up_, OtherUnit>&& o) noexcept(std::is_nothrow_move_constructible_v<Rep>)
        : val_(std::move(o.value()))
        {
            val_ *= conversionPrefix(OtherUnit{}, Unit{});
            val_ *= conversionScale(OtherUnit{}, Unit{});
        }

        /**
         * @brief Assigns the value of the specified quantity to this
         * 
         *
         * Assigns the value of the specified Basic_Quantity to *this, converting from 
         * the units o the other quantity to the units of *this if necessary. If the units 
         * of the other quantity cannot be assigned to *this, the program is ill-formed. 
         *
         * @tparam Up_ the representation type of the other quantity
         * @tparam OtherUnit the units of the other quantity
         * @param o the other quantity
         * @return a reference to *this
         */
        template<typename Up_, typename OtherUnit>
            requires UnitAssignable<OtherUnit, Unit>
        constexpr Basic_Quantity& operator=(const Basic_Quantity<Up_, OtherUnit>& o) noexcept(std::is_nothrow_copy_assignable_v<Rep>)
        {
            val_ = o.value();
            val_ *= conversionPrefix(OtherUnit{}, Unit{});
            val_ *= conversionScale(OtherUnit{}, Unit{});

            return *this;
        }

        template<typename Up_, typename OtherUnit>
            requires UnitAssignable<OtherUnit, Unit>
        constexpr Basic_Quantity& operator=(Basic_Quantity<Up_, OtherUnit>&& o) noexcept(std::is_nothrow_move_assignable_v<Rep>)
        {
            val_ = std::move(o.value());
            val_ *= conversionPrefix(OtherUnit{}, Unit{});
            val_ *= conversionScale(OtherUnit{}, Unit{});

            return *this;
        }

        /**
         * @brief Returns the value of the Quantity
         * 
         * @return the value of the Quantity
         */
        constexpr const Rep& value() const noexcept 
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
            const double prefixScale = conversionPrefix(Unit{}, coherent_unit_t<Unit>{});
            const double scale = conversionScale(Unit{}, coherent_unit_t<Unit>{});
            return Basic_Quantity<Rep, coherent_unit_t<Unit>>{val_*prefixScale};
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
        /**
         * @brief Adds the value of the quantity to *this
         * 
         * Adds the value of the quantity to *this.
         * @post this->value() == this->value() + q.value()
         * 
         * @param q the other quantity
         * @return a reference to *this
         */
        constexpr Basic_Quantity& operator+=(Basic_Quantity q) noexcept 
        {
            val_ += q.val_;
            return *this;
        }

        /** @brief Subtracts the value of the quantity from *this
         * 
         * Subtracts the value of the quantity frm *this.
         * @post this->value() == this->value() - q.value()
         *
         * @param q the other quantity
         * @return a reference to *this
         */
        constexpr Basic_Quantity& operator-=(Basic_Quantity q) noexcept 
        {
            val_ -= q.val_;
        }

        /** @brief Multiplies the value of the other quantity by *this
         * 
         * Multiples the value of the other quantity by *this and stores it 
         * it in *this
         * @post this->value() == this->value() * q.value()
         * 
         * @param q the other quantity
         * @return a reference to *this
         */
        constexpr Basic_Quantity& operator*=(Basic_Quantity q) noexcept 
        {
            val_ *= q.val_;
        }

        /** @brief Divides the value of *this by the other quantity
         * 
         * Divides the value of *this by the other quantity and stores it 
         * it in *this
         * @pre q.value() != 0
         * @post this->value() == this->value() / q.value()
         * 
         * @param q the other quantity
         * @return a reference to *this
         */
        constexpr Basic_Quantity& operator/=(Basic_Quantity q) noexcept 
        {
            val_ /= q.val_;
        }

         /** @brief Computes the module of *this with the other quantity
         * 
         * Computes the module of *this with the other quantity and stores it 
         * it in *this. This function only participates in overload resolution 
         * if the represenation type of the quantity is an integer type.
         *
         * @pre q.value() != 0
         * @pre std::integral<Rep>
         * @post this->value() == this->value() % q.value()
         * 
         * @param q the other quantity
         * @return a reference to *this
         */
        constexpr Basic_Quantity& operator%=(Basic_Quantity q) noexcept 
            requires std::integral<Rep>
        {
            return val_ %= q.val_;
        }

        //Dimensioness value arithmetic operators
        /**
         * @brief Adds the scalar value to *this
         * 
         * Adds the scalar value to *this. This function only particpates in overload 
         * resolution if Unit is dimensionless.
         *
         * @pre DimensionlessUnit<Unit>
         * @post this->value() == this->value() + val
         *
         * @param val the value to add to *this
         * @return *this
         */
        constexpr Basic_Quantity& operator+=(Arithmetic auto val) noexcept 
            requires DimensionlessUnit<Unit>
        {
            val_ += val;
            return *this;
        }

         /**
         * @brief Subtracts the scalar value from *this
         * 
         * Adds the scalar value to *this. This function only particpates in overload 
         * resolution if Unit is dimensionless.
         *
         * @pre DimensionlessUnit<Unit>
         * @post this->value() == this->value() - val
         *
         * @param val the value to subtract from *this
         * @return *this
         */
        constexpr Basic_Quantity& operator-=(Arithmetic auto val) noexcept 
            requires DimensionlessUnit<Unit>
        {
            val_ -= val;
            return *this;
        }

        /**
         * @brief Multiplies *this by the scalar value
         * 
         * Multiples the value of *this by the scalar value.
         *
         * @post this->value() == this->value() * val
         *
         * @param val the value to multply by
         * @return *this
         */
        constexpr Basic_Quantity& operator*=(Arithmetic auto val) noexcept 
        {
            val_ *= val; 
            return *this;
        }

        /**
         * @brief Divides *this by the scalar value
         * 
         * Divides the value of *this by the scalar value.
         *
         * @pre this->value() != 0
         * @post this->value() == this->value() / val
         *
         * @param val the value to divide by
         * @return *this
         */
        constexpr Basic_Quantity& operator/=(Arithmetic auto val) noexcept 
        {
            val_ /= val; 
            return *this;
        }

        /**
         * @brief Compules the modulo of *this and the specified value
         * 
         * Compules the modulo of *this and the specified value. This function 
         * only participates in overload resolution if Rep is an integer type. The 
         * behavior is undefined if this->value() == 0
         *
         * @pre std::integral<Rep>
         * @pre this->value() != 0
         * @post this->value() == this->value() / val
         *
         * @param val the value to modulo by
         * @return *this
         */
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
            Basic_Quantity{other};
            val_ += other.val_;
            return *this;
        }

        template<Arithmetic Up, UnitLike OtherUnit>
            requires UnitAssignable<Unit, OtherUnit>
        constexpr Basic_Quantity& operator-=(Basic_Quantity<Up, OtherUnit> other) noexcept 
        {
            Basic_Quantity{other};
            val_ -= other.val_;
            return *this;
        }

        friend constexpr std::compare_three_way_result_t<Rep> operator<=>(const Basic_Quantity& lhs, const Basic_Quantity& rhs) noexcept = default;
    private:
        Rep val_{};
    };

    // Deduction guides
    template<Arithmetic Tp, UnitLike Unit> 
    Basic_Quantity(Basic_Quantity<Tp, Unit>) -> Basic_Quantity<Tp, Unit>;

    template<Arithmetic Tp, UnitLike Unit> 
    Basic_Quantity(Tp, Unit) -> Basic_Quantity<Tp, Unit>;

    template<typename Tp_, typename Unit_> 
    std::ostream& operator<<(std::ostream& os, Basic_Quantity<Tp_, Unit_> q)
    {
        os << q.value();
        os << q.units();
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
            valUsedRhs = rhs.toCoherentUnits().value();
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

    template<typename Tp, Arithmetic Rep, UnitLike Unit> 
         requires (!_is_quantity<Tp>::value && Arithmetic<Tp>)
    constexpr auto operator*(Tp lhs, Basic_Quantity<Rep, Unit> rhs) noexcept 
    {
        rhs *= lhs; 
        return rhs;
    }
    
    template<typename Tp, Arithmetic Rep, UnitLike Unit> 
        requires (!_is_quantity<Tp>::value && Arithmetic<Tp>)
    constexpr auto operator*(Basic_Quantity<Rep, Unit> lhs, Tp rhs) noexcept 
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

    template<typename Tp, Arithmetic Rep, UnitLike Unit> 
        requires (!_is_quantity<Tp>::value && Arithmetic<Tp>)
    constexpr auto operator/(Tp lhs, Basic_Quantity<Rep, Unit> rhs) noexcept 
        requires (!_is_quantity<decltype(lhs)>::value)
    {
        using OutputUnits = unit_inverse_t<Unit>;
        return Basic_Quantity<Rep, OutputUnits>(1/rhs.value());
    }
    
    template<typename Tp, Arithmetic Rep, UnitLike Unit> 
        requires (!_is_quantity<Tp>::value && Arithmetic<Tp>)
    constexpr auto operator/(Basic_Quantity<Rep, Unit> lhs, Tp rhs) noexcept 
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

namespace std 
{
    template<Maxwell::Arithmetic Rep, Maxwell::UnitLike Unit> 
    struct hash<Maxwell::Basic_Quantity<Rep, Unit>>
    {
        std::uint64_t operator()(const Maxwell::Basic_Quantity<Rep, Unit>& q) noexcept 
        {
            return hash<Rep>{}(q.value());
        }
    };
}