/**
 * @file SystemMonitor.cpp
 * @brief System monitoring implementation for SM-GE3222M V2.0
 */

#include "SystemMonitor.h"
#include "Logger.h"
#include "NetworkManager.h"
#include "TaskManager.h"
#include <esp_heap_caps.h>
#include <Preferences.h>

SystemMonitor& SystemMonitor::getInstance() {
    static SystemMonitor instance;
    return instance;
}

SystemMonitor::SystemMonitor() 
    : _mutex(nullptr)
    , _initialized(false)
    , _bootTime(0) {
    memset(&_status, 0, sizeof(SystemStatus));
}

SystemMonitor::~SystemMonitor() {
    if (_mutex) {
        vSemaphoreDelete(_mutex);
    }
}

bool SystemMonitor::init() {
    if (_initialized) {
        return true;
    }
    
    _mutex = xSemaphoreCreateMutex();
    if (!_mutex) {
        Logger::getInstance().error("SystemMonitor: Failed to create mutex");
        return false;
    }
    
    _bootTime = millis();
    
    // Load boot count from NVS
    Preferences prefs;
    if (prefs.begin("system", false)) {
        _status.bootCount = prefs.getUInt("bootCount", 0) + 1;
        prefs.putUInt("bootCount", _status.bootCount);
        prefs.end();
    }
    
    // Initialize status
    _status.lastError = ErrorCode::NONE;
    _status.errorCount = 0;
    
    _initialized = true;
    Logger::getInstance().info("SystemMonitor: Initialized (boot #%u)", _status.bootCount);
    
    return true;
}

void SystemMonitor::update() {
    if (!_initialized) {
        return;
    }
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        updateHeapMetrics();
        updateCpuMetrics();
        updateNetworkMetrics();
        updateTaskMetrics();
        
        _status.uptime = (millis() - _bootTime) / 1000;
        
        xSemaphoreGive(_mutex);
    }
}

void SystemMonitor::updateHeapMetrics() {
    _status.freeHeap = ESP.getFreeHeap();
    _status.minFreeHeap = ESP.getMinFreeHeap();
    
    // Check for low heap condition
    if (_status.freeHeap < 10000) {
        Logger::getInstance().warn("SystemMonitor: Low heap memory: %u bytes", _status.freeHeap);
    }
}

void SystemMonitor::updateCpuMetrics() {
    _status.cpuFreqMHz = ESP.getCpuFreqMHz();
    
    // Get internal temperature (if available on ESP32)
#ifdef CONFIG_IDF_TARGET_ESP32S2
    _status.cpuTemperature = temperatureRead();
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    _status.cpuTemperature = temperatureRead();
#else
    _status.cpuTemperature = 0.0f;  // Not available on ESP32
#endif
}

void SystemMonitor::updateNetworkMetrics() {
    NetworkManager& netMgr = NetworkManager::getInstance();
    _status.wifiConnected = netMgr.isConnected();
    
    // MQTT status would be set externally by MQTTPublisher
    // _status.mqttConnected is set via external call
}

void SystemMonitor::updateTaskMetrics() {
    // Task metrics can be queried on-demand via getTaskStackWatermark()
    // No need to store all watermarks in SystemStatus
}

SystemStatus SystemMonitor::getSystemStatus() const {
    SystemStatus status;
    if (_initialized && xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        status = _status;
        xSemaphoreGive(_mutex);
    }
    return status;
}

String SystemMonitor::getUptimeString() const {
    uint32_t uptime = getUptime();
    
    uint32_t days = uptime / 86400;
    uptime %= 86400;
    uint32_t hours = uptime / 3600;
    uptime %= 3600;
    uint32_t minutes = uptime / 60;
    
    char buffer[32];
    if (days > 0) {
        snprintf(buffer, sizeof(buffer), "%ud %uh %um", days, hours, minutes);
    } else if (hours > 0) {
        snprintf(buffer, sizeof(buffer), "%uh %um", hours, minutes);
    } else {
        snprintf(buffer, sizeof(buffer), "%um", minutes);
    }
    
    return String(buffer);
}

void SystemMonitor::incrementErrorCount() {
    if (_initialized && xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        _status.errorCount++;
        xSemaphoreGive(_mutex);
    }
}

void SystemMonitor::resetErrorCount() {
    if (_initialized && xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        _status.errorCount = 0;
        Logger::getInstance().info("SystemMonitor: Error count reset");
        xSemaphoreGive(_mutex);
    }
}

void SystemMonitor::setLastError(ErrorCode error) {
    if (_initialized && xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        _status.lastError = error;
        xSemaphoreGive(_mutex);
    }
}

uint32_t SystemMonitor::getFreeHeap() const {
    return _status.freeHeap;
}

uint32_t SystemMonitor::getMinFreeHeap() const {
    return _status.minFreeHeap;
}

uint8_t SystemMonitor::getCpuFreqMHz() const {
    return _status.cpuFreqMHz;
}

uint32_t SystemMonitor::getUptime() const {
    return _status.uptime;
}

int SystemMonitor::getRSSI() const {
    return NetworkManager::getInstance().getRSSI();
}

uint16_t SystemMonitor::getErrorCount() const {
    return _status.errorCount;
}

UBaseType_t SystemMonitor::getTaskStackWatermark(TaskHandle_t taskHandle) const {
    if (taskHandle == nullptr) {
        return 0;
    }
    return uxTaskGetStackHighWaterMark(taskHandle);
}
