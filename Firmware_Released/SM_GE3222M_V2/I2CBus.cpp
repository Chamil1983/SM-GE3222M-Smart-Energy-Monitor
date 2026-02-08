/**
 * SM-GE3222M V2.0 - I2C Bus Manager Implementation
 */

#include "I2CBus.h"
#include "Logger.h"

I2CBus::I2CBus() 
    : m_initialized(false)
    , m_sdaPin(PIN_I2C_SDA)
    , m_sclPin(PIN_I2C_SCL)
    , m_frequency(I2C_FREQUENCY)
{
    m_mutex = xSemaphoreCreateMutex();
    if (m_mutex == NULL) {
        Logger::error("I2CBus: Failed to create mutex");
    }
}

I2CBus::~I2CBus() {
    if (m_mutex != NULL) {
        vSemaphoreDelete(m_mutex);
    }
}

bool I2CBus::init(uint8_t sda, uint8_t scl, uint32_t frequency) {
    if (m_initialized) {
        Logger::warning("I2CBus: Already initialized");
        return true;
    }

    m_sdaPin = sda;
    m_sclPin = scl;
    m_frequency = frequency;

    if (!Wire.begin(m_sdaPin, m_sclPin, m_frequency)) {
        Logger::error("I2CBus: Failed to initialize I2C");
        return false;
    }

    m_initialized = true;
    Logger::info("I2CBus: Initialized (SDA=%d, SCL=%d, freq=%d Hz)", 
                 m_sdaPin, m_sclPin, m_frequency);
    
    return true;
}

bool I2CBus::init() {
    return init(PIN_I2C_SDA, PIN_I2C_SCL, I2C_FREQUENCY);
}

uint8_t I2CBus::scanBus() {
    if (!m_initialized) {
        Logger::error("I2CBus: Not initialized");
        return 0;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::error("I2CBus: Failed to acquire mutex for scan");
        return 0;
    }

    uint8_t count = 0;
    Logger::info("I2CBus: Scanning bus...");
    
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            Logger::info("I2CBus: Device found at 0x%02X", addr);
            count++;
        }
    }
    
    Logger::info("I2CBus: Found %d device(s)", count);
    xSemaphoreGive(m_mutex);
    
    return count;
}

uint8_t I2CBus::readByte(uint8_t addr, uint8_t reg) {
    if (!m_initialized) {
        Logger::error("I2CBus: Not initialized");
        return 0xFF;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::error("I2CBus: Failed to acquire mutex for read");
        return 0xFF;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        xSemaphoreGive(m_mutex);
        return 0xFF;
    }

    if (Wire.requestFrom(addr, (uint8_t)1) != 1) {
        xSemaphoreGive(m_mutex);
        return 0xFF;
    }

    uint8_t value = Wire.read();
    xSemaphoreGive(m_mutex);
    
    return value;
}

bool I2CBus::writeByte(uint8_t addr, uint8_t reg, uint8_t data) {
    if (!m_initialized) {
        Logger::error("I2CBus: Not initialized");
        return false;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::error("I2CBus: Failed to acquire mutex for write");
        return false;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(data);
    uint8_t result = Wire.endTransmission();
    
    xSemaphoreGive(m_mutex);
    
    return (result == 0);
}

uint8_t I2CBus::readBytes(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len) {
    if (!m_initialized || buf == nullptr || len == 0) {
        Logger::error("I2CBus: Invalid read parameters");
        return 0;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::error("I2CBus: Failed to acquire mutex for read");
        return 0;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) {
        xSemaphoreGive(m_mutex);
        return 0;
    }

    uint8_t received = Wire.requestFrom(addr, len);
    for (uint8_t i = 0; i < received; i++) {
        buf[i] = Wire.read();
    }

    xSemaphoreGive(m_mutex);
    
    return received;
}

bool I2CBus::writeBytes(uint8_t addr, uint8_t reg, const uint8_t* buf, uint8_t len) {
    if (!m_initialized || buf == nullptr || len == 0) {
        Logger::error("I2CBus: Invalid write parameters");
        return false;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::error("I2CBus: Failed to acquire mutex for write");
        return false;
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(buf, len);
    uint8_t result = Wire.endTransmission();
    
    xSemaphoreGive(m_mutex);
    
    return (result == 0);
}
