/**
 * @file ModbusServer.cpp
 * @brief Implementation of unified Modbus RTU/TCP server
 */

#include "ModbusServer.h"
#include "PinMap.h"
#include <cstring>

ModbusServer& ModbusServer::getInstance() {
    static ModbusServer instance;
    return instance;
}

ModbusServer::ModbusServer() 
    : _tcpServer(nullptr),
      _initialized(false),
      _rtuEnabled(false),
      _tcpEnabled(false),
      _lastRTUActivity(0),
      _lastTCPActivity(0) {
    memset(_inputRegisters, 0, sizeof(_inputRegisters));
    memset(_holdingRegisters, 0, sizeof(_holdingRegisters));
    memset(_coils, 0, sizeof(_coils));
    memset(_discreteInputs, 0, sizeof(_discreteInputs));
}

ModbusServer::~ModbusServer() {
    if (_tcpServer) {
        delete _tcpServer;
        _tcpServer = nullptr;
    }
}

bool ModbusServer::begin(const ModbusConfig& config) {
    _config = config;
    
    Logger::getInstance().info("ModbusServer: Starting (RTU=%d, TCP=%d, SlaveID=%d, Baud=%d)",
        config.rtuEnabled, config.tcpEnabled, config.slaveID, config.baudrate);
    
    setupRegisters();
    
    if (config.rtuEnabled) {
        Serial2.begin(config.baudrate, SERIAL_8N1, PIN_MODBUS_RX, PIN_MODBUS_TX);
        
        _modbusRTU.begin(&Serial2, PIN_MODBUS_DE);
        _modbusRTU.setBaudrate(config.baudrate);
        _modbusRTU.slave(config.slaveID);
        
        _modbusRTU.onSetCoil([this](TRegister* reg, uint16_t val) -> uint16_t {
            writeCoil(reg->address.address, val != 0);
            return val;
        });
        
        _modbusRTU.onGetCoil([this](TRegister* reg, uint16_t val) -> uint16_t {
            return readCoil(reg->address.address) ? 1 : 0;
        });
        
        _modbusRTU.onGetHreg([this](TRegister* reg, uint16_t val) -> uint16_t {
            return readHoldingRegister(reg->address.address);
        });
        
        _modbusRTU.onSetHreg([this](TRegister* reg, uint16_t val) -> uint16_t {
            writeHoldingRegister(reg->address.address, val);
            return val;
        });
        
        _modbusRTU.onGetIreg([this](TRegister* reg, uint16_t val) -> uint16_t {
            return readInputRegister(reg->address.address);
        });
        
        _modbusRTU.onGetIsts([this](TRegister* reg, uint16_t val) -> uint16_t {
            return readDiscreteInput(reg->address.address) ? 1 : 0;
        });
        
        _rtuEnabled = true;
        Logger::getInstance().info("ModbusServer: RTU started on Serial2 (baud=%d)", config.baudrate);
    }
    
    if (config.tcpEnabled) {
        _tcpServer = new WiFiServer(config.tcpPort);
        _tcpServer->begin();
        _tcpEnabled = true;
        Logger::getInstance().info("ModbusServer: TCP started on port %d", config.tcpPort);
    }
    
    _initialized = true;
    return true;
}

void ModbusServer::setupRegisters() {
    for (uint16_t i = 0; i < MB_INPUT_REG_COUNT; i++) {
        _modbusRTU.addIreg(i);
    }
    
    for (uint16_t i = 0; i < MB_HOLDING_REG_COUNT; i++) {
        _modbusRTU.addHreg(i);
    }
    
    for (uint16_t i = 0; i < MB_COIL_COUNT; i++) {
        _modbusRTU.addCoil(i);
    }
    
    for (uint16_t i = 0; i < MB_DISCRETE_INPUT_COUNT; i++) {
        _modbusRTU.addIsts(i);
    }
}

void ModbusServer::handle() {
    if (!_initialized) return;
    
    if (_rtuEnabled) {
        handleRTU();
    }
    
    if (_tcpEnabled) {
        handleTCP();
    }
}

