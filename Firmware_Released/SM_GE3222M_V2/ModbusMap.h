#pragma once

// SM-GE3222M V2.0 - Modbus Register Map
// Modbus RTU/TCP register layout for energy meter data

#include <cstdint>

// ============================================================================
// MODBUS CONFIGURATION
// ============================================================================
constexpr uint8_t  MODBUS_DEFAULT_SLAVE_ID  = 1;
constexpr uint32_t MODBUS_DEFAULT_BAUDRATE  = 9600;
constexpr uint16_t MODBUS_TCP_PORT          = 502;

// ============================================================================
// REGISTER BLOCKS
// ============================================================================

// Input Registers (Read-Only, Function Code 0x04)
// IEEE754 float values stored as 2 consecutive registers (32-bit)

// ============================================================================
// VOLTAGE RMS (Input Registers 0-5, 6 registers)
// ============================================================================
constexpr uint16_t MB_URMS_A            = 0;     // Phase A Voltage RMS (float)
constexpr uint16_t MB_URMS_B            = 2;     // Phase B Voltage RMS (float)
constexpr uint16_t MB_URMS_C            = 4;     // Phase C Voltage RMS (float)

// ============================================================================
// CURRENT RMS (Input Registers 6-11, 6 registers)
// ============================================================================
constexpr uint16_t MB_IRMS_A            = 6;     // Phase A Current RMS (float)
constexpr uint16_t MB_IRMS_B            = 8;     // Phase B Current RMS (float)
constexpr uint16_t MB_IRMS_C            = 10;    // Phase C Current RMS (float)

// ============================================================================
// ACTIVE POWER (Input Registers 12-23, 12 registers)
// ============================================================================
constexpr uint16_t MB_ACTIVE_POWER_A    = 12;    // Phase A Active Power (float)
constexpr uint16_t MB_ACTIVE_POWER_B    = 14;    // Phase B Active Power (float)
constexpr uint16_t MB_ACTIVE_POWER_C    = 16;    // Phase C Active Power (float)
constexpr uint16_t MB_ACTIVE_POWER_T    = 18;    // Total Active Power (float)

// ============================================================================
// REACTIVE POWER (Input Registers 20-27, 8 registers)
// ============================================================================
constexpr uint16_t MB_REACTIVE_POWER_A  = 20;    // Phase A Reactive Power (float)
constexpr uint16_t MB_REACTIVE_POWER_B  = 22;    // Phase B Reactive Power (float)
constexpr uint16_t MB_REACTIVE_POWER_C  = 24;    // Phase C Reactive Power (float)
constexpr uint16_t MB_REACTIVE_POWER_T  = 26;    // Total Reactive Power (float)

// ============================================================================
// APPARENT POWER (Input Registers 28-35, 8 registers)
// ============================================================================
constexpr uint16_t MB_APPARENT_POWER_A  = 28;    // Phase A Apparent Power (float)
constexpr uint16_t MB_APPARENT_POWER_B  = 30;    // Phase B Apparent Power (float)
constexpr uint16_t MB_APPARENT_POWER_C  = 32;    // Phase C Apparent Power (float)
constexpr uint16_t MB_APPARENT_POWER_T  = 34;    // Total Apparent Power (float)

// ============================================================================
// POWER FACTOR (Input Registers 36-43, 8 registers)
// ============================================================================
constexpr uint16_t MB_POWER_FACTOR_A    = 36;    // Phase A Power Factor (float)
constexpr uint16_t MB_POWER_FACTOR_B    = 38;    // Phase B Power Factor (float)
constexpr uint16_t MB_POWER_FACTOR_C    = 40;    // Phase C Power Factor (float)
constexpr uint16_t MB_POWER_FACTOR_T    = 42;    // Total Power Factor (float)

// ============================================================================
// PHASE ANGLES (Input Registers 44-51, 8 registers)
// ============================================================================
constexpr uint16_t MB_PHASE_ANGLE_A     = 44;    // Phase A Mean Phase Angle (float)
constexpr uint16_t MB_PHASE_ANGLE_B     = 46;    // Phase B Mean Phase Angle (float)
constexpr uint16_t MB_PHASE_ANGLE_C     = 48;    // Phase C Mean Phase Angle (float)

// ============================================================================
// VOLTAGE THD (Input Registers 52-57, 6 registers)
// ============================================================================
constexpr uint16_t MB_VOLTAGE_THD_A     = 52;    // Phase A Voltage THD (float)
constexpr uint16_t MB_VOLTAGE_THD_B     = 54;    // Phase B Voltage THD (float)
constexpr uint16_t MB_VOLTAGE_THD_C     = 56;    // Phase C Voltage THD (float)

