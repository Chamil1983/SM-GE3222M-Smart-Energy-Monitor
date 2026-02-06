/**
 * @file SPIBus.h
 * @brief Thread-safe SPI bus management
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Manages shared SPI bus access for ATM90E36 and W5500
 */

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "../../include/PinMap.h"

/**
 * @brief SPI device identifiers
 */
enum class SPIDevice {
    ATM90E36,   // Energy meter IC
    W5500       // Ethernet controller
};

/**
 * @class SPIBus
 * @brief Singleton class for thread-safe SPI bus management
 * 
 * Provides mutex-protected access to shared SPI bus
 * Manages chip select and speed configuration per device
 * Tracks error counts for diagnostics
 */
class SPIBus {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to SPIBus instance
     */
    static SPIBus& getInstance();
    
    /**
     * @brief Initialize SPI bus
     * @return true if initialization successful
     */
    bool init();
    
    /**
     * @brief Begin SPI transaction for specific device
     * @param device Device to communicate with
     * @return true if transaction started successfully
     */
    bool beginTransaction(SPIDevice device);
    
    /**
     * @brief End SPI transaction
     */
    void endTransaction();
    
    /**
     * @brief Transfer 16-bit data
     * @param data Data to send
     * @return Received data
     */
    uint16_t transfer16(uint16_t data);
    
    /**
     * @brief Transfer 8-bit data
     * @param data Data to send
     * @return Received data
     */
    uint8_t transfer8(uint8_t data);
    
    /**
     * @brief Transfer multiple bytes
     * @param buffer Data buffer (in/out)
     * @param size Number of bytes to transfer
     */
    void transfer(uint8_t* buffer, size_t size);
    
    /**
     * @brief Get error count for device
     * @param device Device to query
     * @return Number of errors recorded
     */
    uint32_t getErrorCount(SPIDevice device) const;
    
    /**
     * @brief Reset error count for device
     * @param device Device to reset
     */
    void resetErrorCount(SPIDevice device);
    
    /**
     * @brief Check if bus is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Record an error for current device
     */
    void recordError();
    
private:
    SPIBus();
    ~SPIBus();
    SPIBus(const SPIBus&) = delete;
    SPIBus& operator=(const SPIBus&) = delete;
    
    void selectDevice(SPIDevice device);
    void deselectDevice();
    uint32_t getDeviceSpeed(SPIDevice device) const;
    uint8_t getDeviceCSPin(SPIDevice device) const;
    
    bool initialized;
    SPIDevice currentDevice;
    bool transactionActive;
    
    SemaphoreHandle_t mutex;
    static const TickType_t MUTEX_TIMEOUT = pdMS_TO_TICKS(100);
    
    uint32_t errorCountATM;
    uint32_t errorCountW5500;
};
