/**
 * @file ModbusTCPServer.cpp
 * @brief Modbus TCP server implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "ModbusTCPServer.h"
#include "../core/ErrorHandler.h"

// Singleton instance
ModbusTCPServer& ModbusTCPServer::getInstance() {
    static ModbusTCPServer instance;
    return instance;
}

// Constructor
ModbusTCPServer::ModbusTCPServer()
    : serverPort(DEFAULT_PORT),
      maxConnections(DEFAULT_MAX_CONNECTIONS),
      initialized(false),
      running(false),
      requestCount(0),
      errorCount(0),
      lastActivityTime(0) {
}

// Destructor
ModbusTCPServer::~ModbusTCPServer() {
    stop();
}

// Initialize Modbus TCP server
bool ModbusTCPServer::init(uint16_t port, uint8_t maxConn) {
    serverPort = port;
    maxConnections = maxConn;
    
    // Setup register callbacks
    setupRegisters();
    
    initialized = true;
    
    return true;
}

// Start Modbus TCP server
bool ModbusTCPServer::begin() {
    if (!initialized) {
        return false;
    }
    
    // Start Modbus TCP server on specified port
    modbusTCP.server(serverPort);
    
    running = true;
    lastActivityTime = millis();
    
    return true;
}

// Stop Modbus TCP server
void ModbusTCPServer::stop() {
    if (running) {
        running = false;
    }
}

// Main loop update
void ModbusTCPServer::update() {
    if (!initialized || !running) {
        return;
    }
    
    // Process Modbus TCP requests
    modbusTCP.task();
}

// Set server port
void ModbusTCPServer::setPort(uint16_t port) {
    serverPort = port;
    if (running) {
        stop();
        begin();
    }
}

// Set max connections
void ModbusTCPServer::setMaxConnections(uint8_t max) {
    maxConnections = max;
}

// Get active connections
uint8_t ModbusTCPServer::getActiveConnections() const {
    // ModbusTCP library doesn't expose active connection count
    // Would need to track this internally
    return 0;
}

// Setup Modbus registers
void ModbusTCPServer::setupRegisters() {
    // Add input registers for all measurements (read-only)
    // Energy registers
    modbusTCP.addIreg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAA_FWD_ACTIVE_ENERGY, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_FWD_ACTIVE_ENERGY, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_FWD_ACTIVE_ENERGY, 0, 2);
    
    // Power registers
    modbusTCP.addIreg(MODBUS_TOTAL_ACTIVE_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAA_ACTIVE_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_ACTIVE_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_ACTIVE_POWER, 0, 2);
    
    modbusTCP.addIreg(MODBUS_TOTAL_REACTIVE_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAA_REACTIVE_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_REACTIVE_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_REACTIVE_POWER, 0, 2);
    
    modbusTCP.addIreg(MODBUS_TOTAL_APPARENT_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAA_APPARENT_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_APPARENT_POWER, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_APPARENT_POWER, 0, 2);
    
    // Voltage and current
    modbusTCP.addIreg(MODBUS_PHAA_VOLTAGE_RMS, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_VOLTAGE_RMS, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_VOLTAGE_RMS, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAA_CURRENT_RMS, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_CURRENT_RMS, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_CURRENT_RMS, 0, 2);
    
    // Power factor
    modbusTCP.addIreg(MODBUS_TOTAL_POWER_FACTOR, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAA_POWER_FACTOR, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_POWER_FACTOR, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_POWER_FACTOR, 0, 2);
    
    // THD
    modbusTCP.addIreg(MODBUS_PHAA_VOLTAGE_THDN, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_VOLTAGE_THDN, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_VOLTAGE_THDN, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAA_CURRENT_THDN, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAB_CURRENT_THDN, 0, 2);
    modbusTCP.addIreg(MODBUS_PHAC_CURRENT_THDN, 0, 2);
    
    // Frequency and temperature
    modbusTCP.addIreg(MODBUS_FREQUENCY, 0, 2);
    modbusTCP.addIreg(MODBUS_MEASURED_TEMPERATURE, 0, 2);
    
    // Add holding registers for configuration (read/write)
    modbusTCP.addHreg(MODBUS_PROTOCOL_ADDRESS, 1);
    modbusTCP.addHreg(MODBUS_BAUD_RATE, 9600);
    modbusTCP.addHreg(MODBUS_PARITY, 0);
    
    // Calibration registers
    modbusTCP.addHreg(MODBUS_CAL_LINE_FREQ, 0, 2);
    modbusTCP.addHreg(MODBUS_CAL_PGA_GAIN, 0, 2);
    
    // Add coils for digital outputs (read/write)
    modbusTCP.addCoil(MODBUS_COIL_RELAY_0, false);
    modbusTCP.addCoil(MODBUS_COIL_RELAY_1, false);
    modbusTCP.addCoil(MODBUS_COIL_LED_0, false);
    modbusTCP.addCoil(MODBUS_COIL_LED_1, false);
    modbusTCP.addCoil(MODBUS_COIL_LED_2, false);
    modbusTCP.addCoil(MODBUS_COIL_LED_3, false);
    
    // Add discrete inputs for digital inputs (read-only)
    modbusTCP.addIsts(MODBUS_DI_MODE_SWITCH, false);
    modbusTCP.addIsts(MODBUS_DI_SET_SWITCH, false);
    
    // Set callbacks
    modbusTCP.onGetHreg(MODBUS_PROTOCOL_ADDRESS, cbReadHoldingRegister, 10);
    modbusTCP.onSetHreg(MODBUS_PROTOCOL_ADDRESS, cbWriteHoldingRegister, 10);
    modbusTCP.onGetIreg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY, cbReadInputRegister, 200);
    modbusTCP.onGetCoil(MODBUS_COIL_RELAY_0, cbReadCoil, 20);
    modbusTCP.onSetCoil(MODBUS_COIL_RELAY_0, cbWriteCoil, 20);
}

// Add holding register
bool ModbusTCPServer::addHoldingRegister(uint16_t addr, uint16_t value) {
    return modbusTCP.addHreg(addr, value);
}

// Add input register
bool ModbusTCPServer::addInputRegister(uint16_t addr, uint16_t value) {
    return modbusTCP.addIreg(addr, value);
}

// Add coil
bool ModbusTCPServer::addCoil(uint16_t addr, bool value) {
    return modbusTCP.addCoil(addr, value);
}

// Add discrete input
bool ModbusTCPServer::addDiscreteInput(uint16_t addr, bool value) {
    return modbusTCP.addIsts(addr, value);
}

// Update holding register
bool ModbusTCPServer::updateHoldingRegister(uint16_t addr, uint16_t value) {
    modbusTCP.Hreg(addr, value);
    return true;
}

// Update input register
bool ModbusTCPServer::updateInputRegister(uint16_t addr, uint16_t value) {
    modbusTCP.Ireg(addr, value);
    return true;
}

// Update coil
bool ModbusTCPServer::updateCoil(uint16_t addr, bool value) {
    modbusTCP.Coil(addr, value);
    return true;
}

// Update meter data in Modbus registers
bool ModbusTCPServer::updateMeterData(const MeterData& meterData) {
    if (!initialized) {
        return false;
    }
    
    uint16_t regHigh, regLow;
    
    // Update voltage registers
    floatToRegisters(meterData.phaseA.voltage, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_VOLTAGE_RMS, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_VOLTAGE_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseB.voltage, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_VOLTAGE_RMS, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_VOLTAGE_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseC.voltage, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_VOLTAGE_RMS, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_VOLTAGE_RMS + 1, regLow);
    
    // Update current registers
    floatToRegisters(meterData.phaseA.current, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_CURRENT_RMS, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_CURRENT_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseB.current, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_CURRENT_RMS, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_CURRENT_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseC.current, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_CURRENT_RMS, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_CURRENT_RMS + 1, regLow);
    
    // Update active power registers
    floatToRegisters(meterData.totalActivePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_TOTAL_ACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_TOTAL_ACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseA.activePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_ACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_ACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseB.activePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_ACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_ACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseC.activePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_ACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_ACTIVE_POWER + 1, regLow);
    
    // Update reactive power registers
    floatToRegisters(meterData.totalReactivePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_TOTAL_REACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_TOTAL_REACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseA.reactivePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_REACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_REACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseB.reactivePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_REACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_REACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseC.reactivePower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_REACTIVE_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_REACTIVE_POWER + 1, regLow);
    
    // Update apparent power registers
    floatToRegisters(meterData.totalApparentPower, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_TOTAL_APPARENT_POWER, regHigh);
    modbusTCP.Ireg(MODBUS_TOTAL_APPARENT_POWER + 1, regLow);
    
    // Update power factor registers
    floatToRegisters(meterData.totalPowerFactor, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_TOTAL_POWER_FACTOR, regHigh);
    modbusTCP.Ireg(MODBUS_TOTAL_POWER_FACTOR + 1, regLow);
    
    floatToRegisters(meterData.phaseA.powerFactor, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_POWER_FACTOR, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_POWER_FACTOR + 1, regLow);
    
    floatToRegisters(meterData.phaseB.powerFactor, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_POWER_FACTOR, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_POWER_FACTOR + 1, regLow);
    
    floatToRegisters(meterData.phaseC.powerFactor, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_POWER_FACTOR, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_POWER_FACTOR + 1, regLow);
    
    // Update energy registers (import)
    floatToRegisters(meterData.importActiveEnergy[0], &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY, regHigh);
    modbusTCP.Ireg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY + 1, regLow);
    
    floatToRegisters(meterData.importActiveEnergy[1], &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_FWD_ACTIVE_ENERGY, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_FWD_ACTIVE_ENERGY + 1, regLow);
    
    floatToRegisters(meterData.importActiveEnergy[2], &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_FWD_ACTIVE_ENERGY, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_FWD_ACTIVE_ENERGY + 1, regLow);
    
    floatToRegisters(meterData.importActiveEnergy[3], &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_FWD_ACTIVE_ENERGY, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_FWD_ACTIVE_ENERGY + 1, regLow);
    
    // Update THD registers
    floatToRegisters(meterData.phaseA.voltageTHDN, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_VOLTAGE_THDN, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_VOLTAGE_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseB.voltageTHDN, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_VOLTAGE_THDN, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_VOLTAGE_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseC.voltageTHDN, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_VOLTAGE_THDN, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_VOLTAGE_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseA.currentTHDN, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAA_CURRENT_THDN, regHigh);
    modbusTCP.Ireg(MODBUS_PHAA_CURRENT_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseB.currentTHDN, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAB_CURRENT_THDN, regHigh);
    modbusTCP.Ireg(MODBUS_PHAB_CURRENT_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseC.currentTHDN, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_PHAC_CURRENT_THDN, regHigh);
    modbusTCP.Ireg(MODBUS_PHAC_CURRENT_THDN + 1, regLow);
    
    // Update frequency and temperature
    floatToRegisters(meterData.frequency, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_FREQUENCY, regHigh);
    modbusTCP.Ireg(MODBUS_FREQUENCY + 1, regLow);
    
    floatToRegisters(meterData.temperature, &regHigh, &regLow);
    modbusTCP.Ireg(MODBUS_MEASURED_TEMPERATURE, regHigh);
    modbusTCP.Ireg(MODBUS_MEASURED_TEMPERATURE + 1, regLow);
    
    lastActivityTime = millis();
    
    return true;
}

// Update calibration data
bool ModbusTCPServer::updateCalibration(const CalibrationConfig& calibConfig) {
    if (!initialized) {
        return false;
    }
    
    // Update calibration holding registers
    modbusTCP.Hreg(MODBUS_CAL_LINE_FREQ, calibConfig.lineFreq);
    modbusTCP.Hreg(MODBUS_CAL_PGA_GAIN, calibConfig.pgaGain);
    
    modbusTCP.Hreg(MODBUS_CAL_VOLTAGE_GAIN_A, calibConfig.voltageGain[0]);
    modbusTCP.Hreg(MODBUS_CAL_VOLTAGE_GAIN_B, calibConfig.voltageGain[1]);
    modbusTCP.Hreg(MODBUS_CAL_VOLTAGE_GAIN_C, calibConfig.voltageGain[2]);
    
    modbusTCP.Hreg(MODBUS_CAL_CURRENT_GAIN_A, calibConfig.currentGain[0]);
    modbusTCP.Hreg(MODBUS_CAL_CURRENT_GAIN_B, calibConfig.currentGain[1]);
    modbusTCP.Hreg(MODBUS_CAL_CURRENT_GAIN_C, calibConfig.currentGain[2]);
    modbusTCP.Hreg(MODBUS_CAL_CURRENT_GAIN_N, calibConfig.currentGainN);
    
    return true;
}

// Convert float to two 16-bit registers (IEEE 754)
void ModbusTCPServer::floatToRegisters(float value, uint16_t* regHigh, uint16_t* regLow) {
    union {
        float f;
        uint32_t u;
    } converter;
    
    converter.f = value;
    *regHigh = (uint16_t)(converter.u >> 16);
    *regLow = (uint16_t)(converter.u & 0xFFFF);
}

// Convert two 16-bit registers to float
float ModbusTCPServer::registersToFloat(uint16_t regHigh, uint16_t regLow) {
    union {
        float f;
        uint32_t u;
    } converter;
    
    converter.u = ((uint32_t)regHigh << 16) | regLow;
    return converter.f;
}

// Modbus callbacks
uint16_t ModbusTCPServer::cbReadHoldingRegister(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    return val;
}

uint16_t ModbusTCPServer::cbWriteHoldingRegister(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    return val;
}

uint16_t ModbusTCPServer::cbReadInputRegister(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    return val;
}

bool ModbusTCPServer::cbReadCoil(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    return val != 0;
}

bool ModbusTCPServer::cbWriteCoil(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    
    // Handle coil writes (relay control, etc.)
    // TODO: Integrate with GPIO manager
    
    return val != 0;
}
