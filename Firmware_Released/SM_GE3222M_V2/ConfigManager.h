/**
 * @file ConfigManager.h
 * @brief NVS-based configuration manager for SM-GE3222M V2.0
 * @details Singleton pattern - Replaces V1.0 EEPROM_Defs.h functionality with NVS
 */

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include "DataTypes.h"

class ConfigManager {
public:
    static ConfigManager& getInstance();
    
    bool init();
    
    // WiFi Configuration
    bool loadWiFiConfig(WiFiConfig& config);
    bool saveWiFiConfig(const WiFiConfig& config);
    
    // Modbus Configuration
    bool loadModbusConfig(ModbusConfig& config);
    bool saveModbusConfig(const ModbusConfig& config);
    
    // MQTT Configuration
    bool loadMQTTConfig(MQTTConfig& config);
    bool saveMQTTConfig(const MQTTConfig& config);
    
    // Network Configuration
    bool loadNetworkConfig(NetworkConfig& config);
    bool saveNetworkConfig(const NetworkConfig& config);
    
    // System Configuration
    bool loadSystemConfig(SystemConfig& config);
    bool saveSystemConfig(const SystemConfig& config);
    
    // Factory Reset
    bool factoryReset();
    
    // Check if configs exist
    bool hasWiFiConfig();
    bool hasModbusConfig();
    bool hasMQTTConfig();
    bool hasNetworkConfig();
    bool hasSystemConfig();

    // Compatibility helpers for ProtocolV2 naming
    bool getWiFiConfig(WiFiConfig& config) { return loadWiFiConfig(config); }
    bool setWiFiConfig(const WiFiConfig& config) { return saveWiFiConfig(config); }
    bool getModbusConfig(ModbusConfig& config) { return loadModbusConfig(config); }
    bool setModbusConfig(const ModbusConfig& config) { return saveModbusConfig(config); }
    bool getMQTTConfig(MQTTConfig& config) { return loadMQTTConfig(config); }
    bool setMQTTConfig(const MQTTConfig& config) { return saveMQTTConfig(config); }
    bool getNetworkConfig(NetworkConfig& config) { return loadNetworkConfig(config); }
    bool setNetworkConfig(const NetworkConfig& config) { return saveNetworkConfig(config); }
    bool getSystemConfig(SystemConfig& config) { return loadSystemConfig(config); }
    bool setSystemConfig(const SystemConfig& config) { return saveSystemConfig(config); }

    
private:
    ConfigManager();
    ~ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    bool _initialized;
    
    // NVS Namespaces
    static const char* NVS_NAMESPACE_WIFI;
    static const char* NVS_NAMESPACE_MODBUS;
    static const char* NVS_NAMESPACE_MQTT;
    static const char* NVS_NAMESPACE_NETWORK;
    static const char* NVS_NAMESPACE_SYSTEM;
};

#endif // CONFIG_MANAGER_H
