#ifndef BASE_UNIT_SCALES_HPP
#define BASE_UNIT_SCALES_HPP

#include "UnitCore.hpp"

namespace Maxwell
{
     // Scaled versions of second
    /// Unit representing 1 quectosecond (1*10^-30 sec)
    using QuectosecondUnit = scale_unit_time_t<SecondUnit, -30>;
    /// Unit representing 1 rontosecond (1*10^-27 sec)
    using RontosecondUnit  = scale_unit_time_t<SecondUnit, -27>; 
    /// Unit representing 1 yoctosecond (1*10^-24 sec)
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

    // Scaled version liter
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

    // Scaled version of gram
    using QuectogramUnit = scale_unit_mass_t<KilogramUnit, -33>;
    using RontogramUnit  = scale_unit_mass_t<KilogramUnit, -30>; 
    using YoctogramUnit  = scale_unit_mass_t<KilogramUnit, -27>;
    using ZeptogramUnit  = scale_unit_mass_t<KilogramUnit, -24>;
    using AttogramUnit   = scale_unit_mass_t<KilogramUnit, -21>;
    using FemtogramUnit  = scale_unit_mass_t<KilogramUnit, -18>;
    using PicogramUnit   = scale_unit_mass_t<KilogramUnit, -15>;
    using NanogramUnit   = scale_unit_mass_t<KilogramUnit, -12>;
    using MicrogramUnit  = scale_unit_mass_t<KilogramUnit, -9>;
    using MilligramUnit  = scale_unit_mass_t<KilogramUnit, -6>;
    using CentigramUnit  = scale_unit_mass_t<KilogramUnit, -5>;
    using DecigramUnit   = scale_unit_mass_t<KilogramUnit, -4>;
    using GramUnit       = scale_unit_mass_t<KilogramUnit, -3>;
    using DecagramUnit   = scale_unit_mass_t<KilogramUnit, -2>;
    using HectogramUnit  = scale_unit_mass_t<KilogramUnit, -1>;
    using MegagramUnit   = scale_unit_mass_t<KilogramUnit, 3>;
    using GigagramUnit   = scale_unit_mass_t<KilogramUnit, 6>;
    using TeragramUnit   = scale_unit_mass_t<KilogramUnit, 9>;
    using PetagramUnit   = scale_unit_mass_t<KilogramUnit, 12>;
    using ExagramUnit    = scale_unit_mass_t<KilogramUnit, 15>; 
    using ZettagramUnit  = scale_unit_mass_t<KilogramUnit, 18>;
    using YottagramUnit  = scale_unit_mass_t<KilogramUnit, 21>; 
    using RonnagramUnit  = scale_unit_mass_t<KilogramUnit, 24>;
    using QuettagramUnit = scale_unit_mass_t<KilogramUnit, 27>;

    // Scaled version of Ampere
    using QuectoampereUnit = scale_unit_current_t<AmpereUnit, -30>;
    using RontoampereUnit  = scale_unit_current_t<AmpereUnit, -27>; 
    using YoctoampereUnit  = scale_unit_current_t<AmpereUnit, -24>;
    using ZeptoampereUnit  = scale_unit_current_t<AmpereUnit, -21>;
    using AttoampereUnit   = scale_unit_current_t<AmpereUnit, -18>;
    using FemtoampereUnit  = scale_unit_current_t<AmpereUnit, -15>;
    using PicoampereUnit   = scale_unit_current_t<AmpereUnit, -12>;
    using NanoampereUnit   = scale_unit_current_t<AmpereUnit, -9>;
    using MicroampereUnit  = scale_unit_current_t<AmpereUnit, -6>;
    using MilliampereUnit  = scale_unit_current_t<AmpereUnit, -3>;
    using CentiampereUnit  = scale_unit_current_t<AmpereUnit, -2>;
    using DeciampereUnit   = scale_unit_current_t<AmpereUnit, -1>;
    using DecaampereUnit   = scale_unit_current_t<AmpereUnit, 1>;
    using HectoampereUnit  = scale_unit_current_t<AmpereUnit, 2>;
    using KiloampereUnit   = scale_unit_current_t<AmpereUnit, 3>;
    using MegaampereUnit   = scale_unit_current_t<AmpereUnit, 6>;
    using GigaampereUnit   = scale_unit_current_t<AmpereUnit, 9>;
    using TeraampereUnit   = scale_unit_current_t<AmpereUnit, 12>;
    using PetaampereUnit   = scale_unit_current_t<AmpereUnit, 15>;
    using ExaampereUnit    = scale_unit_current_t<AmpereUnit, 18>; 
    using ZettaampereUnit  = scale_unit_current_t<AmpereUnit, 21>;
    using YottaampereUnit  = scale_unit_current_t<AmpereUnit, 24>; 
    using RonnaampereUnit  = scale_unit_current_t<AmpereUnit, 27>;
    using QuettaampereUnit = scale_unit_current_t<AmpereUnit, 30>;

