/**
 * @file EnergyMeter.h
 * @brief High-level energy measurement engine
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides unified interface for all energy measurements using ATM90E36Driver.
 * Includes data filtering, validation, and JSON export capabilities.
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"
#include "ATM90E36Driver.h"

/**
 * @class EnergyMeter
 * @brief High-level measurement engine with filtering and validation
 * 
 * Singleton class providing:
 * - Single-call read of all measurements
 * - Optional moving average filter
 * - Measurement validation and range checking
 * - JSON export of measurements
 * - Easy access to phase and meter data
 */
class EnergyMeter {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to EnergyMeter instance
     */
    static EnergyMeter& getInstance();
    
    /**
     * @brief Initialize energy meter
     * @param filterWindowSize Moving average window size (0 = disabled, default 5)
     * @return true if initialization successful
     */
    bool init(uint8_t filterWindowSize = 5);
    
    /**
     * @brief Read all measurements from ATM90E36 and populate MeterData
     * @return true if read successful
     */
    bool readAll();
    
    /**
     * @brief Get data for specific phase
     * @param phase Phase identifier (1=A, 2=B, 3=C)
     * @return PhaseData struct for requested phase
     */
    PhaseData getPhaseData(uint8_t phase) const;
    
    /**
     * @brief Get complete meter data
     * @return MeterData struct with all measurements
     */
    MeterData getMeterData() const;
    
    /**
     * @brief Get measurements as JSON string
     * @param prettyPrint Format with indentation (default false)
     * @return JSON string representation of measurements
     */
    String getJSON(bool prettyPrint = false) const;
    
    /**
     * @brief Update measurements (call periodically from task loop)
     * @return true if update successful
     */
    bool update();
    
    /**
     * @brief Check if meter is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Enable/disable moving average filter
     * @param enable true to enable, false to disable
     */
    void setFilterEnabled(bool enable) { filterEnabled = enable; }
    
    /**
     * @brief Set moving average window size
     * @param size Window size (1-20, 0 disables filter)
     */
    void setFilterWindowSize(uint8_t size);
    
    /**
     * @brief Check if last read was successful
     * @return true if last read succeeded
     */
    bool isDataValid() const { return dataValid; }
    
    /**
     * @brief Get time since last successful read
     * @return Milliseconds since last read
     */
    unsigned long getTimeSinceLastRead() const;
    
private:
    EnergyMeter();
    ~EnergyMeter();
    EnergyMeter(const EnergyMeter&) = delete;
    EnergyMeter& operator=(const EnergyMeter&) = delete;
    
    /**
     * @brief Validate measurements for reasonable ranges
     * @param data MeterData to validate
     * @return true if data passes validation
     */
    bool validateMeasurements(const MeterData& data) const;
    
    /**
     * @brief Apply moving average filter to phase data
     * @param phase Phase data to filter
     */
    void applyFilter(PhaseData& phase);
    
    /**
     * @brief Read single phase data from driver
     * @param phase Phase number (1=A, 2=B, 3=C)
     * @return PhaseData struct
     */
    PhaseData readPhaseData(uint8_t phase);
    
    bool initialized;
    bool dataValid;
    bool filterEnabled;
    uint8_t filterWindowSize;
    
    MeterData currentData;
    ATM90E36Driver& driver;
    
    // Filter buffers for moving average
    static const uint8_t MAX_FILTER_SIZE = 20;
    float voltageBuffer[3][MAX_FILTER_SIZE];
    float currentBuffer[3][MAX_FILTER_SIZE];
    float powerBuffer[3][MAX_FILTER_SIZE];
    uint8_t filterIndex;
    
    // Validation thresholds
    static constexpr float MAX_VOLTAGE = 300.0f;
    static constexpr float MIN_VOLTAGE = 80.0f;
    static constexpr float MAX_CURRENT = 200.0f;
    static constexpr float MAX_FREQUENCY_DEVIATION = 5.0f; // Hz from 50/60
    static constexpr float NOMINAL_FREQUENCY_50HZ = 50.0f;
    static constexpr float NOMINAL_FREQUENCY_60HZ = 60.0f;
};
