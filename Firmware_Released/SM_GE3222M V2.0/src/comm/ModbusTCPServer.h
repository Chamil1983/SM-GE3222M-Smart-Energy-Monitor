/**
 * @file ModbusTCPServer.h
 * @brief Modbus TCP server implementation over Ethernet/WiFi
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Implements Modbus TCP protocol on port 502
 */

#pragma once

#include <Arduino.h>
#include <ModbusTCP.h>
#include "../../include/GlobalTypes.h"
#include "../../include/ModbusRegMap.h"

class ModbusTCPServer {
public:
    // Singleton instance
    static ModbusTCPServer& getInstance();
    
    // Initialization
    bool init(uint16_t port = 502, uint8_t maxConnections = 4);
    bool begin();
    void stop();
    
    // Main loop update
    void update();
    
    // Configuration
    void setPort(uint16_t port);
    void setMaxConnections(uint8_t max);
    uint16_t getPort() const { return serverPort; }
    uint8_t getMaxConnections() const { return maxConnections; }
    
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
    bool isRunning() const { return running; }
    uint8_t getActiveConnections() const;
    uint32_t getRequestCount() const { return requestCount; }
    uint32_t getErrorCount() const { return errorCount; }
    uint32_t getLastActivityTime() const { return lastActivityTime; }
    
private:
    ModbusTCPServer();
    ~ModbusTCPServer();
    ModbusTCPServer(const ModbusTCPServer&) = delete;
    ModbusTCPServer& operator=(const ModbusTCPServer&) = delete;
    
    // Internal methods
    void setupRegisters();
    void floatToRegisters(float value, uint16_t* regHigh, uint16_t* regLow);
    float registersToFloat(uint16_t regHigh, uint16_t regLow);
    
    // Modbus callbacks
    static uint16_t cbReadHoldingRegister(TRegister* reg, uint16_t val);
    static uint16_t cbWriteHoldingRegister(TRegister* reg, uint16_t val);
    static uint16_t cbReadInputRegister(TRegister* reg, uint16_t val);
    static bool cbReadCoil(TRegister* reg, uint16_t val);
    static bool cbWriteCoil(TRegister* reg, uint16_t val);
    
    // Member variables
    ModbusTCP modbusTCP;
    
    uint16_t serverPort;
    uint8_t maxConnections;
    
    bool initialized;
    bool running;
    uint32_t requestCount;
    uint32_t errorCount;
    uint32_t lastActivityTime;
    
    // Constants
    static constexpr uint16_t DEFAULT_PORT = 502;
    static constexpr uint8_t DEFAULT_MAX_CONNECTIONS = 4;
};
