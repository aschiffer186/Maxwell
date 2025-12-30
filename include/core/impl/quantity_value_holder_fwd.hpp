#ifndef QUANTITY_VALUE_HOLDER_FWD_HPP
#define QUANTITY_VALUE_HOLDER_FWD_HPP

#include <stdexcept>   // runtime_error
#include <type_traits> // false_type, remove_cvref_t, true_type

#include "core/quantity.hpp"
#include "core/unit.hpp"

namespace maxwell {
/// \brief Exception thrown when attempting to perform an operation on
/// incompatible \c quantity_holders
///
/// Class \c incompatible_quantity_holder is an exception thrown when an
/// arithmetic operation is performed on two instaces of \c quantity_holder with
/// difference references. Because the units of a \c quantity_holder are
/// determined at run-time, this error cannot be caught at compile-time.
MODULE_EXPORT class incompatible_quantity_holder : public std::runtime_error {
public:
  /// \brief Constructor
  ///
  /// \param message The error message associated with the exception.
  explicit incompatible_quantity_holder(const std::string& message)
      : std::runtime_error(message) {}
};

MODULE_EXPORT template <auto Q, typename T = double>
  requires quantity<decltype(Q)>
class quantity_holder;

MODULE_EXPORT template <auto U, auto Q = U.quantity, typename T = double>
  requires unit<decltype(U)> && quantity<decltype(Q)>
class quantity_value;

/// \cond
namespace _detail {
template <typename> struct is_quantity_value : std::false_type {};

template <auto U, auto Q, typename T>
struct is_quantity_value<quantity_value<U, Q, T>> : std::true_type {};

template <typename T>
constexpr bool is_quantity_value_v =
    is_quantity_value<std::remove_cvref_t<T>>::value;

template <typename T>
concept quantity_value_like = is_quantity_value_v<T>;

template <typename> struct is_quantity_holder : std::false_type {};

template <auto Q, typename T>
struct is_quantity_holder<quantity_holder<Q, T>> : std::true_type {};

template <typename T>
constexpr bool is_quantity_holder_v =
    is_quantity_holder<std::remove_cvref_t<T>>::value;

template <typename T>
concept quantity_holder_like = is_quantity_holder_v<std::remove_cvref_t<T>>;
} // namespace _detail
/// \endcond
} // namespace maxwell

#endif