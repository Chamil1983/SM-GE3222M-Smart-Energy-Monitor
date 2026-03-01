/**
 * @file TaskManager.cpp
 * @brief Implementation of FreeRTOS task management
 */

#include "TaskManager.h"
#include "EnergyMeter.h"
#include "EnergyAccumulator.h"
#include "ModbusServer.h"
#include "TCPDataServer.h"
#include "WebUIManager.h"
#include "DHTSensorManager.h"
#include "SMNetworkManager.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <esp_task_wdt.h>
#endif
#include "ConfigManager.h"
#include "EventBus.h"
#include "WatchdogManager.h"
#include "DataLogger.h"

TaskManager& TaskManager::getInstance() {
    static TaskManager instance;
    return instance;
}

TaskManager::TaskManager()
    : _energyTask(nullptr)
    , _accumulatorTask(nullptr)
    , _modbusTask(nullptr)
    , _tcpServerTask(nullptr)
    , _mqttTask(nullptr)
    , _diagnosticsTask(nullptr)
    , _dhtTask(nullptr)
    , _webUiTask(nullptr)
    , _tasksRunning(false) {
}

TaskManager::~TaskManager() {
    stopAllTasks();
}

bool TaskManager::createAllTasks() {
    if (_tasksRunning) {
        Logger::getInstance().warn("TaskManager: Tasks already running");
        return true;
    }
    
    Logger::getInstance().info("TaskManager: Creating all tasks...");
    
    // Create Energy Task (Core 1, Priority 5)
    BaseType_t result = xTaskCreatePinnedToCore(
        energyTaskFunc,
        "EnergyTask",
        ENERGY_STACK_SIZE,
        nullptr,
        ENERGY_PRIORITY,
        &_energyTask,
        CORE_1
    );
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create EnergyTask");
        return false;
    }
    Logger::getInstance().info("TaskManager: Created EnergyTask on Core 1");
    
    // Create Accumulator Task (Core 1, Priority 4)
    result = xTaskCreatePinnedToCore(
        accumulatorTaskFunc,
        "AccumulatorTask",
        ACCUMULATOR_STACK_SIZE,
        nullptr,
        ACCUMULATOR_PRIORITY,
        &_accumulatorTask,
        CORE_1
    );
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create AccumulatorTask");
        return false;
    }
    Logger::getInstance().info("TaskManager: Created AccumulatorTask on Core 1");
    
    // Create Modbus Task (Core 1, Priority 3)
    result = xTaskCreatePinnedToCore(
        modbusTaskFunc,
        "ModbusTask",
        MODBUS_STACK_SIZE,
        nullptr,
        MODBUS_PRIORITY,
        &_modbusTask,
        CORE_1
    );
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create ModbusTask");
        return false;
    }
    Logger::getInstance().info("TaskManager: Created ModbusTask on Core 1");

    // Create TCP Server Task (Core 0, Priority 2) - OPTIONAL
    // In AP setup mode, keep this task disabled (server.begin is skipped and we want Core0 headroom for WiFi/DNS).
    if (networkManager.isAPMode()) {
        _tcpServerTask = nullptr;
        Logger::getInstance().info("TaskManager: TCPServerTask skipped in AP mode");
    } else {
        result = xTaskCreatePinnedToCore(
            tcpServerTaskFunc,
            "TCPServerTask",
            TCP_SERVER_STACK_SIZE,
            nullptr,
            TCP_SERVER_PRIORITY,
            &_tcpServerTask,
            CORE_0
        );

        if (result != pdPASS) {
            _tcpServerTask = nullptr;
            Logger::getInstance().warn("TaskManager: Failed to create TCPServerTask (optional) - TCP server task disabled");
        } else {
            Logger::getInstance().info("TaskManager: Created TCPServerTask on Core 0");
        }
    }

    // Create Web UI / DNS Service Task (Core 0, Priority 2) - OPTIONAL
    // Disabled by default for synchronous WebServer builds; streamFile() can block long enough to starve IDLE0 and
    // trigger task WDT in AP mode. Arduino loop() fallback remains active and is safer here.
    if (ENABLE_WEBUI_TASK) {
        result = xTaskCreatePinnedToCore(
            webUiTaskFunc,
            "WebUITask",
            WEBUI_STACK_SIZE,
            nullptr,
            WEBUI_PRIORITY,
            &_webUiTask,
            CORE_0
        );

        if (result != pdPASS) {
            _webUiTask = nullptr;
            Logger::getInstance().warn("TaskManager: Failed to create WebUITask (optional) - web UI will be serviced in loop()");
        } else {
            Logger::getInstance().info("TaskManager: Created WebUITask on Core 0");
        }
    } else {
        _webUiTask = nullptr;
        Logger::getInstance().warn("TaskManager: WebUITask disabled (synchronous HTTP serviced in loop for AP stability)");
    }

    // Create Diagnostics Task (Core 0, Priority 1) - OPTIONAL
    result = xTaskCreatePinnedToCore(
        diagnosticsTaskFunc,
        "DiagnosticsTask",
        DIAGNOSTICS_STACK_SIZE,
        nullptr,
        DIAGNOSTICS_PRIORITY,
        &_diagnosticsTask,
        CORE_0
    );

    if (result != pdPASS) {
        _diagnosticsTask = nullptr;
        Logger::getInstance().warn("TaskManager: Failed to create DiagnosticsTask (optional) - diagnostics task disabled");
    } else {
        Logger::getInstance().info("TaskManager: Created DiagnosticsTask on Core 0");
    }

    // Create DHT Sensor Task (Core 0, Priority 1) - OPTIONAL
    result = xTaskCreatePinnedToCore(
        dhtTaskFunc,
        "DHTTask",
        DHT_STACK_SIZE,
        nullptr,
        DHT_PRIORITY,
        &_dhtTask,
        CORE_0
    );

    if (result != pdPASS) {
        _dhtTask = nullptr;
        Logger::getInstance().warn("TaskManager: Failed to create DHTTask (optional) - DHT polling task disabled");
    } else {
        Logger::getInstance().info("TaskManager: Created DHTTask on Core 0");
    }

    
    _tasksRunning = true;
    Logger::getInstance().info("TaskManager: All tasks created successfully");
    return true;
}

