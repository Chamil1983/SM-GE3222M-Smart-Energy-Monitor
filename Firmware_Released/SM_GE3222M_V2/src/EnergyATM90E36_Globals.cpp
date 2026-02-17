// EnergyATM90E36_Globals.cpp
// Provides global calibration/config symbols expected by the original V1 EnergyATM90E36.cpp driver.
// In the original firmware these lived across Settings.ino + config.cpp. In this refactor build,
// we define safe defaults here, and runtime code may override them later via CalibrationManager.

#include <Arduino.h>

// These are consumed by ATM90E3x::InitEnergy() in EnergyATM90E36.cpp
unsigned int MMode0_Reg   = 389;     // line frequency setting (0x0185) - 50Hz preset used in original project
unsigned int MMode1_Reg   = 0x557F;  // PGA gain preset used in original project
unsigned int PMPGA_Reg    = 0;       // not used in current init flow
unsigned int PStartTh_Reg = 0;
unsigned int QStartTh_Reg = 0;
unsigned int SStartTh_Reg = 0;
unsigned int PPhaseTh_Reg = 0;
unsigned int QPhaseTh_Reg = 0;
unsigned int SPhaseTh_Reg = 0;

// Arrays consumed by InitEnergy()
unsigned short CalRegisters[13]     = {0};
unsigned short HarCalRegisters[7]   = {0};
unsigned short MeasurCalRegisters[15] = {
    // Matches the intent of V1 defaults (see Definitions.h / Calibration defaults):
    // [0]=UGainA, [1]=IGainA, [4]=UGainB, [5]=IGainB, [8]=UGainC, [9]=IGainC, [12]=IGainN
    38800, 45578, 0, 0,
    38500, 45578, 0, 0,
    38800, 45578, 0, 0,
    45578, 0, 0
};

unsigned short voltageread = 0;
