#ifndef UNIT_TYPES_HPP
#define UNIT_TYPES_HPP

#include "UnitCore.hpp"

namespace Maxwell
{
    // Scaled versions of second
    using QuectosecondUnit = scale_unit_time_t<SecondUnit, -30>;
    using RontosecondUnit  = scale_unit_time_t<SecondUnit, -27>; 
    using YoctosecondUnit  = scale_unit_time_t<SecondUnit, -24>;
    using ZeptosecondUnit  = scale_unit_time_t<SecondUnit, -21>;
    using AttosecondUnit   = scale_unit_time_t<SecondUnit, -18>;
    using FemtosecondUnit  = scale_unit_time_t<SecondUnit, -15>;
    using PicosecondUnit   = scale_unit_time_t<SecondUnit, -12>;
    using NanosecondUnit   = scale_unit_time_t<SecondUnit, -9>;
    using MicrosecondUnit  = scale_unit_time_t<SecondUnit, -6>;
    using MillisecondUnit  = scale_unit_time_t<SecondUnit, -3>;
    using CentisecondUnit  = scale_unit_time_t<SecondUnit, -2>;
    using DecisecondUnit   = scale_unit_time_t<SecondUnit, -1>;
    using DecasecondUnit   = scale_unit_time_t<SecondUnit, 1>;
    using HectosecondUnit  = scale_unit_time_t<SecondUnit, 2>;
    using KilosecondUnit   = scale_unit_time_t<SecondUnit, 3>;
    using MegasecondUnit   = scale_unit_time_t<SecondUnit, 6>;
    using GigasecondUnit   = scale_unit_time_t<SecondUnit, 9>;
    using TerasecondUnit   = scale_unit_time_t<SecondUnit, 12>;
    using PetasecondUnit   = scale_unit_time_t<SecondUnit, 15>;
    using ExasecondUnit    = scale_unit_time_t<SecondUnit, 18>; 
    using ZettasecondUnit  = scale_unit_time_t<SecondUnit, 21>;
    using YottasecondUnit  = scale_unit_time_t<SecondUnit, 24>; 
    using RonnasecondUnit  = scale_unit_time_t<SecondUnit, 27>;
    using QuettasecondUnit = scale_unit_time_t<SecondUnit, 30>;

    //Scaled versions of meter
    using QuectometerUnit = scale_unit_length_t<MeterUnit, -30>;
    using RontometerUnit  = scale_unit_length_t<MeterUnit, -27>; 
    using YoctometerUnit  = scale_unit_length_t<MeterUnit, -24>;
    using ZeptometerUnit  = scale_unit_length_t<MeterUnit, -21>;
    using AttometerUnit   = scale_unit_length_t<MeterUnit, -18>;
    using FemtometerUnit  = scale_unit_length_t<MeterUnit, -15>;
    using PicometerUnit   = scale_unit_length_t<MeterUnit, -12>;
    using NanometerUnit   = scale_unit_length_t<MeterUnit, -9>;
    using MicrometerUnit  = scale_unit_length_t<MeterUnit, -6>;
    using MillimeterUnit  = scale_unit_length_t<MeterUnit, -3>;
    using CentimeterUnit  = scale_unit_length_t<MeterUnit, -2>;
    using DecimeterUnit   = scale_unit_length_t<MeterUnit, -1>;
    using DecameterUnit   = scale_unit_length_t<MeterUnit, 1>;
    using HectometerUnit  = scale_unit_length_t<MeterUnit, 2>;
    using KilometerUnit   = scale_unit_length_t<MeterUnit, 3>;
    using MegameterUnit   = scale_unit_length_t<MeterUnit, 6>;
    using GigameterUnit   = scale_unit_length_t<MeterUnit, 9>;
    using TerameterUnit   = scale_unit_length_t<MeterUnit, 12>;
    using PetameterUnit   = scale_unit_length_t<MeterUnit, 15>;
    using ExameterUnit    = scale_unit_length_t<MeterUnit, 18>; 
    using ZettameterUnit  = scale_unit_length_t<MeterUnit, 21>;
    using YottameterUnit  = scale_unit_length_t<MeterUnit, 24>; 
    using RonnameterUnit  = scale_unit_length_t<MeterUnit, 27>;
    using QuettameterUnit = scale_unit_length_t<MeterUnit, 30>;

