/**
 * @file TimerManager.cpp
 * @brief Implementation of hardware timer abstraction
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "TimerManager.h"
#include "esp_task_wdt.h"

TimerManager& TimerManager::getInstance() {
    static TimerManager instance;
    return instance;
}

bool TimerManager::init() {
    Serial.println("[TimerManager] Initializing hardware timer and watchdog");
    
    // Initialize watchdog timer with 5 second timeout
    esp_err_t err = esp_task_wdt_init(WATCHDOG_TIMEOUT_MS / 1000, true);
    if (err != ESP_OK) {
        Serial.printf("[TimerManager] Failed to initialize watchdog: %d\n", err);
        return false;
    }
    
    // Add current task to watchdog
    err = esp_task_wdt_add(NULL);
    if (err != ESP_OK) {
        Serial.printf("[TimerManager] Failed to add task to watchdog: %d\n", err);
        return false;
    }
    
    watchdogEnabled = true;
    lastWatchdogFeed = millis();
    
    Serial.println("[TimerManager] Hardware timer and watchdog initialized");
    Serial.printf("[TimerManager] Watchdog timeout: %lu ms\n", WATCHDOG_TIMEOUT_MS);
    
    return true;
}

unsigned long TimerManager::getTicks() const {
    return millis();
}

void TimerManager::feedWatchdog() {
    if (!watchdogEnabled) {
        return;
    }
    
    esp_task_wdt_reset();
    lastWatchdogFeed = millis();
}

bool TimerManager::enableWatchdog() {
    if (watchdogEnabled) {
        return true;
    }
    
    esp_err_t err = esp_task_wdt_add(NULL);
    if (err != ESP_OK) {
        Serial.printf("[TimerManager] Failed to enable watchdog: %d\n", err);
        return false;
    }
    
    watchdogEnabled = true;
    lastWatchdogFeed = millis();
    Serial.println("[TimerManager] Watchdog enabled");
    
    return true;
}

bool TimerManager::disableWatchdog() {
    if (!watchdogEnabled) {
        return true;
    }
    
    esp_err_t err = esp_task_wdt_delete(NULL);
    if (err != ESP_OK) {
        Serial.printf("[TimerManager] Failed to disable watchdog: %d\n", err);
        return false;
    }
    
    watchdogEnabled = false;
    Serial.println("[TimerManager] Watchdog disabled");
    
    return true;
}