    // Scaled verion of Kelvin
    using QuectokelvinUnit = scale_unit_temperature_t<KelvinUnit, -30>;
    using RontokelvinUnit  = scale_unit_temperature_t<KelvinUnit, -27>; 
    using YoctokelvinUnit  = scale_unit_temperature_t<KelvinUnit, -24>;
    using ZeptokelvinUnit  = scale_unit_temperature_t<KelvinUnit, -21>;
    using AttokelvinUnit   = scale_unit_temperature_t<KelvinUnit, -18>;
    using FemtokelvinUnit  = scale_unit_temperature_t<KelvinUnit, -15>;
    using PicokelvinUnit   = scale_unit_temperature_t<KelvinUnit, -12>;
    using NanokelvinUnit   = scale_unit_temperature_t<KelvinUnit, -9>;
    using MicrokelvinUnit  = scale_unit_temperature_t<KelvinUnit, -6>;
    using MillikelvinUnit  = scale_unit_temperature_t<KelvinUnit, -3>;
    using CentikelvinUnit  = scale_unit_temperature_t<KelvinUnit, -2>;
    using DecikelvinUnit   = scale_unit_temperature_t<KelvinUnit, -1>;
    using DecakelvinUnit   = scale_unit_temperature_t<KelvinUnit, 1>;
    using HectokelvinUnit  = scale_unit_temperature_t<KelvinUnit, 2>;
    using KilokelvinUnit   = scale_unit_temperature_t<KelvinUnit, 3>;
    using MegakelvinUnit   = scale_unit_temperature_t<KelvinUnit, 6>;
    using GigakelvinUnit   = scale_unit_temperature_t<KelvinUnit, 9>;
    using TerakelvinUnit   = scale_unit_temperature_t<KelvinUnit, 12>;
    using PetakelvinUnit   = scale_unit_temperature_t<KelvinUnit, 15>;
    using ExakelvinUnit    = scale_unit_temperature_t<KelvinUnit, 18>; 
    using ZettakelvinUnit  = scale_unit_temperature_t<KelvinUnit, 21>;
    using YottakelvinUnit  = scale_unit_temperature_t<KelvinUnit, 24>; 
    using RonnakelvinUnit  = scale_unit_temperature_t<KelvinUnit, 27>;
    using QuettakelvinUnit = scale_unit_temperature_t<KelvinUnit, 30>;

    // Scaled version of mole
    using QuectomoleUnit = scale_unit_amount_t<MoleUnit, -30>;
    using RontomoleUnit  = scale_unit_amount_t<MoleUnit, -27>; 
    using YoctomoleUnit  = scale_unit_amount_t<MoleUnit, -24>;
    using ZeptomoleUnit  = scale_unit_amount_t<MoleUnit, -21>;
    using AttomoleUnit   = scale_unit_amount_t<MoleUnit, -18>;
    using FemtomoleUnit  = scale_unit_amount_t<MoleUnit, -15>;
    using PicomoleUnit   = scale_unit_amount_t<MoleUnit, -12>;
    using NanomoleUnit   = scale_unit_amount_t<MoleUnit, -9>;
    using MicromoleUnit  = scale_unit_amount_t<MoleUnit, -6>;
    using MillimoleUnit  = scale_unit_amount_t<MoleUnit, -3>;
    using CentimoleUnit  = scale_unit_amount_t<MoleUnit, -2>;
    using DecimoleUnit   = scale_unit_amount_t<MoleUnit, -1>;
    using DecamoleUnit   = scale_unit_amount_t<MoleUnit, 1>;
    using HectomoleUnit  = scale_unit_amount_t<MoleUnit, 2>;
    using KilomoleUnit   = scale_unit_amount_t<MoleUnit, 3>;
    using MegamoleUnit   = scale_unit_amount_t<MoleUnit, 6>;
    using GigamoleUnit   = scale_unit_amount_t<MoleUnit, 9>;
    using TeramoleUnit   = scale_unit_amount_t<MoleUnit, 12>;
    using PetamoleUnit   = scale_unit_amount_t<MoleUnit, 15>;
    using ExamoleUnit    = scale_unit_amount_t<MoleUnit, 18>; 
    using ZettamoleUnit  = scale_unit_amount_t<MoleUnit, 21>;
    using YottamoleUnit  = scale_unit_amount_t<MoleUnit, 24>; 
    using RonnamoleUnit  = scale_unit_amount_t<MoleUnit, 27>;
    using QuettamoleUnit = scale_unit_amount_t<MoleUnit, 30>;