    using LiterUnit = decltype(MillimeterUnit{}*MeterUnit{}*MeterUnit{});
    using QuectoliterUnit = scale_unit_length_t<LiterUnit, -30>;
    using RontoliterUnit  = scale_unit_length_t<LiterUnit, -27>; 
    using YoctoliterUnit  = scale_unit_length_t<LiterUnit, -24>;
    using ZeptoliterUnit  = scale_unit_length_t<LiterUnit, -21>;
    using AttoliterUnit   = scale_unit_length_t<LiterUnit, -18>;
    using FemtoliterUnit  = scale_unit_length_t<LiterUnit, -15>;
    using PicoliterUnit   = scale_unit_length_t<LiterUnit, -12>;
    using NanoliterUnit   = scale_unit_length_t<LiterUnit, -9>;
    using MicroliterUnit  = scale_unit_length_t<LiterUnit, -6>;
    using MilliliterUnit  = scale_unit_length_t<LiterUnit, -3>;
    using CentiliterUnit  = scale_unit_length_t<LiterUnit, -2>;
    using DeciliterUnit   = scale_unit_length_t<LiterUnit, -1>;
    using DecaliterUnit   = scale_unit_length_t<LiterUnit, 1>;
    using HectoliterUnit  = scale_unit_length_t<LiterUnit, 2>;
    using KiloliterUnit   = scale_unit_length_t<LiterUnit, 3>;
    using MegaliterUnit   = scale_unit_length_t<LiterUnit, 6>;
    using GigaliterUnit   = scale_unit_length_t<LiterUnit, 9>;
    using TeraliterUnit   = scale_unit_length_t<LiterUnit, 12>;
    using PetaliterUnit   = scale_unit_length_t<LiterUnit, 15>;
    using ExaliterUnit    = scale_unit_length_t<LiterUnit, 18>; 
    using ZettaliterUnit  = scale_unit_length_t<LiterUnit, 21>;
    using YottaliterUnit  = scale_unit_length_t<LiterUnit, 24>; 
    using RonnaliterUnit  = scale_unit_length_t<LiterUnit, 27>;
    using QuettaliterUnit = scale_unit_length_t<LiterUnit, 30>;

    using QuectogramUnit = scale_unit_length_t<KilogramUnit, -33>;
    using RontogramUnit  = scale_unit_length_t<KilogramUnit, -30>; 
    using YoctogramUnit  = scale_unit_length_t<KilogramUnit, -27>;
    using ZeptogramUnit  = scale_unit_length_t<KilogramUnit, -24>;
    using AttogramUnit   = scale_unit_length_t<KilogramUnit, -21>;
    using FemtogramUnit  = scale_unit_length_t<KilogramUnit, -18>;
    using PicogramUnit   = scale_unit_length_t<KilogramUnit, -15>;
    using NanogramUnit   = scale_unit_length_t<KilogramUnit, -12>;
    using MicrogramUnit  = scale_unit_length_t<KilogramUnit, -9>;
    using MilligramUnit  = scale_unit_length_t<KilogramUnit, -6>;
    using CentigramUnit  = scale_unit_length_t<KilogramUnit, -5>;
    using DecigramUnit   = scale_unit_length_t<KilogramUnit, -4>;
    using GramUnit       = scale_unit_length_t<KilogramUnit, -3>;
    using DecagramUnit   = scale_unit_length_t<KilogramUnit, -2>;
    using HectogramUnit  = scale_unit_length_t<KilogramUnit, -1>;
    using MegagramUnit   = scale_unit_length_t<KilogramUnit, 3>;
    using GigagramUnit   = scale_unit_length_t<KilogramUnit, 6>;
    using TeragramUnit   = scale_unit_length_t<KilogramUnit, 9>;
    using PetagramUnit   = scale_unit_length_t<KilogramUnit, 12>;
    using ExagramUnit    = scale_unit_length_t<KilogramUnit, 15>; 
    using ZettagramUnit  = scale_unit_length_t<KilogramUnit, 18>;
    using YottagramUnit  = scale_unit_length_t<KilogramUnit, 21>; 
    using RonnagramUnit  = scale_unit_length_t<KilogramUnit, 24>;
    using QuettagramUnit = scale_unit_length_t<KilogramUnit, 37>;

    using NewtonUnit = decltype(KilogramUnit{}*MeterUnit{}/(SecondUnit{}*SecondUnit{}));
    using JouleUnit = decltype(NewtonUnit{}*MeterUnit{});
    using WattUnit = decltype(JouleUnit{}/SecondUnit{});
    using HertzUnit = decltype(Dimensionless_Unit{}/SecondUnit{});
    using PascalUnit = decltype(NewtonUnit{}/(MeterUnit{}*MeterUnit{}));
    using CoulumbUnit = decltype(SecondUnit{}*AmpereUnit{});
    using VoltUnit = decltype(JouleUnit{}/CoulumbUnit{});
    using FaradUnit = decltype(CoulumbUnit{}/VoltUnit{});
    using OhmUnit = decltype(VoltUnit{}/AmpereUnit{});
    using SiemensUnit = decltype(AmpereUnit{}/VoltUnit{});
    using WeberUnit = decltype(VoltUnit{}*SecondUnit{});
    using TeslaUnit = decltype(WeberUnit{}/(MeterUnit{}*MeterUnit{}));
    using HenryUnit = decltype(WeberUnit{}/AmpereUnit{}); 

    //using CelsiusUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit

    using RadianUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1>>;
    using DegreeUnit = Unit<NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, NullUnit, UnitBase<0, 1, 572'957'795'131, 10'000'000'000>>;
}

#endif