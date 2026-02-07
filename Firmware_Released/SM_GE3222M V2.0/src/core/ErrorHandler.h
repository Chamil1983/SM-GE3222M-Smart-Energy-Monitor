/**
 * @file ErrorHandler.h
 * @brief Centralized error management system
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides centralized error logging, tracking, and recovery management
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"

#define ERROR_HISTORY_SIZE 50

/**
 * @struct ErrorRecord
 * @brief Individual error record with metadata
 */
struct ErrorRecord {
    ErrorCode code;
    char module[16];
    char message[64];
    unsigned long timestamp;
    
    ErrorRecord() : code(ErrorCode::NONE), timestamp(0) {
        module[0] = '\0';
        message[0] = '\0';
    }
};

/**
 * @class ErrorHandler
 * @brief Centralized error management with history and recovery
 * 
 * Singleton class that manages error reporting, tracking, and recovery
 * actions. Maintains a circular buffer of recent errors.
 */
class ErrorHandler {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to ErrorHandler instance
     */
    static ErrorHandler& getInstance();
    
    /**
     * @brief Report an error
     * @param code Error code from ErrorCode enum
     * @param module Module name reporting the error
     * @param message Error description
     * 
     * Logs the error, updates statistics, and triggers recovery actions
     */
    void reportError(ErrorCode code, const char* module, const char* message);
    
    /**
     * @brief Convenience method to log error (wraps reportError)
     * @param code Error code from ErrorCode enum
     * @param message Error description
     */
    void logError(ErrorCode code, const String& message);
    
    /**
     * @brief Log an info message
     * @param message Info message
     */
    void logInfo(const String& message);
    
    /**
     * @brief Log a warning message
     * @param message Warning message
     */
    void logWarning(const String& message);
    
    /**
     * @brief Log a message with specific level
     * @param level Log level
     * @param message Message to log
     */
    void logMessage(LogLevel level, const String& message);
    
    /**
     * @brief Set error without message (wraps reportError)
     * @param code Error code to set
     */
    void setError(ErrorCode code);
    
    /**
     * @brief Get error count for specific error code
     * @param code Error code to query
     * @return Number of times this error has occurred
     */
    uint32_t getErrorCount(ErrorCode code) const;
    
    /**
     * @brief Get total error count
     * @return Total number of errors reported
     */
    uint32_t getTotalErrorCount() const { return totalErrors; }
    
    /**
     * @brief Clear all error statistics
     */
    void clearErrors();
    
    /**
     * @brief Get most recent error
     * @return Reference to last error record
     */
    const ErrorRecord& getLastError() const;
    
    /**
     * @brief Get error from history
     * @param index History index (0 = most recent)
     * @return Pointer to error record, or nullptr if invalid index
     */
    const ErrorRecord* getErrorHistory(uint8_t index) const;
    
    /**
     * @brief Get number of errors in history
     * @return Number of errors stored in circular buffer
     */
    uint8_t getHistoryCount() const;
    
    /**
     * @brief Check if system has critical errors
     * @return true if any critical errors are present
     */
    bool hasCriticalErrors() const;
    
    /**
     * @brief Get error code name as string
     * @param code Error code
     * @return String representation of error code
     */
    static const char* getErrorCodeName(ErrorCode code);

private:
    ErrorHandler() : historyHead(0), historyCount(0), totalErrors(0), criticalErrors(0) {
        // Initialize error counters
        for (uint8_t i = 0; i < 26; i++) {
            errorCounts[i] = 0;
        }
    }
    
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;
    
    ErrorRecord errorHistory[ERROR_HISTORY_SIZE];
    uint8_t historyHead;
    uint8_t historyCount;
    uint32_t totalErrors;
    uint32_t criticalErrors;
    uint32_t errorCounts[26];  // One counter per ErrorCode
    
    /**
     * @brief Add error to circular buffer
     * @param record Error record to add
     */
    void addToHistory(const ErrorRecord& record);
    
    /**
     * @brief Execute recovery action for error
     * @param code Error code requiring recovery
     */
    void executeRecoveryAction(ErrorCode code);
    
    /**
     * @brief Check if error is critical
     * @param code Error code to check
     * @return true if error is critical
     */
    bool isCriticalError(ErrorCode code) const;
};
