#ifndef UNIT_HPP
#define UNIT_HPP

#include "Dimension.hpp"
#include <array>
#include <ratio>
#include <string>

/// @file Unit.hpp Contains the definition of UnitType

namespace Maxwell {
/**
 * @brief Helper type to create unique units
 *
 * UnitTagType is a helper type that can be used to create unique units
 * who have the same dimensionality (e.g. Hertz and Bercquel). By supplying
 * unique values of UnitTagType, two different units can be created. Units
 * with different tags are not convertible.
 *
 * Example usage:
 *  constexpr auto SteradianUnit = RadianUnit.addTag<UnitTagType{1}>();
 *  bool b = SteradianUnt = RadianUnit; //false
 *  bool b2 = UnitConvertibleTo<SteradianUnit, RadianUnit> // false
 */
struct UnitTagType {
    int i{};

    friend auto constexpr operator==(UnitTagType,
                                     UnitTagType) noexcept -> bool = default;
};

/// Constant for when the tag type isn't needed
constexpr UnitTagType NullTag{0};

// --- Forward Declaration ---

/// @tparam Amount_ the amount dimension of the unit
/// @tparam Current_ the current dimension of the unit
/// @tparam Length_ the length dimension of the unit
/// @tparam Luminosity_ the luminosity dimension of the unit
/// @tparam Mass_ the mass dimension of the unit
/// @tparam Temperature_ the temperature dimension of the unit
/// @tparam Time_ the time dimension of the unit
/// @tparam Angle_ the angle "dimension" of the unit
/// @tparam Tag_ a tag for creating unique units with the same dimension
template <Dimension auto Amount_, Dimension auto Current_,
          Dimension auto Length_, Dimension auto Luminosity_,
          Dimension auto Mass_, Dimension auto Temperature_,
          Dimension auto Time_, Dimension auto Angle_, UnitTagType = NullTag>
struct UnitType;

namespace _detail {
template <typename> struct is_unit : std::false_type {};

template <typename T> struct is_unit<const T> : is_unit<T> {};

template <Dimension auto Amount_, Dimension auto Current_,
          Dimension auto Length_, Dimension auto Luminosity_,
          Dimension auto Mass_, Dimension auto Temperature_,
          Dimension auto Time_, Dimension auto Angle_, UnitTagType Tag>
struct is_unit<UnitType<Amount_, Current_, Length_, Luminosity_, Mass_,
                        Temperature_, Time_, Angle_, Tag>> : std::true_type {};
}   // namespace _detail

/// @brief Concept modeling types that are instantiations of UnitType
template <typename T>
concept Unit = _detail::is_unit<T>::value;

/// --- Definition of Unit ---
template <Dimension auto Amount_, Dimension auto Current_,
          Dimension auto Length_, Dimension auto Luminosity_,
          Dimension auto Mass_, Dimension auto Temperature_,
          Dimension auto Time_, Dimension auto Angle_, UnitTagType Tag_>
struct UnitType {
    /// The amount dimension of the unit
    static constexpr Dimension auto Amount = Amount_;
    /// The current dimension of the unit
    static constexpr Dimension auto Current = Current_;
    /// The length dimension of the unit
    static constexpr Dimension auto Length = Length_;
    /// The luminosity dimension of the unit
    static constexpr Dimension auto Luminosity = Luminosity_;
    /// The mass dimension of the unit
    static constexpr Dimension auto Mass = Mass_;
    /// The temperature dimension of the unit
    static constexpr Dimension auto Temperature = Temperature_;
    //// The time dimension of the unit
    static constexpr Dimension auto Time = Time_;
    //// The angle "dimension" of the unit
    static constexpr Dimension auto Angle = Angle_;
    /// The unit's tag
    static constexpr UnitTagType Tag = Tag_;

    /// @brief Returns the amount dimension of the unit
    ///
    /// Returns the amount dimension of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the amount dimension of the unit
    auto consteval amount() const noexcept -> decltype(Amount) {
        return Amount;
    }

    /// @brief Returns the current dimension of the unit
    ///
    /// Returns the current dimension of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the current dimension of the unit
    auto consteval current() const noexcept -> decltype(Current) {
        return Current;
    }

    //// @brief Returns the length dimension of the unit
    ///
    /// Returns the length dimension of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the length dimension of the unit
    auto consteval length() const noexcept -> decltype(Length) {
        return Length;
    }

    /// @brief Returns the luminosity dimension of the unit
    ///
    /// Returns the luminosity dimension of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the luminosity dimension of the unit
    auto consteval luminosity() const noexcept -> decltype(Luminosity) {
        return Luminosity;
    }

    /// @brief Returns the mass dimension of the unit
    ///
    /// Returns the mass dimension of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the mass dimension of the unit
    auto consteval mass() const noexcept -> decltype(Mass) { return Mass; }

