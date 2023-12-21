#ifndef UNIT_CORE_HPP 
#define UNIT_CORE_HPP

#include <array>
#include <concepts>
#include <ratio>
#include <type_traits>

namespace Maxwell 
{
    /**
     * @brief Represents one "dimension"
     * 
     * The base unit class template represents one "dimension" 
     * e.g. length, time, mass, etc. and how it is scaled relative 
     * to the SI coherent unit of that dimension. The scaling is expressed 
     * through template parameters, allowing it to be queried at compile-time. 
     * 
     * The UnitBase can be related to the coherent unit through the following equation:
     *
     * UB = 10^Prefix * ScaleNum/ScaleDenom * U ^ Pow 
     *
     * where UB is the modified unit and U is the coherent unit.
     *
     * The cohrent unit will have a prefix of 0, pow of 1 and scale numerato/denominator 
     * of one. 
     *
     * @tparam Prefix_ the metric prefix
     * @tparam Pow_ the power of the unit
     * @tparam ScaleNum_ the numerator of the scale factor
     * @tparam ScaleDenom_ the denominator of the scale factor
     */
    template<std::signed_integral auto Prefix_, 
             std::signed_integral auto Pow_,
             std::intmax_t ScaleNum_ = 1, 
             std::intmax_t ScaleDenom_ = 1>
    struct UnitBase
    {
        /// The metric prefix of the unit
        static constexpr auto Prefix = Prefix_;
        /// The power of the unit
        static constexpr auto Pow = Pow_;
        /// The scale factor of th unit 
        using Scale = std::ratio<ScaleNum_, ScaleDenom_>;
    };

    // Unit base type aliases
    /// The NullUnit represents a dimension not part of the overall unit
    using NullUnit = UnitBase<0, 0>;

    /**
     * @brief Scales the unit's prefix
     * 
     * Scales the unit's prefix by the specified power of 10. The scaled 
     * unit is representged by the member type "type". It is related to the 
     * original unit by the following formula: 
     *
     * type = 10^amt * UnitBaseT
     *
     * @tparam UnitBaseT the original unit
     * @tparam Amt the power of 10 to scale by
     */
    template<typename UnitBaseT, std::signed_integral auto Amt> 
    struct scale_unit_base
    {
        /// The scaled unit
        using type = UnitBase<UnitBaseT::Prefix + Amt, UnitBaseT::Pow>;
    };

    template<typename UnitBaseT, std::signed_integral auto Amt>
    using scale_unit_base_t = scale_unit_base<UnitBaseT, Amt>::type;

    //Unit traits 
    template<typename>
    struct is_unit_base : std::false_type {};

    template<std::integral auto Prefix_, 
             std::integral auto Pow_, 
             std::intmax_t ScaleNum_, 
             std::intmax_t ScaleDenom_>
    struct is_unit_base<UnitBase<Prefix_, Pow_, ScaleNum_, ScaleDenom_>> : std::true_type{};

    /// True if the template parameter is an instantiation of class template UnitBase
    template<typename Tp>
    inline constexpr bool is_unit_base_v = is_unit_base<Tp>::value;

    /// Concept that is true if the type is an instantiation of class template UnitBase
    template<typename Tp>
    concept UnitBaseLike = is_unit_base_v<Tp>;

    template<UnitBaseLike UnitBase1, UnitBaseLike UnitBase2> 
    struct is_unit_base_equal : std::bool_constant<UnitBase1::Prefix == UnitBase2::Prefix &&
                                                   UnitBase1::Pow == UnitBase2::Pow &&
                                                   UnitBase1::Scale::num == UnitBase2::Scale::num &&
                                                   UnitBase1::Scale::den == UnitBase2::Scale::den> {};

    /**
     * @brief Variable template that is true if two unit bases are equal
     * 
     * Variable template that is true if two unit bases are equal. Two unit bases are equal 
     * if and only if they have the same prefix, power, and scale.
     *
     * @tparam UnitBase1 one unit to compare for equality
     * @tparam UnitBase2 the other unit to compare for equality
     */
    template<UnitBaseLike UnitBase1, UnitBaseLike UnitBase2> 
    inline constexpr bool is_unit_base_equal_v = is_unit_base_equal<UnitBase1, UnitBase2>::value;
    
