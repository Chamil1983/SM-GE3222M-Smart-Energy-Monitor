/**
 * SM-GE3222M V2.0 - GPIO Manager Implementation
 */

#include "GPIOManager.h"
#include "I2CBus.h"
#include "Logger.h"

GPIOManager::GPIOManager() 
    : m_initialized(false)
{
    m_mutex = xSemaphoreCreateMutex();
    if (m_mutex == NULL) {
        Logger::getInstance().error("GPIOManager: Failed to create mutex");
    }

    // Initialize blink states
    for (int i = 0; i < 4; i++) {
        m_blinkStates[i].active = false;
        m_blinkStates[i].on_ms = 0;
        m_blinkStates[i].off_ms = 0;
        m_blinkStates[i].count = 0;
        m_blinkStates[i].remaining = 0;
        m_blinkStates[i].last_toggle = 0;
        m_blinkStates[i].is_on = false;
    }

    // Initialize button states
    m_buttonSet.pin = PIN_BUTTON_SET;
    m_buttonSet.last_state = HIGH;
    m_buttonSet.last_debounce_time = 0;

    m_buttonMode.pin = PIN_BUTTON_MODE;
    m_buttonMode.last_state = HIGH;
    m_buttonMode.last_debounce_time = 0;
}

GPIOManager::~GPIOManager() {
    if (m_mutex != NULL) {
        vSemaphoreDelete(m_mutex);
    }
}

bool GPIOManager::init() {
    if (m_initialized) {
        Logger::getInstance().warn("GPIOManager: Already initialized");
        return true;
    }

    // Ensure I2C bus is initialized
    I2CBus& i2c = I2CBus::getInstance();
    if (!i2c.isInitialized()) {
        if (!i2c.init()) {
            Logger::getInstance().error("GPIOManager: Failed to initialize I2C bus");
            return false;
        }
    }

    // Initialize MCP23017
    if (!m_mcp.begin_I2C(I2C_ADDR_MCP23017)) {
        Logger::getInstance().error("GPIOManager: Failed to initialize MCP23017 at 0x%02X", I2C_ADDR_MCP23017);
        return false;
    }

    setupMCP23017();
    setupButtons();
    setupLEDs();

    m_initialized = true;
    Logger::getInstance().info("GPIOManager: Initialized successfully");
    
    return true;
}

void GPIOManager::setupMCP23017() {
    // Configure Port A as outputs (LEDs and Relays)
    for (uint8_t i = 0; i <= 5; i++) {
        m_mcp.pinMode(i, OUTPUT);
        m_mcp.digitalWrite(i, LED_OFF);  // All outputs OFF initially
    }

    // Configure Port B as inputs (CF pulses, IRQs, warnings)
    for (uint8_t i = 8; i <= 15; i++) {
        m_mcp.pinMode(i, INPUT_PULLUP);
    }

    Logger::getInstance().info("GPIOManager: MCP23017 configured (PortA=outputs, PortB=inputs)");
}

void GPIOManager::setupButtons() {
    // Configure ESP32 button pins with internal pull-up
    pinMode(PIN_BUTTON_SET, INPUT_PULLUP);
    pinMode(PIN_BUTTON_MODE, INPUT_PULLUP);

    Logger::getInstance().info("GPIOManager: Buttons configured (SET=GPIO%d, MODE=GPIO%d)", 
                 PIN_BUTTON_SET, PIN_BUTTON_MODE);
}

void GPIOManager::setupLEDs() {
    // Turn off all LEDs initially
    setLED(LED::RUN, false);
    setLED(LED::FAULT, false);
    setLED(LED::WIFI, false);
    setLED(LED::MODBUS, false);

    Logger::getInstance().info("GPIOManager: LEDs initialized (all OFF)");
}

void GPIOManager::setLED(LED led, bool state) {
    if (!m_initialized) {
        return;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(10)) != pdTRUE) {
        return;
    }

    uint8_t pin = static_cast<uint8_t>(led);
    uint8_t value = state ? LED_ON : LED_OFF;  // Active-low logic
    m_mcp.digitalWrite(pin, value);

    xSemaphoreGive(m_mutex);
}

