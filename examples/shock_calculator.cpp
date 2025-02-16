#include "Maxwell.hpp"

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>

using Mach = maxwell::unitless_quantity;

std::tuple<Mach, maxwell::pascal, maxwell::kelvin> normal_shock_wave(Mach M0, maxwell::pressure auto p0,
                                                                     maxwell::temperature auto T0)
{
    constexpr double gamma{1.4};

    const Mach M1 = std::sqrt(((gamma - 1) * M0 * M0 + 2) / (2 * gamma * M0 * M0 - (gamma - 1)));

    const double          T_rat_num = (2 * gamma * M0 * M0 - (gamma - 1)) * ((gamma - 1) * M0 * M0 + 2);
    const double          T_rat_den = (gamma + 1) * (gamma + 1) * M0 * M0;
    const maxwell::kelvin T1        = T0 * (T_rat_num / T_rat_den);

    const double          p1_p0 = (2 * gamma * M0 * M0 - (gamma - 1)) / (gamma + 1);
    const maxwell::pascal p1    = p1_p0 * p0;

    return {M1, p1, T1};
}

int main()
{
    using namespace maxwell::metric_literals;

    const auto p0_pa = 26436.3_Pa;
    const auto T0    = 223.150_K;
    const auto M0    = 2.5;

    auto [M1, p1, T1] = normal_shock_wave(M0, p0_pa, T0);
    std::cout << "Upstream Mach: " << M1;
    std::cout << "\nUpstream pressure: " << p1;
    std::cout << "\nUpstream temperature: " << T1 << '\n';

    // const auto p0_atm = 0.2609059956_atm;

    // std::tie(M1, p1, T1) = normal_shock_wave(M0, p0_atm, 1.0_m);
    std::cout << "Upstream Mach: " << M1;
    std::cout << "\nUpstream pressure: " << p1;
    std::cout << "\nUpstream temperature: " << T1 << '\n';

    return 0;
}