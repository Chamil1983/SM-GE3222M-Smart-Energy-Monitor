/**
 * @file ConfigManager.h
 * @brief JSON configuration management with SPIFFS persistence
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Manages device configuration with schema validation and backup support
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../include/GlobalTypes.h"

class ConfigManager {
public:
    // Singleton instance
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    // Initialize and load configuration
    bool init();

    // Load configuration from SPIFFS
    bool load();

    // Save configuration to SPIFFS (with backup)
    bool save();

    // Configuration getters
    WiFiConfig getWiFiConfig() const { return _wifiConfig; }
    MQTTConfig getMQTTConfig() const { return _mqttConfig; }
    ModbusConfig getModbusConfig() const { return _modbusConfig; }
    CalibrationConfig getCalibrationConfig() const { return _calibrationConfig; }

    // Configuration setters
    void setWiFiConfig(const WiFiConfig& config);
    void setMQTTConfig(const MQTTConfig& config);
    void setModbusConfig(const ModbusConfig& config);
    void setCalibrationConfig(const CalibrationConfig& config);

    // JSON import/export
    String getConfigJSON();
    bool setConfigJSON(const String& jsonStr);

    // Validation
    bool validate();

    // Restore factory defaults
    void restoreDefaults();

    // Status
    bool isLoaded() const { return _loaded; }
    bool hasUnsavedChanges() const { return _dirty; }
    void markDirty() { _dirty = true; }

private:
    ConfigManager();
    ~ConfigManager() {}

    // Configuration structures
    WiFiConfig _wifiConfig;
    MQTTConfig _mqttConfig;
    ModbusConfig _modbusConfig;
    CalibrationConfig _calibrationConfig;

    // State
    bool _loaded;
    bool _dirty;

    // File paths
    static constexpr const char* CONFIG_FILE = "/config.json";
    static constexpr const char* BACKUP_FILE = "/config.json.bak";

    // JSON serialization/deserialization
    bool serializeToFile(const char* filename);
    bool deserializeFromFile(const char* filename);

    // Backup management
    bool createBackup();
    bool restoreBackup();

    // Schema validation helpers
    bool validateWiFiConfig(const WiFiConfig& config);
    bool validateMQTTConfig(const MQTTConfig& config);
    bool validateModbusConfig(const ModbusConfig& config);
    bool validateCalibrationConfig(const CalibrationConfig& config);

    // JSON conversion helpers
    void wifiConfigToJson(JsonObject& obj);
    void mqttConfigToJson(JsonObject& obj);
    void modbusConfigToJson(JsonObject& obj);
    void calibrationConfigToJson(JsonObject& obj);

    void jsonToWifiConfig(JsonObject& obj);
    void jsonToMqttConfig(JsonObject& obj);
    void jsonToModbusConfig(JsonObject& obj);
    void jsonToCalibrationConfig(JsonObject& obj);
};

