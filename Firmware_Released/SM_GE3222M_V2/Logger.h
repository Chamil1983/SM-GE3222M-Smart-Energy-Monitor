/**
 * @file Logger.h
 * @brief Thread-safe logging system with ring buffer for SM-GE3222M
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "DataTypes.h"

class Logger {
public:
    static Logger& getInstance();
    
    // Returns true on success (mutex created and logger ready)
    bool init(LogLevel level, bool serialEnabled, bool fileEnabled = false);
    
    void error(const char* format, ...);
    void warn(const char* format, ...);
    void info(const char* format, ...);
    void debug(const char* format, ...);
    void trace(const char* format, ...);
    
    const LogEntry* getRecentLogs(size_t& count) const;
    void setLogLevel(LogLevel level);
    LogLevel getLogLevel() const;
    
private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void log(LogLevel level, const char* format, va_list args);
    const char* levelToString(LogLevel level) const;
    
    LogLevel _minLevel;
    bool _serialEnabled;
    bool _fileEnabled;
    
    static const size_t RING_BUFFER_SIZE = 100;
    LogEntry _ringBuffer[RING_BUFFER_SIZE];
    size_t _ringBufferHead;
    size_t _ringBufferCount;
    
    SemaphoreHandle_t _mutex;
    bool _initialized;
};

#endif // LOGGER_H
