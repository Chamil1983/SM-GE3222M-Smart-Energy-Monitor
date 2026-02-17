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
    _config.tcpEnabled = false; // enforce RTU-only

    
    const bool rtuEnabled = config.rtuEnabled;
    const bool tcpEnabled = false; // Force Modbus TCP disabled (RTU-only requirement)
    Logger::getInstance().info("ModbusServer: Starting (RTU=%d, TCP=%d, SlaveID=%d, Baud=%d)",
        rtuEnabled, tcpEnabled, config.slaveID, config.baudrate);
    
    // NOTE: Do NOT add registers before ModbusRTU.begin()/slave() to avoid abort in library.

    if (rtuEnabled) {
        Serial2.begin(config.baudrate, SERIAL_8N1, PIN_MODBUS_RX, PIN_MODBUS_TX);
        Logger::getInstance().info("ModbusServer: Serial2.begin OK (RX=%d TX=%d DE=%d)", PIN_MODBUS_RX, PIN_MODBUS_TX, PIN_MODBUS_DE);

        
        _modbusRTU.begin(&Serial2, PIN_MODBUS_DE);
        Logger::getInstance().info("ModbusServer: ModbusRTU.begin OK");

        _modbusRTU.setBaudrate(config.baudrate);
        _modbusRTU.slave(config.slaveID);
        Logger::getInstance().info("ModbusServer: ModbusRTU.slave OK (id=%d)", config.slaveID);


        // Add register map AFTER ModbusRTU is initialized.
        Logger::getInstance().info("ModbusServer: Adding register map...");
        setupRegisters();
        // Callbacks disabled (SAFE MODE)
        // On ESP32, the emelianov modbus-esp8266 callbacks can trigger abort() in some builds
        // during registration. We avoid callbacks entirely and use the library's internal register
        // store (Hreg/Ireg/Coil/Ists). Meter/system updates write directly into those registers.
        Logger::getInstance().info("ModbusServer: Register map added; callbacks disabled (SAFE MODE)");

        _rtuEnabled = true;

        Logger::getInstance().info("ModbusServer: RTU started on Serial2 (baud=%d)", config.baudrate);
    }
    
    if (tcpEnabled) {
        _tcpServer = new WiFiServer(config.tcpPort);
        _tcpServer->begin();
        _tcpEnabled = true;
        Logger::getInstance().info("ModbusServer: TCP started on port %d", config.tcpPort);
    }
    
    _initialized = true;
    return true;
}


