/**
 * @file DisplayManager.h
 * @brief Unified display interface wrapping LCDManager
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * High-level display management with page rotation and thread-safe operations
 */

#pragma once

#include <Arduino.h>
#include <mutex>
#include "../display/LCDManager.h"
#include "../../include/GlobalTypes.h"

/**
 * @class DisplayManager
 * @brief Thread-safe wrapper around LCDManager with enhanced features
 * 
 * Features:
 * - Multiple display pages (boot, measurements, network, errors)
 * - Page rotation support
 * - Thread-safe operations with mutex
 * - Simplified interface for application layer
 * - Error message queue
 * - Status indicators
 */
class DisplayManager {
public:
    /**
     * @brief Display page types
     */
    enum class Page {
        BOOT,           // Boot splash screen
        PHASE_A,        // Phase A measurements
        PHASE_B,        // Phase B measurements
        PHASE_C,        // Phase C measurements
        TOTALS,         // Total power measurements
        ENERGY,         // Energy counters
        NETWORK,        // Network status
        SYSTEM,         // System information
        ERROR,          // Error messages
        CUSTOM          // Custom message page
    };

    /**
     * @brief Get singleton instance
     * @return Reference to DisplayManager instance
     */
    static DisplayManager& getInstance();

    /**
     * @brief Initialize display manager
     * @return true if successful, false on failure
     */
    bool init();

    /**
     * @brief Update display (call from main loop)
     * 
     * Handles page rotation and periodic refreshes
     */
    void update();

    /**
     * @brief Set current display page
     * @param page Page to display
     */
    void setPage(Page page);

    /**
     * @brief Get current page
     * @return Current page
     */
    Page getCurrentPage() const { return _currentPage; }

    /**
     * @brief Navigate to next page
     */
    void nextPage();

    /**
     * @brief Navigate to previous page
     */
    void previousPage();

    /**
     * @brief Update meter data for display
     * @param data MeterData reference
     */
    void updateMeterData(const MeterData& data);

    /**
     * @brief Set network information
     * @param ip IP address
     * @param rssi WiFi RSSI (-1 for Ethernet)
     * @param mqttConnected MQTT status
     */
    void setNetworkInfo(const char* ip, int rssi, bool mqttConnected);

    /**
     * @brief Set system information
     * @param uptime Uptime in seconds
     * @param temp Temperature in °C
     * @param freeHeap Free heap in bytes
     * @param errorCount Error count
     */
    void setSystemInfo(uint32_t uptime, float temp, uint32_t freeHeap, uint32_t errorCount);

    /**
     * @brief Display custom message
     * @param line Line number (0-3)
     * @param message Message text
     */
    void displayMessage(uint8_t line, const char* message);

    /**
     * @brief Display error message
     * @param errorCode Error code
     * @param message Error message
     * @param duration Duration in ms (0 = persistent)
     */
    void showError(ErrorCode errorCode, const char* message, unsigned long duration = 5000);

    /**
     * @brief Clear error display
     */
    void clearError();

    /**
     * @brief Enable/disable auto page rotation
     * @param enabled True to enable
     * @param intervalMs Rotation interval in ms
     */
    void setAutoRotation(bool enabled, uint32_t intervalMs = 5000);

    /**
     * @brief Check if auto rotation is enabled
     * @return true if enabled
     */
    bool isAutoRotationEnabled() const { return _autoRotationEnabled; }

    /**
     * @brief Clear display
     */
    void clear();

    /**
     * @brief Show boot splash screen
     * @param version Firmware version string
     */
    void showBootSplash(const char* version);

    /**
     * @brief Show initialization status
     * @param component Component name
     * @param success Success status
     */
    void showInitStatus(const char* component, bool success);

    /**
     * @brief Lock display updates (for critical sections)
     */
    void lock() { _mutex.lock(); }

    /**
     * @brief Unlock display updates
     */
    void unlock() { _mutex.unlock(); }

private:
    DisplayManager();
    ~DisplayManager() = default;
    DisplayManager(const DisplayManager&) = delete;
    DisplayManager& operator=(const DisplayManager&) = delete;

    /**
     * @brief Convert Page enum to DisplayPage enum
     * @param page Page enum
     * @return DisplayPage enum
     */
    DisplayPage pageToDisplayPage(Page page);

    /**
     * @brief Render current page
     */
    void renderCurrentPage();

    /**
     * @brief Handle error display timeout
     */
    void handleErrorTimeout();

    std::mutex _mutex;
    bool _initialized;
    Page _currentPage;
    Page _previousPage;

    // Auto rotation
    bool _autoRotationEnabled;
    uint32_t _rotationInterval;
    unsigned long _lastRotationTime;

    // Error handling
    bool _errorActive;
    ErrorCode _errorCode;
    char _errorMessage[64];
    unsigned long _errorStartTime;
    unsigned long _errorDuration;

    // Cached data
    MeterData _meterData;
    char _networkIP[16];
    int _networkRSSI;
    bool _networkMQTT;
    uint32_t _systemUptime;
    float _systemTemp;
    uint32_t _systemFreeHeap;
    uint32_t _systemErrorCount;

    bool _dataUpdated;
};
