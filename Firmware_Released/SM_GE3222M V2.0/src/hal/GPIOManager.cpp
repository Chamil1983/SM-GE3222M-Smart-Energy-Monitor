/**
 * @file GPIOManager.cpp
 * @brief GPIO management implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "GPIOManager.h"
#include "I2CBus.h"

GPIOManager::GPIOManager() 
    : initialized(false) {
    // Initialize LED states
    for (int i = 0; i < 4; i++) {
        ledStates[i].pattern = LEDPattern::OFF;
        ledStates[i].currentState = false;
        ledStates[i].patternStartTime = 0;
        ledStates[i].patternDuration = 0;
        ledStates[i].lastUpdateTime = 0;
        ledStates[i].pulseValue = 0;
        ledStates[i].pulseDirection = true;
    }
    
    // Initialize button states
    for (int i = 0; i < 2; i++) {
        buttonInfo[i].state = ButtonState::RELEASED;
        buttonInfo[i].rawState = false;
        buttonInfo[i].lastChangeTime = 0;
        buttonInfo[i].pressStartTime = 0;
        buttonInfo[i].callback = nullptr;
    }
    
    // Initialize relay states
    relayStates[0] = false;
    relayStates[1] = false;
}

GPIOManager& GPIOManager::getInstance() {
    static GPIOManager instance;
    return instance;
}

bool GPIOManager::init() {
    if (initialized) {
        return true;
    }
    
    Serial.println(F("[GPIOManager] Initializing GPIO manager..."));
    
    // Initialize MCP23017
    if (!initMCP23017()) {
        Serial.println(F("[GPIOManager] ERROR: MCP23017 initialization failed"));
        return false;
    }
    
    // Configure MCP pins
    configureMCPPins();
    
    // Turn off all LEDs
    setLED(LED::WIFI, false);
    setLED(LED::RUN, false);
    setLED(LED::FAULT, false);
    setLED(LED::MODBUS, false);
    
    // Turn off all relays
    setRelay(Relay::RELAY_1, false);
    setRelay(Relay::RELAY_2, false);
    
    initialized = true;
    
    Serial.println(F("[GPIOManager] GPIO manager initialized"));
    return true;
}

bool GPIOManager::initMCP23017() {
    // Get I2C bus instance
    I2CBus& i2c = I2CBus::getInstance();
    
    // Check if I2C is initialized
    if (!i2c.isInitialized()) {
        Serial.println(F("[GPIOManager] ERROR: I2C not initialized"));
        return false;
    }
    
    // Check if MCP23017 is present
    if (!i2c.isDevicePresent(I2C_ADDR_MCP23017)) {
        Serial.println(F("[GPIOManager] ERROR: MCP23017 not found"));
        return false;
    }
    
    // Initialize MCP23017
    if (!mcp.begin_I2C(I2C_ADDR_MCP23017)) {
        Serial.println(F("[GPIOManager] ERROR: MCP23017 begin failed"));
        return false;
    }
    
    Serial.println(F("[GPIOManager] MCP23017 initialized"));
    return true;
}

void GPIOManager::configureMCPPins() {
    // Configure Port A as outputs (LEDs and Relays)
    for (uint8_t pin = 0; pin < 8; pin++) {
        mcp.pinMode(pin, OUTPUT);
        mcp.digitalWrite(pin, LED_OFF);  // All outputs high (LEDs off, relays off)
    }
    
    // Configure Port B as inputs (with pullup)
    for (uint8_t pin = 8; pin < 16; pin++) {
        mcp.pinMode(pin, INPUT_PULLUP);
    }
}

uint8_t GPIOManager::getLEDPin(LED led) const {
    switch (led) {
        case LED::WIFI:     return MCP_WIFI_LED;
        case LED::RUN:      return MCP_RUN_LED;
        case LED::FAULT:    return MCP_FLT_LED;
        case LED::MODBUS:   return MCP_MODBUS_LED;
        default:            return MCP_WIFI_LED;
    }
}

uint8_t GPIOManager::getRelayPin(Relay relay) const {
    switch (relay) {
        case Relay::RELAY_1:    return MCP_RELAY_1;
        case Relay::RELAY_2:    return MCP_RELAY_2;
        default:                return MCP_RELAY_1;
    }
}

void GPIOManager::setLED(LED led, bool state) {
    if (!initialized) {
        return;
    }
    
    uint8_t pin = getLEDPin(led);
    uint8_t index = static_cast<uint8_t>(led);
    
    // Update state
    ledStates[index].currentState = state;
    ledStates[index].pattern = state ? LEDPattern::ON : LEDPattern::OFF;
    
    // Write to MCP23017 (LEDs are active LOW)
    mcp.digitalWrite(pin, state ? LED_ON : LED_OFF);
}

void GPIOManager::setLEDPattern(LED led, LEDPattern pattern, uint32_t durationMs) {
    if (!initialized) {
        return;
    }
    
    uint8_t index = static_cast<uint8_t>(led);
    
    ledStates[index].pattern = pattern;
    ledStates[index].patternStartTime = millis();
    ledStates[index].patternDuration = durationMs;
    ledStates[index].lastUpdateTime = millis();
    ledStates[index].pulseValue = 0;
    ledStates[index].pulseDirection = true;
    
    // Immediate update
    if (pattern == LEDPattern::OFF) {
        setLED(led, false);
    } else if (pattern == LEDPattern::ON) {
        setLED(led, true);
    }
}

void GPIOManager::updateLEDs() {
    if (!initialized) {
        return;
    }
    
    for (uint8_t i = 0; i < 4; i++) {
        LED led = static_cast<LED>(i);
        updateLEDPattern(led);
    }
}

void GPIOManager::updateLEDPattern(LED led) {
    uint8_t index = static_cast<uint8_t>(led);
    LEDState& state = ledStates[index];
    uint32_t now = millis();
    
    // Check if pattern has expired
    if (state.patternDuration > 0) {
        if ((now - state.patternStartTime) >= state.patternDuration) {
            setLED(led, false);
            return;
        }
    }
    
    uint8_t pin = getLEDPin(led);
    
    switch (state.pattern) {
        case LEDPattern::OFF:
            if (state.currentState) {
                state.currentState = false;
                mcp.digitalWrite(pin, LED_OFF);
            }
            break;
            
        case LEDPattern::ON:
            if (!state.currentState) {
                state.currentState = true;
                mcp.digitalWrite(pin, LED_ON);
            }
            break;
            
        case LEDPattern::BLINK_SLOW:
            // 1Hz = 500ms on, 500ms off
            if ((now - state.lastUpdateTime) >= 500) {
                state.currentState = !state.currentState;
                mcp.digitalWrite(pin, state.currentState ? LED_ON : LED_OFF);
                state.lastUpdateTime = now;
            }
            break;
            
        case LEDPattern::BLINK_FAST:
            // 5Hz = 100ms on, 100ms off
            if ((now - state.lastUpdateTime) >= 100) {
                state.currentState = !state.currentState;
                mcp.digitalWrite(pin, state.currentState ? LED_ON : LED_OFF);
                state.lastUpdateTime = now;
            }
            break;
            
        case LEDPattern::PULSE:
            // Breathing effect - update every 10ms
            if ((now - state.lastUpdateTime) >= 10) {
                if (state.pulseDirection) {
                    state.pulseValue += 5;
                    if (state.pulseValue >= 255) {
                        state.pulseValue = 255;
                        state.pulseDirection = false;
                    }
                } else {
                    state.pulseValue -= 5;
                    if (state.pulseValue <= 0) {
                        state.pulseValue = 0;
                        state.pulseDirection = true;
                    }
                }
                
                // MCP23017 doesn't support PWM, so approximate with on/off
                // On if pulse value > 128
                bool ledOn = (state.pulseValue > 128);
                if (state.currentState != ledOn) {
                    state.currentState = ledOn;
                    mcp.digitalWrite(pin, ledOn ? LED_ON : LED_OFF);
                }
                
                state.lastUpdateTime = now;
            }
            break;
            
        case LEDPattern::HEARTBEAT:
            // Double blink pattern: on 100ms, off 100ms, on 100ms, off 700ms
            {
                uint32_t elapsed = (now - state.patternStartTime) % 1000;
                bool shouldBeOn = (elapsed < 100) || (elapsed >= 200 && elapsed < 300);
                
                if (state.currentState != shouldBeOn) {
                    state.currentState = shouldBeOn;
                    mcp.digitalWrite(pin, shouldBeOn ? LED_ON : LED_OFF);
                }
            }
            break;
    }
}

bool GPIOManager::getLEDState(LED led) const {
    if (!initialized) {
        return false;
    }
    
    uint8_t index = static_cast<uint8_t>(led);
    return ledStates[index].currentState;
}

ButtonState GPIOManager::getButtonState(Button button) {
    if (!initialized) {
        return ButtonState::RELEASED;
    }
    
    uint8_t index = static_cast<uint8_t>(button);
    return buttonInfo[index].state;
}

void GPIOManager::updateButtons() {
    if (!initialized) {
        return;
    }
    
    updateButton(Button::MODE);
    updateButton(Button::SET);
}

void GPIOManager::updateButton(Button button) {
    uint8_t index = static_cast<uint8_t>(button);
    ButtonInfo& info = buttonInfo[index];
    
    // Read raw button state from ESP32 GPIO
    bool currentRaw;
    if (button == Button::MODE) {
        currentRaw = (digitalRead(PIN_MODE_BUTTON) == BUTTON_PRESSED);
    } else {
        currentRaw = (digitalRead(PIN_SET_BUTTON) == BUTTON_PRESSED);
    }
    
    uint32_t now = millis();
    
    // Debouncing
    if (currentRaw != info.rawState) {
        info.rawState = currentRaw;
        info.lastChangeTime = now;
        return;  // Wait for debounce
    }
    
    // Check if debounce time has passed
    if ((now - info.lastChangeTime) < DEBOUNCE_TIME_MS) {
        return;
    }
    
    // State machine
    ButtonState oldState = info.state;
    
    if (currentRaw) {
        // Button is pressed
        if (info.state == ButtonState::RELEASED) {
            info.state = ButtonState::PRESSED;
            info.pressStartTime = now;
        } else if (info.state == ButtonState::PRESSED) {
            if ((now - info.pressStartTime) >= LONG_PRESS_TIME_MS) {
                info.state = ButtonState::LONG_PRESS;
            } else if ((now - info.pressStartTime) >= HOLD_TIME_MS) {
                info.state = ButtonState::HELD;
            }
        }
    } else {
        // Button is released
        info.state = ButtonState::RELEASED;
    }
    
    // Call callback if state changed
    if (info.state != oldState && info.callback != nullptr) {
        info.callback(button, info.state);
    }
}

void GPIOManager::attachButtonHandler(Button button, ButtonCallback callback) {
    if (!initialized) {
        return;
    }
    
    uint8_t index = static_cast<uint8_t>(button);
    buttonInfo[index].callback = callback;
}

void GPIOManager::detachButtonHandler(Button button) {
    if (!initialized) {
        return;
    }
    
    uint8_t index = static_cast<uint8_t>(button);
    buttonInfo[index].callback = nullptr;
}

bool GPIOManager::setRelay(Relay relay, bool state) {
    if (!initialized) {
        return false;
    }
    
    uint8_t pin = getRelayPin(relay);
    uint8_t index = static_cast<uint8_t>(relay);
    
    // Update state
    relayStates[index] = state;
    
    // Write to MCP23017 (relays active LOW)
    mcp.digitalWrite(pin, state ? LOW : HIGH);
    
    return true;
}

bool GPIOManager::getRelayState(Relay relay) const {
    if (!initialized) {
        return false;
    }
    
    uint8_t index = static_cast<uint8_t>(relay);
    return relayStates[index];
}

void GPIOManager::emergencyStop() {
    if (!initialized) {
        return;
    }
    
    Serial.println(F("[GPIOManager] EMERGENCY STOP - Disabling all relays"));
    
    setRelay(Relay::RELAY_1, false);
    setRelay(Relay::RELAY_2, false);
    
    // Set fault LED
    setLEDPattern(LED::FAULT, LEDPattern::BLINK_FAST);
}

bool GPIOManager::readMCPPin(uint8_t pin) {
    if (!initialized || pin > 15) {
        return false;
    }
    
    return mcp.digitalRead(pin);
}

void GPIOManager::writeMCPPin(uint8_t pin, bool state) {
    if (!initialized || pin > 15) {
        return;
    }
    
    mcp.digitalWrite(pin, state ? HIGH : LOW);
}
