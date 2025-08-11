#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <version> // version macros

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#define MAXWELL_HAS_PRINT
#endif

#if defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202202L
#define MAXWELL_BASIC_CMATH_CONSTEXPR
#elif defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202306L
#define MAXWELL_EXTENDED_CMATH_CONSTEXPR
#endif

#if defined(__cpp_constexpr) && __cpp_constexpr >= 202207L
#define MAXWELL_CONSTEXPR23 constexpr
#else
#define MAXWELL_CONSTEXPR23 constexpr
#endif

#endif