/**
 * @file DataLogger.h
 * @brief Historical data logging to SPIFFS with CSV format
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Logs meter data to CSV files with configurable fields and auto-rotation
 */

#pragma once

#include <Arduino.h>
#include <vector>
#include "../../include/GlobalTypes.h"

class DataLogger {
public:
    // Field selection flags
    enum LogField {
        TIMESTAMP       = 0x0001,
        VOLTAGE_A       = 0x0002,
        VOLTAGE_B       = 0x0004,
        VOLTAGE_C       = 0x0008,
        CURRENT_A       = 0x0010,
        CURRENT_B       = 0x0020,
        CURRENT_C       = 0x0040,
        POWER_TOTAL     = 0x0080,
        POWER_FACTOR    = 0x0100,
        FREQUENCY       = 0x0200,
        TEMPERATURE     = 0x0400,
        ENERGY_IMPORT   = 0x0800,
        ENERGY_EXPORT   = 0x1000,
        ALL_FIELDS      = 0xFFFF
    };

    // Singleton instance
    static DataLogger& getInstance() {
        static DataLogger instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    DataLogger(const DataLogger&) = delete;
    DataLogger& operator=(const DataLogger&) = delete;

    // Initialize logger
    bool init(const char* filename = "/datalog.csv", size_t maxSize = 1048576);

    // Log a meter data snapshot
    bool log(const MeterData& data);

    // Configure which fields to log
    void setFields(uint16_t fieldMask);
    uint16_t getFields() const { return _fieldMask; }

    // Set logging interval
    void setInterval(unsigned long intervalMs);
    unsigned long getInterval() const { return _logInterval; }

    // File operations
    bool rotate();
    bool clear();
    size_t getSize();
    size_t getEntryCount();

    // Status
    bool isEnabled() const { return _enabled; }
    void setEnabled(bool enabled) { _enabled = enabled; }
    bool isInitialized() const { return _initialized; }

    // Get log file path
    const char* getFilename() const { return _filename; }

private:
    DataLogger();
    ~DataLogger() {}

    bool _initialized;
    bool _enabled;
    char _filename[64];
    size_t _maxSize;
    uint16_t _fieldMask;
    unsigned long _logInterval;
    unsigned long _lastLogTime;
    size_t _entryCount;

    // Write CSV header
    bool writeHeader();

    // Format single log entry
    String formatLogEntry(const MeterData& data);

    // Check if rotation needed
    bool needsRotation();

    // Get rotated filename
    String getRotatedFilename();
};

