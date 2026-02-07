/**
 * @file ButtonHandler.h
 * @brief Physical button handling with debouncing and press detection
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Handles MODE and SET buttons for display navigation and system functions
 */

#pragma once

#include <Arduino.h>
#include <functional>
#include "../../include/PinMap.h"
#include "../../include/GlobalTypes.h"

/**
 * @class ButtonHandler
 * @brief Button input manager with debouncing and press detection
 * 
 * Features:
 * - Debounced button reading (50ms)
 * - Short press detection
 * - Long press detection (configurable)
 * - Factory reset on 10s hold
 * - ISR-safe operation
 * - Callback support for button events
 */
class ButtonHandler {
public:
    /**
     * @brief Button identifiers
     */
    enum class Button {
        MODE,       // Mode button (page change)
        SET         // Set button (future use)
    };

    /**
     * @brief Button event types
     */
    enum class Event {
        SHORT_PRESS,    // Short press (< 1s)
        LONG_PRESS,     // Long press (1-10s)
        FACTORY_RESET   // Very long press (>10s)
    };

    /**
     * @brief Button event callback signature
     */
    using ButtonCallback = std::function<void(Button, Event)>;

    /**
     * @brief Get singleton instance
     * @return Reference to ButtonHandler instance
     */
    static ButtonHandler& getInstance();

    /**
     * @brief Initialize button handler
     * @return true if successful, false on failure
     */
    bool init();

    /**
     * @brief Update button states (call from main loop)
     * 
     * Handles debouncing and press detection
     */
    void update();

    /**
     * @brief Register callback for button events
     * @param callback Callback function
     */
    void setCallback(ButtonCallback callback);

    /**
     * @brief Enable/disable buttons
     * @param enabled True to enable
     */
    void setEnabled(bool enabled) { _enabled = enabled; }

    /**
     * @brief Check if buttons are enabled
     * @return true if enabled
     */
    bool isEnabled() const { return _enabled; }

    /**
     * @brief Set long press threshold
     * @param thresholdMs Threshold in milliseconds
     */
    void setLongPressThreshold(unsigned long thresholdMs) { 
        _longPressThreshold = thresholdMs; 
    }

    /**
     * @brief Set factory reset threshold
     * @param thresholdMs Threshold in milliseconds
     */
    void setFactoryResetThreshold(unsigned long thresholdMs) { 
        _factoryResetThreshold = thresholdMs; 
    }

    /**
     * @brief Get button state (for testing/debugging)
     * @param button Button identifier
     * @return true if pressed
     */
    bool isPressed(Button button) const;

private:
    ButtonHandler();
    ~ButtonHandler() = default;
    ButtonHandler(const ButtonHandler&) = delete;
    ButtonHandler& operator=(const ButtonHandler&) = delete;

    /**
     * @brief Button state structure
     */
    struct ButtonState {
        uint8_t pin;
        bool currentState;
        bool lastState;
        bool debounced;
        unsigned long lastChangeTime;
        unsigned long pressStartTime;
        bool longPressTriggered;
        bool factoryResetTriggered;
        
        ButtonState(uint8_t p) 
            : pin(p)
            , currentState(BUTTON_RELEASED)
            , lastState(BUTTON_RELEASED)
            , debounced(false)
            , lastChangeTime(0)
            , pressStartTime(0)
            , longPressTriggered(false)
            , factoryResetTriggered(false) {}
    };

    /**
     * @brief Read and debounce button state
     * @param state Button state structure
     * @return true if state changed
     */
    bool readButton(ButtonState& state);

    /**
     * @brief Process button press
     * @param button Button identifier
     * @param state Button state structure
     */
    void processPress(Button button, ButtonState& state);

    /**
     * @brief Process button release
     * @param button Button identifier
     * @param state Button state structure
     */
    void processRelease(Button button, ButtonState& state);

    /**
     * @brief Invoke callback if set
     * @param button Button identifier
     * @param event Event type
     */
    void invokeCallback(Button button, Event event);

    /**
     * @brief ISR for MODE button
     */
    static void IRAM_ATTR modeButtonISR();

    /**
     * @brief ISR for SET button
     */
    static void IRAM_ATTR setButtonISR();

    bool _initialized;
    bool _enabled;
    
    ButtonState _modeButton;
    ButtonState _setButton;
    
    ButtonCallback _callback;
    
    unsigned long _debounceDelay;
    unsigned long _longPressThreshold;
    unsigned long _factoryResetThreshold;
    
    volatile bool _modeButtonInterrupt;
    volatile bool _setButtonInterrupt;
    
    static const unsigned long DEFAULT_DEBOUNCE_MS = 50;
    static const unsigned long DEFAULT_LONG_PRESS_MS = 1000;
    static const unsigned long DEFAULT_FACTORY_RESET_MS = 10000;
};
