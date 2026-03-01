/**
 * @file ConfigManager.cpp
 * @brief NVS-based configuration manager implementation
 */

#include "ConfigManager.h"
#include "Logger.h"
#include <nvs_flash.h>

// Preferences.begin() can fail on devices with corrupted NVS or unexpected partition state.
// This helper attempts a single NVS repair (erase+init) and retries once.
static bool beginPrefsSafe(Preferences& prefs, const char* ns, bool readOnly) {
    if (prefs.begin(ns, readOnly)) {
        return true;
    }

    Logger::getInstance().error("[NVS] Failed to open namespace '%s' (attempting NVS repair)", ns);

    // Try NVS repair: erase and re-init
    esp_err_t err = nvs_flash_erase();
    if (err != ESP_OK) {
        Logger::getInstance().error("[NVS] nvs_flash_erase failed: %d", (int)err);
        return false;
    }
    err = nvs_flash_init();
    if (err != ESP_OK) {
        Logger::getInstance().error("[NVS] nvs_flash_init after erase failed: %d", (int)err);
        return false;
    }

    if (!prefs.begin(ns, readOnly)) {
        Logger::getInstance().error("[NVS] Failed to open namespace '%s' after repair", ns);
        return false;
    }

    Logger::getInstance().warn("[NVS] Namespace '%s' opened after repair (defaults will be used)", ns);
    return true;
}
#include <nvs_flash.h>

// NVS Namespace constants
const char* ConfigManager::NVS_NAMESPACE_WIFI    = "wifi";
const char* ConfigManager::NVS_NAMESPACE_MODBUS  = "modbus";
const char* ConfigManager::NVS_NAMESPACE_MQTT    = "mqtt";
const char* ConfigManager::NVS_NAMESPACE_NETWORK = "network";
const char* ConfigManager::NVS_NAMESPACE_SYSTEM  = "system";

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() : _initialized(false) {
}

ConfigManager::~ConfigManager() {
}

bool ConfigManager::init() {
    if (_initialized) {
        return true;
    }
    
    Logger& logger = Logger::getInstance();
    
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        logger.warn("NVS partition was truncated, erasing...");
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    
    if (err != ESP_OK) {
        logger.error("Failed to initialize NVS: %d", err);
        return false;
    }
    
    _initialized = true;
    logger.info("ConfigManager initialized");
    return true;
}

// ============================================================================
// WiFi Configuration
// ============================================================================

// ============================================================================
// Modbus Configuration
// ============================================================================

bool ConfigManager::loadModbusConfig(ModbusConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_MODBUS, false)) {
        Logger::getInstance().error("Failed to open Modbus config namespace");
        return false;
    }
    
    config.rtuEnabled = prefs.getBool("rtuEnabled", true);
    config.tcpEnabled = prefs.getBool("tcpEnabled", true);
    config.slaveID = prefs.getUChar("slaveID", 1);
    config.baudrate = prefs.getUInt("baudrate", 9600);
    config.dataBits = prefs.getUChar("dataBits", 8);
    config.stopBits = prefs.getUChar("stopBits", 1);
    config.parity = prefs.getChar("parity", 'N');
    config.tcpPort = prefs.getUShort("tcpPort", 502);
    
    prefs.end();
    return true;
}

bool ConfigManager::saveModbusConfig(const ModbusConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_MODBUS, false)) {
        Logger::getInstance().error("Failed to open Modbus config namespace for writing");
        return false;
    }
    
    prefs.putBool("rtuEnabled", config.rtuEnabled);
    prefs.putBool("tcpEnabled", config.tcpEnabled);
    prefs.putUChar("slaveID", config.slaveID);
    prefs.putUInt("baudrate", config.baudrate);
    prefs.putUChar("dataBits", config.dataBits);
    prefs.putUChar("stopBits", config.stopBits);
    prefs.putChar("parity", config.parity);
    prefs.putUShort("tcpPort", config.tcpPort);
    
    prefs.end();
    Logger::getInstance().info("Modbus config saved");
    return true;
}

