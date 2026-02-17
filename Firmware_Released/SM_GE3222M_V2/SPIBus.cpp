/**
 * SM-GE3222M V2.0 - SPI Bus Manager Implementation
 * 
 * Thread-safe SPI bus manager using FreeRTOS semaphores.
 * Ported from V1.0 EnergyATM90E36.cpp CommEnergyIC() function.
 */

#include "SPIBus.h"

SPIBus::SPIBus()
    : m_initialized(false)
    , m_defaultFrequency(SPI_FREQUENCY_ATM90E36)
    , m_mutex(nullptr)
    , m_sckPin(-1)
    , m_misoPin(-1)
    , m_mosiPin(-1)
{
    // Create mutex for thread-safe SPI access
    m_mutex = xSemaphoreCreateMutex();
}

SPIBus::~SPIBus() {
    if (m_mutex != nullptr) {
        vSemaphoreDelete(m_mutex);
        m_mutex = nullptr;
    }
}

bool SPIBus::init(int8_t sck, int8_t miso, int8_t mosi, uint32_t frequency) {
    if (m_initialized) {
        return true; // Already initialized
    }

    if (m_mutex == nullptr) {
        return false; // Mutex creation failed
    }

    m_sckPin = sck;
    m_misoPin = miso;
    m_mosiPin = mosi;
    m_defaultFrequency = frequency;

    // Initialize SPI with custom pins
    SPI.begin(sck, miso, mosi);

    m_initialized = true;
    return true;
}


void SPIBus::reinit() {
    // If not initialized yet, just init
    if (!m_initialized) {
        init();
        return;
    }
    if (m_mutex && xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        SPI.end();
        delay(1);
        SPI.begin(m_sckPin, m_misoPin, m_mosiPin);
        xSemaphoreGive(m_mutex);
    }
}

bool SPIBus::init() {
    // Initialize with default pins from PinMap.h
    return init(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, SPI_FREQUENCY_ATM90E36);
}

uint16_t SPIBus::transfer16(uint8_t csPin, uint16_t data, uint32_t frequency, uint8_t mode) {
    if (!m_initialized) {
        return 0xFFFF;
    }

    if (m_mutex == nullptr) {
        return 0xFFFF;
    }

    // Take mutex with 100ms timeout
    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return 0xFFFF; // Timeout
    }

    // Ensure CS pin is configured as output (robust against accidental reconfiguration)
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);

    SPISettings settings(frequency, MSBFIRST, mode);
    
    // Begin transaction
    SPI.beginTransaction(settings);
    
    // Assert chip select
    digitalWrite(csPin, LOW);
    delayMicroseconds(10);

    // Transfer data
    uint16_t result = SPI.transfer16(data);

    // Deassert chip select
    digitalWrite(csPin, HIGH);
    delayMicroseconds(10);

    // End transaction
    SPI.endTransaction();

    // Release mutex
    xSemaphoreGive(m_mutex);

    return result;
}

uint16_t SPIBus::readRegister(uint8_t csPin, uint16_t address) {
    // Use ATM90E36 protocol: R/W bit = 1 for read
    return commEnergyIC(csPin, 1, address, 0xFFFF);
}

bool SPIBus::writeRegister(uint8_t csPin, uint16_t address, uint16_t value) {
    // Use ATM90E36 protocol: R/W bit = 0 for write
    commEnergyIC(csPin, 0, address, value);
    return true; // Assuming write always succeeds
}

/**
 * Low-level SPI communication for ATM90E36
 * Ported from V1.0 CommEnergyIC() function
 * 
 * Protocol:
 * - 200kHz SPI clock
 * - SPI Mode 3 (CPOL=1, CPHA=1)
 * - MSB first
 * - Bit 15 of address = R/W flag (1=read, 0=write)
 * - Address and data are byte-swapped (MSB<->LSB)
 * - 4us delay after address before data transfer
 * - 10us CS setup/hold times
 */
uint16_t SPIBus::commEnergyIC(uint8_t csPin, uint8_t rw, uint16_t address, uint16_t value) {
    if (!m_initialized) {
        return 0xFFFF;
    }

    if (m_mutex == nullptr) {
        return 0xFFFF;
    }

    // Take mutex with 100ms timeout
    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return 0xFFFF; // Timeout
    }

    // Swap MSB and LSB of value (ATM90E36 protocol requirement)
    uint16_t val = swapBytes(value);

    // Set R/W flag in bit 15
    address |= (rw << 15);

    // Swap address bytes (ATM90E36 protocol requirement)
    uint16_t address1 = swapBytes(address);

    // Prepare byte pointers for transfer
    uint8_t* data = (uint8_t*)&val;
    uint8_t* adata = (uint8_t*)&address1;

    // SPI Settings for ATM90E36: 200kHz, MSB first, Mode 3
    SPISettings settings(SPI_FREQUENCY_ATM90E36, MSBFIRST, SPI_MODE_ATM90E36);

    // Ensure CS pin is configured correctly (robust against accidental reconfiguration)
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    delayMicroseconds(2);

    // Begin transaction
    SPI.beginTransaction(settings);

    // Chip enable and wait for SPI activation
    digitalWrite(csPin, LOW);
    delayMicroseconds(10);

    // Write address byte by byte (2 bytes)
    for (uint8_t i = 0; i < 2; i++) {
        SPI.transfer(*adata);
        adata++;
    }

    // Must wait 4us for data to become valid (ATM90E36 requirement)
    delayMicroseconds(4);

    // READ or WRITE Data
    if (rw) {
        // READ: Transfer dummy bytes and receive data
        for (uint8_t i = 0; i < 2; i++) {
            *data = SPI.transfer(0x00);
            data++;
        }
    } else {
        // WRITE: Transfer data bytes
        for (uint8_t i = 0; i < 2; i++) {
            SPI.transfer(*data);
            data++;
        }
    }

    // Chip disable and wait for transaction to end
    digitalWrite(csPin, HIGH);
    delayMicroseconds(10);

    // End transaction
    SPI.endTransaction();

    // Release mutex
    xSemaphoreGive(m_mutex);

    // Swap MSB and LSB back for return value
    uint16_t output = swapBytes(val);
    
    return output;
}