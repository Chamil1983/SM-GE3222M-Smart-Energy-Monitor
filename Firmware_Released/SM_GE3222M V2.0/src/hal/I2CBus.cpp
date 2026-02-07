/**
 * @file I2CBus.cpp
 * @brief I2C bus management implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "I2CBus.h"

I2CBus::I2CBus() 
    : initialized(false)
    , errorCount(0)
    , timeout(DEFAULT_TIMEOUT_MS) {
}

I2CBus& I2CBus::getInstance() {
    static I2CBus instance;
    return instance;
}

bool I2CBus::init(uint32_t frequency) {
    if (initialized) {
        return true;
    }
    
    Serial.println(F("[I2CBus] Initializing I2C bus..."));
    
    // Initialize Wire library
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.setClock(frequency);
    Wire.setTimeOut(timeout);
    
    initialized = true;
    
    Serial.print(F("[I2CBus] I2C initialized at "));
    Serial.print(frequency / 1000);
    Serial.println(F(" kHz"));
    
    // Scan for devices
    uint8_t addresses[16];
    uint8_t count = scan(addresses, 16);
    
    Serial.print(F("[I2CBus] Found "));
    Serial.print(count);
    Serial.println(F(" device(s)"));
    
    if (count > 0) {
        Serial.print(F("[I2CBus] Addresses: "));
        for (uint8_t i = 0; i < count; i++) {
            Serial.print(F("0x"));
            if (addresses[i] < 16) Serial.print('0');
            Serial.print(addresses[i], HEX);
            if (i < count - 1) Serial.print(F(", "));
        }
        Serial.println();
    }
    
    return true;
}

uint8_t I2CBus::scan(uint8_t* addresses, uint8_t maxDevices) {
    if (!initialized || addresses == nullptr || maxDevices == 0) {
        return 0;
    }
    
    uint8_t count = 0;
    
    // Scan I2C address range 0x08 to 0x77
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();
        
        if (error == 0) {
            // Device found
            if (count < maxDevices) {
                addresses[count] = addr;
                count++;
            }
        }
        
        delay(1);  // Small delay between scans
    }
    
    return count;
}

bool I2CBus::isDevicePresent(uint8_t address) {
    if (!initialized) {
        return false;
    }
    
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();
    
    return (error == 0);
}

bool I2CBus::readRegister(uint8_t address, uint8_t reg, uint8_t* value) {
    if (!initialized || value == nullptr) {
        recordError();
        return false;
    }
    
    // Write register address
    Wire.beginTransmission(address);
    Wire.write(reg);
    uint8_t error = Wire.endTransmission(false);  // Don't send stop
    
    if (error != 0) {
        recordError();
        return false;
    }
    
    // Read data
    uint8_t bytesRead = Wire.requestFrom(address, (uint8_t)1);
    if (bytesRead != 1) {
        recordError();
        return false;
    }
    
    *value = Wire.read();
    return true;
}

bool I2CBus::writeRegister(uint8_t address, uint8_t reg, uint8_t value) {
    if (!initialized) {
        recordError();
        return false;
    }
    
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    uint8_t error = Wire.endTransmission();
    
    if (error != 0) {
        recordError();
        return false;
    }
    
    return true;
}

bool I2CBus::read(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t length) {
    if (!initialized || buffer == nullptr || length == 0) {
        recordError();
        return false;
    }
    
    // Write register address
    Wire.beginTransmission(address);
    Wire.write(reg);
    uint8_t error = Wire.endTransmission(false);  // Don't send stop
    
    if (error != 0) {
        recordError();
        return false;
    }
    
    // Read data
    uint8_t bytesRead = Wire.requestFrom(address, length);
    if (bytesRead != length) {
        recordError();
        return false;
    }
    
    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = Wire.read();
    }
    
    return true;
}

bool I2CBus::write(uint8_t address, uint8_t reg, const uint8_t* buffer, uint8_t length) {
    if (!initialized || buffer == nullptr || length == 0) {
        recordError();
        return false;
    }
    
    Wire.beginTransmission(address);
    Wire.write(reg);
    
    for (uint8_t i = 0; i < length; i++) {
        Wire.write(buffer[i]);
    }
    
    uint8_t error = Wire.endTransmission();
    
    if (error != 0) {
        recordError();
        return false;
    }
    
    return true;
}

bool I2CBus::readByte(uint8_t address, uint8_t* value) {
    if (!initialized || value == nullptr) {
        recordError();
        return false;
    }
    
    uint8_t bytesRead = Wire.requestFrom(address, (uint8_t)1);
    if (bytesRead != 1) {
        recordError();
        return false;
    }
    
    *value = Wire.read();
    return true;
}

bool I2CBus::writeByte(uint8_t address, uint8_t value) {
    if (!initialized) {
        recordError();
        return false;
    }
    
    Wire.beginTransmission(address);
    Wire.write(value);
    uint8_t error = Wire.endTransmission();
    
    if (error != 0) {
        recordError();
        return false;
    }
    
    return true;
}

void I2CBus::recordError() {
    errorCount++;
}

bool I2CBus::waitForReady(uint32_t timeoutMs) {
    uint32_t startTime = millis();
    
    while ((millis() - startTime) < timeoutMs) {
        // Check if bus is ready (implementation depends on hardware)
        // For now, just add a small delay
        delay(1);
    }
    
    return true;
}
