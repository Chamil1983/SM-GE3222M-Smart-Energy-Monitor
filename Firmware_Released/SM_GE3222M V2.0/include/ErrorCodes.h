/**
 * @file ErrorCodes.h
 * @brief Centralized error code definitions
 * @author Microcode Engineering
 * @date 2026-02-08
 */

#ifndef ERRORCODES_H
#define ERRORCODES_H

#include <cstdint>

/**
 * @brief Error code enumeration with category prefixes
 * 
 * Categories:
 * - 0x01xx: ATM90E36 Energy IC errors
 * - 0x02xx: WiFi/Network errors
 * - 0x03xx: TCP/Communication errors
 * - 0x04xx: Configuration/Storage errors
 * - 0x05xx: Energy measurement errors
 * - 0x06xx: System errors
 */
enum class ErrorCode : uint16_t {
    // Success
    SUCCESS = 0x0000,
    
    // ATM90E36 Errors (0x01xx)
    ATM_SPI_INIT_FAILED = 0x0101,
    ATM_CHIP_NOT_RESPONDING = 0x0102,
    ATM_CHECKSUM_MISMATCH = 0x0103,
    ATM_CONFIG_FAILED = 0x0104,
    ATM_CALIBRATION_FAILED = 0x0105,
    ATM_READ_TIMEOUT = 0x0106,
    ATM_WRITE_FAILED = 0x0107,
    ATM_INVALID_REGISTER = 0x0108,
    
    // WiFi Errors (0x02xx)
    WIFI_INIT_FAILED = 0x0201,
    WIFI_CONNECT_FAILED = 0x0202,
    WIFI_CONNECT_TIMEOUT = 0x0203,
    WIFI_DISCONNECTED = 0x0204,
    WIFI_AP_START_FAILED = 0x0205,
    WIFI_WEAK_SIGNAL = 0x0206,
    WIFI_DHCP_FAILED = 0x0207,
    WIFI_DNS_FAILED = 0x0208,
    
    // TCP/Communication Errors (0x03xx)
    TCP_SERVER_START_FAILED = 0x0301,
    TCP_CLIENT_CONNECT_FAILED = 0x0302,
    TCP_SEND_FAILED = 0x0303,
    TCP_RECEIVE_FAILED = 0x0304,
    TCP_TIMEOUT = 0x0305,
    TCP_MAX_CLIENTS_REACHED = 0x0306,
    TCP_PROTOCOL_ERROR = 0x0307,
    TCP_INVALID_COMMAND = 0x0308,
    
    // Configuration/Storage Errors (0x04xx)
    CONFIG_FILE_NOT_FOUND = 0x0401,
    CONFIG_PARSE_ERROR = 0x0402,
    CONFIG_VALIDATION_ERROR = 0x0403,
    CONFIG_SAVE_FAILED = 0x0404,
    SPIFFS_MOUNT_FAILED = 0x0405,
    SPIFFS_FORMAT_FAILED = 0x0406,
    SPIFFS_WRITE_FAILED = 0x0407,
    SPIFFS_READ_FAILED = 0x0408,
    EEPROM_INIT_FAILED = 0x0409,
    EEPROM_WRITE_FAILED = 0x040A,
    EEPROM_READ_FAILED = 0x040B,
    EEPROM_VALIDATION_FAILED = 0x040C,
    
    // Energy Measurement Errors (0x05xx)
    ENERGY_VOLTAGE_OUT_OF_RANGE = 0x0501,
    ENERGY_CURRENT_OUT_OF_RANGE = 0x0502,
    ENERGY_FREQUENCY_OUT_OF_RANGE = 0x0503,
    ENERGY_POWER_FACTOR_INVALID = 0x0504,
    ENERGY_PHASE_LOSS = 0x0505,
    ENERGY_OVERVOLTAGE = 0x0506,
    ENERGY_UNDERVOLTAGE = 0x0507,
    ENERGY_OVERCURRENT = 0x0508,
    ENERGY_CALIBRATION_INVALID = 0x0509,
    ENERGY_ACCUMULATOR_OVERFLOW = 0x050A,
    
    // System Errors (0x06xx)
    SYSTEM_LOW_MEMORY = 0x0601,
    SYSTEM_CRITICAL_MEMORY = 0x0602,
    SYSTEM_WATCHDOG_TIMEOUT = 0x0603,
    SYSTEM_STACK_OVERFLOW = 0x0604,
    SYSTEM_TASK_CREATION_FAILED = 0x0605,
    SYSTEM_MUTEX_TIMEOUT = 0x0606,
    SYSTEM_INVALID_STATE = 0x0607,
    SYSTEM_INIT_FAILED = 0x0608,
    SYSTEM_REBOOT_REQUESTED = 0x0609,
    SYSTEM_OTA_FAILED = 0x060A,
    SYSTEM_NTP_SYNC_FAILED = 0x060B,
};

/**
 * @brief Error severity levels
 */
enum class ErrorSeverity : uint8_t {
    INFO = 0,      ///< Informational, no action required
    WARNING = 1,   ///< Warning, system continues operation
    ERROR = 2,     ///< Error, feature may not work correctly
    CRITICAL = 3,  ///< Critical, system operation compromised
    FATAL = 4      ///< Fatal, system cannot continue
};

/**
 * @brief Get error category from error code
 * @param code Error code
 * @return Category code (0x01-0x06)
 */
inline uint8_t getErrorCategory(ErrorCode code) {
    return (static_cast<uint16_t>(code) >> 8) & 0xFF;
}

/**
 * @brief Get error code within category
 * @param code Error code
 * @return Specific error code within category
 */
inline uint8_t getErrorSubcode(ErrorCode code) {
    return static_cast<uint16_t>(code) & 0xFF;
}

/**
 * @brief Convert error code to string
 * @param code Error code
 * @return Human-readable error description
 */
const char* errorCodeToString(ErrorCode code);

/**
 * @brief Get default severity for error code
 * @param code Error code
 * @return Default severity level
 */
ErrorSeverity getDefaultSeverity(ErrorCode code);

#endif // ERRORCODES_H