// ============================================================================
// CURRENT THD (Input Registers 58-63, 6 registers)
// ============================================================================
constexpr uint16_t MB_CURRENT_THD_A     = 58;    // Phase A Current THD (float)
constexpr uint16_t MB_CURRENT_THD_B     = 60;    // Phase B Current THD (float)
constexpr uint16_t MB_CURRENT_THD_C     = 62;    // Phase C Current THD (float)

// ============================================================================
// FREQUENCY & NEUTRAL (Input Registers 64-67, 4 registers)
// ============================================================================
constexpr uint16_t MB_FREQUENCY         = 64;    // Line Frequency (float)
constexpr uint16_t MB_NEUTRAL_CURRENT   = 66;    // Neutral Current (float)

// ============================================================================
// FORWARD ACTIVE ENERGY (Input Registers 100-107, 8 registers)
// ============================================================================
constexpr uint16_t MB_FWD_ACTIVE_ENERGY_A = 100;  // Phase A Forward Active Energy (float)
constexpr uint16_t MB_FWD_ACTIVE_ENERGY_B = 102;  // Phase B Forward Active Energy (float)
constexpr uint16_t MB_FWD_ACTIVE_ENERGY_C = 104;  // Phase C Forward Active Energy (float)
constexpr uint16_t MB_FWD_ACTIVE_ENERGY_T = 106;  // Total Forward Active Energy (float)

// ============================================================================
// REVERSE ACTIVE ENERGY (Input Registers 108-115, 8 registers)
// ============================================================================
constexpr uint16_t MB_REV_ACTIVE_ENERGY_A = 108;  // Phase A Reverse Active Energy (float)
constexpr uint16_t MB_REV_ACTIVE_ENERGY_B = 110;  // Phase B Reverse Active Energy (float)
constexpr uint16_t MB_REV_ACTIVE_ENERGY_C = 112;  // Phase C Reverse Active Energy (float)
constexpr uint16_t MB_REV_ACTIVE_ENERGY_T = 114;  // Total Reverse Active Energy (float)

// ============================================================================
// FORWARD REACTIVE ENERGY (Input Registers 116-123, 8 registers)
// ============================================================================
constexpr uint16_t MB_FWD_REACTIVE_ENERGY_A = 116; // Phase A Forward Reactive Energy (float)
constexpr uint16_t MB_FWD_REACTIVE_ENERGY_B = 118; // Phase B Forward Reactive Energy (float)
constexpr uint16_t MB_FWD_REACTIVE_ENERGY_C = 120; // Phase C Forward Reactive Energy (float)
constexpr uint16_t MB_FWD_REACTIVE_ENERGY_T = 122; // Total Forward Reactive Energy (float)

// ============================================================================
// REVERSE REACTIVE ENERGY (Input Registers 124-131, 8 registers)
// ============================================================================
constexpr uint16_t MB_REV_REACTIVE_ENERGY_A = 124; // Phase A Reverse Reactive Energy (float)
constexpr uint16_t MB_REV_REACTIVE_ENERGY_B = 126; // Phase B Reverse Reactive Energy (float)
constexpr uint16_t MB_REV_REACTIVE_ENERGY_C = 128; // Phase C Reverse Reactive Energy (float)
constexpr uint16_t MB_REV_REACTIVE_ENERGY_T = 130; // Total Reverse Reactive Energy (float)

// ============================================================================
// APPARENT ENERGY (Input Registers 132-139, 8 registers)
// ============================================================================
constexpr uint16_t MB_APPARENT_ENERGY_A = 132;    // Phase A Apparent Energy (float)
constexpr uint16_t MB_APPARENT_ENERGY_B = 134;    // Phase B Apparent Energy (float)
constexpr uint16_t MB_APPARENT_ENERGY_C = 136;    // Phase C Apparent Energy (float)
constexpr uint16_t MB_APPARENT_ENERGY_T = 138;    // Total Apparent Energy (float)

// ============================================================================
// FUNDAMENTAL POWER (Input Registers 140-147, 8 registers)
// ============================================================================
constexpr uint16_t MB_FUNDAMENTAL_POWER_A = 140;  // Phase A Fundamental Power (float)
constexpr uint16_t MB_FUNDAMENTAL_POWER_B = 142;  // Phase B Fundamental Power (float)
constexpr uint16_t MB_FUNDAMENTAL_POWER_C = 144;  // Phase C Fundamental Power (float)
constexpr uint16_t MB_FUNDAMENTAL_POWER_T = 146;  // Total Fundamental Power (float)

// ============================================================================
// HARMONIC POWER (Input Registers 148-155, 8 registers)
// ============================================================================
constexpr uint16_t MB_HARMONIC_POWER_A  = 148;    // Phase A Harmonic Power (float)
constexpr uint16_t MB_HARMONIC_POWER_B  = 150;    // Phase B Harmonic Power (float)
constexpr uint16_t MB_HARMONIC_POWER_C  = 152;    // Phase C Harmonic Power (float)
constexpr uint16_t MB_HARMONIC_POWER_T  = 154;    // Total Harmonic Power (float)

