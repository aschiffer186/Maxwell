#include "include/Unit.hpp"
#include "include/Quantity.hpp"

using namespace Maxwell::Literals;

int main()
{
    Maxwell::Quantity<double, Maxwell::Meter> q;
    constexpr Maxwell::Quantity<int, Maxwell::Meter> q2{1_m};
    constexpr auto q3 = 1_s*2.0_m;
    static_assert(q3.units() == Maxwell::Second{}*Maxwell::Meter{});

    return 0;
}