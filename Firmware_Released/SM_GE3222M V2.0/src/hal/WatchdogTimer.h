/**
 * @file WatchdogTimer.h
 * @brief Hardware and software watchdog timer management
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides watchdog functionality to detect and recover from system hangs
 */

#pragma once

#include <Arduino.h>
#include <esp_task_wdt.h>
#include "../../include/FirmwareConfig.h"

/**
 * @class WatchdogTimer
 * @brief Manages hardware and software watchdogs for system reliability
 * 
 * Singleton class providing:
 * - Hardware watchdog timer (TWDT)
 * - Per-task software watchdog monitoring
 * - Automatic reset on timeout
 * - Watchdog status reporting
 */
class WatchdogTimer {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to WatchdogTimer instance
     */
    static WatchdogTimer& getInstance();
    
    /**
     * @brief Initialize watchdog timer
     * @param timeoutSec Timeout in seconds (default from FirmwareConfig.h)
     * @return true if initialization successful
     */
    bool init(uint8_t timeoutSec = WATCHDOG_TIMEOUT_SEC);
    
    /**
     * @brief Enable watchdog
     * @return true if successful
     */
    bool enable();
    
    /**
     * @brief Disable watchdog
     * @return true if successful
     */
    bool disable();
    
    /**
     * @brief Feed (reset) the watchdog timer
     * 
     * Must be called periodically (before timeout) to prevent system reset.
     * Thread-safe operation.
     */
    void feed();
    
    /**
     * @brief Add current task to watchdog monitoring
     * @param taskName Optional task name for identification
     * @return true if task added successfully
     * 
     * Call this from FreeRTOS tasks that need watchdog protection.
     */
    bool addTask(const char* taskName = nullptr);
    
    /**
     * @brief Remove current task from watchdog monitoring
     * @return true if task removed successfully
     */
    bool removeTask();
    
    /**
     * @brief Reset (feed) watchdog for current task
     * 
     * Each monitored task must call this periodically.
     */
    void feedTask();
    
    /**
     * @brief Check if watchdog is enabled
     * @return true if enabled
     */
    bool isEnabled() const { return enabled; }
    
    /**
     * @brief Get watchdog timeout value
     * @return Timeout in seconds
     */
    uint8_t getTimeout() const { return timeoutSec; }
    
    /**
     * @brief Get time until watchdog timeout
     * @return Milliseconds until timeout (approximate)
     */
    unsigned long getTimeUntilTimeout() const;
    
    /**
     * @brief Get last feed timestamp
     * @return Timestamp of last feed (millis)
     */
    unsigned long getLastFeedTime() const { return lastFeedTime; }
    
    /**
     * @brief Trigger immediate watchdog reset (for testing)
     * 
     * WARNING: This will reset the ESP32!
     */
    void triggerReset();

private:
    WatchdogTimer() : initialized(false), enabled(false), 
                      timeoutSec(WATCHDOG_TIMEOUT_SEC), lastFeedTime(0) {}
    ~WatchdogTimer() {}
    
    WatchdogTimer(const WatchdogTimer&) = delete;
    WatchdogTimer& operator=(const WatchdogTimer&) = delete;
    
    bool initialized;               ///< Initialization status
    bool enabled;                   ///< Enable status
    uint8_t timeoutSec;             ///< Timeout in seconds
    unsigned long lastFeedTime;     ///< Last feed timestamp
};
