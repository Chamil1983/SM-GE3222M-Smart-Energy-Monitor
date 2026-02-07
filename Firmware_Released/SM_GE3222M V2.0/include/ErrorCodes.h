/**
 * @file ErrorCodes.h
 * @brief Centralized error code definitions
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Defines error codes for all subsystems
 */

#pragma once

/**
 * @enum ErrorCode
 * @brief System-wide error codes with subsystem prefixes
 */
enum class ErrorCode {
    // Success
    SUCCESS = 0,
    
    // Core errors (1xx)
    CORE_INIT_FAILED = 100,
    CORE_STATE_INVALID = 101,
    CORE_WATCHDOG_TIMEOUT = 102,
    CORE_TASK_OVERFLOW = 103,
    CORE_EVENT_QUEUE_FULL = 104,
    CORE_MEMORY_ALLOC_FAILED = 105,
    
    // HAL errors (2xx)
    HAL_SPI_INIT_FAILED = 200,
    HAL_SPI_TRANSFER_FAILED = 201,
    HAL_SPI_TIMEOUT = 202,
    HAL_I2C_INIT_FAILED = 210,
    HAL_I2C_TRANSFER_FAILED = 211,
    HAL_I2C_TIMEOUT = 212,
    HAL_GPIO_INIT_FAILED = 220,
    HAL_PIN_INVALID = 221,
    
    // Energy IC errors (3xx)
    ENERGY_IC_NOT_FOUND = 300,
    ENERGY_IC_INIT_FAILED = 301,
    ENERGY_IC_COMM_ERROR = 302,
    ENERGY_IC_CALIBRATION_ERROR = 303,
    ENERGY_IC_CHECKSUM_ERROR = 304,
    ENERGY_IC_REGISTER_READ_FAILED = 305,
    ENERGY_IC_REGISTER_WRITE_FAILED = 306,
    ENERGY_IC_MEASUREMENT_INVALID = 307,
    ENERGY_IC_TIMEOUT = 308,
    
    // Energy meter errors (31x)
    ENERGY_METER_NOT_INIT = 310,
    ENERGY_METER_DATA_INVALID = 311,
    ENERGY_METER_VOLTAGE_OUT_OF_RANGE = 312,
    ENERGY_METER_CURRENT_OUT_OF_RANGE = 313,
    ENERGY_METER_FREQUENCY_OUT_OF_RANGE = 314,
    ENERGY_METER_POWER_QUALITY_ISSUE = 315,
    
    // Calibration errors (32x)
    CALIBRATION_LOAD_FAILED = 320,
    CALIBRATION_SAVE_FAILED = 321,
    CALIBRATION_DATA_CORRUPT = 322,
    CALIBRATION_CHECKSUM_MISMATCH = 323,
    CALIBRATION_INVALID_PARAMS = 324,
    
    // Storage errors (4xx)
    STORAGE_NVS_INIT_FAILED = 400,
    STORAGE_NVS_READ_FAILED = 401,
    STORAGE_NVS_WRITE_FAILED = 402,
    STORAGE_SPIFFS_INIT_FAILED = 410,
    STORAGE_SPIFFS_MOUNT_FAILED = 411,
    STORAGE_SPIFFS_READ_FAILED = 412,
    STORAGE_SPIFFS_WRITE_FAILED = 413,
    STORAGE_SPIFFS_FULL = 414,
    STORAGE_FILE_NOT_FOUND = 415,
    STORAGE_FILE_OPEN_FAILED = 416,
    
    // Configuration errors (42x)
    CONFIG_LOAD_FAILED = 420,
    CONFIG_SAVE_FAILED = 421,
    CONFIG_PARSE_ERROR = 422,
    CONFIG_INVALID_VALUE = 423,
    CONFIG_MISSING_FIELD = 424,
    
    // WiFi errors (5xx)
    WIFI_INIT_FAILED = 500,
    WIFI_CONNECT_FAILED = 501,
    WIFI_DISCONNECT = 502,
    WIFI_TIMEOUT = 503,
    WIFI_CREDENTIALS_INVALID = 504,
    WIFI_AP_START_FAILED = 505,
    WIFI_MDNS_FAILED = 506,
    WIFI_SIGNAL_WEAK = 507,
    
    // Ethernet errors (51x)
    ETHERNET_INIT_FAILED = 510,
    ETHERNET_LINK_DOWN = 511,
    ETHERNET_DHCP_FAILED = 512,
    ETHERNET_CABLE_DISCONNECTED = 513,
    
    // Web server errors (52x)
    WEB_SERVER_INIT_FAILED = 520,
    WEB_SERVER_START_FAILED = 521,
    WEB_SOCKET_CONNECT_FAILED = 522,
    WEB_SOCKET_SEND_FAILED = 523,
    WEB_API_INVALID_REQUEST = 524,
    WEB_API_AUTH_FAILED = 525,
    WEB_API_RATE_LIMIT = 526,
    
    // Modbus errors (53x)
    MODBUS_RTU_INIT_FAILED = 530,
    MODBUS_RTU_TIMEOUT = 531,
    MODBUS_RTU_CRC_ERROR = 532,
    MODBUS_RTU_EXCEPTION = 533,
    MODBUS_TCP_INIT_FAILED = 540,
    MODBUS_TCP_CONNECT_FAILED = 541,
    MODBUS_TCP_TIMEOUT = 542,
    MODBUS_INVALID_REGISTER = 543,
    MODBUS_INVALID_FUNCTION = 544,
    
