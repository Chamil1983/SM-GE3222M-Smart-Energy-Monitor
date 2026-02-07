/**
 * @file I2CBus.h
 * @brief I2C bus management and device detection
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Manages I2C communication for LCD, MCP23017, and sensors
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "../../include/PinMap.h"

/**
 * @class I2CBus
 * @brief Singleton class for I2C bus management
 * 
 * Provides I2C communication primitives with timeout handling
 * Device scanning and presence detection
 * Error tracking for diagnostics
 */
class I2CBus {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to I2CBus instance
     */
    static I2CBus& getInstance();
    
    /**
     * @brief Initialize I2C bus
     * @param frequency I2C clock frequency (default 100kHz)
     * @return true if initialization successful
     */
    bool init(uint32_t frequency = 100000);
    
    /**
     * @brief Scan I2C bus for devices
     * @param addresses Array to store found addresses
     * @param maxDevices Maximum number of devices to find
     * @return Number of devices found
     */
    uint8_t scan(uint8_t* addresses, uint8_t maxDevices = 16);
    
    /**
     * @brief Check if device is present on bus
     * @param address I2C device address
     * @return true if device responds
     */
    bool isDevicePresent(uint8_t address);
    
    /**
     * @brief Read byte from device register
     * @param address I2C device address
     * @param reg Register address
     * @param value Pointer to store read value
     * @return true if read successful
     */
    bool readRegister(uint8_t address, uint8_t reg, uint8_t* value);
    
    /**
     * @brief Write byte to device register
     * @param address I2C device address
     * @param reg Register address
     * @param value Value to write
     * @return true if write successful
     */
    bool writeRegister(uint8_t address, uint8_t reg, uint8_t value);
    
    /**
     * @brief Read multiple bytes from device
     * @param address I2C device address
     * @param reg Starting register address
     * @param buffer Buffer to store data
     * @param length Number of bytes to read
     * @return true if read successful
     */
    bool read(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t length);
    
    /**
     * @brief Write multiple bytes to device
     * @param address I2C device address
     * @param reg Starting register address
     * @param buffer Data to write
     * @param length Number of bytes to write
     * @return true if write successful
     */
    bool write(uint8_t address, uint8_t reg, const uint8_t* buffer, uint8_t length);
    
    /**
     * @brief Read byte without register address (simple read)
     * @param address I2C device address
     * @param value Pointer to store read value
     * @return true if read successful
     */
    bool readByte(uint8_t address, uint8_t* value);
    
    /**
     * @brief Write byte without register address (simple write)
     * @param address I2C device address
     * @param value Value to write
     * @return true if write successful
     */
    bool writeByte(uint8_t address, uint8_t value);
    
    /**
     * @brief Get error count
     * @return Total number of I2C errors
     */
    uint32_t getErrorCount() const { return errorCount; }
    
    /**
     * @brief Reset error count
     */
    void resetErrorCount() { errorCount = 0; }
    
    /**
     * @brief Set timeout for I2C operations
     * @param timeoutMs Timeout in milliseconds
     */
    void setTimeout(uint32_t timeoutMs) { timeout = timeoutMs; }
    
    /**
     * @brief Check if bus is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
private:
    I2CBus();
    ~I2CBus() = default;
    I2CBus(const I2CBus&) = delete;
    I2CBus& operator=(const I2CBus&) = delete;
    
    void recordError();
    bool waitForReady(uint32_t timeoutMs);
    
    bool initialized;
    uint32_t errorCount;
    uint32_t timeout;  // Default timeout in milliseconds
    
    static const uint32_t DEFAULT_TIMEOUT_MS = 500;
};
