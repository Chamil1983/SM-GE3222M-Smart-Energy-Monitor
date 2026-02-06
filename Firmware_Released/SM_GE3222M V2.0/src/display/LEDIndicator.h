/**
 * @file LEDIndicator.h
 * @brief LED status pattern management
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Manages LED patterns for WiFi, Run, Fault, and Modbus indicators
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"
#include "../hal/GPIOManager.h"

/**
 * @brief WiFi connection states
 */
enum class WiFiState {
    OFF,            // WiFi disabled
    CONNECTING,     // Attempting connection
    AP_MODE,        // Access Point mode
    CONNECTED       // Connected to network
};

/**
 * @brief Fault types
 */
enum class FaultType {
    NONE = 0,
    COMMUNICATION,  // I2C/SPI communication fault
    SENSOR,         // Sensor read fault
    NETWORK,        // Network connection fault
    CRITICAL        // Critical system fault
};

/**
 * @class LEDIndicator
 * @brief Manages LED status indicators with non-blocking patterns
 * 
 * Singleton class providing LED pattern management:
 * - WiFi LED: Shows connection status (off/slow blink/fast blink/solid)
 * - RUN LED: Heartbeat pattern when system running
 * - FLT LED: Fault indication with pattern based on fault type
 * - MODBUS LED: Flash on Modbus activity
 * 
 * All patterns are non-blocking using millis() timing
 */
class LEDIndicator {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to LEDIndicator instance
     */
    static LEDIndicator& getInstance();
    
    /**
     * @brief Initialize LED indicator system
     * @return true if successful
     */
    bool init();
    
    /**
     * @brief Set WiFi state
     * @param state Current WiFi state
     */
    void setWiFiState(WiFiState state);
    
    /**
     * @brief Set run state
     * @param running True if system is running normally
     */
    void setRunState(bool running);
    
    /**
     * @brief Set fault state
     * @param faulted True if fault is active
     * @param type Type of fault
     */
    void setFaultState(bool faulted, FaultType type = FaultType::NONE);
    
    /**
     * @brief Trigger Modbus activity flash
     * 
     * Flashes Modbus LED for 100ms
     */
    void setModbusActivity();
    
    /**
     * @brief Update LED patterns (call from main loop)
     * 
     * Non-blocking update that manages all LED patterns
     */
    void update();
    
    /**
     * @brief Get WiFi state
     * @return Current WiFi state
     */
    WiFiState getWiFiState() const { return wifiState; }
    
    /**
     * @brief Get fault state
     * @return True if fault is active
     */
    bool isFaulted() const { return faulted; }
    
    /**
     * @brief Get fault type
     * @return Current fault type
     */
    FaultType getFaultType() const { return faultType; }

private:
    // Private constructor for singleton
    LEDIndicator();
    
    // Delete copy constructor and assignment operator
    LEDIndicator(const LEDIndicator&) = delete;
    LEDIndicator& operator=(const LEDIndicator&) = delete;
    
    /**
     * @brief Update WiFi LED pattern
     */
    void updateWiFiLED();
    
    /**
     * @brief Update RUN LED pattern
     */
    void updateRunLED();
    
    /**
     * @brief Update Fault LED pattern
     */
    void updateFaultLED();
    
    /**
     * @brief Update Modbus LED pattern
     */
    void updateModbusLED();
    
    /**
     * @brief Calculate heartbeat pattern value
     * @param time Current time in milliseconds
     * @return LED state (true = on)
     */
    bool getHeartbeatState(unsigned long time);
    
    // State
    bool initialized;
    WiFiState wifiState;
    bool running;
    bool faulted;
    FaultType faultType;
    
    // LED states
    bool wifiLEDState;
    bool runLEDState;
    bool faultLEDState;
    bool modbusLEDState;
    
    // Timing
    unsigned long lastWiFiToggle;
    unsigned long lastRunUpdate;
    unsigned long lastFaultToggle;
    unsigned long modbusFlashStart;
    
    // Pattern parameters
    static const uint16_t SLOW_BLINK_MS = 1000;     // 1Hz blink (500ms on/off)
    static const uint16_t FAST_BLINK_MS = 200;      // 5Hz blink (100ms on/off)
    static const uint16_t MODBUS_FLASH_MS = 100;    // 100ms flash duration
    static const uint16_t HEARTBEAT_CYCLE_MS = 2000; // 2 second heartbeat cycle
    
    // Fault blink patterns (in ms)
    static const uint16_t FAULT_BLINK_COMM = 500;
    static const uint16_t FAULT_BLINK_SENSOR = 300;
    static const uint16_t FAULT_BLINK_NETWORK = 700;
    static const uint16_t FAULT_BLINK_CRITICAL = 150;
};
