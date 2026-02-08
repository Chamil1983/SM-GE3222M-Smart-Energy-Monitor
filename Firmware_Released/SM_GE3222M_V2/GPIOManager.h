#pragma once

/**
 * SM-GE3222M V2.0 - GPIO Manager
 * 
 * Manages all GPIO operations including:
 * - MCP23017 I/O expander for LEDs, relays, and inputs
 * - ESP32 buttons with debouncing
 * - LED control with blinking support
 * 
 * Port A (Outputs): LEDs and Relays
 * Port B (Inputs): CF pulses, IRQs, and warnings
 * 
 * Features:
 * - Singleton pattern
 * - Thread-safe operations
 * - Button debouncing (50ms)
 * - LED blinking with configurable timing
 * - MCP23017 address 0x20 from PinMap.h
 */

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "PinMap.h"

// LED identifiers
enum class LED {
    RUN = MCP_PORTA_RUN_LED,
    FAULT = MCP_PORTA_FLT_LED,
    WIFI = MCP_PORTA_WIFI_LED,
    MODBUS = MCP_PORTA_MODBUS_LED
};

// Relay identifiers
enum class Relay {
    RELAY_1 = MCP_PORTA_RELAY_1,
    RELAY_2 = MCP_PORTA_RELAY_2
};

class GPIOManager {
public:
    /**
     * Get singleton instance
     */
    static GPIOManager& getInstance() {
        static GPIOManager instance;
        return instance;
    }

    /**
     * Initialize MCP23017, buttons, and LEDs
     * @return true if successful
     */
    bool init();

    /**
     * Set LED state
     * @param led LED identifier from LED enum
     * @param state true=ON, false=OFF (handles active-low logic)
     */
    void setLED(LED led, bool state);

    /**
     * Blink LED with specified timing
     * @param led LED identifier from LED enum
     * @param on_ms ON duration in milliseconds
     * @param off_ms OFF duration in milliseconds
     * @param count Number of blinks (0 = infinite)
     */
    void blinkLED(LED led, uint16_t on_ms, uint16_t off_ms, uint16_t count = 1);

    /**
     * Update LED blinking states (call from main loop)
     */
    void updateLEDs();

    /**
     * Set relay state
     * @param relay Relay identifier from Relay enum
     * @param state true=ON, false=OFF
     */
    void setRelay(Relay relay, bool state);

    /**
     * Check if button is pressed (with debouncing)
     * @param pin Button GPIO pin (PIN_BUTTON_SET or PIN_BUTTON_MODE)
     * @return true if button is pressed
     */
    bool isButtonPressed(uint8_t pin);

    /**
     * Read MCP23017 Port B input
     * @param pin Port B pin number (0-7)
     * @return Pin state
     */
    bool readPortB(uint8_t pin);

    /**
     * Check if MCP23017 is initialized
     */
    bool isInitialized() const { return m_initialized; }

private:
    // Singleton - prevent copying
    GPIOManager();
    ~GPIOManager();
    GPIOManager(const GPIOManager&) = delete;
    GPIOManager& operator=(const GPIOManager&) = delete;

    // Blink state structure
    struct BlinkState {
        bool active;
        uint16_t on_ms;
        uint16_t off_ms;
        uint16_t count;
        uint16_t remaining;
        unsigned long last_toggle;
        bool is_on;
    };

    // Button debounce structure
    struct ButtonState {
        uint8_t pin;
        bool last_state;
        unsigned long last_debounce_time;
    };

    // Helper methods
    void setupMCP23017();
    void setupButtons();
    void setupLEDs();

    // State
    bool m_initialized;
    Adafruit_MCP23X17 m_mcp;
    SemaphoreHandle_t m_mutex;
    
    // Blink states for each LED
    BlinkState m_blinkStates[4];  // RUN, FAULT, WIFI, MODBUS
    
    // Button states
    ButtonState m_buttonSet;
    ButtonState m_buttonMode;
};
