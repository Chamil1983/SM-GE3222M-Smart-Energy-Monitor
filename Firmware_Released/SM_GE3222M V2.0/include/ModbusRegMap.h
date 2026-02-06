/**
 * @file ModbusRegMap.h
 * @brief Modbus register address map
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Modbus RTU and TCP register addresses for all meter parameters
 */

#pragma once

#include <Arduino.h>

// ============================================================
// Device Identification (Read Only - Input Registers)
// ============================================================

#define MODBUS_VENDOR_NAME          0x0000  // Vendor name (10 registers)
#define MODBUS_PRODUCT_CODE         0x001E  // Product code (10 registers)
#define MODBUS_REVISION_NUMBER      0x002D  // Revision number (5 registers)
#define MODBUS_METER_NAME           0x0082  // Meter name string
#define MODBUS_MODEL                0x0096  // Model string
#define MODBUS_MANUFACTURER         0x00AA  // Manufacturer string
#define MODBUS_SERIAL_NO            0x00C8  // Serial number string
#define MODBUS_HARDWARE_REV         0x00E6  // Hardware revision string
#define MODBUS_FIRMWARE_VER         0x00EE  // Firmware version string
#define MODBUS_MAC_ADDRESS          0x0102  // MAC address string
#define MODBUS_NODE_NAME            0x0120  // Node name string
#define MODBUS_NODE_ID              0x013E  // Node ID

#define MODBUS_METER_DATETIME       240     // Meter date/time (40 registers)

// ============================================================
// Energy Pulse Configuration (Holding Registers)
// ============================================================

#define MODBUS_ENERGY_PULSE_DURATION      0x0850  // Energy pulse duration
#define MODBUS_ACTIVE_E_PULSE_ENABLE      0x0852  // Active energy pulse output enable
#define MODBUS_REACTIVE_E_PULSE_ENABLE    0x0854  // Reactive energy pulse output enable

// ============================================================
// Communications Configuration (Holding Registers)
// ============================================================

#define MODBUS_PROTOCOL             0x1901  // Protocol type
#define MODBUS_PROTOCOL_ADDRESS     0x1902  // Protocol address
#define MODBUS_BAUD_RATE            0x1903  // Baud rate
#define MODBUS_PARITY               0x1904  // Parity setting

// ============================================================
// Digital Inputs Configuration (Holding Registers)
// ============================================================

#define MODBUS_DIGITAL_INPUT_1_MODE     0x1C69  // Digital input 1 control mode
#define MODBUS_DIGITAL_INPUT_2_MODE     0x1C81  // Digital input 2 control mode
#define MODBUS_DIGITAL_INPUT_STATUS     0x22C8  // Digital input status

// ============================================================
// Energy Registers (Input Registers - Float, 2 registers each)
// ============================================================

// Active Energy
#define MODBUS_TOTAL_FWD_ACTIVE_ENERGY      0    // Total forward active energy (kWh)
#define MODBUS_PHAA_FWD_ACTIVE_ENERGY       2    // Phase A forward active energy
#define MODBUS_PHAB_FWD_ACTIVE_ENERGY       4    // Phase B forward active energy
#define MODBUS_PHAC_FWD_ACTIVE_ENERGY       6    // Phase C forward active energy
#define MODBUS_TOTAL_REV_ACTIVE_ENERGY      8    // Total reverse active energy
#define MODBUS_PHAA_REV_ACTIVE_ENERGY       10   // Phase A reverse active energy
#define MODBUS_PHAB_REV_ACTIVE_ENERGY       12   // Phase B reverse active energy
#define MODBUS_PHAC_REV_ACTIVE_ENERGY       14   // Phase C reverse active energy

// Reactive Energy
#define MODBUS_TOTAL_FWD_REACTIVE_ENERGY    16   // Total forward reactive energy (kVARh)
#define MODBUS_PHAA_FWD_REACTIVE_ENERGY     18   // Phase A forward reactive energy
#define MODBUS_PHAB_FWD_REACTIVE_ENERGY     20   // Phase B forward reactive energy
#define MODBUS_PHAC_FWD_REACTIVE_ENERGY     22   // Phase C forward reactive energy
#define MODBUS_TOTAL_REV_REACTIVE_ENERGY    24   // Total reverse reactive energy
#define MODBUS_PHAA_REV_REACTIVE_ENERGY     26   // Phase A reverse reactive energy
#define MODBUS_PHAB_REV_REACTIVE_ENERGY     28   // Phase B reverse reactive energy
#define MODBUS_PHAC_REV_REACTIVE_ENERGY     30   // Phase C reverse reactive energy

