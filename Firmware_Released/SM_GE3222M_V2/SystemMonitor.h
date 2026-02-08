/**
 * @file SystemMonitor.h
 * @brief System monitoring and metrics tracking for SM-GE3222M V2.0
 * @details Singleton class for monitoring system health, resource usage, and performance metrics
 */

#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include "DataTypes.h"

class SystemMonitor {
public:
    static SystemMonitor& getInstance();
    
    bool init();
    void update();
    
    SystemStatus getSystemStatus() const;
    String getUptimeString() const;
    
    void incrementErrorCount();
    void resetErrorCount();
    void setLastError(ErrorCode error);
    
    uint32_t getFreeHeap() const;
    uint32_t getMinFreeHeap() const;
    uint8_t getCpuFreqMHz() const;
    uint32_t getUptime() const;
    int getRSSI() const;
    uint16_t getErrorCount() const;
    
    UBaseType_t getTaskStackWatermark(TaskHandle_t taskHandle) const;
    
private:
    SystemMonitor();
    ~SystemMonitor();
    SystemMonitor(const SystemMonitor&) = delete;
    SystemMonitor& operator=(const SystemMonitor&) = delete;
    
    void updateHeapMetrics();
    void updateCpuMetrics();
    void updateNetworkMetrics();
    void updateTaskMetrics();
    
    SystemStatus _status;
    SemaphoreHandle_t _mutex;
    bool _initialized;
    uint32_t _bootTime;
};

#endif // SYSTEMMONITOR_H