void ModbusServer::setupRegisters() {
    // IMPORTANT:
    // The emelianov ModbusRTU library requires that callback address ranges map to
    // registers that have been added. If the callback range is larger than the
    // created map, the library can assert/abort() during callback registration.
    //
    // Therefore we add the full configured ranges defined in ModbusMap.h.
#if defined(ARDUINO_ARCH_ESP32)
    const uint32_t heapBefore = ESP.getFreeHeap();
    Logger::getInstance().info("ModbusServer: Heap before map: %u bytes", (unsigned)heapBefore);
#endif

    for (uint16_t i = 0; i < MB_INPUT_REG_COUNT; i++) {
        _modbusRTU.addIreg(i);
        _modbusRTU.Ireg(i, 0);
        if ((i % 64) == 0) delay(0); // yield
    }

    for (uint16_t i = 0; i < MB_HOLDING_REG_COUNT; i++) {
        _modbusRTU.addHreg(i);
        _modbusRTU.Hreg(i, 0);
        if ((i % 32) == 0) delay(0); // yield
    }

    for (uint16_t i = 0; i < MB_COIL_COUNT; i++) {
        _modbusRTU.addCoil(i);
        _modbusRTU.Coil(i, false);
    }

    for (uint16_t i = 0; i < MB_DISCRETE_INPUT_COUNT; i++) {
        _modbusRTU.addIsts(i);
        _modbusRTU.Ists(i, false);
    }

#if defined(ARDUINO_ARCH_ESP32)
    const uint32_t heapAfter = ESP.getFreeHeap();
    Logger::getInstance().info("ModbusServer: Heap after map: %u bytes (delta=%d)",
                               (unsigned)heapAfter, (int)((int32_t)heapBefore - (int32_t)heapAfter));
#endif
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

    const bool mbReady = _rtuEnabled;
    uint16_t highWord, lowWord;
    
    float2registers(data.phaseA.voltageRMS, highWord, lowWord);
    _inputRegisters[MB_URMS_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_URMS_A, highWord);
    _inputRegisters[MB_URMS_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_URMS_A + 1, lowWord);
    
    float2registers(data.phaseB.voltageRMS, highWord, lowWord);
    _inputRegisters[MB_URMS_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_URMS_B, highWord);
    _inputRegisters[MB_URMS_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_URMS_B + 1, lowWord);
    
    float2registers(data.phaseC.voltageRMS, highWord, lowWord);
    _inputRegisters[MB_URMS_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_URMS_C, highWord);
    _inputRegisters[MB_URMS_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_URMS_C + 1, lowWord);
    
    float2registers(data.phaseA.currentRMS, highWord, lowWord);
    _inputRegisters[MB_IRMS_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_IRMS_A, highWord);
    _inputRegisters[MB_IRMS_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_IRMS_A + 1, lowWord);
    
    float2registers(data.phaseB.currentRMS, highWord, lowWord);
    _inputRegisters[MB_IRMS_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_IRMS_B, highWord);
    _inputRegisters[MB_IRMS_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_IRMS_B + 1, lowWord);
    
    float2registers(data.phaseC.currentRMS, highWord, lowWord);
    _inputRegisters[MB_IRMS_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_IRMS_C, highWord);
    _inputRegisters[MB_IRMS_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_IRMS_C + 1, lowWord);
    
    float2registers(data.phaseA.activePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_A, highWord);
    _inputRegisters[MB_ACTIVE_POWER_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_A + 1, lowWord);
    
    float2registers(data.phaseB.activePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_B, highWord);
    _inputRegisters[MB_ACTIVE_POWER_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_B + 1, lowWord);
    
    float2registers(data.phaseC.activePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_C, highWord);
    _inputRegisters[MB_ACTIVE_POWER_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_C + 1, lowWord);
    
    float2registers(data.totalActivePower, highWord, lowWord);
    _inputRegisters[MB_ACTIVE_POWER_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_T, highWord);
    _inputRegisters[MB_ACTIVE_POWER_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_ACTIVE_POWER_T + 1, lowWord);
    
    float2registers(data.phaseA.reactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_A, highWord);
    _inputRegisters[MB_REACTIVE_POWER_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_A + 1, lowWord);
    
    float2registers(data.phaseB.reactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_B, highWord);
    _inputRegisters[MB_REACTIVE_POWER_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_B + 1, lowWord);
    
    float2registers(data.phaseC.reactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_C, highWord);
    _inputRegisters[MB_REACTIVE_POWER_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_C + 1, lowWord);
    
    float2registers(data.totalReactivePower, highWord, lowWord);
    _inputRegisters[MB_REACTIVE_POWER_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_T, highWord);
    _inputRegisters[MB_REACTIVE_POWER_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REACTIVE_POWER_T + 1, lowWord);
    
    float2registers(data.phaseA.apparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_A, highWord);
    _inputRegisters[MB_APPARENT_POWER_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_A + 1, lowWord);
    
    float2registers(data.phaseB.apparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_B, highWord);
    _inputRegisters[MB_APPARENT_POWER_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_B + 1, lowWord);
    
    float2registers(data.phaseC.apparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_C, highWord);
    _inputRegisters[MB_APPARENT_POWER_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_C + 1, lowWord);
    
    float2registers(data.totalApparentPower, highWord, lowWord);
    _inputRegisters[MB_APPARENT_POWER_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_T, highWord);
    _inputRegisters[MB_APPARENT_POWER_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_POWER_T + 1, lowWord);
    
    float2registers(data.phaseA.powerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_A, highWord);
    _inputRegisters[MB_POWER_FACTOR_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_A + 1, lowWord);
    
    float2registers(data.phaseB.powerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_B, highWord);
    _inputRegisters[MB_POWER_FACTOR_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_B + 1, lowWord);
    
    float2registers(data.phaseC.powerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_C, highWord);
    _inputRegisters[MB_POWER_FACTOR_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_C + 1, lowWord);
    
    float2registers(data.totalPowerFactor, highWord, lowWord);
    _inputRegisters[MB_POWER_FACTOR_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_T, highWord);
    _inputRegisters[MB_POWER_FACTOR_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_POWER_FACTOR_T + 1, lowWord);
    
    float2registers(data.phaseA.meanPhaseAngle, highWord, lowWord);
    _inputRegisters[MB_PHASE_ANGLE_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_PHASE_ANGLE_A, highWord);
    _inputRegisters[MB_PHASE_ANGLE_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_PHASE_ANGLE_A + 1, lowWord);
    
    float2registers(data.phaseB.meanPhaseAngle, highWord, lowWord);
    _inputRegisters[MB_PHASE_ANGLE_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_PHASE_ANGLE_B, highWord);
    _inputRegisters[MB_PHASE_ANGLE_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_PHASE_ANGLE_B + 1, lowWord);
    
    float2registers(data.phaseC.meanPhaseAngle, highWord, lowWord);
    _inputRegisters[MB_PHASE_ANGLE_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_PHASE_ANGLE_C, highWord);
    _inputRegisters[MB_PHASE_ANGLE_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_PHASE_ANGLE_C + 1, lowWord);
    
    float2registers(data.phaseA.voltageTHDN, highWord, lowWord);
    _inputRegisters[MB_VOLTAGE_THD_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_VOLTAGE_THD_A, highWord);
    _inputRegisters[MB_VOLTAGE_THD_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_VOLTAGE_THD_A + 1, lowWord);
    
    float2registers(data.phaseB.voltageTHDN, highWord, lowWord);
    _inputRegisters[MB_VOLTAGE_THD_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_VOLTAGE_THD_B, highWord);
    _inputRegisters[MB_VOLTAGE_THD_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_VOLTAGE_THD_B + 1, lowWord);
    
    float2registers(data.phaseC.voltageTHDN, highWord, lowWord);
    _inputRegisters[MB_VOLTAGE_THD_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_VOLTAGE_THD_C, highWord);
    _inputRegisters[MB_VOLTAGE_THD_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_VOLTAGE_THD_C + 1, lowWord);
    
    float2registers(data.phaseA.currentTHDN, highWord, lowWord);
    _inputRegisters[MB_CURRENT_THD_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_CURRENT_THD_A, highWord);
    _inputRegisters[MB_CURRENT_THD_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_CURRENT_THD_A + 1, lowWord);
    
    float2registers(data.phaseB.currentTHDN, highWord, lowWord);
    _inputRegisters[MB_CURRENT_THD_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_CURRENT_THD_B, highWord);
    _inputRegisters[MB_CURRENT_THD_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_CURRENT_THD_B + 1, lowWord);
    
    float2registers(data.phaseC.currentTHDN, highWord, lowWord);
    _inputRegisters[MB_CURRENT_THD_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_CURRENT_THD_C, highWord);
    _inputRegisters[MB_CURRENT_THD_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_CURRENT_THD_C + 1, lowWord);
    
    float2registers(data.frequency, highWord, lowWord);
    _inputRegisters[MB_FREQUENCY] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FREQUENCY, highWord);
    _inputRegisters[MB_FREQUENCY + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FREQUENCY + 1, lowWord);
    
    float2registers(data.neutralCurrent, highWord, lowWord);
    _inputRegisters[MB_NEUTRAL_CURRENT] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_NEUTRAL_CURRENT, highWord);
    _inputRegisters[MB_NEUTRAL_CURRENT + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_NEUTRAL_CURRENT + 1, lowWord);
    
    float2registers(data.phaseA.fwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_A, highWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_A + 1, lowWord);
    
    float2registers(data.phaseB.fwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_B, highWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_B + 1, lowWord);
    
    float2registers(data.phaseC.fwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_C, highWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_C + 1, lowWord);
    
    float2registers(data.totalFwdActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_T, highWord);
    _inputRegisters[MB_FWD_ACTIVE_ENERGY_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_ACTIVE_ENERGY_T + 1, lowWord);
    
    float2registers(data.phaseA.revActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_A, highWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_A + 1, lowWord);
    
    float2registers(data.phaseB.revActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_B, highWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_B + 1, lowWord);
    
    float2registers(data.phaseC.revActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_C, highWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_C + 1, lowWord);
    
    float2registers(data.totalRevActiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_T, highWord);
    _inputRegisters[MB_REV_ACTIVE_ENERGY_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_ACTIVE_ENERGY_T + 1, lowWord);
    
    float2registers(data.phaseA.fwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_A, highWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_A + 1, lowWord);
    
    float2registers(data.phaseB.fwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_B, highWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_B + 1, lowWord);
    
    float2registers(data.phaseC.fwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_C, highWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_C + 1, lowWord);
    
    float2registers(data.totalFwdReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_T, highWord);
    _inputRegisters[MB_FWD_REACTIVE_ENERGY_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FWD_REACTIVE_ENERGY_T + 1, lowWord);
    
    float2registers(data.phaseA.revReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_A, highWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_A + 1, lowWord);
    
    float2registers(data.phaseB.revReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_B, highWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_B + 1, lowWord);
    
    float2registers(data.phaseC.revReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_C, highWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_C + 1, lowWord);
    
    float2registers(data.totalRevReactiveEnergy, highWord, lowWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_T, highWord);
    _inputRegisters[MB_REV_REACTIVE_ENERGY_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_REV_REACTIVE_ENERGY_T + 1, lowWord);
    
    float2registers(data.phaseA.apparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_A, highWord);
    _inputRegisters[MB_APPARENT_ENERGY_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_A + 1, lowWord);
    
    float2registers(data.phaseB.apparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_B, highWord);
    _inputRegisters[MB_APPARENT_ENERGY_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_B + 1, lowWord);
    
    float2registers(data.phaseC.apparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_C, highWord);
    _inputRegisters[MB_APPARENT_ENERGY_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_C + 1, lowWord);
    
    float2registers(data.totalApparentEnergy, highWord, lowWord);
    _inputRegisters[MB_APPARENT_ENERGY_T] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_T, highWord);
    _inputRegisters[MB_APPARENT_ENERGY_T + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_APPARENT_ENERGY_T + 1, lowWord);
    
    float2registers(data.phaseA.fundamentalPower, highWord, lowWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FUNDAMENTAL_POWER_A, highWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FUNDAMENTAL_POWER_A + 1, lowWord);
    
    float2registers(data.phaseB.fundamentalPower, highWord, lowWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FUNDAMENTAL_POWER_B, highWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FUNDAMENTAL_POWER_B + 1, lowWord);
    
    float2registers(data.phaseC.fundamentalPower, highWord, lowWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_FUNDAMENTAL_POWER_C, highWord);
    _inputRegisters[MB_FUNDAMENTAL_POWER_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_FUNDAMENTAL_POWER_C + 1, lowWord);
    
    float2registers(data.phaseA.harmonicPower, highWord, lowWord);
    _inputRegisters[MB_HARMONIC_POWER_A] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_HARMONIC_POWER_A, highWord);
    _inputRegisters[MB_HARMONIC_POWER_A + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_HARMONIC_POWER_A + 1, lowWord);
    
    float2registers(data.phaseB.harmonicPower, highWord, lowWord);
    _inputRegisters[MB_HARMONIC_POWER_B] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_HARMONIC_POWER_B, highWord);
    _inputRegisters[MB_HARMONIC_POWER_B + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_HARMONIC_POWER_B + 1, lowWord);
    
    float2registers(data.phaseC.harmonicPower, highWord, lowWord);
    _inputRegisters[MB_HARMONIC_POWER_C] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_HARMONIC_POWER_C, highWord);
    _inputRegisters[MB_HARMONIC_POWER_C + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_HARMONIC_POWER_C + 1, lowWord);
    
    float2registers(data.boardTemperature, highWord, lowWord);
    _inputRegisters[MB_BOARD_TEMP] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_BOARD_TEMP, highWord);
    _inputRegisters[MB_BOARD_TEMP + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_BOARD_TEMP + 1, lowWord);
    
    float2registers(data.ambientTemperature, highWord, lowWord);
    _inputRegisters[MB_AMBIENT_TEMP] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_AMBIENT_TEMP, highWord);
    _inputRegisters[MB_AMBIENT_TEMP + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_AMBIENT_TEMP + 1, lowWord);
    
    float2registers(data.ambientHumidity, highWord, lowWord);
    _inputRegisters[MB_AMBIENT_HUMIDITY] = highWord;
    if (mbReady) _modbusRTU.Ireg(MB_AMBIENT_HUMIDITY, highWord);
    _inputRegisters[MB_AMBIENT_HUMIDITY + 1] = lowWord;
    if (mbReady) _modbusRTU.Ireg(MB_AMBIENT_HUMIDITY + 1, lowWord);
}

