#include "Maxwell.hpp"
#include "Unit.hpp"

#include <iostream>
#include <tuple>

using Mach = maxwell::unitless_quantity;

void func(maxwell::unit auto u)
{
    [[maybe_unused]] auto a = u.template adjust_scale_time<std::ratio<1, 1>>();
}

std::tuple<Mach, maxwell::pascal, maxwell::kelvin> normal_shock_wave(Mach M0, maxwell::pressure auto p0,
                                                                     maxwell::temperature auto T0)
{
    constexpr double gamma{1.4};

    const Mach M1 = ((gamma - 1) * M0 * M0 + 2) / (2 * gamma * M0 * M0 - (gamma - 1));

    const double          T_rat_num = (2 * gamma * M0 - (gamma - 1)) * ((gamma - 1) * M0 * M0 + 2);
    const double          T_rat_den = (gamma + 1) * (gamma + 1) * M0 * M0;
    const maxwell::kelvin T1        = T0 * (T_rat_num / T_rat_den);

    const double          p1_p0 = (2 * gamma * M0 * M0 - (gamma - 1)) / (gamma - 1);
    const maxwell::pascal p1    = p1_p0 * p0;

    return {M1, p1, T1};
}

int main()
{
    using namespace maxwell::metric_literals;

    func(maxwell::meter_unit);

    auto [M1, p1, T1] = normal_shock_wave(1.7, 101325_Pa, 275_K);
    std::cout << "Upstream Mach: " << M1;
    std::cout << "\nUpstream pressure: " << p1;
    std::cout << "\nUpstream temperature: " << T1 << '\n';
    return 0;
}