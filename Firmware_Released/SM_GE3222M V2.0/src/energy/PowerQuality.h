/**
 * @file PowerQuality.h
 * @brief Power quality analysis and event detection
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Monitors and analyzes power quality metrics including voltage sag/swell,
 * phase imbalance, THD, and frequency deviations.
 */

#pragma once

#include <Arduino.h>
#include <vector>
#include "../../include/GlobalTypes.h"

/**
 * @class PowerQuality
 * @brief Power quality monitoring and analysis engine
 * 
 * Singleton class providing:
 * - Voltage sag/swell detection
 * - Phase imbalance calculation (NEMA/IEC)
 * - THD threshold monitoring
 * - Frequency range checking
 * - Event logging with timestamps
 */
class PowerQuality {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to PowerQuality instance
     */
    static PowerQuality& getInstance();
    
    /**
     * @brief Initialize power quality analyzer
     * @param nominalVoltage Nominal system voltage (default 230V)
     * @param nominalFrequency Nominal frequency (default 50Hz)
     * @return true if initialization successful
     */
    bool init(float nominalVoltage = 230.0f, float nominalFrequency = 50.0f);
    
    /**
     * @brief Analyze power quality from meter data
     * @param meterData Current meter measurements
     * @return Number of new events detected
     */
    uint8_t analyze(const MeterData& meterData);
    
    /**
     * @brief Set voltage sag threshold
     * @param threshold Threshold in per-unit (default 0.9 = 90%)
     */
    void setVoltageSagThreshold(float threshold);
    
    /**
     * @brief Set voltage swell threshold
     * @param threshold Threshold in per-unit (default 1.1 = 110%)
     */
    void setVoltageSwellThreshold(float threshold);
    
    /**
     * @brief Set THD alarm threshold
     * @param threshold THD percentage threshold (default 5%)
     */
    void setTHDThreshold(float threshold);
    
    /**
     * @brief Set phase imbalance threshold
     * @param threshold Imbalance percentage threshold (default 2%)
     */
    void setImbalanceThreshold(float threshold);
    
    /**
     * @brief Get last N power quality events
     * @param count Number of events to retrieve (default 10)
     * @return Vector of PowerQualityEvent structs
     */
    std::vector<PowerQualityEvent> getEvents(uint8_t count = 10) const;
    
    /**
     * @brief Get all events
     * @return Vector of all PowerQualityEvent structs
     */
    std::vector<PowerQualityEvent> getAllEvents() const;
    
    /**
     * @brief Clear event history
     */
    void clearEvents();
    
    /**
     * @brief Get current phase imbalance (NEMA method)
     * @return Imbalance percentage
     */
    float getPhaseImbalance() const { return currentImbalance; }
    
    /**
     * @brief Check if any voltage sag is active
     * @return true if sag detected
     */
    bool isVoltageSag() const { return voltageSagActive; }
    
    /**
     * @brief Check if any voltage swell is active
     * @return true if swell detected
     */
    bool isVoltageSwell() const { return voltageSwellActive; }
    
    /**
     * @brief Check if THD exceeds threshold
     * @return true if THD alarm active
     */
    bool isTHDAlarm() const { return thdAlarmActive; }
    
    /**
     * @brief Check if frequency is out of range
     * @return true if frequency deviation detected
     */
    bool isFrequencyOutOfRange() const { return frequencyOutOfRange; }
    
    /**
     * @brief Get total event count
     * @return Total number of events logged
     */
    uint16_t getEventCount() const { return events.size(); }
    
private:
    PowerQuality();
    ~PowerQuality();
    PowerQuality(const PowerQuality&) = delete;
    PowerQuality& operator=(const PowerQuality&) = delete;
    
    /**
     * @brief Detect voltage sag/swell events
     * @param meterData Current meter data
     */
    void detectVoltageSagSwell(const MeterData& meterData);
    
    /**
     * @brief Calculate phase imbalance (NEMA method)
     * @param meterData Current meter data
     * @return Imbalance percentage
     */
    float calculatePhaseImbalance(const MeterData& meterData);
    
    /**
     * @brief Check THD levels against threshold
     * @param meterData Current meter data
     */
    void checkTHD(const MeterData& meterData);
    
    /**
     * @brief Check frequency range
     * @param meterData Current meter data
     */
    void checkFrequency(const MeterData& meterData);
    
    /**
     * @brief Log power quality event
     * @param event Event to log
     */
    void logEvent(const PowerQualityEvent& event);
    
    bool initialized;
    float nominalVoltage;
    float nominalFrequency;
    
    // Thresholds
    float voltageSagThreshold;
    float voltageSwellThreshold;
    float thdThreshold;
    float imbalanceThreshold;
    float minFrequency;
    float maxFrequency;
    
    // Current status flags
    bool voltageSagActive;
    bool voltageSwellActive;
    bool thdAlarmActive;
    bool frequencyOutOfRange;
    float currentImbalance;
    
    // Event storage (circular buffer, last 100 events)
    static const uint16_t MAX_EVENTS = 100;
    std::vector<PowerQualityEvent> events;
    
    // Debounce timing to prevent event flooding
    unsigned long lastEventTime[5]; // One for each event type
    static const unsigned long EVENT_DEBOUNCE_MS = 5000; // 5 seconds
};