bool ConfigManager::hasModbusConfig() {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_MODBUS, false)) {
        return false;
    }
    bool exists = prefs.isKey("slaveID");
    prefs.end();
    return exists;
}

// ============================================================================
// MQTT Configuration
// ============================================================================

bool ConfigManager::loadMQTTConfig(MQTTConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_MQTT, false)) {
        Logger::getInstance().error("Failed to open MQTT config namespace");
        return false;
    }
    
    config.enabled = prefs.getBool("enabled", false);
    config.port = prefs.getUShort("port", 1883);
    config.publishInterval = prefs.getUShort("publishInterval", 10);
    config.useHomeAssistant = prefs.getBool("useHA", true);
    config.useTLS = prefs.getBool("useTLS", false);
    
    prefs.getString("broker", config.broker, sizeof(config.broker));
    prefs.getString("username", config.username, sizeof(config.username));
    prefs.getString("password", config.password, sizeof(config.password));
    prefs.getString("clientID", config.clientID, sizeof(config.clientID));
    prefs.getString("baseTopic", config.baseTopic, sizeof(config.baseTopic));
    
    prefs.end();
    return true;
}

bool ConfigManager::saveMQTTConfig(const MQTTConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_MQTT, false)) {
        Logger::getInstance().error("Failed to open MQTT config namespace for writing");
        return false;
    }
    
    prefs.putBool("enabled", config.enabled);
    prefs.putUShort("port", config.port);
    prefs.putUShort("publishInterval", config.publishInterval);
    prefs.putBool("useHA", config.useHomeAssistant);
    prefs.putBool("useTLS", config.useTLS);
    
    prefs.putString("broker", config.broker);
    prefs.putString("username", config.username);
    prefs.putString("password", config.password);
    prefs.putString("clientID", config.clientID);
    prefs.putString("baseTopic", config.baseTopic);
    
    prefs.end();
    Logger::getInstance().info("MQTT config saved");
    return true;
}

bool ConfigManager::hasMQTTConfig() {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_MQTT, false)) {
        return false;
    }
    bool exists = prefs.isKey("broker");
    prefs.end();
    return exists;
}

// ============================================================================
// Network Configuration
// ============================================================================

bool ConfigManager::loadNetworkConfig(NetworkConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_NETWORK, false)) {
        Logger::getInstance().error("Failed to open Network config namespace");
        return false;
    }
    
    config.mdnsEnabled = prefs.getBool("mdnsEnabled", true);
    config.ntpEnabled = prefs.getBool("ntpEnabled", true);
    config.timezoneOffset = prefs.getShort("tzOffset", 0);
    
    prefs.getString("hostname", config.hostname, sizeof(config.hostname));
    prefs.getString("mdnsName", config.mdnsName, sizeof(config.mdnsName));
    prefs.getString("ntpServer", config.ntpServer, sizeof(config.ntpServer));
    
    prefs.end();
    return true;
}

bool ConfigManager::saveNetworkConfig(const NetworkConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_NETWORK, false)) {
        Logger::getInstance().error("Failed to open Network config namespace for writing");
        return false;
    }
    
    prefs.putBool("mdnsEnabled", config.mdnsEnabled);
    prefs.putBool("ntpEnabled", config.ntpEnabled);
    prefs.putShort("tzOffset", config.timezoneOffset);
    
    prefs.putString("hostname", config.hostname);
    prefs.putString("mdnsName", config.mdnsName);
    prefs.putString("ntpServer", config.ntpServer);
    
    prefs.end();
    Logger::getInstance().info("Network config saved");
    return true;
}

bool ConfigManager::hasNetworkConfig() {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_NETWORK, false)) {
        return false;
    }
    bool exists = prefs.isKey("hostname");
    prefs.end();
    return exists;
}

// ============================================================================
// System Configuration
// ============================================================================

