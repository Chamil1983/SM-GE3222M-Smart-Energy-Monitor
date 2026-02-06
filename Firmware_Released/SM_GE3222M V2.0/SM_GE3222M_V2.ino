/**
 * @file SM_GE3222M_V2.ino
 * @brief Main entry point for SM-GE3222M Smart Energy Monitor V2.0
 * 
 * Production-grade firmware with complete architecture overhaul
 * 
 * @author Microcode Engineering
 * @version 2.0.0
 * @date 2024
 */

// ============================================================
// Include All Subsystem Headers
// ============================================================

// Core subsystem
#include "src/core/SystemManager.h"
#include "src/core/TaskScheduler.h"
#include "src/core/ErrorHandler.h"
#include "src/core/TimerManager.h"

// Hardware Abstraction Layer
#include "src/hal/BoardConfig.h"
#include "src/hal/SPIBus.h"
#include "src/hal/I2CBus.h"
#include "src/hal/GPIOManager.h"

// Energy subsystem
#include "src/energy/ATM90E36Driver.h"
#include "src/energy/EnergyMeter.h"
#include "src/energy/Calibration.h"
#include "src/energy/PowerQuality.h"
#include "src/energy/EnergyAccumulator.h"

// Storage subsystem
#include "src/storage/NVStorage.h"
#include "src/storage/ConfigManager.h"
#include "src/storage/DataLogger.h"
#include "src/storage/SPIFFSManager.h"

// Communication subsystem
#include "src/comm/WiFiManager.h"
#include "src/comm/EthernetManager.h"
#include "src/comm/WebServerManager.h"
#include "src/comm/MQTTClient.h"
#include "src/comm/ModbusRTUServer.h"
#include "src/comm/ModbusTCPServer.h"
#include "src/comm/OTAManager.h"

// Display subsystem
#include "src/display/LCDManager.h"
#include "src/display/LEDIndicator.h"

// Diagnostics subsystem
#include "src/diagnostics/Logger.h"
#include "src/diagnostics/SelfTest.h"
#include "src/diagnostics/HealthMonitor.h"

// Security subsystem
#include "src/security/AuthManager.h"

// Include files
#include "include/Version.h"
#include "include/PinMap.h"
#include "include/GlobalTypes.h"

// ============================================================
// Task Callbacks
// ============================================================

/**
 * @brief Read energy data from ATM90E36
 */
void taskEnergyRead() {
    EnergyMeter::getInstance().update();
}

/**
 * @brief Poll Modbus RTU requests
 */
void taskModbusRTUPoll() {
    ModbusRTUServer::getInstance().update();
}

/**
 * @brief Poll Modbus TCP requests
 */
void taskModbusTCPPoll() {
    ModbusTCPServer::getInstance().update();
}

/**
 * @brief Push data to WebSocket clients
 */
void taskWebSocketPush() {
    MeterData data = EnergyMeter::getInstance().getMeterData();
    WebServerManager::getInstance().pushData(EnergyMeter::getInstance().getJSON());
}

/**
 * @brief Update LCD display
 */
void taskLCDUpdate() {
    MeterData data = EnergyMeter::getInstance().getMeterData();
    LCDManager::getInstance().displayMeterData(data);
    LCDManager::getInstance().update();
}

/**
 * @brief Update LED indicators
 */
void taskLEDUpdate() {
    LEDIndicator::getInstance().update();
}

/**
 * @brief Check WiFi connection and reconnect if needed
 */
void taskWiFiCheck() {
    WiFiManager::getInstance().update();
}

/**
 * @brief Check Ethernet connection
 */
void taskEthernetCheck() {
    EthernetManager::getInstance().update();
}

/**
 * @brief Update MQTT client
 */
void taskMQTTUpdate() {
    MQTTClient::getInstance().update();
    
    // Publish meter data if connected
    if (MQTTClient::getInstance().isConnected()) {
        MeterData data = EnergyMeter::getInstance().getMeterData();
        MQTTClient::getInstance().publishMeterData(data);
    }
}

/**
 * @brief Perform health check
 */
void taskHealthCheck() {
    HealthMonitor::getInstance().update();
    
    // Log health warnings
    if (!HealthMonitor::getInstance().isHealthy()) {
        HealthData health = HealthMonitor::getInstance().getHealthData();
        LOG_WARN("System", "Health check warnings detected - Heap: %d bytes", health.freeHeap);
    }
}

