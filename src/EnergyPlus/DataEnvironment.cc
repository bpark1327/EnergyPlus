// EnergyPlus, Copyright (c) 1996-2020, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// C++ Headers
#include <cmath>

// EnergyPlus Headers
#include <EnergyPlus/Data/EnergyPlusData.hh>
#include <EnergyPlus/DataEnvironment.hh>
#include <EnergyPlus/DataGlobals.hh>
#include <EnergyPlus/UtilityRoutines.hh>

namespace EnergyPlus {

namespace DataEnvironment {

    // MODULE INFORMATION:
    //       AUTHOR         Rick Strand, Dan Fisher, Linda Lawrie
    //       DATE WRITTEN   December 1997
    //       MODIFIED       November 1998, Fred Winkelmann
    //       MODIFIED       June 1999,June 2000, Linda Lawrie
    //       RE-ENGINEERED  na

    // PURPOSE OF THIS MODULE:
    // This data-only module is a repository for the variables that relate specifically
    // to the "environment" (i.e. current date data, tomorrow's date data, and
    // current weather variables)

    // MODULE VARIABLE DECLARATIONS:
    Real64 OutRelHum;                         // Current outdoor relative humidity [%]
    Real64 OutRelHumValue;                    // Current outdoor relative humidity value [0.0-1.0]
    bool EMSOutRelHumOverrideOn(false);       // EMS flag for outdoor relative humidity value
    Real64 EMSOutRelHumOverrideValue;         // EMS override value for outdoor relative humidity value
    Real64 OutEnthalpy;                       // Current outdoor enthalpy
    Real64 OutAirDensity;                     // Current outdoor air density
    Real64 OutWetBulbTemp;                    // Current outdoor air wet bulb temperature
    Real64 OutDewPointTemp;                   // Current outdoor dewpoint temperature
    bool EMSOutDewPointTempOverrideOn(false); // EMS flag for outdoor dewpoint temperature
    Real64 EMSOutDewPointTempOverrideValue;   // EMS override value for outdoor dewpoint temperature
    Real64 SkyTemp;                           // Current sky temperature {C}
    Real64 SkyTempKelvin;                     // Current sky temperature {K}
    Real64 LiquidPrecipitation;               // Current liquid precipitation amount (rain) {m}
    bool SunIsUp;                             // True when Sun is over horizon, False when not
    Real64 WindDir;                           // Current outdoor air wind direction
    bool EMSWindDirOverrideOn(false);         // EMS flag for outdoor air wind direction
    Real64 EMSWindDirOverrideValue;           // EMS override value for outdoor air wind direction
    Real64 WindSpeed;                         // Current outdoor air wind speed
    bool EMSWindSpeedOverrideOn(false);       // EMS flag for outdoor air wind speed
    Real64 EMSWindSpeedOverrideValue;         // EMS override value for outdoor air wind speed
    Real64 WaterMainsTemp;                    // Current water mains temperature
    int Year;                                 // Current calendar year of the simulation from the weather file
    int YearTomorrow;                         // Tomorrow's calendar year of the simulation
    Array1D<Real64> SOLCOS(3);                // Solar direction cosines at current time step
    Real64 CloudFraction;                     // Fraction of sky covered by clouds
    Real64 HISKF;                             // Exterior horizontal illuminance from sky (lux).
    Real64 HISUNF;                            // Exterior horizontal beam illuminance (lux)
    Real64 HISUNFnorm;                        // Exterior beam normal illuminance (lux)
    Real64 PDIRLW;                            // Luminous efficacy (lum/W) of beam solar radiation
    Real64 PDIFLW;                            // Luminous efficacy (lum/W) of sky diffuse solar radiation
    Real64 SkyClearness;                      // Sky clearness (see subr. DayltgLuminousEfficacy)
    Real64 SkyBrightness;                     // Sky brightness (see subr. DayltgLuminousEfficacy)
    Real64 TotalCloudCover(5.0);              // Total Sky Cover (tenth of sky)
    Real64 OpaqueCloudCover(5.0);             // Opaque Sky Cover (tenth of sky)
    Real64 StdBaroPress(DataEnvironment::StdPressureSeaLevel); // Standard "atmospheric pressure" based on elevation (ASHRAE HOF p6.1)
    Real64 StdRhoAir;                         // Standard "rho air" set in WeatherManager - based on StdBaroPress
    Real64 rhoAirSTP;                         // Standard density of dry air at 101325 Pa, 20.0C temperaure
    Real64 TimeZoneNumber;                    // Time Zone Number of building location
    Real64 TimeZoneMeridian;                  // Standard Meridian of TimeZone
    std::string EnvironmentName;              // Current environment name (longer for weather file names)
    std::string WeatherFileLocationTitle;     // Location Title from Weather File
    std::string CurMnDyHr;                    // Current Month/Day/Hour timestamp info
    std::string CurMnDy;                      // Current Month/Day timestamp info
    std::string CurMnDyYr;                    // Current Month/Day/Year timestamp info
    int CurEnvirNum;                          // current environment number
    int TotDesDays(0);                        // Total number of Design days to Setup
    int TotRunDesPersDays(0);                 // Total number of Run Design Periods [Days] (Weather data) to Setup
    int CurrentOverallSimDay;                 // Count of current simulation day in total of all sim days
    int TotalOverallSimDays;                  // Count of all possible simulation days in all environments
    int MaxNumberSimYears;                    // Maximum number of simulation years requested in all RunPeriod statements
    int RunPeriodStartDayOfWeek;              // Day of week of the first day of the run period. (or design day - day of week)

