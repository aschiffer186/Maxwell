#ifndef UNIT_TYPES_HPP
#define UNIT_TYPES_HPP

#include "UnitCore.hpp"

namespace Maxwell
{
    using Newton = decltype(Kilogram{}*Meter{}/(Second{}*Second{}));
    using Joule = decltype(Newton{}*Meter{});
    using Watt = decltype(Joule{}/Second{});
    using Hertz = decltype(Dimensionless{}/Second{});
    using Pascal = decltype(Newton{}/(Meter{}*Meter{}));
    using Coulumb = decltype(Second{}*Ampere{});
    using Volt = decltype(Joule{}/Coulumb{});
    using Farad = decltype(Coulumb{}/Volt{});
    using Ohm = decltype(Volt{}/Ampere{});
    using Siemens = decltype(Ampere{}/Volt{});
    using Weber = decltype(Volt{}*Second{});
    using Tesla = decltype(Weber{}/(Meter{}*Meter{}));
    using Henry = decltype(Weber{}/Ampere{}); 
}

#endif