    // MQTT errors (54x)
    MQTT_INIT_FAILED = 550,
    MQTT_CONNECT_FAILED = 551,
    MQTT_DISCONNECT = 552,
    MQTT_PUBLISH_FAILED = 553,
    MQTT_SUBSCRIBE_FAILED = 554,
    MQTT_TIMEOUT = 555,
    
    // OTA errors (55x)
    OTA_INIT_FAILED = 560,
    OTA_UPDATE_FAILED = 561,
    OTA_VERIFY_FAILED = 562,
    OTA_INSUFFICIENT_SPACE = 563,
    OTA_ABORT = 564,
    
    // TCP Data Server errors (56x)
    TCP_SERVER_INIT_FAILED = 570,
    TCP_SERVER_BIND_FAILED = 571,
    TCP_CLIENT_CONNECT_FAILED = 572,
    TCP_SEND_FAILED = 573,
    TCP_RECEIVE_FAILED = 574,
    TCP_PROTOCOL_ERROR = 575,
    
    // Display errors (6xx)
    DISPLAY_INIT_FAILED = 600,
    DISPLAY_I2C_ERROR = 601,
    DISPLAY_NOT_FOUND = 602,
    DISPLAY_UPDATE_FAILED = 603,
    
    // LED/Button errors (61x)
    LED_INIT_FAILED = 610,
    BUTTON_INIT_FAILED = 620,
    BUTTON_DEBOUNCE_ERROR = 621,
    
    // Security errors (7xx)
    SECURITY_AUTH_FAILED = 700,
    SECURITY_SESSION_INVALID = 701,
    SECURITY_SESSION_EXPIRED = 702,
    SECURITY_PERMISSION_DENIED = 703,
    SECURITY_PASSWORD_WEAK = 704,
    SECURITY_TOKEN_INVALID = 705,
    SECURITY_CRC_MISMATCH = 706,
    SECURITY_HASH_FAILED = 707,
    
    // Data logging errors (8xx)
    DATALOG_INIT_FAILED = 800,
    DATALOG_WRITE_FAILED = 801,
    DATALOG_READ_FAILED = 802,
    DATALOG_BUFFER_FULL = 803,
    DATALOG_EXPORT_FAILED = 804,
    
    // System errors (9xx)
    SYSTEM_LOW_MEMORY = 900,
    SYSTEM_HEAP_CORRUPT = 901,
    SYSTEM_STACK_OVERFLOW = 902,
    SYSTEM_TASK_FAILED = 903,
    SYSTEM_REBOOT_REQUIRED = 904,
    SYSTEM_HARDWARE_FAULT = 905,
    
    // Unknown/generic
    UNKNOWN_ERROR = 999
};

/**
 * @enum ErrorSeverity
 * @brief Error severity levels
 */
enum class ErrorSeverity {
    INFO = 0,       ///< Informational, no action needed
    WARNING = 1,    ///< Warning, may require attention
    ERROR = 2,      ///< Error, functionality impaired
    CRITICAL = 3,   ///< Critical error, system unstable
    FATAL = 4       ///< Fatal error, system cannot continue
};

/**
 * @brief Convert error code to string
 * @param code Error code
 * @return Error code name as string
 */
inline const char* errorCodeToString(ErrorCode code) {
    switch (code) {
        case ErrorCode::SUCCESS: return "SUCCESS";
        
        // Core errors
        case ErrorCode::CORE_INIT_FAILED: return "CORE_INIT_FAILED";
        case ErrorCode::CORE_STATE_INVALID: return "CORE_STATE_INVALID";
        case ErrorCode::CORE_WATCHDOG_TIMEOUT: return "CORE_WATCHDOG_TIMEOUT";
        case ErrorCode::CORE_TASK_OVERFLOW: return "CORE_TASK_OVERFLOW";
        case ErrorCode::CORE_EVENT_QUEUE_FULL: return "CORE_EVENT_QUEUE_FULL";
        case ErrorCode::CORE_MEMORY_ALLOC_FAILED: return "CORE_MEMORY_ALLOC_FAILED";
        
        // HAL errors
        case ErrorCode::HAL_SPI_INIT_FAILED: return "HAL_SPI_INIT_FAILED";
        case ErrorCode::HAL_SPI_TRANSFER_FAILED: return "HAL_SPI_TRANSFER_FAILED";
        case ErrorCode::HAL_SPI_TIMEOUT: return "HAL_SPI_TIMEOUT";
        case ErrorCode::HAL_I2C_INIT_FAILED: return "HAL_I2C_INIT_FAILED";
        case ErrorCode::HAL_I2C_TRANSFER_FAILED: return "HAL_I2C_TRANSFER_FAILED";
        case ErrorCode::HAL_I2C_TIMEOUT: return "HAL_I2C_TIMEOUT";
        case ErrorCode::HAL_GPIO_INIT_FAILED: return "HAL_GPIO_INIT_FAILED";
        case ErrorCode::HAL_PIN_INVALID: return "HAL_PIN_INVALID";
        
        // Energy IC errors
        case ErrorCode::ENERGY_IC_NOT_FOUND: return "ENERGY_IC_NOT_FOUND";
        case ErrorCode::ENERGY_IC_INIT_FAILED: return "ENERGY_IC_INIT_FAILED";
        case ErrorCode::ENERGY_IC_COMM_ERROR: return "ENERGY_IC_COMM_ERROR";
        case ErrorCode::ENERGY_IC_CALIBRATION_ERROR: return "ENERGY_IC_CALIBRATION_ERROR";
        case ErrorCode::ENERGY_IC_CHECKSUM_ERROR: return "ENERGY_IC_CHECKSUM_ERROR";
        
        // Add more as needed...
        
        default: return "UNKNOWN_ERROR";
    }
}
