/**
 * @file GPIOManager.h
 * @brief High-level GPIO management using MCP23017
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * LED patterns, button handling, and relay control
 */

#pragma once

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include "../../include/PinMap.h"
#include "../../include/GlobalTypes.h"

/**
 * @brief LED identifiers
 */
enum class LED {
    WIFI,       // WiFi status LED
    RUN,        // Run/Heartbeat LED
    FAULT,      // Fault indicator LED
    MODBUS      // Modbus activity LED
};

/**
 * @brief LED pattern types
 */
enum class LEDPattern {
    OFF,            // LED off
    ON,             // LED on
    BLINK_SLOW,     // 1Hz blink (500ms on/off)
    BLINK_FAST,     // 5Hz blink (100ms on/off)
    PULSE,          // Breathing effect
    HEARTBEAT       // Double blink pattern
};

/**
 * @brief Button identifiers
 */
enum class Button {
    MODE,       // Mode button
    SET         // Set button
};

/**
 * @brief Button state
 */
enum class ButtonState {
    RELEASED,
    PRESSED,
    HELD,       // Held for > 1 second
    LONG_PRESS  // Held for > 3 seconds
};

/**
 * @brief Relay identifiers
 */
enum class Relay {
    RELAY_1,
    RELAY_2
};

/**
 * @brief Button callback function type
 */
typedef void (*ButtonCallback)(Button button, ButtonState state);

/**
 * @class GPIOManager
 * @brief Singleton class for GPIO management via MCP23017
 * 
 * Provides high-level control of LEDs, buttons, and relays
 * LED pattern engine with various effects
 * Button debouncing and state machine
 * Safety interlocks for relay control
 */
class GPIOManager {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to GPIOManager instance
     */
    static GPIOManager& getInstance();
    
    /**
     * @brief Initialize GPIO manager and MCP23017
     * @return true if initialization successful
     */
    bool init();
    
    // ========== LED Control ==========
    
    /**
     * @brief Set LED state directly
     * @param led LED to control
     * @param state true = on, false = off
     */
    void setLED(LED led, bool state);
    
    /**
     * @brief Set LED pattern
     * @param led LED to control
     * @param pattern Pattern to apply
     * @param durationMs Duration in milliseconds (0 = infinite)
     */
    void setLEDPattern(LED led, LEDPattern pattern, uint32_t durationMs = 0);
    
    /**
     * @brief Update LED patterns (call from main loop)
     * Should be called frequently (every 10-20ms) for smooth patterns
     */
    void updateLEDs();
    
    /**
     * @brief Get current LED state
     * @param led LED to query
     * @return true if LED is on
     */
    bool getLEDState(LED led) const;
    
    // ========== Button Control ==========
    
    /**
     * @brief Get button state
     * @param button Button to query
     * @return Current button state
     */
    ButtonState getButtonState(Button button);
    
    /**
     * @brief Update button states (call from main loop)
     * Handles debouncing and state transitions
     */
    void updateButtons();
    
    /**
     * @brief Attach callback for button events
     * @param button Button to monitor
     * @param callback Function to call on state change
     */
    void attachButtonHandler(Button button, ButtonCallback callback);
    
    /**
     * @brief Detach button callback
     * @param button Button to stop monitoring
     */
    void detachButtonHandler(Button button);
    
    // ========== Relay Control ==========
    
    /**
     * @brief Set relay state
     * @param relay Relay to control
     * @param state true = energized, false = de-energized
     * @return true if successful
     */
    bool setRelay(Relay relay, bool state);
    
    /**
     * @brief Get relay state
     * @param relay Relay to query
     * @return true if relay is energized
     */
    bool getRelayState(Relay relay) const;
    
    /**
     * @brief Emergency relay shutdown (both relays off)
     */
    void emergencyStop();
    
    // ========== MCP23017 Direct Access ==========
    
    /**
     * @brief Read MCP23017 input pin
     * @param pin MCP pin number (0-15)
     * @return Pin state
     */
    bool readMCPPin(uint8_t pin);
    
    /**
     * @brief Write MCP23017 output pin
     * @param pin MCP pin number (0-15)
     * @param state Pin state
     */
    void writeMCPPin(uint8_t pin, bool state);
    
    /**
     * @brief Check if manager is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
private:
    GPIOManager();
    ~GPIOManager() = default;
    GPIOManager(const GPIOManager&) = delete;
    GPIOManager& operator=(const GPIOManager&) = delete;
    
    bool initMCP23017();
    void configureMCPPins();
    
    uint8_t getLEDPin(LED led) const;
    uint8_t getRelayPin(Relay relay) const;
    
    void updateLEDPattern(LED led);
    void updateButton(Button button);
    
    bool initialized;
    Adafruit_MCP23X17 mcp;
    
    // LED state tracking
    struct LEDState {
        LEDPattern pattern;
        bool currentState;
        uint32_t patternStartTime;
        uint32_t patternDuration;
        uint32_t lastUpdateTime;
        uint8_t pulseValue;  // For pulse effect (0-255)
        bool pulseDirection; // true = increasing, false = decreasing
    };
    LEDState ledStates[4];  // WIFI, RUN, FAULT, MODBUS
    
    // Button state tracking
    struct ButtonInfo {
        ButtonState state;
        bool rawState;
        uint32_t lastChangeTime;
        uint32_t pressStartTime;
        ButtonCallback callback;
    };
    ButtonInfo buttonInfo[2];  // MODE, SET
    
    // Relay state tracking
    bool relayStates[2];  // RELAY_1, RELAY_2
    
    static const uint32_t DEBOUNCE_TIME_MS = BUTTON_DEBOUNCE_MS;
    static const uint32_t HOLD_TIME_MS = 1000;
    static const uint32_t LONG_PRESS_TIME_MS = 3000;
};
