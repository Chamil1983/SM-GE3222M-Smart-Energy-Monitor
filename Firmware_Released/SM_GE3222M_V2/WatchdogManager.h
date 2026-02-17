/**
 * @file WatchdogManager.h
 * @brief Hardware watchdog manager for SM-GE3222M V2.0
 * @details Singleton class for managing ESP32 Task Watchdog Timer (TWDT)
 */

#ifndef WATCHDOGMANAGER_H
#define WATCHDOGMANAGER_H

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class WatchdogManager {
public:
    static WatchdogManager& getInstance();
    
    bool init(uint32_t timeoutSeconds = 30);
    void feed();
    
    bool subscribeTask(TaskHandle_t taskHandle, const char* taskName = nullptr);
    bool unsubscribeTask(TaskHandle_t taskHandle);
    
    bool isEnabled() const { return _enabled; }
    uint32_t getTimeout() const { return _timeoutSeconds; }
    
private:
    WatchdogManager();
    ~WatchdogManager();
    WatchdogManager(const WatchdogManager&) = delete;
    WatchdogManager& operator=(const WatchdogManager&) = delete;
    
    static void panicHandler();

    // Track tasks successfully registered with TWDT.
    // This avoids calling esp_task_wdt_reset() from a task that isn't registered,
    // which produces: "task not found" and can stall the firmware with log spam.
    static constexpr int MAX_WDT_TASKS = 12;
    TaskHandle_t _subscribedTasks[MAX_WDT_TASKS];
    int _subscribedCount;
    bool isTaskSubscribed(TaskHandle_t h) const;
    void addSubscribed(TaskHandle_t h);
    void removeSubscribed(TaskHandle_t h);
    
    bool _enabled;
    bool _initialized;
    bool _ownsTwdt;
    uint32_t _timeoutSeconds;
    TaskHandle_t _mainTaskHandle;
};

#endif // WATCHDOGMANAGER_H
