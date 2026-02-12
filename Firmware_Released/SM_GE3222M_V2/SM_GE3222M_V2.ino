/**
 * SM-GE3222M V2.0 - Main Entry Point
 * 
 * Complete 6-phase boot sequence for the modernized Smart Energy Monitor firmware.
 * This replaces the V1.0 monolithic Arduino sketch with a layered, FreeRTOS-based architecture.
 * 
 * Boot Sequence:
 * Phase 1: HAL Initialization - Logger, GPIO, SPI, I2C, System Monitor, Watchdog
 * Phase 2: Storage - SPIFFS, NVS, Configuration Loading
 * Phase 3: Energy Metering - ATM90E36 Init, Calibration, Accumulator Restore
 * Phase 4: Network - WiFi STA/AP, mDNS, NTP
 * Phase 5: Communications - TCP, WebServer, Modbus, MQTT, OTA
 * Phase 6: Task Launch - Create all FreeRTOS tasks
 */

#include <SPI.h>
#include <Wire.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <DHT.h>
#include <ESPmDNS.h>

// Some ESP32 board definitions (e.g. some Visual Micro setups) may not define LED_BUILTIN.
// GPIO2 is the usual on-board LED for ESP32 Dev Module.
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Include files (flat structure for Arduino IDE)
#include "PinMap.h"
#include "Version.h"
#include "RegisterMap.h"
#include "ModbusMap.h"
#include "DataTypes.h"

// HAL modules
#include "SPIBus.h"
#include "I2CBus.h"
#include "GPIOManager.h"

// Energy modules
#include "ATM90E36Driver.h"
#include "EnergyMeter.h"
#include "EnergyAccumulator.h"
#include "CalibrationManager.h"

// Storage modules
#include "ConfigManager.h"
#include "SPIFFSManager.h"
#include "DataLogger.h"

// Network modules
#include "NetworkManager.h"
#include "OTAManager.h"
#include "NTPSync.h"

// Communication modules
#include "TCPDataServer.h"
#include "ProtocolV2.h"
#include "WebServerManager.h"
#include "ModbusServer.h"
#include "MQTTPublisher.h"

// Core modules
#include "TaskManager.h"
#include "EventBus.h"

// Diagnostics modules
#include "Logger.h"
#include "SystemMonitor.h"
#include "WatchdogManager.h"

// ============================================================================
// GLOBAL STATE
// ============================================================================
static bool g_bootComplete = false;
static ErrorCode g_lastError = ErrorCode::NONE;
static uint32_t g_bootStartTime = 0;

// ============================================================================
// BOOT PHASE HELPERS
// ============================================================================

/**
 * Print boot phase message
 */
void printBootPhase(const char* phase, const char* description) {
    Serial.println();
    Serial.println("============================================================");
    Serial.printf("PHASE %s: %s\n", phase, description);
    Serial.println("============================================================");
}

/**
 * Print boot step
 */
void printBootStep(const char* step, bool success = true) {
    Serial.printf("  [%s] %s\n", success ? "OK" : "FAIL", step);
    if (!success) {
        Serial.println("  ERROR: Boot sequence failed!");
    }
}

// Backward-compatible helper used throughout this sketch.
inline void checkBootStep(const char* step, bool success = true) {
    printBootStep(step, success);
}

/**
 * Check boot status and halt if failed
 */
bool checkBootStatus(const char* step, bool success) {
    printBootStep(step, success);
    if (!success) {
        Serial.println("\n*** SYSTEM HALT ***");
        Serial.printf("Failed at: %s\n", step);
        while(1) {
            delay(1000);
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Blink error
        }
    }
    return success;
}