    Real64 CosSolarDeclinAngle; // Cosine of the solar declination angle
    Real64 EquationOfTime;      // Value of the equation of time formula
    Real64 SinLatitude;         // Sine of Latitude
    Real64 CosLatitude;         // Cosine of Latitude
    Real64 SinSolarDeclinAngle; // Sine of the solar declination angle
    Real64 TS1TimeOffset(-0.5); // offset when TS=1 for solar calculations

    Real64 WeatherFileWindModCoeff(1.5863); // =(WindBLHeight/WindSensorHeight)**WindExp for conditions at the weather station
    Real64 WeatherFileTempModCoeff(0.0);    // =AtmosphericTempGradient*EarthRadius*SensorHeight/(EarthRadius+SensorHeight)

    Real64 SiteWindExp(0.22);        // Exponent for the wind velocity profile at the site
    Real64 SiteWindBLHeight(370.0);  // Boundary layer height for the wind velocity profile at the site (m)
    Real64 SiteTempGradient(0.0065); // Air temperature gradient coefficient (K/m)

    bool GroundTempObjInput(false);         // Ground temperature object input
    bool GroundTemp_SurfaceObjInput(false); // Surface ground temperature object input
    bool GroundTemp_DeepObjInput(false);    // Deep ground temperature object input
    bool FCGroundTemps(false);
    bool DisplayWeatherMissingDataWarnings(false); // Display missing/out of range weather warnings
    bool IgnoreSolarRadiation(false);              // TRUE if all solar radiation is to be ignored
    bool IgnoreBeamRadiation(false);               // TRUE if beam (aka direct normal) radiation is to be ignored
    bool IgnoreDiffuseRadiation(false);            // TRUE if diffuse horizontal radiation is to be ignored

    bool PrintEnvrnStampWarmup(false);
    bool PrintEnvrnStampWarmupPrinted(false);

    bool RunPeriodEnvironment(false);  // True if Run Period, False if DesignDay
    std::string EnvironmentStartEnd;   // Start/End dates for Environment
    bool CurrentYearIsLeapYear(false); // true when current year is leap year (convoluted logic dealing with
    // whether weather file allows leap years, runperiod inputs.

    int varyingLocationSchedIndexLat(0);
    int varyingLocationSchedIndexLong(0);
    int varyingOrientationSchedIndex(0);

    // for PerformancePrecisionTradeoffs
    bool forceBeginEnvResetSuppress(false);

