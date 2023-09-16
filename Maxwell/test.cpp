#include "include/QuantityCore.hpp"
#include "include/Unit.hpp"
#include "include/Quantity.hpp"
#include "include/UnitCore.hpp"

#include <cmath>
#include <compare>
#include <concepts>
#include <type_traits>

using namespace Maxwell::Literals;

int main()
{
    constexpr Maxwell::Quantity<double, Maxwell::Meter> q;
    constexpr Maxwell::Quantity<double, Maxwell::Meter> comp = std::move(q);
    Maxwell::Quantity<int, Maxwell::Meter> q2{1_m};
    constexpr auto q3 = 1_s*2.0_m;
    constexpr Maxwell::Quantity q4{1_A};
    static_assert(q3.units() == Maxwell::Second{}*Maxwell::Meter{});
    static_assert(q4.value() == 1);

    double d = std::pow(1.0_m/2.0_m, 2);
    // bool b = q == comp;

    // static_assert(std::is_trivially_default_constructible_v<decltype(q2)>);
    static_assert(std::is_nothrow_default_constructible_v<decltype(q2)>);
    static_assert(std::is_trivially_copy_constructible_v<decltype(q2)>);
    static_assert(std::is_nothrow_copy_constructible_v<decltype(q2)>);
    static_assert(std::is_trivially_move_constructible_v<decltype(q2)>);
    static_assert(std::is_nothrow_move_constructible_v<decltype(q2)>);
    static_assert(std::is_nothrow_default_constructible_v<decltype(q2)>);
    static_assert(std::is_trivially_copy_assignable_v<decltype(q2)>);
    static_assert(std::is_nothrow_copy_assignable_v<decltype(q2)>);
    static_assert(std::is_trivially_move_assignable_v<decltype(q2)>);
    static_assert(std::is_nothrow_move_assignable_v<decltype(q2)>);
    static_assert(std::is_trivially_destructible_v<decltype(q2)>);
    static_assert(std::equality_comparable<decltype(q2)>);
    static_assert(std::three_way_comparable<decltype(q2)>);
    static_assert(std::totally_ordered<decltype(q2)>);
    // static_assert(std::is_nothrow_move_assignable_v<decltype(q)>);
    // static_assert(std::regular<decltype(q)>);

    return 0;
}