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
 * Phase 4: Network Layer Initialization - WiFi STA (fallback to AP)
 * Phase 5: Communications - Modbus RTU + TCP Data Server
 * Phase 6: Task Launch - Create all FreeRTOS tasks
 */

#include <SPI.h>
#include <Wire.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include <DHT.h>

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
#include "SMNetworkManager.h"

// Communication modules
// Web interface (HTTP + SPIFFS dashboard)
#include "WebUIManager.h"
#include "ModbusServer.h"
#include "TCPDataServer.h"

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



/**
 * Print optional boot step (does not mark boot as failed)
 */
void printBootOptionalStep(const char* step, bool running) {
    Serial.printf("  [%s] %s\n", running ? "OK" : "SKIP", step);
    if (!running) {
        Serial.println("  NOTE: Optional component not running (fallback servicing enabled)");
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
        Serial.println("\n*** BOOT WARNING ***");
        Serial.printf("Non-fatal boot issue at: %s\n", step);
        // Do NOT halt; keep system alive for diagnostics and allow fallback servicing in loop().
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
    // Initialize NVS (required for Preferences / ConfigManager).
    // On first boot or after partition changes, NVS can be in an uninitialized state.
    bool nvsOk = true;
    esp_err_t nvsErr = nvs_flash_init();
    if (nvsErr == ESP_ERR_NVS_NO_FREE_PAGES || nvsErr == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvsErr = nvs_flash_init();
    }
    nvsOk = (nvsErr == ESP_OK);
    checkBootStep("NVS Init", nvsOk);
    // Do not hard-fail boot here; continue so web/VB diagnostics are still reachable.

    
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
    
    // Load required configurations
    ModbusConfig modbusConfig;
    SystemConfig systemConfig;

    ConfigManager::getInstance().loadModbusConfig(modbusConfig);
    ConfigManager::getInstance().loadSystemConfig(systemConfig);

    checkBootStep("Modbus Config Loaded", true);
    checkBootStep("System Config Loaded", true);
// Initialize Data Logger
    // Initialize Data Logger with a safe default (avoid heap exhaustion when PSRAM is not present)
    size_t logEntries = psramFound() ? 400 : 200;
    bool dlOk = DataLogger::getInstance().init((uint16_t)logEntries);
    { String _step = String("Data Logger Init (") + String(logEntries) + String(" entries)"); checkBootStep(_step.c_str(), dlOk); }

if (!dlOk && logEntries > 200) {
    Logger::getInstance().warn("DataLogger: Allocation failed for %d entries, retrying with 200", logEntries);
    logEntries = 200;
    dlOk = DataLogger::getInstance().init((uint16_t)logEntries);
    { String _step2 = String("Data Logger Init Retry (") + String(logEntries) + String(" entries)"); checkBootStep(_step2.c_str(), dlOk); }
}


    
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
        Serial.printf("  Initial Read (3-Phase):\n");
Serial.printf("    A: V=%.2fV I=%.3fA P=%.2fW Q=%.2fVAR S=%.2fVA PF=%.3f\n",
              data.phaseA.voltageRMS, data.phaseA.currentRMS,
              data.phaseA.activePower, data.phaseA.reactivePower,
              data.phaseA.apparentPower, data.phaseA.powerFactor);
Serial.printf("    B: V=%.2fV I=%.3fA P=%.2fW Q=%.2fVAR S=%.2fVA PF=%.3f\n",
              data.phaseB.voltageRMS, data.phaseB.currentRMS,
              data.phaseB.activePower, data.phaseB.reactivePower,
              data.phaseB.apparentPower, data.phaseB.powerFactor);
Serial.printf("    C: V=%.2fV I=%.3fA P=%.2fW Q=%.2fVAR S=%.2fVA PF=%.3f\n",
              data.phaseC.voltageRMS, data.phaseC.currentRMS,
              data.phaseC.activePower, data.phaseC.reactivePower,
              data.phaseC.apparentPower, data.phaseC.powerFactor);
Serial.printf("    Totals: P=%.2fW Q=%.2fVAR S=%.2fVA PF=%.3f F=%.2fHz\n",
              data.totalActivePower, data.totalReactivePower,
              data.totalApparentPower, data.totalPowerFactor,
              data.frequency);
} else {
        Serial.println("  WARNING: Initial meter read failed");
    }
    
    return true;
}

