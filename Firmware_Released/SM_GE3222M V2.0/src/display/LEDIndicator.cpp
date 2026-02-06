/**
 * @file LEDIndicator.cpp
 * @brief LED status pattern management implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "LEDIndicator.h"
#include "../core/ErrorHandler.h"

LEDIndicator::LEDIndicator()
    : initialized(false),
      wifiState(WiFiState::OFF),
      running(false),
      faulted(false),
      faultType(FaultType::NONE),
      wifiLEDState(false),
      runLEDState(false),
      faultLEDState(false),
      modbusLEDState(false),
      lastWiFiToggle(0),
      lastRunUpdate(0),
      lastFaultToggle(0),
      modbusFlashStart(0) {
}

LEDIndicator& LEDIndicator::getInstance() {
    static LEDIndicator instance;
    return instance;
}

bool LEDIndicator::init() {
    // GPIOManager should already be initialized
    // Just set all LEDs to off initially
    GPIOManager& gpio = GPIOManager::getInstance();
    gpio.setLED(LED::WIFI, false);
    gpio.setLED(LED::RUN, false);
    gpio.setLED(LED::FAULT, false);
    gpio.setLED(LED::MODBUS, false);
    
    initialized = true;
    return true;
}

void LEDIndicator::setWiFiState(WiFiState state) {
    wifiState = state;
}

void LEDIndicator::setRunState(bool isRunning) {
    running = isRunning;
}

void LEDIndicator::setFaultState(bool isFaulted, FaultType type) {
    faulted = isFaulted;
    faultType = type;
    
    if (!faulted) {
        // Turn off fault LED immediately when fault cleared
        GPIOManager::getInstance().setLED(LED::FAULT, false);
        faultLEDState = false;
    }
}

void LEDIndicator::setModbusActivity() {
    modbusFlashStart = millis();
    modbusLEDState = true;
    GPIOManager::getInstance().setLED(LED::MODBUS, true);
}

void LEDIndicator::update() {
    if (!initialized) return;
    
    updateWiFiLED();
    updateRunLED();
    updateFaultLED();
    updateModbusLED();
}

void LEDIndicator::updateWiFiLED() {
    GPIOManager& gpio = GPIOManager::getInstance();
    unsigned long now = millis();
    
    switch (wifiState) {
        case WiFiState::OFF:
            // LED off
            if (wifiLEDState) {
                wifiLEDState = false;
                gpio.setLED(LED::WIFI, false);
            }
            break;
            
        case WiFiState::CONNECTING:
            // Slow blink (1Hz)
            if (now - lastWiFiToggle >= SLOW_BLINK_MS / 2) {
                wifiLEDState = !wifiLEDState;
                gpio.setLED(LED::WIFI, wifiLEDState);
                lastWiFiToggle = now;
            }
            break;
            
        case WiFiState::AP_MODE:
            // Fast blink (5Hz)
            if (now - lastWiFiToggle >= FAST_BLINK_MS / 2) {
                wifiLEDState = !wifiLEDState;
                gpio.setLED(LED::WIFI, wifiLEDState);
                lastWiFiToggle = now;
            }
            break;
            
        case WiFiState::CONNECTED:
            // Solid on
            if (!wifiLEDState) {
                wifiLEDState = true;
                gpio.setLED(LED::WIFI, true);
            }
            break;
    }
}

void LEDIndicator::updateRunLED() {
    if (!running) {
        // Turn off if not running
        if (runLEDState) {
            runLEDState = false;
            GPIOManager::getInstance().setLED(LED::RUN, false);
        }
        return;
    }
    
    // Heartbeat pattern
    unsigned long now = millis();
    bool newState = getHeartbeatState(now);
    
    if (newState != runLEDState) {
        runLEDState = newState;
        GPIOManager::getInstance().setLED(LED::RUN, runLEDState);
    }
}

void LEDIndicator::updateFaultLED() {
    if (!faulted) {
        return; // Already turned off in setFaultState
    }
    
    GPIOManager& gpio = GPIOManager::getInstance();
    unsigned long now = millis();
    
    // Determine blink rate based on fault type
    uint16_t blinkRate;
    switch (faultType) {
        case FaultType::COMMUNICATION:
            blinkRate = FAULT_BLINK_COMM;
            break;
        case FaultType::SENSOR:
            blinkRate = FAULT_BLINK_SENSOR;
            break;
        case FaultType::NETWORK:
            blinkRate = FAULT_BLINK_NETWORK;
            break;
        case FaultType::CRITICAL:
            blinkRate = FAULT_BLINK_CRITICAL;
            break;
        default:
            blinkRate = FAULT_BLINK_COMM;
            break;
    }
    
    // Toggle based on blink rate
    if (now - lastFaultToggle >= blinkRate / 2) {
        faultLEDState = !faultLEDState;
        gpio.setLED(LED::FAULT, faultLEDState);
        lastFaultToggle = now;
    }
}

void LEDIndicator::updateModbusLED() {
    if (!modbusLEDState) return;
    
    unsigned long now = millis();
    
    // Turn off after flash duration
    if (now - modbusFlashStart >= MODBUS_FLASH_MS) {
        modbusLEDState = false;
        GPIOManager::getInstance().setLED(LED::MODBUS, false);
    }
}

bool LEDIndicator::getHeartbeatState(unsigned long time) {
    // Heartbeat pattern: two quick pulses then pause
    // Pattern: ON-OFF-ON-OFF-PAUSE
    // Timing:  100ms on, 100ms off, 100ms on, 100ms off, 1600ms pause = 2000ms cycle
    
    uint16_t position = time % HEARTBEAT_CYCLE_MS;
    
    // First pulse: 0-100ms
    if (position < 100) return true;
    // Gap: 100-200ms
    if (position < 200) return false;
    // Second pulse: 200-300ms
    if (position < 300) return true;
    // Long pause: 300-2000ms
    return false;
}
