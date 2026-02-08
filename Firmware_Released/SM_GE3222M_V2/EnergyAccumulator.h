#pragma once

/**
 * SM-GE3222M V2.0 - Energy Accumulator
 * 
 * Accumulates energy from power readings over time
 * Persists to NVS for recovery after power loss
 * 
 * Features:
 * - Singleton pattern
 * - Automatic periodic NVS persistence
 * - Per-phase energy tracking
 * - Import/Export energy separation
 * - Thread-safe operations
 */

#include <Arduino.h>
#include <Preferences.h>
#include "DataTypes.h"
#include "Logger.h"

class EnergyAccumulator {
public:
    /**
     * Get singleton instance
     */
    static EnergyAccumulator& getInstance() {
        static EnergyAccumulator instance;
        return instance;
    }

    /**
     * Initialize energy accumulator
     * @param persistIntervalSec Auto-save interval in seconds (0 = disabled)
     * @return true if successful
     */
    bool init(uint32_t persistIntervalSec = 300);

    /**
     * Update accumulator with new meter data
     * Calculates energy delta and accumulates
     * @param data Current meter data
     */
    void update(const MeterData& data);

    /**
     * Load accumulated energy from NVS
     * @return true if loaded successfully
     */
    bool loadFromNVS();

    /**
     * Save accumulated energy to NVS
     * @return true if saved successfully
     */
    bool saveToNVS();

    /**
     * Get current accumulated energy
     * @return Energy data structure
     */
    EnergyData getAccumulatedEnergy();

    /**
     * Reset all accumulated energy to zero
     */
    void reset();

private:
    // Singleton - prevent copying
    EnergyAccumulator();
    ~EnergyAccumulator() = default;
    EnergyAccumulator(const EnergyAccumulator&) = delete;
    EnergyAccumulator& operator=(const EnergyAccumulator&) = delete;

    /**
     * Accumulate energy for a single phase
     */
    void accumulatePhase(EnergyData::PhaseEnergy& energy, float activePower, 
                         float reactivePower, float deltaTime);

    // State
    bool m_initialized;
    uint32_t m_persistInterval;
    uint32_t m_lastPersistTime;
    uint32_t m_lastUpdateTime;
    EnergyData m_energy;
    SemaphoreHandle_t m_mutex;
    Preferences m_preferences;
    
    static constexpr const char* NVS_NAMESPACE = "energy_acc";
};
