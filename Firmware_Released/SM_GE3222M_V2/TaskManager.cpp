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
#include <esp_heap_caps.h>
#endif
#include "ConfigManager.h"
#include "EventBus.h"
#include "WatchdogManager.h"
#include "DataLogger.h"


namespace {
static BaseType_t createPinnedTaskWithFallback(TaskFunction_t fn, const char* name, const uint32_t* stacks, size_t nStacks,
                                              UBaseType_t prio, TaskHandle_t* outHandle, BaseType_t preferredCore) {
    if (!fn || !name || !outHandle || !stacks || nStacks == 0) return errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;

    // Robust core fallback: some ESP32 Arduino builds are unicore (Core 0 only), so pinning to Core 1 fails even with ample heap.
    BaseType_t coresToTry[3];
    size_t nCores = 0;
    coresToTry[nCores++] = preferredCore;
    if (preferredCore != tskNO_AFFINITY) coresToTry[nCores++] = tskNO_AFFINITY;
    if (preferredCore != 0 && preferredCore != tskNO_AFFINITY) coresToTry[nCores++] = 0;

    for (size_t ci = 0; ci < nCores; ++ci) {
        BaseType_t core = coresToTry[ci];
        for (size_t i = 0; i < nStacks; ++i) {
            *outHandle = nullptr;
            BaseType_t r = xTaskCreatePinnedToCore(fn, name, stacks[i], nullptr, prio, outHandle, core);
            if (r == pdPASS) {
                Logger::getInstance().info("TaskManager: %s created stack=%u core=%d", name, (unsigned)stacks[i], (int)core);
                return r;
            }
            Logger::getInstance().warn("TaskManager: %s create failed stack=%u core=%d", name, (unsigned)stacks[i], (int)core);
            delay(1);
        }
    }
    return errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
}

static BaseType_t createOptionalPinnedTask(TaskFunction_t fn, const char* name, uint32_t stack,
                                           UBaseType_t prio, TaskHandle_t* outHandle, BaseType_t preferredCore) {
    const uint32_t stacks[] = { stack, (stack > 3072 ? 3072U : stack), (stack > 2560 ? 2560U : stack), (stack > 2048 ? 2048U : stack) };
    // Deduplicate stack values while preserving order
    uint32_t uniq[4]; size_t n=0;
    for (uint32_t v : stacks) { bool exists=false; for (size_t i=0;i<n;++i) if (uniq[i]==v) { exists=true; break; } if (!exists) uniq[n++]=v; }
    return createPinnedTaskWithFallback(fn, name, uniq, n, prio, outHandle, preferredCore);
}
}

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
#if defined(ARDUINO_ARCH_ESP32)
    Logger::getInstance().info("TaskManager: Heap free=%u internal=%u", (unsigned)ESP.getFreeHeap(), (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
#endif
    
    // Create Energy Task (Core 1, Priority 5)
    const uint32_t energyStacks[] = { ENERGY_STACK_SIZE, 3072, 2560, 2048 };
    BaseType_t result = createPinnedTaskWithFallback(
        energyTaskFunc, "EnergyTask", energyStacks, sizeof(energyStacks)/sizeof(energyStacks[0]),
        ENERGY_PRIORITY, &_energyTask, CORE_1);
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create EnergyTask (all stack/core fallbacks exhausted)");
        return false;
    }
    Logger::getInstance().info("TaskManager: EnergyTask created");
    
    // Create Accumulator Task (Core 1, Priority 4)
    {
        const uint32_t stacks[] = { ACCUMULATOR_STACK_SIZE, 3072, 2560, 2048 };
        result = createPinnedTaskWithFallback(
            accumulatorTaskFunc, "AccumulatorTask", stacks, sizeof(stacks)/sizeof(stacks[0]),
            ACCUMULATOR_PRIORITY, &_accumulatorTask, CORE_1);
    }
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create AccumulatorTask");
        if (_energyTask) { vTaskDelete(_energyTask); _energyTask = nullptr; }
        return false;
    }
    Logger::getInstance().info("TaskManager: AccumulatorTask created");
    
    // Create Modbus Task (Core 1, Priority 3)
    {
        const uint32_t stacks[] = { MODBUS_STACK_SIZE, 3072, 2560, 2048 };
        result = createPinnedTaskWithFallback(
            modbusTaskFunc, "ModbusTask", stacks, sizeof(stacks)/sizeof(stacks[0]),
            MODBUS_PRIORITY, &_modbusTask, CORE_1);
    }
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create ModbusTask");
        if (_accumulatorTask) { vTaskDelete(_accumulatorTask); _accumulatorTask = nullptr; }
        if (_energyTask) { vTaskDelete(_energyTask); _energyTask = nullptr; }
        return false;
    }
    Logger::getInstance().info("TaskManager: ModbusTask created");

    // Create TCP Server Task (Core 0, Priority 2) - OPTIONAL
    // In AP setup mode, keep this task disabled (server.begin is skipped and we want Core0 headroom for WiFi/DNS).
    if (networkManager.isAPMode()) {
        _tcpServerTask = nullptr;
        Logger::getInstance().info("TaskManager: TCPServerTask skipped in AP mode");
    } else {
        result = createOptionalPinnedTask(
            tcpServerTaskFunc,
            "TCPServerTask",
            TCP_SERVER_STACK_SIZE,
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
    // In AP setup mode with synchronous WebServer fallback, a dedicated pump task improves DNS/HTTP responsiveness
    // and avoids dependence on Arduino loop timing (LCD updates, etc.). Root page in AP mode is small (not SPIFFS-heavy).
    const bool enableWebUiTaskNow = ENABLE_WEBUI_TASK;
    if (enableWebUiTaskNow) {
        result = createOptionalPinnedTask(
            webUiTaskFunc,
            "WebUITask",
            WEBUI_STACK_SIZE,
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
        Logger::getInstance().warn("TaskManager: WebUITask disabled (HTTP/DNS serviced in loop fallback)");
    }

    // Create Diagnostics Task (Core 0, Priority 1) - OPTIONAL
    result = createOptionalPinnedTask(
        diagnosticsTaskFunc,
        "DiagnosticsTask",
        DIAGNOSTICS_STACK_SIZE,
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
    result = createOptionalPinnedTask(
        dhtTaskFunc,
        "DHTTask",
        DHT_STACK_SIZE,
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
        // TWDT feed disabled here: some ESP32 core 3.x builds produce task_wdt "task not found" storms
        // in this optional task path, which can starve WiFi/AP servicing.
        // WatchdogManager::getInstance().feed();
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