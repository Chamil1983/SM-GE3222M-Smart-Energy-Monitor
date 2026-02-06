/**
 * @file ATM90E36Driver.h
 * @brief Low-level SPI driver for ATM90E36 energy metering IC
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides complete interface to ATM90E36 3-phase energy metering IC
 * including initialization, calibration, and all measurement getters.
 * 
 * This is the single most critical driver - all energy measurement
 * functionality depends on it.
 */

#pragma once

#include <Arduino.h>
#include "../../include/RegisterMap.h"
#include "../../include/PinMap.h"
#include "../hal/SPIBus.h"
#include "../core/ErrorHandler.h"

/**
 * @class ATM90E36Driver
 * @brief Complete low-level driver for ATM90E36 energy IC
 * 
 * Singleton class providing:
 * - SPI communication with retry mechanism
 * - Full initialization and calibration
 * - All voltage, current, power, energy measurements
 * - THD, power factor, phase angle measurements
 * - Status and diagnostic registers
 * - Checksum verification
 */
class ATM90E36Driver {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to ATM90E36Driver instance
     */
    static ATM90E36Driver& getInstance();
    
    /**
     * @brief Initialize ATM90E36 with calibration parameters
     * @param lineFreq Line frequency configuration (4485 for 60Hz, 389 for 50Hz)
     * @param pgagain PGA gain for current channels (0x1500 for 2x, 0x0000 for 1x)
     * @param ugain1 Voltage gain for phase A
     * @param ugain2 Voltage gain for phase B
     * @param ugain3 Voltage gain for phase C
     * @param igainA Current gain for CT1 (phase A)
     * @param igainB Current gain for CT2 (phase B)
     * @param igainC Current gain for CT3 (phase C)
     * @param igainN Current gain for CTN (neutral)
     * @return true if initialization successful
     */
    bool begin(unsigned short lineFreq, unsigned short pgagain, 
               unsigned short ugain1, unsigned short ugain2, unsigned short ugain3,
               unsigned short igainA, unsigned short igainB, unsigned short igainC, 
               unsigned short igainN);
    
    /**
     * @brief Check for calibration checksum errors
     * @return true if any checksum error detected
     */
    bool calibrationError();
    
    // ============================================================
    // Voltage Measurements
    // ============================================================
    
    double GetLineVoltage1();      ///< Phase A RMS voltage (V)
    double GetLineVoltage2();      ///< Phase B RMS voltage (V)
    double GetLineVoltage3();      ///< Phase C RMS voltage (V)
    
    // ============================================================
    // Current Measurements
    // ============================================================
    
    double GetLineCurrentCT1();    ///< Phase A RMS current (A)
    double GetLineCurrentCT2();    ///< Phase B RMS current (A)
    double GetLineCurrentCT3();    ///< Phase C RMS current (A)
    double GetLineCurrentCTN();    ///< Neutral calculated current (A)
    double GetLineCurrentCTNSamp(); ///< Neutral sampled current (A)
    
    // ============================================================
    // THD Measurements
    // ============================================================
    
    double GetLineVoltage1THDN();  ///< Phase A voltage THD+N (%)
    double GetLineVoltage2THDN();  ///< Phase B voltage THD+N (%)
    double GetLineVoltage3THDN();  ///< Phase C voltage THD+N (%)
    double GetLineCurrentCT1THDN(); ///< Phase A current THD+N (%)
    double GetLineCurrentCT2THDN(); ///< Phase B current THD+N (%)
    double GetLineCurrentCT3THDN(); ///< Phase C current THD+N (%)
    
    // ============================================================
    // Active Power Measurements
    // ============================================================
    
    double GetActivePowerCT1();    ///< Phase A active power (W)
    double GetActivePowerCT2();    ///< Phase B active power (W)
    double GetActivePowerCT3();    ///< Phase C active power (W)
    double GetTotalActivePower();  ///< Total active power (W)
    double GetMeanActivePowerPhaA(); ///< Phase A mean active power (W)
    double GetMeanActivePowerPhaB(); ///< Phase B mean active power (W)
    double GetMeanActivePowerPhaC(); ///< Phase C mean active power (W)
    
    // ============================================================
    // Fundamental Power Measurements
    // ============================================================
    
