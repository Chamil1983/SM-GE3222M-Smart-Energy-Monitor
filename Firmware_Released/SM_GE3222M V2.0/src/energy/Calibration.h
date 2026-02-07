/**
 * @file Calibration.h
 * @brief Calibration management for ATM90E36
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Handles loading, saving, and applying calibration parameters
 * to ATM90E36 energy metering IC with NVS persistence.
 */

#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "../../include/GlobalTypes.h"
#include "ATM90E36Driver.h"

/**
 * @class Calibration
 * @brief Calibration management system with NVS persistence
 * 
 * Singleton class providing:
 * - Load/save calibration from/to NVS
 * - Apply calibration to ATM90E36 driver
 * - Checksum verification
 * - Factory defaults restoration
 * - Calibration parameter access
 */
class Calibration {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to Calibration instance
     */
    static Calibration& getInstance();
    
    /**
     * @brief Initialize calibration system
     * @return true if initialization successful
     */
    bool init();
    
    /**
     * @brief Load calibration from NVS
     * @return true if load successful
     */
    bool load();
    
    /**
     * @brief Save calibration to NVS
     * @return true if save successful
     */
    bool save();
    
    /**
     * @brief Apply calibration to ATM90E36 driver
     * @return true if apply successful
     */
    bool apply();
    
    /**
     * @brief Get current calibration configuration
     * @return CalibrationConfig struct
     */
    CalibrationConfig getConfig() const { return config; }
    
    /**
     * @brief Set new calibration configuration
     * @param newConfig New calibration parameters
     */
    void setConfig(const CalibrationConfig& newConfig);
    
    /**
     * @brief Verify calibration checksums
     * @return true if checksums are valid
     */
    bool verify();
    
    /**
     * @brief Reset calibration to factory defaults
     * @return true if reset successful
     */
    bool reset();
    
    /**
     * @brief Set default calibration values from V1.0
     */
    void setDefault();
    
    /**
     * @brief Check if calibration is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Get individual voltage gain
     * @param phase Phase (0=A, 1=B, 2=C)
     * @return Voltage gain value
     */
    uint16_t getVoltageGain(uint8_t phase) const;
    
    /**
     * @brief Set individual voltage gain
     * @param phase Phase (0=A, 1=B, 2=C)
     * @param gain Gain value
     */
    void setVoltageGain(uint8_t phase, uint16_t gain);
    
    /**
     * @brief Get individual current gain
     * @param phase Phase (0=A, 1=B, 2=C)
     * @return Current gain value
     */
    uint16_t getCurrentGain(uint8_t phase) const;
    
    /**
     * @brief Set individual current gain
     * @param phase Phase (0=A, 1=B, 2=C)
     * @param gain Gain value
     */
    void setCurrentGain(uint8_t phase, uint16_t gain);
    
    /**
     * @brief Get line frequency setting
     * @return Line frequency register value
     */
    uint16_t getLineFreq() const { return config.lineFreq; }
    
    /**
     * @brief Set line frequency
     * @param freq Line frequency register value (389 for 50Hz, 4485 for 60Hz)
     */
    void setLineFreq(uint16_t freq);
    
private:
    Calibration();
    ~Calibration();
    Calibration(const Calibration&) = delete;
    Calibration& operator=(const Calibration&) = delete;
    
    /**
     * @brief Calculate checksums for calibration data
     */
    void calculateChecksums();
    
    /**
     * @brief Verify calculated checksums match stored
     * @return true if checksums match
     */
    bool verifyChecksums() const;
    
    bool initialized;
    CalibrationConfig config;
    Preferences preferences;
    ATM90E36Driver& driver;
    
    static const char* NVS_NAMESPACE;
    static const char* NVS_KEY_CONFIG;
    static const uint16_t CALIBRATION_VERSION = 1;
};
