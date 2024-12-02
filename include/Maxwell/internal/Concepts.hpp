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
#include <memory>
#include <type_traits>
namespace maxwell::internal
{
/// \brief Specifies two types can be added together
/// \tparam Tp The left hand side type of the addition
/// \tparam Up The right hand side type of the addition
template <typename Tp, typename Up>
concept add_enabled_with = requires(Tp a, Up b) { a + b; };

/// \brief Specifies a type supports addition with itself
/// \tparam Tp The type to check
template <typename Tp>
concept add_enabled = add_enabled_with<Tp, Tp>;

/// \brief Specifies two types can be added together and the addition is \c noexcept
/// \tparam Tp The left hand side type of the addition
/// \tparam Up The right hand side type of the addition
template <typename Tp, typename Up>
concept nothrow_add_enabled_with = add_enabled_with<Tp, Up> && requires(Tp a, Up b) { noexcept(a + b); };

/// \brief Specifies a type supports addition with itself and the addition is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept nothrow_add_enabled = nothrow_add_enabled_with<Tp, Tp>;

/// \brief Specifies two types can be subtracted
/// \tparam Tp The left hand side type of the subtraction
/// \tparam Up The right hand side type of the subtraction
template <typename Tp, typename Up>
concept subtract_enabled_with = requires(Tp a, Up b) { a + b; };

/// \brief Specifies a type supports subtraction from itself
/// \tparam Tp The type to check
template <typename Tp>
concept subtract_enabled = subtract_enabled_with<Tp, Tp>;

/// \brief Specifies two types can be subtracted and the subtraction is \c noexcept
/// \tparam Tp The left hand side type of the subtraction
/// \tparam Up The right hand side type of the subtraction
template <typename Tp, typename Up>
concept nothrow_subtract_enabled_with = subtract_enabled_with<Tp, Up> && requires(Tp a, Up b) { noexcept(a - b); };

/// \brief Specifies a type supports subtraction from itself and the subtraction is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept nothrow_subtract_enabled = nothrow_subtract_enabled_with<Tp, Tp>;

// clang-format off
/// \brief Specifies two types can be multiplied
/// \tparam Tp The left hand side type of the multiplication
/// \tparam Up The right hand side type of the multiplication
template <typename Tp, typename Up>
concept multiply_enabled_with = requires(Tp a, Up b) { a * b; };

/// \brief Specifies a type supports multiplied by itself
/// \tparam Tp The type to check
template <typename Tp>
concept multiply_enabled = multiply_enabled_with<Tp, Tp>;

/// \brief Specifies two types can be multiplied and the multiplication is \c noexcept
/// \tparam Tp The left hand side type of the multiplication
/// \tparam Up The right hand side type of the multiplication
template <typename Tp, typename Up>
concept nothrow_multiply_enabled_with = multiply_enabled_with<Tp, Up> && requires(Tp a, Up b) { noexcept(a * b); };

/// \brief Specifies a type supports multiplication by itself and the multiplication is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept nothrow_multiply_enabled = nothrow_multiply_enabled_with<Tp, Tp>;
// clang-format on

/// \brief Specifies two types can be divided
/// \tparam Tp The left hand side type of the division
/// \tparam Up The right hand side type of the division
template <typename Tp, typename Up>
concept divide_enabled_with = requires(Tp a, Up b) { a / b; };

/// \brief Specifies a type supports division by itself
/// \tparam Tp The type to check
template <typename Tp>
concept divide_enabled = divide_enabled_with<Tp, Tp>;

/// \brief Specifies two types can be divided and the division is \c noexcept
/// \tparam Tp The left hand side type of the division
/// \tparam Up The right hand side type of the division
template <typename Tp, typename Up>
concept nothrow_divide_enabled_with = divide_enabled_with<Tp, Up> && requires(Tp a, Up b) { noexcept(a / b); };

/// \brief Specifies a type supports divided by itself and the division is \c noexcept
/// \tparam Tp The type to check
template <typename Tp>
concept nothrow_divide_enabled = nothrow_divide_enabled_with<Tp, Tp>;

/// \brief Specifies modulus can be computed between two types
/// \tparam Tp The left hand side type of the modulo
/// \tparam Up The right hand side type of the modulo
template <typename Tp, typename Up>
concept modulo_enabled_with = requires(Tp a, Tp b) { a % b; };

/// \brief Specifies modulus can be computed for a type
/// \tparam Tp The type to check
template <typename Tp>
concept module_enabled = modulo_enabled_with<Tp, Tp>;

/// \brief Specifies modulus can be computed between two types and the calculation does not throw exceptions.
/// \tparam Tp The left hand side type of the modulo
/// \tparam Up The right hand side type of the modulo
template <typename Tp, typename Up>
concept nothrow_modulo_enabled_with = modulo_enabled_with<Tp, Up> && requires(Tp a, Tp b) { noexcept(a % b); };

/// \brief Specifies modulus can be computed for a type and the calculation does not throw exceptions.
/// \tparam Tp The type to check
template <typename Tp>
concept nothrow_modulo_enabled = nothrow_modulo_enabled_with<Tp, Tp>;

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
concept chrono_duration = is_chrono_dur<std::remove_cvref_t<T>>::value;
} // namespace _detail
/// \endcond

/// \brief Specifies two types are the same up to cvref-qualifiers
///
/// \tparam The first type to check
/// \tparam The second type to check
template <typename T, typename U>
concept similar = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

namespace _detail
{
template <typename>
struct specializes_pointer_traits : std::false_type
{
};

template <typename Ptr>
struct specializes_pointer_traits<std::pointer_traits<Ptr>> : std::true_type
{
};

template <typename T>
concept pointer_like = specializes_pointer_traits<T>::value;
} // namespace _detail
} // namespace maxwell::internal

#endif