void ModbusServer::handleRTU() {
    _modbusRTU.task();
}

void ModbusServer::handleTCP() {
    if (!_tcpServer) return;
    
    if (_tcpServer->hasClient()) {
        for (int i = 0; i < MAX_TCP_CLIENTS; i++) {
            if (!_tcpClients[i] || !_tcpClients[i].connected()) {
                if (_tcpClients[i]) _tcpClients[i].stop();
                _tcpClients[i] = _tcpServer->available();
                Logger::getInstance().info("ModbusServer: TCP client %d connected", i);
                break;
            }
        }
    }
    
    for (int i = 0; i < MAX_TCP_CLIENTS; i++) {
        if (_tcpClients[i] && _tcpClients[i].connected()) {
            if (_tcpClients[i].available()) {
                _lastTCPActivity = millis();
            }
        }
    }
}

void ModbusServer::updateMeterData(const MeterData& data) {
    _meterData = data;
    
    uint16_t highWord, lowWord;
    
    float2registers(data.phaseA.voltageRMS, highWord, lowWord);
    _inputRegisters[MB_URMS_A] = highWord;
    _inputRegisters[MB_URMS_A + 1] = lowWord;
    
    float2registers(data.phaseB.voltageRMS, highWord, lowWord);
    _inputRegisters[MB_URMS_B] = highWord;
    _inputRegisters[MB_URMS_B + 1] = lowWord;
    
    float2registers(data.phaseC.voltageRMS, highWord, lowWord);
    _inputRegisters[MB_URMS_C] = highWord;
    _inputRegisters[MB_URMS_C + 1] = lowWord;
    
    float2registers(data.phaseA.currentRMS, highWord, lowWord);
    _inputRegisters[MB_IRMS_A] = highWord;
    _inputRegisters[MB_IRMS_A + 1] = lowWord;
    
    float2registers(data.phaseB.currentRMS, highWord, lowWord);
    _inputRegisters[MB_IRMS_B] = highWord;
    _inputRegisters[MB_IRMS_B + 1] = lowWord;
    
    float2registers(data.phaseC.currentRMS, highWord, lowWord);
    _inputRegisters[MB_IRMS_C] = highWord;
    _inputRegisters[MB_IRMS_C + 1] = lowWord;
    
    float2registers(data.phaseA.activePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_A] = highWord;
    _inputRegisters[MB_ACTIVE_POWER_A + 1] = lowWord;
    
    float2registers(data.phaseB.activePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_B] = highWord;
    _inputRegisters[MB_ACTIVE_POWER_B + 1] = lowWord;
    
    float2registers(data.phaseC.activePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_C] = highWord;
    _inputRegisters[MB_ACTIVE_POWER_C + 1] = lowWord;
    
    float2registers(data.totalActivePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_T] = highWord;
    _inputRegisters[MB_ACTIVE_POWER_T + 1] = lowWord;
    
    float2registers(data.phaseA.reactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_A] = highWord;
    _inputRegisters[MB_REACTIVE_POWER_A + 1] = lowWord;
    
    float2registers(data.phaseB.reactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_B] = highWord;
    _inputRegisters[MB_REACTIVE_POWER_B + 1] = lowWord;
    
    float2registers(data.phaseC.reactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_C] = highWord;
    _inputRegisters[MB_REACTIVE_POWER_C + 1] = lowWord;
    
    float2registers(data.totalReactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_T] = highWord;
    _inputRegisters[MB_REACTIVE_POWER_T + 1] = lowWord;
    
    float2registers(data.phaseA.apparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_A] = highWord;
    _inputRegisters[MB_APPARENT_POWER_A + 1] = lowWord;
    
    float2registers(data.phaseB.apparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_B] = highWord;
    _inputRegisters[MB_APPARENT_POWER_B + 1] = lowWord;
    
    float2registers(data.phaseC.apparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_C] = highWord;
    _inputRegisters[MB_APPARENT_POWER_C + 1] = lowWord;
    
    float2registers(data.totalApparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_T] = highWord;
    _inputRegisters[MB_APPARENT_POWER_T + 1] = lowWord;
    
    float2registers(data.phaseA.powerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_A] = highWord;
    _inputRegisters[MB_POWER_FACTOR_A + 1] = lowWord;
    
    float2registers(data.phaseB.powerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_B] = highWord;
    _inputRegisters[MB_POWER_FACTOR_B + 1] = lowWord;
    
    float2registers(data.phaseC.powerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_C] = highWord;
    _inputRegisters[MB_POWER_FACTOR_C + 1] = lowWord;
    
    float2registers(data.totalPowerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_T] = highWord;
    _inputRegisters[MB_POWER_FACTOR_T + 1] = lowWord;
    
    float2registers(data.phaseA.meanPhaseAngle, highWord, lowWord);
    _inputRegisters[MB_PHASE_ANGLE_A] = highWord;
    _inputRegisters[MB_PHASE_ANGLE_A + 1] = lowWord;
    
    float2registers(data.phaseB.meanPhaseAngle, highWord, lowWord);
    _inputRegisters[MB_PHASE_ANGLE_B] = highWord;
    _inputRegisters[MB_PHASE_ANGLE_B + 1] = lowWord;
    
    float2registers(data.phaseC.meanPhaseAngle, highWord, lowWord);
    _inputRegisters[MB_PHASE_ANGLE_C] = highWord;
    _inputRegisters[MB_PHASE_ANGLE_C + 1] = lowWord;
    
    float2registers(data.phaseA.voltageTHDN, highWord, lowWord);
    _inputRegisters[MB_VOLTAGE_THD_A] = highWord;
    _inputRegisters[MB_VOLTAGE_THD_A + 1] = lowWord;
    
    float2registers(data.phaseB.voltageTHDN, highWord, lowWord);
    _inputRegisters[MB_VOLTAGE_THD_B] = highWord;
    _inputRegisters[MB_VOLTAGE_THD_B + 1] = lowWord;
    
    float2registers(data.phaseC.voltageTHDN, highWord, lowWord);
    _inputRegisters[MB_VOLTAGE_THD_C] = highWord;
    _inputRegisters[MB_VOLTAGE_THD_C + 1] = lowWord;
    
    float2registers(data.phaseA.currentTHDN, highWord, lowWord);
    _inputRegisters[MB_CURRENT_THD_A] = highWord;
    _inputRegisters[MB_CURRENT_THD_A + 1] = lowWord;
    
    float2registers(data.phaseB.currentTHDN, highWord, lowWord);
    _inputRegisters[MB_CURRENT_THD_B] = highWord;
    _inputRegisters[MB_CURRENT_THD_B + 1] = lowWord;
    
    float2registers(data.phaseC.currentTHDN, highWord, lowWord);
    _inputRegisters[MB_CURRENT_THD_C] = highWord;
    _inputRegisters[MB_CURRENT_THD_C + 1] = lowWord;
    
    float2registers(data.frequency, highWord, lowWord);
    _inputRegisters[MB_FREQUENCY] = highWord;
    _inputRegisters[MB_FREQUENCY + 1] = lowWord;
    
    float2registers(data.neutralCurrent, highWord, lowWord);
    _inputRegisters[MB_NEUTRAL_CURRENT] = highWord;
    _inputRegisters[MB_NEUTRAL_CURRENT + 1] = lowWord;
    
    float2registers(data.phaseA.fwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_A] = highWord;
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_A + 1] = lowWord;
    
    float2registers(data.phaseB.fwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_B] = highWord;
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_B + 1] = lowWord;
    
    float2registers(data.phaseC.fwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_C] = highWord;
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_C + 1] = lowWord;
    
    float2registers(data.totalFwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_T] = highWord;
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_T + 1] = lowWord;
    
    float2registers(data.phaseA.revActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_A] = highWord;
    _inputRegisters[MB_REV_ACTIVE_ENERGY_A + 1] = lowWord;
    
    float2registers(data.phaseB.revActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_B] = highWord;
    _inputRegisters[MB_REV_ACTIVE_ENERGY_B + 1] = lowWord;
    
    float2registers(data.phaseC.revActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_C] = highWord;
    _inputRegisters[MB_REV_ACTIVE_ENERGY_C + 1] = lowWord;
    
    float2registers(data.totalRevActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_T] = highWord;
    _inputRegisters[MB_REV_ACTIVE_ENERGY_T + 1] = lowWord;
    
    float2registers(data.phaseA.fwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_A] = highWord;
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_A + 1] = lowWord;
    
    float2registers(data.phaseB.fwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_B] = highWord;
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_B + 1] = lowWord;
    
    float2registers(data.phaseC.fwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_C] = highWord;
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_C + 1] = lowWord;
    
    float2registers(data.totalFwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_T] = highWord;
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_T + 1] = lowWord;
    
    float2registers(data.phaseA.revReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_A] = highWord;
    _inputRegisters[MB_REV_REACTIVE_ENERGY_A + 1] = lowWord;
    
    float2registers(data.phaseB.revReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_B] = highWord;
    _inputRegisters[MB_REV_REACTIVE_ENERGY_B + 1] = lowWord;
    
    float2registers(data.phaseC.revReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_C] = highWord;
    _inputRegisters[MB_REV_REACTIVE_ENERGY_C + 1] = lowWord;
    
    float2registers(data.totalRevReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_T] = highWord;
    _inputRegisters[MB_REV_REACTIVE_ENERGY_T + 1] = lowWord;
    
    float2registers(data.phaseA.apparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_A] = highWord;
    _inputRegisters[MB_APPARENT_ENERGY_A + 1] = lowWord;
    
    float2registers(data.phaseB.apparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_B] = highWord;
    _inputRegisters[MB_APPARENT_ENERGY_B + 1] = lowWord;
    
    float2registers(data.phaseC.apparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_C] = highWord;
    _inputRegisters[MB_APPARENT_ENERGY_C + 1] = lowWord;
    
    float2registers(data.totalApparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_T] = highWord;
    _inputRegisters[MB_APPARENT_ENERGY_T + 1] = lowWord;
    
    float2registers(data.phaseA.fundamentalPower, highWord, lowWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_A] = highWord;
    _inputRegisters[MB_FUNDAMENTAL_POWER_A + 1] = lowWord;
    
    float2registers(data.phaseB.fundamentalPower, highWord, lowWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_B] = highWord;
    _inputRegisters[MB_FUNDAMENTAL_POWER_B + 1] = lowWord;
    
    float2registers(data.phaseC.fundamentalPower, highWord, lowWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_C] = highWord;
    _inputRegisters[MB_FUNDAMENTAL_POWER_C + 1] = lowWord;
    
    float2registers(data.phaseA.harmonicPower, highWord, lowWord);
    _inputRegisters[MB_HARMONIC_POWER_A] = highWord;
    _inputRegisters[MB_HARMONIC_POWER_A + 1] = lowWord;
    
    float2registers(data.phaseB.harmonicPower, highWord, lowWord);
    _inputRegisters[MB_HARMONIC_POWER_B] = highWord;
    _inputRegisters[MB_HARMONIC_POWER_B + 1] = lowWord;
    
    float2registers(data.phaseC.harmonicPower, highWord, lowWord);
    _inputRegisters[MB_HARMONIC_POWER_C] = highWord;
    _inputRegisters[MB_HARMONIC_POWER_C + 1] = lowWord;
    
    float2registers(data.boardTemperature, highWord, lowWord);
    _inputRegisters[MB_BOARD_TEMP] = highWord;
    _inputRegisters[MB_BOARD_TEMP + 1] = lowWord;
    
    float2registers(data.ambientTemperature, highWord, lowWord);
    _inputRegisters[MB_AMBIENT_TEMP] = highWord;
    _inputRegisters[MB_AMBIENT_TEMP + 1] = lowWord;
    
    float2registers(data.ambientHumidity, highWord, lowWord);
    _inputRegisters[MB_AMBIENT_HUMIDITY] = highWord;
    _inputRegisters[MB_AMBIENT_HUMIDITY + 1] = lowWord;
}

