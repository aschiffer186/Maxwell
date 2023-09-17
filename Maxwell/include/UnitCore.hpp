#pragma once 

#include <concepts>
#include <ratio>
#include <type_traits>

namespace Maxwell 
{
    template<std::signed_integral auto Prefix_, 
             std::signed_integral auto Pow_,
             std::signed_integral auto ScaleNum_ = 1, 
             std::signed_integral auto ScaleDenom_ = 1>
    struct UnitBase
    {
        static constexpr auto Prefix = Prefix_;
        static constexpr auto Pow = Pow_;
        using Scale = std::ratio<ScaleNum_, ScaleDenom_>;
    };

    // Unit base type aliases
    using NullUnit = UnitBase<0, 0>;

    template<typename UnitBaseT, std::signed_integral auto Amt> 
    struct scale_unit_base
    {
        using type = UnitBase<UnitBaseT::Prefix + Amt, UnitBaseT::Pow>;
    };

    template<typename UnitBaseT, std::signed_integral auto Amt>
    using scale_unit_base_t = scale_unit_base<UnitBaseT, Amt>::type;

    //Unit traits 
    template<typename>
    struct is_unit_base : std::false_type {};

    template<std::integral auto Prefix_, std::integral auto Pow_, std::signed_integral auto ScaleNum_, 
             std::signed_integral auto ScaleDenom_ >
    struct is_unit_base<UnitBase<Prefix_, Pow_, ScaleNum_, ScaleDenom_>> : std::true_type{};

    template<typename Tp>
    inline constexpr bool is_unit_base_v = is_unit_base<Tp>::value;

    template<typename Tp>
    concept UnitBaseLike = is_unit_base_v<Tp>;

    template<UnitBaseLike UnitBase1, UnitBaseLike UnitBase2> 
    struct is_unit_base_equal : std::bool_constant<UnitBase1::Prefix == UnitBase2::Prefix &&
                                                   UnitBase1::Pow == UnitBase2::Pow> {};

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
    struct unit_base_product 
    {
        using type = UnitBase<Unit1::Prefix + Unit2::Prefix, Unit1::Pow + Unit2::Pow>;
    };

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    using unit_base_product_t = unit_base_product<Unit1, Unit2>::type;

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    constexpr auto operator*(Unit1, Unit2) noexcept 
    {
        return unit_base_product_t<Unit1, Unit2>{};
    }

    template<UnitBaseLike Unit1, UnitBaseLike Unit2> 
    struct unit_base_quotient
    {
        using type = UnitBase<Unit1::Prefix - Unit2::Prefix, Unit1::Pow - Unit2::Pow>;
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
    using unit_base_inverse_t = unit_base_inverse<U>;

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
    struct is_dimensionless_unit : std::bool_constant<Tp::Time::Pow == 0 &&
                                                      Tp::Length::Pow == 0 &&
                                                      Tp::Mass::Pow == 0 &&
                                                      Tp::Current::Pow == 0 &&
                                                      Tp::Temperature::Pow == 0 &&
                                                      Tp::Amount::Pow == 0 &&
                                                      Tp::Luminosity::Pow == 0>{};

    template<UnitLike Tp> 
    inline constexpr bool is_dimensionless_unit_v = is_dimensionless_unit<Tp>::value;

    template<typename Tp> 
    concept DimensionlessUnit = is_dimensionless_unit_v<Tp>;

    template<UnitLike Unit1, UnitLike Unit2> 
    struct is_unit_equal : std::bool_constant<is_unit_base_equal_v<typename Unit1::Time, typename Unit2::Time> &&
                                              is_unit_base_equal_v<typename Unit1::Length, typename Unit2::Length> &&
                                              is_unit_base_equal_v<typename Unit1::Mass, typename Unit2::Mass> && 
                                              is_unit_base_equal_v<typename Unit1::Current, typename Unit2::Current> && 
                                              is_unit_base_equal_v<typename Unit1::Temperature, typename Unit2::Temperature> && 
                                              is_unit_base_equal_v<typename Unit1::Amount, typename Unit2::Amount> && 
                                              is_unit_base_equal_v<typename Unit1::Luminosity, typename Unit2::Luminosity>> {};
    
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
        using type = Unit<unit_base_product_t<typename Unit1::Time, typename Unit2::Time>,
                          unit_base_product_t<typename Unit1::Length, typename Unit2::Length>,
                          unit_base_product_t<typename Unit1::Mass, typename Unit2::Mass>,
                          unit_base_product_t<typename Unit1::Current, typename Unit2::Current>,
                          unit_base_product_t<typename Unit1::Temperature, typename Unit2::Temperature>,
                          unit_base_product_t<typename Unit1::Amount, typename Unit2::Amount>,
                          unit_base_product_t<typename Unit1::Luminosity, typename Unit2::Luminosity>>;
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
        using type = Unit<unit_base_quotient_t<typename Unit1::Time, typename Unit2::Time>,
                          unit_base_quotient_t<typename Unit1::Length, typename Unit2::Length>,
                          unit_base_quotient_t<typename Unit1::Mass, typename Unit2::Mass>,
                          unit_base_quotient_t<typename Unit1::Current, typename Unit2::Current>,
                          unit_base_quotient_t<typename Unit1::Temperature, typename Unit2::Temperature>,
                          unit_base_quotient_t<typename Unit1::Amount, typename Unit2::Amount>,
                          unit_base_quotient_t<typename Unit1::Luminosity, typename Unit2::Luminosity>>;
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
        using type = Unit<unit_base_inverse<typename U::Time>,
                          unit_base_inverse<typename U::Length>,
                          unit_base_inverse<typename U::Mass>,
                          unit_base_inverse<typename U::Current>,
                          unit_base_inverse<typename U::Temperature>,
                          unit_base_inverse<typename U::Amount>,
                          unit_base_inverse<typename U::Luminosity>>;
                          
    };

    template<UnitLike U> 
    using unit_inverse_t = unit_inverse<U>::type;

    template<typename LHS, typename RHS> 
    concept UnitAddable = UnitLike<LHS> && UnitLike<RHS> &&
        UnitBaseAddable<typename LHS::Time, typename RHS::Time> &&
        UnitBaseAddable<typename LHS::Length, typename RHS::Length> &&
        UnitBaseAddable<typename LHS::Mass, typename RHS::Mass> &&
        UnitBaseAddable<typename LHS::Current, typename RHS::Current> &&
        UnitBaseAddable<typename LHS::Temperature, typename RHS::Temperature> &&
        UnitBaseAddable<typename LHS::Amount, typename RHS::Amount> &&
        UnitBaseAddable<typename LHS::Luminosity, typename RHS::Luminosity>;

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

    //Base Units 
    using Second = Unit<UnitBase<0, 1>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using Meter = Unit<NullUnit, UnitBase<0, 1>, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
    using Kilogram = Unit<NullUnit, NullUnit, UnitBase<3, 1>, NullUnit, NullUnit, NullUnit, NullUnit>; 
    using Ampere = Unit<NullUnit, NullUnit, NullUnit, UnitBase<0, 1>, NullUnit, NullUnit, NullUnit>; 
    using Kelvin = Unit<NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>, NullUnit, NullUnit>; 
    using Mole = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>, NullUnit>;
    using Candela = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>>;
    using Dimensionless = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit>;
}