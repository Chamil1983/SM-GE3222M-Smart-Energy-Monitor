/**
 * @file Version.h
 * @brief Version information for SM-GE3222M Firmware V2.0
 * @author Microcode Engineering
 * @date 2026-02-08
 */

#ifndef VERSION_H
#define VERSION_H

// Firmware version information
#define FW_VERSION_MAJOR 2
#define FW_VERSION_MINOR 0
#define FW_VERSION_PATCH 0
#define FW_VERSION "2.0.0"

// Build information
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__
#define FW_BUILD_NUMBER 1

// Product information
#define PRODUCT_NAME "SM-GE3222M Smart Energy Monitor"
#define PRODUCT_CODE "GE3222M"
#define HARDWARE_VERSION "1.1"
#define VENDOR_NAME "Microcode Engineering"

// Compatibility
#define PROTOCOL_VERSION "1.0"  // V1.0 protocol compatibility
#define API_VERSION "1.0"

// Feature flags
#define ENABLE_MQTT 1
#define ENABLE_MODBUS_TCP 1
#define ENABLE_MODBUS_RTU 1
#define ENABLE_WEB_SERVER 1
#define ENABLE_OTA 1
#define ENABLE_ETHERNET 0  // W5500 not currently used

#endif // VERSION_H
