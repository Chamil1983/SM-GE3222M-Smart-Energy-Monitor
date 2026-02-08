/**
 * @file Logger.cpp
 * @brief Thread-safe logging system implementation
 */

#include "Logger.h"
#include <cstdarg>
#include <cstdio>

Logger::Logger() 
    : _minLevel(LogLevel::INFO)
    , _serialEnabled(false)
    , _fileEnabled(false)
    , _ringBufferHead(0)
    , _ringBufferCount(0)
    , _mutex(nullptr)
    , _initialized(false) {
}

Logger::~Logger() {
    if (_mutex != nullptr) {
        vSemaphoreDelete(_mutex);
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::init(LogLevel level, bool serialEnabled, bool fileEnabled) {
    if (_initialized) {
        return;
    }
    
    _minLevel = level;
    _serialEnabled = serialEnabled;
    _fileEnabled = fileEnabled;
    
    _mutex = xSemaphoreCreateMutex();
    if (_mutex == nullptr) {
        if (_serialEnabled) {
            Serial.println("[ERROR] Failed to create logger mutex");
        }
        return;
    }
    
    _initialized = true;
    
    if (_serialEnabled) {
        Serial.println("[INFO] Logger initialized");
    }
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::ERROR, format, args);
    va_end(args);
}

void Logger::warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::WARN, format, args);
    va_end(args);
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::INFO, format, args);
    va_end(args);
}

void Logger::debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::DEBUG, format, args);
    va_end(args);
}

void Logger::trace(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::TRACE, format, args);
    va_end(args);
}

void Logger::log(LogLevel level, const char* format, va_list args) {
    if (!_initialized || level > _minLevel) {
        return;
    }
    
    if (_mutex == nullptr || xSemaphoreTake(_mutex, portMAX_DELAY) != pdTRUE) {
        return;
    }
    
    char buffer[128];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    unsigned long timestamp = millis();
    
    if (_serialEnabled) {
        Serial.printf("[%lu][%s] %s\n", timestamp, levelToString(level), buffer);
    }
    
    LogEntry& entry = _ringBuffer[_ringBufferHead];
    entry.timestamp = timestamp;
    entry.level = level;
    strncpy(entry.module, "SYSTEM", sizeof(entry.module) - 1);
    entry.module[sizeof(entry.module) - 1] = '\0';
    strncpy(entry.message, buffer, sizeof(entry.message) - 1);
    entry.message[sizeof(entry.message) - 1] = '\0';
    
    _ringBufferHead = (_ringBufferHead + 1) % RING_BUFFER_SIZE;
    if (_ringBufferCount < RING_BUFFER_SIZE) {
        _ringBufferCount++;
    }
    
    xSemaphoreGive(_mutex);
}

const char* Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::TRACE: return "TRACE";
        default:              return "UNKNOWN";
    }
}

const LogEntry* Logger::getRecentLogs(size_t& count) const {
    if (_mutex != nullptr && xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE) {
        count = _ringBufferCount;
        xSemaphoreGive(_mutex);
        return _ringBuffer;
    }
    count = 0;
    return nullptr;
}

void Logger::setLogLevel(LogLevel level) {
    if (_mutex != nullptr && xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE) {
        _minLevel = level;
        xSemaphoreGive(_mutex);
    }
}

LogLevel Logger::getLogLevel() const {
    return _minLevel;
}
