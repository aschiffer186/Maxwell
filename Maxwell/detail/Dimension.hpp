#ifndef DIMENSION_HPP
#define DIMENSION_HPP

#include <concepts>
#include <cstdint>
#include <ratio>
#include <type_traits>

///@file Dimension.hpp Definition of DimensionType

namespace Maxwell {
/// \cond
namespace _detail {
template <typename> struct is_ratio : std::false_type {};

template <typename std::intmax_t Num, typename std::intmax_t Den>
struct is_ratio<std::ratio<Num, Den>> : std::true_type {};

template <typename T>
concept Ratio = is_ratio<T>::value;
}   // namespace _detail
/// \endcond

// --- Dimension Concept ---

// Forward declaration

/// @brief Represents a single dimension of a unit
///
/// DimensionType reprsents a single dimension of a unit.
/// Each dimension is specified in terms of the coherent SI unit
/// for that dimension (e.g. mole, meter). The dimension is related
/// to the SI coherent unit by
///
/// D = ScaleFactor::num/ScaleFactor::den * 10^Prefix * C^Power + Offset.
///
/// For dimension with both a scale factor and an offset, the offset is
/// the offset after scaling. For example, Farenheit would have ScaleFactr
/// of 9/5 and an offset of 9/5 * -273.15 + 32 == -45967/1000
///
/// @tparam Power_ the power relative to the SI coherent unit
/// @tparam Prefix_ the metric prefix relative to the SI coherent unit
/// @tparam ScaleFactor_ the scale factor relative to the SI coherent unit
/// @tparam Offset_ the offset relative to the SI prefix unit
template <std::intmax_t Power, std::intmax_t Prefix, _detail::Ratio ScaleFactor, _detail::Ratio Offset>
struct DimensionType;

/// \cond
namespace _detail {
template <typename> struct is_dimension : std::false_type {};

template <typename T> struct is_dimension<const T> : is_dimension<T> {};

template <typename T> struct is_dimension<T&> : is_dimension<T> {};

template <typename T> struct is_dimension<T&&> : is_dimension<T> {};

template <std::intmax_t Power, std::intmax_t Prefix, _detail::Ratio ScaleFactor, _detail::Ratio Offset>
struct is_dimension<DimensionType<Power, Prefix, ScaleFactor, Offset>> : std::true_type {};

using One  = std::ratio<1, 1>;
using Zero = std::ratio<0, 1>;
}   // namespace _detail
/// \endcond

/// Concept modeling an instantiation of DimensionType
template <typename T>
concept Dimension = _detail::is_dimension<T>::value;

// Definition of unit
template <std::intmax_t Power_, std::intmax_t Prefix_, _detail::Ratio ScaleFactor_, _detail::Ratio Offset_>
struct DimensionType {
    /// The power of the dimension relative to the SI coherent unit
    static constexpr auto Power = Power_;
    /// The prefix of the dimension relative to the SI coherent unit
    static constexpr auto Prefix = Prefix_;
    /// The scale factor of the dimension relative to the SI coherent unit
    using ScaleFactor = ScaleFactor_;
    /// The offset of the dimension realtive to the SI coheren tunit
    using Offset = Offset_;

    /// @brief Returns the power of the dimension
    ///
    /// Returns the power of the dimension relative to the SI
    /// coherent. This function may also be used in a constant expression.
    ///
    /// @brief Returns the power of the dimension
    auto consteval power() const noexcept -> std::intmax_t { return Power; }

    /// @brief Returns the prefix of the dimension
    ///
    /// Returns the prefix of the dimension relative to the SI
    /// coherent. This function may also be used in a constant expression.
    ///
    /// @brief Returns the power of the dimension
    auto consteval prefix() const noexcept -> std::intmax_t { return Prefix; }

    ///@brief Adjusts the prefix of the dimension
    ///
    /// Adjusts the prefix of the dimension so the new prefix
    /// is equal to the old prefix + the specified prefix.
    /// This function may always be used in a constant expression.
    ///
    /// @post prefix() == old Prefix + NewPrefix
    ///
    /// @tparam NewPrefix the prefix to add by
    template <std::intmax_t NewPrefix> auto consteval adjustPrefix() const noexcept -> Dimension auto {
        return DimensionType<Power, Prefix + NewPrefix, ScaleFactor, Offset>{};
    }

    ///@brief Adjusts the scale factor of the dimesnion
    ///
    /// Adjusts the scale factor fo the dimension so the new
    /// scale factor is equal to the current scale factor *
    /// the specified scale factor. This function may always be
    /// used in a constant expression.
    ///
    ///@post std::ratio_equal_v<std::ratio_multiply<previous ScaleFactor,
    /// NewScaleFactor>, ScaleFactor>
    ///
    ///@tparam NewScaleFactor the scale factor to multiply by
    template <_detail::Ratio NewScaleFactor> auto consteval adjustScale() const noexcept -> Dimension auto {
        return DimensionType<Power, Prefix, std::ratio_multiply<NewScaleFactor, ScaleFactor>, Offset>{};
    }