    /// @brief Returns the temperature dimension of the unit
    ///
    /// Returns the temperature dimension of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the temperature dimension of the unit
    auto consteval temperature() const noexcept -> decltype(Temperature) {
        return Temperature;
    }

    /// @brief Returns the time dimension of the unit
    ///
    /// Returns the time dimension of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the time dimension of the unit
    auto consteval time() const noexcept -> decltype(Time) { return Time; }

    /// @brief Returns the angle "dimension" of the unit
    ///
    /// Returns the angle "dimension" of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the angle "dimension" of the unit
    auto consteval angle() const noexcept -> decltype(Angle) { return Angle; }

    ///@brief Adds a tag to the unit
    ///
    /// Adds a tag to the unit. This is useful for creating
    /// two separate units that have te same dimensions (e.g. radians and
    /// steradians). The new unit will have the same dimensions as the current
    /// unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @post the dimensions of *this and addTag() are the same
    ///
    /// @tparam NewTag the tag of the new unit
    ///
    ///@return a new unit with the specified tag.
    template <UnitTagType NewTag>
    auto consteval addTag() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature,
                        Time, Angle, NewTag>{};
    }

    ///@brief Returns the tag of the unit
    ///
    /// Returns the tag of the unit. This function
    /// is always usable in a constant expression even if *this
    /// isn't a compile-time constant.
    ///
    /// @return the tag of the unit
    auto consteval tag() const noexcept -> UnitTagType { return Tag; }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefixAmount() const noexcept -> Unit auto {
        return UnitType<Amount.template adjustPrefix<NewPrefix>(), Current,
                        Length, Luminosity, Mass, Temperature, Time, Angle>{};
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

    template <_detail::Ratio NewScaleFactor>
    auto consteval adjustScaleLength() const noexcept -> Unit auto {
        return UnitType<Amount, Current,
                        Length.template adjustScale<NewScaleFactor>(),
                        Luminosity, Mass, Temperature, Time, Angle>{};
    }

    template <_detail::Ratio NewScaleFactor>
    auto consteval adjustScaleMass() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity,
                        Mass.template adjustScale<NewScaleFactor>(),
                        Temperature, Time, Angle>{};
    }

    template <_detail::Ratio NewScaleFactor>
    auto consteval adjustScaleTime() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature,
                        Time.template adjustScale<NewScaleFactor>(), Angle>{};
    }

    template <_detail::Ratio NewScaleFactor>
    auto consteval adjustScaleAngle() const noexcept -> Unit auto {
        return UnitType<Amount, Current, Length, Luminosity, Mass, Temperature,
                        Time, Angle.template adjustScale<NewScaleFactor>()>{};
    }

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
               Temperature.isCoherentDimension() &&
               Time.isCoherentDimension() && Angle.isCoherentDimension();
    }
};

/// --- Query Functions ---

/// @brief Returns true if two units are equal
///
/// Returns true if two units are equal. Two units
/// are equal if they have the same dimensions and
/// the same tag. This implies two units are equal
/// if and only if they are the same type.
///
/// @param lhs one unit to compare
/// @param rhs the other unit to compare
///
/// @return true if two units are equal.
auto constexpr
operator==(Unit auto lhs, Unit auto rhs) noexcept -> bool {
    return std::same_as<decltype(lhs), decltype(rhs)>;
}

template <auto From, auto To>
concept AmountConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept CurrentConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept LengthConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept LuminosityConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept MassConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept TemperatureConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept TimeConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept AngleConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    DimensionConvertibleTo<From, To>;

template <auto From, auto To>
concept UnitConvertibleTo =
    Unit<decltype(From)> && Unit<decltype(To)> &&
    AmountConvertibleTo<From.amount(), To.amount()> &&
    CurrentConvertibleTo<From.current(), To.current()> &&
    LengthConvertibleTo<From.length(), To.length()> &&
    LuminosityConvertibleTo<From.luminosity(), To.luminosity()> &&
    MassConvertibleTo<From.mass(), To.mass()> &&
    TemperatureConvertibleTo<From.temperature(), To.temperature()> &&
    TimeConvertibleTo<From.time(), To.time()> &&
    AngleConvertibleTo<From.angle(), To.angle()> && From.tag() == To.tag();

auto constexpr isConvertibleTo(Unit auto from, Unit auto to) noexcept -> bool {
    return UnitConvertibleTo<from, to>;
}

template <auto U>
concept Unitless =
    Unit<decltype(U)> && U.amount() == NullDimension &&
    U.current() == NullDimension && U.length() == NullDimension &&
    U.luminosity() == NullDimension && U.mass() == NullDimension &&
    U.temperature() == NullDimension && U.time() == NullDimension &&
    U.angle() == NullDimension;

auto constexpr isUnitless(Unit auto u) noexcept -> bool {
    return Unitless<decltype(u){}>;
}

