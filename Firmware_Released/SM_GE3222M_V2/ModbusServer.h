/**
 * @file ModbusServer.h
 * @brief Unified Modbus RTU/TCP server for SM-GE3222M V2.0
 * 
 * Consolidates Modbus Serial (RTU) and Modbus TCP functionality from V1.0
 * into a single, unified implementation using singleton pattern.
 * Supports IEEE754 float encoding for meter data.
 * NOTE: Modbus TCP is disabled in this firmware (RTU-only).
 */

#ifndef MODBUSSERVER_H
#define MODBUSSERVER_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <ModbusRTU.h>
#include "DataTypes.h"
#include "ModbusMap.h"
#include "Logger.h"

class ModbusServer {
public:
    static ModbusServer& getInstance();
    
    bool begin(const ModbusConfig& config);
    void handle();
    void updateMeterData(const MeterData& data);
    void updateSystemStatus(const SystemStatus& status);
    
    void setCoil(uint16_t address, bool state);
    bool getCoil(uint16_t address);
    void setDiscreteInput(uint16_t address, bool state);
    
private:
    ModbusServer();
    ~ModbusServer();
    ModbusServer(const ModbusServer&) = delete;
    ModbusServer& operator=(const ModbusServer&) = delete;
    
    void setupRegisters();
    void handleRTU();
    void handleTCP();
    
    void float2registers(float value, uint16_t& highWord, uint16_t& lowWord);
    float registers2float(uint16_t highWord, uint16_t lowWord);
    
    uint16_t readInputRegister(uint16_t address);
    uint16_t readHoldingRegister(uint16_t address);
    void writeHoldingRegister(uint16_t address, uint16_t value);
    bool readCoil(uint16_t address);
    void writeCoil(uint16_t address, bool state);
    bool readDiscreteInput(uint16_t address);
    
    ModbusRTU _modbusRTU;
    WiFiServer* _tcpServer;
    WiFiClient _tcpClients[4];
    
    ModbusConfig _config;
    bool _initialized;
    bool _rtuEnabled;
    bool _tcpEnabled;
    
    uint16_t _inputRegisters[MB_INPUT_REG_COUNT];
    uint16_t _holdingRegisters[MB_HOLDING_REG_COUNT];
    bool _coils[MB_COIL_COUNT];
    bool _discreteInputs[MB_DISCRETE_INPUT_COUNT];
    
    MeterData _meterData;
    SystemStatus _systemStatus;
    
    unsigned long _lastRTUActivity;
    unsigned long _lastTCPActivity;
    
    static constexpr uint16_t TCP_TIMEOUT_MS = 30000;
    static constexpr uint8_t MAX_TCP_CLIENTS = 4;
};

#endif // MODBUSSERVER_H
