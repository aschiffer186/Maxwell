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

#ifdef __cpp_lib_print
#define MAXWELL_USE_PRINT
#endif

#if defined(__cpp_constexpr) && _cpp_constexpr >= 202207L
#define MAXWELL_CONSTEXPR23 constexpr
#else
#define MAXWELL_CONSTEXPR23
#endif

#endif