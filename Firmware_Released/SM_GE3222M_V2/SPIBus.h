#pragma once

/**
 * SM-GE3222M V2.0 - SPI Bus Manager
 * 
 * Thread-safe SPI bus manager using FreeRTOS semaphores.
 * Implements singleton pattern for centralized SPI access.
 * 
 * Features:
 * - Thread-safe transactions with mutex protection
 * - ATM90E36-specific protocol support (200kHz, Mode3, MSB-first)
 * - Automatic byte swapping for 16-bit data
 * - Multiple chip select support
 * 
 * Ported from V1.0 EnergyATM90E36.cpp CommEnergyIC() function
 */

#include <Arduino.h>
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "PinMap.h"

class SPIBus {
public:
    /**
     * Get singleton instance
     */
    static SPIBus& getInstance() {
        static SPIBus instance;
        return instance;
    }

    /**
     * Initialize SPI bus with custom pins
     * @param sck SPI clock pin
     * @param miso SPI MISO pin
     * @param mosi SPI MOSI pin
     * @param frequency Default SPI frequency
     * @return true if successful
     */
    bool init(int8_t sck, int8_t miso, int8_t mosi, uint32_t frequency = SPI_FREQUENCY_ATM90E36);

    /**
     * Initialize SPI bus with default pins from PinMap.h
     * @return true if successful
     */
    bool init();

    /**
     * Generic 16-bit SPI transfer with chip select
     * @param csPin Chip select pin
     * @param data 16-bit data to send
     * @param frequency SPI frequency (default: 200kHz)
     * @param mode SPI mode (default: MODE3)
     * @return 16-bit received data
     */
    uint16_t transfer16(uint8_t csPin, uint16_t data, uint32_t frequency = SPI_FREQUENCY_ATM90E36, uint8_t mode = SPI_MODE_ATM90E36);

    /**
     * Read 16-bit register from ATM90E36
     * Uses ATM90E36 protocol: bit 15 = 1 for read, byte-swapped address and data
     * @param csPin Chip select pin
     * @param address Register address (0x0000-0x7FFF)
     * @return 16-bit register value
     */
    uint16_t readRegister(uint8_t csPin, uint16_t address);

    /**
     * Write 16-bit register to ATM90E36
     * Uses ATM90E36 protocol: bit 15 = 0 for write, byte-swapped address and data
     * @param csPin Chip select pin
     * @param address Register address (0x0000-0x7FFF)
     * @param value 16-bit value to write
     * @return true if successful
     */
    bool writeRegister(uint8_t csPin, uint16_t address, uint16_t value);

    /**
     * Check if SPI bus is initialized
     */
    bool isInitialized() const { return m_initialized; }

    // Reinitialize the SPI bus pins/state (useful if another component changed SPI configuration)
    void reinit();

private:
    // Singleton - prevent copying
    SPIBus();
    ~SPIBus();
    SPIBus(const SPIBus&) = delete;
    SPIBus& operator=(const SPIBus&) = delete;

    /**
     * Swap MSB and LSB of 16-bit value
     * Required for ATM90E36 protocol
     */
    inline uint16_t swapBytes(uint16_t value) {
        return (value >> 8) | (value << 8);
    }

    /**
     * Low-level SPI transaction (ATM90E36 protocol)
     * Ported from V1.0 CommEnergyIC() function
     * @param rw Read/Write flag (1=read, 0=write)
     * @param address Register address
     * @param value Value to write (ignored for reads)
     * @return Read value (for reads) or written value (for writes)
     */
    uint16_t commEnergyIC(uint8_t csPin, uint8_t rw, uint16_t address, uint16_t value);

    // State
    bool m_initialized;
    uint32_t m_defaultFrequency;
    SemaphoreHandle_t m_mutex;
    
    // Pin configuration
    int8_t m_sckPin;
    int8_t m_misoPin;
    int8_t m_mosiPin;
};
