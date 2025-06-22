/// \file formatting.hpp
/// \brief Defines functions for displaying types in Maxwell.
#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include "core.hpp"

#include <format>
#include <iterator>
#include <ostream>
#if MAXWELL_HAS_PRINT
#include <print>
#endif

#include "dimensional_product.hpp"
#include "unit.hpp"

template <maxwell::dimension T>
struct std::formatter<T> : std::formatter<std::string_view> {
  auto constexpr parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const T&, std::format_context& ctx) const {
    std::string out;
    constexpr auto name = T::name;
    std::format_to(std::back_inserter(out), "{}",
                   std::string_view{name.begin(), name.end()});
    if constexpr (T::power::num != 1 || T::power::den != 1) {
      out.append("^");
      if constexpr (T::power::den != 1) {
        std::format_to(std::back_inserter(out), "({}/{})", T::power::num,
                       T::power::den);
      } else {
        std::format_to(std::back_inserter(out), "{}", T::power::num);
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

template <maxwell::unit T>
struct std::formatter<T> : std::formatter<std::string_view> {
  auto constexpr parse(std::format_parse_context& ctx) { return ctx.begin(); }

  auto format(const maxwell::unit auto&, std::format_context& ctx) const {
    std::string temp;
    std::format_to(std::back_inserter(temp), "{}", T::unit_name());
  }
};

namespace maxwell {
std::ostream& operator<<(std::ostream& os, const dimension auto& dim) {
#ifdef MAXWELL_HAS_PRINT
  std::print(os, "{}", dim);
#else
  os << std::format("{}", dim);
#endif
  return os;
}

std::ostream& operator<<(std::ostream& os, const dimension_product auto& prod) {
#ifdef MAXWELL_HAS_PRINT
  std::print(os, "{}", prod);
#else
  os << std::format("{}", prod);
#endif
  return os;
}

std::ostream& operator<<(std::ostream& os, const unit auto& u) {
#ifdef MAXWELL_HAS_PRINT
  std::print(os, "{}", u);
#else
  os << std::format("{}", u);
#endif
  return os;
}
} // namespace maxwell

#endif