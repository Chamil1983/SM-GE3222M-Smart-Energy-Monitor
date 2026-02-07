/**
 * @file ButtonHandler.cpp
 * @brief Physical button handling implementation
 */

#include "ButtonHandler.h"
#include "DisplayManager.h"
#include "../diagnostics/Logger.h"
#include "../storage/ConfigManager.h"
#include "../storage/NVStorage.h"
#include <ESP.h>

// Static instance pointer for ISR access
static ButtonHandler* s_instance = nullptr;

ButtonHandler& ButtonHandler::getInstance() {
    static ButtonHandler instance;
    s_instance = &instance;
    return instance;
}

ButtonHandler::ButtonHandler()
    : _initialized(false)
    , _enabled(true)
    , _modeButton(PIN_MODE_BUTTON)
    , _setButton(PIN_SET_BUTTON)
    , _debounceDelay(DEFAULT_DEBOUNCE_MS)
    , _longPressThreshold(DEFAULT_LONG_PRESS_MS)
    , _factoryResetThreshold(DEFAULT_FACTORY_RESET_MS)
    , _modeButtonInterrupt(false)
    , _setButtonInterrupt(false) {
}

bool ButtonHandler::init() {
    if (_initialized) {
        return true;
    }

    // Configure MODE button
    pinMode(_modeButton.pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_modeButton.pin), modeButtonISR, CHANGE);

    // Configure SET button (may share pin with RS485 DE/RE)
    // Only initialize if not in RS485 mode
    pinMode(_setButton.pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_setButton.pin), setButtonISR, CHANGE);

    _initialized = true;
    Logger::getInstance().info("ButtonHandler", "Initialized successfully");
    return true;
}

void ButtonHandler::update() {
    if (!_initialized || !_enabled) {
        return;
    }

    unsigned long now = millis();

    // Update MODE button
    if (readButton(_modeButton)) {
        if (_modeButton.currentState == BUTTON_PRESSED) {
            processPress(Button::MODE, _modeButton);
        } else {
            processRelease(Button::MODE, _modeButton);
        }
    }

    // Check for long press on MODE button
    if (_modeButton.currentState == BUTTON_PRESSED && 
        !_modeButton.longPressTriggered) {
        
        unsigned long pressDuration = now - _modeButton.pressStartTime;
        
        // Factory reset check (10s)
        if (pressDuration >= _factoryResetThreshold && 
            !_modeButton.factoryResetTriggered) {
            
            _modeButton.factoryResetTriggered = true;
            invokeCallback(Button::MODE, Event::FACTORY_RESET);
            Logger::getInstance().warn("ButtonHandler", "Factory reset triggered");
        }
        // Long press check (1s)
        else if (pressDuration >= _longPressThreshold) {
            _modeButton.longPressTriggered = true;
            invokeCallback(Button::MODE, Event::LONG_PRESS);
        }
    }

    // Update SET button
    if (readButton(_setButton)) {
        if (_setButton.currentState == BUTTON_PRESSED) {
            processPress(Button::SET, _setButton);
        } else {
            processRelease(Button::SET, _setButton);
        }
    }

    // Check for long press on SET button
    if (_setButton.currentState == BUTTON_PRESSED && 
        !_setButton.longPressTriggered) {
        
        unsigned long pressDuration = now - _setButton.pressStartTime;
        
        if (pressDuration >= _longPressThreshold) {
            _setButton.longPressTriggered = true;
            invokeCallback(Button::SET, Event::LONG_PRESS);
        }
    }

    // Reset interrupt flags
    _modeButtonInterrupt = false;
    _setButtonInterrupt = false;
}

void ButtonHandler::setCallback(ButtonCallback callback) {
    _callback = callback;
}

bool ButtonHandler::isPressed(Button button) const {
    if (button == Button::MODE) {
        return _modeButton.currentState == BUTTON_PRESSED;
    } else {
        return _setButton.currentState == BUTTON_PRESSED;
    }
}

bool ButtonHandler::readButton(ButtonState& state) {
    bool reading = digitalRead(state.pin);
    unsigned long now = millis();

    // If the switch changed, reset debounce timer
    if (reading != state.lastState) {
        state.lastChangeTime = now;
    }

    // Check if debounce time has passed
    if ((now - state.lastChangeTime) > _debounceDelay) {
        // If the button state has changed
        if (reading != state.currentState) {
            state.currentState = reading;
            state.lastState = reading;
            return true;  // State changed
        }
    }

    state.lastState = reading;
    return false;  // No change
}

void ButtonHandler::processPress(Button button, ButtonState& state) {
    state.pressStartTime = millis();
    state.longPressTriggered = false;
    state.factoryResetTriggered = false;

    const char* buttonName = (button == Button::MODE) ? "MODE" : "SET";
    Logger::getInstance().debug("ButtonHandler", (String(buttonName) + " button pressed").c_str());
}

void ButtonHandler::processRelease(Button button, ButtonState& state) {
    unsigned long pressDuration = millis() - state.pressStartTime;

    const char* buttonName = (button == Button::MODE) ? "MODE" : "SET";
    
    // Only trigger short press if it wasn't already a long press or factory reset
    if (!state.longPressTriggered && !state.factoryResetTriggered) {
        if (pressDuration < _longPressThreshold) {
            invokeCallback(button, Event::SHORT_PRESS);
            Logger::getInstance().debug("ButtonHandler", (String(buttonName) + " short press").c_str());
        }
    }

    // Reset flags
    state.pressStartTime = 0;
    state.longPressTriggered = false;
    state.factoryResetTriggered = false;
}

void ButtonHandler::invokeCallback(Button button, Event event) {
    if (_callback) {
        _callback(button, event);
    } else {
        // Default behavior if no callback set
        if (button == Button::MODE && event == Event::SHORT_PRESS) {
            // Next page
            DisplayManager::getInstance().nextPage();
        }
        else if (button == Button::MODE && event == Event::LONG_PRESS) {
            // Toggle auto-rotation
            DisplayManager& display = DisplayManager::getInstance();
            display.setAutoRotation(!display.isAutoRotationEnabled());
        }
        else if (button == Button::MODE && event == Event::FACTORY_RESET) {
            // Factory reset
            DisplayManager& display = DisplayManager::getInstance();
            display.clear();
            display.displayMessage(0, "Factory Reset...");
            display.displayMessage(1, "Please wait...");
            
            // Restore defaults
            ConfigManager::getInstance().restoreDefaults();
            ConfigManager::getInstance().save();
            NVStorage::getInstance().clear();
            
            display.displayMessage(2, "Done!");
            display.displayMessage(3, "Rebooting...");
            // No delay needed - ESP.restart() is immediate
            
            ESP.restart();
        }
        else if (button == Button::SET && event == Event::SHORT_PRESS) {
            // Previous page
            DisplayManager::getInstance().previousPage();
        }
        else if (button == Button::SET && event == Event::LONG_PRESS) {
            // Toggle display backlight or other function
            Logger::getInstance().info("ButtonHandler", "SET long press - no action defined");
        }
    }
}

void IRAM_ATTR ButtonHandler::modeButtonISR() {
    if (s_instance) {
        s_instance->_modeButtonInterrupt = true;
    }
}

void IRAM_ATTR ButtonHandler::setButtonISR() {
    if (s_instance) {
        s_instance->_setButtonInterrupt = true;
    }
}
