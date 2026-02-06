/**
 * @file BoardConfig.h
 * @brief Board configuration and hardware identification
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Hardware abstraction for board initialization and identification
 */

#pragma once

#include <Arduino.h>
#include "../../include/PinMap.h"
#include "../../include/GlobalTypes.h"

/**
 * @class BoardConfig
 * @brief Singleton class for board configuration and identification
 * 
 * Handles hardware initialization, pin configuration, and board self-identification
 * Validates hardware presence and configuration on startup
 */
class BoardConfig {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to BoardConfig instance
     */
    static BoardConfig& getInstance();
    
    /**
     * @brief Initialize board hardware
     * @return true if initialization successful
     */
    bool init();
    
    /**
     * @brief Get board name
     * @return Board name string
     */
    const char* getName() const { return boardName; }
    
    /**
     * @brief Get board hardware revision
     * @return Hardware revision string
     */
    const char* getRevision() const { return hwRevision; }
    
    /**
     * @brief Validate hardware configuration
     * @return true if all hardware validated successfully
     */
    bool validate();
    
    /**
     * @brief Check if board is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Get ESP32 chip ID
     * @return Chip ID as uint64_t
     */
    uint64_t getChipID() const;
    
    /**
     * @brief Get MAC address
     * @param mac Buffer to store MAC address (6 bytes)
     */
    void getMacAddress(uint8_t* mac) const;
    
    /**
     * @brief Get chip temperature (if available)
     * @return Temperature in Celsius, or NAN if not available
     */
    float getChipTemperature() const;
    
    /**
     * @brief Print board information to Serial
     */
    void printInfo() const;
    
private:
    BoardConfig();
    ~BoardConfig() = default;
    BoardConfig(const BoardConfig&) = delete;
    BoardConfig& operator=(const BoardConfig&) = delete;
    
    bool initGPIO();
    bool initPeripherals();
    bool detectHardware();
    
    bool initialized;
    const char* boardName;
    const char* hwRevision;
    uint64_t chipID;
};
