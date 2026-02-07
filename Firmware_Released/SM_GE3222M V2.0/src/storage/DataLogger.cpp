/**
 * @file DataLogger.cpp
 * @brief Historical data logging implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "DataLogger.h"
#include "SPIFFSManager.h"
#include "../core/ErrorHandler.h"
#include <SPIFFS.h>
#include <time.h>

DataLogger::DataLogger() 
    : _initialized(false), 
      _enabled(true),
      _maxSize(1048576),
      _fieldMask(LogField::ALL_FIELDS),
      _logInterval(60000),
      _lastLogTime(0),
      _entryCount(0) {
    strcpy(_filename, "/datalog.csv");
}

bool DataLogger::init(const char* filename, size_t maxSize) {
    if (_initialized) {
        return true;
    }

    // Ensure SPIFFS is mounted
    if (!SPIFFSManager::getInstance().isMounted()) {
        if (!SPIFFSManager::getInstance().init()) {
            ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
                "DataLogger: SPIFFS not mounted");
            return false;
        }
    }

    strlcpy(_filename, filename, sizeof(_filename));
    _maxSize = maxSize;

    // Check if log file exists
    if (!SPIFFSManager::getInstance().exists(_filename)) {
        // Create new file with header
        if (!writeHeader()) {
            ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
                "DataLogger: Failed to create log file");
            return false;
        }
        _entryCount = 0;
    } else {
        // Count existing entries
        File file = SPIFFS.open(_filename, "r");
        if (file) {
            _entryCount = 0;
            while (file.available()) {
                String line = file.readStringUntil('\n');
                if (line.length() > 0) {
                    _entryCount++;
                }
            }
            if (_entryCount > 0) _entryCount--; // Subtract header line
            file.close();
        }
    }

    _initialized = true;
    ErrorHandler::getInstance().logMessage(LogLevel::INFO, 
        String("DataLogger initialized: ") + String(_entryCount) + " entries");
    
    return true;
}

bool DataLogger::writeHeader() {
    File file = SPIFFS.open(_filename, "w");
    if (!file) {
        return false;
    }

    // Write CSV header based on field mask
    String header = "";
    
    if (_fieldMask & LogField::TIMESTAMP) header += "Timestamp,";
    if (_fieldMask & LogField::VOLTAGE_A) header += "V1,";
    if (_fieldMask & LogField::VOLTAGE_B) header += "V2,";
    if (_fieldMask & LogField::VOLTAGE_C) header += "V3,";
    if (_fieldMask & LogField::CURRENT_A) header += "I1,";
    if (_fieldMask & LogField::CURRENT_B) header += "I2,";
    if (_fieldMask & LogField::CURRENT_C) header += "I3,";
    if (_fieldMask & LogField::POWER_TOTAL) header += "P_total,";
    if (_fieldMask & LogField::POWER_FACTOR) header += "PF,";
    if (_fieldMask & LogField::FREQUENCY) header += "Freq,";
    if (_fieldMask & LogField::TEMPERATURE) header += "Temp,";
    if (_fieldMask & LogField::ENERGY_IMPORT) header += "E_import,";
    if (_fieldMask & LogField::ENERGY_EXPORT) header += "E_export,";

    // Remove trailing comma
    if (header.length() > 0 && header.endsWith(",")) {
        header.remove(header.length() - 1);
    }

    file.println(header);
    file.close();
    return true;
}

String DataLogger::formatLogEntry(const MeterData& data) {
    String entry = "";

    if (_fieldMask & LogField::TIMESTAMP) {
        entry += String(millis()) + ",";
    }
    if (_fieldMask & LogField::VOLTAGE_A) {
        entry += String(data.phaseA.voltage, 2) + ",";
    }
    if (_fieldMask & LogField::VOLTAGE_B) {
        entry += String(data.phaseB.voltage, 2) + ",";
    }
    if (_fieldMask & LogField::VOLTAGE_C) {
        entry += String(data.phaseC.voltage, 2) + ",";
    }
    if (_fieldMask & LogField::CURRENT_A) {
        entry += String(data.phaseA.current, 3) + ",";
    }
    if (_fieldMask & LogField::CURRENT_B) {
        entry += String(data.phaseB.current, 3) + ",";
    }
    if (_fieldMask & LogField::CURRENT_C) {
        entry += String(data.phaseC.current, 3) + ",";
    }
    if (_fieldMask & LogField::POWER_TOTAL) {
        entry += String(data.totalActivePower, 2) + ",";
    }
    if (_fieldMask & LogField::POWER_FACTOR) {
        entry += String(data.totalPowerFactor, 3) + ",";
    }
    if (_fieldMask & LogField::FREQUENCY) {
        entry += String(data.frequency, 2) + ",";
    }
    if (_fieldMask & LogField::TEMPERATURE) {
        entry += String(data.temperature, 1) + ",";
    }
    if (_fieldMask & LogField::ENERGY_IMPORT) {
        entry += String(data.importActiveEnergy[0], 3) + ",";
    }
    if (_fieldMask & LogField::ENERGY_EXPORT) {
        entry += String(data.exportActiveEnergy[0], 3) + ",";
    }

    // Remove trailing comma
    if (entry.length() > 0 && entry.endsWith(",")) {
        entry.remove(entry.length() - 1);
    }

    return entry;
}

bool DataLogger::log(const MeterData& data) {
    if (!_initialized || !_enabled) {
        return false;
    }

    // Check if enough time has passed
    unsigned long now = millis();
    if (now - _lastLogTime < _logInterval) {
        return false;
    }

    // Check if rotation needed
    if (needsRotation()) {
        if (!rotate()) {
            ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
                "DataLogger: Failed to rotate log file");
            // Continue anyway, try to log
        }
    }

    // Open file for append
    File file = SPIFFS.open(_filename, "a");
    if (!file) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            "DataLogger: Failed to open log file for append");
        return false;
    }

    // Write log entry
    String entry = formatLogEntry(data);
    file.println(entry);
    file.close();

    _entryCount++;
    _lastLogTime = now;

    return true;
}

void DataLogger::setFields(uint16_t fieldMask) {
    if (_fieldMask != fieldMask) {
        _fieldMask = fieldMask;
        
        // If already initialized, recreate header
        if (_initialized) {
            // Backup existing data
            String backupName = String(_filename) + ".tmp";
            if (SPIFFSManager::getInstance().exists(_filename)) {
                // Rename existing file
                SPIFFS.rename(_filename, backupName.c_str());
            }
            
            // Create new file with new header
            writeHeader();
            
            // Optionally delete backup or keep it
            SPIFFS.remove(backupName.c_str());
            _entryCount = 0;
        }
    }
}

void DataLogger::setInterval(unsigned long intervalMs) {
    if (intervalMs >= 100 && intervalMs <= 3600000) {
        _logInterval = intervalMs;
    }
}

bool DataLogger::needsRotation() {
    if (!SPIFFSManager::getInstance().exists(_filename)) {
        return false;
    }

    size_t currentSize = SPIFFSManager::getInstance().getFileSize(_filename);
    return currentSize >= _maxSize;
}

String DataLogger::getRotatedFilename() {
    // Generate filename with timestamp: datalog_TIMESTAMP.csv
    String baseName = String(_filename);
    int dotPos = baseName.lastIndexOf('.');
    String name = baseName.substring(0, dotPos);
    String ext = baseName.substring(dotPos);
    
    return name + "_" + String(millis()) + ext;
}

bool DataLogger::rotate() {
    if (!SPIFFSManager::getInstance().exists(_filename)) {
        return writeHeader();
    }

    // Rename current log to timestamped file
    String rotatedName = getRotatedFilename();
    
    ErrorHandler::getInstance().logMessage(LogLevel::INFO, 
        String("Rotating log: ") + String(_filename) + " -> " + rotatedName);

    if (!SPIFFS.rename(_filename, rotatedName.c_str())) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
            "DataLogger: Failed to rename log file");
        return false;
    }

    // Create new log file with header
    _entryCount = 0;
    return writeHeader();
}

bool DataLogger::clear() {
    if (SPIFFSManager::getInstance().exists(_filename)) {
        if (!SPIFFS.remove(_filename)) {
            ErrorHandler::getInstance().logMessage(LogLevel::ERROR, 
                "DataLogger: Failed to delete log file");
            return false;
        }
    }

    _entryCount = 0;
    ErrorHandler::getInstance().logMessage(LogLevel::INFO, "DataLogger: Log cleared");
    
    return writeHeader();
}

size_t DataLogger::getSize() {
    if (!SPIFFSManager::getInstance().exists(_filename)) {
        return 0;
    }
    return SPIFFSManager::getInstance().getFileSize(_filename);
}

size_t DataLogger::getEntryCount() {
    return _entryCount;
}
