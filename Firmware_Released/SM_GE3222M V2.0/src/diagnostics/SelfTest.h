/**
 * @file SelfTest.h
 * @brief Power-on self-test (POST) system
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Comprehensive hardware and software verification
 */

#pragma once

#include <Arduino.h>
#include "../../include/GlobalTypes.h"

/**
 * @struct SelfTestResult
 * @brief Individual test result
 */
struct SelfTestResult {
    bool passed;
    char testName[32];
    char message[64];
    
    SelfTestResult() : passed(false) {
        testName[0] = '\0';
        message[0] = '\0';
    }
};

/**
 * @struct SelfTestResults
 * @brief Complete self-test results
 */
struct SelfTestResults {
    bool allPassed;
    uint8_t testCount;
    uint8_t passedCount;
    uint8_t failedCount;
    SelfTestResult i2cTest;
    SelfTestResult spiTest;
    SelfTestResult lcdTest;
    SelfTestResult mcpTest;
    SelfTestResult atmTest;
    SelfTestResult spiffsTest;
    SelfTestResult nvsTest;
    SelfTestResult wifiTest;
    unsigned long testDuration;
    
    SelfTestResults() : allPassed(false), testCount(0), passedCount(0), 
                        failedCount(0), testDuration(0) {}
};

/**
 * @class SelfTest
 * @brief Power-on self-test system
 * 
 * Singleton class that performs comprehensive hardware and software tests:
 * - I2C bus scan and device detection
 * - SPI device communication tests
 * - LCD display test
 * - MCP23017 I/O expander test
 * - ATM90E36 energy meter test
 * - SPIFFS filesystem test
 * - NVS storage test
 * - WiFi hardware test
 * 
 * Results are displayed on LCD and logged
 */
class SelfTest {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to SelfTest instance
     */
    static SelfTest& getInstance();
    
    /**
     * @brief Run complete self-test suite
     * @return true if all tests passed
     */
    bool run();
    
    /**
     * @brief Test I2C bus and scan for devices
     * @return true if I2C bus is operational
     */
    bool testI2C();
    
    /**
     * @brief Test SPI devices (ATM90E36, W5500)
     * @return true if SPI bus is operational
     */
    bool testSPI();
    
    /**
     * @brief Test LCD display
     * @return true if LCD responds
     */
    bool testLCD();
    
    /**
     * @brief Test MCP23017 I/O expander
     * @return true if MCP23017 responds
     */
    bool testMCP23017();
    
    /**
     * @brief Test ATM90E36 energy meter
     * @return true if ATM90E36 responds
     */
    bool testATM90E36();
    
    /**
     * @brief Test SPIFFS filesystem
     * @return true if SPIFFS is mounted and has space
     */
    bool testSPIFFS();
    
    /**
     * @brief Test NVS partition
     * @return true if NVS is accessible
     */
    bool testNVS();
    
    /**
     * @brief Test WiFi hardware
     * @return true if WiFi hardware is present
     */
    bool testWiFi();
    
    /**
     * @brief Get test results
     * @return Reference to SelfTestResults structure
     */
    const SelfTestResults& getResults() const { return results; }
    
    /**
     * @brief Get results as formatted string
     * @param buffer Output buffer
     * @param bufSize Buffer size
     */
    void getResultsString(char* buffer, size_t bufSize);
    
    /**
     * @brief Check if all tests passed
     * @return true if all tests passed
     */
    bool passed() const { return results.allPassed; }
    
    /**
     * @brief Get test duration
     * @return Duration in milliseconds
     */
    unsigned long getDuration() const { return results.testDuration; }

private:
    // Private constructor for singleton
    SelfTest();
    
    // Delete copy constructor and assignment operator
    SelfTest(const SelfTest&) = delete;
    SelfTest& operator=(const SelfTest&) = delete;
    
    /**
     * @brief Record test result
     * @param result Result structure to update
     * @param testName Test name
     * @param passed Pass/fail status
     * @param message Result message
     */
    void recordResult(SelfTestResult& result, const char* testName, 
                     bool passed, const char* message);
    
    /**
     * @brief Display test progress on LCD
     * @param testName Test being performed
     * @param status Status message
     */
    void displayProgress(const char* testName, const char* status);
    
    // Results
    SelfTestResults results;
    
    // I2C scan results
    uint8_t i2cDevicesFound;
    uint8_t i2cAddresses[8];
};

#endif // SELFTEST_H