// ============================================================================
// PHASE 1: HAL INITIALIZATION
// ============================================================================
bool initPhase1_HAL() {
    printBootPhase("1", "Hardware Abstraction Layer Initialization");
    
    // Initialize Serial for debugging
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {
        delay(10);
    }
    Serial.println();
    Serial.println(getFullVersionString());
    Serial.printf("Build: %s %s\n", FW_BUILD_DATE, FW_BUILD_TIME);
    Serial.println();
    
    // Initialize Logger
    if (!Logger::getInstance().init(LogLevel::INFO, true, false)) {
        return checkBootStatus("Logger Init", false);
    }
    checkBootStep("Logger Init", true);
    
    // Initialize built-in LED for status
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    // Initialize GPIOManager
    if (!GPIOManager::getInstance().init()) {
        return checkBootStatus("GPIO Manager Init", false);
    }
    GPIOManager::getInstance().setLED(LED::RUN, true);
    checkBootStep("GPIO Manager Init", true);
    
    // Initialize SPI Bus
    if (!SPIBus::getInstance().init()) {
        return checkBootStatus("SPI Bus Init", false);
    }
    checkBootStep("SPI Bus Init", true);
    
    // Initialize I2C Bus (idempotent)
    if (!I2CBus::getInstance().isInitialized()) {
        if (!I2CBus::getInstance().init()) {
            return checkBootStatus("I2C Bus Init", false);
        }
    }
    checkBootStep("I2C Bus Init", true);
    
    // Initialize System Monitor
    SystemMonitor::getInstance().init();
    checkBootStep("System Monitor Init", true);
    
    // Initialize Watchdog Manager (non-fatal if already initialized by core)
    bool wdtOk = WatchdogManager::getInstance().init(30);
    checkBootStep("Watchdog Init", wdtOk);
    // Do NOT halt the system here: on some Arduino/ESP-IDF builds, TWDT is initialized before sketch startup.
    
    return true;
}

// ============================================================================
// PHASE 2: STORAGE INITIALIZATION
// ============================================================================
bool initPhase2_Storage() {
    printBootPhase("2", "Storage & Configuration Loading");
    
    // Initialize SPIFFS
    if (!SPIFFSManager::getInstance().init(true)) {
        return checkBootStatus("SPIFFS Mount", false);
    }
    checkBootStep("SPIFFS Mount", true);
    
    // Print SPIFFS info
    SPIFFSInfo info = SPIFFSManager::getInstance().getInfo();
    Serial.printf("  SPIFFS: %u KB total, %u KB used, %u KB free\n", 
                  info.totalBytes / 1024, info.usedBytes / 1024, info.freeBytes / 1024);
    
    // Initialize NVS and ConfigManager
    if (!ConfigManager::getInstance().init()) {
        return checkBootStatus("Config Manager Init", false);
    }
    checkBootStep("Config Manager Init", true);
    
    // Load all configurations
    WiFiConfig wifiConfig;
    ModbusConfig modbusConfig;
    MQTTConfig mqttConfig;
    NetworkConfig networkConfig;
    SystemConfig systemConfig;
    
    ConfigManager::getInstance().loadWiFiConfig(wifiConfig);
    ConfigManager::getInstance().loadModbusConfig(modbusConfig);
    ConfigManager::getInstance().loadMQTTConfig(mqttConfig);
    ConfigManager::getInstance().loadNetworkConfig(networkConfig);
    ConfigManager::getInstance().loadSystemConfig(systemConfig);
    
    checkBootStep("WiFi Config Loaded", true);
    checkBootStep("Modbus Config Loaded", true);
    checkBootStep("MQTT Config Loaded", true);
    checkBootStep("Network Config Loaded", true);
    checkBootStep("System Config Loaded", true);
    
    // Initialize Data Logger
    // Initialize Data Logger with a safe default (avoid heap exhaustion when PSRAM is not present)
    size_t logEntries = psramFound() ? 1000 : 200;
    bool dlOk = DataLogger::getInstance().init((uint16_t)logEntries);
    { String _step = String("Data Logger Init (") + String(logEntries) + String(" entries)"); checkBootStep(_step.c_str(), dlOk); }

    
    return true;
}

// ============================================================================
// PHASE 3: ENERGY METERING INITIALIZATION
// ============================================================================
bool initPhase3_EnergyMetering() {
    printBootPhase("3", "Energy Metering System Initialization");
    
    // Initialize ATM90E36 Driver
    CalibrationConfig calConfig;
    if (!CalibrationManager::getInstance().loadCalibration(calConfig)) {
        Serial.println("  WARNING: Using default calibration");
        calConfig = CalibrationManager::getInstance().getDefaultCalibration();
    }
    if (!ATM90E36Driver::getInstance().init(calConfig)) {
        return checkBootStatus("ATM90E36 Init", false);
    }
    checkBootStep("ATM90E36 Init", true);
    checkBootStep("Calibration Applied", true);
    
    // Verify checksums
    if (!ATM90E36Driver::getInstance().verifyChecksums()) {
        Serial.println("  WARNING: Calibration checksum verification failed");
    } else {
        checkBootStep("Checksum Verification", true);
    }
    
    // Initialize Energy Meter (with 5-sample moving average filtering)
    if (!EnergyMeter::getInstance().init(5)) {
        return checkBootStatus("Energy Meter Init", false);
    }
    checkBootStep("Energy Meter Init", true);
    
    // Initialize Energy Accumulator and restore state
    if (!EnergyAccumulator::getInstance().init(60)) { // Persist every 60 seconds
        return checkBootStatus("Energy Accumulator Init", false);
    }
    EnergyAccumulator::getInstance().loadFromNVS();
    checkBootStep("Energy Accumulator Init", true);
    checkBootStep("Energy State Restored", true);
    
    // Perform initial read
    if (EnergyMeter::getInstance().update()) {
        MeterData data = EnergyMeter::getInstance().getSnapshot();
        Serial.printf("  Initial Read: VA=%.1fV IA=%.2fA PA=%.1fW F=%.2fHz\n",
                      data.phaseA.voltageRMS, data.phaseA.currentRMS,
                      data.phaseA.activePower, data.frequency);
    } else {
        Serial.println("  WARNING: Initial meter read failed");
    }
    
    return true;
}