// ============================================================================
// BOARD DATA (Input Registers 200-205, 6 registers)
// ============================================================================
constexpr uint16_t MB_BOARD_TEMP        = 200;    // PCB Temperature (float)
constexpr uint16_t MB_AMBIENT_TEMP      = 202;    // Ambient Temperature (float)
constexpr uint16_t MB_AMBIENT_HUMIDITY  = 204;    // Ambient Humidity (float)

// ============================================================================
// SYSTEM STATUS (Input Registers 300-310, 11 registers)
// ============================================================================
constexpr uint16_t MB_UPTIME_SECONDS    = 300;    // System Uptime (uint32)
constexpr uint16_t MB_ERROR_COUNT       = 302;    // Total Error Count (uint16)
constexpr uint16_t MB_SEQUENCE_NUMBER   = 303;    // Data Sequence Number (uint16)
constexpr uint16_t MB_FIRMWARE_VERSION  = 304;    // Firmware Version (uint16: major.minor)
constexpr uint16_t MB_FREE_HEAP         = 305;    // Free Heap (uint32)
constexpr uint16_t MB_STATUS_FLAGS      = 307;    // Status Flags (uint16)
constexpr uint16_t MB_METERING_STATUS   = 308;    // ATM90E36 Metering Status (uint16)

// ============================================================================
// HOLDING REGISTERS (Read/Write, Function Code 0x03/0x06/0x10)
// ============================================================================

// System Control (Holding Registers 0-9)
constexpr uint16_t MB_HOLD_RESET_ENERGY = 0;      // Write 1 to reset energy accumulators
constexpr uint16_t MB_HOLD_REBOOT       = 1;      // Write 0x5A5A to reboot system
constexpr uint16_t MB_HOLD_PUBLISH_INTERVAL = 2;  // Data publish interval (seconds)
constexpr uint16_t MB_HOLD_MODBUS_BAUD  = 3;      // Modbus RTU baudrate
constexpr uint16_t MB_HOLD_MODBUS_SLAVEID = 4;    // Modbus slave ID

// ============================================================================
// COILS (Discrete Outputs, Function Code 0x01/0x05/0x0F)
// ============================================================================
constexpr uint16_t MB_COIL_RELAY_1      = 0;      // Relay 1 State
constexpr uint16_t MB_COIL_RELAY_2      = 1;      // Relay 2 State
constexpr uint16_t MB_COIL_LED_RUN      = 2;      // Run LED State
constexpr uint16_t MB_COIL_LED_FAULT    = 3;      // Fault LED State
constexpr uint16_t MB_COIL_LED_WIFI     = 4;      // WiFi LED State
constexpr uint16_t MB_COIL_LED_MODBUS   = 5;      // Modbus LED State

// ============================================================================
// DISCRETE INPUTS (Read-Only, Function Code 0x02)
// ============================================================================
constexpr uint16_t MB_DI_BUTTON_SET     = 0;      // Set Button State
constexpr uint16_t MB_DI_BUTTON_MODE    = 1;      // Mode Button State
constexpr uint16_t MB_DI_WIFI_CONNECTED = 2;      // WiFi Connection Status
constexpr uint16_t MB_DI_MQTT_CONNECTED = 3;      // MQTT Connection Status
constexpr uint16_t MB_DI_ATM_ERROR      = 4;      // ATM90E36 Error Status

// ============================================================================
// STATUS FLAGS BIT DEFINITIONS (MB_STATUS_FLAGS)
// ============================================================================
constexpr uint16_t STATUS_FLAG_WIFI_CONNECTED   = 0x0001;
constexpr uint16_t STATUS_FLAG_MQTT_CONNECTED   = 0x0002;
constexpr uint16_t STATUS_FLAG_ETH_CONNECTED    = 0x0004;
constexpr uint16_t STATUS_FLAG_MODBUS_ACTIVE    = 0x0008;
constexpr uint16_t STATUS_FLAG_ATM_ERROR        = 0x0010;
constexpr uint16_t STATUS_FLAG_CALIBRATING      = 0x0020;
constexpr uint16_t STATUS_FLAG_LOGGING          = 0x0040;
constexpr uint16_t STATUS_FLAG_OTA_ACTIVE       = 0x0080;

// ============================================================================
// HELPER MACROS
// ============================================================================

// Calculate number of registers needed for a float (2 registers)
#define MB_FLOAT_REGS   2

// Calculate total input register count
constexpr uint16_t MB_INPUT_REG_COUNT   = 400;

// Calculate total holding register count
constexpr uint16_t MB_HOLDING_REG_COUNT = 100;

// Calculate total coil count
constexpr uint16_t MB_COIL_COUNT        = 10;

// Calculate total discrete input count
constexpr uint16_t MB_DISCRETE_INPUT_COUNT = 10;
