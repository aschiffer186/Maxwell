/**
 * @file UnitBase.hh
 * @author Alex Schiffer
 * @brief Definition of the Unit type and operations on it.
 * @version 0.1
 * @date 2024-03-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef UNIT_BASE_HH
#define UNIT_BASE_HH

#include <array>
#include <concepts>
#include <ratio>

namespace Maxwell
{
    /// One SI Base Unit
    /// UnitBase = ScaleNum/ScaleDen * 10^Prefix * SI-Unit^Pow +
    /// OffsetNum/OffsetDen
    template <int Pow_, int Prefix_, int OffsetNum_, int OffsetDen_,
              int ScaleNum_, int ScaleDen_>
    struct UnitBase
    {
        static constexpr int Power     = Pow_;
        static constexpr int Prefix    = Prefix_;
        static constexpr int OffsetNum = OffsetNum_;
        static constexpr int OffsetDen = OffsetDen_;
        static constexpr int ScaleNum  = ScaleNum_;
        static constexpr int ScaleDen  = ScaleDen_;

        constexpr auto power() const noexcept -> int { return Power; }
        constexpr auto prefix() const noexcept -> int { return Prefix; }

        constexpr auto
        offset() const noexcept -> std::ratio<OffsetNum, OffsetDen>
        {
            return std::ratio<OffsetNum, OffsetDen>{};
        }

        constexpr auto offsetNum() const noexcept -> int { return OffsetNum; }

        constexpr auto offsetDen() const noexcept -> int { return OffsetDen; }

        constexpr auto scale() const noexcept -> std::ratio<ScaleNum, ScaleDen>
        {
            return std::ratio<ScaleNum, ScaleDen>{};
        }

        constexpr auto scaleNum() const noexcept -> int { return ScaleNum; }

        constexpr auto scaleDen() const noexcept -> int { return ScaleDen; }
    };

    // Constants
    constexpr inline UnitBase<0, 0, 1, 1, 1, 1> NullUnitBase;
    constexpr inline UnitBase<1, 0, 1, 1, 1, 1> CoherentUnitBase;

    template <typename>
    struct is_unit_base : std::false_type
    {
    };

    template <typename Tp>
    struct is_unit_base<const Tp> : is_unit_base<Tp>
    {
    };

    template <int Pow, int Prefix, int OffsetNum, int OffsetDen, int ScaleNum,
              int ScaleDen>
    struct is_unit_base<
        UnitBase<Pow, Prefix, OffsetNum, OffsetDen, ScaleNum, ScaleDen>>
        : std::true_type
    {
    };

    template <typename Tp>
    concept UnitBaseLike = is_unit_base<Tp>::value;

    constexpr auto isCoherentUnitBase(UnitBaseLike auto ub) noexcept -> bool
    {
        return ub.prefix() == 0;
    }

    consteval auto toCoherentUnitBase(UnitBaseLike auto ub) noexcept -> auto
    {
        return UnitBase<ub.power(), 0, ub.offsetNum(), ub.offsetDen(),
                        ub.scaleNum(), ub.scaleDen()>{};
    }

    constexpr auto operator==(UnitBaseLike auto lhs,
                              UnitBaseLike auto rhs) noexcept -> bool
    {
        return lhs.power() == rhs.power() && lhs.prefix() == rhs.prefix() &&
               std::ratio_equal_v<decltype(lhs.offset()),
                                  decltype(rhs.offset())> &&
               std::ratio_equal_v<decltype(lhs.scale()), decltype(rhs.scale())>;
    }

    constexpr auto operator*(UnitBaseLike auto lhs,
                             UnitBaseLike auto rhs) noexcept -> auto
    {
        return UnitBase<lhs.power() + rhs.power(), lhs.power(), lhs.offsetNum(),
                        lhs.offsetDen(), lhs.scaleNum(), lhs.scaleDen()>{};
    }

    constexpr auto operator/(UnitBaseLike auto lhs,
                             UnitBaseLike auto rhs) noexcept -> auto
    {
        return UnitBase<lhs.power() - rhs.power(), lhs.power(), lhs.offsetNum(),
                        lhs.offsetDen(), lhs.scaleNum(), lhs.scaleDen()>{};
    }

    /**
     * @brief Calculates the inverse of a UnitBase
     *
     * Calculates the inverse of a UnitBase.
     *
     * @post unitBaseInverse(unit).power() == -unit.power()
     *
     * @param unit the UnitBase to find the inverse of
     * @return the inverse of the UnitBase
     */
    consteval auto unitBaseInverse(UnitBaseLike auto unit) noexcept -> auto
    {
        return UnitBase<-unit.power(), unit.prefix(), unit.offsetNum(),
                        unit.offsetDen(), unit.scaleNum(), unit.scaleDen()>{};
    }

    consteval auto
    unitBaseScaleConversion(UnitBaseLike auto from,
                            UnitBaseLike auto to) noexcept -> double
    {
        return static_cast<double>(to.scaleNum()) /
               static_cast<double>(to.scaleDen());
    }

    /**
     * @brief Changes the prefix of the unit base
     *
     * Changes the prefix of the unit base by the specified amount.
     *
     * @post scaleUnitBase<Amount>(unitBase).prefix() -
     *       unitBase.prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unitBase the UnitBase to change
     * @return the changed UnitBase
     */
    template <int Amount>
    consteval auto
    adjustUnitBasePrefix(UnitBaseLike auto unitBase) noexcept -> auto
    {
        return UnitBase<unitBase.power(), unitBase.prefix() + Amount,
                        unitBase.offsetNum(), unitBase.offsetDen(),
                        unitBase.scaleNum(), unitBase.scaleDen()>{};
    }

    template <int Num, int Den>
    consteval auto
    adjustUnitBaseScale(UnitBaseLike auto unitBase) noexcept -> auto
    {
        return UnitBase<unitBase.power(), unitBase.prefix(),
                        unitBase.offsetNum(), unitBase.offsetDen(),
                        unitBase.scaleNum() * Num, unitBase.scaleDen() * Den>{};
    }

    /**
     * @brief Concept for conversion between unit bases
     *
     * Concept modeling the ability to convert from a unit base From to a unit
     * To. A unit base From can be converted unit To if there exists some
     * constant C such C*From == To
     *
     * @tparam FromUnitBase the starting unit base
     * @tparam ToUnitBase the goal unit base
     */
    template <typename FromUnitBase, typename ToUnitBase>
    concept UnitBaseConvertibleTo =
        UnitBaseLike<FromUnitBase> && UnitBaseLike<ToUnitBase> &&
        requires(FromUnitBase, ToUnitBase) {
            FromUnitBase::Power == ToUnitBase::Power;
        };

    template <UnitBaseLike auto AmountType, UnitBaseLike auto CurrentType,
              UnitBaseLike auto LengthType, UnitBaseLike auto LuminosityType,
              UnitBaseLike auto MassType, UnitBaseLike auto TemperatureType,
              UnitBaseLike auto TimeType,
              UnitBaseLike auto AngleType = NullUnitBase>
    struct Unit
    {
        static constexpr UnitBase Amount      = AmountType;
        static constexpr UnitBase Current     = CurrentType;
        static constexpr UnitBase Length      = LengthType;
        static constexpr UnitBase Luminosity  = LuminosityType;
        static constexpr UnitBase Mass        = MassType;
        static constexpr UnitBase Temperature = TemperatureType;
        static constexpr UnitBase Time        = TimeType;
        static constexpr UnitBase Angle       = AngleType;

        constexpr auto amount() const noexcept -> auto { return Amount; }

        constexpr auto current() const noexcept -> auto { return Current; }

        constexpr auto length() const noexcept -> auto { return Length; }

        constexpr auto luminosity() const noexcept -> auto
        {
            return Luminosity;
        }

        constexpr auto mass() const noexcept -> auto { return Mass; }

        constexpr auto temperature() const noexcept -> auto
        {
            return Temperature;
        }

        constexpr auto time() const noexcept -> auto { return Time; }

        constexpr auto angle() const noexcept -> auto { return Angle; }
    };

    /**
     * @brief Type trait to indicate a type is a unit
     *
     * Primary specialization is false
     *
     */
    template <typename>
    struct is_unit : std::false_type
    {
    };

    /**
     * @brief Type trait to indicate a type is a unit
     *
     * Specialization for const-qualified types:
     * the result is the same as the non-const qualified
     * type.
     *
     */
    template <typename Tp>
    struct is_unit<const Tp> : is_unit<Tp>
    {
    };

    /**
     * @brief Type trait to indicate a type is a unit
     *
     * Specialization for instantiations of the Unit type
     * the trait is true for these types.
     *
     */
    template <UnitBaseLike auto AmountType, UnitBaseLike auto CurrentType,
              UnitBaseLike auto LengthType, UnitBaseLike auto LuminosityType,
              UnitBaseLike auto MassType, UnitBaseLike auto TemperatureType,
              UnitBaseLike auto TimeType, UnitBaseLike auto AngleType>
    struct is_unit<Unit<AmountType, CurrentType, LengthType, LuminosityType,
                        MassType, TemperatureType, TimeType, AngleType>>
        : std::true_type
    {
    };

    template <typename Tp>
    concept UnitLike = is_unit<Tp>::value;

    constexpr auto isCoherentUnit(UnitLike auto unit) noexcept -> bool
    {
        return isCoherentUnitBase(unit.amount()) &&
               isCoherentUnitBase(unit.current()) &&
               isCoherentUnitBase(unit.length()) &&
               isCoherentUnitBase(unit.luminosity()) &&
               isCoherentUnitBase(unit.mass()) &&
               isCoherentUnitBase(unit.temperature()) &&
               isCoherentUnitBase(unit.time()) &&
               isCoherentUnitBase(unit.angle());
    }

    consteval auto toCoherentUnit(UnitLike auto unit) noexcept -> auto
    {
        return Unit<toCoherentUnitBase(unit.amount()),
                    toCoherentUnitBase(unit.current()),
                    toCoherentUnitBase(unit.length()),
                    toCoherentUnitBase(unit.luminosity()),
                    toCoherentUnitBase(unit.mass()),
                    toCoherentUnitBase(unit.temperature()),
                    toCoherentUnitBase(unit.time()),
                    toCoherentUnitBase(unit.angle())>{};
    }

    constexpr auto operator==(UnitLike auto lhs,
                              UnitLike auto rhs) noexcept -> bool
    {
        return lhs.amount() == rhs.amount() && lhs.current() == rhs.current() &&
               lhs.length() == rhs.length() &&
               lhs.luminosity() == rhs.luminosity() &&
               lhs.mass() == rhs.mass() &&
               lhs.temperature() == rhs.temperature() &&
               lhs.time() == rhs.time() && lhs.angle() == rhs.angle();
    }

    constexpr auto operator*(UnitLike auto lhs,
                             UnitLike auto rhs) noexcept -> auto
    {
        return Unit<
            lhs.amount() * rhs.amount(), lhs.current() * rhs.current(),
            lhs.length() * rhs.length(), lhs.luminosity() * rhs.luminosity(),
            lhs.mass() * rhs.mass(), lhs.temperature() * rhs.temperature(),
            lhs.time() * rhs.time(), lhs.angle() * rhs.angle()>{};
    }

    constexpr auto operator/(UnitLike auto lhs,
                             UnitLike auto rhs) noexcept -> auto
    {
        return Unit<
            lhs.amount() / rhs.amount(), lhs.current() / rhs.current(),
            lhs.length() / rhs.length(), lhs.luminosity() / rhs.luminosity(),
            lhs.mass() / rhs.mass(), lhs.temperature() / rhs.temperature(),
            lhs.time() / rhs.time(), lhs.angle() / rhs.angle()>{};
    }

    /**
     * @brief Computes the inverse of a unit
     *
     * Computes the inverse of a unit. The inverse of a unit is the product
     * of the inverse of each of the SI base units making up the unit.
     *
     * @param unit the unit to invert
     * @return the inverse of the unit
     */
    consteval auto unitInverse(UnitLike auto unit) noexcept -> auto
    {
        return Unit<
            unitBaseInverse(unit.amount()), unitBaseInverse(unit.current()),
            unitBaseInverse(unit.length()), unitBaseInverse(unit.luminosity()),
            unitBaseInverse(unit.mass()), unitBaseInverse(unit.temperature()),
            unitBaseInverse(unit.time()), unitBaseInverse(unit.angle())>{};
    }

    /**
     * @brief Concept for conversion between units
     *
     * Concept modeling the ability to convert from a unit From to a unit To.
     * A unit From can be converted unit To if there exists some constant
     * C such C*From == To
     *
     * @tparam FromUnit the starting unit
     * @tparam ToUnit the goal unit
     */
    template <typename FromUnit, typename ToUnit>
    concept UnitConvertibleTo =
        UnitLike<FromUnit> && UnitLike<ToUnit> &&
        FromUnit::Amount.power() == ToUnit::Amount.power() &&
        FromUnit::Current.power() == ToUnit::Current.power() &&
        FromUnit::Length.power() == ToUnit::Length.power() &&
        FromUnit::Luminosity.power() == ToUnit::Luminosity.power() &&
        FromUnit::Mass.power() == ToUnit::Mass.power() &&
        FromUnit::Temperature.power() == ToUnit::Temperature.power() &&
        FromUnit::Time.power() == ToUnit::Time.power() &&
        FromUnit::Angle.power() == ToUnit::Angle.power();

    /**
     * @brief Concept indicating that a unit is unitless
     *
     * The Unitless concept is true if a unit can be implicitly
     * converted to a scalar value. This is true if and only if
     * the power of all the components of the unit are 0.
     *
     * @tparam Unit the unit to check
     */
    template <typename Unit>
    concept Unitless =
        UnitLike<Unit> && Unit::Amount.power() == 0 &&
        Unit::Current.power() == 0 && Unit::Length.power() == 0 &&
        Unit::Luminosity.power() == 0 && Unit::Mass.power() == 0 &&
        Unit::Temperature.power() == 0 && Unit::Time.power() == 0 &&
        Unit::Angle.power() == 0;

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the amount prefix of the unit by the
     * specified amount.
     *
     * @post scaleAmount<Amount>(unit).amount().prefix -
     *       unit.amount().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixAmount(UnitLike auto unit) noexcept -> auto
    {
        return Unit<adjustUnitBasePrefix<Amount>(unit.amount()), unit.current(),
                    unit.length(), unit.luminosity(), unit.mass(),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the current prefix of the unit by the
     * specified amount.
     *
     * @post scaleCurrent<Amount>(unit).current().prefix -
     *       unit.current().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixCurrent(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), adjustUnitBasePrefix<Amount>(unit.current()),
                    unit.length(), unit.luminosity(), unit.mass(),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the length prefix of the unit by the
     * specified amount.
     *
     * @post scaleLength<Amount>(unit).length().prefix -
     *       unit.length().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixLength(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(),
                    adjustUnitBasePrefix<Amount>(unit.length()),
                    unit.luminosity(), unit.mass(), unit.temperature(),
                    unit.time(), unit.angle()>{};
    }

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the luminosity prefix of the unit by the
     * specified amount.
     *
     * @post scaleLuminosity<Amount>(unit).luminosity().prefix -
     *       unit.luminosity().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixLuminosity(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    adjustUnitBasePrefix<Amount>(unit.luminosity()),
                    unit.mass(), unit.temperature(), unit.time(),
                    unit.angle()>{};
    }

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the mass prefix of the unit by the
     * specified amount.
     *
     * @post scaleMass<Amount>(unit).mass().prefix -
     *       unit.mass().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixMass(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(),
                    adjustUnitBasePrefix<Amount>(unit.mass()),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the temperature prefix of the unit by the
     * specified amount.
     *
     * @post scaleTemperature<Amount>(unit).temperature().prefix -
     *       unit.temperature().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixTemperature(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(), unit.mass(),
                    adjustUnitBasePrefix<Amount>(unit.temperature()),
                    unit.time(), unit.angle()>{};
    }

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the time prefix of the unit by the
     * specified amount.
     *
     * @post scaleTime<Amount>(unit).time().prefix -
     *       unit.time().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixTime(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(), unit.mass(), unit.temperature(),
                    adjustUnitBasePrefix<Amount>(unit.time()), unit.angle()>{};
    }

    /**
     * @brief Changes the prefix of the unit
     *
     * Changes the angle prefix of the unit by the
     * specified amount.
     *
     * @post scaleAngle<Amount>(unit).angle().prefix -
     *       unit.angle().prefix() == Amount
     *
     * @tparam Amount the amount to change the prefix by
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Amount>
    consteval auto adjustPrefixAngle(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(), unit.mass(), unit.temperature(),
                    unit.time(), adjustUnitBasePrefix<Amount>(unit.angle())>{};
    }

    /**
     * @brief Changes the scale of the unit
     *
     * Changes the length scale of the unit by the
     * specified amount.
     *
     * @post adjustScaleTime<Num, Den>(unit) == Num/Den*unit.length()
     *
     * @tparam Num the numerator of the new scale
     * @tparam Den the denominator of the new scale
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Num, int Den>
    consteval auto adjustScaleLength(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(),
                    adjustUnitBaseScale<Num, Den>(unit.length()),
                    unit.luminosity(), unit.mass(), unit.temperature(),
                    unit.time(), unit.angle()>{};
    }

    /**
     * @brief Changes the scale of the unit
     *
     * Changes the length scale of the unit by the
     * specified amount.
     *
     * @post adjustScaleTime<Num, Den>(unit) == Num/Den*unit.length()
     *
     * @tparam Num the numerator of the new scale
     * @tparam Den the denominator of the new scale
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Num, int Den>
    consteval auto adjustScaleMass(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(),
                    adjustUnitBaseScale<Num, Den>(unit.mass()),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    /**
     * @brief Changes the scale of the unit
     *
     * Changes the time scale of the unit by the
     * specified amount.
     *
     * @post adjustScaleTime<Num, Den>(unit) == Num/Den*unit.time()
     *
     * @tparam Num the numerator of the new scale
     * @tparam Den the denominator of the new scale
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Num, int Den>
    consteval auto adjustScaleTime(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(), unit.mass(), unit.temperature(),
                    adjustUnitBaseScale<Num, Den>(unit.time()), unit.angle()>{};
    }

    /**
     * @brief Changes the scale of the unit
     *
     * Changes the angle scale of the unit by the
     * specified amount.
     *
     * @post adjustScaleAngle<Num, Den>(unit) == Num/Den*unit.angle()
     *
     * @tparam Num the numerator of the new scale
     * @tparam Den the denominator of the new scale
     * @param unit the unit to change
     * @return the changed unit
     */
    template <int Num, int Den>
    consteval auto adjustScaleAngle(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(), unit.mass(), unit.temperature(),
                    unit.time(), adjustUnitBaseScale<Num, Den>(unit.angle())>{};
    }

    /// Cached powers of 10
    inline constexpr std::array powersOf10{
        1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22,
        1e-21, 1e-20, 1e-19, 1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13,
        1e-12, 1e-11, 1e-10, 1e-9,  1e-8,  1e-7,  1e-6,  1e-5,  1e-4,
        1e-3,  1e-2,  1e-1,  1e0,   1e1,   1e2,   1e3,   1e4,   1e5,
        1e6,   1e7,   1e8,   1e9,   1e10,  1e11,  1e12,  1e13,  1e14,
        1e15,  1e16,  1e17,  1e18,  1e19,  1e20,  1e21,  1e22,  1e23,
        1e24,  1e25,  1e26,  1e27,  1e28,  1e29,  1e30};

    /**
     * @brief Calculates powers of 10
     *
     * Calculates 10^exp
     *
     * @param exp the exponent
     * @return 10^exp
     */
    consteval auto pow10(int exp) noexcept -> double
    {
        if (exp >= -30 && exp <= 30)
        {
            return powersOf10[exp + 30];
        }
        return 0;
    }

    /**
     * @brief Calculate conversion factor due to differences in prefix
     *
     * Calculate the multiplication factor used to multiply the value of
     * a quantity going from one unit to another due to differences in
     * their prefixes. Equal to 10^(from.prefix() - to.prefix())
     *
     * @param from the starting unit
     * @param to to target unit
     * @return the conversion factor
     */
    consteval auto unitPrefixConversion(UnitLike auto from,
                                        UnitLike auto to) noexcept -> double
        requires UnitConvertibleTo<decltype(from), decltype(to)>
    {
        int delta  = from.amount().prefix() - to.amount().prefix();
        delta     += from.current().prefix() - to.current().prefix();
        delta     += from.length().prefix() - to.length().prefix();
        delta     += from.luminosity().prefix() - to.luminosity().prefix();
        delta     += from.mass().prefix() - to.mass().prefix();
        delta     += from.temperature().prefix() - to.temperature().prefix();
        delta     += from.time().prefix() - to.time().prefix();
        delta     += from.angle().prefix() - to.angle().prefix();
        return pow10(delta);
    }

    /**
     * @brief Calculate conversion factor due to differences in scale
     *
     *
     * Calculate the multiplication factor used the multiply the value of a
     * quantity going from one unit to another due to their differences in
     * scale factors. Equal to from.scaleDen()/from.scaleNum()
     *
     * @param from the starting unit
     * @param to the target unit
     * @return the conversion factor
     */
    consteval auto unitScaleConversion(UnitLike auto from,
                                       UnitLike auto to) noexcept -> double
        requires UnitConvertibleTo<decltype(from), decltype(to)>
    {
        double mult  = unitBaseScaleConversion(from.amount(), to.amount());
        mult        *= unitBaseConversion(from.current(), to.current());
        mult        *= unitBaseConversion(from.length(), to.length());
        mult        *= unitBaseConversion(from.luminosity(), to.luminosity());
        mult        *= unitBaseConversion(from.mass(), to.mass());
        mult        *= unitBaseConversion(from.temperature(), to.temperature());
        mult        *= unitBaseConversion(from.time(), to.time());
        mult        *= unitBaseConversion(from.angle(), to.angle());
        return mult;
    }
} // namespace Maxwell

#endif