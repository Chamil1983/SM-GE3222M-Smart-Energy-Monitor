/**
 * @file PinMap.h
 * @brief Hardware pin definitions and I/O mapping
 *
 * SM-GE3222M Smart Energy Monitor V2.0
 * All GPIO, I2C, SPI, and UART pin assignments
 */

#pragma once

#include <Arduino.h>

 // ============================================================
 // ESP32 GPIO Pin Assignments
 // ============================================================

 // SPI Bus (Shared between ATM90E36 and W5500)
#define PIN_SPI_MISO        19
#define PIN_SPI_MOSI        23
#define PIN_SPI_SCK         18
#define PIN_SPI_CS_ATM      5      // ATM90E36 Chip Select
#define PIN_SPI_CS_W5500    15     // W5500 Ethernet Chip Select

// I2C Bus (Shared for LCD, MCP23017, DHT22)
#define PIN_I2C_SDA         21
#define PIN_I2C_SCL         22

// UART2 (RS-485 Modbus RTU)
#define PIN_UART2_TX        17
#define PIN_UART2_RX        16
#define PIN_RS485_DE        33     // MAX485 Driver Enable
#define PIN_RS485_RE        33     // MAX485 Receiver Enable (same pin)

// Analog Input
#define PIN_ADC_DCV_IN      36     // DC Voltage Input (GPIO36 / ADC1_CH0)

// Digital Inputs (Buttons)
#define PIN_MODE_BUTTON     32     // Mode button with ISR
#define PIN_SET_BUTTON      33     // Set button with ISR (shared with RS485_DE when not in use)

// DHT22 Temperature/Humidity Sensor
#define PIN_DHT22           4

// MCP23017 Interrupt Pins
#define PIN_MCP_INT_A       25     // MCP23017 INTA
#define PIN_MCP_INT_B       26     // MCP23017 INTB

// ============================================================
// I2C Device Addresses
// ============================================================

#define I2C_ADDR_LCD        0x27   // LCD 20x4 I2C address
#define I2C_ADDR_MCP23017   0x20   // MCP23017 I/O expander
#define I2C_ADDR_EEPROM     0x50   // External EEPROM (if used)

// ============================================================
// MCP23017 Pin Mapping (I/O Expander)
// ============================================================

// Port A Outputs (GPA0-GPA7)
#define MCP_RELAY_1         0      // GPA0 - Relay 1
#define MCP_RELAY_2         1      // GPA1 - Relay 2
#define MCP_WIFI_LED        2      // GPA2 - WiFi Status LED
#define MCP_RUN_LED         3      // GPA3 - Run/Heartbeat LED
#define MCP_FLT_LED         4      // GPA4 - Fault LED
#define MCP_MODBUS_LED      5      // GPA5 - Modbus Activity LED
#define MCP_SPARE_OUT_1     6      // GPA6 - Spare output
#define MCP_SPARE_OUT_2     7      // GPA7 - Spare output

// Port B Inputs (GPB0-GPB7)
#define MCP_IRQ0            8      // GPB0 - ATM90E36 IRQ0
#define MCP_IRQ1            9      // GPB1 - ATM90E36 IRQ1
#define MCP_ATM_WO          10     // GPB2 - ATM90E36 Warning Output
#define MCP_CF4             11     // GPB3 - CF4 Pulse Input
#define MCP_CF3             12     // GPB4 - CF3 Pulse Input
#define MCP_CF2             13     // GPB5 - CF2 Pulse Input
#define MCP_CF1             14     // GPB6 - CF1 Pulse Input
#define MCP_SPARE_IN_1      15     // GPB7 - Spare input

// ============================================================
// LED Logic Levels
// ============================================================

#define LED_ON              LOW    // MCP23017 LEDs are active LOW
#define LED_OFF             HIGH

// ============================================================
// Button Configuration
// ============================================================

#define BUTTON_PRESSED      LOW    // Buttons are active LOW (with pullup)
#define BUTTON_RELEASED     HIGH
#define BUTTON_DEBOUNCE_MS  50     // Debounce time in milliseconds

// ============================================================
// SPI Configuration
// ============================================================

#define SPI_FREQ_ATM        1000000    // 1 MHz for ATM90E36
#define SPI_FREQ_W5500      8000000    // 8 MHz for W5500 Ethernet
#define ATM_SPI_MODE        SPI_MODE3  // Clock polarity and phase

// ============================================================
// UART Configuration
// ============================================================

#define UART2_BAUD_DEFAULT  9600       // Default Modbus RTU baud rate

// ============================================================
// DHT22 Configuration
// ============================================================

#define DHT_TYPE            DHT22      // DHT 22 (AM2302)