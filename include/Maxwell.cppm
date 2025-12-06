module;

#include <cassert>
#include <chrono>
#include <cmath>
#include <compare>
#include <concepts>
#include <format>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <numbers>
#include <numeric>
#include <ostream>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

export module Maxwell;

#include "core/dimension.hpp"
#include "core/quantity_holder.hpp"
#include "core/quantity_system.hpp"
#include "core/quantity_value.hpp"
#include "core/scale.hpp"
#include "core/unit.hpp"
#include "formatting/formatting.hpp"
#include "math/quantity_limits.hpp"
#include "math/quantity_value_math.hpp"
#include "quantity_systems/isq.hpp"
#include "quantity_systems/other.hpp"
#include "quantity_systems/si.hpp"
#include "quantity_systems/si_constants.hpp"
#include "quantity_systems/us.hpp"
#include "utility/compile_time_math.hpp"
#include "utility/config.hpp"
#include "utility/template_string.hpp"
#include "utility/type_traits.hpp"