// Apparent Energy
#define MODBUS_TOTAL_APPARENT_ENERGY_ARITH  32   // Total apparent energy (arithmetic sum)
#define MODBUS_PHAA_APPARENT_ENERGY         34   // Phase A apparent energy
#define MODBUS_PHAB_APPARENT_ENERGY         36   // Phase B apparent energy
#define MODBUS_PHAC_APPARENT_ENERGY         38   // Phase C apparent energy
#define MODBUS_TOTAL_APPARENT_ENERGY_VECT   40   // Total apparent energy (vector sum)
#define MODBUS_VECTOR_APPARENT_POWER        42   // Vector sum total apparent power

// Energy Status
#define MODBUS_METERING_STATUS_0            50   // Metering status 0
#define MODBUS_METERING_STATUS_1            52   // Metering status 1

// ============================================================
// Power Registers (Input Registers - Float, 2 registers each)
// ============================================================

// Active Power
#define MODBUS_TOTAL_ACTIVE_POWER           60   // Total active power (W)
#define MODBUS_PHAA_ACTIVE_POWER            62   // Phase A active power
#define MODBUS_PHAB_ACTIVE_POWER            64   // Phase B active power
#define MODBUS_PHAC_ACTIVE_POWER            66   // Phase C active power

// Reactive Power
#define MODBUS_TOTAL_REACTIVE_POWER         68   // Total reactive power (VAR)
#define MODBUS_PHAA_REACTIVE_POWER          70   // Phase A reactive power
#define MODBUS_PHAB_REACTIVE_POWER          72   // Phase B reactive power
#define MODBUS_PHAC_REACTIVE_POWER          74   // Phase C reactive power

// Apparent Power
#define MODBUS_TOTAL_APPARENT_POWER         76   // Total apparent power (arithmetic sum)
#define MODBUS_PHAA_APPARENT_POWER          78   // Phase A apparent power
#define MODBUS_PHAB_APPARENT_POWER          80   // Phase B apparent power
#define MODBUS_PHAC_APPARENT_POWER          82   // Phase C apparent power

// Power Factor
#define MODBUS_TOTAL_POWER_FACTOR           84   // Total power factor
#define MODBUS_PHAA_POWER_FACTOR            86   // Phase A power factor
#define MODBUS_PHAB_POWER_FACTOR            88   // Phase B power factor
#define MODBUS_PHAC_POWER_FACTOR            90   // Phase C power factor

// ============================================================
// Fundamental/Harmonic Power Registers (Float, 2 registers each)
// ============================================================

#define MODBUS_TOTAL_ACTIVE_FUND_POWER      100  // Total active fundamental power
#define MODBUS_PHAA_ACTIVE_FUND_POWER       102  // Phase A active fundamental power
#define MODBUS_PHAB_ACTIVE_FUND_POWER       104  // Phase B active fundamental power
#define MODBUS_PHAC_ACTIVE_FUND_POWER       106  // Phase C active fundamental power
#define MODBUS_TOTAL_ACTIVE_HARM_POWER      108  // Total active harmonic power
#define MODBUS_PHAA_ACTIVE_HARM_POWER       110  // Phase A active harmonic power
#define MODBUS_PHAB_ACTIVE_HARM_POWER       112  // Phase B active harmonic power
#define MODBUS_PHAC_ACTIVE_HARM_POWER       114  // Phase C active harmonic power

// ============================================================
// RMS Voltage and Current Registers (Float, 2 registers each)
// ============================================================

#define MODBUS_N_LINE_SAMPLED_CURRENT       116  // N line sampled current RMS
#define MODBUS_PHAA_VOLTAGE_RMS             118  // Phase A voltage RMS (V)
#define MODBUS_PHAB_VOLTAGE_RMS             120  // Phase B voltage RMS
#define MODBUS_PHAC_VOLTAGE_RMS             122  // Phase C voltage RMS
#define MODBUS_N_LINE_CALC_CURRENT          124  // N line calculated current RMS
#define MODBUS_PHAA_CURRENT_RMS             126  // Phase A current RMS (A)
#define MODBUS_PHAB_CURRENT_RMS             128  // Phase B current RMS
#define MODBUS_PHAC_CURRENT_RMS             130  // Phase C current RMS

// ============================================================
// THD+N Registers (Float, 2 registers each)
// ============================================================