void TaskManager::stopAllTasks() {
    if (!_tasksRunning) {
        return;
    }
    
    Logger::getInstance().info("TaskManager: Stopping all tasks...");
    
    if (_diagnosticsTask) {
        vTaskDelete(_diagnosticsTask);
        _diagnosticsTask = nullptr;
    }

    if (_dhtTask) {
        vTaskDelete(_dhtTask);
        _dhtTask = nullptr;
    }

    if (_webUiTask) {
        vTaskDelete(_webUiTask);
        _webUiTask = nullptr;
    }

    if (_mqttTask) {
        vTaskDelete(_mqttTask);
        _mqttTask = nullptr;
    }

    if (_tcpServerTask) {
        vTaskDelete(_tcpServerTask);
        _tcpServerTask = nullptr;
    }
    
    if (_modbusTask) {
        vTaskDelete(_modbusTask);
        _modbusTask = nullptr;
    }
    
    if (_accumulatorTask) {
        vTaskDelete(_accumulatorTask);
        _accumulatorTask = nullptr;
    }
    
    if (_energyTask) {
        vTaskDelete(_energyTask);
        _energyTask = nullptr;
    }
    
    _tasksRunning = false;
    Logger::getInstance().info("TaskManager: All tasks stopped");
}

// ============================================================================
// TASK IMPLEMENTATIONS
// ============================================================================

