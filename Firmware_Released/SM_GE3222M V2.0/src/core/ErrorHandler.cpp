/**
 * @file ErrorHandler.cpp
 * @brief Implementation of centralized error management system
 *
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "ErrorHandler.h"

ErrorHandler& ErrorHandler::getInstance() {
    static ErrorHandler instance;
    return instance;
}

void ErrorHandler::reportError(ErrorCode code, const char* module, const char* message) {
    // Create error record
    ErrorRecord record;
    record.code = code;
    record.timestamp = millis();

    if (module != nullptr) {
        strncpy(record.module, module, sizeof(record.module) - 1);
        record.module[sizeof(record.module) - 1] = '\0';
    }

    if (message != nullptr) {
        strncpy(record.message, message, sizeof(record.message) - 1);
        record.message[sizeof(record.message) - 1] = '\0';
    }

    // Update statistics
    totalErrors++;
    if (static_cast<uint8_t>(code) < 19) {
        errorCounts[static_cast<uint8_t>(code)]++;
    }

    if (isCriticalError(code)) {
        criticalErrors++;
    }

    // Add to history
    addToHistory(record);

    // Log to Serial
    Serial.print("[ERROR] ");
    Serial.print(getErrorCodeName(code));
    Serial.print(" in ");
    Serial.print(record.module);
    Serial.print(": ");
    Serial.println(record.message);

    // Execute recovery action
    executeRecoveryAction(code);
}

uint32_t ErrorHandler::getErrorCount(ErrorCode code) const {
    uint8_t index = static_cast<uint8_t>(code);
    if (index >= 19) {
        return 0;
    }
    return errorCounts[index];
}

void ErrorHandler::clearErrors() {
    historyHead = 0;
    historyCount = 0;
    totalErrors = 0;
    criticalErrors = 0;

    for (uint8_t i = 0; i < 19; i++) {
        errorCounts[i] = 0;
    }

    Serial.println("[ErrorHandler] All errors cleared");
}

const ErrorRecord& ErrorHandler::getLastError() const {
    if (historyCount == 0) {
        static ErrorRecord emptyRecord;
        return emptyRecord;
    }

    // Get most recent error (head - 1)
    uint8_t lastIndex = (historyHead + ERROR_HISTORY_SIZE - 1) % ERROR_HISTORY_SIZE;
    return errorHistory[lastIndex];
}

const ErrorRecord* ErrorHandler::getErrorHistory(uint8_t index) const {
    if (index >= historyCount) {
        return nullptr;
    }

    // Calculate actual index in circular buffer (most recent first)
    uint8_t actualIndex = (historyHead + ERROR_HISTORY_SIZE - 1 - index) % ERROR_HISTORY_SIZE;
    return &errorHistory[actualIndex];
}

uint8_t ErrorHandler::getHistoryCount() const {
    return historyCount;
}

bool ErrorHandler::hasCriticalErrors() const {
    return criticalErrors > 0;
}

const char* ErrorHandler::getErrorCodeName(ErrorCode code) {
    switch (code) {
    case ErrorCode::NONE: return "NONE";
    case ErrorCode::SPI_COMM_FAIL: return "SPI_COMM_FAIL";
    case ErrorCode::WIFI_DISCONNECTED: return "WIFI_DISCONNECTED";
    case ErrorCode::NVS_CORRUPT: return "NVS_CORRUPT";
    case ErrorCode::ATM_INIT_FAIL: return "ATM_INIT_FAIL";
    case ErrorCode::MODBUS_TIMEOUT: return "MODBUS_TIMEOUT";
    case ErrorCode::SPIFFS_MOUNT_FAIL: return "SPIFFS_MOUNT_FAIL";
    case ErrorCode::I2C_DEVICE_MISSING: return "I2C_DEVICE_MISSING";
    case ErrorCode::HEAP_LOW: return "HEAP_LOW";
    case ErrorCode::WATCHDOG_RESET: return "WATCHDOG_RESET";
    case ErrorCode::CALIBRATION_ERROR: return "CALIBRATION_ERROR";
    case ErrorCode::MQTT_DISCONNECTED: return "MQTT_DISCONNECTED";
    case ErrorCode::ETHERNET_LINK_DOWN: return "ETHERNET_LINK_DOWN";
    case ErrorCode::LCD_INIT_FAIL: return "LCD_INIT_FAIL";
    case ErrorCode::MCP23017_INIT_FAIL: return "MCP23017_INIT_FAIL";
    case ErrorCode::CONFIG_LOAD_FAIL: return "CONFIG_LOAD_FAIL";
    case ErrorCode::CONFIG_SAVE_FAIL: return "CONFIG_SAVE_FAIL";
    case ErrorCode::ENERGY_READ_FAIL: return "ENERGY_READ_FAIL";
    case ErrorCode::DHT_READ_FAIL: return "DHT_READ_FAIL";
    default: return "UNKNOWN";
    }
}

void ErrorHandler::addToHistory(const ErrorRecord& record) {
    errorHistory[historyHead] = record;
    historyHead = (historyHead + 1) % ERROR_HISTORY_SIZE;

    if (historyCount < ERROR_HISTORY_SIZE) {
        historyCount++;
    }
}

void ErrorHandler::executeRecoveryAction(ErrorCode code) {
    switch (code) {
    case ErrorCode::SPI_COMM_FAIL:
        Serial.println("[Recovery] SPI communication error - reinitialize recommended");
        break;

    case ErrorCode::WIFI_DISCONNECTED:
        Serial.println("[Recovery] WiFi disconnected - attempting reconnection");
        break;

    case ErrorCode::NVS_CORRUPT:
        Serial.println("[Recovery] NVS corrupted - format and restore defaults");
        break;

    case ErrorCode::ATM_INIT_FAIL:
        Serial.println("[Recovery] ATM90E32 init failed - check connections");
        break;

    case ErrorCode::MODBUS_TIMEOUT:
        Serial.println("[Recovery] Modbus timeout - check slave device");
        break;

    case ErrorCode::SPIFFS_MOUNT_FAIL:
        Serial.println("[Recovery] SPIFFS mount failed - format filesystem");
        break;

    case ErrorCode::I2C_DEVICE_MISSING:
        Serial.println("[Recovery] I2C device missing - check connections");
        break;

    case ErrorCode::HEAP_LOW:
        Serial.println("[Recovery] Low heap memory - reduce allocations");
        break;

    case ErrorCode::WATCHDOG_RESET:
        Serial.println("[Recovery] Watchdog reset detected - check for blocking code");
        break;

    case ErrorCode::MQTT_DISCONNECTED:
        Serial.println("[Recovery] MQTT disconnected - reconnecting");
        break;

    case ErrorCode::ETHERNET_LINK_DOWN:
        Serial.println("[Recovery] Ethernet link down - check cable");
        break;

    default:
        break;
    }
}

bool ErrorHandler::isCriticalError(ErrorCode code) const {
    switch (code) {
    case ErrorCode::NVS_CORRUPT:
    case ErrorCode::ATM_INIT_FAIL:
    case ErrorCode::SPIFFS_MOUNT_FAIL:
    case ErrorCode::WATCHDOG_RESET:
    case ErrorCode::HEAP_LOW:
        return true;
    default:
        return false;
    }
}

// Convenience logging methods
void ErrorHandler::logError(ErrorCode code, const String& message) {
    reportError(code, "System", message.c_str());
}

void ErrorHandler::logInfo(const String& message) {
    Serial.print("[INFO] ");
    Serial.println(message);
}

void ErrorHandler::logWarning(const String& message) {
    Serial.print("[WARNING] ");
    Serial.println(message);
}

void ErrorHandler::logMessage(LogLevel level, const String& message) {
    const char* levelStr = "";
    switch (level) {
    case LogLevel::TRACE:   levelStr = "TRACE"; break;
    case LogLevel::DEBUG:   levelStr = "DEBUG"; break;
    case LogLevel::INFO:    levelStr = "INFO"; break;
    case LogLevel::WARN:    levelStr = "WARN"; break;
    case LogLevel::ERROR:   levelStr = "ERROR"; break;
    case LogLevel::FATAL:   levelStr = "FATAL"; break;
    }
    Serial.print("[");
    Serial.print(levelStr);
    Serial.print("] ");
    Serial.println(message);
}

void ErrorHandler::setError(ErrorCode code) {
    reportError(code, "System", "");
}