void ModbusServer::updateSystemStatus(const SystemStatus& status) {
    _systemStatus = status;
    
    _inputRegisters[MB_UPTIME_SECONDS] = (status.uptime >> 16) & 0xFFFF;
    _inputRegisters[MB_UPTIME_SECONDS + 1] = status.uptime & 0xFFFF;
    
    _inputRegisters[MB_ERROR_COUNT] = status.errorCount;
    _inputRegisters[MB_FIRMWARE_VERSION] = 0x0200;
    
    _inputRegisters[MB_FREE_HEAP] = (status.freeHeap >> 16) & 0xFFFF;
    _inputRegisters[MB_FREE_HEAP + 1] = status.freeHeap & 0xFFFF;
    
    uint16_t statusFlags = 0;
    if (status.wifiConnected) statusFlags |= STATUS_FLAG_WIFI_CONNECTED;
    if (status.mqttConnected) statusFlags |= STATUS_FLAG_MQTT_CONNECTED;
    if (status.modbusActive) statusFlags |= STATUS_FLAG_MODBUS_ACTIVE;
    _inputRegisters[MB_STATUS_FLAGS] = statusFlags;
    
    _discreteInputs[MB_DI_WIFI_CONNECTED] = status.wifiConnected;
    _discreteInputs[MB_DI_MQTT_CONNECTED] = status.mqttConnected;
}

