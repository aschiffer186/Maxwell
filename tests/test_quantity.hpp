#ifndef TEST_QUANTITY_HPP
#define TEST_QUANTITY_HPP

#include <initializer_list>
#include <numeric>

template <bool Throwing> struct noisy {
  static inline int num_default_ctor_calls{0};
  static inline int num_copy_ctor_calls{0};
  static inline int num_move_ctor_calls{0};

  constexpr noisy() noexcept(Throwing) { ++num_default_ctor_calls; }
  constexpr explicit noisy(double d) noexcept(Throwing) : value(d) {}
  constexpr noisy(double d1, double d2) noexcept(Throwing) : value(d1 + d2) {}
  constexpr noisy(std::initializer_list<double> il, double d3) noexcept(Throwing)
      : value(std::accumulate(il.begin(), il.end(), d3)) {}
  constexpr noisy(const noisy& other) noexcept(Throwing) : value(other.value) { ++num_copy_ctor_calls; }
  constexpr noisy(noisy&& other) noexcept(Throwing) : value(other.value) { ++num_move_ctor_calls; }

  double value{0.0};
};

struct literal {};

using nothrow_noisy = noisy<true>;
using throwing_noisy = noisy<false>;

template <typename Lambda, int = (Lambda{}(), 0)> constexpr bool is_constant_expression(Lambda) noexcept {
  return true;
}

constexpr bool is_constant_expression(...) noexcept { return false; }

#endif