    void clear_state()
    {
        OutRelHum = Real64();
        OutRelHumValue = Real64();
        EMSOutRelHumOverrideOn = false;
        EMSOutRelHumOverrideValue = Real64();
        OutEnthalpy = Real64();
        OutAirDensity = Real64();
        OutWetBulbTemp = Real64();
        OutDewPointTemp = Real64();
        EMSOutDewPointTempOverrideOn = false;
        EMSOutDewPointTempOverrideValue = Real64();
        SkyTemp = Real64();
        SkyTempKelvin = Real64();
        LiquidPrecipitation = Real64();
        SunIsUp = bool();
        WindDir = Real64();
        EMSWindDirOverrideOn = false;
        EMSWindDirOverrideValue = Real64();
        WindSpeed = Real64();
        EMSWindSpeedOverrideOn = false;
        EMSWindSpeedOverrideValue = Real64();
        WaterMainsTemp = Real64();
        Year = int();
        YearTomorrow = int();
        SOLCOS.dimension(3);
        CloudFraction = Real64();
        HISKF = Real64();
        HISUNF = Real64();
        HISUNFnorm = Real64();
        PDIRLW = Real64();
        PDIFLW = Real64();
        SkyClearness = Real64();
        SkyBrightness = Real64();
        TotalCloudCover = 0.0;
        OpaqueCloudCover = 0.0;
        StdBaroPress = 101325.0;
        StdRhoAir = Real64();
        TimeZoneNumber = Real64();
        TimeZoneMeridian = Real64();
        EnvironmentName = std::string();
        WeatherFileLocationTitle = std::string();
        CurMnDyHr = std::string();
        CurMnDy = std::string();
        CurMnDyYr = std::string();
        CurEnvirNum = int();
        TotDesDays = 0;
        TotRunDesPersDays = 0;
        CurrentOverallSimDay = int();
        TotalOverallSimDays = int();
        MaxNumberSimYears = int();
        RunPeriodStartDayOfWeek = int();
        CosSolarDeclinAngle = Real64();
        EquationOfTime = Real64();
        SinLatitude = Real64();
        CosLatitude = Real64();
        SinSolarDeclinAngle = Real64();
        TS1TimeOffset = -0.5;
        WeatherFileWindModCoeff = 1.5863;
        WeatherFileTempModCoeff = 0.0;
        SiteWindExp = 0.22;
        SiteWindBLHeight = 370.0;
        SiteTempGradient = 0.0065;
        GroundTempObjInput = false;
        GroundTemp_SurfaceObjInput = false;
        GroundTemp_DeepObjInput = false;
        FCGroundTemps = false;
        DisplayWeatherMissingDataWarnings = false;
        IgnoreSolarRadiation = false;
        IgnoreBeamRadiation = false;
        IgnoreDiffuseRadiation = false;
        PrintEnvrnStampWarmup = false;
        PrintEnvrnStampWarmupPrinted = false;
        RunPeriodEnvironment = false;
        EnvironmentStartEnd = std::string();
        CurrentYearIsLeapYear = false;
        varyingLocationSchedIndexLat = 0;
        varyingLocationSchedIndexLong = 0;
        varyingOrientationSchedIndex = 0;
    }

    Real64 OutDryBulbTempAt(EnergyPlusData &state, Real64 const Z) // Height above ground (m)
    {

        // FUNCTION INFORMATION:
        //       AUTHOR         Peter Graham Ellis
        //       DATE WRITTEN   January 2006
        //       MODIFIED       na
        //       RE-ENGINEERED  na

        // PURPOSE OF THIS FUNCTION:
        // Calculates outdoor dry bulb temperature at a given altitude.

        // METHODOLOGY EMPLOYED:
        // 1976 U.S. Standard Atmosphere.

        // REFERENCES:
        // 1976 U.S. Standard Atmosphere. 1976. U.S. Government Printing Office, Washington, D.C.

        // Using/Aliasing

        // Return value
        Real64 LocalOutDryBulbTemp; // Return result for function (C)

        // Locals
        // FUNCTION ARGUMENT DEFINITIONS:

        // FUNCTION LOCAL VARIABLE DECLARATIONS:
        Real64 BaseTemp; // Base temperature at Z = 0 (C)

        BaseTemp = state.dataEnvrn->OutDryBulbTemp + WeatherFileTempModCoeff;

        if (SiteTempGradient == 0.0) {
            LocalOutDryBulbTemp = state.dataEnvrn->OutDryBulbTemp;
        } else if (Z <= 0.0) {
            LocalOutDryBulbTemp = BaseTemp;
        } else {
            LocalOutDryBulbTemp = BaseTemp - SiteTempGradient * DataEnvironment::EarthRadius * Z / (DataEnvironment::EarthRadius + Z);
        }

        if (LocalOutDryBulbTemp < -100.0) {
            ShowSevereError(state, "OutDryBulbTempAt: outdoor drybulb temperature < -100 C");
            ShowContinueError(state, format("...check heights, this height=[{:.0R}].", Z));
            ShowFatalError(state, "Program terminates due to preceding condition(s).");
        }

        return LocalOutDryBulbTemp;
    }