    // Scaled version of candel
    using QuectocandelaUnit = scale_unit_luminosity_t<CandelaUnit, -30>;
    using RontocandelaUnit  = scale_unit_luminosity_t<CandelaUnit, -27>; 
    using YoctocandelaUnit  = scale_unit_luminosity_t<CandelaUnit, -24>;
    using ZeptocandelaUnit  = scale_unit_luminosity_t<CandelaUnit, -21>;
    using AttocandelaUnit   = scale_unit_luminosity_t<CandelaUnit, -18>;
    using FemtocandelaUnit  = scale_unit_luminosity_t<CandelaUnit, -15>;
    using PicocandelaUnit   = scale_unit_luminosity_t<CandelaUnit, -12>;
    using NanocandelaUnit   = scale_unit_luminosity_t<CandelaUnit, -9>;
    using MicrocandelaUnit  = scale_unit_luminosity_t<CandelaUnit, -6>;
    using MillicandelaUnit  = scale_unit_luminosity_t<CandelaUnit, -3>;
    using CenticandelaUnit  = scale_unit_luminosity_t<CandelaUnit, -2>;
    using DecicandelaUnit   = scale_unit_luminosity_t<CandelaUnit, -1>;
    using DecacandelaUnit   = scale_unit_luminosity_t<CandelaUnit, 1>;
    using HectocandelaUnit  = scale_unit_luminosity_t<CandelaUnit, 2>;
    using KilocandelaUnit   = scale_unit_luminosity_t<CandelaUnit, 3>;
    using MegacandelaUnit   = scale_unit_luminosity_t<CandelaUnit, 6>;
    using GigacandelaUnit   = scale_unit_luminosity_t<CandelaUnit, 9>;
    using TeracandelaUnit   = scale_unit_luminosity_t<CandelaUnit, 12>;
    using PetacandelaUnit   = scale_unit_luminosity_t<CandelaUnit, 15>;
    using ExacandelaUnit    = scale_unit_luminosity_t<CandelaUnit, 18>; 
    using ZettacandelaUnit  = scale_unit_luminosity_t<CandelaUnit, 21>;
    using YottacandelaUnit  = scale_unit_luminosity_t<CandelaUnit, 24>; 
    using RonnacandelaUnit  = scale_unit_luminosity_t<CandelaUnit, 27>;
    using QuettacandelaUnit = scale_unit_luminosity_t<CandelaUnit, 30>;

    using QuectoradianUnit = scale_unit_angle_t<RadianUnit, -30>;
    using RontoradianUnit  = scale_unit_angle_t<RadianUnit, -27>; 
    using YoctoradianUnit  = scale_unit_angle_t<RadianUnit, -24>;
    using ZeptoradianUnit  = scale_unit_angle_t<RadianUnit, -21>;
    using AttoradianUnit   = scale_unit_angle_t<RadianUnit, -18>;
    using FemtoradianUnit  = scale_unit_angle_t<RadianUnit, -15>;
    using PicoradianUnit   = scale_unit_angle_t<RadianUnit, -12>;
    using NanoradianUnit   = scale_unit_angle_t<RadianUnit, -9>;
    using MicroradianUnit  = scale_unit_angle_t<RadianUnit, -6>;
    using MilliradianUnit  = scale_unit_angle_t<RadianUnit, -3>;
    using CentiradianUnit  = scale_unit_angle_t<RadianUnit, -2>;
    using DeciradianUnit   = scale_unit_angle_t<RadianUnit, -1>;
    using DecaradianUnit   = scale_unit_angle_t<RadianUnit, 1>;
    using HectoradianUnit  = scale_unit_angle_t<RadianUnit, 2>;
    using KiloradianUnit   = scale_unit_angle_t<RadianUnit, 3>;
    using MegaradianUnit   = scale_unit_angle_t<RadianUnit, 6>;
    using GigaradianUnit   = scale_unit_angle_t<RadianUnit, 9>;
    using TeraradianUnit   = scale_unit_angle_t<RadianUnit, 12>;
    using PetaradianUnit   = scale_unit_angle_t<RadianUnit, 15>;
    using ExaradianUnit    = scale_unit_angle_t<RadianUnit, 18>; 
    using ZettaradianUnit  = scale_unit_angle_t<RadianUnit, 21>;
    using YottaradianUnit  = scale_unit_angle_t<RadianUnit, 24>; 
    using RonnaradianUnit  = scale_unit_angle_t<RadianUnit, 27>;
    using QuettaradianUnit = scale_unit_angle_t<RadianUnit, 30>;
}

#endif