    double GetTotalActiveFundPower();    ///< Total fundamental active power (W)
    double GetMeanActiveFundPowerPhaA(); ///< Phase A fundamental active power (W)
    double GetMeanActiveFundPowerPhaB(); ///< Phase B fundamental active power (W)
    double GetMeanActiveFundPowerPhaC(); ///< Phase C fundamental active power (W)
    
    // ============================================================
    // Harmonic Power Measurements
    // ============================================================
    
    double GetTotalActiveHarPower();    ///< Total harmonic active power (W)
    double GetMeanActiveHarPowerPhaA(); ///< Phase A harmonic active power (W)
    double GetMeanActiveHarPowerPhaB(); ///< Phase B harmonic active power (W)
    double GetMeanActiveHarPowerPhaC(); ///< Phase C harmonic active power (W)
    
    // ============================================================
    // Reactive Power Measurements
    // ============================================================
    
    double GetReactivePowerCT1();    ///< Phase A reactive power (VAR)
    double GetReactivePowerCT2();    ///< Phase B reactive power (VAR)
    double GetReactivePowerCT3();    ///< Phase C reactive power (VAR)
    double GetTotalReactivePower();  ///< Total reactive power (VAR)
    double GetMeanReactivePowerPhaA(); ///< Phase A mean reactive power (VAR)
    double GetMeanReactivePowerPhaB(); ///< Phase B mean reactive power (VAR)
    double GetMeanReactivePowerPhaC(); ///< Phase C mean reactive power (VAR)
    
    // ============================================================
    // Apparent Power Measurements
    // ============================================================
    
    double GetApparentPowerCT1();    ///< Phase A apparent power (VA)
    double GetApparentPowerCT2();    ///< Phase B apparent power (VA)
    double GetApparentPowerCT3();    ///< Phase C apparent power (VA)
    double GetTotalApparentPower();  ///< Total apparent power (VA)
    double GetMeanApparentPowerPhaA(); ///< Phase A mean apparent power (VA)
    double GetMeanApparentPowerPhaB(); ///< Phase B mean apparent power (VA)
    double GetMeanApparentPowerPhaC(); ///< Phase C mean apparent power (VA)
    double GetTotalVSumApparentPower(); ///< Total vector sum apparent power (VA)
    
    // ============================================================
    // Power Factor Measurements
    // ============================================================
    
    double GetPowerFactorCT1();    ///< Phase A power factor
    double GetPowerFactorCT2();    ///< Phase B power factor
    double GetPowerFactorCT3();    ///< Phase C power factor
    double GetTotalPowerFactor();  ///< Total power factor
    
    // ============================================================
    // Phase Angle Measurements
    // ============================================================
    
    double GetPhaseCT1();          ///< Phase A current-voltage angle (degrees)
    double GetPhaseCT2();          ///< Phase B current-voltage angle (degrees)
    double GetPhaseCT3();          ///< Phase C current-voltage angle (degrees)
    double GetPhaseangV1();        ///< Phase A voltage angle (degrees)
    double GetPhaseangV2();        ///< Phase B voltage angle (degrees)
    double GetPhaseangV3();        ///< Phase C voltage angle (degrees)
    
    // ============================================================
    // Frequency and Temperature
    // ============================================================
    
    double GetFrequency();         ///< Line frequency (Hz)
    double GetTemperature();       ///< IC temperature (°C)
    
    // ============================================================
    // Energy Measurements (Import/Export)
    // ============================================================
    
    // Active Energy Import
    double GetImportEnergy();      ///< Total import active energy (kWh)
    double GetImportEnergyPhaA();  ///< Phase A import active energy (kWh)
    double GetImportEnergyPhaB();  ///< Phase B import active energy (kWh)
    double GetImportEnergyPhaC();  ///< Phase C import active energy (kWh)
    
    // Active Energy Export
    double GetExportEnergy();      ///< Total export active energy (kWh)
    double GetExportEnergyPhaA();  ///< Phase A export active energy (kWh)
    double GetExportEnergyPhaB();  ///< Phase B export active energy (kWh)
    double GetExportEnergyPhaC();  ///< Phase C export active energy (kWh)
    
