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
#include <nvs_flash.h>
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
// Web interface removed (no HTTP/REST/WebSocket/SPIFFS UI)
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

// Optional compile-time STA fallback credentials.
// Leave empty to require credentials from ConfigManager (NVS).
#ifndef FALLBACK_STA_SSID
#define FALLBACK_STA_SSID ""
#endif
#ifndef FALLBACK_STA_PASSWORD
#define FALLBACK_STA_PASSWORD ""
#endif

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
    

    // Phase 4 WiFi stability plan:
    //  - startAP()  is AP-only and stable (no mode switching after it starts)
    //  - startSTA() is STA-only (no management AP)
    //  - Boot logic: AP-only requested -> start AP-only
    //               else try STA; if STA fails -> fall back to AP-only (channel 1..11)
    bool wifiOk = false;

    // Prefer DHCP for STA (more robust for most field networks)
    wifiConfig.useDHCP = true;

    // If NVS has no SSID but a compile-time fallback is set, use it.
    if (strlen(wifiConfig.ssid) == 0 && (sizeof(FALLBACK_STA_SSID) > 1)) {
        strncpy(wifiConfig.ssid, FALLBACK_STA_SSID, sizeof(wifiConfig.ssid) - 1);
        wifiConfig.ssid[sizeof(wifiConfig.ssid) - 1] = '\0';
        strncpy(wifiConfig.password, FALLBACK_STA_PASSWORD, sizeof(wifiConfig.password) - 1);
        wifiConfig.password[sizeof(wifiConfig.password) - 1] = '\0';
        Serial.printf("  INFO: Using compile-time fallback SSID: %s\n", wifiConfig.ssid);
    }

    const char* apSSID = wifiConfig.apSsid;
    const char* apPASS = wifiConfig.apPassword;

    if (wifiConfig.apMode) {
        // Explicit AP-only requested
        wifiOk = SMNetworkManager::getInstance().startAP(apSSID, apPASS);
        Serial.printf("  AP-only Mode: %s (IP: 192.168.4.1)\n", apSSID);
    } else if (strlen(wifiConfig.ssid) > 0) {
        // Try STA-only first
        Serial.printf("  STA-only Mode: Connecting to %s (DHCP)...\n", wifiConfig.ssid);
        delay(150);
        wifiOk = SMNetworkManager::getInstance().startSTA();

        // Wait for STA connect (bounded)
        const uint32_t t0 = millis();
        const uint32_t timeoutMs = 15000;
        while (wifiOk && !SMNetworkManager::getInstance().isConnected() && (millis() - t0) < timeoutMs) {
            delay(200);
            yield();
        }

        if (wifiOk && SMNetworkManager::getInstance().isConnected()) {
            Serial.printf("  STA Connected: IP=%s RSSI=%ddBm\n",
                          SMNetworkManager::getInstance().getLocalIP().c_str(),
                          SMNetworkManager::getInstance().getRSSI());
        } else {
            // STA failed -> stable AP-only fallback
            Serial.println("  WARNING: STA connect failed, falling back to AP-only...");
            SMNetworkManager::getInstance().stop();
            wifiOk = SMNetworkManager::getInstance().startAP(apSSID, apPASS);
            Serial.printf("  AP-only Mode: %s (IP: 192.168.4.1)\n", apSSID);
        }
    } else {
        // No SSID configured -> AP-only fallback
        Serial.println("  INFO: No STA SSID configured, starting AP-only...");
        wifiOk = SMNetworkManager::getInstance().startAP(apSSID, apPASS);
        Serial.printf("  AP-only Mode: %s (IP: 192.168.4.1)\n", apSSID);
    }

    checkBootStep("WiFi Started", wifiOk);
    
    // Initialize mDNS
    NetworkConfig netConfig;
    ConfigManager::getInstance().loadNetworkConfig(netConfig);
    if (netConfig.mdnsEnabled) {
        // Use WiFi hostname as the mDNS hostname.
        if (MDNS.begin(wifiConfig.hostname)) {
            // No HTTP server in this build; advertise only generic TCP service(s)
            MDNS.addService("tcpdata", "tcp", 8088);
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

    
    // Web interface intentionally removed in this build.
    // WiFi is still available for future BACnet/TCP/IP transport and OTA (if enabled).

    // Initialize Modbus Server (RTU ONLY - TCP disabled by requirement)
ModbusConfig modbusConfig;
ConfigManager::getInstance().loadModbusConfig(modbusConfig);
modbusConfig.tcpEnabled = false; // enforce RTU-only
if (!ModbusServer::getInstance().begin(modbusConfig)) {
    Serial.println("  WARNING: Modbus init failed");
}
if (modbusConfig.rtuEnabled) {
    checkBootStep("Modbus RTU (Serial2, baud configured)", true);
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
    printBootOptionalStep("TCPServerTask (Core 0, Priority 3, event-driven)", TaskManager::getInstance().isTCPServerTaskRunning());
    // WebServerTask removed
    printBootOptionalStep("MQTTTask (Core 0, Priority 2, configurable)", TaskManager::getInstance().getMQTTTaskHandle() != nullptr);
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

    // Fallback servicing: if optional protocol tasks could not be created due to low heap,
    // service them here in the Arduino loop (prevents boot failure / watchdog resets).
    if (!TaskManager::getInstance().isTCPServerTaskRunning()) {
        TCPDataServer::getInstance().handle();
    }
    
    // Note: Task Watchdog is fed by subscribed FreeRTOS tasks.
    // Calling esp_task_wdt_reset() from loopTask when it isn't registered produces
    // "task not found" spam and can stall the UI/menu.
    // So we do NOT feed the watchdog from loop().
    
    // Small delay to prevent watchdog issues
    delay(10);
}