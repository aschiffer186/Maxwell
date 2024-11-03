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

/// \brief Specifies a type supports addition with itself andthe addition is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept NothrowAddEnabled = NothrowAddEnabledWith<Tp, Tp>;

template <typename Tp>
concept SubtractEnabled = requires(Tp a, Tp b) { a - b; };

template <typename Tp, typename Up>
concept SubtractEnabledWith = requires(Tp a, Up b) { a + b; };

template <typename Tp>
concept NothrowSubtractEnabled = SubtractEnabled<Tp> && requires(Tp a, Tp b) { noexcept(a - b); };

template <typename Tp, typename Up>
concept NothrowSubtractEnabledWith = SubtractEnabledWith<Tp, Up> && requires(Tp a, Up b) { noexcept(a - b); };

template <typename Tp>
concept MultiplyEnabled = requires(Tp a, Tp b) { a* b; };

template <typename Tp>
concept NothrowMultiplyEnabled = MultiplyEnabled<Tp> && requires(Tp a, Tp b) { noexcept(a * b); };

template <typename Tp>
concept DivideEnabled = requires(Tp a, Tp b) { a / b; };

template <typename Tp>
concept NothrowDivideEnabled = DivideEnabled<Tp> && requires(Tp a, Tp b) { noexcept(a / b); };

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
} // namespace Maxwell::Internal

#endif