// ============================================================================
// PHASE 4: NETWORK LAYER INITIALIZATION (WiFi STA -> AP fallback)
// ============================================================================
bool initPhase4_Network() {
    printBootPhase("4", "Network Layer Initialization");

    // Start WiFi using SMNetworkManager (STA first, fallback to AP)
    networkManager.begin();

    bool connected = networkManager.isConnected();
    bool apMode = networkManager.isAPMode();
    bool staMode = networkManager.isSTAMode();

    if (staMode && connected) {
        checkBootStep("WiFi STA Connected", true);
        Serial.printf("  STA IP: %s\n", networkManager.getIPAddress().c_str());
    } else if (apMode && connected) {
        checkBootStep("WiFi AP Started (Setup)", true);
        Serial.printf("  AP SSID: %s\n", networkManager.getSSID().c_str());
        Serial.printf("  AP IP: %s\n", networkManager.getIPAddress().c_str());
    } else {
        printBootOptionalStep("WiFi Network (disabled / failed)", false);
    }

    // Publish status to SystemMonitor
    SystemMonitor::getInstance().setNetworkStatus(connected, apMode, staMode, false, false);

    return true;
}

// ============================================================================
// PHASE 5: COMMUNICATIONS INITIALIZATION (Modbus RTU + TCP JSON)
// ============================================================================
bool initPhase5_Communications() {
    printBootPhase("5", "Communication Protocols Initialization");

    // Modbus RTU
    ModbusConfig modbusConfig;
    ConfigManager::getInstance().loadModbusConfig(modbusConfig);

    bool modbusOk = ModbusServer::getInstance().begin(modbusConfig);
    if (!modbusOk) {
        Serial.println("  WARNING: Modbus init failed");
    }
    if (modbusConfig.rtuEnabled) {
        checkBootStep("Modbus RTU (Serial2, baud configured)", modbusOk);
    }
    SystemMonitor::getInstance().setModbusActive(modbusOk);

    // Web UI (HTTP + SPIFFS dashboard + WiFi setup page)
    bool webOk = WebUIManager::getInstance().begin(80);
    if (webOk) {
        checkBootStep("Web UI HTTP Server (port 80)", true);
    } else {
        printBootOptionalStep("Web UI HTTP Server (disabled / failed)", false);
    }

    // TCP JSON server (WiFi only)
    // Default to port 8080 unless config says otherwise
    uint16_t tcpPort = 8080;
    if (modbusConfig.tcpPort > 0) tcpPort = modbusConfig.tcpPort; // reuse field if provided
    bool tcpOk = TCPDataServer::getInstance().begin(tcpPort);
    if (tcpOk) {
        checkBootStep("TCP JSON Server (port configured)", true);
        Serial.printf("  TCP Port: %u\n", (unsigned)tcpPort);
    } else {
        printBootOptionalStep("TCP JSON Server (disabled / failed)", false);
    }

    // Update status with TCP flag
    SystemStatus st = SystemMonitor::getInstance().getSystemStatus();
    SystemMonitor::getInstance().setNetworkStatus(st.wifiConnected, st.wifiAPMode, st.wifiSTAMode, tcpOk, st.mqttConnected);

    return true;
}

// ============================================================================
// PHASE 6: TASK CREATION
// ============================================================================
bool initPhase6_Tasks() {
    printBootPhase("6", "FreeRTOS Task Creation");

    if (!TaskManager::getInstance().createAllTasks()) {
        return checkBootStatus("Task Creation", false);
    }

    checkBootStep("EnergyTask (Core 1, Priority 5, 500ms)", true);
    checkBootStep("AccumulatorTask (Core 1, Priority 4, 1000ms)", true);
    checkBootStep("ModbusTask (Core 1, Priority 3, 10ms poll)", true);
    printBootOptionalStep("TCPServerTask (Core 0, Priority 2, 20ms poll)", TaskManager::getInstance().isTCPServerTaskRunning());
    printBootOptionalStep("MQTTTask (stub)", TaskManager::getInstance().getMQTTTaskHandle() != nullptr);
    printBootOptionalStep("DiagnosticsTask (Core 0, Priority 1, 5000ms)", TaskManager::getInstance().getDiagnosticsTaskHandle() != nullptr);

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
        g_lastError = ErrorCode::MODBUS_INIT_ERROR;
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
    // Keep captive portal DNS responsive and monitor WiFi.
    networkManager.loop();
    WebUIManager::getInstance().loop();
    // FreeRTOS tasks handle all runtime processing.
    // Keep the Arduino loop idle to minimize jitter and watchdog interactions.
    delay(50);
}
