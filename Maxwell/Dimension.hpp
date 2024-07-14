#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include <concepts>
#include <cstdint>
#include <ratio>
#include <type_traits>

namespace Maxwell {
namespace _detail {
template <typename> struct is_ratio : std::false_type {};

template <typename std::intmax_t Num, typename std::intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> : std::true_type {};

template <typename T>
concept Ratio = is_ratio<T>::value;
}   // namespace _detail

// --- Dimension Concept ---
template <std::intmax_t Power, std::intmax_t Prefix, _detail::Ratio ScaleFactor,
          _detail::Ratio Offset>
struct DimensionType;

namespace _detail {
template <typename> struct is_dimension : std::false_type {};

template <typename T> struct is_dimension<const T> : is_dimension<T> {};

template <typename T> struct is_dimension<T&> : is_dimension<T> {};

template <typename T> struct is_dimension<T&&> : is_dimension<T> {};

template <std::intmax_t Power, std::intmax_t Prefix, _detail::Ratio ScaleFactor,
          _detail::Ratio Offset>
struct is_dimension<DimensionType<Power, Prefix, ScaleFactor, Offset>>
    : std::true_type {};

using One  = std::ratio<1, 1>;
using Zero = std::ratio<0, 1>;
}   // namespace _detail

template <typename T>
concept Dimension = _detail::is_dimension<T>::value;

// Represents a single dimension of a unit
template <std::intmax_t Power_, std::intmax_t Prefix_,
          _detail::Ratio ScaleFactor_, _detail::Ratio Offset_>
struct DimensionType {
    static constexpr auto Power  = Power_;
    static constexpr auto Prefix = Prefix_;
    using ScaleFactor            = ScaleFactor_;
    using Offset                 = Offset_;

    auto consteval prefix() const noexcept -> std::intmax_t { return Power; }

    auto consteval power() const noexcept -> std::intmax_t { return Prefix; }

    template <std::intmax_t NewPrefix>
    auto consteval adjustPrefix() const noexcept -> Dimension auto {
        return DimensionType<Power, NewPrefix, ScaleFactor, Offset>{};
    }

    template <_detail::Ratio NewScaleFactor>
    auto consteval adjustScale() const noexcept -> Dimension auto {
        return DimensionType<Power, Prefix,
                             std::ratio_multiply<NewScaleFactor, ScaleFactor>,
                             Offset>{};
    }

    auto consteval toCoherentDimension() const noexcept -> Dimension auto {
        return DimensionType<Power_, 0, _detail::One, _detail::Zero>{};
    }

    auto consteval isCoherentDimension() const noexcept -> bool {
        return Prefix == 0 && std::ratio_equal_v<ScaleFactor, _detail::One> &&
               std::ratio_equal_v<Offset, _detail::Zero>;
    }
};

// --- Constants ---
constexpr DimensionType<1, 0, _detail::One, _detail::Zero> CoherentDimension{};
constexpr DimensionType<0, 0, _detail::One, _detail::Zero> NullDimension{};

// --- Query functions ---
auto consteval
operator==(Dimension auto lhs, Dimension auto rhs) noexcept -> bool {
    return std::same_as<decltype(lhs), decltype(rhs)>;
}

template <auto From, auto To>
concept DimensionConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> &&
    From.power() == To.power();

// --- Dimension Composition ---
// Multiplies two dimensions. Always returns a coherent dimension
auto consteval
operator*(Dimension auto lhs, Dimension auto rhs) noexcept -> Dimension auto {
    constexpr Dimension auto res = DimensionType<lhs.power() + rhs.power(), 0,
                                                 _detail::One, _detail::Zero>{};
    static_assert(res.isCoherentDimension());
    return res;
}

// Divides two dimensions. Always returns a coherent dimension
auto consteval
operator/(Dimension auto lhs, Dimension auto rhs) noexcept -> Dimension auto {
    constexpr Dimension auto res = DimensionType<lhs.power() - rhs.power(), 0,
                                                 _detail::One, _detail::Zero>{};
    static_assert(res.isCoherentDimension());
    return res;
}
}   // namespace Maxwell
#endif