void ModbusServer::updateSystemStatus(const SystemStatus& status) {
    _systemStatus = status;

    // Mirror the cached system status values into the Modbus input registers
    // when RTU is enabled. (Modbus TCP is disabled in this firmware.)
    const bool mbReady = _rtuEnabled;
    
    _inputRegisters[MB_UPTIME_SECONDS] = (status.uptime >> 16) & 0xFFFF;
    if (mbReady) _modbusRTU.Ireg(MB_UPTIME_SECONDS, (status.uptime >> 16) & 0xFFFF);
    _inputRegisters[MB_UPTIME_SECONDS + 1] = status.uptime & 0xFFFF;
    if (mbReady) _modbusRTU.Ireg(MB_UPTIME_SECONDS + 1, status.uptime & 0xFFFF);
    
    _inputRegisters[MB_ERROR_COUNT] = status.errorCount;
    if (mbReady) _modbusRTU.Ireg(MB_ERROR_COUNT, status.errorCount);
    _inputRegisters[MB_FIRMWARE_VERSION] = 0x0200;
    if (mbReady) _modbusRTU.Ireg(MB_FIRMWARE_VERSION, 0x0200);
    
    _inputRegisters[MB_FREE_HEAP] = (status.freeHeap >> 16) & 0xFFFF;
    if (mbReady) _modbusRTU.Ireg(MB_FREE_HEAP, (status.freeHeap >> 16) & 0xFFFF);
    _inputRegisters[MB_FREE_HEAP + 1] = status.freeHeap & 0xFFFF;
    if (mbReady) _modbusRTU.Ireg(MB_FREE_HEAP + 1, status.freeHeap & 0xFFFF);
    
    uint16_t statusFlags = 0;
    if (status.wifiConnected) statusFlags |= STATUS_FLAG_WIFI_CONNECTED;
    if (status.mqttConnected) statusFlags |= STATUS_FLAG_MQTT_CONNECTED;
    if (status.modbusActive) statusFlags |= STATUS_FLAG_MODBUS_ACTIVE;
    _inputRegisters[MB_STATUS_FLAGS] = statusFlags;
    if (mbReady) _modbusRTU.Ireg(MB_STATUS_FLAGS, statusFlags);
    
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
    if (address >= MB_INPUT_REG_COUNT) return 0;
    // Prefer the Modbus library's internal register store (no callbacks).
    if (_rtuEnabled) {
        return _modbusRTU.Ireg(address);
    }
    return _inputRegisters[address];
}

