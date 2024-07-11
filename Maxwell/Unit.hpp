#ifndef UNIT_HPP
#define UNIT_HPP

#include "Dimension.hpp"
#include <string>

namespace Maxwell {
template <Dimension auto Amount_, Dimension auto Current_,
          Dimension auto Length_, Dimension auto Luminosity_,
          Dimension auto Mass_, Dimension auto Temperature_,
          Dimension auto Time_, Dimension auto Angle_>
struct UnitType;

namespace _detail {
template <typename> struct is_unit : std::false_type {};

template <typename T> struct is_unit<const T> : is_unit<T> {};

template <Dimension auto Amount_, Dimension auto Current_,
          Dimension auto Length_, Dimension auto Luminosity_,
          Dimension auto Mass_, Dimension auto Temperature_,
          Dimension auto Time_, Dimension auto Angle_>
struct is_unit<UnitType<Amount_, Current_, Length_, Luminosity_, Mass_,
                        Temperature_, Time_, Angle_>> : std::true_type {};
}   // namespace _detail

template <typename T>
concept Unit = _detail::is_unit<T>::value;

template <Dimension auto Amount_, Dimension auto Current_,
          Dimension auto Length_, Dimension auto Luminosity_,
          Dimension auto Mass_, Dimension auto Temperature_,
          Dimension auto Time_, Dimension auto Angle_>
struct UnitType {
    static constexpr Dimension auto Amount = Amount_;
    static constexpr Dimension auto Current = Current_;
    static constexpr Dimension auto Length = Length_;
    static constexpr Dimension auto Luminosity = Luminosity_;
    static constexpr Dimension auto Mass = Mass_;
    static constexpr Dimension auto Temperature = Temperature_;
    static constexpr Dimension auto Time = Time_;
    static constexpr Dimension auto Angle = Angle_;

    auto consteval toCoherentUnit() const noexcept -> Unit auto {
        return UnitType<
            Amount.toCoherentDimension(), Current.toCoherentDimension(),
            Length.toCoherentDimension(), Luminosity.toCoherentDimension(),
            Mass.toCoherentDimension(), Temperature.toCoherentDimension(),
            Time.toCoherentDimension(), Angle.toCoherentDimension()>{};
    }

    auto consteval isCoherentUnit() const noexcept -> bool {
        return Amount.isCoherentDimension() && Current.isCoherentDimension() &&
               Length.isCoherentDimension() &&
               Luminosity.isCoherentDimension() && Mass.isCoherentDimension() &&
               Temperature.isCoherenDimension() && Time.isCoherentDimension() &&
               Angle.isCoherentDimension();
    }

    auto consteval amount() const noexcept -> decltype(Amount) {
        return Amount;
    }

    auto consteval current() const noexcept -> decltype(Current) {
        return Current;
    }

    auto consteval length() const noexcept -> decltype(Length) {
        return Length;
    }

    auto consteval luminosity() const noexcept -> decltype(Luminosity) {
        return Luminosity;
    }

    auto consteval temperature() const noexcept -> decltype(Temperature) {
        return Temperature;
    }

    auto consteval time() const noexcept -> decltype(Time) { return Time; }

    auto consteval angle() const noexcept -> decltype(Angle) { return Angle; }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixAmount() const noexcept -> Unit auto {
        constexpr Dimension auto newAmount =
            Amount.template adjustPrefix<NewPrefix>();
        return UnitType<newAmount, Current, Length, Luminosity, Mass,
                        Temperature, Time, Angle>{};
    }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixCurrent() const noexcept -> Unit auto {
        return UnitType<Amount, Current.template adjustPrefix<NewPrefix>(),
                        Length, Luminosity, Mass, Temperature, Time, Angle>{};
    }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixLength() const noexcept -> Unit auto {
        return UnitType<Amount, Current,
                        Length.template adjustPrefix<NewPrefix>(), Luminosity,
                        Mass, Temperature, Time, Angle>{};
    }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixLuminosity() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length,
                        Luminosity.template adjustPrefix<NewPrefix>(), Mass,
                        Temperature, Time, Angle>{};
    }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixMass() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity,
                        Mass.template adjustPrefix<NewPrefix>(), Temperature,
                        Time, Angle>{};
    }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixTemperature() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity, Mass,
                        Temperature.template adjustPrefix<NewPrefix>(), Time,
                        Angle>{};
    }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixTime() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature,
                        Time.template adjustPrefix<NewPrefix>(), Angle>{};
    }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixAngle() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature,
                        Time, Angle.template adjustPrefix<NewPrefix>()>{};
    }
};

/// --- Query Functions ---
auto consteval
operator==(Unit auto lhs, Unit auto rhs) noexcept -> bool {
    return std::same_as<decltype(lhs), decltype(rhs)>;
}

template <auto From, auto To>
concept AmountConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<decltype(From), decltype(To)>;

template <auto From, auto To>
concept CurrentConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<decltype(From), decltype(To)>;

template <auto From, auto To>
concept LengthConvertibleTo =
    DimensionConvertibleTo<decltype(From), decltype(To)>;

template <auto From, auto To>
concept UnitConvertibleTo = Unit<decltype(From)> && Unit<decltype(To)> &&
                            AmountConvertibleTo<From.amount(), To.amount()> &&
                            CurrentConvertibleTo<From.current(), To.current()>;

template <auto U>
concept Unitless = Unit<decltype(U)>;

/// --- Unit Composition ---
auto consteval
operator*(Unit auto lhs, Unit auto rhs) noexcept -> Unit auto {
    return UnitType<
        lhs.amount() * rhs.amount(), lhs.current() * rhs.current(),
        lhs.length() * rhs.length(), lhs.luminosity() * rhs.luminosity(),
        lhs.mass() * rhs.mass(), lhs.temperature() * rhs.temperature(),
        lhs.time() * rhs.time(), lhs.angle() * rhs.angle()>{};
}

auto consteval
operator/(Unit auto lhs, Unit auto rhs) noexcept -> Unit auto {
    return UnitType<
        lhs.amount() / rhs.amount(), lhs.current() / rhs.current(),
        lhs.length() / rhs.length(), lhs.luminosity() / rhs.luminosity(),
        lhs.mass() / rhs.mass(), lhs.temperature() / rhs.temperature(),
        lhs.time() / rhs.time(), lhs.angle() / rhs.angle()>{};
}

/// --- Unit Formatting ---
template <Unit auto U> struct UnitFormat {
    static constexpr bool IsSpecialized = false;

    auto constexpr static unitString() -> std::string { return ""; }
};
}   // namespace Maxwell

#endif