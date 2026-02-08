#pragma once

/**
 * SM-GE3222M V2.0 - ATM90E36 Energy IC Driver
 * 
 * Low-level driver for ATM90E36 3-Phase Energy Metering IC
 * 
 * Features:
 * - Thread-safe SPI communication
 * - Complete calibration sequence
 * - Checksum verification
 * - Bulk register reading
 * - 32-bit energy register support
 * 
 * Ported from V1.0 EnergyATM90E36.cpp with modernization
 */

#include <Arduino.h>
#include "RegisterMap.h"
#include "DataTypes.h"

class ATM90E36Driver {
public:
    /**
     * Get singleton instance
     */
    static ATM90E36Driver& getInstance() {
        static ATM90E36Driver instance;
        return instance;
    }

    /**
     * Initialize ATM90E36 with calibration
     * @param config Calibration configuration
     * @return true if successful
     */
    bool init(const CalibrationConfig& config);

    /**
     * Read all meter data in one call (optimized)
     * @param data Output meter data structure
     * @return true if successful
     */
    bool readAll(MeterData& data);

    /**
     * Verify calibration checksums
     * @return true if all checksums valid
     */
    bool verifyChecksums();

    /**
     * Check for calibration errors
     * @return true if calibration error detected
     */
    bool hasCalibrationError();

    /**
     * Perform soft reset
     * @return true if successful
     */
    bool softReset();

    // Individual register getters (for backward compatibility)
    float getVoltageA();
    float getVoltageB();
    float getVoltageC();
    float getCurrentA();
    float getCurrentB();
    float getCurrentC();
    float getCurrentN();
    float getActivePowerA();
    float getActivePowerB();
    float getActivePowerC();
    float getActivePowerTotal();
    float getReactivePowerA();
    float getReactivePowerB();
    float getReactivePowerC();
    float getReactivePowerTotal();
    float getApparentPowerA();
    float getApparentPowerB();
    float getApparentPowerC();
    float getApparentPowerTotal();
    float getPowerFactorA();
    float getPowerFactorB();
    float getPowerFactorC();
    float getPowerFactorTotal();
    float getPhaseAngleA();
    float getPhaseAngleB();
    float getPhaseAngleC();
    float getVoltageTHDA();
    float getVoltageTHDB();
    float getVoltageTHDC();
    float getCurrentTHDA();
    float getCurrentTHDB();
    float getCurrentTHDC();
    float getFrequency();
    float getTemperature();
    
    // 32-bit energy registers
    uint32_t getFwdActiveEnergyA();
    uint32_t getFwdActiveEnergyB();
    uint32_t getFwdActiveEnergyC();
    uint32_t getFwdActiveEnergyTotal();
    uint32_t getRevActiveEnergyA();
    uint32_t getRevActiveEnergyB();
    uint32_t getRevActiveEnergyC();
    uint32_t getRevActiveEnergyTotal();

    /**
     * Get last error code
     */
    ErrorCode getLastError() const { return m_lastError; }

private:
    // Singleton - prevent copying
    ATM90E36Driver();
    ~ATM90E36Driver() = default;
    ATM90E36Driver(const ATM90E36Driver&) = delete;
    ATM90E36Driver& operator=(const ATM90E36Driver&) = delete;

    /**
     * Low-level SPI communication
     * Implements V1.0 protocol: 200kHz, Mode3, MSB-first, byte-swapped
     */
    uint16_t readRegister(uint16_t address);
    bool writeRegister(uint16_t address, uint16_t value);
    uint32_t read32BitRegister(uint16_t highAddress, uint16_t lowAddress);

    /**
     * Apply calibration sequence
     * Implements V1.0 multi-phase calibration:
     * Phase 1: Configuration registers (ConfigStart)
     * Phase 2: Calibration registers (CalStart)
     * Phase 3: Harmonic registers (HarmStart)
     * Phase 4: Measurement registers (AdjStart)
     */
    bool applyCalibration(const CalibrationConfig& config);

    /**
     * Calculate checksum for calibration
     */
    uint16_t calculateChecksum(const uint16_t* registers, uint8_t count);

    // State
    bool m_initialized;
    CalibrationConfig m_calibration;
    ErrorCode m_lastError;
    uint32_t m_lastReadTime;
    
    // Cached checksums
    uint16_t m_csZero;
    uint16_t m_csOne;
    uint16_t m_csTwo;
    uint16_t m_csThree;
};