    Real64 OutWetBulbTempAt(EnergyPlusData &state, Real64 const Z) // Height above ground (m)
    {

        // FUNCTION INFORMATION:
        //       AUTHOR         Peter Graham Ellis
        //       DATE WRITTEN   January 2006
        //       MODIFIED       na
        //       RE-ENGINEERED  na

        // PURPOSE OF THIS FUNCTION:
        // Calculates outdoor wet bulb temperature at a given altitude.

        // METHODOLOGY EMPLOYED:
        // 1976 U.S. Standard Atmosphere.

        // REFERENCES:
        // 1976 U.S. Standard Atmosphere. 1976. U.S. Government Printing Office, Washington, D.C.

        // Return value
        Real64 LocalOutWetBulbTemp; // Return result for function (C)

        // FUNCTION LOCAL VARIABLE DECLARATIONS:
        Real64 BaseTemp; // Base temperature at Z = 0 (C)

        BaseTemp = OutWetBulbTemp + WeatherFileTempModCoeff;

        if (SiteTempGradient == 0.0) {
            LocalOutWetBulbTemp = OutWetBulbTemp;
        } else if (Z <= 0.0) {
            LocalOutWetBulbTemp = BaseTemp;
        } else {
            LocalOutWetBulbTemp = BaseTemp - SiteTempGradient * DataEnvironment::EarthRadius * Z / (DataEnvironment::EarthRadius + Z);
        }

        if (LocalOutWetBulbTemp < -100.0) {
            ShowSevereError(state, "OutWetBulbTempAt: outdoor wetbulb temperature < -100 C");
            ShowContinueError(state, format("...check heights, this height=[{:.0R}].", Z));
            ShowFatalError(state, "Program terminates due to preceding condition(s).");
        }

        return LocalOutWetBulbTemp;
    }

    Real64 OutDewPointTempAt(EnergyPlusData &state, Real64 const Z) // Height above ground (m)
    {

        // FUNCTION INFORMATION:
        //       AUTHOR         Linda Lawrie
        //       DATE WRITTEN   March 2007
        //       MODIFIED       na
        //       RE-ENGINEERED  na

        // PURPOSE OF THIS FUNCTION:
        // Calculates outdoor dew point temperature at a given altitude.

        // METHODOLOGY EMPLOYED:
        // 1976 U.S. Standard Atmosphere.
        // copied from outwetbulbtempat

        // REFERENCES:
        // 1976 U.S. Standard Atmosphere. 1976. U.S. Government Printing Office, Washington, D.C.

        // Return value
        Real64 LocalOutDewPointTemp; // Return result for function (C)

        // FUNCTION LOCAL VARIABLE DECLARATIONS:
        Real64 BaseTemp; // Base temperature at Z = 0 (C)

        BaseTemp = OutDewPointTemp + WeatherFileTempModCoeff;

        if (SiteTempGradient == 0.0) {
            LocalOutDewPointTemp = OutDewPointTemp;
        } else if (Z <= 0.0) {
            LocalOutDewPointTemp = BaseTemp;
        } else {
            LocalOutDewPointTemp = BaseTemp - SiteTempGradient * DataEnvironment::EarthRadius * Z / (DataEnvironment::EarthRadius + Z);
        }

        if (LocalOutDewPointTemp < -100.0) {
            ShowSevereError(state, "OutDewPointTempAt: outdoor dewpoint temperature < -100 C");
            ShowContinueError(state, format("...check heights, this height=[{:.0R}].", Z));
            ShowFatalError(state, "Program terminates due to preceding condition(s).");
        }

        return LocalOutDewPointTemp;
    }

    Real64 WindSpeedAt(Real64 const Z) // Height above ground (m)
    {

        // FUNCTION INFORMATION:
        //       AUTHOR         Peter Graham Ellis
        //       DATE WRITTEN   January 2006
        //       MODIFIED       na
        //       RE-ENGINEERED  na

        // PURPOSE OF THIS FUNCTION:
        // Calculates local wind speed at a given altitude.

        // METHODOLOGY EMPLOYED:
        // 2005 ASHRAE Fundamentals, Chapter 16, Equation 4.  (Different depending on terrain).

        // REFERENCES:
        // 2005 ASHRAE Fundamentals, Chapter 16, Equation 4.  (Different depending on terrain).
        // Terrain variables are set in HeatBalanceManager or entered by the user.

        // Return value
        Real64 LocalWindSpeed; // Return result for function (m/s)

        if (Z <= 0.0) {
            LocalWindSpeed = 0.0;
        } else if (SiteWindExp == 0.0) {
            LocalWindSpeed = WindSpeed;
        } else {
            //  [Met] - at meterological Station, Height of measurement is usually 10m above ground
            //  LocalWindSpeed = Windspeed [Met] * (Wind Boundary LayerThickness [Met]/Height [Met])**Wind Exponent[Met] &
            //                     * (Height above ground / Site Wind Boundary Layer Thickness) ** Site Wind Exponent
            LocalWindSpeed = WindSpeed * WeatherFileWindModCoeff * std::pow(Z / SiteWindBLHeight, SiteWindExp);
        }

        return LocalWindSpeed;
    }

