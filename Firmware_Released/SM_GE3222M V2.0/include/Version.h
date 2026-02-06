/**
 * @file Version.h
 * @brief Firmware version and build information
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Production-grade firmware
 */

#pragma once

// Firmware Version
#define FW_VERSION_MAJOR 2
#define FW_VERSION_MINOR 0
#define FW_VERSION_PATCH 0
#define FW_VERSION_STRING "2.0.0"

// Build Information
#define FW_BUILD_DATE __DATE__
#define FW_BUILD_TIME __TIME__

// Hardware Information
#define HW_MODEL "SM-GE3222M"
#define HW_REVISION "V1.1"

// Application Information
#define APP_NAME "SM-GE3222M ATM90E36 IoT Power Energy Meter"
#define APP_ACRONYM "SMENG"
#define MANUFACTURER "Microcode Engineering"

// Compatibility
#define V1_COMPAT_MODE false  // Set to true to enable V1.0 compatibility mode

#endif // VERSION_H
