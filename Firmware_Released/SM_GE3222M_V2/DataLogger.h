/**
 * @file DataLogger.h
 * @brief Ring buffer-based data logger for SM-GE3222M V2.0
 * @details Singleton pattern - Stores meter readings with thread-safe access
 */

#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <vector>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "DataTypes.h"

struct LoggedReading {
    MeterData data;
    uint32_t timestamp;
    
    LoggedReading() : timestamp(0) {}
};

class DataLogger {
public:
    static DataLogger& getInstance();
    
    bool init(size_t maxEntries = 1000);
    
    void logReading(const MeterData& data);
    std::vector<LoggedReading> getRecentReadings(size_t count);
    
    bool exportToCSV();
    void clearBuffer();
    
    size_t getBufferSize() const;
    size_t getBufferCount() const;
    
private:
    DataLogger();
    ~DataLogger();
    DataLogger(const DataLogger&) = delete;
    DataLogger& operator=(const DataLogger&) = delete;
    
    bool _initialized;
    size_t _maxEntries;
    size_t _head;
    size_t _count;
    
    LoggedReading* _buffer;
    SemaphoreHandle_t _mutex;
    
    String generateCSVHeader();
    String generateCSVRow(const LoggedReading& reading);
};

#endif // DATA_LOGGER_H
