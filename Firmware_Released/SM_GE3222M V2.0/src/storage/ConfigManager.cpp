/**
 * @file ConfigManager.cpp
 * @brief JSON configuration management implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "ConfigManager.h"
#include "SPIFFSManager.h"
#include "../core/ErrorHandler.h"
#include <SPIFFS.h>

ConfigManager::ConfigManager() : _loaded(false), _dirty(false) {
    restoreDefaults();
}

bool ConfigManager::init() {
    ErrorHandler::getInstance().logMessage(LogLevel::INFO, "Initializing ConfigManager...");

    // Ensure SPIFFS is mounted
    if (!SPIFFSManager::getInstance().isMounted()) {
        if (!SPIFFSManager::getInstance().init()) {
            ErrorHandler::getInstance().setError(ErrorCode::CONFIG_LOAD_FAIL);
            return false;
        }
    }

    // Try to load configuration
    if (!load()) {
        ErrorHandler::getInstance().logMessage(LogLevel::WARN, 
            "Failed to load config, using defaults");
        restoreDefaults();
        save(); // Save defaults
    }

    _loaded = true;
    _dirty = false;
    return true;
}

bool ConfigManager::load() {
    if (!deserializeFromFile(CONFIG_FILE)) {
        // Try backup
        ErrorHandler::getInstance().logMessage(LogLevel::WARN, 
            "Primary config failed, trying backup...");
        if (!deserializeFromFile(BACKUP_FILE)) {
            return false;
        }
    }

    if (!validate()) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            "Configuration validation failed");
        return false;
    }

    ErrorHandler::getInstance().logMessage(LogLevel::INFO, "Configuration loaded successfully");
    _dirty = false;
    return true;
}

bool ConfigManager::save() {
    if (!validate()) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            "Cannot save invalid configuration");
        ErrorHandler::getInstance().setError(ErrorCode::CONFIG_SAVE_FAIL);
        return false;
    }

    // Create backup of existing config
    if (SPIFFSManager::getInstance().exists(CONFIG_FILE)) {
        createBackup();
    }

    // Save new configuration
    if (!serializeToFile(CONFIG_FILE)) {
        ErrorHandler::getInstance().setError(ErrorCode::CONFIG_SAVE_FAIL);
        return false;
    }

    _dirty = false;
    ErrorHandler::getInstance().logMessage(LogLevel::INFO, "Configuration saved successfully");
    return true;
}

bool ConfigManager::createBackup() {
    if (!SPIFFSManager::getInstance().exists(CONFIG_FILE)) {
        return false;
    }

    // Remove old backup
    if (SPIFFSManager::getInstance().exists(BACKUP_FILE)) {
        SPIFFS.remove(BACKUP_FILE);
    }

    // Copy current config to backup
    File src = SPIFFS.open(CONFIG_FILE, "r");
    if (!src) {
        return false;
    }

    File dst = SPIFFS.open(BACKUP_FILE, "w");
    if (!dst) {
        src.close();
        return false;
    }

    while (src.available()) {
        dst.write(src.read());
    }

    src.close();
    dst.close();

    return true;
}

bool ConfigManager::restoreBackup() {
    return deserializeFromFile(BACKUP_FILE);
}

bool ConfigManager::serializeToFile(const char* filename) {
    DynamicJsonDocument doc(8192);

    // Create JSON structure
    JsonObject wifi = doc.createNestedObject("wifi");
    wifiConfigToJson(wifi);

    JsonObject mqtt = doc.createNestedObject("mqtt");
    mqttConfigToJson(mqtt);

    JsonObject modbus = doc.createNestedObject("modbus");
    modbusConfigToJson(modbus);

    JsonObject calibration = doc.createNestedObject("calibration");
    calibrationConfigToJson(calibration);

    // Open file for writing
    File file = SPIFFS.open(filename, "w");
    if (!file) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            String("Failed to open file for writing: ") + String(filename));
        return false;
    }

    // Serialize JSON to file
    size_t bytesWritten = serializeJsonPretty(doc, file);
    file.close();

    if (bytesWritten == 0) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Failed to write JSON to file");
        return false;
    }

    return true;
}

bool ConfigManager::deserializeFromFile(const char* filename) {
    if (!SPIFFSManager::getInstance().exists(filename)) {
        ErrorHandler::getInstance().logMessage(LogLevel::WARN, 
            String("Config file not found: ") + String(filename));
        return false;
    }

    File file = SPIFFS.open(filename, "r");
    if (!file) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            String("Failed to open file: ") + String(filename));
        return false;
    }

    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            String("JSON parse error: ") + String(error.c_str()));
        return false;
    }

    // Parse JSON into configuration structures
    if (doc.containsKey("wifi")) {
        JsonObject wifi = doc["wifi"];
        jsonToWifiConfig(wifi);
    }

    if (doc.containsKey("mqtt")) {
        JsonObject mqtt = doc["mqtt"];
        jsonToMqttConfig(mqtt);
    }

    if (doc.containsKey("modbus")) {
        JsonObject modbus = doc["modbus"];
        jsonToModbusConfig(modbus);
    }

    if (doc.containsKey("calibration")) {
        JsonObject calibration = doc["calibration"];
        jsonToCalibrationConfig(calibration);
    }

    return true;
}

void ConfigManager::wifiConfigToJson(JsonObject& obj) {
    obj["ssid"] = _wifiConfig.ssid;
    obj["password"] = _wifiConfig.password;
    obj["hostname"] = _wifiConfig.hostname;
    obj["useDHCP"] = _wifiConfig.useDHCP;
    obj["staticIP"] = _wifiConfig.staticIP;
    obj["gateway"] = _wifiConfig.gateway;
    obj["subnet"] = _wifiConfig.subnet;
    obj["dns1"] = _wifiConfig.dns1;
    obj["dns2"] = _wifiConfig.dns2;
    obj["apMode"] = _wifiConfig.apMode;
    obj["apSSID"] = _wifiConfig.apSSID;
    obj["apPassword"] = _wifiConfig.apPassword;
}

void ConfigManager::mqttConfigToJson(JsonObject& obj) {
    obj["enabled"] = _mqttConfig.enabled;
    obj["server"] = _mqttConfig.server;
    obj["port"] = _mqttConfig.port;
    obj["user"] = _mqttConfig.user;
    obj["password"] = _mqttConfig.password;
    obj["topicPrefix"] = _mqttConfig.topicPrefix;
    obj["publishInterval"] = _mqttConfig.publishInterval;
    obj["useTLS"] = _mqttConfig.useTLS;
}

void ConfigManager::modbusConfigToJson(JsonObject& obj) {
    obj["rtuEnabled"] = _modbusConfig.rtuEnabled;
    obj["rtuSlaveID"] = _modbusConfig.rtuSlaveID;
    obj["rtuBaudRate"] = _modbusConfig.rtuBaudRate;
    obj["rtuParity"] = _modbusConfig.rtuParity;
    obj["tcpEnabled"] = _modbusConfig.tcpEnabled;
    obj["tcpPort"] = _modbusConfig.tcpPort;
    obj["tcpMaxConnections"] = _modbusConfig.tcpMaxConnections;
}

void ConfigManager::calibrationConfigToJson(JsonObject& obj) {
    obj["lineFreq"] = _calibrationConfig.lineFreq;
    obj["pgaGain"] = _calibrationConfig.pgaGain;
    
    JsonArray vGain = obj.createNestedArray("voltageGain");
    for (int i = 0; i < 3; i++) {
        vGain.add(_calibrationConfig.voltageGain[i]);
    }
    
    JsonArray iGain = obj.createNestedArray("currentGain");
    for (int i = 0; i < 3; i++) {
        iGain.add(_calibrationConfig.currentGain[i]);
    }
    
    obj["currentGainN"] = _calibrationConfig.currentGainN;
}

void ConfigManager::jsonToWifiConfig(JsonObject& obj) {
    if (obj.containsKey("ssid")) strlcpy(_wifiConfig.ssid, obj["ssid"] | "", sizeof(_wifiConfig.ssid));
    if (obj.containsKey("password")) strlcpy(_wifiConfig.password, obj["password"] | "", sizeof(_wifiConfig.password));
    if (obj.containsKey("hostname")) strlcpy(_wifiConfig.hostname, obj["hostname"] | "SM-GE3222M", sizeof(_wifiConfig.hostname));
    if (obj.containsKey("useDHCP")) _wifiConfig.useDHCP = obj["useDHCP"];
    if (obj.containsKey("staticIP")) strlcpy(_wifiConfig.staticIP, obj["staticIP"] | "192.168.1.100", sizeof(_wifiConfig.staticIP));
    if (obj.containsKey("gateway")) strlcpy(_wifiConfig.gateway, obj["gateway"] | "192.168.1.1", sizeof(_wifiConfig.gateway));
    if (obj.containsKey("subnet")) strlcpy(_wifiConfig.subnet, obj["subnet"] | "255.255.255.0", sizeof(_wifiConfig.subnet));
    if (obj.containsKey("dns1")) strlcpy(_wifiConfig.dns1, obj["dns1"] | "8.8.8.8", sizeof(_wifiConfig.dns1));
    if (obj.containsKey("dns2")) strlcpy(_wifiConfig.dns2, obj["dns2"] | "8.8.4.4", sizeof(_wifiConfig.dns2));
    if (obj.containsKey("apMode")) _wifiConfig.apMode = obj["apMode"];
    if (obj.containsKey("apSSID")) strlcpy(_wifiConfig.apSSID, obj["apSSID"] | "SM-GE3222M-AP", sizeof(_wifiConfig.apSSID));
    if (obj.containsKey("apPassword")) strlcpy(_wifiConfig.apPassword, obj["apPassword"] | "12345678", sizeof(_wifiConfig.apPassword));
}

void ConfigManager::jsonToMqttConfig(JsonObject& obj) {
    if (obj.containsKey("enabled")) _mqttConfig.enabled = obj["enabled"];
    if (obj.containsKey("server")) strlcpy(_mqttConfig.server, obj["server"] | "", sizeof(_mqttConfig.server));
    if (obj.containsKey("port")) _mqttConfig.port = obj["port"];
    if (obj.containsKey("user")) strlcpy(_mqttConfig.user, obj["user"] | "", sizeof(_mqttConfig.user));
    if (obj.containsKey("password")) strlcpy(_mqttConfig.password, obj["password"] | "", sizeof(_mqttConfig.password));
    if (obj.containsKey("topicPrefix")) strlcpy(_mqttConfig.topicPrefix, obj["topicPrefix"] | "smartmeter", sizeof(_mqttConfig.topicPrefix));
    if (obj.containsKey("publishInterval")) _mqttConfig.publishInterval = obj["publishInterval"];
    if (obj.containsKey("useTLS")) _mqttConfig.useTLS = obj["useTLS"];
}

void ConfigManager::jsonToModbusConfig(JsonObject& obj) {
    if (obj.containsKey("rtuEnabled")) _modbusConfig.rtuEnabled = obj["rtuEnabled"];
    if (obj.containsKey("rtuSlaveID")) _modbusConfig.rtuSlaveID = obj["rtuSlaveID"];
    if (obj.containsKey("rtuBaudRate")) _modbusConfig.rtuBaudRate = obj["rtuBaudRate"];
    if (obj.containsKey("rtuParity")) _modbusConfig.rtuParity = obj["rtuParity"];
    if (obj.containsKey("tcpEnabled")) _modbusConfig.tcpEnabled = obj["tcpEnabled"];
    if (obj.containsKey("tcpPort")) _modbusConfig.tcpPort = obj["tcpPort"];
    if (obj.containsKey("tcpMaxConnections")) _modbusConfig.tcpMaxConnections = obj["tcpMaxConnections"];
}

void ConfigManager::jsonToCalibrationConfig(JsonObject& obj) {
    if (obj.containsKey("lineFreq")) _calibrationConfig.lineFreq = obj["lineFreq"];
    if (obj.containsKey("pgaGain")) _calibrationConfig.pgaGain = obj["pgaGain"];
    
    if (obj.containsKey("voltageGain")) {
        JsonArray vGain = obj["voltageGain"];
        for (int i = 0; i < 3 && i < vGain.size(); i++) {
            _calibrationConfig.voltageGain[i] = vGain[i];
        }
    }
    
    if (obj.containsKey("currentGain")) {
        JsonArray iGain = obj["currentGain"];
        for (int i = 0; i < 3 && i < iGain.size(); i++) {
            _calibrationConfig.currentGain[i] = iGain[i];
        }
    }
    
    if (obj.containsKey("currentGainN")) _calibrationConfig.currentGainN = obj["currentGainN"];
}

void ConfigManager::setWiFiConfig(const WiFiConfig& config) {
    if (validateWiFiConfig(config)) {
        _wifiConfig = config;
        _dirty = true;
    }
}

void ConfigManager::setMQTTConfig(const MQTTConfig& config) {
    if (validateMQTTConfig(config)) {
        _mqttConfig = config;
        _dirty = true;
    }
}

void ConfigManager::setModbusConfig(const ModbusConfig& config) {
    if (validateModbusConfig(config)) {
        _modbusConfig = config;
        _dirty = true;
    }
}

void ConfigManager::setCalibrationConfig(const CalibrationConfig& config) {
    if (validateCalibrationConfig(config)) {
        _calibrationConfig = config;
        _dirty = true;
    }
}

String ConfigManager::getConfigJSON() {
    DynamicJsonDocument doc(8192);

    JsonObject wifi = doc.createNestedObject("wifi");
    wifiConfigToJson(wifi);

    JsonObject mqtt = doc.createNestedObject("mqtt");
    mqttConfigToJson(mqtt);

    JsonObject modbus = doc.createNestedObject("modbus");
    modbusConfigToJson(modbus);

    JsonObject calibration = doc.createNestedObject("calibration");
    calibrationConfigToJson(calibration);

    String output;
    serializeJsonPretty(doc, output);
    return output;
}

bool ConfigManager::setConfigJSON(const String& jsonStr) {
    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, jsonStr);

    if (error) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            String("JSON parse error: ") + String(error.c_str()));
        return false;
    }

    // Parse and validate each section
    if (doc.containsKey("wifi")) {
        JsonObject wifi = doc["wifi"];
        jsonToWifiConfig(wifi);
    }

    if (doc.containsKey("mqtt")) {
        JsonObject mqtt = doc["mqtt"];
        jsonToMqttConfig(mqtt);
    }

    if (doc.containsKey("modbus")) {
        JsonObject modbus = doc["modbus"];
        jsonToModbusConfig(modbus);
    }

    if (doc.containsKey("calibration")) {
        JsonObject calibration = doc["calibration"];
        jsonToCalibrationConfig(calibration);
    }

    if (!validate()) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            "Configuration validation failed after import");
        return false;
    }

    _dirty = true;
    return true;
}

bool ConfigManager::validate() {
    return validateWiFiConfig(_wifiConfig) &&
           validateMQTTConfig(_mqttConfig) &&
           validateModbusConfig(_modbusConfig) &&
           validateCalibrationConfig(_calibrationConfig);
}

bool ConfigManager::validateWiFiConfig(const WiFiConfig& config) {
    // Hostname must not be empty
    if (strlen(config.hostname) == 0) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "WiFi: hostname cannot be empty");
        return false;
    }

    // AP password must be at least 8 characters if AP mode enabled
    if (config.apMode && strlen(config.apPassword) < 8) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "WiFi: AP password must be at least 8 characters");
        return false;
    }

    return true;
}

bool ConfigManager::validateMQTTConfig(const MQTTConfig& config) {
    // If enabled, server must not be empty
    if (config.enabled && strlen(config.server) == 0) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "MQTT: server cannot be empty when enabled");
        return false;
    }

    // Port must be valid
    if (config.port == 0 || config.port > 65535) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "MQTT: invalid port");
        return false;
    }

    // Publish interval must be reasonable
    if (config.publishInterval < 100 || config.publishInterval > 3600000) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "MQTT: publish interval out of range");
        return false;
    }

    return true;
}

bool ConfigManager::validateModbusConfig(const ModbusConfig& config) {
    // Slave ID must be 1-247
    if (config.rtuSlaveID < 1 || config.rtuSlaveID > 247) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Modbus: slave ID out of range (1-247)");
        return false;
    }

    // Baud rate must be standard
    if (config.rtuBaudRate != 9600 && config.rtuBaudRate != 19200 && 
        config.rtuBaudRate != 38400 && config.rtuBaudRate != 57600 && 
        config.rtuBaudRate != 115200) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Modbus: invalid baud rate");
        return false;
    }

    // Parity must be 0-2
    if (config.rtuParity > 2) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Modbus: invalid parity");
        return false;
    }

    // TCP port must be valid
    if (config.tcpPort == 0 || config.tcpPort > 65535) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Modbus: invalid TCP port");
        return false;
    }

    // Max connections must be reasonable
    if (config.tcpMaxConnections < 1 || config.tcpMaxConnections > 10) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Modbus: max connections out of range (1-10)");
        return false;
    }

    return true;
}

bool ConfigManager::validateCalibrationConfig(const CalibrationConfig& config) {
    // Basic sanity checks - values should be non-zero for gains
    for (int i = 0; i < 3; i++) {
        if (config.voltageGain[i] == 0) {
            ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
                String("Calibration: voltage gain[") + String(i) + "] cannot be zero");
            return false;
        }
        if (config.currentGain[i] == 0) {
            ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
                String("Calibration: current gain[") + String(i) + "] cannot be zero");
            return false;
        }
    }

    if (config.currentGainN == 0) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Calibration: neutral current gain cannot be zero");
        return false;
    }

    return true;
}

void ConfigManager::restoreDefaults() {
    ErrorHandler::getInstance().logMessage(LogLevel::INFO, "Restoring factory defaults");
    
    _wifiConfig = WiFiConfig();
    _mqttConfig = MQTTConfig();
    _modbusConfig = ModbusConfig();
    _calibrationConfig = CalibrationConfig();
    
    _dirty = true;
}
