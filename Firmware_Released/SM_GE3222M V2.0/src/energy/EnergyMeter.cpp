/**
 * @file EnergyMeter.cpp
 * @brief High-level energy measurement engine implementation
 */

#include "EnergyMeter.h"
#include "../core/ErrorHandler.h"
#include <ArduinoJson.h>

EnergyMeter::EnergyMeter() 
    : initialized(false), 
      dataValid(false),
      filterEnabled(false),
      filterWindowSize(5),
      driver(ATM90E36Driver::getInstance()),
      filterIndex(0) {
    // Initialize filter buffers
    memset(voltageBuffer, 0, sizeof(voltageBuffer));
    memset(currentBuffer, 0, sizeof(currentBuffer));
    memset(powerBuffer, 0, sizeof(powerBuffer));
}

EnergyMeter::~EnergyMeter() {
}

EnergyMeter& EnergyMeter::getInstance() {
    static EnergyMeter instance;
    return instance;
}

bool EnergyMeter::init(uint8_t filterWindowSize) {
    if (!driver.isInitialized()) {
        ErrorHandler::getInstance().logError(ErrorCode::ATM_INIT_FAIL, 
            "EnergyMeter: ATM90E36Driver not initialized");
        return false;
    }
    
    setFilterWindowSize(filterWindowSize);
    filterEnabled = (filterWindowSize > 0);
    filterIndex = 0;
    
    initialized = true;
    return true;
}

bool EnergyMeter::readAll() {
    if (!initialized || !driver.isInitialized()) {
        return false;
    }
    
    // Read phase data
    currentData.phaseA = readPhaseData(1);
    currentData.phaseB = readPhaseData(2);
    currentData.phaseC = readPhaseData(3);
    
    // Read total measurements
    currentData.totalActivePower = driver.GetTotalActivePower();
    currentData.totalReactivePower = driver.GetTotalReactivePower();
    currentData.totalApparentPower = driver.GetTotalApparentPower();
    currentData.totalPowerFactor = driver.GetTotalPowerFactor();
    
    // System parameters
    currentData.frequency = driver.GetFrequency();
    currentData.temperature = driver.GetTemperature();
    currentData.neutralCurrentCalc = driver.GetLineCurrentCTN();
    currentData.neutralCurrentSampled = driver.GetLineCurrentCTNSamp();
    
    // Energy accumulators
    currentData.importActiveEnergy[0] = driver.GetImportEnergy();
    currentData.importActiveEnergy[1] = driver.GetImportEnergyPhaA();
    currentData.importActiveEnergy[2] = driver.GetImportEnergyPhaB();
    currentData.importActiveEnergy[3] = driver.GetImportEnergyPhaC();
    
    currentData.exportActiveEnergy[0] = driver.GetExportEnergy();
    currentData.exportActiveEnergy[1] = driver.GetExportEnergyPhaA();
    currentData.exportActiveEnergy[2] = driver.GetExportEnergyPhaB();
    currentData.exportActiveEnergy[3] = driver.GetExportEnergyPhaC();
    
    currentData.importReactiveEnergy[0] = driver.GetImportReactiveEnergy();
    currentData.importReactiveEnergy[1] = driver.GetImportReactiveEnergyPhaA();
    currentData.importReactiveEnergy[2] = driver.GetImportReactiveEnergyPhaB();
    currentData.importReactiveEnergy[3] = driver.GetImportReactiveEnergyPhaC();
    
    currentData.exportReactiveEnergy[0] = driver.GetExportReactiveEnergy();
    currentData.exportReactiveEnergy[1] = driver.GetExportReactiveEnergyPhaA();
    currentData.exportReactiveEnergy[2] = driver.GetExportReactiveEnergyPhaB();
    currentData.exportReactiveEnergy[3] = driver.GetExportReactiveEnergyPhaC();
    
    currentData.apparentEnergy[0] = driver.GetImportApparentEnergy();
    currentData.apparentEnergy[1] = driver.GetImportApparentEnergyPhaA();
    currentData.apparentEnergy[2] = driver.GetImportApparentEnergyPhaB();
    currentData.apparentEnergy[3] = driver.GetImportApparentEnergyPhaC();
    
    // Fundamental/Harmonic power
    currentData.totalActiveFundPower = driver.GetTotalActiveFundPower();
    currentData.totalActiveHarPower = driver.GetTotalActiveHarPower();
    
    // Timestamp
    currentData.readTimestamp = millis();
    
    // Validate data
    dataValid = validateMeasurements(currentData);
    
    if (!dataValid) {
        ErrorHandler::getInstance().logError(ErrorCode::ENERGY_READ_FAIL,
            "EnergyMeter: Measurement validation failed");
    }
    
    return dataValid;
}

