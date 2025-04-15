/**
 * @file config.hpp
 * @author Alex Schiffer
 * @brief Configuration macros for Maxwell
 * @version 1.0
 * @date 2025-04-15
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <version>

#if defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202202L
#define MAXWELL_MATH_CONSTEXPR23 constexpr
#else
#define MAXWELL_MATH_CONSTEXPR23
#endif

#if defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202306L
#define MAXWELL_MATH_CONSTEXPR26 constexpr
#else
#define MAXWELL_MATH_CONSTEXPR26
#endif

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L
#define MAXWELL_CONSTEXPR23 constexpr
#else
#define MAXWELL_CONSTEXPR23 constexpr
#endif

#define MATH_NOEXCEPT(func) noexcept(noexcept(std::func(std::declval<T>())))

#endif