    template<UnitBaseLike UnitBase1, UnitBaseLike UnitBase2> 
    struct is_unit_base_not_equal : std::bool_constant<!is_unit_base_equal_v<UnitBase1, UnitBase2>> {};

    template<UnitBaseLike UnitBase1, UnitBaseLike UnitBase2> 
    inline constexpr bool is_unit_base_not_equal_v = is_unit_base_not_equal<UnitBase1, UnitBase2>::value;

    template<typename LHS, typename RHS> 
    concept UnitBaseAddable = UnitBaseLike<LHS> && UnitBaseLike<RHS> &&
        LHS::Pow == RHS::Pow;

    template<UnitBaseLike Unit> 
    struct coherent_unit_base 
    {
        using type = UnitBase<0, Unit::Pow>;
    };

    template<UnitBaseLike Unit> 
    using coherent_unit_base_t = coherent_unit_base<Unit>::type;

    // Unit relations 
    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
        requires (Unit1::Prefix == Unit2::Prefix || 
                 (Unit1::Prefix == 0 || Unit2::Prefix == 0))
    struct unit_base_product 
    {
    private:
        static constexpr int productPrefix(Unit1, Unit2) noexcept
        {
            int lhsPrefix = Unit1::Prefix;
            int rhsPrefix = Unit2::Prefix;

            if (lhsPrefix == 0 && rhsPrefix == 0)
                return 0;
            if (lhsPrefix == 0)
                return rhsPrefix;
            if (rhsPrefix == 0)
                return lhsPrefix; 
            return lhsPrefix;
        }
    public:
        using type = UnitBase<productPrefix(Unit1{}, Unit2{}), 
                              Unit1::Pow + Unit2::Pow, 
                              Unit1::Scale::num, 
                              Unit1::Scale::den>;
    };

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    using unit_base_product_t = unit_base_product<Unit1, Unit2>::type;

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    constexpr auto operator*(Unit1, Unit2) noexcept 
    {
        return unit_base_product_t<Unit1, Unit2>{};
    }

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    requires (Unit1::Prefix == Unit2::Prefix || 
              (Unit1::Prefix == 0 || Unit2::Prefix == 0))
    struct unit_base_quotient
    { 
    private:
        static constexpr int quotientPrefix(Unit1, Unit2) noexcept
        {
            int lhsPrefix = Unit1::Prefix;
            int rhsPrefix = Unit2::Prefix;

            if (lhsPrefix == 0 && rhsPrefix == 0)
                return 0;
            if (lhsPrefix == 0)
                return rhsPrefix;
            if (rhsPrefix == 0)
                return lhsPrefix; 
            return lhsPrefix;
        }
    public:
        using type = UnitBase<quotientPrefix(Unit1{}, Unit2{}), Unit1::Pow - Unit2::Pow, Unit1::Scale::num, Unit1::Scale::den>;
    };

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    using unit_base_quotient_t = unit_base_quotient<Unit1, Unit2>::type;

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    constexpr auto operator/(Unit1, Unit2) noexcept 
    {
        return unit_base_quotient_t<Unit1, Unit2>{};
    }

    template<UnitBaseLike U> 
    struct unit_base_inverse 
    {
        using type = UnitBase<U::Prefix, -1*U::Pow>;
    };

    template<UnitBaseLike U> 
    using unit_base_inverse_t = unit_base_inverse<U>::type;

