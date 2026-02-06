/**
 * @file NVStorage.h
 * @brief Non-Volatile Storage wrapper using ESP32 NVS (Preferences library)
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides a unified interface for storing configuration and calibration data
 * in ESP32's NVS partition with CRC32 integrity checking
 */

#pragma once

#include <Arduino.h>
#include <Preferences.h>

// NVS Namespaces
namespace NVSNamespace {
    constexpr const char* WIFI = "wifi";
    constexpr const char* MQTT = "mqtt";
    constexpr const char* MODBUS = "modbus";
    constexpr const char* CALIBRATION = "calibration";
    constexpr const char* ENERGY = "energy";
    constexpr const char* SYSTEM = "system";
    constexpr const char* DEVICE = "device";
}

class NVStorage {
public:
    // Singleton instance
    static NVStorage& getInstance() {
        static NVStorage instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    NVStorage(const NVStorage&) = delete;
    NVStorage& operator=(const NVStorage&) = delete;

    // Initialize NVS
    bool init();

    // Float operations
    bool putFloat(const char* nameSpace, const char* key, float value);
    float getFloat(const char* nameSpace, const char* key, float defaultVal = 0.0f);

    // Integer operations
    bool putInt(const char* nameSpace, const char* key, int32_t value);
    int32_t getInt(const char* nameSpace, const char* key, int32_t defaultVal = 0);

    // Unsigned integer operations
    bool putUInt(const char* nameSpace, const char* key, uint32_t value);
    uint32_t getUInt(const char* nameSpace, const char* key, uint32_t defaultVal = 0);

    // String operations
    bool putString(const char* nameSpace, const char* key, const char* value);
    String getString(const char* nameSpace, const char* key, const char* defaultVal = "");

    // Binary data operations
    bool putBytes(const char* nameSpace, const char* key, const void* value, size_t len);
    size_t getBytes(const char* nameSpace, const char* key, void* buffer, size_t maxLen);

    // Key operations
    bool remove(const char* nameSpace, const char* key);
    bool clear(const char* nameSpace);
    bool clearAll();

    // CRC32 operations for data integrity
    bool saveCRC32(const char* nameSpace, const char* key, const void* data, size_t len);
    bool verifyCRC32(const char* nameSpace, const char* key, const void* data, size_t len);

    // Utility
    bool isInitialized() const { return _initialized; }

private:
    NVStorage() : _initialized(false) {}
    ~NVStorage() {}

    Preferences _prefs;
    bool _initialized;

    // CRC32 calculation
    uint32_t calculateCRC32(const void* data, size_t len);
    
    // Helper to open namespace
    bool openNamespace(const char* nameSpace, bool readOnly);
    void closeNamespace();
};

