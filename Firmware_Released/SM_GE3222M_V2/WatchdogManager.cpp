/**
 * @file WatchdogManager.cpp
 * @brief Watchdog manager implementation for SM-GE3222M V2.0
 */

#include "WatchdogManager.h"
#include "Logger.h"
#include "DataTypes.h"
#include <esp_system.h>

WatchdogManager& WatchdogManager::getInstance() {
    static WatchdogManager instance;
    return instance;
}

WatchdogManager::WatchdogManager() 
    : _enabled(false)
    , _initialized(false)
    , _timeoutSeconds(30)
    , _mainTaskHandle(nullptr) {
}

WatchdogManager::~WatchdogManager() {
    if (_initialized && _enabled) {
        esp_task_wdt_deinit();
    }
}

bool WatchdogManager::init(uint32_t timeoutSeconds) {
    if (_initialized) {
        Logger::getInstance().warn("WatchdogManager: Already initialized");
        return true;
    }
    
    _timeoutSeconds = timeoutSeconds;
    
    // Configure Task Watchdog Timer
    esp_task_wdt_config_t twdt_config = {
        .timeout_ms = _timeoutSeconds * 1000,
        .idle_core_mask = 0,  // Don't watch IDLE tasks
        .trigger_panic = true  // Trigger panic on timeout
    };
    
    esp_err_t err = esp_task_wdt_init(&twdt_config);
    if (err != ESP_OK) {
        Logger::getInstance().error("WatchdogManager: Init failed (err=%d)", err);
        return false;
    }
    
    // Subscribe main loop task (current task)
    _mainTaskHandle = xTaskGetCurrentTaskHandle();
    err = esp_task_wdt_add(_mainTaskHandle);
    if (err != ESP_OK) {
        Logger::getInstance().error("WatchdogManager: Failed to add main task (err=%d)", err);
        esp_task_wdt_deinit();
        return false;
    }
    
    _enabled = true;
    _initialized = true;
    
    Logger::getInstance().info("WatchdogManager: Initialized (timeout=%us)", _timeoutSeconds);
    
    return true;
}

void WatchdogManager::feed() {
    if (!_enabled || !_initialized) {
        return;
    }
    
    esp_err_t err = esp_task_wdt_reset();
    if (err != ESP_OK) {
        Logger::getInstance().warn("WatchdogManager: Feed failed (err=%d)", err);
    }
}

bool WatchdogManager::subscribeTask(TaskHandle_t taskHandle, const char* taskName) {
    if (!_initialized) {
        Logger::getInstance().error("WatchdogManager: Not initialized");
        return false;
    }
    
    if (taskHandle == nullptr) {
        Logger::getInstance().error("WatchdogManager: Invalid task handle");
        return false;
    }
    
    esp_err_t err = esp_task_wdt_add(taskHandle);
    if (err != ESP_OK) {
        Logger::getInstance().error("WatchdogManager: Failed to subscribe task %s (err=%d)", 
                                   taskName ? taskName : "unknown", err);
        return false;
    }
    
    Logger::getInstance().info("WatchdogManager: Subscribed task %s", 
                              taskName ? taskName : "unknown");
    return true;
}

bool WatchdogManager::unsubscribeTask(TaskHandle_t taskHandle) {
    if (!_initialized) {
        return false;
    }
    
    if (taskHandle == nullptr) {
        return false;
    }
    
    esp_err_t err = esp_task_wdt_delete(taskHandle);
    if (err != ESP_OK) {
        Logger::getInstance().warn("WatchdogManager: Failed to unsubscribe task (err=%d)", err);
        return false;
    }
    
    return true;
}

void WatchdogManager::panicHandler() {
    Logger::getInstance().error("WatchdogManager: WATCHDOG TIMEOUT - System will restart!");
    
    // Log additional debug info before panic
    Serial.println("\n\n=== WATCHDOG TIMEOUT ===");
    Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
    Serial.printf("Min Free Heap: %u bytes\n", ESP.getMinFreeHeap());
    Serial.println("========================\n");
    Serial.flush();
    
    // The system will panic and restart automatically
}