bool ConfigManager::loadSystemConfig(SystemConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_SYSTEM, false)) {
        Logger::getInstance().error("Failed to open System config namespace");
        return false;
    }
    
    config.readInterval = prefs.getUShort("readInterval", 500);
    config.publishInterval = prefs.getUShort("publishInterval", 1);
    config.webServerEnabled = prefs.getBool("webEnabled", true);
    config.webServerPort = prefs.getUShort("webPort", 80);
    config.otaEnabled = prefs.getBool("otaEnabled", true);
    config.logLevel = static_cast<LogLevel>(prefs.getUChar("logLevel", static_cast<uint8_t>(LogLevel::INFO)));
    config.watchdogEnabled = prefs.getBool("wdEnabled", true);
    config.watchdogTimeout = prefs.getUShort("wdTimeout", 30);
    config.dhtEnabled = prefs.getBool("dhtEnabled", true);
    config.dhtReadIntervalMs = prefs.getUShort("dhtIntMs", 3000);
    config.dhtDebugLogging = prefs.getBool("dhtDbg", false);
    if (config.dhtReadIntervalMs < 2000) config.dhtReadIntervalMs = 2000;
    
    prefs.getString("otaPassword", config.otaPassword, sizeof(config.otaPassword));
    
    prefs.end();
    return true;
}

bool ConfigManager::saveSystemConfig(const SystemConfig& config) {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_SYSTEM, false)) {
        Logger::getInstance().error("Failed to open System config namespace for writing");
        return false;
    }
    
    prefs.putUShort("readInterval", config.readInterval);
    prefs.putUShort("publishInterval", config.publishInterval);
    prefs.putBool("webEnabled", config.webServerEnabled);
    prefs.putUShort("webPort", config.webServerPort);
    prefs.putBool("otaEnabled", config.otaEnabled);
    prefs.putUChar("logLevel", static_cast<uint8_t>(config.logLevel));
    prefs.putBool("wdEnabled", config.watchdogEnabled);
    prefs.putUShort("wdTimeout", config.watchdogTimeout);
    prefs.putBool("dhtEnabled", config.dhtEnabled);
    prefs.putUShort("dhtIntMs", (config.dhtReadIntervalMs < 2000 ? 2000 : config.dhtReadIntervalMs));
    prefs.putBool("dhtDbg", config.dhtDebugLogging);
    
    prefs.putString("otaPassword", config.otaPassword);
    
    prefs.end();
    Logger::getInstance().info("System config saved");
    return true;
}

bool ConfigManager::hasSystemConfig() {
    Preferences prefs;
    if (!beginPrefsSafe(prefs, NVS_NAMESPACE_SYSTEM, false)) {
        return false;
    }
    bool exists = prefs.isKey("readInterval");
    prefs.end();
    return exists;
}

// ============================================================================
// Factory Reset
// ============================================================================

bool ConfigManager::factoryReset() {
    Logger& logger = Logger::getInstance();
    logger.warn("Performing factory reset...");
    
    Preferences prefs;
    bool success = true;
    
    // Clear all namespaces
    if (beginPrefsSafe(prefs, NVS_NAMESPACE_WIFI, false)) {
        prefs.clear();
        prefs.end();
    } else {
        success = false;
    }
    
    if (beginPrefsSafe(prefs, NVS_NAMESPACE_MODBUS, false)) {
        prefs.clear();
        prefs.end();
    } else {
        success = false;
    }
    
    if (beginPrefsSafe(prefs, NVS_NAMESPACE_MQTT, false)) {
        prefs.clear();
        prefs.end();
    } else {
        success = false;
    }
    
    if (beginPrefsSafe(prefs, NVS_NAMESPACE_NETWORK, false)) {
        prefs.clear();
        prefs.end();
    } else {
        success = false;
    }
    
    if (beginPrefsSafe(prefs, NVS_NAMESPACE_SYSTEM, false)) {
        prefs.clear();
        prefs.end();
    } else {
        success = false;
    }
    
    if (success) {
        logger.info("Factory reset completed");
    } else {
        logger.error("Factory reset failed");
    }
    
    return success;
}
