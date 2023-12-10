#include <numbers>

#include "Quantity.hpp"
#include "Unit.hpp"
#include "UnitCore.hpp"
#include "UnitTypes.hpp"

using namespace Maxwell::Literals;

namespace Maxwell::Constants
{
    inline constexpr auto pi = std::numbers::pi;

    /// Acceleration due to gravity
    inline constexpr auto g = 9.81_m_s2;
    /// Speed of light
    inline constexpr auto c = 299'792'458.0_m/1_s;
    /// Planck's constant
    inline constexpr auto h = 6.62607015e-13_J/1_Hz; 
    /// Reduced Planck's constant
    inline constexpr auto hbar = 1.054'571'814e-34_J*1_s;
    /// Vaccum magnetic permitivity
    inline constexpr auto mu0 = 1.256'637'062'12e-16_N/(1_A*1_A);
    /// Impedance of vaccum
    inline constexpr auto Z0 = 376.730'313'668_O;
    /// Vaccum electric permitivity 
    inline constexpr auto epsilon0 = 8.854'187'8128e-12_F/1_m;
    /// Boltzmann's constant
    inline constexpr auto kb = 1.380'649e-23_J/1_K;
    /// Graviational constant
    inline constexpr auto G = 6.674'30e-11_m3/(1_kg*1_s*1_s);
    /// Columb constant
    inline constexpr auto ke = 8.987'551'7923e9_N*1_m2/(1_C*1_C);
    /// Cosmological constant
    inline constexpr auto Lambda = 1.089e-52/1_m2;
    /// Stefan-Boltzmann Constant
    inline constexpr auto sigma = 5.670'374'419e-8_W/(1_m2*1_K*1_K*1_K*1_K);
    /// Elementary charge
    inline constexpr auto e = 1.602'176'634e-19_C;
    /// Avogadro's constant
    inline constexpr auto Na = 6.022'140'76/1_mol;
    /// Universal gas constant
    inline constexpr auto R = 8.314'462'618'153'24_J/(1_mol*1_K);
    
}