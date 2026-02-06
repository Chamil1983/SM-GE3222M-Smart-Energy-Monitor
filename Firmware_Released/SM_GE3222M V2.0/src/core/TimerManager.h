/**
 * @file TimerManager.h
 * @brief Hardware timer abstraction for ESP32
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides hardware timer functionality and watchdog management
 */

#pragma once

#include <Arduino.h>

/**
 * @class TimerManager
 * @brief Manages ESP32 hardware timers and watchdog
 * 
 * Singleton class that provides 1ms system tick for the TaskScheduler
 * and manages the hardware watchdog timer.
 */
class TimerManager {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to TimerManager instance
     */
    static TimerManager& getInstance();
    
    /**
     * @brief Initialize hardware timer and watchdog
     * @return true if initialization successful
     * 
     * Configures hardware timer for 1ms tick and initializes watchdog
     * with 5 second timeout.
     */
    bool init();
    
    /**
     * @brief Get millisecond tick count
     * @return Current tick count in milliseconds
     * 
     * Returns the number of milliseconds since system start.
     * Uses Arduino millis() function.
     */
    unsigned long getTicks() const;
    
    /**
     * @brief Feed the watchdog timer
     * 
     * Must be called periodically (at least every 5 seconds)
     * to prevent watchdog reset.
     */
    void feedWatchdog();
    
    /**
     * @brief Enable watchdog timer
     * @return true if successful
     */
    bool enableWatchdog();
    
    /**
     * @brief Disable watchdog timer
     * @return true if successful
     */
    bool disableWatchdog();
    
    /**
     * @brief Check if watchdog is enabled
     * @return true if watchdog is enabled
     */
    bool isWatchdogEnabled() const { return watchdogEnabled; }
    
    /**
     * @brief Get watchdog timeout in milliseconds
     * @return Watchdog timeout value
     */
    uint32_t getWatchdogTimeout() const { return WATCHDOG_TIMEOUT_MS; }

private:
    TimerManager() : watchdogEnabled(false), lastWatchdogFeed(0) {}
    TimerManager(const TimerManager&) = delete;
    TimerManager& operator=(const TimerManager&) = delete;
    
    static const uint32_t WATCHDOG_TIMEOUT_MS = 5000;  // 5 second timeout
    
    bool watchdogEnabled;
    unsigned long lastWatchdogFeed;
};
