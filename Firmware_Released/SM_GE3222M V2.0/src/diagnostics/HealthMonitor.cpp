/**
 * @file HealthMonitor.cpp
 * @brief Runtime health monitoring implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "HealthMonitor.h"
#include "Logger.h"
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <WiFi.h>

HealthMonitor::HealthMonitor()
    : initialized(false),
      lastUpdateTime(0),
      bootTime(0),
      lastIdleTime(0),
      lastTotalTime(0) {
}

HealthMonitor& HealthMonitor::getInstance() {
    static HealthMonitor instance;
    return instance;
}

bool HealthMonitor::init() {
    bootTime = millis();
    
    // Load restart count from NVS
    loadRestartCount();
    
    // Increment and save restart count
    healthData.restartCount++;
    saveRestartCount();
    
    // Initialize heap tracking
    healthData.freeHeap = ESP.getFreeHeap();
    healthData.minFreeHeap = healthData.freeHeap;
    
    initialized = true;
    LOG_INFO("HealthMon", "Initialized - Restart #%lu", healthData.restartCount);
    return true;
}

void HealthMonitor::update() {
    if (!initialized) return;
    
    unsigned long now = millis();
    
    // Throttle updates to 1Hz
    if (now - lastUpdateTime < 1000) {
        return;
    }
    
    // Update uptime
    healthData.uptime = (now - bootTime) / 1000;
    
    // Update heap stats
    updateHeapStats();
    
    // Update CPU load
    updateCPULoad();
    
    lastUpdateTime = now;
}

void HealthMonitor::updateHeapStats() {
    healthData.freeHeap = ESP.getFreeHeap();
    
    // Track minimum
    if (healthData.freeHeap < healthData.minFreeHeap) {
        healthData.minFreeHeap = healthData.freeHeap;
        
        // Log warning if below threshold
        if (healthData.minFreeHeap < HEAP_WARNING_THRESHOLD) {
            LOG_WARN("HealthMon", "Low heap: %lu bytes", healthData.minFreeHeap);
        }
    }
}

void HealthMonitor::updateCPULoad() {
    // Simple CPU load estimation based on idle task runtime
    // This is a rough estimate for ESP32
    
    // Get current CPU time
    unsigned long currentTotal = millis();
    
    // On ESP32, we can estimate load as a percentage of loop execution
    // For now, use a simplified approach
    // A more accurate method would use vTaskGetRunTimeStats()
    
    // Calculate based on time difference
    if (lastTotalTime > 0) {
        unsigned long deltaTotal = currentTotal - lastTotalTime;
        
        // Estimate: assume if we're updating regularly, load is moderate
        // This is a placeholder - real implementation would use FreeRTOS stats
        healthData.cpuLoad = 25.0; // Placeholder value
    }
    
    lastTotalTime = currentTotal;
}

void HealthMonitor::setWiFiStatus(bool connected, int rssi) {
    healthData.wifiConnected = connected;
    healthData.wifiRSSI = rssi;
}

void HealthMonitor::setMQTTStatus(bool connected) {
    healthData.mqttConnected = connected;
}

void HealthMonitor::setEthernetStatus(bool linked) {
    healthData.ethernetLinked = linked;
}

bool HealthMonitor::isHealthy() const {
    if (!initialized) return false;
    
    // Check all health thresholds
    bool healthy = true;
    
    // Heap check
    if (healthData.freeHeap < HEAP_WARNING_THRESHOLD) {
        healthy = false;
    }
    
    // CPU load check
    if (healthData.cpuLoad > CPU_WARNING_THRESHOLD) {
        healthy = false;
    }
    
    // Task overrun check
    if (healthData.taskOverruns > TASK_OVERRUN_WARNING) {
        healthy = false;
    }
    
    // WiFi RSSI check (only if connected)
    if (healthData.wifiConnected && healthData.wifiRSSI < RSSI_WARNING_THRESHOLD) {
        healthy = false;
    }
    
    return healthy;
}

bool HealthMonitor::getWarnings(char* buffer, size_t bufSize) {
    buffer[0] = '\0';
    bool hasWarnings = false;
    size_t offset = 0;
    
    // Check heap
    if (healthData.freeHeap < HEAP_WARNING_THRESHOLD) {
        offset += snprintf(buffer + offset, bufSize - offset, 
                          "Low heap: %lu KB; ", healthData.freeHeap / 1024);
        hasWarnings = true;
    }
    
    // Check CPU load
    if (healthData.cpuLoad > CPU_WARNING_THRESHOLD) {
        offset += snprintf(buffer + offset, bufSize - offset,
                          "High CPU: %.0f%%; ", healthData.cpuLoad);
        hasWarnings = true;
    }
    
    // Check task overruns
    if (healthData.taskOverruns > TASK_OVERRUN_WARNING) {
        offset += snprintf(buffer + offset, bufSize - offset,
                          "Task overruns: %lu; ", healthData.taskOverruns);
        hasWarnings = true;
    }
    
    // Check WiFi RSSI
    if (healthData.wifiConnected && healthData.wifiRSSI < RSSI_WARNING_THRESHOLD) {
        offset += snprintf(buffer + offset, bufSize - offset,
                          "Weak WiFi: %d dBm; ", healthData.wifiRSSI);
        hasWarnings = true;
    }
    
    return hasWarnings;
}

void HealthMonitor::getHealthJSON(char* buffer, size_t bufSize) {
    snprintf(buffer, bufSize,
             "{"
             "\"freeHeap\":%lu,"
             "\"minFreeHeap\":%lu,"
             "\"uptime\":%lu,"
             "\"restartCount\":%lu,"
             "\"cpuLoad\":%.1f,"
             "\"taskOverruns\":%lu,"
             "\"wifiRSSI\":%d,"
             "\"wifiConnected\":%s,"
             "\"mqttConnected\":%s,"
             "\"ethernetLinked\":%s,"
             "\"spiErrors\":%lu,"
             "\"i2cErrors\":%lu,"
             "\"healthy\":%s"
             "}",
             healthData.freeHeap,
             healthData.minFreeHeap,
             healthData.uptime,
             healthData.restartCount,
             healthData.cpuLoad,
             healthData.taskOverruns,
             healthData.wifiRSSI,
             healthData.wifiConnected ? "true" : "false",
             healthData.mqttConnected ? "true" : "false",
             healthData.ethernetLinked ? "true" : "false",
             healthData.spiErrors,
             healthData.i2cErrors,
             isHealthy() ? "true" : "false");
}

void HealthMonitor::loadRestartCount() {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("health", NVS_READONLY, &handle);
    
    if (err == ESP_OK) {
        uint32_t count = 0;
        err = nvs_get_u32(handle, "restart_cnt", &count);
        if (err == ESP_OK) {
            healthData.restartCount = count;
        }
        nvs_close(handle);
    }
    
    // If failed to read, start at 0
    if (err != ESP_OK) {
        healthData.restartCount = 0;
    }
}

void HealthMonitor::saveRestartCount() {
    nvs_handle_t handle;
    esp_err_t err = nvs_open("health", NVS_READWRITE, &handle);
    
    if (err == ESP_OK) {
        nvs_set_u32(handle, "restart_cnt", healthData.restartCount);
        nvs_commit(handle);
        nvs_close(handle);
    } else {
        LOG_ERROR("HealthMon", "Failed to save restart count");
    }
}
