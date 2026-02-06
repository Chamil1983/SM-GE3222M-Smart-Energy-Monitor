/**
 * @file Logger.cpp
 * @brief Advanced logging system implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "Logger.h"
#include <SPIFFS.h>
#include <stdarg.h>

Logger::Logger()
    : initialized(false),
      currentLevel(LogLevel::INFO),
      serialEnabled(true),
      fileEnabled(false),
      webSocketEnabled(false) {
    strcpy(logFilePath, "/log.txt");
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::init(LogLevel level) {
    currentLevel = level;
    serialEnabled = true;
    fileEnabled = false;
    webSocketEnabled = false;
    initialized = true;
    
    info("Logger", "Logger initialized");
    return true;
}

void Logger::setLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::log(LogLevel level, const char* module, const char* message) {
    if (!initialized) return;
    if (level < currentLevel) return;
    
    writeLog(level, module, message);
}

void Logger::logf(LogLevel level, const char* module, const char* format, ...) {
    if (!initialized) return;
    if (level < currentLevel) return;
    
    char message[256];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    
    writeLog(level, module, message);
}

void Logger::trace(const char* module, const char* message) {
    log(LogLevel::TRACE, module, message);
}

void Logger::debug(const char* module, const char* message) {
    log(LogLevel::DEBUG, module, message);
}

void Logger::info(const char* module, const char* message) {
    log(LogLevel::INFO, module, message);
}

void Logger::warn(const char* module, const char* message) {
    log(LogLevel::WARN, module, message);
}

void Logger::error(const char* module, const char* message) {
    log(LogLevel::ERROR, module, message);
}

void Logger::fatal(const char* module, const char* message) {
    log(LogLevel::FATAL, module, message);
}

void Logger::enableSerialOutput(bool enabled) {
    serialEnabled = enabled;
}

void Logger::enableFileOutput(bool enabled, const char* filename) {
    fileEnabled = enabled;
    if (filename) {
        strncpy(logFilePath, filename, sizeof(logFilePath) - 1);
        logFilePath[sizeof(logFilePath) - 1] = '\0';
    }
    
    // Check if rotation needed when enabling
    if (enabled && needsRotation()) {
        rotateLogFile();
    }
}

void Logger::enableWebSocketOutput(bool enabled) {
    webSocketEnabled = enabled;
}

void Logger::rotateLogFile() {
    if (!fileEnabled) return;
    
    // Close and rename current log to .old
    char oldPath[68];
    snprintf(oldPath, sizeof(oldPath), "%s.old", logFilePath);
    
    // Remove old backup if exists
    if (SPIFFS.exists(oldPath)) {
        SPIFFS.remove(oldPath);
    }
    
    // Rename current to old
    if (SPIFFS.exists(logFilePath)) {
        SPIFFS.rename(logFilePath, oldPath);
    }
}

bool Logger::needsRotation() {
    if (!SPIFFS.exists(logFilePath)) return false;
    
    File file = SPIFFS.open(logFilePath, FILE_READ);
    if (!file) return false;
    
    size_t size = file.size();
    file.close();
    
    return size >= MAX_LOG_FILE_SIZE;
}

const char* Logger::getLevelName(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKN ";
    }
}

void Logger::writeLog(LogLevel level, const char* module, const char* message) {
    char logEntry[384];
    formatLogEntry(logEntry, sizeof(logEntry), level, module, message);
    
    // Output to Serial
    if (serialEnabled) {
        Serial.println(logEntry);
    }
    
    // Output to file
    if (fileEnabled) {
        // Check rotation before writing
        if (needsRotation()) {
            rotateLogFile();
        }
        
        File file = SPIFFS.open(logFilePath, FILE_APPEND);
        if (file) {
            file.println(logEntry);
            file.close();
        }
    }
    
    // Output to WebSocket
    if (webSocketEnabled) {
        // WebSocket output would be implemented here
        // Requires WebServerManager integration
        // For now, this is a placeholder
    }
}

void Logger::formatLogEntry(char* buffer, size_t bufSize, LogLevel level, 
                           const char* module, const char* message) {
    unsigned long timestamp = millis();
    const char* levelName = getLevelName(level);
    
    snprintf(buffer, bufSize, "[%10lu][%s][%-12s] %s",
             timestamp, levelName, module, message);
}