uint16_t ModbusServer::readHoldingRegister(uint16_t address) {
    if (address >= MB_HOLDING_REG_COUNT) return 0;
    if (_rtuEnabled) {
        return _modbusRTU.Hreg(address);
    }
    return _holdingRegisters[address];
}

void ModbusServer::writeHoldingRegister(uint16_t address, uint16_t value) {
    if (address >= MB_HOLDING_REG_COUNT) return;
    _holdingRegisters[address] = value;
    if (_rtuEnabled) {
        _modbusRTU.Hreg(address, value);
    }
}

bool ModbusServer::readCoil(uint16_t address) {
    if (address >= MB_COIL_COUNT) return false;
    if (_rtuEnabled) {
        return _modbusRTU.Coil(address);
    }
    return _coils[address];
}

void ModbusServer::writeCoil(uint16_t address, bool state) {
    if (address >= MB_COIL_COUNT) return;
    _coils[address] = state;
    if (_rtuEnabled) {
        _modbusRTU.Coil(address, state);
    }
}

bool ModbusServer::readDiscreteInput(uint16_t address) {
    if (address >= MB_DISCRETE_INPUT_COUNT) return false;
    if (_rtuEnabled) {
        return _modbusRTU.Ists(address);
    }
    return _discreteInputs[address];
}

void ModbusServer::setCoil(uint16_t address, bool state) {
    writeCoil(address, state);
}

bool ModbusServer::getCoil(uint16_t address) {
    return readCoil(address);
}

void ModbusServer::setDiscreteInput(uint16_t address, bool state) {
    if (address >= MB_DISCRETE_INPUT_COUNT) return;
    _discreteInputs[address] = state;
    if (_rtuEnabled) {
        _modbusRTU.Ists(address, state);
    }
}
