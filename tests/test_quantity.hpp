#ifndef TEST_QUANTITY_HPP
#define TEST_QUANTITY_HPP

#include <initializer_list>
#include <numeric>

template <bool Throwing> struct noisy {
  static inline int num_default_ctor_calls{0};
  static inline int num_copy_ctor_calls{0};
  static inline int num_move_ctor_calls{0};

  noisy() noexcept(Throwing) { ++num_default_ctor_calls; }
  explicit noisy(double d) noexcept(Throwing) : value(d) {}
  noisy(double d1, double d2) noexcept(Throwing) : value(d1 + d2) {}
  noisy(std::initializer_list<double> il, double d3) : value(std::accumulate(il.begin(), il.end(), d3)) {}
  noisy(const noisy& other) noexcept(Throwing) : value(other.value) { ++num_copy_ctor_calls; }
  noisy(noisy&& other) noexcept(Throwing) : value(other.value) { ++num_move_ctor_calls; }

  double value{0.0};
};

using nothrow_noisy = noisy<true>;
using throwing_noisy = noisy<false>;

#endif