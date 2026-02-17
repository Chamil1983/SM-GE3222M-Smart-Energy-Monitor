#pragma once

// SM-GE3222M V2.0 - Firmware Version Information

// ============================================================================
// Firmware Version
// ============================================================================
#define FW_VERSION_MAJOR    2
#define FW_VERSION_MINOR    0
#define FW_VERSION_PATCH    1

#define FW_VERSION          "2.0.1"
#define FW_BUILD_DATE       __DATE__
#define FW_BUILD_TIME       __TIME__

// ============================================================================
// Protocol Version
// ============================================================================
#define PROTOCOL_VERSION    2

// ============================================================================
// Hardware Version
// ============================================================================
#define HARDWARE_VERSION    "1.0"
#define BOARD_NAME          "SM-GE3222M"

// ============================================================================
// Manufacturer Information
// ============================================================================
#define MANUFACTURER        "Microcode Engineering"
#define PRODUCT_CODE        "GE3222M"
#define MODEL_NAME          "SM-GE3222M"

// ============================================================================
// Build Information
// ============================================================================
#ifdef DEBUG
    #define BUILD_TYPE      "Debug"
#else
    #define BUILD_TYPE      "Release"
#endif

// ============================================================================
// Feature Flags
// ============================================================================
#define FEATURE_WIFI        1
#define FEATURE_ETHERNET    0   // Optional W5500
#define FEATURE_MODBUS_RTU  1
#define FEATURE_MODBUS_TCP  0
#define FEATURE_MQTT        1
#define FEATURE_WEBSOCKET   1
#define FEATURE_OTA         1
#define FEATURE_NTP         1
#define FEATURE_SPIFFS      1

// ============================================================================
// Version String Functions
// ============================================================================
inline const char* getFullVersionString() {
    static char versionStr[64];
    snprintf(versionStr, sizeof(versionStr), 
             "%s v%s (%s %s)", 
             BOARD_NAME, FW_VERSION, FW_BUILD_DATE, FW_BUILD_TIME);
    return versionStr;
}

inline const char* getShortVersionString() {
    static char versionStr[32];
    snprintf(versionStr, sizeof(versionStr), 
             "%s v%s", 
             BOARD_NAME, FW_VERSION);
    return versionStr;
}