/**
 * @brief Log data to SPIFFS
 */
void taskDataLog() {
    MeterData data = EnergyMeter::getInstance().getMeterData();
    DataLogger::getInstance().log(data);
}

/**
 * @brief Persist energy accumulators
 */
void taskEnergyPersist() {
    MeterData data = EnergyMeter::getInstance().getMeterData();
    EnergyAccumulator::getInstance().update(data);
    EnergyAccumulator::getInstance().save();
}

/**
 * @brief Analyze power quality
 */
void taskPowerQualityAnalysis() {
    MeterData data = EnergyMeter::getInstance().getMeterData();
    PowerQuality::getInstance().analyze(data);
}

/**
 * @brief Handle OTA updates
 */
void taskOTAHandle() {
    OTAManager::getInstance().update();
}

/**
 * @brief Update GPIO manager for LED patterns and buttons
 */
void taskGPIOUpdate() {
    GPIOManager::getInstance().updateLEDs();
}

// ============================================================
// Arduino Setup Function
// ============================================================

void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);
    while (!Serial && millis() < 3000); // Wait up to 3s for Serial
    
    Serial.println("\n\n");
    Serial.println("========================================");
    Serial.println("  SM-GE3222M V2.0 Smart Energy Monitor");
    Serial.println("  " + String(FW_VERSION_STRING));
    Serial.println("  Build: " + String(FW_BUILD_DATE) + " " + String(FW_BUILD_TIME));
    Serial.println("========================================");
    Serial.println();
    
    // Initialize Logger first
    Logger::getInstance().init(LogLevel::INFO);
    LOG_INFO("Main", "Starting firmware initialization...");
    
    // Initialize SystemManager (coordinates all subsystems)
    SystemManager& sysMgr = SystemManager::getInstance();
    
    if (!sysMgr.init()) {
        LOG_FATAL("Main", "System initialization failed!");
        while (1) {
            delay(1000);
        }
    }
    
    LOG_INFO("Main", "System initialization complete");
    
    // Register all tasks with TaskScheduler
    TaskScheduler& scheduler = TaskScheduler::getInstance();
    
    // Critical priority tasks
    scheduler.addTask("EnergyRead", taskEnergyRead, 100, TaskPriority::CRITICAL);
    scheduler.addTask("ModbusRTU", taskModbusRTUPoll, 50, TaskPriority::CRITICAL);
    scheduler.addTask("ModbusTCP", taskModbusTCPPoll, 50, TaskPriority::CRITICAL);
    
    // High priority tasks
    scheduler.addTask("WebSocketPush", taskWebSocketPush, 1000, TaskPriority::HIGH);
    scheduler.addTask("GPIOUpdate", taskGPIOUpdate, 50, TaskPriority::HIGH);
    
    // Medium priority tasks
    scheduler.addTask("LCDUpdate", taskLCDUpdate, 500, TaskPriority::MEDIUM);
    scheduler.addTask("LEDUpdate", taskLEDUpdate, 100, TaskPriority::MEDIUM);
    scheduler.addTask("MQTTUpdate", taskMQTTUpdate, 5000, TaskPriority::MEDIUM);
    scheduler.addTask("PowerQuality", taskPowerQualityAnalysis, 1000, TaskPriority::MEDIUM);
    
    // Low priority tasks
    scheduler.addTask("WiFiCheck", taskWiFiCheck, 5000, TaskPriority::LOW);
    scheduler.addTask("EthernetCheck", taskEthernetCheck, 5000, TaskPriority::LOW);
    scheduler.addTask("HealthCheck", taskHealthCheck, 10000, TaskPriority::LOW);
    scheduler.addTask("OTAHandle", taskOTAHandle, 100, TaskPriority::LOW);
    
    // Idle priority tasks
    scheduler.addTask("DataLog", taskDataLog, 30000, TaskPriority::IDLE);
    scheduler.addTask("EnergyPersist", taskEnergyPersist, 60000, TaskPriority::IDLE);
    
    LOG_INFO("Main", "All tasks registered with scheduler");
    
    // Transition to RUNNING state
    sysMgr.setState(SystemState::RUNNING);
    
    LOG_INFO("Main", "System is now RUNNING");
    Serial.println("\n*** SM-GE3222M V2.0 is READY ***\n");
}

// ============================================================
// Arduino Loop Function
// ============================================================

void loop() {
    // Delegate to SystemManager
    SystemManager::getInstance().run();
}
