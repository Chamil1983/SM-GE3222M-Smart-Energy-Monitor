/**
 * @file BoardConfig.cpp
 * @brief Board configuration implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "BoardConfig.h"
#include <esp_system.h>
#include <esp_chip_info.h>
#include <esp_mac.h>

BoardConfig::BoardConfig() 
    : initialized(false)
    , boardName("SM-GE3222M")
    , hwRevision("V2.0")
    , chipID(0) {
}

BoardConfig& BoardConfig::getInstance() {
    static BoardConfig instance;
    return instance;
}

bool BoardConfig::init() {
    if (initialized) {
        return true;
    }
    
    Serial.println(F("[BoardConfig] Initializing board configuration..."));
    
    // Get chip ID
    chipID = ESP.getEfuseMac();
    
    // Initialize GPIO
    if (!initGPIO()) {
        Serial.println(F("[BoardConfig] ERROR: GPIO initialization failed"));
        return false;
    }
    
    // Initialize peripherals
    if (!initPeripherals()) {
        Serial.println(F("[BoardConfig] ERROR: Peripheral initialization failed"));
        return false;
    }
    
    // Detect and validate hardware
    if (!detectHardware()) {
        Serial.println(F("[BoardConfig] WARNING: Some hardware not detected"));
        // Not a fatal error, continue
    }
    
    initialized = true;
    
    // Print board info
    printInfo();
    
    Serial.println(F("[BoardConfig] Initialization complete"));
    return true;
}

bool BoardConfig::initGPIO() {
    // Configure chip select pins as outputs (high = inactive)
    pinMode(PIN_SPI_CS_ATM, OUTPUT);
    digitalWrite(PIN_SPI_CS_ATM, HIGH);
    
    pinMode(PIN_SPI_CS_W5500, OUTPUT);
    digitalWrite(PIN_SPI_CS_W5500, HIGH);
    
    // Configure RS485 direction pins
    pinMode(PIN_RS485_DE, OUTPUT);
    digitalWrite(PIN_RS485_DE, LOW);  // Receiver mode by default
    
    // Configure button inputs with pullup
    pinMode(PIN_MODE_BUTTON, INPUT_PULLUP);
    pinMode(PIN_SET_BUTTON, INPUT_PULLUP);
    
    // Configure MCP23017 interrupt pins
    pinMode(PIN_MCP_INT_A, INPUT_PULLUP);
    pinMode(PIN_MCP_INT_B, INPUT_PULLUP);
    
    // Configure DHT22 pin
    pinMode(PIN_DHT22, INPUT_PULLUP);
    
    // Configure ADC pin
    pinMode(PIN_ADC_DCV_IN, INPUT);
    
    return true;
}

bool BoardConfig::initPeripherals() {
    // SPI and I2C will be initialized by their respective managers
    // Just verify pins are available
    
    // Check for pin conflicts
    if (PIN_SPI_CS_ATM == PIN_SPI_CS_W5500) {
        Serial.println(F("[BoardConfig] ERROR: SPI CS pin conflict"));
        return false;
    }
    
    return true;
}

bool BoardConfig::detectHardware() {
    bool allDetected = true;
    
    Serial.println(F("[BoardConfig] Detecting hardware..."));
    
    // Hardware detection will be done by individual drivers
    // This is a placeholder for future expansion
    
    Serial.println(F("[BoardConfig] Hardware detection complete"));
    
    return allDetected;
}

bool BoardConfig::validate() {
    if (!initialized) {
        return false;
    }
    
    bool valid = true;
    
    // Validate chip select pins are high (inactive)
    if (digitalRead(PIN_SPI_CS_ATM) != HIGH) {
        Serial.println(F("[BoardConfig] ERROR: ATM CS pin not high"));
        valid = false;
    }
    
    if (digitalRead(PIN_SPI_CS_W5500) != HIGH) {
        Serial.println(F("[BoardConfig] ERROR: W5500 CS pin not high"));
        valid = false;
    }
    
    return valid;
}

uint64_t BoardConfig::getChipID() const {
    return chipID;
}

void BoardConfig::getMacAddress(uint8_t* mac) const {
    if (mac != nullptr) {
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
    }
}

float BoardConfig::getChipTemperature() const {
    // ESP32 doesn't have a reliable internal temperature sensor
    // Return NAN to indicate unavailable
    return NAN;
}

void BoardConfig::printInfo() const {
    Serial.println(F("\n========================================"));
    Serial.println(F("  SM-GE3222M Board Information"));
    Serial.println(F("========================================"));
    
    Serial.print(F("Board Name      : "));
    Serial.println(boardName);
    
    Serial.print(F("Hardware Rev    : "));
    Serial.println(hwRevision);
    
    Serial.print(F("Chip ID         : 0x"));
    Serial.println((uint32_t)(chipID >> 32), HEX);
    Serial.print(F("                  0x"));
    Serial.println((uint32_t)(chipID & 0xFFFFFFFF), HEX);
    
    // Get chip info
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    
    Serial.print(F("Chip Model      : ESP32"));
    if (chip_info.model == CHIP_ESP32) {
        Serial.println(F(" (single core)"));
    } else {
        Serial.println();
    }
    
    Serial.print(F("CPU Cores       : "));
    Serial.println(chip_info.cores);
    
    Serial.print(F("CPU Frequency   : "));
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(F(" MHz"));
    
    Serial.print(F("Flash Size      : "));
    Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
    Serial.println(F(" MB"));
    
    Serial.print(F("Flash Speed     : "));
    Serial.print(ESP.getFlashChipSpeed() / 1000000);
    Serial.println(F(" MHz"));
    
    Serial.print(F("Free Heap       : "));
    Serial.print(ESP.getFreeHeap() / 1024);
    Serial.println(F(" KB"));
    
    uint8_t mac[6];
    getMacAddress(mac);
    Serial.print(F("MAC Address     : "));
    for (int i = 0; i < 6; i++) {
        if (mac[i] < 16) Serial.print('0');
        Serial.print(mac[i], HEX);
        if (i < 5) Serial.print(':');
    }
    Serial.println();
    
    Serial.println(F("========================================\n"));
}