void GPIOManager::blinkLED(LED led, uint16_t on_ms, uint16_t off_ms, uint16_t count) {
    if (!m_initialized) {
        return;
    }

    // Map LED enum to blink state array index
    uint8_t index = 0;
    switch (led) {
        case LED::RUN:    index = 0; break;
        case LED::FAULT:  index = 1; break;
        case LED::WIFI:   index = 2; break;
        case LED::MODBUS: index = 3; break;
        default: return;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(10)) != pdTRUE) {
        return;
    }

    BlinkState& blink = m_blinkStates[index];
    blink.active = true;
    blink.on_ms = on_ms;
    blink.off_ms = off_ms;
    blink.count = count;
    blink.remaining = count;
    blink.last_toggle = millis();
    blink.is_on = true;

    // Turn LED on immediately
    uint8_t pin = static_cast<uint8_t>(led);
    m_mcp.digitalWrite(pin, LED_ON);

    xSemaphoreGive(m_mutex);
}

void GPIOManager::updateLEDs() {
    if (!m_initialized) {
        return;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(10)) != pdTRUE) {
        return;
    }

    unsigned long now = millis();

    // Update all active blink states
    LED leds[] = {LED::RUN, LED::FAULT, LED::WIFI, LED::MODBUS};
    
    for (int i = 0; i < 4; i++) {
        BlinkState& blink = m_blinkStates[i];
        
        if (!blink.active) {
            continue;
        }

        unsigned long elapsed = now - blink.last_toggle;
        uint16_t threshold = blink.is_on ? blink.on_ms : blink.off_ms;

        if (elapsed >= threshold) {
            blink.is_on = !blink.is_on;
            blink.last_toggle = now;

            // Toggle LED
            uint8_t pin = static_cast<uint8_t>(leds[i]);
            m_mcp.digitalWrite(pin, blink.is_on ? LED_ON : LED_OFF);

            // If transitioning from OFF to ON, decrement count
            if (blink.is_on && blink.count > 0) {
                blink.remaining--;
                if (blink.remaining == 0) {
                    blink.active = false;
                    m_mcp.digitalWrite(pin, LED_OFF);  // Ensure LED is OFF
                }
            }
        }
    }

    xSemaphoreGive(m_mutex);
}

void GPIOManager::setRelay(Relay relay, bool state) {
    if (!m_initialized) {
        return;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(10)) != pdTRUE) {
        return;
    }

    uint8_t pin = static_cast<uint8_t>(relay);
    uint8_t value = state ? LED_ON : LED_OFF;  // Active-low logic
    m_mcp.digitalWrite(pin, value);

    xSemaphoreGive(m_mutex);

    Logger::getInstance().debug("GPIOManager: Relay %d set to %s", 
                  pin, state ? "ON" : "OFF");
}

bool GPIOManager::isButtonPressed(uint8_t pin) {
    ButtonState* button = nullptr;
    
    if (pin == PIN_BUTTON_SET) {
        button = &m_buttonSet;
    } else if (pin == PIN_BUTTON_MODE) {
        button = &m_buttonMode;
    } else {
        return false;
    }

    bool current_state = digitalRead(pin);
    unsigned long now = millis();

    // Check if state has changed
    if (current_state != button->last_state) {
        button->last_debounce_time = now;
    }

    // Check if debounce time has passed
    if ((now - button->last_debounce_time) > BUTTON_DEBOUNCE_MS) {
        button->last_state = current_state;
        // Button is active-low (pressed = LOW)
        return (current_state == LOW);
    }

    return false;
}

bool GPIOManager::readPortB(uint8_t pin) {
    if (!m_initialized || pin > 7) {
        return false;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(10)) != pdTRUE) {
        return false;
    }

    bool state = m_mcp.digitalRead(pin + 8);  // Port B starts at pin 8
    
    xSemaphoreGive(m_mutex);
    
    return state;
}