#define MODBUS_PHAA_VOLTAGE_THDN            132  // Phase A voltage THD+N (%)
#define MODBUS_PHAB_VOLTAGE_THDN            134  // Phase B voltage THD+N
#define MODBUS_PHAC_VOLTAGE_THDN            136  // Phase C voltage THD+N
#define MODBUS_PHAA_CURRENT_THDN            138  // Phase A current THD+N
#define MODBUS_PHAB_CURRENT_THDN            140  // Phase B current THD+N
#define MODBUS_PHAC_CURRENT_THDN            142  // Phase C current THD+N

// ============================================================
// Frequency and Phase Angle Registers (Float, 2 registers each)
// ============================================================

#define MODBUS_FREQUENCY                    144  // Line frequency (Hz)
#define MODBUS_PHAA_MEAN_PHASE_ANGLE        146  // Phase A mean phase angle (degrees)
#define MODBUS_PHAB_MEAN_PHASE_ANGLE        148  // Phase B mean phase angle
#define MODBUS_PHAC_MEAN_PHASE_ANGLE        150  // Phase C mean phase angle
#define MODBUS_MEASURED_TEMPERATURE         152  // Chip temperature (°C)
#define MODBUS_PHAA_VOLTAGE_PHASE_ANGLE     154  // Phase A voltage phase angle
#define MODBUS_PHAB_VOLTAGE_PHASE_ANGLE     156  // Phase B voltage phase angle
#define MODBUS_PHAC_VOLTAGE_PHASE_ANGLE     158  // Phase C voltage phase angle

// ============================================================
// Environmental Sensors (Float, 2 registers each)
// ============================================================

#define MODBUS_DHT_HUMIDITY                 180  // DHT22 humidity (%)
#define MODBUS_DHT_TEMPERATURE_F            184  // DHT22 temperature (°F)
#define MODBUS_DHT_TEMPERATURE_FAF          188  // DHT22 feels-like temperature (°F)
#define MODBUS_DHT_HIC                      192  // DHT22 heat index (°C)
#define MODBUS_DHT_HIF                      196  // DHT22 heat index (°F)

// ============================================================
// Energy Reset Date/Time
// ============================================================

#define MODBUS_ENERGY_RESET_DATETIME        0x0CB3  // Energy reset date/time

// ============================================================
// Calibration Registers (Holding Registers)
// ============================================================

#define MODBUS_CAL_LINE_FREQ                300  // Line frequency calibration
#define MODBUS_CAL_PGA_GAIN                 302  // PGA gain calibration
#define MODBUS_CAL_VOLTAGE_GAIN_A           304  // Phase A voltage gain
#define MODBUS_CAL_VOLTAGE_GAIN_B           306  // Phase B voltage gain
#define MODBUS_CAL_VOLTAGE_GAIN_C           308  // Phase C voltage gain
#define MODBUS_CAL_CURRENT_GAIN_A           310  // Phase A current gain
#define MODBUS_CAL_CURRENT_GAIN_B           312  // Phase B current gain
#define MODBUS_CAL_CURRENT_GAIN_C           314  // Phase C current gain
#define MODBUS_CAL_CURRENT_GAIN_N           316  // Neutral current gain

// Power offsets
#define MODBUS_CAL_P_OFFSET_A               320  // Phase A active power offset
#define MODBUS_CAL_Q_OFFSET_A               322  // Phase A reactive power offset
#define MODBUS_CAL_P_OFFSET_B               324  // Phase B active power offset
#define MODBUS_CAL_Q_OFFSET_B               326  // Phase B reactive power offset
#define MODBUS_CAL_P_OFFSET_C               328  // Phase C active power offset
#define MODBUS_CAL_Q_OFFSET_C               330  // Phase C reactive power offset

// Gain and phase
#define MODBUS_CAL_GAIN_A                   340  // Phase A calibration gain
#define MODBUS_CAL_PHI_A                    342  // Phase A calibration angle
#define MODBUS_CAL_GAIN_B                   344  // Phase B calibration gain
#define MODBUS_CAL_PHI_B                    346  // Phase B calibration angle
#define MODBUS_CAL_GAIN_C                   348  // Phase C calibration gain
#define MODBUS_CAL_PHI_C                    350  // Phase C calibration angle

// Measurement offsets
#define MODBUS_CAL_U_OFFSET_A               360  // Phase A voltage offset
#define MODBUS_CAL_I_OFFSET_A               362  // Phase A current offset
#define MODBUS_CAL_U_OFFSET_B               364  // Phase B voltage offset
#define MODBUS_CAL_I_OFFSET_B               366  // Phase B current offset
#define MODBUS_CAL_U_OFFSET_C               368  // Phase C voltage offset
#define MODBUS_CAL_I_OFFSET_C               370  // Phase C current offset
#define MODBUS_CAL_I_OFFSET_N               372  // Neutral current offset

