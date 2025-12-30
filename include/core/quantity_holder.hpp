/// \file quantity_holder.hpp
/// \brief Definition of class template \c quantity_holder.

#ifndef QUANTITY_HOLDER_HPP
#define QUANTITY_HOLDER_HPP

#include <functional> // hash

#include "impl/quantity_holder_declaration.hpp"
#include "impl/quantity_holder_impl.hpp"

MODULE_EXPORT template <auto Q, typename T>
struct std::hash<maxwell::quantity_holder<Q, T>> {
  auto operator()(const maxwell::quantity_holder<Q, T>& q) noexcept
      -> std::size_t {
    std::size_t hash_code = std::hash<T>{}(q.in_base_units().value_);
    return hash_code;
  }
};

#endif