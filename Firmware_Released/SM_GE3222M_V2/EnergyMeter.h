#pragma once

/**
 * SM-GE3222M V2.0 - Energy Meter
 * 
 * High-level energy meter interface with filtering
 * Built on top of ATM90E36Driver
 * 
 * Features:
 * - Singleton pattern
 * - Moving average filter for voltage/current readings
 * - Thread-safe with mutex protection
 * - Complete meter data snapshot
 */

#include <Arduino.h>
#include "ATM90E36Driver.h"
#include "DataTypes.h"
#include "Logger.h"

class EnergyMeter {
public:
    /**
     * Get singleton instance
     */
    static EnergyMeter& getInstance() {
        static EnergyMeter instance;
        return instance;
    }

    /**
     * Initialize energy meter
     * @param filterSize Size of moving average filter (default 5)
     * @return true if successful
     */
    bool init(uint8_t filterSize = 5);

    /**
     * Update meter readings
     * Reads from ATM90E36, applies filtering, updates snapshot
     * @return true if successful
     */
    bool update();

    /**
     * Get complete meter data snapshot
     * Thread-safe access to latest filtered data
     * @return Current meter data
     */
    MeterData getSnapshot();

private:
    // Singleton - prevent copying
    EnergyMeter();
    ~EnergyMeter() = default;
    EnergyMeter(const EnergyMeter&) = delete;
    EnergyMeter& operator=(const EnergyMeter&) = delete;

    /**
     * Apply moving average filter to voltage and current
     */
    void applyFilter(MeterData& data);

    // State
    bool m_initialized;
    uint8_t m_filterSize;
    MeterData m_snapshot;
    SemaphoreHandle_t m_mutex;
    
    // Filter buffers (circular)
    float m_voltageBufferA[10];
    float m_voltageBufferB[10];
    float m_voltageBufferC[10];
    float m_currentBufferA[10];
    float m_currentBufferB[10];
    float m_currentBufferC[10];
    uint8_t m_filterIndex;
    bool m_filterFilled;
};
