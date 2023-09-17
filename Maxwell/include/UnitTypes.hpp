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

    using Liter = decltype(Millimeter{}*Meter{}*Meter{});
    using Quectoliter = scale_unit_length_t<Liter, -30>;
    using Rontoliter  = scale_unit_length_t<Liter, -27>; 
    using Yoctoliter  = scale_unit_length_t<Liter, -24>;
    using Zeptoliter  = scale_unit_length_t<Liter, -21>;
    using Attoliter   = scale_unit_length_t<Liter, -18>;
    using Femtoliter  = scale_unit_length_t<Liter, -15>;
    using Picoliter   = scale_unit_length_t<Liter, -12>;
    using Nanoliter   = scale_unit_length_t<Liter, -9>;
    using Microliter  = scale_unit_length_t<Liter, -6>;
    using Milliliter  = scale_unit_length_t<Liter, -3>;
    using Centiliter  = scale_unit_length_t<Liter, -2>;
    using Deciliter   = scale_unit_length_t<Liter, -1>;
    using Decaliter   = scale_unit_length_t<Liter, 1>;
    using Hectoliter  = scale_unit_length_t<Liter, 2>;
    using Kiloliter   = scale_unit_length_t<Liter, 3>;
    using Megaliter   = scale_unit_length_t<Liter, 6>;
    using Gigaliter   = scale_unit_length_t<Liter, 9>;
    using Teraliter   = scale_unit_length_t<Liter, 12>;
    using Petaliter   = scale_unit_length_t<Liter, 15>;
    using Exaliter    = scale_unit_length_t<Liter, 18>; 
    using Zettaliter  = scale_unit_length_t<Liter, 21>;
    using Yottaliter  = scale_unit_length_t<Liter, 24>; 
    using Ronnaliter  = scale_unit_length_t<Liter, 27>;
    using Quettaliter = scale_unit_length_t<Liter, 30>;

    using Quectogram = scale_unit_length_t<Kilogram, -33>;
    using Rontogram  = scale_unit_length_t<Kilogram, -30>; 
    using Yoctogram  = scale_unit_length_t<Kilogram, -27>;
    using Zeptogram  = scale_unit_length_t<Kilogram, -24>;
    using Attogram   = scale_unit_length_t<Kilogram, -21>;
    using Femtogram  = scale_unit_length_t<Kilogram, -18>;
    using Picogram   = scale_unit_length_t<Kilogram, -15>;
    using Nanogram   = scale_unit_length_t<Kilogram, -12>;
    using Microgram  = scale_unit_length_t<Kilogram, -9>;
    using Milligram  = scale_unit_length_t<Kilogram, -6>;
    using Centigram  = scale_unit_length_t<Kilogram, -5>;
    using Decigram   = scale_unit_length_t<Kilogram, -4>;
    using Gram       = scale_unit_length_t<Kilogram, -3>;
    using Decagram   = scale_unit_length_t<Kilogram, -2>;
    using Hectogram  = scale_unit_length_t<Kilogram, -1>;
    using Megagram   = scale_unit_length_t<Kilogram, 3>;
    using Gigagram   = scale_unit_length_t<Kilogram, 6>;
    using Teragram   = scale_unit_length_t<Kilogram, 9>;
    using Petagram   = scale_unit_length_t<Kilogram, 12>;
    using Exagram    = scale_unit_length_t<Kilogram, 15>; 
    using Zettagram  = scale_unit_length_t<Kilogram, 18>;
    using Yottagram  = scale_unit_length_t<Kilogram, 21>; 
    using Ronnagram  = scale_unit_length_t<Kilogram, 24>;
    using Quettagram = scale_unit_length_t<Kilogram, 37>;

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

    using Radian = Dimensionless;
    using Degree = Meter;
}

#endif