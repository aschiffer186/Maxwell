#include "Maxwell.hpp"
#include "QuantityRepo.hpp"
#include <tuple>

using Mach = maxwell::unitless_quantity;

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
}