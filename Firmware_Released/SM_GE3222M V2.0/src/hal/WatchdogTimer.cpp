/**
 * @file WatchdogTimer.cpp
 * @brief Implementation of WatchdogTimer
 */

#include "WatchdogTimer.h"
#include "../diagnostics/Logger.h"

WatchdogTimer& WatchdogTimer::getInstance() {
    static WatchdogTimer instance;
    return instance;
}

bool WatchdogTimer::init(uint8_t timeoutSec) {
    if (initialized) {
        LOG_WARN("WatchdogTimer", "Already initialized");
        return true;
    }
    
    this->timeoutSec = timeoutSec;
    
    // Configure ESP32 Task Watchdog Timer (TWDT)
    esp_err_t err = esp_task_wdt_init(timeoutSec, true);  // true = panic on timeout
    if (err != ESP_OK) {
        LOG_ERROR("WatchdogTimer", "Failed to initialize TWDT: %d", err);
        return false;
    }
    
    initialized = true;
    lastFeedTime = millis();
    
    LOG_INFO("WatchdogTimer", "Initialized with timeout: %d seconds", timeoutSec);
    return true;
}

bool WatchdogTimer::enable() {
    if (!initialized) {
        LOG_ERROR("WatchdogTimer", "Not initialized");
        return false;
    }
    
    // Watchdog is enabled by default after init
    // This function is for re-enabling after disable
    enabled = true;
    lastFeedTime = millis();
    
    LOG_INFO("WatchdogTimer", "Watchdog enabled");
    return true;
}

bool WatchdogTimer::disable() {
    if (!initialized) {
        LOG_ERROR("WatchdogTimer", "Not initialized");
        return false;
    }
    
    // Note: ESP32 TWDT cannot be completely disabled at runtime
    // We can only unsubscribe tasks
    // Setting enabled flag to prevent feeding
    enabled = false;
    
    LOG_WARN("WatchdogTimer", "Watchdog disabled (tasks still monitored)");
    return true;
}

void WatchdogTimer::feed() {
    if (!initialized || !enabled) {
        return;
    }
    
    // Reset the watchdog timer
    esp_task_wdt_reset();
    lastFeedTime = millis();
    
    LOG_DEBUG("WatchdogTimer", "Watchdog fed");
}

bool WatchdogTimer::addTask(const char* taskName) {
    if (!initialized) {
        LOG_ERROR("WatchdogTimer", "Not initialized");
        return false;
    }
    
    // Add current task to TWDT monitoring
    esp_err_t err = esp_task_wdt_add(nullptr);  // nullptr = current task
    if (err != ESP_OK) {
        if (err == ESP_ERR_INVALID_STATE) {
            // Task already added
            return true;
        }
        LOG_ERROR("WatchdogTimer", "Failed to add task: %d", err);
        return false;
    }
    
    if (taskName != nullptr) {
        LOG_INFO("WatchdogTimer", "Added task '%s' to watchdog monitoring", taskName);
    } else {
        LOG_INFO("WatchdogTimer", "Added current task to watchdog monitoring");
    }
    
    return true;
}

bool WatchdogTimer::removeTask() {
    if (!initialized) {
        LOG_ERROR("WatchdogTimer", "Not initialized");
        return false;
    }
    
    // Remove current task from TWDT monitoring
    esp_err_t err = esp_task_wdt_delete(nullptr);  // nullptr = current task
    if (err != ESP_OK) {
        LOG_ERROR("WatchdogTimer", "Failed to remove task: %d", err);
        return false;
    }
    
    LOG_INFO("WatchdogTimer", "Removed task from watchdog monitoring");
    return true;
}

void WatchdogTimer::feedTask() {
    if (!initialized || !enabled) {
        return;
    }
    
    // Reset watchdog for current task
    esp_task_wdt_reset();
    
    LOG_DEBUG("WatchdogTimer", "Task watchdog fed");
}

unsigned long WatchdogTimer::getTimeUntilTimeout() const {
    if (!initialized || !enabled) {
        return 0;
    }
    
    unsigned long elapsed = millis() - lastFeedTime;
    unsigned long timeoutMs = (unsigned long)timeoutSec * 1000;
    
    if (elapsed >= timeoutMs) {
        return 0;
    }
    
    return timeoutMs - elapsed;
}

void WatchdogTimer::triggerReset() {
    LOG_WARN("WatchdogTimer", "Triggering immediate watchdog reset!");
    
    // Disable watchdog feeding and wait for timeout
    enabled = false;
    
    // Or trigger immediate reset
    esp_restart();
}
