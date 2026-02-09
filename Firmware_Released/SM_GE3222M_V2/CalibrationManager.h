#pragma once

/**
 * SM-GE3222M V2.0 - Calibration Manager
 * 
 * Manages calibration configuration storage in NVS
 * Provides factory defaults from V1.0 hardware specifications
 * 
 * Features:
 * - Singleton pattern
 * - NVS persistence using Preferences library
 * - Factory default calibration values
 * - Thread-safe operations
 */

#include <Arduino.h>
#include <Preferences.h>
#include "DataTypes.h"
#include "Logger.h"

class CalibrationManager {
public:
    /**
     * Get singleton instance
     */
    static CalibrationManager& getInstance() {
        static CalibrationManager instance;
        return instance;
    }

    /**
     * Load calibration from NVS
     * @param config Output calibration configuration
     * @return true if loaded successfully, false if using defaults
     */
    bool loadCalibration(CalibrationConfig& config);

    /**
     * Save calibration to NVS
     * @param config Calibration configuration to save
     * @return true if saved successfully
     */
    bool saveCalibration(const CalibrationConfig& config);

    /**
     * Get factory default calibration (V1.0 specifications)
     * @return Default calibration configuration
     */
        // Compatibility helpers for ProtocolV2
    bool getCalibration(CalibrationConfig& config) { return loadCalibration(config); }
    bool setCalibration(const CalibrationConfig& config) { return saveCalibration(config); }

CalibrationConfig getDefaultCalibration();

private:
    // Singleton - prevent copying
    CalibrationManager();
    ~CalibrationManager() = default;
    CalibrationManager(const CalibrationManager&) = delete;
    CalibrationManager& operator=(const CalibrationManager&) = delete;

    Preferences m_preferences;
    static constexpr const char* NVS_NAMESPACE = "calibration";
};