PhaseData EnergyMeter::readPhaseData(uint8_t phase) {
    PhaseData data;
    
    switch(phase) {
        case 1: // Phase A
            data.voltage = driver.GetLineVoltage1();
            data.current = driver.GetLineCurrentCT1();
            data.activePower = driver.GetActivePowerCT1();
            data.reactivePower = driver.GetReactivePowerCT1();
            data.apparentPower = driver.GetApparentPowerCT1();
            data.powerFactor = driver.GetPowerFactorCT1();
            data.phaseAngle = driver.GetPhaseCT1();
            data.voltagePhaseAngle = driver.GetPhaseangV1();
            data.voltageTHDN = driver.GetLineVoltage1THDN();
            data.currentTHDN = driver.GetLineCurrentCT1THDN();
            break;
            
        case 2: // Phase B
            data.voltage = driver.GetLineVoltage2();
            data.current = driver.GetLineCurrentCT2();
            data.activePower = driver.GetActivePowerCT2();
            data.reactivePower = driver.GetReactivePowerCT2();
            data.apparentPower = driver.GetApparentPowerCT2();
            data.powerFactor = driver.GetPowerFactorCT2();
            data.phaseAngle = driver.GetPhaseCT2();
            data.voltagePhaseAngle = driver.GetPhaseangV2();
            data.voltageTHDN = driver.GetLineVoltage2THDN();
            data.currentTHDN = driver.GetLineCurrentCT2THDN();
            break;
            
        case 3: // Phase C
            data.voltage = driver.GetLineVoltage3();
            data.current = driver.GetLineCurrentCT3();
            data.activePower = driver.GetActivePowerCT3();
            data.reactivePower = driver.GetReactivePowerCT3();
            data.apparentPower = driver.GetApparentPowerCT3();
            data.powerFactor = driver.GetPowerFactorCT3();
            data.phaseAngle = driver.GetPhaseCT3();
            data.voltagePhaseAngle = driver.GetPhaseangV3();
            data.voltageTHDN = driver.GetLineVoltage3THDN();
            data.currentTHDN = driver.GetLineCurrentCT3THDN();
            break;
    }
    
    if (filterEnabled && phase >= 1 && phase <= 3) {
        applyFilter(data);
    }
    
    return data;
}

void EnergyMeter::applyFilter(PhaseData& phase) {
    // Simple moving average filter
    // This is a placeholder - in production would need proper circular buffer
    // Currently just validates the data is reasonable
    if (phase.voltage < 0 || phase.voltage > MAX_VOLTAGE) {
        phase.voltage = 0;
    }
    if (phase.current < 0 || phase.current > MAX_CURRENT) {
        phase.current = 0;
    }
}

bool EnergyMeter::validateMeasurements(const MeterData& data) const {
    // Check voltage ranges
    if (data.phaseA.voltage > MAX_VOLTAGE || data.phaseB.voltage > MAX_VOLTAGE || 
        data.phaseC.voltage > MAX_VOLTAGE) {
        return false;
    }
    
    // Check for negative voltages (should not happen)
    if (data.phaseA.voltage < 0 || data.phaseB.voltage < 0 || data.phaseC.voltage < 0) {
        return false;
    }
    
    // Check current ranges
    if (data.phaseA.current > MAX_CURRENT || data.phaseB.current > MAX_CURRENT || 
        data.phaseC.current > MAX_CURRENT) {
        return false;
    }
    
    // Check frequency is reasonable (50Hz or 60Hz ± deviation)
    if (data.frequency > 0) {
        float deviation50 = abs(data.frequency - NOMINAL_FREQUENCY_50HZ);
        float deviation60 = abs(data.frequency - NOMINAL_FREQUENCY_60HZ);
        float minDeviation = min(deviation50, deviation60);
        
        if (minDeviation > MAX_FREQUENCY_DEVIATION) {
            return false;
        }
    }
    
    return true;
}

