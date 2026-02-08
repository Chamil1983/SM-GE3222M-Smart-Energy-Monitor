/**
 * @file TaskManager.cpp
 * @brief Implementation of FreeRTOS task management
 */

#include "TaskManager.h"
#include "EnergyMeter.h"
#include "EnergyAccumulator.h"
#include "ModbusServer.h"
#include "TCPDataServer.h"
#include "WebServerManager.h"
#include "MQTTPublisher.h"
#include "ConfigManager.h"
#include "EventBus.h"

TaskManager& TaskManager::getInstance() {
    static TaskManager instance;
    return instance;
}

TaskManager::TaskManager()
    : _energyTask(nullptr)
    , _accumulatorTask(nullptr)
    , _modbusTask(nullptr)
    , _tcpServerTask(nullptr)
    , _webServerTask(nullptr)
    , _mqttTask(nullptr)
    , _diagnosticsTask(nullptr)
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
    
    // Create TCP Server Task (Core 0, Priority 3)
    result = xTaskCreatePinnedToCore(
        tcpServerTaskFunc,
        "TCPServerTask",
        TCP_STACK_SIZE,
        nullptr,
        TCP_PRIORITY,
        &_tcpServerTask,
        CORE_0
    );
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create TCPServerTask");
        return false;
    }
    Logger::getInstance().info("TaskManager: Created TCPServerTask on Core 0");
    
    // Create Web Server Task (Core 0, Priority 2)
    result = xTaskCreatePinnedToCore(
        webServerTaskFunc,
        "WebServerTask",
        WEB_STACK_SIZE,
        nullptr,
        WEB_PRIORITY,
        &_webServerTask,
        CORE_0
    );
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create WebServerTask");
        return false;
    }
    Logger::getInstance().info("TaskManager: Created WebServerTask on Core 0");
    
    // Create MQTT Task (Core 0, Priority 2)
    result = xTaskCreatePinnedToCore(
        mqttTaskFunc,
        "MQTTTask",
        MQTT_STACK_SIZE,
        nullptr,
        MQTT_PRIORITY,
        &_mqttTask,
        CORE_0
    );
    
    if (result != pdPASS) {
        Logger::getInstance().error("TaskManager: Failed to create MQTTTask");
        return false;
    }
    Logger::getInstance().info("TaskManager: Created MQTTTask on Core 0");
    
    // Create Diagnostics Task (Core 0, Priority 1)
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
        Logger::getInstance().error("TaskManager: Failed to create DiagnosticsTask");
        return false;
    }
    Logger::getInstance().info("TaskManager: Created DiagnosticsTask on Core 0");
    
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
    
    if (_mqttTask) {
        vTaskDelete(_mqttTask);
        _mqttTask = nullptr;
    }
    
    if (_webServerTask) {
        vTaskDelete(_webServerTask);
        _webServerTask = nullptr;
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
            accumulator.save();
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
    const TickType_t interval = pdMS_TO_TICKS(10);
    
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
    Logger::getInstance().info("TCPServerTask: Started (event-driven)");
    
    TCPDataServer& tcpServer = TCPDataServer::getInstance();
    
    while (true) {
        tcpServer.handle();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void TaskManager::webServerTaskFunc(void* param) {
    Logger::getInstance().info("WebServerTask: Started (10ms interval)");
    
    WebServerManager& webServer = WebServerManager::getInstance();
    
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(10);
    
    uint32_t broadcastCounter = 0;
    const uint32_t BROADCAST_INTERVAL = 100;  // Broadcast every 1 second
    
    while (true) {
        webServer.handle();
        
        broadcastCounter++;
        if (broadcastCounter >= BROADCAST_INTERVAL) {
            webServer.broadcastMeterData();
            broadcastCounter = 0;
        }
        
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}

void TaskManager::mqttTaskFunc(void* param) {
    Logger::getInstance().info("MQTTTask: Started");
    
    MQTTPublisher& mqtt = MQTTPublisher::getInstance();
    EnergyMeter& meter = EnergyMeter::getInstance();
    ConfigManager& config = ConfigManager::getInstance();
    
    while (true) {
        mqtt.handle();
        
        if (mqtt.isConnected()) {
            MQTTConfig mqttConfig = config.getMQTTConfig();
            uint32_t interval = mqttConfig.publishInterval * 1000;
            
            static uint32_t lastPublish = 0;
            uint32_t now = millis();
            
            if (now - lastPublish >= interval) {
                MeterData data = meter.getSnapshot();
                mqtt.publish(data);
                lastPublish = now;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void TaskManager::diagnosticsTaskFunc(void* param) {
    Logger::getInstance().info("DiagnosticsTask: Started (5000ms interval)");
    
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t interval = pdMS_TO_TICKS(5000);
    
    while (true) {
        uint32_t freeHeap = ESP.getFreeHeap();
        uint32_t minFreeHeap = ESP.getMinFreeHeap();
        
        Logger::getInstance().debug("Heap: %u bytes free (min: %u)", freeHeap, minFreeHeap);
        
        if (freeHeap < 10000) {
            Logger::getInstance().warn("Low heap memory: %u bytes", freeHeap);
        }
        
        vTaskDelayUntil(&lastWakeTime, interval);
    }
}