void ModbusServer::float2registers(float value, uint16_t& highWord, uint16_t& lowWord) {
    union {
        float f;
        uint32_t u32;
    } converter;
    
    converter.f = value;
    highWord = (converter.u32 >> 16) & 0xFFFF;
    lowWord = converter.u32 & 0xFFFF;
}

float ModbusServer::registers2float(uint16_t highWord, uint16_t lowWord) {
    union {
        float f;
        uint32_t u32;
    } converter;
    
    converter.u32 = ((uint32_t)highWord << 16) | lowWord;
    return converter.f;
}

uint16_t ModbusServer::readInputRegister(uint16_t address) {
    if (address < MB_INPUT_REG_COUNT) {
        return _inputRegisters[address];
    }
    return 0;
}

uint16_t ModbusServer::readHoldingRegister(uint16_t address) {
    if (address < MB_HOLDING_REG_COUNT) {
        return _holdingRegisters[address];
    }
    return 0;
}

void ModbusServer::writeHoldingRegister(uint16_t address, uint16_t value) {
    if (address < MB_HOLDING_REG_COUNT) {
        _holdingRegisters[address] = value;
        
        if (address == MB_HOLD_MODBUS_SLAVEID) {
            _config.slaveID = value;
            _modbusRTU.slave(value);
            Logger::getInstance().info("ModbusServer: Slave ID changed to %d", value);
        }
    }
}

bool ModbusServer::readCoil(uint16_t address) {
    if (address < MB_COIL_COUNT) {
        return _coils[address];
    }
    return false;
}

void ModbusServer::writeCoil(uint16_t address, bool state) {
    if (address < MB_COIL_COUNT) {
        _coils[address] = state;
    }
}

bool ModbusServer::readDiscreteInput(uint16_t address) {
    if (address < MB_DISCRETE_INPUT_COUNT) {
        return _discreteInputs[address];
    }
    return false;
}

void ModbusServer::setCoil(uint16_t address, bool state) {
    writeCoil(address, state);
}

bool ModbusServer::getCoil(uint16_t address) {
    return readCoil(address);
}

void ModbusServer::setDiscreteInput(uint16_t address, bool state) {
    if (address < MB_DISCRETE_INPUT_COUNT) {
        _discreteInputs[address] = state;
    }
}