PhaseData EnergyMeter::getPhaseData(uint8_t phase) const {
    switch(phase) {
        case 1:
            return currentData.phaseA;
        case 2:
            return currentData.phaseB;
        case 3:
            return currentData.phaseC;
        default:
            return PhaseData();
    }
}

MeterData EnergyMeter::getMeterData() const {
    return currentData;
}

String EnergyMeter::getJSON(bool prettyPrint) const {
    DynamicJsonDocument doc(4096);
    
    // Phase A
    JsonObject phaseA = doc.createNestedObject("phaseA");
    phaseA["voltage"] = currentData.phaseA.voltage;
    phaseA["current"] = currentData.phaseA.current;
    phaseA["activePower"] = currentData.phaseA.activePower;
    phaseA["reactivePower"] = currentData.phaseA.reactivePower;
    phaseA["apparentPower"] = currentData.phaseA.apparentPower;
    phaseA["powerFactor"] = currentData.phaseA.powerFactor;
    phaseA["phaseAngle"] = currentData.phaseA.phaseAngle;
    phaseA["voltageTHDN"] = currentData.phaseA.voltageTHDN;
    phaseA["currentTHDN"] = currentData.phaseA.currentTHDN;
    
    // Phase B
    JsonObject phaseB = doc.createNestedObject("phaseB");
    phaseB["voltage"] = currentData.phaseB.voltage;
    phaseB["current"] = currentData.phaseB.current;
    phaseB["activePower"] = currentData.phaseB.activePower;
    phaseB["reactivePower"] = currentData.phaseB.reactivePower;
    phaseB["apparentPower"] = currentData.phaseB.apparentPower;
    phaseB["powerFactor"] = currentData.phaseB.powerFactor;
    phaseB["phaseAngle"] = currentData.phaseB.phaseAngle;
    phaseB["voltageTHDN"] = currentData.phaseB.voltageTHDN;
    phaseB["currentTHDN"] = currentData.phaseB.currentTHDN;
    
    // Phase C
    JsonObject phaseC = doc.createNestedObject("phaseC");
    phaseC["voltage"] = currentData.phaseC.voltage;
    phaseC["current"] = currentData.phaseC.current;
    phaseC["activePower"] = currentData.phaseC.activePower;
    phaseC["reactivePower"] = currentData.phaseC.reactivePower;
    phaseC["apparentPower"] = currentData.phaseC.apparentPower;
    phaseC["powerFactor"] = currentData.phaseC.powerFactor;
    phaseC["phaseAngle"] = currentData.phaseC.phaseAngle;
    phaseC["voltageTHDN"] = currentData.phaseC.voltageTHDN;
    phaseC["currentTHDN"] = currentData.phaseC.currentTHDN;
    
    // Totals
    doc["totalActivePower"] = currentData.totalActivePower;
    doc["totalReactivePower"] = currentData.totalReactivePower;
    doc["totalApparentPower"] = currentData.totalApparentPower;
    doc["totalPowerFactor"] = currentData.totalPowerFactor;
    
    // System
    doc["frequency"] = currentData.frequency;
    doc["temperature"] = currentData.temperature;
    doc["neutralCurrent"] = currentData.neutralCurrentCalc;
    
    // Timestamp
    doc["timestamp"] = currentData.readTimestamp;
    
    String output;
    if (prettyPrint) {
        serializeJsonPretty(doc, output);
    } else {
        serializeJson(doc, output);
    }
    
    return output;
}

bool EnergyMeter::update() {
    return readAll();
}

void EnergyMeter::setFilterWindowSize(uint8_t size) {
    if (size > MAX_FILTER_SIZE) {
        size = MAX_FILTER_SIZE;
    }
    filterWindowSize = size;
    filterEnabled = (size > 0);
}

unsigned long EnergyMeter::getTimeSinceLastRead() const {
    if (currentData.readTimestamp == 0) {
        return 0;
    }
    return millis() - currentData.readTimestamp;
}