void TaskManager::energyTaskFunc(void* param) {
    Logger::getInstance().info("EnergyTask: Started (500ms interval)");
    EnergyMeter& meter = EnergyMeter::getInstance();
    EventBus& eventBus = EventBus::getInstance();
    
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(500);
    
    while (true) {
        if (meter.update()) {
            MeterData data = meter.getSnapshot();
            DataLogger::getInstance().logReading(data);
            eventBus.publish(EventType::METER_DATA_UPDATED, &data, sizeof(data));
        }
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}

void TaskManager::accumulatorTaskFunc(void* param) {
    Logger::getInstance().info("AccumulatorTask: Started (1000ms interval)");
    EnergyAccumulator& accumulator = EnergyAccumulator::getInstance();
    EnergyMeter& meter = EnergyMeter::getInstance();
    
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(1000);
    
    uint32_t autoSaveCounter = 0;
    const uint32_t AUTO_SAVE_INTERVAL = 300;  // Save every 5 minutes
    
    while (true) {
        MeterData data = meter.getSnapshot();
        accumulator.update(data);
        
        autoSaveCounter++;
        if (autoSaveCounter >= AUTO_SAVE_INTERVAL) {
            accumulator.saveToNVS();
            autoSaveCounter = 0;
        }
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}

void TaskManager::modbusTaskFunc(void* param) {
    Logger::getInstance().info("ModbusTask: Started (10ms poll)");
    ModbusServer& modbus = ModbusServer::getInstance();
    EnergyMeter& meter = EnergyMeter::getInstance();
    
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(50);
    
    uint32_t updateCounter = 0;
    const uint32_t UPDATE_INTERVAL = 50;  // Update every 500ms
    
    while (true) {
        modbus.handle();
        
        updateCounter++;
        if (updateCounter >= UPDATE_INTERVAL) {
            MeterData data = meter.getSnapshot();
            modbus.updateMeterData(data);
            updateCounter = 0;
        }
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}




void TaskManager::tcpServerTaskFunc(void* param) {
    Logger::getInstance().info("TCPServerTask: Started (20ms poll)");
    TCPDataServer& server = TCPDataServer::getInstance();

    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(20);

    while (true) {
        // handle() is safe even if begin() wasn't called; it will just do nothing.
        server.handle();
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}

void TaskManager::webUiTaskFunc(void* param) {
    Logger::getInstance().info("WebUITask: Started (10ms poll)");
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(10);

    while (true) {
        // Service DNS captive portal + synchronous HTTP server from a single context (Core 0).
        networkManager.loop();
        WebUIManager::getInstance().loop();
#if defined(ARDUINO_ARCH_ESP32)
        // If ever enabled, reset TWDT for this task and yield once more to let IDLE0 run.
        esp_task_wdt_reset();
#endif
        taskYIELD();
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}

void TaskManager::dhtTaskFunc(void* param) {
    Logger::getInstance().info("DHTTask: Started (500ms scheduler, GPIO4 DHT22)");
    DHTSensorManager& dht = DHTSensorManager::getInstance();

    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(500);

    while (true) {
        dht.poll();
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}

void TaskManager::mqttTaskFunc(void* param) {
    // Placeholder to keep API compatibility; MQTT can be added/enabled later.
    Logger::getInstance().info("MQTTTask: Started (disabled stub)");
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void TaskManager::diagnosticsTaskFunc(void* param) {
    Logger::getInstance().info("DiagnosticsTask: Started (5000ms interval)");
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(5000);
    
    while (true) {
        uint32_t freeHeap = ESP.getFreeHeap();
        uint32_t minFreeHeap = ESP.getMinFreeHeap();
        
        static uint32_t lastLogMs = 0;
        uint32_t nowMs = millis();
        if (nowMs - lastLogMs > 60000) {
            lastLogMs = nowMs;
            Logger::getInstance().info("Heap: %u bytes free (min: %u)", freeHeap, minFreeHeap);
            auto dht = DHTSensorManager::getInstance().getSnapshot();
            Logger::getInstance().info("DHT22 status: en=%s valid=%s T=%.1fC RH=%.1f%% ok=%lu fail=%lu age=%lums",
                                       dht.enabled ? "Y" : "N",
                                       dht.valid ? "Y" : "N",
                                       dht.temperatureC, dht.humidityPct,
                                       (unsigned long)dht.successCount,
                                       (unsigned long)dht.failCount,
                                       (unsigned long)(dht.lastGoodReadMs ? (millis() - dht.lastGoodReadMs) : 0));
        }
        
        if (freeHeap < 10000) {
            Logger::getInstance().warn("Low heap memory: %u bytes", freeHeap);
        }
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}