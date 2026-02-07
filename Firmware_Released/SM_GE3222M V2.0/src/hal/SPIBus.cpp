/**
 * @file SPIBus.cpp
 * @brief SPI bus management implementation
 *
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "SPIBus.h"

SPIBus::SPIBus()
    : initialized(false)
    , currentDevice(SPIDevice::ATM90E36)
    , transactionActive(false)
    , mutex(nullptr)
    , errorCountATM(0)
    , errorCountW5500(0) {
}

SPIBus::~SPIBus() {
    if (mutex != nullptr) {
        vSemaphoreDelete(mutex);
        mutex = nullptr;
    }
}

SPIBus& SPIBus::getInstance() {
    static SPIBus instance;
    return instance;
}

bool SPIBus::init() {
    if (initialized) {
        return true;
    }

    Serial.println(F("[SPIBus] Initializing SPI bus..."));

    // Create mutex for thread safety
    mutex = xSemaphoreCreateMutex();
    if (mutex == nullptr) {
        Serial.println(F("[SPIBus] ERROR: Failed to create mutex"));
        return false;
    }

    // Initialize SPI bus
    SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);

    // Configure CS pins (already done in BoardConfig, but ensure they're high)
    digitalWrite(PIN_SPI_CS_ATM, HIGH);
    digitalWrite(PIN_SPI_CS_W5500, HIGH);

    initialized = true;

    Serial.println(F("[SPIBus] SPI bus initialized"));
    Serial.print(F("[SPIBus] ATM90E36 speed: "));
    Serial.print(SPI_FREQ_ATM / 1000000);
    Serial.println(F(" MHz"));
    Serial.print(F("[SPIBus] W5500 speed: "));
    Serial.print(SPI_FREQ_W5500 / 1000000);
    Serial.println(F(" MHz"));

    return true;
}

bool SPIBus::beginTransaction(SPIDevice device) {
    if (!initialized) {
        Serial.println(F("[SPIBus] ERROR: Not initialized"));
        return false;
    }

    // Acquire mutex with timeout
    if (xSemaphoreTake(mutex, MUTEX_TIMEOUT) != pdTRUE) {
        Serial.println(F("[SPIBus] ERROR: Mutex timeout"));
        recordError();
        return false;
    }

    currentDevice = device;
    transactionActive = true;

    // Configure SPI for this device
    uint32_t speed = getDeviceSpeed(device);
    SPI.beginTransaction(SPISettings(speed, MSBFIRST, ATM_SPI_MODE));

    // Assert chip select
    selectDevice(device);

    return true;
}

void SPIBus::endTransaction() {
    if (!transactionActive) {
        return;
    }

    // Deassert chip select
    deselectDevice();

    // End SPI transaction
    SPI.endTransaction();

    transactionActive = false;

    // Release mutex
    xSemaphoreGive(mutex);
}

uint16_t SPIBus::transfer16(uint16_t data) {
    if (!transactionActive) {
        Serial.println(F("[SPIBus] ERROR: No active transaction"));
        return 0;
    }

    return SPI.transfer16(data);
}

uint8_t SPIBus::transfer8(uint8_t data) {
    if (!transactionActive) {
        Serial.println(F("[SPIBus] ERROR: No active transaction"));
        return 0;
    }

    return SPI.transfer(data);
}

void SPIBus::transfer(uint8_t* buffer, size_t size) {
    if (!transactionActive) {
        Serial.println(F("[SPIBus] ERROR: No active transaction"));
        return;
    }

    if (buffer == nullptr || size == 0) {
        return;
    }

    SPI.transfer(buffer, size);
}

uint32_t SPIBus::getErrorCount(SPIDevice device) const {
    switch (device) {
    case SPIDevice::ATM90E36:
        return errorCountATM;
    case SPIDevice::W5500:
        return errorCountW5500;
    default:
        return 0;
    }
}

void SPIBus::resetErrorCount(SPIDevice device) {
    switch (device) {
    case SPIDevice::ATM90E36:
        errorCountATM = 0;
        break;
    case SPIDevice::W5500:
        errorCountW5500 = 0;
        break;
    }
}

void SPIBus::recordError() {
    if (transactionActive) {
        switch (currentDevice) {
        case SPIDevice::ATM90E36:
            errorCountATM++;
            break;
        case SPIDevice::W5500:
            errorCountW5500++;
            break;
        }
    }
}

void SPIBus::selectDevice(SPIDevice device) {
    uint8_t csPin = getDeviceCSPin(device);
    digitalWrite(csPin, LOW);
    // Small delay for CS setup time
    delayMicroseconds(1);
}

void SPIBus::deselectDevice() {
    if (transactionActive) {
        uint8_t csPin = getDeviceCSPin(currentDevice);
        digitalWrite(csPin, HIGH);
        // Small delay for CS hold time
        delayMicroseconds(1);
    }
}

uint32_t SPIBus::getDeviceSpeed(SPIDevice device) const {
    switch (device) {
    case SPIDevice::ATM90E36:
        return SPI_FREQ_ATM;
    case SPIDevice::W5500:
        return SPI_FREQ_W5500;
    default:
        return 1000000;  // Default 1 MHz
    }
}

uint8_t SPIBus::getDeviceCSPin(SPIDevice device) const {
    switch (device) {
    case SPIDevice::ATM90E36:
        return PIN_SPI_CS_ATM;
    case SPIDevice::W5500:
        return PIN_SPI_CS_W5500;
    default:
        return PIN_SPI_CS_ATM;
    }
}