    // Metric prefixes
    inline constexpr std::integral auto quetta = 30;
    inline constexpr std::integral auto ronna = 27;
    inline constexpr std::integral auto yotta = 24;
    inline constexpr std::integral auto zetta = 21;
    inline constexpr std::integral auto exa = 18;
    inline constexpr std::integral auto peta = 15;
    inline constexpr std::integral auto tera = 12;
    inline constexpr std::integral auto giga = 9;
    inline constexpr std::integral auto mega = 6;
    inline constexpr std::integral auto kilo = 3;
    inline constexpr std::integral auto hecto = 2;
    inline constexpr std::integral auto deca = 1;
    inline constexpr std::integral auto deci = -1;
    inline constexpr std::integral auto centi = -2;
    inline constexpr std::integral auto milli = -3;
    inline constexpr std::integral auto micro = -6;
    inline constexpr std::integral auto nano = -9;
    inline constexpr std::integral auto pico = -12;
    inline constexpr std::integral auto femto = -15;
    inline constexpr std::integral auto atto = -18;
    inline constexpr std::integral auto zepto = -21;
    inline constexpr std::integral auto yocto = -24; 
    inline constexpr std::integral auto ronto = -27;
    inline constexpr std::integral auto quecto = -30;

    template<UnitBaseLike Time_,
             UnitBaseLike Length_,
             UnitBaseLike Mass_, 
             UnitBaseLike Current_, 
             UnitBaseLike Temperature_, 
             UnitBaseLike Amount_, 
             UnitBaseLike Luminosity_, 
             UnitBaseLike Angle_ = NullUnit>
    struct Unit 
    {
        using Time = Time_;
        using Length = Length_;
        using Mass = Mass_;
        using Current = Current_;
        using Temperature = Temperature_;
        using Amount = Amount_; 
        using Luminosity = Luminosity_;
        using Angle = Angle_;
    };

    template<typename> 
    struct is_unit : std::false_type{};

    template<UnitBaseLike Time_,
             UnitBaseLike Length_,
             UnitBaseLike Mass_, 
             UnitBaseLike Current_, 
             UnitBaseLike Temperature_, 
             UnitBaseLike Amount_, 
             UnitBaseLike Luminosity_,
             UnitBaseLike Angle_>
    struct is_unit<Unit<Time_, Length_, Mass_, Current_, Temperature_, Amount_, Luminosity_, Angle_>> : std::true_type {};

    template<typename Tp> 
    inline constexpr bool is_unit_v = is_unit<Tp>::value;

    template<typename Tp> 
    concept UnitLike = is_unit_v<Tp>;

    template<UnitLike Tp> 
    struct is_unitless : std::bool_constant<Tp::Time::Pow        == 0 &&
                                            Tp::Length::Pow      == 0 &&
                                            Tp::Mass::Pow        == 0 &&
                                            Tp::Current::Pow     == 0 &&
                                            Tp::Temperature::Pow == 0 &&
                                            Tp::Amount::Pow      == 0 &&
                                            Tp::Luminosity::Pow  == 0 &&
                                            Tp::Angle::Pow       == 0>{};

    template<UnitLike Tp> 
    inline constexpr bool is_unitless_v = is_unitless<Tp>::value;

    template<typename Tp> 
    concept Unitless = is_unitless_v<Tp>;

    template<UnitLike Unit1, UnitLike Unit2> 
    struct is_unit_equal : std::bool_constant<is_unit_base_equal_v<typename Unit1::Time,        typename Unit2::Time>        &&
                                              is_unit_base_equal_v<typename Unit1::Length,      typename Unit2::Length>      &&
                                              is_unit_base_equal_v<typename Unit1::Mass,        typename Unit2::Mass>        && 
                                              is_unit_base_equal_v<typename Unit1::Current,     typename Unit2::Current>     && 
                                              is_unit_base_equal_v<typename Unit1::Temperature, typename Unit2::Temperature> && 
                                              is_unit_base_equal_v<typename Unit1::Amount,      typename Unit2::Amount>      && 
                                              is_unit_base_equal_v<typename Unit1::Luminosity,  typename Unit2::Luminosity>  &&
                                              is_unit_base_equal_v<typename Unit1::Angle,       typename Unit2::Angle>> {};
    
    template<UnitLike Unit1, UnitLike Unit2> 
    inline constexpr bool is_unit_equal_v = is_unit_equal<Unit1, Unit2>::value;

    template<UnitLike Unit1, UnitLike Unit2> 
    struct is_unit_not_equal : std::bool_constant<!is_unit_equal_v<Unit1, Unit2>> {};

    template<UnitLike Unit1, UnitLike Unit2> 
    inline constexpr bool is_unit_not_equal_v = is_unit_not_equal<Unit1, Unit2>::value;

