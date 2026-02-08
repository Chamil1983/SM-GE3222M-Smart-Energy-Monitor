#pragma once

/**
 * SM-GE3222M V2.0 - I2C Bus Manager
 * 
 * Thread-safe I2C bus manager using FreeRTOS semaphores.
 * Implements singleton pattern for centralized I2C access.
 * 
 * Features:
 * - Thread-safe transactions with mutex protection
 * - Bus scanning for device discovery
 * - Single and multi-byte read/write operations
 * - Default pins from PinMap.h (SDA=21, SCL=22, 100kHz)
 */

#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "PinMap.h"

class I2CBus {
public:
    /**
     * Get singleton instance
     */
    static I2CBus& getInstance() {
        static I2CBus instance;
        return instance;
    }

    /**
     * Initialize I2C bus with custom pins
     * @param sda I2C data pin
     * @param scl I2C clock pin
     * @param frequency I2C frequency in Hz
     * @return true if successful
     */
    bool init(uint8_t sda, uint8_t scl, uint32_t frequency);

    /**
     * Initialize I2C bus with default pins from PinMap.h
     * @return true if successful
     */
    bool init();

    /**
     * Scan I2C bus for connected devices
     * @return Number of devices found
     */
    uint8_t scanBus();

    /**
     * Read single byte from device register
     * @param addr Device I2C address
     * @param reg Register address
     * @return Byte value read (0xFF on error)
     */
    uint8_t readByte(uint8_t addr, uint8_t reg);

    /**
     * Write single byte to device register
     * @param addr Device I2C address
     * @param reg Register address
     * @param data Byte value to write
     * @return true if successful
     */
    bool writeByte(uint8_t addr, uint8_t reg, uint8_t data);

    /**
     * Read multiple bytes from device
     * @param addr Device I2C address
     * @param reg Starting register address
     * @param buf Buffer to store read data
     * @param len Number of bytes to read
     * @return Number of bytes read (0 on error)
     */
    uint8_t readBytes(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len);

    /**
     * Write multiple bytes to device
     * @param addr Device I2C address
     * @param reg Starting register address
     * @param buf Buffer containing data to write
     * @param len Number of bytes to write
     * @return true if successful
     */
    bool writeBytes(uint8_t addr, uint8_t reg, const uint8_t* buf, uint8_t len);

    /**
     * Check if I2C bus is initialized
     */
    bool isInitialized() const { return m_initialized; }

private:
    // Singleton - prevent copying
    I2CBus();
    ~I2CBus();
    I2CBus(const I2CBus&) = delete;
    I2CBus& operator=(const I2CBus&) = delete;

    // State
    bool m_initialized;
    SemaphoreHandle_t m_mutex;
    
    // Pin configuration
    uint8_t m_sdaPin;
    uint8_t m_sclPin;
    uint32_t m_frequency;
};
