#ifndef COMPOSITE_QUANTITY_HPP
#define COMPOSITE_QUANTITY_HPP

#include "unit.hpp"
#include <cstddef>
#include <type_traits>

namespace maxwell {
template <typename Rep, std::size_t NumRows, std::size_t NumColumns,
          auto... Units>
  requires(unit<decltype(Units)> && ...) &&
          (NumRows * NumColumns == sizeof...(Units))
class quantity_matrix {
public:
  constexpr quantity_matrix()
    requires std::is_default_constructible_v<Rep>
  = default;

  template <typename U = Rep>
    requires std::constructible_from<Rep, U>
  constexpr quantity_matrix(U&& u) noexcept(
      std::is_nothrow_constructible_v<Rep, U>)
      : rep_(std::forward<U>(u)) {}

  constexpr const Rep& underlying_value() const& noexcept { return rep_; }

  constexpr const Rep&& underlying_value() const&& noexcept {
    return std::move(rep_);
  }

  constexpr Rep&& underlying_value() && noexcept { return std::move(rep_); }

  template <std::intmax_t Row, std::intmax_t Column> constexpr auto get() {
    decltype(auto) value = rep_(Row, Column);
    return value;
  }

private:
  Rep rep_;
};
} // namespace maxwell

#endif