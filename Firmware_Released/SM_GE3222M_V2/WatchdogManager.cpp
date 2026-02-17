/**
 * @file WatchdogManager.cpp
 * @brief Watchdog manager implementation for SM-GE3222M V2.0
 */

#include "WatchdogManager.h"
#include "Logger.h"
#include "DataTypes.h"
#include <esp_system.h>
#include <esp_err.h>

WatchdogManager& WatchdogManager::getInstance() {
    static WatchdogManager instance;
    return instance;
}

WatchdogManager::WatchdogManager()
    : _subscribedTasks{nullptr}
    , _subscribedCount(0)
    , _enabled(false)
    , _initialized(false)
    , _ownsTwdt(false)
    , _timeoutSeconds(30)
    , _mainTaskHandle(nullptr) {
}

WatchdogManager::~WatchdogManager() {
    if (_initialized && _enabled && _ownsTwdt) {
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
    if (err == ESP_OK) {
        _ownsTwdt = true;
    } else if (err == ESP_ERR_INVALID_STATE) {
        // TWDT may already be initialized by Arduino/ESP-IDF startup code.
        // Treat as non-fatal and continue by attaching our tasks.
        _ownsTwdt = false;
        Logger::getInstance().warn("WatchdogManager: TWDT already initialized (will attach tasks)");
    } else {
        Logger::getInstance().error("WatchdogManager: Init failed (err=%d)", err);
        return false;
    }
    
    

    // Note: If TWDT was already initialized by the Arduino core, we do NOT try to delete
    // loopTask from TWDT here because:
    //  - loopTask may not be registered (causes noisy "task not found" logs)
    //  - removing it is not required for stability in our design
    // We instead ensure that we only "feed" TWDT from tasks that we have subscribed.

    _enabled = true;
    _initialized = true;

    Logger::getInstance().info("WatchdogManager: Initialized (timeout=%us)", _timeoutSeconds);

    return true;
}

void WatchdogManager::feed() {
    if (!_enabled || !_initialized) {
        return;
    }

    // Only reset TWDT from a task that is actually subscribed.
    TaskHandle_t cur = xTaskGetCurrentTaskHandle();
    if (!isTaskSubscribed(cur)) {
        return; // don't spam logs or stall the firmware
    }

    esp_err_t err = esp_task_wdt_reset();
    if (err != ESP_OK) {
        // ESP_ERR_NOT_FOUND happens if the current task isn't registered; avoid log spam.
        if (err != ESP_ERR_NOT_FOUND) {
            Logger::getInstance().warn("WatchdogManager: Feed failed (err=%d)", err);
        }
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

    addSubscribed(taskHandle);
    
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

    removeSubscribed(taskHandle);
    
    return true;
}

bool WatchdogManager::isTaskSubscribed(TaskHandle_t h) const {
    if (h == nullptr) return false;
    for (int i = 0; i < _subscribedCount; ++i) {
        if (_subscribedTasks[i] == h) return true;
    }
    return false;
}

void WatchdogManager::addSubscribed(TaskHandle_t h) {
    if (h == nullptr) return;
    if (isTaskSubscribed(h)) return;
    if (_subscribedCount >= MAX_WDT_TASKS) return;
    _subscribedTasks[_subscribedCount++] = h;
}

void WatchdogManager::removeSubscribed(TaskHandle_t h) {
    if (h == nullptr) return;
    for (int i = 0; i < _subscribedCount; ++i) {
        if (_subscribedTasks[i] == h) {
            // compact
            for (int j = i; j < _subscribedCount - 1; ++j) {
                _subscribedTasks[j] = _subscribedTasks[j + 1];
            }
            _subscribedTasks[_subscribedCount - 1] = nullptr;
            _subscribedCount--;
            return;
        }
    }
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
