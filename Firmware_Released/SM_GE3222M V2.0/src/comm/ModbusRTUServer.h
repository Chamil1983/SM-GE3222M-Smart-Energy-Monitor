/**
 * @file ModbusRTUServer.h
 * @brief Modbus RTU server implementation over RS-485
 *
 * SM-GE3222M Smart Energy Monitor V2.0
 * Implements Modbus RTU slave protocol on UART2 with RS-485 transceiver
 */

#pragma once

#include <Arduino.h>
#include <ModbusRTU.h>
#include "../../include/GlobalTypes.h"
#include "../../include/ModbusRegMap.h"
#include "../../include/PinMap.h"

class ModbusRTUServer {
public:
    // Singleton instance
    static ModbusRTUServer& getInstance();

    // Initialization
    bool init(uint8_t slaveID = 1, uint32_t baudRate = 9600, uint8_t parity = 0);
    bool begin();
    void stop();

    // Main loop update
    void update();

    // Configuration
    void setSlaveID(uint8_t id);
    void setBaudRate(uint32_t rate);
    void setParity(uint8_t parity);
    uint8_t getSlaveID() const { return slaveID; }
    uint32_t getBaudRate() const { return baudRate; }

    // Register management
    bool addHoldingRegister(uint16_t addr, uint16_t value);
    bool addInputRegister(uint16_t addr, uint16_t value);
    bool addCoil(uint16_t addr, bool value);
    bool addDiscreteInput(uint16_t addr, bool value);

    bool updateHoldingRegister(uint16_t addr, uint16_t value);
    bool updateInputRegister(uint16_t addr, uint16_t value);
    bool updateCoil(uint16_t addr, bool value);

    // High-level data updates
    bool updateMeterData(const MeterData& meterData);
    bool updateCalibration(const CalibrationConfig& calibConfig);

    // Status
    bool isInitialized() const { return initialized; }
    uint32_t getRequestCount() const { return requestCount; }
    uint32_t getErrorCount() const { return errorCount; }
    uint32_t getLastActivityTime() const { return lastActivityTime; }

private:
    ModbusRTUServer();
    ~ModbusRTUServer();
    ModbusRTUServer(const ModbusRTUServer&) = delete;
    ModbusRTUServer& operator=(const ModbusRTUServer&) = delete;

    // Internal methods
    void setupRegisters();
    void floatToRegisters(float value, uint16_t* regHigh, uint16_t* regLow);
    float registersToFloat(uint16_t regHigh, uint16_t regLow);
    void flashModbusLED();

    // Modbus callbacks
    static uint16_t cbReadHoldingRegister(TRegister* reg, uint16_t val);
    static uint16_t cbWriteHoldingRegister(TRegister* reg, uint16_t val);
    static uint16_t cbReadInputRegister(TRegister* reg, uint16_t val);
    static bool cbReadCoil(TRegister* reg, uint16_t val);
    static bool cbWriteCoil(TRegister* reg, uint16_t val);

    // Member variables
    ModbusRTU modbusRTU;

    uint8_t slaveID;
    uint32_t baudRate;
    uint8_t parity;

    bool initialized;
    uint32_t requestCount;
    uint32_t errorCount;
    uint32_t lastActivityTime;
    uint32_t lastLEDFlash;

    // Constants
    static constexpr uint32_t MODBUS_TIMEOUT = 5000;  // 5 seconds
    static constexpr uint32_t LED_FLASH_DURATION = 50; // 50ms
};