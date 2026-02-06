/**
 * @file ModbusRTUServer.cpp
 * @brief Modbus RTU server implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "ModbusRTUServer.h"
#include "../core/ErrorHandler.h"
#include "../hal/GPIOManager.h"

// Singleton instance
ModbusRTUServer& ModbusRTUServer::getInstance() {
    static ModbusRTUServer instance;
    return instance;
}

// Constructor
ModbusRTUServer::ModbusRTUServer()
    : slaveID(1),
      baudRate(9600),
      parity(0),
      initialized(false),
      requestCount(0),
      errorCount(0),
      lastActivityTime(0),
      lastLEDFlash(0) {
}

// Destructor
ModbusRTUServer::~ModbusRTUServer() {
    stop();
}

// Initialize Modbus RTU server
bool ModbusRTUServer::init(uint8_t id, uint32_t baud, uint8_t par) {
    slaveID = id;
    baudRate = baud;
    parity = par;
    
    // Validate slave ID (1-247)
    if (slaveID < 1 || slaveID > 247) {
        ErrorHandler::getInstance().setError(ErrorCode::MODBUS_TIMEOUT, "Invalid Modbus slave ID");
        return false;
    }
    
    // Configure UART2 for RS-485
    Serial2.begin(baudRate, SERIAL_8N1, PIN_UART2_RX, PIN_UART2_TX);
    
    // Configure DE/RE pin for RS-485 transceiver
    pinMode(PIN_RS485_DE, OUTPUT);
    digitalWrite(PIN_RS485_DE, LOW);  // Receive mode by default
    
    // Initialize Modbus RTU
    modbusRTU.begin(&Serial2, PIN_RS485_DE);
    modbusRTU.slave(slaveID);
    
    // Setup register callbacks
    setupRegisters();
    
    initialized = true;
    lastActivityTime = millis();
    
    return true;
}

// Start Modbus RTU server
bool ModbusRTUServer::begin() {
    if (!initialized) {
        return false;
    }
    
    return true;
}

// Stop Modbus RTU server
void ModbusRTUServer::stop() {
    if (initialized) {
        Serial2.end();
        initialized = false;
    }
}

// Main loop update
void ModbusRTUServer::update() {
    if (!initialized) {
        return;
    }
    
    // Process Modbus requests
    modbusRTU.task();
    
    // Check for communication timeout
    if (millis() - lastActivityTime > MODBUS_TIMEOUT) {
        // Timeout detected - could log or handle
    }
    
    // Turn off LED after flash duration
    if (lastLEDFlash > 0 && millis() - lastLEDFlash > LED_FLASH_DURATION) {
        // Turn off Modbus LED via GPIO manager
        lastLEDFlash = 0;
    }
}

// Set slave ID
void ModbusRTUServer::setSlaveID(uint8_t id) {
    if (id >= 1 && id <= 247) {
        slaveID = id;
        if (initialized) {
            modbusRTU.slave(slaveID);
        }
    }
}

// Set baud rate
void ModbusRTUServer::setBaudRate(uint32_t rate) {
    baudRate = rate;
    if (initialized) {
        Serial2.updateBaudRate(baudRate);
    }
}

// Set parity
void ModbusRTUServer::setParity(uint8_t par) {
    parity = par;
    // Would need to reconfigure Serial2 with new parity
}

// Setup Modbus registers
void ModbusRTUServer::setupRegisters() {
    // Add input registers for all measurements (read-only)
    // Energy registers
    modbusRTU.addIreg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAA_FWD_ACTIVE_ENERGY, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_FWD_ACTIVE_ENERGY, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_FWD_ACTIVE_ENERGY, 0, 2);
    
    // Power registers
    modbusRTU.addIreg(MODBUS_TOTAL_ACTIVE_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAA_ACTIVE_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_ACTIVE_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_ACTIVE_POWER, 0, 2);
    
    modbusRTU.addIreg(MODBUS_TOTAL_REACTIVE_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAA_REACTIVE_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_REACTIVE_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_REACTIVE_POWER, 0, 2);
    
    modbusRTU.addIreg(MODBUS_TOTAL_APPARENT_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAA_APPARENT_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_APPARENT_POWER, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_APPARENT_POWER, 0, 2);
    
    // Voltage and current
    modbusRTU.addIreg(MODBUS_PHAA_VOLTAGE_RMS, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_VOLTAGE_RMS, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_VOLTAGE_RMS, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAA_CURRENT_RMS, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_CURRENT_RMS, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_CURRENT_RMS, 0, 2);
    
    // Power factor
    modbusRTU.addIreg(MODBUS_TOTAL_POWER_FACTOR, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAA_POWER_FACTOR, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_POWER_FACTOR, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_POWER_FACTOR, 0, 2);
    
    // THD
    modbusRTU.addIreg(MODBUS_PHAA_VOLTAGE_THDN, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_VOLTAGE_THDN, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_VOLTAGE_THDN, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAA_CURRENT_THDN, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAB_CURRENT_THDN, 0, 2);
    modbusRTU.addIreg(MODBUS_PHAC_CURRENT_THDN, 0, 2);
    
    // Frequency and temperature
    modbusRTU.addIreg(MODBUS_FREQUENCY, 0, 2);
    modbusRTU.addIreg(MODBUS_MEASURED_TEMPERATURE, 0, 2);
    
    // Add holding registers for configuration (read/write)
    modbusRTU.addHreg(MODBUS_PROTOCOL_ADDRESS, slaveID);
    modbusRTU.addHreg(MODBUS_BAUD_RATE, 9600);
    modbusRTU.addHreg(MODBUS_PARITY, 0);
    
    // Calibration registers
    modbusRTU.addHreg(MODBUS_CAL_LINE_FREQ, 0, 2);
    modbusRTU.addHreg(MODBUS_CAL_PGA_GAIN, 0, 2);
    
    // Add coils for digital outputs (read/write)
    modbusRTU.addCoil(MODBUS_COIL_RELAY_0, false);
    modbusRTU.addCoil(MODBUS_COIL_RELAY_1, false);
    modbusRTU.addCoil(MODBUS_COIL_LED_0, false);
    modbusRTU.addCoil(MODBUS_COIL_LED_1, false);
    modbusRTU.addCoil(MODBUS_COIL_LED_2, false);
    modbusRTU.addCoil(MODBUS_COIL_LED_3, false);
    
    // Add discrete inputs for digital inputs (read-only)
    modbusRTU.addIsts(MODBUS_DI_MODE_SWITCH, false);
    modbusRTU.addIsts(MODBUS_DI_SET_SWITCH, false);
    
    // Set callbacks
    modbusRTU.onGetHreg(MODBUS_PROTOCOL_ADDRESS, cbReadHoldingRegister, 10);
    modbusRTU.onSetHreg(MODBUS_PROTOCOL_ADDRESS, cbWriteHoldingRegister, 10);
    modbusRTU.onGetIreg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY, cbReadInputRegister, 200);
}

// Add holding register
bool ModbusRTUServer::addHoldingRegister(uint16_t addr, uint16_t value) {
    return modbusRTU.addHreg(addr, value);
}

// Add input register
bool ModbusRTUServer::addInputRegister(uint16_t addr, uint16_t value) {
    return modbusRTU.addIreg(addr, value);
}

// Add coil
bool ModbusRTUServer::addCoil(uint16_t addr, bool value) {
    return modbusRTU.addCoil(addr, value);
}

// Add discrete input
bool ModbusRTUServer::addDiscreteInput(uint16_t addr, bool value) {
    return modbusRTU.addIsts(addr, value);
}

// Update holding register
bool ModbusRTUServer::updateHoldingRegister(uint16_t addr, uint16_t value) {
    modbusRTU.Hreg(addr, value);
    return true;
}

// Update input register
bool ModbusRTUServer::updateInputRegister(uint16_t addr, uint16_t value) {
    modbusRTU.Ireg(addr, value);
    return true;
}

// Update coil
bool ModbusRTUServer::updateCoil(uint16_t addr, bool value) {
    modbusRTU.Coil(addr, value);
    return true;
}

// Update meter data in Modbus registers
bool ModbusRTUServer::updateMeterData(const MeterData& meterData) {
    if (!initialized) {
        return false;
    }
    
    uint16_t regHigh, regLow;
    
    // Update voltage registers
    floatToRegisters(meterData.phaseA.voltage, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_VOLTAGE_RMS, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_VOLTAGE_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseB.voltage, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_VOLTAGE_RMS, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_VOLTAGE_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseC.voltage, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_VOLTAGE_RMS, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_VOLTAGE_RMS + 1, regLow);
    
    // Update current registers
    floatToRegisters(meterData.phaseA.current, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_CURRENT_RMS, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_CURRENT_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseB.current, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_CURRENT_RMS, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_CURRENT_RMS + 1, regLow);
    
    floatToRegisters(meterData.phaseC.current, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_CURRENT_RMS, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_CURRENT_RMS + 1, regLow);
    
    // Update active power registers
    floatToRegisters(meterData.totalActivePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_TOTAL_ACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_TOTAL_ACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseA.activePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_ACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_ACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseB.activePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_ACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_ACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseC.activePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_ACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_ACTIVE_POWER + 1, regLow);
    
    // Update reactive power registers
    floatToRegisters(meterData.totalReactivePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_TOTAL_REACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_TOTAL_REACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseA.reactivePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_REACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_REACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseB.reactivePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_REACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_REACTIVE_POWER + 1, regLow);
    
    floatToRegisters(meterData.phaseC.reactivePower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_REACTIVE_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_REACTIVE_POWER + 1, regLow);
    
    // Update apparent power registers
    floatToRegisters(meterData.totalApparentPower, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_TOTAL_APPARENT_POWER, regHigh);
    modbusRTU.Ireg(MODBUS_TOTAL_APPARENT_POWER + 1, regLow);
    
    // Update power factor registers
    floatToRegisters(meterData.totalPowerFactor, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_TOTAL_POWER_FACTOR, regHigh);
    modbusRTU.Ireg(MODBUS_TOTAL_POWER_FACTOR + 1, regLow);
    
    floatToRegisters(meterData.phaseA.powerFactor, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_POWER_FACTOR, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_POWER_FACTOR + 1, regLow);
    
    floatToRegisters(meterData.phaseB.powerFactor, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_POWER_FACTOR, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_POWER_FACTOR + 1, regLow);
    
    floatToRegisters(meterData.phaseC.powerFactor, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_POWER_FACTOR, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_POWER_FACTOR + 1, regLow);
    
    // Update energy registers (import)
    floatToRegisters(meterData.importActiveEnergy[0], &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY, regHigh);
    modbusRTU.Ireg(MODBUS_TOTAL_FWD_ACTIVE_ENERGY + 1, regLow);
    
    floatToRegisters(meterData.importActiveEnergy[1], &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_FWD_ACTIVE_ENERGY, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_FWD_ACTIVE_ENERGY + 1, regLow);
    
    floatToRegisters(meterData.importActiveEnergy[2], &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_FWD_ACTIVE_ENERGY, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_FWD_ACTIVE_ENERGY + 1, regLow);
    
    floatToRegisters(meterData.importActiveEnergy[3], &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_FWD_ACTIVE_ENERGY, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_FWD_ACTIVE_ENERGY + 1, regLow);
    
    // Update THD registers
    floatToRegisters(meterData.phaseA.voltageTHDN, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_VOLTAGE_THDN, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_VOLTAGE_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseB.voltageTHDN, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_VOLTAGE_THDN, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_VOLTAGE_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseC.voltageTHDN, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_VOLTAGE_THDN, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_VOLTAGE_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseA.currentTHDN, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAA_CURRENT_THDN, regHigh);
    modbusRTU.Ireg(MODBUS_PHAA_CURRENT_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseB.currentTHDN, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAB_CURRENT_THDN, regHigh);
    modbusRTU.Ireg(MODBUS_PHAB_CURRENT_THDN + 1, regLow);
    
    floatToRegisters(meterData.phaseC.currentTHDN, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_PHAC_CURRENT_THDN, regHigh);
    modbusRTU.Ireg(MODBUS_PHAC_CURRENT_THDN + 1, regLow);
    
    // Update frequency and temperature
    floatToRegisters(meterData.frequency, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_FREQUENCY, regHigh);
    modbusRTU.Ireg(MODBUS_FREQUENCY + 1, regLow);
    
    floatToRegisters(meterData.temperature, &regHigh, &regLow);
    modbusRTU.Ireg(MODBUS_MEASURED_TEMPERATURE, regHigh);
    modbusRTU.Ireg(MODBUS_MEASURED_TEMPERATURE + 1, regLow);
    
    lastActivityTime = millis();
    flashModbusLED();
    
    return true;
}

// Update calibration data
bool ModbusRTUServer::updateCalibration(const CalibrationConfig& calibConfig) {
    if (!initialized) {
        return false;
    }
    
    // Update calibration holding registers
    modbusRTU.Hreg(MODBUS_CAL_LINE_FREQ, calibConfig.lineFreq);
    modbusRTU.Hreg(MODBUS_CAL_PGA_GAIN, calibConfig.pgaGain);
    
    modbusRTU.Hreg(MODBUS_CAL_VOLTAGE_GAIN_A, calibConfig.voltageGain[0]);
    modbusRTU.Hreg(MODBUS_CAL_VOLTAGE_GAIN_B, calibConfig.voltageGain[1]);
    modbusRTU.Hreg(MODBUS_CAL_VOLTAGE_GAIN_C, calibConfig.voltageGain[2]);
    
    modbusRTU.Hreg(MODBUS_CAL_CURRENT_GAIN_A, calibConfig.currentGain[0]);
    modbusRTU.Hreg(MODBUS_CAL_CURRENT_GAIN_B, calibConfig.currentGain[1]);
    modbusRTU.Hreg(MODBUS_CAL_CURRENT_GAIN_C, calibConfig.currentGain[2]);
    modbusRTU.Hreg(MODBUS_CAL_CURRENT_GAIN_N, calibConfig.currentGainN);
    
    return true;
}

// Convert float to two 16-bit registers (IEEE 754)
void ModbusRTUServer::floatToRegisters(float value, uint16_t* regHigh, uint16_t* regLow) {
    union {
        float f;
        uint32_t u;
    } converter;
    
    converter.f = value;
    *regHigh = (uint16_t)(converter.u >> 16);
    *regLow = (uint16_t)(converter.u & 0xFFFF);
}

// Convert two 16-bit registers to float
float ModbusRTUServer::registersToFloat(uint16_t regHigh, uint16_t regLow) {
    union {
        float f;
        uint32_t u;
    } converter;
    
    converter.u = ((uint32_t)regHigh << 16) | regLow;
    return converter.f;
}

// Flash Modbus activity LED
void ModbusRTUServer::flashModbusLED() {
    // Flash LED via GPIO manager
    lastLEDFlash = millis();
    // GPIOManager::getInstance().setLED(MCP_MODBUS_LED, true);
}

// Modbus callbacks
uint16_t ModbusRTUServer::cbReadHoldingRegister(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    getInstance().flashModbusLED();
    return val;
}

uint16_t ModbusRTUServer::cbWriteHoldingRegister(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    getInstance().flashModbusLED();
    return val;
}

uint16_t ModbusRTUServer::cbReadInputRegister(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    getInstance().flashModbusLED();
    return val;
}

bool ModbusRTUServer::cbReadCoil(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    getInstance().flashModbusLED();
    return val != 0;
}

bool ModbusRTUServer::cbWriteCoil(TRegister* reg, uint16_t val) {
    getInstance().requestCount++;
    getInstance().lastActivityTime = millis();
    getInstance().flashModbusLED();
    
    // Handle coil writes (relay control, etc.)
    // TODO: Integrate with GPIO manager
    
    return val != 0;
}