/// --- Unit Conversion ---
namespace _detail {
constexpr std::array pow10Arr{
    1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22, 1e-21, 1e-20,
    1e-19, 1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10, 1e-9,
    1e-8,  1e-7,  1e-6,  1e-5,  1e-4,  1e-3,  1e-2,  1e-1,  1.0,   1e1,   1e2,
    1e3,   1e4,   1e5,   1e6,   1e7,   1e8,   1e9,   1e10,  1e11,  1e12,  1e13,
    1e14,  1e15,  1e16,  1e17,  1e18,  1e19,  1e20,  1e21,  1e22,  1e23,  1e24,
    1e25,  1e26,  1e27,  1e28,  1e29,  1e30};

auto consteval pow10(int exp) noexcept -> double {
    if (exp >= -30 && exp <= 30) {
        return pow10Arr[exp + 30];
    }
    if (exp < 0) {
        return 1.0 / pow10(-exp);
    }
    std::uintmax_t res{};
    constexpr std::uintmax_t N{1000000007};
    int base{10};
    while (exp > 0) {
        if (exp % 2 != 0) {
            res = (res * base) % N;
        }
        base = (base * base) % N;
        exp /= 2;
    }
    return res;
}
}   // namespace _detail

auto consteval convertPrefix(Unit auto from, Unit auto to) noexcept -> double {
    const int amountDiff  = from.amount().prefix() - to.amount().prefix();
    const int currentDiff = from.current().prefix() - to.current().prefix();
    const int lengthDiff  = from.length().prefix() - to.length().prefix();
    const int luminosityDiff =
        from.luminosity().prefix() - to.luminosity().prefix();
    const int massDiff = from.mass().prefix() - to.mass().prefix();
    const int temperatureDiff =
        from.temperature().prefix() - to.temperature().prefix();
    const int timeDiff  = from.time().prefix() - to.time().prefix();
    const int angleDiff = from.angle().prefix() - to.angle().prefix();

    return _detail::pow10(amountDiff) * _detail::pow10(currentDiff) *
           _detail::pow10(lengthDiff) * _detail::pow10(luminosityDiff) *
           _detail::pow10(massDiff) * _detail::pow10(temperatureDiff) *
           _detail::pow10(timeDiff) * _detail::pow10(angleDiff);
}

auto consteval convertScale(Unit auto from, Unit auto to) noexcept -> double {

    using amountRatio =
        std::ratio_divide<typename decltype(to.amount())::ScaleFactor,
                          typename decltype(from.amount())::ScaleFactor>;
    using currentRatio =
        std::ratio_divide<typename decltype(to.current())::ScaleFactor,
                          typename decltype(from.current())::ScaleFactor>;
    using lengthRatio =
        std::ratio_divide<typename decltype(to.length())::ScaleFactor,
                          typename decltype(from.length())::ScaleFactor>;
    using luminosityRatio =
        std::ratio_divide<typename decltype(to.luminosity())::ScaleFactor,
                          typename decltype(from.luminosity())::ScaleFactor>;
    using massRatio =
        std::ratio_divide<typename decltype(to.mass())::ScaleFactor,
                          typename decltype(from.mass())::ScaleFactor>;
    using temperatureRatio =
        std::ratio_divide<typename decltype(to.temperature())::ScaleFactor,
                          typename decltype(from.temperature())::ScaleFactor>;
    using timeRatio =
        std::ratio_divide<typename decltype(to.time())::ScaleFactor,
                          typename decltype(from.time())::ScaleFactor>;
    using angleRatio =
        std::ratio_divide<typename decltype(to.angle())::ScaleFactor,
                          typename decltype(from.angle())::ScaleFactor>;
    double res{1.0};
    res *= static_cast<double>(amountRatio::num) /
           static_cast<double>(amountRatio::den);
    res *= static_cast<double>(currentRatio::num) /
           static_cast<double>(currentRatio::den);
    res *= static_cast<double>(lengthRatio::num) /
           static_cast<double>(lengthRatio::den);
    res *= static_cast<double>(luminosityRatio::num) /
           static_cast<double>(luminosityRatio::den);
    res *= static_cast<double>(massRatio::num) /
           static_cast<double>(massRatio::den);
    res *= static_cast<double>(temperatureRatio::num) /
           static_cast<double>(temperatureRatio::den);
    res *= static_cast<double>(timeRatio::num) /
           static_cast<double>(timeRatio::den);
    res *= static_cast<double>(angleRatio::num) /
           static_cast<double>(angleRatio::den);
    return res;
}

auto consteval conversionFactor(Unit auto from,
                                Unit auto to) noexcept -> double {
    return convertPrefix(from, to) * convertScale(from, to);
}

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

auto constexpr toString([[maybe_unused]] Unit auto u) -> std::string {
    return "";
}
}   // namespace Maxwell

#endif