    // Reactive Energy Import
    double GetImportReactiveEnergy();     ///< Total import reactive energy (kVARh)
    double GetImportReactiveEnergyPhaA(); ///< Phase A import reactive energy (kVARh)
    double GetImportReactiveEnergyPhaB(); ///< Phase B import reactive energy (kVARh)
    double GetImportReactiveEnergyPhaC(); ///< Phase C import reactive energy (kVARh)
    
    // Reactive Energy Export
    double GetExportReactiveEnergy();     ///< Total export reactive energy (kVARh)
    double GetExportReactiveEnergyPhaA(); ///< Phase A export reactive energy (kVARh)
    double GetExportReactiveEnergyPhaB(); ///< Phase B export reactive energy (kVARh)
    double GetExportReactiveEnergyPhaC(); ///< Phase C export reactive energy (kVARh)
    
    // Apparent Energy
    double GetImportApparentEnergy();     ///< Total apparent energy (kVAh)
    double GetImportApparentEnergyPhaA(); ///< Phase A apparent energy (kVAh)
    double GetImportApparentEnergyPhaB(); ///< Phase B apparent energy (kVAh)
    double GetImportApparentEnergyPhaC(); ///< Phase C apparent energy (kVAh)
    double GetImportApparentEnergyArt();  ///< Total arithmetic apparent energy (kVAh)
    double GetImportApparentEnergyVec();  ///< Total vector apparent energy (kVAh)
    
    // ============================================================
    // Status Registers
    // ============================================================
    
    unsigned short GetSysStatus0();    ///< System status register 0
    unsigned short GetSysStatus1();    ///< System status register 1
    unsigned short GetMeterStatus0();  ///< Metering status register 0
    unsigned short GetMeterStatus1();  ///< Metering status register 1
    
    // ============================================================
    // Calibration and Utility Functions
    // ============================================================
    
    /**
     * @brief Calculate voltage/current offset for calibration
     * @param regh_addr High register address
     * @param regl_addr Low register address
     * @return Calculated offset value
     */
    double CalculateVIOffset(unsigned short regh_addr, unsigned short regl_addr);
    
    /**
     * @brief Calculate power offset for calibration
     * @param regh_addr High register address
     * @param regl_addr Low register address
     * @return Calculated offset value
     */
    double CalculatePowerOffset(unsigned short regh_addr, unsigned short regl_addr);
    
    /**
     * @brief Calibrate voltage or current against known value
     * @param reg Register to calibrate
     * @param actualVal Actual measured value from reference
     * @return Calculated gain value
     */
    double CalibrateVI(unsigned short reg, unsigned short actualVal);
    
    /**
     * @brief Get raw register value
     * @param registerRead Register address to read
     * @return Register value
     */
    unsigned short GetValueRegister(unsigned short registerRead);
    
    /**
     * @brief Get register value (alias)
     * @param registerRead Register address to read
     * @return Register value
     */
    unsigned short GetValue(unsigned short registerRead);
    
    /**
     * @brief Get register value by address
     * @param address Register address
     * @return Register value
     */
    unsigned short GetRegisterValue(unsigned char address);
    
    /**
     * @brief Check if driver is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
private:
    ATM90E36Driver();
    ~ATM90E36Driver();
    ATM90E36Driver(const ATM90E36Driver&) = delete;
    ATM90E36Driver& operator=(const ATM90E36Driver&) = delete;
    
    /**
     * @brief Core SPI communication function with retry
     * @param rw Read (1) or Write (0)
     * @param address Register address
     * @param val Value to write (ignored for reads)
     * @return Value read from register
     */
    unsigned short CommEnergyIC(unsigned char rw, unsigned short address, unsigned short val);
    
    /**
     * @brief Read 32-bit register (concatenates two 16-bit registers)
     * @param regh_addr High register address
     * @param regl_addr Low register address
     * @return 32-bit value
     */
    int Read32Register(signed short regh_addr, signed short regl_addr);
    
    bool initialized;
    
    // Configuration parameters
    unsigned short lineFreq;
    unsigned short pgagain;
    unsigned short ugain1;
    unsigned short ugain2;
    unsigned short ugain3;
    unsigned short igainA;
    unsigned short igainB;
    unsigned short igainC;
    unsigned short igainN;
    
    static const uint8_t MAX_RETRIES = 3;
};
