/**
 * @file Logger.h
 * @brief Advanced logging system with multiple output targets
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Multi-level logging to Serial, SPIFFS, and WebSocket
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"

/**
 * @class Logger
 * @brief Advanced logging system with multiple output targets
 * 
 * Singleton class providing comprehensive logging:
 * - Multiple log levels (TRACE, DEBUG, INFO, WARN, ERROR, FATAL)
 * - Multiple output targets (Serial, SPIFFS file, WebSocket)
 * - Log file rotation (max 100KB)
 * - Timestamp and module tagging
 * - Convenient macros for easy use
 */
class Logger {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to Logger instance
     */
    static Logger& getInstance();
    
    /**
     * @brief Initialize logger
     * @param level Initial log level (default INFO)
     * @return true if successful
     */
    bool init(LogLevel level = LogLevel::INFO);
    
    /**
     * @brief Set runtime log level
     * @param level New log level
     */
    void setLevel(LogLevel level);
    
    /**
     * @brief Get current log level
     * @return Current LogLevel
     */
    LogLevel getLevel() const { return currentLevel; }
    
    /**
     * @brief Log a message
     * @param level Log level
     * @param module Module name
     * @param message Message text
     */
    void log(LogLevel level, const char* module, const char* message);
    
    /**
     * @brief Log formatted message
     * @param level Log level
     * @param module Module name
     * @param format Printf-style format string
     * @param ... Variable arguments
     */
    void logf(LogLevel level, const char* module, const char* format, ...);
    
    /**
     * @brief Trace level log
     * @param module Module name
     * @param message Message text
     */
    void trace(const char* module, const char* message);
    
    /**
     * @brief Debug level log
     * @param module Module name
     * @param message Message text
     */
    void debug(const char* module, const char* message);
    
    /**
     * @brief Info level log
     * @param module Module name
     * @param message Message text
     */
    void info(const char* module, const char* message);
    
    /**
     * @brief Warning level log
     * @param module Module name
     * @param message Message text
     */
    void warn(const char* module, const char* message);
    
    /**
     * @brief Error level log
     * @param module Module name
     * @param message Message text
     */
    void error(const char* module, const char* message);
    
    /**
     * @brief Fatal level log
     * @param module Module name
     * @param message Message text
     */
    void fatal(const char* module, const char* message);
    
    /**
     * @brief Enable/disable Serial output
     * @param enabled True to enable (default)
     */
    void enableSerialOutput(bool enabled);
    
    /**
     * @brief Enable/disable file output
     * @param enabled True to enable
     * @param filename Log file path (default /log.txt)
     */
    void enableFileOutput(bool enabled, const char* filename = "/log.txt");
    
    /**
     * @brief Enable/disable WebSocket output
     * @param enabled True to enable
     */
    void enableWebSocketOutput(bool enabled);
    
    /**
     * @brief Get current log file path
     * @return Log file path string
     */
    const char* getLogFile() const { return logFilePath; }
    
    /**
     * @brief Rotate log file
     * 
     * Renames current log to .old and starts fresh log
     */
    void rotateLogFile();
    
    /**
     * @brief Check if log file needs rotation
     * @return true if file exceeds size limit
     */
    bool needsRotation();

private:
    // Private constructor for singleton
    Logger();
    
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    /**
     * @brief Get log level name string
     * @param level Log level
     * @return Level name
     */
    const char* getLevelName(LogLevel level);
    
    /**
     * @brief Write log entry to all enabled outputs
     * @param level Log level
     * @param module Module name
     * @param message Message text
     */
    void writeLog(LogLevel level, const char* module, const char* message);
    
    /**
     * @brief Format log entry
     * @param buffer Output buffer
     * @param bufSize Buffer size
     * @param level Log level
     * @param module Module name
     * @param message Message text
     */
    void formatLogEntry(char* buffer, size_t bufSize, LogLevel level, 
                       const char* module, const char* message);
    
    // State
    bool initialized;
    LogLevel currentLevel;
    
    // Output enables
    bool serialEnabled;
    bool fileEnabled;
    bool webSocketEnabled;
    
    // File output
    char logFilePath[64];
    static const uint32_t MAX_LOG_FILE_SIZE = 100 * 1024; // 100KB
};

// Convenience macros for logging
#define LOG_TRACE(module, ...) Logger::getInstance().logf(LogLevel::TRACE, module, __VA_ARGS__)
#define LOG_DEBUG(module, ...) Logger::getInstance().logf(LogLevel::DEBUG, module, __VA_ARGS__)
#define LOG_INFO(module, ...)  Logger::getInstance().logf(LogLevel::INFO, module, __VA_ARGS__)
#define LOG_WARN(module, ...)  Logger::getInstance().logf(LogLevel::WARN, module, __VA_ARGS__)
#define LOG_ERROR(module, ...) Logger::getInstance().logf(LogLevel::ERROR, module, __VA_ARGS__)
#define LOG_FATAL(module, ...) Logger::getInstance().logf(LogLevel::FATAL, module, __VA_ARGS__)
