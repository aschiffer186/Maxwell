/// \file formatting.hpp
/// \brief Provides formatting and printing functions for types in maxwell.

#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <format>      // formatter
#include <iterator>    // back_inserter
#include <ostream>     // ostream
#include <string>      // string
#include <string_view> // string_view

#include "core/dimension.hpp"
#include "core/unit.hpp"

template <maxwell::dimension D>
struct std::formatter<D> : std::formatter<std::string_view> {
  auto constexpr parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const D&, std::format_context& ctx) const {
    std::string out;
    constexpr auto name = D::name;
    std::format_to(std::back_inserter(out), "{}",
                   std::string_view{name.begin(), name.end()});
    if constexpr (D::power.numerator != 1 || D::power.denominator != 1) {
      out.append("^");
      if constexpr (D::power.denominator != 1) {
        std::format_to(std::back_inserter(out), "({}/{})", D::power.numerator,
                       D::power.denominator);
      } else {
        std::format_to(std::back_inserter(out), "{}", D::power.numerator);
      }
    }
    return std::formatter<std::string_view>::format(out, ctx);
  }
};

template <maxwell::dimension... Dims>
struct std::formatter<maxwell::dimension_product_type<Dims...>>
    : std::formatter<std::string_view> {
  auto constexpr parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const maxwell::dimension_product_type<Dims...>&,
              std::format_context& ctx) const {
    std::string out = format_impl<Dims...>();
    return std::formatter<std::string_view>::format(out, ctx);
  }

private:
  template <maxwell::dimension D, maxwell::dimension... Ds>
  static std::string format_impl() {
    if constexpr (sizeof...(Ds) == 0) {
      return std::format({}, D{});
    } else {
      std::string curr = std::format("{}", D{});
      curr.append("*");
      curr.append(format_impl<Ds...>());
    }
  }
};

template <maxwell::unit U>
struct std::formatter<U> : std::formatter<std::string_view> {
  auto constexpr parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const maxwell::unit auto&, std::format_context& ctx) const {
    std::string temp;
    std::string name{U::name.begin(), U::name.end()};
    std::format_to(std::back_inserter(temp), "{}", name);
    return std::formatter<std::string_view>::format(temp, ctx);
  }
};

namespace maxwell {
auto operator<<(std::ostream& os, const dimension auto& dim) -> std::ostream& {
#ifdef MAXWELL_HAS_PRINT
  std::print(os, "{}", dim);
#else
  os << std::format("{}", dim);
#endif
  return os;
}

auto operator<<(std::ostream& os,
                const dimension_product auto& prod) -> std::ostream& {
#ifdef MAXWELL_HAS_PRINT
  std::print(os, "{}", prod);
#else
  os << std::format("{}", prod);
#endif
  return os;
}

auto operator<<(std::ostream& os, const unit auto& u) -> std::ostream& {
#ifdef MAXWELL_HAS_PRINT
  std::print(os, "{}", u);
#else
  os << std::format("{}", u);
#endif
  return os;
}
} // namespace maxwell

#endif