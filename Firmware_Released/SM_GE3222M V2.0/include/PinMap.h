#pragma once

// SM-GE3222M V2.0 - GPIO Pin Mapping
// Hardware abstraction for all pin definitions

// ============================================================================
// SPI Interface - ATM90E36 Energy IC
// ============================================================================
constexpr uint8_t PIN_SPI_CS_ATM90E36   = 5;    // Chip Select for ATM90E36
constexpr uint8_t PIN_SPI_MISO          = 19;   // SPI Master In Slave Out
constexpr uint8_t PIN_SPI_MOSI          = 23;   // SPI Master Out Slave In
constexpr uint8_t PIN_SPI_SCK           = 18;   // SPI Clock

// SPI Settings
constexpr uint32_t SPI_FREQUENCY_ATM90E36 = 200000;  // 200 kHz
constexpr uint8_t  SPI_MODE_ATM90E36      = 3;       // SPI Mode 3
constexpr uint8_t  SPI_BIT_ORDER          = MSBFIRST;

// ============================================================================
// I2C Interface - MCP23017 I/O Expander & Sensors
// ============================================================================
constexpr uint8_t PIN_I2C_SDA           = 21;   // I2C Data
constexpr uint8_t PIN_I2C_SCL           = 22;   // I2C Clock
constexpr uint32_t I2C_FREQUENCY        = 100000; // 100 kHz

// I2C Device Addresses
constexpr uint8_t I2C_ADDR_MCP23017     = 0x20; // MCP23017 I/O Expander
constexpr uint8_t I2C_ADDR_LCD          = 0x27; // LCD Display (optional)

// ============================================================================
// MCP23017 I/O Expander - Port A (Outputs)
// ============================================================================
// LED & Relay Outputs on MCP23017 Port A
constexpr uint8_t MCP_PORTA_RELAY_2     = 0;    // GPA0 - Relay 2
constexpr uint8_t MCP_PORTA_RELAY_1     = 1;    // GPA1 - Relay 1
constexpr uint8_t MCP_PORTA_WIFI_LED    = 2;    // GPA2 - WiFi Status LED
constexpr uint8_t MCP_PORTA_RUN_LED     = 3;    // GPA3 - Running LED (Green)
constexpr uint8_t MCP_PORTA_FLT_LED     = 4;    // GPA4 - Fault LED (Red)
constexpr uint8_t MCP_PORTA_MODBUS_LED  = 5;    // GPA5 - Modbus Activity LED

// ============================================================================
// MCP23017 I/O Expander - Port B (Inputs)
// ============================================================================
// Digital Inputs on MCP23017 Port B
constexpr uint8_t MCP_PORTB_IRQ_1       = 0;    // GPB0 - IRQ 1
constexpr uint8_t MCP_PORTB_IRQ_2       = 1;    // GPB1 - IRQ 2
constexpr uint8_t MCP_PORTB_ATM_WARN    = 2;    // GPB2 - ATM90E36 Warning
constexpr uint8_t MCP_PORTB_CF1         = 3;    // GPB3 - CF1 Pulse
constexpr uint8_t MCP_PORTB_CF2         = 4;    // GPB4 - CF2 Pulse
constexpr uint8_t MCP_PORTB_CF3         = 5;    // GPB5 - CF3 Pulse
constexpr uint8_t MCP_PORTB_CF4         = 6;    // GPB6 - CF4 Pulse
constexpr uint8_t MCP_PORTB_CF_TOTAL    = 7;    // GPB7 - CF Total Pulse

// ============================================================================
// Direct ESP32 GPIO - Digital Inputs
// ============================================================================
constexpr uint8_t PIN_BUTTON_SET        = 33;   // GPIO33 - Set Button
constexpr uint8_t PIN_BUTTON_MODE       = 32;   // GPIO32 - Mode Button
constexpr uint8_t PIN_MCP_INT_A         = 25;   // GPIO25 - MCP23017 INTA
constexpr uint8_t PIN_MCP_INT_B         = 26;   // GPIO26 - MCP23017 INTB

// ============================================================================
// Direct ESP32 GPIO - Analog Inputs
// ============================================================================
constexpr uint8_t PIN_ADC_DCV_IN        = 36;   // GPIO36 (VP/ADC1_CH0) - DC Voltage Input
constexpr uint8_t PIN_ADC_CT4           = 39;   // GPIO39 (VN/ADC1_CH3) - CT4 Current Sensor (if isolated)

// ============================================================================
// Direct ESP32 GPIO - DHT Sensor
// ============================================================================
constexpr uint8_t PIN_DHT               = 4;    // GPIO4 - DHT Temperature/Humidity Sensor

// ============================================================================
// UART Interfaces
// ============================================================================
// Serial (UART0) - USB/Programming - 115200 baud
// Serial1 not used on ESP32 (internal)
// Serial2 (UART2) - Modbus RTU
constexpr uint8_t PIN_UART2_TX          = 17;   // GPIO17 - UART2 TX (Modbus)
constexpr uint8_t PIN_UART2_RX          = 16;   // GPIO16 - UART2 RX (Modbus)

// ============================================================================
// Ethernet Interface (W5500) - if used
// ============================================================================
constexpr uint8_t PIN_ETH_CS            = 15;   // GPIO15 - Ethernet CS
constexpr uint8_t PIN_ETH_INT           = 35;   // GPIO35 - Ethernet Interrupt
constexpr uint8_t PIN_ETH_RST           = 14;   // GPIO14 - Ethernet Reset

// ============================================================================
// LED Logic Levels (Active Low on MCP23017)
// ============================================================================
constexpr uint8_t LED_ON                = 0;    // Active LOW
constexpr uint8_t LED_OFF               = 1;    // Inactive HIGH

// ============================================================================
// ADC Configuration
// ============================================================================
constexpr uint16_t ADC_MAX_VALUE        = 4095; // 12-bit ADC
constexpr float    ADC_VREF             = 3.3;  // Reference voltage

// ============================================================================
// Button Debounce Settings
// ============================================================================
constexpr uint16_t BUTTON_DEBOUNCE_MS   = 50;   // 50ms debounce time

// ============================================================================
// Hardware Version
// ============================================================================
constexpr const char* HARDWARE_VERSION = "SM-GE3222M 1.1V";