// Checksums (read-only)
#define MODBUS_CAL_CS_ZERO                  380  // Checksum 0
#define MODBUS_CAL_CS_ONE                   382  // Checksum 1
#define MODBUS_CAL_CS_TWO                   384  // Checksum 2
#define MODBUS_CAL_CS_THREE                 386  // Checksum 3

// ============================================================
// System Status Registers (Input Registers)
// ============================================================

#define MODBUS_SYS_STATUS_0                 400  // System status 0
#define MODBUS_SYS_STATUS_1                 402  // System status 1
#define MODBUS_FUNC_EN_0                    404  // Function enable 0
#define MODBUS_FUNC_EN_1                    406  // Function enable 1

// ============================================================
// Network Configuration (Holding Registers)
// ============================================================

#define MODBUS_WIFI_SSID                    500  // WiFi SSID (16 registers)
#define MODBUS_WIFI_PASSWORD                520  // WiFi password (32 registers)
#define MODBUS_WIFI_IP                      560  // WiFi IP address (8 registers)
#define MODBUS_MQTT_SERVER                  580  // MQTT server (16 registers)
#define MODBUS_MQTT_PORT                    600  // MQTT port
#define MODBUS_MQTT_USER                    602  // MQTT username (16 registers)
#define MODBUS_MQTT_PASSWORD                620  // MQTT password (32 registers)

// ============================================================
// Control Registers (Coils - Read/Write)
// ============================================================

#define MODBUS_COIL_SOFT_RESET              0x0100  // Software reset
#define MODBUS_COIL_LED_0                   0x0200  // LED 0
#define MODBUS_COIL_LED_1                   0x0201  // LED 1
#define MODBUS_COIL_LED_2                   0x0202  // LED 2
#define MODBUS_COIL_LED_3                   0x0203  // LED 3
#define MODBUS_COIL_RELAY_0                 0x0220  // Relay 0
#define MODBUS_COIL_RELAY_1                 0x0221  // Relay 1
#define MODBUS_COIL_RELAY_2                 0x0222  // Relay 2
#define MODBUS_COIL_RELAY_3                 0x0223  // Relay 3
#define MODBUS_COIL_RELAY_4                 0x0224  // Relay 4
#define MODBUS_COIL_RELAY_5                 0x0225  // Relay 5
#define MODBUS_COIL_RELAY_6                 0x0226  // Relay 6
#define MODBUS_COIL_RELAY_7                 0x0227  // Relay 7

// Status coils (read-only)
#define MODBUS_COIL_CF1                     0x0300  // CF1 pulse output
#define MODBUS_COIL_CF2                     0x0301  // CF2 pulse output
#define MODBUS_COIL_ZX                      0x0302  // Zero-crossing
#define MODBUS_COIL_IRQ                     0x0303  // IRQ status
#define MODBUS_COIL_WO                      0x0304  // Warning output
#define MODBUS_COIL_WIFI_STATE              0x0305  // WiFi connection state
#define MODBUS_COIL_FAULT_STATE             0x0306  // Fault state
#define MODBUS_COIL_EEPROM_CONNECTED        0x0307  // EEPROM connected
#define MODBUS_COIL_EEPROM_BUSY             0x0308  // EEPROM busy
#define MODBUS_COIL_DHT_STATE               0x0309  // DHT sensor state

// ============================================================
// Discrete Inputs (Read-Only)
// ============================================================

#define MODBUS_DI_MODE_SWITCH               0x0400  // Mode switch state
#define MODBUS_DI_SET_SWITCH                0x0401  // Set switch state
#define MODBUS_DI_EXTERNAL_0                0x0410  // External input 0
#define MODBUS_DI_EXTERNAL_1                0x0411  // External input 1
#define MODBUS_DI_EXTERNAL_2                0x0412  // External input 2
#define MODBUS_DI_EXTERNAL_3                0x0413  // External input 3
#define MODBUS_DI_EXTERNAL_4                0x0414  // External input 4
#define MODBUS_DI_EXTERNAL_5                0x0415  // External input 5
#define MODBUS_DI_EXTERNAL_6                0x0416  // External input 6
#define MODBUS_DI_EXTERNAL_7                0x0417  // External input 7
#define MODBUS_DI_EXTERNAL_8                0x0418  // External input 8
#define MODBUS_DI_EXTERNAL_9                0x0419  // External input 9

#endif // MODBUS_REG_MAP_H
