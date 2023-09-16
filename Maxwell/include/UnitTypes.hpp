#ifndef UNIT_TYPES_HPP
#define UNIT_TYPES_HPP

#include "UnitCore.hpp"

namespace Maxwell
{
    // Scaled versions of second
    using Quectosecond = scale_unit_time_t<Second, -30>;
    using Rontosecond  = scale_unit_time_t<Second, -27>; 
    using Yoctosecond  = scale_unit_time_t<Second, -24>;
    using Zeptosecond  = scale_unit_time_t<Second, -21>;
    using Attosecond   = scale_unit_time_t<Second, -18>;
    using Femtosecond  = scale_unit_time_t<Second, -15>;
    using Picosecond   = scale_unit_time_t<Second, -12>;
    using Nanosecond   = scale_unit_time_t<Second, -9>;
    using Microsecond  = scale_unit_time_t<Second, -6>;
    using Millisecond  = scale_unit_time_t<Second, -3>;
    using Centisecond  = scale_unit_time_t<Second, -2>;
    using Decisecond   = scale_unit_time_t<Second, -1>;
    using Decasecond   = scale_unit_time_t<Second, 1>;
    using Hectosecond  = scale_unit_time_t<Second, 2>;
    using Kilosecond   = scale_unit_time_t<Second, 3>;
    using Megasecond   = scale_unit_time_t<Second, 6>;
    using Gigasecond   = scale_unit_time_t<Second, 9>;
    using Terasecond   = scale_unit_time_t<Second, 12>;
    using Petasecond   = scale_unit_time_t<Second, 15>;
    using Exasecond    = scale_unit_time_t<Second, 18>; 
    using Zettasecond  = scale_unit_time_t<Second, 21>;
    using Yottasecond  = scale_unit_time_t<Second, 24>; 
    using Ronnasecond  = scale_unit_time_t<Second, 27>;
    using Quettasecond = scale_unit_time_t<Second, 30>;

    //Scaled versions of meter
    using Quectometer = scale_unit_length_t<Meter, -30>;
    using Rontometer  = scale_unit_length_t<Meter, -27>; 
    using Yoctometer  = scale_unit_length_t<Meter, -24>;
    using Zeptometer  = scale_unit_length_t<Meter, -21>;
    using Attometer   = scale_unit_length_t<Meter, -18>;
    using Femtometer  = scale_unit_length_t<Meter, -15>;
    using Picometer   = scale_unit_length_t<Meter, -12>;
    using Nanometer   = scale_unit_length_t<Meter, -9>;
    using Micrometer  = scale_unit_length_t<Meter, -6>;
    using Millimeter  = scale_unit_length_t<Meter, -3>;
    using Centimeter  = scale_unit_length_t<Meter, -2>;
    using Decimeter   = scale_unit_length_t<Meter, -1>;
    using Decameter   = scale_unit_length_t<Meter, 1>;
    using Hectometer  = scale_unit_length_t<Meter, 2>;
    using Kilometer   = scale_unit_length_t<Meter, 3>;
    using Megameter   = scale_unit_length_t<Meter, 6>;
    using Gigameter   = scale_unit_length_t<Meter, 9>;
    using Terameter   = scale_unit_length_t<Meter, 12>;
    using Petameter   = scale_unit_length_t<Meter, 15>;
    using Exameter    = scale_unit_length_t<Meter, 18>; 
    using Zettameter  = scale_unit_length_t<Meter, 21>;
    using Yottameter  = scale_unit_length_t<Meter, 24>; 
    using Ronnameter  = scale_unit_length_t<Meter, 27>;
    using Quettameter = scale_unit_length_t<Meter, 30>;

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