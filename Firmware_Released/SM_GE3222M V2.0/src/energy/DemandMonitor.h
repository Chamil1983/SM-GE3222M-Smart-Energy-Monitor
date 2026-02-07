/**
 * @file DemandMonitor.h
 * @brief 15-minute demand interval tracking and Time-of-Use (TOU) monitoring
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Tracks peak demand and TOU energy bucketing per utility standards
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"
#include "../../include/FirmwareConfig.h"
#include "../storage/NVStorage.h"

/**
 * @enum TOUPeriod
 * @brief Time-of-Use period classification
 */
enum class TOUPeriod {
    OFF_PEAK,       ///< Off-peak hours (lowest rate)
    SHOULDER,       ///< Shoulder hours (mid rate)
    PEAK            ///< Peak hours (highest rate)
};

/**
 * @struct DemandData
 * @brief Demand monitoring data structure
 */
struct DemandData {
    // Current interval demand (kW)
    float currentDemandTotal;
    float currentDemandPhaseA;
    float currentDemandPhaseB;
    float currentDemandPhaseC;
    
    // Peak demand (kW)
    float peakDemandTotal;
    float peakDemandPhaseA;
    float peakDemandPhaseB;
    float peakDemandPhaseC;
    
    // Peak demand timestamps
    unsigned long peakDemandTimestamp;
    unsigned long peakDemandTimestampA;
    unsigned long peakDemandTimestampB;
    unsigned long peakDemandTimestampC;
    
    // Rolling demand (average of last N intervals)
    float rollingDemandTotal;
    
    // TOU energy accumulators (kWh)
    float touEnergyOffPeak;
    float touEnergyShoulder;
    float touEnergyPeak;
    
    // Current TOU period
    TOUPeriod currentTOUPeriod;
    
    // Interval info
    uint16_t intervalMinutes;
    unsigned long intervalStartTime;
    unsigned long nextIntervalTime;
    
    DemandData() : currentDemandTotal(0), currentDemandPhaseA(0), 
                   currentDemandPhaseB(0), currentDemandPhaseC(0),
                   peakDemandTotal(0), peakDemandPhaseA(0),
                   peakDemandPhaseB(0), peakDemandPhaseC(0),
                   peakDemandTimestamp(0), peakDemandTimestampA(0),
                   peakDemandTimestampB(0), peakDemandTimestampC(0),
                   rollingDemandTotal(0),
                   touEnergyOffPeak(0), touEnergyShoulder(0), touEnergyPeak(0),
                   currentTOUPeriod(TOUPeriod::OFF_PEAK),
                   intervalMinutes(DEMAND_INTERVAL_MIN),
                   intervalStartTime(0), nextIntervalTime(0) {}
};

/**
 * @class DemandMonitor
 * @brief Monitors power demand and Time-of-Use energy consumption
 * 
 * Singleton class providing:
 * - 15-minute demand interval tracking (utility standard)
 * - Peak demand recording with timestamps
 * - Rolling demand calculation
 * - Time-of-Use (TOU) energy bucketing
 * - NVS persistence for peak demand history
 */
class DemandMonitor {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to DemandMonitor instance
     */
    static DemandMonitor& getInstance();
    
    /**
     * @brief Initialize demand monitor
     * @param intervalMinutes Demand interval in minutes (default 15)
     * @return true if initialization successful
     */
    bool init(uint16_t intervalMinutes = DEMAND_INTERVAL_MIN);
    
    /**
     * @brief Update demand with current power readings
     * @param data Current meter data
     * 
     * Call this regularly (e.g., every 100ms) to accumulate power for demand calculation
     */
    void update(const MeterData& data);
    
    /**
     * @brief Get current demand data
     * @return DemandData structure
     */
    DemandData getDemandData() const { return demandData; }
    
    /**
     * @brief Reset peak demand values
     * 
     * Typically called at start of billing period or manually by user
     */
    void resetPeakDemand();
    
    /**
     * @brief Reset TOU energy accumulators
     */
    void resetTOUEnergy();
    
    /**
     * @brief Reset all demand data
     */
    void resetAll();
    
    /**
     * @brief Save demand data to NVS
     * @return true if save successful
     */
    bool save();
    
    /**
     * @brief Load demand data from NVS
     * @return true if load successful
     */
    bool load();
    
    /**
     * @brief Set TOU schedule
     * @param peakStartHour Peak period start hour (0-23)
     * @param peakEndHour Peak period end hour (0-23)
     * @param shoulderStartHour1 Shoulder period 1 start hour
     * @param shoulderEndHour1 Shoulder period 1 end hour
     * @param shoulderStartHour2 Shoulder period 2 start hour
     * @param shoulderEndHour2 Shoulder period 2 end hour
     * 
     * Off-peak is any time not in peak or shoulder periods
     */
    void setTOUSchedule(uint8_t peakStartHour, uint8_t peakEndHour,
                        uint8_t shoulderStartHour1, uint8_t shoulderEndHour1,
                        uint8_t shoulderStartHour2, uint8_t shoulderEndHour2);
    
    /**
     * @brief Get current TOU period based on time of day
     * @return Current TOU period
     */
    TOUPeriod getCurrentTOUPeriod() const;
    
    /**
     * @brief Get time until next demand interval
     * @return Milliseconds until next interval
     */
    unsigned long getTimeUntilNextInterval() const;
    
    /**
     * @brief Check if interval just completed
     * @return true if new interval just started
     */
    bool isIntervalComplete() const { return intervalComplete; }
    
    /**
     * @brief Export demand data as JSON string
     * @return JSON formatted string
     */
    String toJSON() const;

private:
    DemandMonitor() : initialized(false), intervalComplete(false),
                      powerAccumTotal(0), powerAccumPhaseA(0),
                      powerAccumPhaseB(0), powerAccumPhaseC(0),
                      sampleCount(0) {}
    ~DemandMonitor() {}
    
    DemandMonitor(const DemandMonitor&) = delete;
    DemandMonitor& operator=(const DemandMonitor&) = delete;
    
    bool initialized;               ///< Initialization status
    DemandData demandData;          ///< Demand data
    bool intervalComplete;          ///< Flag indicating interval completion
    
    // TOU schedule (hours)
    uint8_t touPeakStart;
    uint8_t touPeakEnd;
    uint8_t touShoulderStart1;
    uint8_t touShoulderEnd1;
    uint8_t touShoulderStart2;
    uint8_t touShoulderEnd2;
    
    // Accumulation for current interval
    double powerAccumTotal;         ///< Accumulated power total (W*samples)
    double powerAccumPhaseA;        ///< Accumulated power phase A
    double powerAccumPhaseB;        ///< Accumulated power phase B
    double powerAccumPhaseC;        ///< Accumulated power phase C
    uint32_t sampleCount;           ///< Number of samples in current interval
    
    // Rolling demand buffer
    float rollingDemandBuffer[DEMAND_ROLLING_SAMPLES];
    uint8_t rollingDemandIndex;
    
    /**
     * @brief Calculate demand for current interval
     */
    void calculateIntervalDemand();
    
    /**
     * @brief Update peak demand if current exceeds previous
     */
    void updatePeakDemand();
    
    /**
     * @brief Update TOU energy accumulators
     * @param energyKWh Energy consumed in current interval (kWh)
     */
    void updateTOUEnergy(float energyKWh);
    
    /**
     * @brief Update rolling demand average
     */
    void updateRollingDemand();
};
