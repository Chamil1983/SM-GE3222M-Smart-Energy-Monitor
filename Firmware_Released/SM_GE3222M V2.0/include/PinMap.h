/**
 * @file PinMap.h
 * @brief GPIO pin assignments for SM-GE3222M hardware
 * @author Microcode Engineering
 * @date 2026-02-08
 * 
 * Pin mappings ported from V1.0 Definitions.h
 * Hardware: ESP32 + ATM90E36 + MCP23017
 */

#ifndef PINMAP_H
#define PINMAP_H

#include <Arduino.h>

// ==================== ESP32 GPIO Pins ====================

// SPI Bus (ATM90E36 Energy IC)
#define PIN_SPI_SCK      18  ///< SPI Clock
#define PIN_SPI_MISO     19  ///< SPI Master In Slave Out
#define PIN_SPI_MOSI     23  ///< SPI Master Out Slave In
#define PIN_SPI_CS_ATM   5   ///< SPI Chip Select for ATM90E36

// I2C Bus (MCP23017, LCD, EEPROM)
#define PIN_I2C_SDA      21  ///< I2C Data
#define PIN_I2C_SCL      22  ///< I2C Clock

// User Input Buttons
#define PIN_USER_BTN_SET  33  ///< Set Button
#define PIN_USER_BTN_MODE 32  ///< Mode Button

// MCP23017 Interrupts
#define PIN_MCP_INTA     25  ///< MCP23017 Port A Interrupt
#define PIN_MCP_INTB     26  ///< MCP23017 Port B Interrupt

// Analog Inputs
#define PIN_ADC_DCVIN    36  ///< DC Voltage Input (ADC1_CH0)

// DHT Sensor
#define PIN_DHT          4   ///< DHT22 Temperature/Humidity Sensor

// ==================== MCP23017 I/O Expander ====================
// I2C Address: 0x20

// Port A Outputs (GPA0-GPA5)
#define MCP_GPA_RELAY_2    0  ///< Relay 2 Control
#define MCP_GPA_RELAY_1    1  ///< Relay 1 Control
#define MCP_GPA_LED_WIFI   2  ///< WiFi Status LED
#define MCP_GPA_LED_RUN    3  ///< Run Status LED (Green)
#define MCP_GPA_LED_FLT    4  ///< Fault LED (Red)
#define MCP_GPA_LED_MODBUS 5  ///< Modbus Status LED (Blue)

// Port B Inputs/Outputs (GPB0-GPB6)
#define MCP_GPB_IRQ1       0  ///< Interrupt Request 1
#define MCP_GPB_IRQ0       1  ///< Interrupt Request 0
#define MCP_GPB_ATM_WO     2  ///< ATM90E36 Watchdog Output
#define MCP_GPB_CF4        3  ///< Pulse Output 4
#define MCP_GPB_CF3        4  ///< Pulse Output 3
#define MCP_GPB_CF2        5  ///< Pulse Output 2
#define MCP_GPB_CF1        6  ///< Pulse Output 1

// ==================== I2C Device Addresses ====================
#define I2C_ADDR_MCP23017  0x20  ///< MCP23017 I/O Expander
#define I2C_ADDR_LCD       0x27  ///< LCD Display (20x4)
#define I2C_ADDR_EEPROM    0x50  ///< External EEPROM AT24C64

// ==================== LED Definitions ====================
// Using MCP23017 Port A bits
enum LED_ID {
    LED_WIFI = MCP_GPA_LED_WIFI,     ///< WiFi status
    LED_RUN = MCP_GPA_LED_RUN,       ///< Running status (Green)
    LED_FAULT = MCP_GPA_LED_FLT,     ///< Fault status (Red)
    LED_MODBUS = MCP_GPA_LED_MODBUS  ///< Modbus status (Blue)
};

// ==================== Relay Definitions ====================
enum RELAY_ID {
    RELAY_1 = MCP_GPA_RELAY_1,  ///< Relay 1
    RELAY_2 = MCP_GPA_RELAY_2   ///< Relay 2
};

#endif // PINMAP_H