// ============================================================================
// PHASE 4: NETWORK INITIALIZATION
// ============================================================================
bool initPhase4_Network() {
    printBootPhase("4", "Network Layer Initialization");
    
    // Initialize Network Manager
    WiFiConfig wifiConfig;
    ConfigManager::getInstance().loadWiFiConfig(wifiConfig);
    if (!SMNetworkManager::getInstance().init(wifiConfig)) {
        Serial.println("  WARNING: WiFi init failed, continuing...");
    }
    checkBootStep("Network Manager Init", true);
    
    // Connect to WiFi (non-blocking)
    if (wifiConfig.apMode) {
        SMNetworkManager::getInstance().startAP(wifiConfig.apSSID, wifiConfig.apPassword);
        Serial.printf("  AP Mode: %s (IP will be assigned)\n", wifiConfig.apSSID);
    } else {
        SMNetworkManager::getInstance().startSTA();
        Serial.printf("  STA Mode: Connecting to %s...\n", wifiConfig.ssid);
    }
    checkBootStep("WiFi Started", true);
    
    // Initialize mDNS
    NetworkConfig netConfig;
    ConfigManager::getInstance().loadNetworkConfig(netConfig);
    if (netConfig.mdnsEnabled) {
        // Use WiFi hostname as the mDNS hostname.
        if (MDNS.begin(wifiConfig.hostname)) {
            MDNS.addService("http", "tcp", 80);
            MDNS.addService("modbus", "tcp", 502);
            checkBootStep("mDNS Started", true);
        }
    }
    
    // Initialize NTP
    if (netConfig.ntpEnabled) {
        NTPSync::getInstance().init(netConfig.ntpServer, netConfig.timezoneOffset);
        checkBootStep("NTP Sync Started", true);
    }
    
    return true;
}

// ============================================================================
// PHASE 5: COMMUNICATIONS INITIALIZATION
// ============================================================================
bool initPhase5_Communications() {
    printBootPhase("5", "Communication Protocols Initialization");
    
    // Initialize TCP Data Server (V1 compatible)
    bool tcpOk = TCPDataServer::getInstance().begin(8088);
    if (!tcpOk) {
        Serial.println("  WARNING: TCP Server init failed");
    }
    checkBootStep("TCP Data Server (port 8088)", tcpOk);

    
    // Initialize Web Server (socket-based WebServer; WebSocket streaming disabled)
    bool webOk = WebServerManager::getInstance().begin(80);
    if (!webOk) {
        Serial.println("  WARNING: Web Server init failed");
    }
    checkBootStep("Web Server (port 80)", webOk);
    checkBootStep("REST API (/api/*)", webOk);
    checkBootStep("WebSocket (/ws) (disabled)", false);

    
    // Initialize Modbus Server
    ModbusConfig modbusConfig;
    ConfigManager::getInstance().loadModbusConfig(modbusConfig);
    if (!ModbusServer::getInstance().begin(modbusConfig)) {
        Serial.println("  WARNING: Modbus init failed");
    }
    if (modbusConfig.rtuEnabled) {
        checkBootStep("Modbus RTU (Serial2, baud configured)", true);
    }
    if (modbusConfig.tcpEnabled) {
        checkBootStep("Modbus TCP (port 502)", true);
    }
    
    // Initialize MQTT Publisher
    MQTTConfig mqttConfig;
    ConfigManager::getInstance().loadMQTTConfig(mqttConfig);
    if (mqttConfig.enabled) {
        if (!MQTTPublisher::getInstance().begin(mqttConfig)) {
            Serial.println("  WARNING: MQTT init failed");
        }
        checkBootStep("MQTT Publisher", true);
    } else {
        checkBootStep("MQTT Publisher (disabled)", true);
    }
    
    // Initialize OTA Manager
    SystemConfig sysConfig;
    ConfigManager::getInstance().loadSystemConfig(sysConfig);
    if (sysConfig.otaEnabled) {
        // Hostname comes from WiFiConfig; password from SystemConfig.
        WiFiConfig wifiConfig;
        ConfigManager::getInstance().loadWiFiConfig(wifiConfig);
        if (!OTAManager::getInstance().begin(wifiConfig.hostname, sysConfig.otaPassword)) {
            Serial.println("  WARNING: OTA init failed");
        }
        checkBootStep("OTA Manager", true);
    } else {
        checkBootStep("OTA Manager (disabled)", true);
    }
    
    return true;
}

