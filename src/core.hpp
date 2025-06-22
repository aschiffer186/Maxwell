#ifndef CORE_HPP
#define CORE_HPP

#include <version>

#if __cplusplus >= 202002L
#define MAXWELL_HAS_CXX_20
#else
#error "Maxwell requires C++20 or later!"
#endif

#if __cplusplus >= 202300L
#define MAXWELL_HAS_CXX_23
#endif

#if MAXWELL_HAS_CXX_23
#define MAXWELL_CXX_23_CONSTEXPR constexpr
#if defined(__cpp_explicit_this_parameter) &&                                  \
    __cpp_explicit_this_parameter >= 202110L
#define MAXWELL_HAS_DEDUCING_THIS
#endif
#else
#define MAXWELL_CXX_23_CONSTEXPR
#endif

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#define MAXWELL_HAS_PRINT
#endif

#if defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202202L
#define MAXWELL_BASIC_CMATH_CONSTEXPR
#elif defined(__cpp_lib_constexpr_cmath) && __cpp_lib_constexpr_cmath >= 202306L
#define MAXWELL_EXTENDED_CMATH_CONSTEXPR
#endif

#endif