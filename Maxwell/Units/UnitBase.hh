/**
 * @file UnitBase.hh
 * @author Alex Schiffer
 * @brief
 * @version 0.1
 * @date 2024-02-29
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
    // One SI Base Unit
    // UnitBase = ScaleNum/ScaleDen * 10^Prefix * Unit^Pow + OffsetNum/OffsetDen
    template <int Pow_, int Prefix_, int OffsetNum_ = 1, int OffsetDen_ = 1,
              int ScaleNum_ = 1, int ScaleDen_ = 1>
    struct UnitBase
    {
        static constexpr int Pow       = Pow_;
        static constexpr int Prefix    = Prefix_;
        static constexpr int OffsetNum = OffsetNum_;
        static constexpr int OffsetDen = OffsetDen_;
        static constexpr int ScaleNum  = ScaleNum_;
        static constexpr int ScaleDen  = ScaleDen_;

        constexpr auto power() const noexcept -> int { return Pow; }
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
    constexpr inline UnitBase<0, 1, 1, 1, 1, 1> NullUnitBase{};
    constexpr inline UnitBase<1, 1, 1, 1, 1, 1> CoherentUnitBase;

    template <typename>
    struct is_unit_base : std::false_type
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

    template <int Amount>
    constexpr auto scaleUnitBase(UnitBaseLike auto unitBase) noexcept -> auto
    {
        return UnitBase<unitBase.power(), unitBase.prefix() + Amount,
                        unitBase.offsetNum(), unitBase.offsetDen(),
                        unitBase.scaleNum(), unitBase.scaleDen()>{};
    }

    // Concept defining when it's possible
    template <typename FromUnitBase, typename ToUnitBase>
    concept UnitBaseConvertibleTo =
        UnitBaseLike<FromUnitBase> && UnitBaseLike<ToUnitBase> &&
        requires(FromUnitBase, ToUnitBase) {
            FromUnitBase::pow == ToUnitBase::pow;
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

    template <typename>
    struct is_unit : std::false_type
    {
    };

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

    template <typename FromUnit, typename ToUnit>
    concept UnitConvertibleTo =
        UnitLike<FromUnit> && UnitLike<ToUnit> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Amount),
                              decltype(ToUnit::Amount)> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Current),
                              decltype(ToUnit::Current)> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Length),
                              decltype(ToUnit::Length)> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Luminosity),
                              decltype(ToUnit::Luminosity)> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Mass),
                              decltype(ToUnit::Mass)> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Temperature),
                              decltype(ToUnit::Temperature)> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Time),
                              decltype(ToUnit::Time)> &&
        UnitBaseConvertibleTo<decltype(FromUnit::Angle),
                              decltype(ToUnit::Angle)>;

    template <typename Unit>
    concept Unitless =
        UnitLike<Unit> && Unit::Amount.power() == 0 &&
        Unit::Current.power() == 0 && Unit::Length.power() == 0 &&
        Unit::Luminosity.power() == 0 && Unit::Mass.power() == 0 &&
        Unit::Temperature.power() == 0 && Unit::Time.power() == 0 &&
        Unit::Angle.power() == 0;

    template <int Amount>
    constexpr auto scaleAmount(UnitLike auto unit) noexcept -> auto
    {
        return Unit<scaleUnitBase<Amount>(unit.amount()), unit.current(),
                    unit.length(), unit.luminosity(), unit.mass(),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    template <int Amount>
    constexpr auto scaleCurrent(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), scaleUnitBase<Amount>(unit.current()),
                    unit.length(), unit.luminosity(), unit.mass(),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    template <int Amount>
    constexpr auto scaleLength(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(),
                    scaleUnitBase<Amount>(unit.length()), unit.luminosity(),
                    unit.mass(), unit.temperature(), unit.time(),
                    unit.angle()>{};
    }

    template <int Amount>
    constexpr auto scaleLuminosity(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    scaleUnitBase<Amount>(unit.luminosity()), unit.mass(),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    template <int Amount>
    constexpr auto scaleMass(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(), scaleUnitBase<Amount>(unit.mass()),
                    unit.temperature(), unit.time(), unit.angle()>{};
    }

    template <int Amount>
    constexpr auto scaleTemperature(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(), unit.length(),
                    unit.luminosity(), unit.mass(),
                    scaleUnitBase<Amount>(unit.temperature()), unit.time(),
                    unit.angle()>{};
    }

    template <int Amount>
    constexpr auto scaleTime(UnitLike auto unit) noexcept -> auto
    {
        return Unit<unit.amount(), unit.current(),
                    scaleUnitBase<Amount>(unit.length()), unit.luminosity(),
                    unit.mass(), unit.temperature(),
                    scaleUnitBase<Amount>(unit.time()), unit.angle()>{};
    }

    // Cached powers of 10
    inline constexpr std::array powersOf10{
        1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22,
        1e-21, 1e-20, 1e-19, 1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13,
        1e-12, 1e-11, 1e-10, 1e-9,  1e-8,  1e-7,  1e-6,  1e-5,  1e-4,
        1e-3,  1e-2,  1e-1,  1e0,   1e1,   1e2,   1e3,   1e4,   1e5,
        1e6,   1e7,   1e8,   1e9,   1e10,  1e11,  1e12,  1e13,  1e14,
        1e15,  1e16,  1e17,  1e18,  1e19,  1e20,  1e21,  1e22,  1e23,
        1e24,  1e25,  1e26,  1e27,  1e28,  1e29,  1e30};

    constexpr auto pow10(int exp) noexcept -> double
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
     * Calculate the multipication factor used to multiplythe value of
     * a quantity going from one unit to another due to differences in
     * their prefixes. Equal to 10^(from.prefix() - to.prefix())
     *
     * @param from the starting unit
     * @param to to target unit
     * @return the convesion factor
     */
    consteval auto unitPrefixConversion(UnitLike auto from,
                                        UnitLike auto to) noexcept -> double
    {
        const int delta = from.prefix() - to.prefix();
        return pow10(delta);
    }
} // namespace Maxwell

#endif