    ///@brief Adjusts the offset of the dimension
    ///
    /// Adjusts the offset of the dimension so the new offset
    /// is equal to the specified offset + the old offset. This
    /// function may always be used in a constant expression.
    ///
    /// @post std::ratio_equal_v<std::ratio_add<old Offset, NewOffset>, Offset>
    ///
    /// @tparam NewPrefix the offset to add by
    template <_detail::Ratio NewOffset> auto consteval adjustOffset() const noexcept -> Dimension auto {
        return DimensionType<Power, Prefix, ScaleFactor, std::ratio_add<Offset, NewOffset>>{};
    }

    ///@brief Returns true if the dimension is coherent.
    ///
    /// Returns true if the dimension is coherent. A dimension is coherent
    /// if its prefix is 0, its scale factor is 1/1 and its offset is 0/1.
    /// This function may always be used in a constant expression.
    ///
    /// @returns true if the dimension is coherent
    auto consteval isCoherentDimension() const noexcept -> bool {
        return Prefix == 0 && std::ratio_equal_v<ScaleFactor, _detail::One> &&
               std::ratio_equal_v<Offset, _detail::Zero>;
    }

    /// @brief Returns a coherent dimension
    ///
    /// Returns a coherent dimension that has the same power as *this. This
    /// function may always be used in a constant expression.
    ///
    /// @post toCoherentDimension().isCoherentDimension() &&
    /// toCoherentDimension().power() == power()
    ///
    ///@return a coherent dimension that has the same power as *this
    auto consteval toCoherentDimension() const noexcept -> Dimension auto {
        return DimensionType<Power_, 0, _detail::One, _detail::Zero>{};
    }
};

// --- Constants ---

/// Constant representing a coheret dimension with power 1.
constexpr DimensionType<1, 0, _detail::One, _detail::Zero> CoherentDimension{};
/// Constant representing the absence of a dimension in a unit.
constexpr DimensionType<0, 0, _detail::One, _detail::Zero> NullDimension{};

// --- Query functions ---

/// @brief Returns true if two dimensons are equal
///
/// Returns true if two dimension are equal. Two dimensions are equal
/// if they have the same poweer, prefix, scale factor, and offset. This
/// also implies they have the same type. This function may always be used
/// in a constant expression.
///
/// @param lhs one dimension to compare
/// @param rhs the other dimension to compare
///
/// @return true if the two dimension are equal
auto constexpr
operator==(Dimension auto lhs, Dimension auto rhs) noexcept -> bool {
    return std::same_as<decltype(lhs), decltype(rhs)>;
}

/// @brief Concept specifying a dimension is convertible to another
///
/// A concept specifying From is convertible to To. From is convertible to To
/// if both are types modeling Dimension and From.power() == To.power()
template <auto From, auto To>
concept DimensionConvertibleTo =
    Dimension<decltype(From)> && Dimension<decltype(To)> && decltype(From)::Power == decltype(To)::Power;

// --- Dimension Composition ---

/// @brief Calculates the product of two dimensions
///
/// Calculates the products of two dimensions. The product of two dimensions
/// is a coherent dimension whose power is the sum of lhs.power() and
/// rhs.power(). If only one dimension has a prefix, the prefix is carried forward,
/// otherwise, the result is coherent dimension. This function may always be used
/// in a constant expression.

/// @post (lhs * rhs).power() == lhs.power() + rhs.power()
///
/// @param lhs one dimension to multiply
/// @param rhs the other dimension to multiply
///
/// @return the product of two dimensions
auto consteval
operator*(Dimension auto lhs, Dimension auto rhs) noexcept -> Dimension auto {
    constexpr int newPrefix      = (lhs.prefix() != 0 && rhs.prefix() != 0) ? 0
                                   : (lhs.prefix() != 0)                    ? lhs.prefix()
                                                                            : rhs.prefix();
    constexpr Dimension auto res = DimensionType<lhs.power() + rhs.power(), newPrefix, _detail::One, _detail::Zero>{};
    return res;
}

/// @brief Calculates the product of two dimensions
///
/// Calculates the quotient of two dimensions. The quotent of two dimensions
/// is a dimension whose power is the difference of lhs.power() and
/// rhs.power(). If only one dimension has a prefix, the prefix is carried forward,
/// otherwise, the result is coherent dimension. This function may always be used
/// in a constant expression.
///
/// @post (lhs / rhs).power() == lhs.power() - rhs.power()
///
/// @param lhs the divident
/// @param rhs the divisor
///
/// @return the quotient of two dimensions
auto consteval
operator/(Dimension auto lhs, Dimension auto rhs) noexcept -> Dimension auto {
    constexpr int newPrefix      = (lhs.prefix() != 0 && rhs.prefix() != 0) ? 0
                                   : (lhs.prefix() != 0)                    ? lhs.prefix()
                                                                            : rhs.prefix();
    constexpr Dimension auto res = DimensionType<lhs.power() - rhs.power(), newPrefix, _detail::One, _detail::Zero>{};
    return res;
}
}   // namespace Maxwell
#endif