    Real64 OutBaroPressAt(EnergyPlusData &state, Real64 const Z) // Height above ground (m)
    {

        // FUNCTION INFORMATION:
        //       AUTHOR         Daeho Kang
        //       DATE WRITTEN   August 2009
        //       MODIFIED       na
        //       RE-ENGINEERED  na

        // PURPOSE OF THIS FUNCTION:
        // Calculates local air barometric pressure at a given altitude.

        // METHODOLOGY EMPLOYED:
        // U.S. Standard Atmosphere1976, Part 1, Chapter 1.3, Equation 33b.

        // REFERENCES:
        // U.S. Standard Atmosphere1976, Part 1, Chapter 1.3, Equation 33b.

        // Return value
        Real64 LocalAirPressure; // Return result for function (Pa)

        // FNCTION PARAMETER DEFINITIONS:
        Real64 constexpr StdGravity(9.80665);    // The acceleration of gravity at the sea level (m/s2)
        Real64 constexpr AirMolarMass(0.028964); // Molar mass of Earth's air (kg/mol)
        Real64 constexpr GasConstant(8.31432);   // Molar gas constant (J/Mol-K)
        Real64 constexpr TempGradient(-0.0065);  // Molecular-scale temperature gradient (K/m)
        Real64 constexpr GeopotentialH(0.0);     // Geopotential height (zero within 11km from the sea level) (m)

        // FUNCTION LOCAL VARIABLE DECLARATIONS:
        Real64 BaseTemp; // Base temperature at Z

        BaseTemp = OutDryBulbTempAt(state, Z) + DataGlobalConstants::KelvinConv();

        if (Z <= 0.0) {
            LocalAirPressure = 0.0;
        } else if (SiteTempGradient == 0.0) {
            LocalAirPressure = state.dataEnvrn->OutBaroPress;
        } else {
            LocalAirPressure = StdBaroPress * std::pow(BaseTemp / (BaseTemp + TempGradient * (Z - GeopotentialH)),
                                                       (StdGravity * AirMolarMass) / (GasConstant * TempGradient));
        }

        return LocalAirPressure;
    }

    void SetOutBulbTempAt_error(EnergyPlusData &state, std::string const &Settings, Real64 const max_height, std::string const &SettingsName)
    {
        // Using/Aliasing

        ShowSevereError(state, "SetOutBulbTempAt: " + Settings + " Outdoor Temperatures < -100 C");
        ShowContinueError(state, format("...check {} Heights - Maximum {} Height=[{:.0R}].", Settings, Settings, max_height));
        if (max_height >= 20000.0) {
            ShowContinueError(state, "...according to your maximum Z height, your building is somewhere in the Stratosphere.");
            ShowContinueError(state, "...look at " + Settings + " Name= " + SettingsName);
        }
        ShowFatalError(state, "Program terminates due to preceding condition(s).");
    }

    void
    SetWindSpeedAt(int const NumItems, const Array1D<Real64> &Heights, Array1D<Real64> &LocalWindSpeed, [[maybe_unused]] std::string const &Settings)
    {

        // SUBROUTINE INFORMATION:
        //       AUTHOR         Linda Lawrie
        //       DATE WRITTEN   June 2013
        //       MODIFIED       na
        //       RE-ENGINEERED  na

        // PURPOSE OF THIS SUBROUTINE:
        // Routine provides facility for doing bulk Set Windspeed at Height.

        if (SiteWindExp == 0.0) {
            LocalWindSpeed = WindSpeed;
        } else {
            Real64 const fac(WindSpeed * WeatherFileWindModCoeff * std::pow(SiteWindBLHeight, -SiteWindExp));
            Real64 Z; // Centroid value
            for (int i = 1; i <= NumItems; ++i) {
                Z = Heights(i);
                if (Z <= 0.0) {
                    LocalWindSpeed(i) = 0.0;
                } else {
                    //  [Met] - at meterological Station, Height of measurement is usually 10m above ground
                    //  LocalWindSpeed = Windspeed [Met] * (Wind Boundary LayerThickness [Met]/Height [Met])**Wind Exponent[Met] &
                    //                     * (Height above ground / Site Wind Boundary Layer Thickness) ** Site Wind Exponent
                    LocalWindSpeed(i) = fac * std::pow(Z, SiteWindExp);
                }
            }
        }
    }

} // namespace DataEnvironment

} // namespace EnergyPlus