    template<UnitLike Unit1, UnitLike Unit2> 
    constexpr bool operator==(Unit1, Unit2) noexcept 
    {
        return is_unit_equal_v<Unit1, Unit2>;
    }

    template<UnitLike Unit1, UnitLike Unit2> 
    constexpr bool operator!=(Unit1, Unit2) noexcept 
    {
        return is_unit_not_equal_v<Unit1, Unit2>;
    }

    template<UnitLike U> 
    struct coherent_unit 
    {
        using type = Unit<coherent_unit_base_t<typename U::Time>,
                          coherent_unit_base_t<typename U::Length>,
                          coherent_unit_base_t<typename U::Mass>,
                          coherent_unit_base_t<typename U::Current>,
                          coherent_unit_base_t<typename U::Temperature>,
                          coherent_unit_base_t<typename U::Amount>,
                          coherent_unit_base_t<typename U::Luminosity>>;
    };

    template<UnitLike U> 
    using coherent_unit_t = coherent_unit<U>::type;

    template<UnitLike Unit1, UnitLike Unit2> 
    struct unit_product
    {
        using type = Unit<unit_base_product_t<typename Unit1::Time,        typename Unit2::Time>,
                          unit_base_product_t<typename Unit1::Length,      typename Unit2::Length>,
                          unit_base_product_t<typename Unit1::Mass,        typename Unit2::Mass>,
                          unit_base_product_t<typename Unit1::Current,     typename Unit2::Current>,
                          unit_base_product_t<typename Unit1::Temperature, typename Unit2::Temperature>,
                          unit_base_product_t<typename Unit1::Amount,      typename Unit2::Amount>,
                          unit_base_product_t<typename Unit1::Luminosity,  typename Unit2::Luminosity>,
                          unit_base_product_t<typename Unit1::Angle,       typename Unit2::Angle>>;
    };

    template<UnitLike Unit1, UnitLike Unit2>
    using unit_product_t = unit_product<Unit1, Unit2>::type;

    template<UnitLike Unit1, UnitLike Unit2> 
    constexpr auto operator*(Unit1, Unit2) noexcept 
    {
        return unit_product_t<Unit1, Unit2>{};
    }

    template<UnitLike Unit1, UnitLike Unit2> 
    struct unit_quotient 
    {
        using type = Unit<unit_base_quotient_t<typename Unit1::Time,        typename Unit2::Time>,
                          unit_base_quotient_t<typename Unit1::Length,      typename Unit2::Length>,
                          unit_base_quotient_t<typename Unit1::Mass,        typename Unit2::Mass>,
                          unit_base_quotient_t<typename Unit1::Current,     typename Unit2::Current>,
                          unit_base_quotient_t<typename Unit1::Temperature, typename Unit2::Temperature>,
                          unit_base_quotient_t<typename Unit1::Amount,      typename Unit2::Amount>,
                          unit_base_quotient_t<typename Unit1::Luminosity,  typename Unit2::Luminosity>,
                          unit_base_quotient_t<typename Unit1::Angle,       typename Unit2::Angle>>;
    };

    template<UnitLike Unit1, UnitLike Unit2> 
    using unit_quotient_t = unit_quotient<Unit1, Unit2>::type;

    template<UnitLike Unit1, UnitLike Unit2> 
    constexpr auto operator/(Unit1, Unit2) noexcept 
    {
        return unit_quotient_t<Unit1, Unit2>{};
    }

    template<UnitLike U> 
    struct unit_inverse 
    {
        using type = Unit<unit_base_inverse_t<typename U::Time>,
                          unit_base_inverse_t<typename U::Length>,
                          unit_base_inverse_t<typename U::Mass>,
                          unit_base_inverse_t<typename U::Current>,
                          unit_base_inverse_t<typename U::Temperature>,
                          unit_base_inverse_t<typename U::Amount>,
                          unit_base_inverse_t<typename U::Luminosity>,
                          unit_base_inverse_t<typename U::Angle>>;
                          
    };

    template<UnitLike U> 
    using unit_inverse_t = unit_inverse<U>::type;

