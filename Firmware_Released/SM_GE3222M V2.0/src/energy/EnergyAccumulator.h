/**
 * @file EnergyAccumulator.h
 * @brief Energy tracking and persistence with daily rollover
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Tracks energy consumption with NVS persistence and provides
 * daily, weekly, and monthly energy statistics.
 */

#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "../../include/GlobalTypes.h"

/**
 * @class EnergyAccumulator
 * @brief Energy accumulation and daily tracking system
 * 
 * Singleton class providing:
 * - Continuous energy accumulation
 * - Periodic NVS persistence
 * - Daily energy tracking with rollover
 * - Weekly and monthly statistics
 * - Per-phase and total energy values
 * - Import/Export/Reactive/Apparent tracking
 */
class EnergyAccumulator {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to EnergyAccumulator instance
     */
    static EnergyAccumulator& getInstance();
    
    /**
     * @brief Initialize energy accumulator
     * @param persistInterval Interval for NVS saves in seconds (default 60)
     * @return true if initialization successful
     */
    bool init(uint32_t persistInterval = 60);
    
    /**
     * @brief Update energy values from meter data
     * @param meterData Current meter measurements
     * @return true if update successful
     */
    bool update(const MeterData& meterData);
    
    /**
     * @brief Reset all energy counters (requires authentication)
     * @param authCode Authentication code (must match)
     * @return true if reset successful
     */
    bool reset(uint32_t authCode = 0);
    
    /**
     * @brief Save current values to NVS
     * @return true if save successful
     */
    bool save();
    
    /**
     * @brief Load values from NVS
     * @return true if load successful
     */
    bool load();
    
    /**
     * @brief Get import active energy
     * @param phase Phase (0=Total, 1=A, 2=B, 3=C)
     * @return Energy in kWh
     */
    float getImportActiveEnergy(uint8_t phase = 0) const;
    
    /**
     * @brief Get export active energy
     * @param phase Phase (0=Total, 1=A, 2=B, 3=C)
     * @return Energy in kWh
     */
    float getExportActiveEnergy(uint8_t phase = 0) const;
    
    /**
     * @brief Get import reactive energy
     * @param phase Phase (0=Total, 1=A, 2=B, 3=C)
     * @return Energy in kVARh
     */
    float getImportReactiveEnergy(uint8_t phase = 0) const;
    
    /**
     * @brief Get export reactive energy
     * @param phase Phase (0=Total, 1=A, 2=B, 3=C)
     * @return Energy in kVARh
     */
    float getExportReactiveEnergy(uint8_t phase = 0) const;
    
    /**
     * @brief Get apparent energy
     * @param phase Phase (0=Total, 1=A, 2=B, 3=C)
     * @return Energy in kVAh
     */
    float getApparentEnergy(uint8_t phase = 0) const;
    
    /**
     * @brief Get today's total energy consumption
     * @return Energy in kWh
     */
    float getTodayEnergy() const { return todayEnergy; }
    
    /**
     * @brief Get yesterday's total energy consumption
     * @return Energy in kWh
     */
    float getYesterdayEnergy() const { return yesterdayEnergy; }
    
    /**
     * @brief Get this week's total energy consumption
     * @return Energy in kWh
     */
    float getThisWeekEnergy() const { return thisWeekEnergy; }
    
    /**
     * @brief Get this month's total energy consumption
     * @return Energy in kWh
     */
    float getThisMonthEnergy() const { return thisMonthEnergy; }
    
    /**
     * @brief Check if accumulator is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Set persist interval
     * @param intervalSeconds Interval in seconds
     */
    void setPersistInterval(uint32_t intervalSeconds);
    
    /**
     * @brief Force immediate save to NVS
     */
    void forceSave() { save(); }
    
    /**
     * @brief Get time since last save
     * @return Seconds since last save
     */
    uint32_t getTimeSinceLastSave() const;
    
private:
    EnergyAccumulator();
    ~EnergyAccumulator();
    EnergyAccumulator(const EnergyAccumulator&) = delete;
    EnergyAccumulator& operator=(const EnergyAccumulator&) = delete;
    
    /**
     * @brief Check and handle daily rollover
     * @return true if rollover occurred
     */
    bool checkDailyRollover();
    
    /**
     * @brief Update daily/weekly/monthly totals
     */
    void updatePeriodTotals();
    
    /**
     * @brief Check if periodic save is needed
     * @return true if save needed
     */
    bool needsPeriodicSave() const;
    
    bool initialized;
    uint32_t persistInterval;
    unsigned long lastSaveTime;
    unsigned long lastUpdateTime;
    
    // Energy accumulators [Total, A, B, C]
    float importActiveEnergy[4];
    float exportActiveEnergy[4];
    float importReactiveEnergy[4];
    float exportReactiveEnergy[4];
    float apparentEnergy[4];
    
    // Previous meter readings for delta calculation
    float prevImportActiveEnergy[4];
    float prevExportActiveEnergy[4];
    float prevImportReactiveEnergy[4];
    float prevExportReactiveEnergy[4];
    float prevApparentEnergy[4];
    
    // Daily tracking
    uint8_t currentDay;
    uint8_t currentWeek;
    uint8_t currentMonth;
    float todayEnergy;
    float yesterdayEnergy;
    float thisWeekEnergy;
    float thisMonthEnergy;
    float dayStartEnergy;
    float weekStartEnergy;
    float monthStartEnergy;
    
    Preferences preferences;
    
    static const char* NVS_NAMESPACE;
    static const uint32_t RESET_AUTH_CODE = 0xDEADBEEF;
    static const uint32_t DEFAULT_PERSIST_INTERVAL = 60; // seconds
};