// ============================================================================
// PHASE 6: TASK CREATION
// ============================================================================
bool initPhase6_Tasks() {
    printBootPhase("6", "FreeRTOS Task Creation");
    
    // Create all tasks via TaskManager
    if (!TaskManager::getInstance().createAllTasks()) {
        return checkBootStatus("Task Creation", false);
    }
    
    // List of tasks created:
    checkBootStep("EnergyTask (Core 1, Priority 5, 500ms)", true);
    checkBootStep("AccumulatorTask (Core 1, Priority 4, 1000ms)", true);
    checkBootStep("ModbusTask (Core 1, Priority 3, 10ms poll)", true);
    checkBootStep("TCPServerTask (Core 0, Priority 3, event-driven)", true);
    checkBootStep("WebServerTask (Core 0, Priority 2, 10ms)", true);
    checkBootStep("MQTTTask (Core 0, Priority 2, configurable)", true);
    checkBootStep("DiagnosticsTask (Core 0, Priority 1, 5000ms)", true);
    
    return true;
}

// ============================================================================
// SETUP - Called once at boot
// ============================================================================
void setup() {
    g_bootStartTime = millis();
    
    // Execute 6-phase boot sequence
    if (!initPhase1_HAL()) {
        g_lastError = ErrorCode::SYSTEM_PANIC;
        return;
    }
    
    if (!initPhase2_Storage()) {
        g_lastError = ErrorCode::NVS_INIT_FAILED;
        return;
    }
    
    if (!initPhase3_EnergyMetering()) {
        g_lastError = ErrorCode::ATM_INIT_FAILED;
        return;
    }
    
    if (!initPhase4_Network()) {
        g_lastError = ErrorCode::NETWORK_INIT_FAILED;
        return;
    }
    
    if (!initPhase5_Communications()) {
        g_lastError = ErrorCode::TCP_SERVER_ERROR;
        return;
    }
    
    if (!initPhase6_Tasks()) {
        g_lastError = ErrorCode::TASK_CREATE_FAILED;
        return;
    }
    
    // Boot complete
    uint32_t bootTime = millis() - g_bootStartTime;
    Serial.println();
    Serial.println("============================================================");
    Serial.printf("BOOT COMPLETE - Took %u ms\n", bootTime);
    Serial.println("============================================================");
    Serial.println();
    Serial.println("System Status:");
    Serial.printf("  Firmware: %s\n", getShortVersionString());
    Serial.printf("  Free Heap: %u KB\n", ESP.getFreeHeap() / 1024);
    Serial.printf("  CPU Freq: %u MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("  Flash Size: %u MB\n", ESP.getFlashChipSize() / (1024 * 1024));
    Serial.println();
    
    // Set system LED to running state
    GPIOManager::getInstance().setLED(LED::RUN, true);
    GPIOManager::getInstance().setLED(LED::FAULT, false);
    digitalWrite(LED_BUILTIN, LOW);
    
    g_bootComplete = true;
}

// ============================================================================
// LOOP - Minimal main loop (tasks handle most work)
// ============================================================================
void loop() {
    // In FreeRTOS-based design, most work is done in tasks
    // Main loop only handles OTA and minimal housekeeping
    
    // Handle OTA updates
    OTAManager::getInstance().handle();
    
    // Handle network manager (reconnection logic)
    SMNetworkManager::getInstance().handle();
    
    // Feed watchdog
    WatchdogManager::getInstance().feed();
    
    // Small delay to prevent watchdog issues
    delay(10);
}