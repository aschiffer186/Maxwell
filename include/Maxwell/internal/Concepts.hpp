///
/// \file Concepts.hpp
/// \author Alex Schiffer
/// \brief Utility concepts not intended to be uesd by consumers of the library.
/// \version 0.1
/// \date 2024-11-02
///
/// \copyright Copyright (c) 2024
///
///
#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <chrono>
#include <type_traits>
namespace Maxwell::Internal
{
/// \brief Specifies two types can be added together
/// \tparam Tp The left hand side type of the addition
/// \tparam Up The right hand side type of the addition
template <typename Tp, typename Up>
concept AddEnabledWith = requires(Tp a, Up b) { a + b; };

/// \brief Specifies a type supports addition with itself
/// \tparam Tp The type to check
template <typename Tp>
concept AddEnabled = AddEnabledWith<Tp, Tp>;

/// \brief Specifies two types can be added together and the addition is \c noexcept
/// \tparam Tp The left hand side type of the addition
/// \tparam Up The right hand side type of the addition
template <typename Tp, typename Up>
concept NothrowAddEnabledWith = AddEnabledWith<Tp, Up> && requires(Tp a, Up b) { noexcept(a + b); };

/// \brief Specifies a type supports addition with itself and the addition is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept NothrowAddEnabled = NothrowAddEnabledWith<Tp, Tp>;

/// \brief Specifies two types can be subtracted
/// \tparam Tp The left hand side type of the subtraction
/// \tparam Up The right hand side type of the subtraction
template <typename Tp, typename Up>
concept SubtractEnabledWith = requires(Tp a, Up b) { a + b; };

/// \brief Specifies a type supports subtraction from itself
/// \tparam Tp The type to check
template <typename Tp>
concept SubtractEnabled = SubtractEnabledWith<Tp, Tp>;

/// \brief Specifies two types can be subtracted and the subtraction is \c noexcept
/// \tparam Tp The left hand side type of the subtraction
/// \tparam Up The right hand side type of the subtraction
template <typename Tp, typename Up>
concept NothrowSubtractEnabledWith = SubtractEnabledWith<Tp, Up> && requires(Tp a, Up b) { noexcept(a - b); };

/// \brief Specifies a type supports subtraction from itself and the subtraction is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept NothrowSubtractEnabled = NothrowSubtractEnabledWith<Tp, Tp>;

// clang-format off
/// \brief Specifies two types can be multiplied
/// \tparam Tp The left hand side type of the multiplication
/// \tparam Up The right hand side type of the multiplication
template <typename Tp, typename Up>
concept MultiplyEnabledWith = requires(Tp a, Up b) { a * b; };

/// \brief Specifies a type supports multiplied by itself
/// \tparam Tp The type to check
template <typename Tp>
concept MultiplyEnabled = MultiplyEnabledWith<Tp, Tp>;

/// \brief Specifies two types can be multiplied and the multiplication is \c noexcept
/// \tparam Tp The left hand side type of the multiplication
/// \tparam Up The right hand side type of the multiplication
template <typename Tp, typename Up>
concept NothrowMultiplyEnabledWith = MultiplyEnabledWith<Tp, Up> && requires(Tp a, Up b) { noexcept(a * b); };

/// \brief Specifies a type supports multiplication by itself and the multiplication is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept NothrowMultiplyEnabled = NothrowMultiplyEnabledWith<Tp, Tp>;
// clang-format on

/// \brief Specifies two types can be divided
/// \tparam Tp The left hand side type of the division
/// \tparam Up The right hand side type of the division
template <typename Tp, typename Up>
concept DivideEnabledWith = requires(Tp a, Up b) { a / b; };

/// \brief Specifies a type supports division by itself
/// \tparam Tp The type to check
template <typename Tp>
concept DivideEnabled = DivideEnabledWith<Tp, Tp>;

/// \brief Specifies two types can be divided and the division is \c noexcept
/// \tparam Tp The left hand side type of the division
/// \tparam Up The right hand side type of the division
template <typename Tp, typename Up>
concept NothrowDivideEnabledWith = DivideEnabledWith<Tp, Up> && requires(Tp a, Up b) { noexcept(a / b); };

/// \brief Specifies a type supports divided by itself and the division is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept NothrowDivideEnabled = NothrowDivideEnabledWith<Tp, Tp>;

/// \cond
namespace _detail
{
template <typename>
struct is_chrono_dur : std::false_type
{
};

template <typename Rep, typename Period>
struct is_chrono_dur<std::chrono::duration<Rep, Period>> : std::true_type
{
};

template <typename T>
concept ChronoDuration = is_chrono_dur<std::remove_cvref_t<T>>::value;
} // namespace _detail
/// \endcond

/// \brief Specifies two types are the same up to cvref-qualifiers
///
/// \tparam The first type to check
/// \tparam The second type to check
template <typename T, typename U>
concept Similar = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;
/// \endcond
} // namespace Maxwell::Internal

#endif