    template<typename LHS, typename RHS> 
    concept UnitAddable = UnitLike<LHS> && UnitLike<RHS>                      &&
        UnitBaseAddable<typename LHS::Time,        typename RHS::Time>        &&
        UnitBaseAddable<typename LHS::Length,      typename RHS::Length>      &&
        UnitBaseAddable<typename LHS::Mass,        typename RHS::Mass>        &&
        UnitBaseAddable<typename LHS::Current,     typename RHS::Current>     &&
        UnitBaseAddable<typename LHS::Temperature, typename RHS::Temperature> &&
        UnitBaseAddable<typename LHS::Amount,      typename RHS::Amount>      &&
        UnitBaseAddable<typename LHS::Luminosity,  typename RHS::Luminosity>  &&
        UnitBaseAddable<typename LHS::Angle,       typename RHS::Angle>;

    template<UnitLike From, UnitLike To> 
    struct is_unit_assignable
    {
        static constexpr bool value = UnitAddable<From, To>;
    };

    template<UnitLike From, UnitLike To> 
    inline constexpr bool is_unit_assignable_v = is_unit_assignable<From, To>::value;

    template<typename From, typename To> 
    concept UnitAssignable = UnitLike<From> && UnitLike<To> && is_unit_assignable_v<From, To>;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_time 
    {
        using type = Unit<scale_unit_base_t<typename U::Time, Amt>,
                          typename U::Length, 
                          typename U::Mass, 
                          typename U::Current, 
                          typename U::Temperature, 
                          typename U::Amount,
                          typename U::Luminosity>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_time_t = scale_unit_time<U, Amt>::type;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_length
    {
        using type = Unit<typename U::Time,
                          scale_unit_base_t<typename U::Length, Amt>, 
                          typename U::Mass, 
                          typename U::Current, 
                          typename U::Temperature, 
                          typename U::Amount,
                          typename U::Luminosity>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_length_t = scale_unit_length<U, Amt>::type;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_current
    {
        using type = Unit<typename U::Time,
                          typename U::Length,
                          typename U::Mass, 
                          scale_unit_base_t<typename U::Current, Amt>, 
                          typename U::Temperature, 
                          typename U::Amount,
                          typename U::Luminosity>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_current_t = scale_unit_current<U, Amt>::type;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_temperature
    {
        using type = Unit<typename U::Time,
                          typename U::Length,
                          typename U::Mass, 
                          typename U::Current, 
                          scale_unit_base_t<typename U::Temperature, Amt>,
                          typename U::Amount,
                          typename U::Luminosity>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_temperature_t = scale_unit_temperature<U, Amt>::type;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_mass
    {
        using type = Unit<typename U::Time,
                          typename U::Length, 
                          scale_unit_base_t<typename U::Mass, Amt>, 
                          typename U::Current, 
                          typename U::Temperature, 
                          typename U::Amount,
                          typename U::Luminosity>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_mass_t = scale_unit_mass<U, Amt>::type;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_amount
    {
        using type = Unit<typename U::Time,
                          typename U::Length, 
                          typename U::Mass,
                          typename U::Current, 
                          typename U::Temperature, 
                          scale_unit_base_t<typename U::Amount, Amt>,
                          typename U::Luminosity>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_amount_t = scale_unit_amount<U, Amt>::type;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_luminosity
    {
        using type = Unit<typename U::Time,
                          typename U::Length, 
                          typename U::Mass,
                          typename U::Current, 
                          typename U::Temperature, 
                          typename U::Amount,
                          scale_unit_base_t<typename U::Luminosity, Amt>>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_luminosity_t = scale_unit_luminosity<U, Amt>::type;

    template<UnitLike U, std::integral auto Amt> 
    struct scale_unit_angle
    {
        using type = Unit<typename U::Time,
                          typename U::Length, 
                          typename U::Mass,
                          typename U::Current, 
                          typename U::Temperature, 
                          typename U::Amount,
                          typename U::Luminosity,
                          scale_unit_base_t<typename U::Angle, Amt>>;
    };

    template<UnitLike U, std::integral auto Amt>
    using scale_unit_angle_t = scale_unit_angle<U, Amt>::type;

    inline constexpr std::array powsOfTen{1e-30,
                         1e-29, 
                         1e-28, 
                         1e-27, 
                         1e-26, 
                         1e-25, 
                         1e-24, 
                         1e-23, 
                         1e-22, 
                         1e-21, 
                         1e-20, 
                         1e-19, 
                         1e-18, 
                         1e-17, 
                         1e-16, 
                         1e-15, 
                         1e-14, 
                         1e-13, 
                         1e-12, 
                         1e-11, 
                         1e-10, 
                         1e-9, 
                         1e-8, 
                         1e-7, 
                         1e-6, 
                         1e-5, 
                         1e-4, 
                         1e-3, 
                         1e-2, 
                         1e-1,
                         1.0,
                         1e1, 
                         1e2, 
                         1e3, 
                         1e4, 
                         1e5, 
                         1e6, 
                         1e7, 
                         1e8, 
                         1e9, 
                         1e10, 
                         1e11, 
                         1e12, 
                         1e13, 
                         1e14, 
                         1e15, 
                         1e16, 
                         1e17, 
                         1e18, 
                         1e19, 
                         1e20, 
                         1e21, 
                         1e22, 
                         1e23, 
                         1e24, 
                         1e25, 
                         1e26, 
                         1e27, 
                         1e28, 
                         1e29, 
                         1e30};
    
    consteval double pow10(int pow)
    {
        if (pow >= -30 && pow <= 30)
        {
            return powsOfTen[pow + 30];
        }
        else 
        {
            if (pow < 0)
                return 1.0 / pow10(-1*pow);
            double retVal = 1;
            double multiplicand = 10;
            while (true) 
            {
                retVal *= (pow & 1) ? multiplicand : 1;
                pow = pow / 2;
                if (pow == 0)
                    break;
                multiplicand *= multiplicand;
            }
            return retVal;
        }
    }

    consteval double pow(double base, int exp)
    {
        if (exp < 0)
            return pow(1/base, -exp);
        if (exp == 0)
            return 1.0;
        if (exp == 1)
            return base;
        double retVal = 1.0;
        for(int i = 0; i < exp; ++i)
            retVal *= base;
        return retVal;
    }

    consteval double conversionPrefix(UnitLike auto from, UnitLike auto to) noexcept 
    {
        using LHSType = decltype(from);
        using RHSType = decltype(to);

        double scale = 1.0;
        scale *= pow(pow10(LHSType::Time::Prefix        - RHSType::Time::Prefix), LHSType::Time::Pow);
        scale *= pow(pow10(LHSType::Length::Prefix      - RHSType::Length::Prefix), LHSType::Length::Pow);
        scale *= pow(pow10(LHSType::Mass::Prefix        - RHSType::Mass::Prefix), LHSType::Mass::Pow);
        scale *= pow(pow10(LHSType::Current::Prefix     - RHSType::Current::Prefix), LHSType::Current::Pow);
        scale *= pow(pow10(LHSType::Temperature::Prefix - RHSType::Temperature::Prefix), LHSType::Temperature::Pow);
        scale *= pow(pow10(LHSType::Amount::Prefix      - RHSType::Amount::Prefix), LHSType::Amount::Pow);
        scale *= pow(pow10(LHSType::Luminosity::Prefix  - RHSType::Luminosity::Prefix), LHSType::Luminosity::Pow);
        scale *= pow(pow10(LHSType::Angle::Prefix       - RHSType::Angle::Prefix), LHSType::Angle::Pow);

        return scale;
    }

    consteval double conversionScale(UnitLike auto from, UnitLike auto to) noexcept 
    {
        using FromType = decltype(from);
        using ToType = decltype(to);

        using FromTimeScale    = FromType::Time::Scale;
        using ToTimeScale    = ToType::Time::Scale;
        using FromLenScale     = FromType::Length::Scale;
        using ToLenScale     = ToType::Length::Scale;
        using FromMassScale    = FromType::Mass::Scale;
        using ToMassScale    = ToType::Mass::Scale;
        using FromCurrentScale = FromType::Current::Scale; 
        using ToCurrentScale = ToType::Current::Scale;
        using ToTempScale    = ToType::Temperature::Scale;
        using FromTempScale    = FromType::Temperature::Scale; 
        using FromAmtScale     = FromType::Amount::Scale;
        using ToAmtScale     = ToType::Amount::Scale; 
        using FromLumScale     = FromType::Luminosity::Scale; 
        using ToLumScale     = ToType::Luminosity::Scale;
        using FromAngScale     = FromType::Angle::Scale;
        using ToAngScale     = ToType::Angle::Scale;

        //Rad scale = 1
        //Deg scale = 180/pi
        //Rad --> deg divide = deg scale / 1
        //Deg --> Rad = 1 / scale scale 
        // to scale / from scale
        
        double conversion = 1.0;
        const double timeFromScale = static_cast<double>(FromTimeScale::num)/static_cast<double>(FromTimeScale::den);
        const double timeToScale   = static_cast<double>(ToTimeScale::num)/static_cast<double>(ToTimeScale::den);
        conversion *= pow(timeFromScale/timeToScale, FromType::Time::Pow);

        const double lenFromScale = static_cast<double>(FromLenScale::num)/static_cast<double>(FromLenScale::den);
        const double lenToScale   = static_cast<double>(ToLenScale::num)/static_cast<double>(ToLenScale::den);
        conversion *= pow(lenFromScale/lenToScale, FromType::Length::Pow);

        const double massFromScale = static_cast<double>(FromMassScale::num)/static_cast<double>(FromMassScale::den);
        const double massToScale   = static_cast<double>(ToMassScale::num)/static_cast<double>(ToMassScale::den);
        conversion *= pow(massFromScale/massToScale, FromType::Mass::Pow);

        const double curretFromScale = static_cast<double>(FromCurrentScale::num)/static_cast<double>(FromCurrentScale::den);
        const double curretToScale   = static_cast<double>(ToCurrentScale::num)/static_cast<double>(ToCurrentScale::den);
        conversion *= pow(curretFromScale/curretToScale, FromType::Current::Pow);

        const double tempFromScale = static_cast<double>(FromTempScale::num)/static_cast<double>(FromTempScale::den);
        const double tempToScale   = static_cast<double>(ToTempScale::num)/static_cast<double>(ToTempScale::den);
        conversion *= pow(tempFromScale/tempToScale, FromType::Temperature::Pow);

        const double amtFromScale = static_cast<double>(FromAmtScale::num)/static_cast<double>(FromAmtScale::den);
        const double amtToScale   = static_cast<double>(ToAmtScale::num)/static_cast<double>(ToAmtScale::den);
        conversion *= pow(amtFromScale/amtToScale, FromType::Amount::Pow);

        const double lumFromScale = static_cast<double>(FromLumScale::num)/static_cast<double>(FromLumScale::den);
        const double lumToScale   = static_cast<double>(ToLumScale::num)/static_cast<double>(ToLumScale::den);
        conversion *= pow(lumFromScale/lumToScale, FromType::Luminosity::Pow);

        const double angleFromScale = static_cast<double>(FromAngScale::num)/static_cast<double>(FromAngScale::den);
        const double angToScale     = static_cast<double>(ToAngScale::num)/static_cast<double>(ToAngScale::den);
        conversion *= pow(angleFromScale/angToScale, FromType::Angle::Pow);
        
        return conversion;
    }

    //SI Base Units 
    using SecondUnit = Unit<UnitBase<0, 1>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using MeterUnit = Unit<NullUnit, UnitBase<0, 1>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using KilogramUnit = Unit<NullUnit, NullUnit, UnitBase<3, 1>, NullUnit, NullUnit, NullUnit, NullUnit>; 
    using AmpereUnit = Unit<NullUnit, NullUnit, NullUnit, UnitBase<0, 1>, NullUnit, NullUnit, NullUnit>; 
    using KelvinUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>, NullUnit, NullUnit>; 
    using MoleUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>, NullUnit>;
    using CandelaUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>>;
    using Dimensionless_Unit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using RadianUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>>;
}

#endif