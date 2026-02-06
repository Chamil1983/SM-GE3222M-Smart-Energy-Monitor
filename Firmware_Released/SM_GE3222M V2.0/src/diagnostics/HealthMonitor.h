/**
 * @file HealthMonitor.h
 * @brief Runtime health monitoring system
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Tracks system health metrics and performance
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"

/**
 * @class HealthMonitor
 * @brief Runtime system health monitoring
 * 
 * Singleton class that monitors system health:
 * - Heap memory usage (free, minimum)
 * - System uptime
 * - Restart counter (persisted)
 * - CPU load estimation
 * - Task scheduler overruns
 * - WiFi signal strength
 * - Communication error rates
 * 
 * Provides health status checking against thresholds
 */
class HealthMonitor {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to HealthMonitor instance
     */
    static HealthMonitor& getInstance();
    
    /**
     * @brief Initialize health monitor
     * @return true if successful
     */
    bool init();
    
    /**
     * @brief Update health metrics (call from main loop)
     * 
     * Should be called regularly (e.g., every 1 second)
     */
    void update();
    
    /**
     * @brief Get free heap bytes
     * @return Current free heap in bytes
     */
    uint32_t getFreeHeap() const { return healthData.freeHeap; }
    
    /**
     * @brief Get minimum free heap ever observed
     * @return Minimum free heap in bytes
     */
    uint32_t getMinFreeHeap() const { return healthData.minFreeHeap; }
    
    /**
     * @brief Get system uptime
     * @return Uptime in seconds
     */
    uint32_t getUptime() const { return healthData.uptime; }
    
    /**
     * @brief Get restart counter
     * @return Number of system restarts
     */
    uint32_t getRestartCount() const { return healthData.restartCount; }
    
    /**
     * @brief Get CPU load percentage
     * @return CPU load (0-100)
     */
    float getCPULoad() const { return healthData.cpuLoad; }
    
    /**
     * @brief Get task overrun count
     * @return Number of task scheduler overruns
     */
    uint32_t getTaskOverruns() const { return healthData.taskOverruns; }
    
    /**
     * @brief Get WiFi RSSI
     * @return RSSI in dBm, or 0 if not connected
     */
    int getWiFiRSSI() const { return healthData.wifiRSSI; }
    
    /**
     * @brief Get SPI error count
     * @return Number of SPI errors
     */
    uint32_t getSPIErrors() const { return healthData.spiErrors; }
    
    /**
     * @brief Get I2C error count
     * @return Number of I2C errors
     */
    uint32_t getI2CErrors() const { return healthData.i2cErrors; }
    
    /**
     * @brief Get complete health data structure
     * @return Reference to HealthData
     */
    const HealthData& getHealthData() const { return healthData; }
    
    /**
     * @brief Get health data as JSON string
     * @param buffer Output buffer
     * @param bufSize Buffer size
     */
    void getHealthJSON(char* buffer, size_t bufSize);
    
    /**
     * @brief Check if system is healthy
     * @return true if all metrics are within thresholds
     */
    bool isHealthy() const;
    
    /**
     * @brief Increment SPI error counter
     */
    void incrementSPIErrors() { healthData.spiErrors++; }
    
    /**
     * @brief Increment I2C error counter
     */
    void incrementI2CErrors() { healthData.i2cErrors++; }
    
    /**
     * @brief Increment task overrun counter
     */
    void incrementTaskOverruns() { healthData.taskOverruns++; }
    
    /**
     * @brief Set WiFi connection status
     * @param connected Connection status
     * @param rssi Signal strength (dBm)
     */
    void setWiFiStatus(bool connected, int rssi);
    
    /**
     * @brief Set MQTT connection status
     * @param connected Connection status
     */
    void setMQTTStatus(bool connected);
    
    /**
     * @brief Set Ethernet link status
     * @param linked Link status
     */
    void setEthernetStatus(bool linked);
    
    /**
     * @brief Get health warning message
     * @param buffer Output buffer
     * @param bufSize Buffer size
     * @return true if warnings present
     */
    bool getWarnings(char* buffer, size_t bufSize);

private:
    // Private constructor for singleton
    HealthMonitor();
    
    // Delete copy constructor and assignment operator
    HealthMonitor(const HealthMonitor&) = delete;
    HealthMonitor& operator=(const HealthMonitor&) = delete;
    
    /**
     * @brief Load restart count from NVS
     */
    void loadRestartCount();
    
    /**
     * @brief Save restart count to NVS
     */
    void saveRestartCount();
    
    /**
     * @brief Update heap statistics
     */
    void updateHeapStats();
    
    /**
     * @brief Update CPU load estimate
     */
    void updateCPULoad();
    
    // Health data
    HealthData healthData;
    
    // State
    bool initialized;
    unsigned long lastUpdateTime;
    unsigned long bootTime;
    
    // CPU load calculation
    unsigned long lastIdleTime;
    unsigned long lastTotalTime;
    
    // Health thresholds
    static const uint32_t HEAP_WARNING_THRESHOLD = 20 * 1024;  // 20KB
    static const uint8_t CPU_WARNING_THRESHOLD = 90;           // 90%
    static const uint32_t TASK_OVERRUN_WARNING = 10;
    static const int RSSI_WARNING_THRESHOLD = -80;             // -80 dBm
};

#endif // HEALTHMONITOR_H
