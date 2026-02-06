/**
 * @file SelfTest.cpp
 * @brief Power-on self-test implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "SelfTest.h"
#include "Logger.h"
#include "../display/LCDManager.h"
#include "../hal/I2CBus.h"
#include "../hal/SPIBus.h"
#include "../hal/GPIOManager.h"
#include "../../include/PinMap.h"
#include "../../include/RegisterMap.h"
#include <Wire.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <WiFi.h>

SelfTest::SelfTest() : i2cDevicesFound(0) {
    memset(i2cAddresses, 0, sizeof(i2cAddresses));
}

SelfTest& SelfTest::getInstance() {
    static SelfTest instance;
    return instance;
}

bool SelfTest::run() {
    LOG_INFO("SelfTest", "Starting power-on self-test...");
    unsigned long startTime = millis();
    
    // Reset results
    results = SelfTestResults();
    results.testCount = 8;
    
    // Run tests
    displayProgress("POST", "Starting...");
    delay(500);
    
    // Test I2C
    displayProgress("I2C Bus", "Testing...");
    bool i2cOk = testI2C();
    delay(300);
    
    // Test MCP23017
    displayProgress("MCP23017", "Testing...");
    bool mcpOk = testMCP23017();
    delay(300);
    
    // Test LCD
    displayProgress("LCD", "Testing...");
    bool lcdOk = testLCD();
    delay(300);
    
    // Test SPI
    displayProgress("SPI Bus", "Testing...");
    bool spiOk = testSPI();
    delay(300);
    
    // Test ATM90E36
    displayProgress("ATM90E36", "Testing...");
    bool atmOk = testATM90E36();
    delay(300);
    
    // Test SPIFFS
    displayProgress("SPIFFS", "Testing...");
    bool spiffsOk = testSPIFFS();
    delay(300);
    
    // Test NVS
    displayProgress("NVS", "Testing...");
    bool nvsOk = testNVS();
    delay(300);
    
    // Test WiFi
    displayProgress("WiFi HW", "Testing...");
    bool wifiOk = testWiFi();
    delay(300);
    
    // Calculate results
    results.testDuration = millis() - startTime;
    results.allPassed = (results.passedCount == results.testCount);
    
    // Display final result
    if (results.allPassed) {
        displayProgress("POST", "PASSED!");
        LOG_INFO("SelfTest", "All tests passed in %lu ms", results.testDuration);
    } else {
        displayProgress("POST", "FAILED!");
        LOG_ERROR("SelfTest", "Tests failed: %d/%d passed", 
                 results.passedCount, results.testCount);
    }
    delay(1000);
    
    return results.allPassed;
}

bool SelfTest::testI2C() {
    i2cDevicesFound = 0;
    
    // Scan I2C bus
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            if (i2cDevicesFound < 8) {
                i2cAddresses[i2cDevicesFound++] = addr;
            }
        }
    }
    
    // I2C is OK if we found at least 1 device
    bool passed = (i2cDevicesFound > 0);
    char msg[64];
    snprintf(msg, sizeof(msg), "Found %d I2C devices", i2cDevicesFound);
    recordResult(results.i2cTest, "I2C Bus", passed, msg);
    
    LOG_INFO("SelfTest", "I2C: %s", msg);
    return passed;
}

bool SelfTest::testSPI() {
    // Initialize SPI
    SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
    
    // Basic SPI initialization is always successful on ESP32
    // Actual device tests are done separately
    recordResult(results.spiTest, "SPI Bus", true, "SPI initialized");
    LOG_INFO("SelfTest", "SPI: Bus initialized");
    return true;
}

bool SelfTest::testLCD() {
    // Check if LCD address was found in I2C scan
    bool found = false;
    for (uint8_t i = 0; i < i2cDevicesFound; i++) {
        if (i2cAddresses[i] == I2C_ADDR_LCD) {
            found = true;
            break;
        }
    }
    
    if (found) {
        recordResult(results.lcdTest, "LCD", true, "LCD detected");
        LOG_INFO("SelfTest", "LCD: Detected at 0x%02X", I2C_ADDR_LCD);
    } else {
        recordResult(results.lcdTest, "LCD", false, "LCD not found");
        LOG_ERROR("SelfTest", "LCD: Not found at 0x%02X", I2C_ADDR_LCD);
    }
    
    return found;
}

bool SelfTest::testMCP23017() {
    // Check if MCP23017 address was found in I2C scan
    bool found = false;
    for (uint8_t i = 0; i < i2cDevicesFound; i++) {
        if (i2cAddresses[i] == I2C_ADDR_MCP23017) {
            found = true;
            break;
        }
    }
    
    if (found) {
        recordResult(results.mcpTest, "MCP23017", true, "MCP23017 detected");
        LOG_INFO("SelfTest", "MCP23017: Detected at 0x%02X", I2C_ADDR_MCP23017);
    } else {
        recordResult(results.mcpTest, "MCP23017", false, "MCP not found");
        LOG_ERROR("SelfTest", "MCP23017: Not found at 0x%02X", I2C_ADDR_MCP23017);
    }
    
    return found;
}

bool SelfTest::testATM90E36() {
    // Try to read SysStatus0 register from ATM90E36
    pinMode(PIN_SPI_CS_ATM, OUTPUT);
    digitalWrite(PIN_SPI_CS_ATM, HIGH);
    
    SPI.beginTransaction(SPISettings(SPI_FREQ_ATM, MSBFIRST, SPI_MODE3));
    digitalWrite(PIN_SPI_CS_ATM, LOW);
    
    // Read SysStatus0 register (0x01)
    SPI.transfer16(0x8001); // Read command for register 0x01
    uint16_t status = SPI.transfer16(0x0000);
    
    digitalWrite(PIN_SPI_CS_ATM, HIGH);
    SPI.endTransaction();
    
    // Check if we got a reasonable response (not 0x0000 or 0xFFFF)
    bool passed = (status != 0x0000 && status != 0xFFFF);
    
    if (passed) {
        char msg[64];
        snprintf(msg, sizeof(msg), "ATM status: 0x%04X", status);
        recordResult(results.atmTest, "ATM90E36", true, msg);
        LOG_INFO("SelfTest", "ATM90E36: Communication OK (0x%04X)", status);
    } else {
        recordResult(results.atmTest, "ATM90E36", false, "No response");
        LOG_ERROR("SelfTest", "ATM90E36: No valid response");
    }
    
    return passed;
}

bool SelfTest::testSPIFFS() {
    // Try to mount SPIFFS
    bool mounted = SPIFFS.begin(true);
    
    if (mounted) {
        size_t totalBytes = SPIFFS.totalBytes();
        size_t usedBytes = SPIFFS.usedBytes();
        size_t freeBytes = totalBytes - usedBytes;
        
        char msg[64];
        snprintf(msg, sizeof(msg), "Free: %lu KB", freeBytes / 1024);
        recordResult(results.spiffsTest, "SPIFFS", true, msg);
        LOG_INFO("SelfTest", "SPIFFS: Mounted, %lu KB free", freeBytes / 1024);
    } else {
        recordResult(results.spiffsTest, "SPIFFS", false, "Mount failed");
        LOG_ERROR("SelfTest", "SPIFFS: Mount failed");
    }
    
    return mounted;
}

bool SelfTest::testNVS() {
    // Try to initialize NVS
    esp_err_t err = nvs_flash_init();
    
    bool passed = (err == ESP_OK || err == ESP_ERR_NVS_NO_FREE_PAGES || 
                   err == ESP_ERR_NVS_NEW_VERSION_FOUND);
    
    if (passed) {
        recordResult(results.nvsTest, "NVS", true, "NVS initialized");
        LOG_INFO("SelfTest", "NVS: Initialized");
    } else {
        recordResult(results.nvsTest, "NVS", false, "NVS init failed");
        LOG_ERROR("SelfTest", "NVS: Init failed");
    }
    
    return passed;
}

bool SelfTest::testWiFi() {
    // Check WiFi hardware presence
    WiFi.mode(WIFI_MODE_NULL);
    delay(100);
    
    // If we can read MAC address, WiFi hardware is present
    uint8_t mac[6];
    esp_err_t err = esp_wifi_get_mac(WIFI_IF_STA, mac);
    bool passed = (err == ESP_OK);
    
    if (passed) {
        char msg[64];
        snprintf(msg, sizeof(msg), "MAC: %02X:%02X:...", mac[0], mac[1]);
        recordResult(results.wifiTest, "WiFi HW", true, msg);
        LOG_INFO("SelfTest", "WiFi: Hardware present");
    } else {
        recordResult(results.wifiTest, "WiFi HW", false, "No WiFi HW");
        LOG_ERROR("SelfTest", "WiFi: Hardware not detected");
    }
    
    return passed;
}

void SelfTest::recordResult(SelfTestResult& result, const char* testName, 
                            bool passed, const char* message) {
    result.passed = passed;
    strncpy(result.testName, testName, sizeof(result.testName) - 1);
    result.testName[sizeof(result.testName) - 1] = '\0';
    strncpy(result.message, message, sizeof(result.message) - 1);
    result.message[sizeof(result.message) - 1] = '\0';
    
    if (passed) {
        results.passedCount++;
    } else {
        results.failedCount++;
    }
}

void SelfTest::displayProgress(const char* testName, const char* status) {
    LCDManager& lcd = LCDManager::getInstance();
    
    char line[21];
    snprintf(line, sizeof(line), "%-20s", testName);
    lcd.displayMessage(2, line);
    
    snprintf(line, sizeof(line), "%-20s", status);
    lcd.displayMessage(3, line);
}

void SelfTest::getResultsString(char* buffer, size_t bufSize) {
    snprintf(buffer, bufSize, 
             "Self-Test: %d/%d passed in %lu ms\n"
             "I2C: %s - %s\n"
             "SPI: %s - %s\n"
             "LCD: %s - %s\n"
             "MCP: %s - %s\n"
             "ATM: %s - %s\n"
             "SPIFFS: %s - %s\n"
             "NVS: %s - %s\n"
             "WiFi: %s - %s",
             results.passedCount, results.testCount, results.testDuration,
             results.i2cTest.passed ? "PASS" : "FAIL", results.i2cTest.message,
             results.spiTest.passed ? "PASS" : "FAIL", results.spiTest.message,
             results.lcdTest.passed ? "PASS" : "FAIL", results.lcdTest.message,
             results.mcpTest.passed ? "PASS" : "FAIL", results.mcpTest.message,
             results.atmTest.passed ? "PASS" : "FAIL", results.atmTest.message,
             results.spiffsTest.passed ? "PASS" : "FAIL", results.spiffsTest.message,
             results.nvsTest.passed ? "PASS" : "FAIL", results.nvsTest.message,
             results.wifiTest.passed ? "PASS" : "FAIL", results.wifiTest.message);
}
