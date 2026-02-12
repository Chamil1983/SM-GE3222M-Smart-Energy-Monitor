/**
 * SM-GE3222M V2.0 - ATM90E36 Energy IC Driver
 * 
 * Complete port of V1.0 EnergyATM90E36.cpp (~800 lines)
 * Modernized with:
 * - Singleton pattern
 * - SPIBus::getInstance() for thread-safe SPI
 * - Logger for diagnostics
 * - RegisterMap.h for all addresses
 * - DataTypes.h structures
 */

#include "ATM90E36Driver.h"
#include "SPIBus.h"
#include "Logger.h"
#include "PinMap.h"
#include <Arduino.h>
#include <cmath>

// SPI protocol constants
#define SPI_READ  1
#define SPI_WRITE 0

// ============================================================================
// CONSTRUCTOR & SINGLETON
// ============================================================================

ATM90E36Driver::ATM90E36Driver()
    : m_initialized(false)
    , m_lastError(ErrorCode::NONE)
    , m_lastReadTime(0)
    , m_csZero(0)
    , m_csOne(0)
    , m_csTwo(0)
    , m_csThree(0)
{
}

// ============================================================================
// INITIALIZATION
// ============================================================================

bool ATM90E36Driver::init(const CalibrationConfig& config) {
    Logger& logger = Logger::getInstance();
    logger.info("ATM90E36: Initializing...");

    // Ensure CS line is configured (prevents 0xFFFF reads)
    pinMode(PIN_SPI_CS_ATM90E36, OUTPUT);
    digitalWrite(PIN_SPI_CS_ATM90E36, HIGH);
    
    m_calibration = config;
    
    // Soft reset
    if (!softReset()) {
        m_lastError = ErrorCode::ATM_INIT_FAILED;
        logger.error("ATM90E36: Soft reset failed");
        return false;
    }
    
    delay(100);
    
    // Apply calibration sequence
    if (!applyCalibration(config)) {
        m_lastError = ErrorCode::ATM_CALIBRATION_ERROR;
        logger.error("ATM90E36: Calibration failed");
        return false;
    }
    
    delay(100);
    
    // Verify checksums (non-fatal: allow SAFE MODE boot and later retry)
    if (!verifyChecksums()) {
        m_lastError = ErrorCode::ATM_CHECKSUM_FAILED;
        logger.warn("ATM90E36: Checksum verification failed (continuing)");
    } else {
        logger.info("ATM90E36: Checksum verification OK");
    }


// Enable metering functions (required for valid RMS/power reads)
// V1 firmware enables all features after config+calibration sections are latched.
writeRegister(REG_FUNC_EN0, 0xFFFF);
writeRegister(REG_FUNC_EN1, 0xFFFF);
delay(10);

    m_initialized = true;
    logger.info("ATM90E36: Initialization complete");
    return true;
}

bool ATM90E36Driver::softReset() {
    Logger& logger = Logger::getInstance();
    logger.debug("ATM90E36: Performing soft reset");
    
    // Write soft reset value
    if (!writeRegister(REG_SOFT_RESET, 0x789A)) {
        return false;
    }
    
    delay(200);
    return true;
}

// ============================================================================
// CALIBRATION
// ============================================================================

bool ATM90E36Driver::applyCalibration(const CalibrationConfig& config) {
    Logger& logger = Logger::getInstance();
    logger.info("ATM90E36: Applying calibration");
    // Function enable registers
    writeRegister(REG_FUNC_EN0, 0x0000);
    writeRegister(REG_FUNC_EN1, 0x0000);
    writeRegister(REG_SAG_TH, 0x0001);
    
    // *** PHASE 1: CONFIGURATION REGISTERS (ConfigStart) ***
    logger.debug("ATM90E36: Writing configuration registers");
    writeRegister(REG_CONFIG_START, CAL_CONFIG_START);  // 0x5678
    writeRegister(REG_PL_CONST_H, DEFAULT_PL_CONST_H);  // 0x0861
    writeRegister(REG_PL_CONST_L, DEFAULT_PL_CONST_L);  // 0xC468
    writeRegister(REG_MMODE0, config.lineFreq);
    writeRegister(REG_MMODE1, config.pgaGain);
    writeRegister(REG_P_START_TH, config.pStartTh);
    writeRegister(REG_Q_START_TH, config.qStartTh);
    writeRegister(REG_S_START_TH, config.sStartTh);
    writeRegister(REG_P_PHASE_TH, config.pPhaseTh);
    writeRegister(REG_Q_PHASE_TH, config.qPhaseTh);
    writeRegister(REG_S_PHASE_TH, config.sPhaseTh);
    
    // Calculate and write CS0
    uint16_t csCalc[11] = {
        DEFAULT_PL_CONST_H, DEFAULT_PL_CONST_L,
        config.lineFreq, config.pgaGain,
        config.pStartTh, config.qStartTh, config.sStartTh,
        config.pPhaseTh, config.qPhaseTh, config.sPhaseTh, 0
    };
    m_csZero = calculateChecksum(csCalc, 11);
    writeRegister(REG_CS_ZERO, m_csZero);
    // End config section (latch checksum)
    writeRegister(REG_CONFIG_START, CONFIG_END_MAGIC);
    delay(10);
    
    // *** PHASE 2: CALIBRATION REGISTERS (CalStart) ***
    logger.debug("ATM90E36: Writing power calibration registers");
    writeRegister(REG_CAL_START, CAL_CONFIG_START);  // 0x5678
    writeRegister(REG_P_OFFSET_A, config.calRegs[0]);
    writeRegister(REG_Q_OFFSET_A, config.calRegs[1]);
    writeRegister(REG_GAIN_A, config.calRegs[2]);
    writeRegister(REG_PHI_A, config.calRegs[3]);
    writeRegister(REG_P_OFFSET_B, config.calRegs[4]);
    writeRegister(REG_Q_OFFSET_B, config.calRegs[5]);
    writeRegister(REG_GAIN_B, config.calRegs[6]);
    writeRegister(REG_PHI_B, config.calRegs[7]);
    writeRegister(REG_P_OFFSET_C, config.calRegs[8]);
    writeRegister(REG_Q_OFFSET_C, config.calRegs[9]);
    writeRegister(REG_GAIN_C, config.calRegs[10]);
    writeRegister(REG_PHI_C, config.calRegs[11]);
    
    // Calculate CS1
    m_csOne = calculateChecksum(config.calRegs, 12);
    writeRegister(REG_CS_ONE, m_csOne);
    // End power calibration section
    writeRegister(REG_CAL_START, CONFIG_END_MAGIC);
    delay(10);
    
    // *** PHASE 3: HARMONIC REGISTERS (HarmStart) ***
    logger.debug("ATM90E36: Writing harmonic calibration registers");
    writeRegister(REG_HARM_START, CAL_CONFIG_START);  // 0x5678
    writeRegister(REG_P_OFFSET_AF, config.harCalRegs[0]);
    writeRegister(REG_P_OFFSET_BF, config.harCalRegs[1]);
    writeRegister(REG_P_OFFSET_CF, config.harCalRegs[2]);
    writeRegister(REG_P_GAIN_AF, config.harCalRegs[3]);
    writeRegister(REG_P_GAIN_BF, config.harCalRegs[4]);
    writeRegister(REG_P_GAIN_CF, config.harCalRegs[5]);
    
    // Calculate CS2
    m_csTwo = calculateChecksum(config.harCalRegs, 6);
    writeRegister(REG_CS_TWO, m_csTwo);
    // End harmonic calibration section
    writeRegister(REG_HARM_START, CONFIG_END_MAGIC);
    delay(10);
    
    // *** PHASE 4: MEASUREMENT CALIBRATION REGISTERS (AdjStart) ***
    logger.debug("ATM90E36: Writing measurement calibration registers");
    writeRegister(REG_ADJ_START, CAL_CONFIG_START);  // 0x5678
    writeRegister(REG_U_GAIN_A, config.measCalRegs[0]);
    writeRegister(REG_I_GAIN_A, config.measCalRegs[1]);
    writeRegister(REG_U_OFFSET_A, config.measCalRegs[2]);
    writeRegister(REG_I_OFFSET_A, config.measCalRegs[3]);
    writeRegister(REG_U_GAIN_B, config.measCalRegs[4]);
    writeRegister(REG_I_GAIN_B, config.measCalRegs[5]);
    writeRegister(REG_U_OFFSET_B, config.measCalRegs[6]);
    writeRegister(REG_I_OFFSET_B, config.measCalRegs[7]);
    writeRegister(REG_U_GAIN_C, config.measCalRegs[8]);
    writeRegister(REG_I_GAIN_C, config.measCalRegs[9]);
    writeRegister(REG_U_OFFSET_C, config.measCalRegs[10]);
    writeRegister(REG_I_OFFSET_C, config.measCalRegs[11]);
    writeRegister(REG_I_GAIN_N, config.measCalRegs[12]);
    writeRegister(REG_I_OFFSET_N, config.measCalRegs[13]);
    
    // Calculate CS3
    m_csThree = calculateChecksum(config.measCalRegs, 14);
    writeRegister(REG_CS_THREE, m_csThree);
    // End measurement calibration section
    writeRegister(REG_ADJ_START, CONFIG_END_MAGIC);
    delay(10);
    
    logger.info("ATM90E36: Calibration complete");
    return true;
}

uint16_t ATM90E36Driver::calculateChecksum(const uint16_t* registers, uint8_t count) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < count; i++) {
        sum += registers[i];
    }
    sum += CAL_CONFIG_START;  // Add magic number
    sum = ~sum;  // One's complement
    return (uint16_t)(sum & 0xFFFF);
}

bool ATM90E36Driver::verifyChecksums() {
    Logger& logger = Logger::getInstance();
    
    // Read system status to check for errors
    uint16_t sysStatus0 = readRegister(REG_SYS_STATUS0);
    
    bool cs0Error = (sysStatus0 & 0x4000) != 0;
    bool cs1Error = (sysStatus0 & 0x1000) != 0;
    bool cs2Error = (sysStatus0 & 0x0400) != 0;
    bool cs3Error = (sysStatus0 & 0x0100) != 0;
    
    logger.debug("ATM90E36: Checksum status - CS0:%d CS1:%d CS2:%d CS3:%d", 
                 cs0Error, cs1Error, cs2Error, cs3Error);
    
    if (cs0Error || cs1Error || cs2Error || cs3Error) {
        logger.error("ATM90E36: Checksum verification failed");
        return false;
    }
    
    return true;
}

bool ATM90E36Driver::hasCalibrationError() {
    uint16_t sysStatus0 = readRegister(REG_SYS_STATUS0);
    return ((sysStatus0 & 0x4000) || (sysStatus0 & 0x1000) || 
            (sysStatus0 & 0x0400) || (sysStatus0 & 0x0100));
}

// ============================================================================
// LOW-LEVEL SPI COMMUNICATION
// ============================================================================

uint16_t ATM90E36Driver::readRegister(uint16_t address) {
    SPIBus& spi = SPIBus::getInstance();
    if (!spi.isInitialized()) {
        m_lastError = ErrorCode::SPI_INIT_FAILED;
        return 0xFFFF;
    }
    
    uint16_t val = spi.readRegister(PIN_SPI_CS_ATM90E36, address);

    // Guard against floating-bus reads (0xFFFF) which indicate CS/config/SPI issues.
    if (val == 0xFFFF) {
        m_lastError = ErrorCode::ATM_SPI_COMM_ERROR;
        static uint32_t s_lastWarn = 0;
        uint32_t now = millis();
        if (now - s_lastWarn > 1000) {
            Logger::getInstance().error("ATM90E36: SPI read returned 0xFFFF at reg 0x%04X", (unsigned)address);
            s_lastWarn = now;
        }
    }

    return val;
}

bool ATM90E36Driver::writeRegister(uint16_t address, uint16_t value) {
    SPIBus& spi = SPIBus::getInstance();
    if (!spi.isInitialized()) {
        m_lastError = ErrorCode::SPI_INIT_FAILED;
        return false;
    }
    
    return spi.writeRegister(PIN_SPI_CS_ATM90E36, address, value);
}

uint32_t ATM90E36Driver::read32BitRegister(uint16_t highAddress, uint16_t lowAddress) {
    // ATM90E36 requires reading high register twice for 32-bit values
    uint16_t valH = readRegister(highAddress);
    uint16_t valL = readRegister(lowAddress);
    uint16_t valH2 = readRegister(highAddress);  // Read high again
    
    uint32_t val = ((uint32_t)valH2 << 16) | valL;
    return val;
}

// ============================================================================
// BULK READ (OPTIMIZED)
// ============================================================================

bool ATM90E36Driver::readAll(MeterData& data) {
    if (!m_initialized) {
        m_lastError = ErrorCode::ATM_INIT_FAILED;
        return false;
    }
    
    // Read all voltage/current/power registers
    data.phaseA.voltageRMS = getVoltageA();
    data.phaseB.voltageRMS = getVoltageB();
    data.phaseC.voltageRMS = getVoltageC();
    
    data.phaseA.currentRMS = getCurrentA();
    data.phaseB.currentRMS = getCurrentB();
    data.phaseC.currentRMS = getCurrentC();
    data.neutralCurrent = getCurrentN();
    
    data.phaseA.activePower = getActivePowerA();
    data.phaseB.activePower = getActivePowerB();
    data.phaseC.activePower = getActivePowerC();
    data.totalActivePower = getActivePowerTotal();
    
    data.phaseA.reactivePower = getReactivePowerA();
    data.phaseB.reactivePower = getReactivePowerB();
    data.phaseC.reactivePower = getReactivePowerC();
    data.totalReactivePower = getReactivePowerTotal();
    
    data.phaseA.apparentPower = getApparentPowerA();
    data.phaseB.apparentPower = getApparentPowerB();
    data.phaseC.apparentPower = getApparentPowerC();
    data.totalApparentPower = getApparentPowerTotal();
    
    data.phaseA.powerFactor = getPowerFactorA();
    data.phaseB.powerFactor = getPowerFactorB();
    data.phaseC.powerFactor = getPowerFactorC();
    data.totalPowerFactor = getPowerFactorTotal();
    
    data.phaseA.meanPhaseAngle = getPhaseAngleA();
    data.phaseB.meanPhaseAngle = getPhaseAngleB();
    data.phaseC.meanPhaseAngle = getPhaseAngleC();
    
    data.phaseA.voltageTHDN = getVoltageTHDA();
    data.phaseB.voltageTHDN = getVoltageTHDB();
    data.phaseC.voltageTHDN = getVoltageTHDC();
    
    data.phaseA.currentTHDN = getCurrentTHDA();
    data.phaseB.currentTHDN = getCurrentTHDB();
    data.phaseC.currentTHDN = getCurrentTHDC();
    
    data.frequency = getFrequency();
    data.boardTemperature = getTemperature();
    
    // Energy counters (optional - clear on read)
    // data.totalFwdActiveEnergy = getFwdActiveEnergyTotal();
    // data.totalRevActiveEnergy = getRevActiveEnergyTotal();
    
    data.meteringStatus0 = readRegister(REG_EN_STATUS0);
    data.meteringStatus1 = readRegister(REG_EN_STATUS1);
    data.timestamp = millis() / 1000;
    data.valid = true;
    
    m_lastReadTime = millis();
    return true;
}

// ============================================================================
// VOLTAGE READINGS
// ============================================================================

float ATM90E36Driver::getVoltageA() {
    uint16_t val = readRegister(REG_U_RMS_A);
    return (float)val / VOLTAGE_SCALE;
}

float ATM90E36Driver::getVoltageB() {
    uint16_t val = readRegister(REG_U_RMS_B);
    return (float)val / VOLTAGE_SCALE;
}

float ATM90E36Driver::getVoltageC() {
    uint16_t val = readRegister(REG_U_RMS_C);
    return (float)val / VOLTAGE_SCALE;
}

// ============================================================================
// CURRENT READINGS
// ============================================================================

float ATM90E36Driver::getCurrentA() {
    uint16_t val = readRegister(REG_I_RMS_A);
    return (float)val / CURRENT_SCALE;
}

float ATM90E36Driver::getCurrentB() {
    uint16_t val = readRegister(REG_I_RMS_B);
    return (float)val / CURRENT_SCALE;
}

float ATM90E36Driver::getCurrentC() {
    uint16_t val = readRegister(REG_I_RMS_C);
    return (float)val / CURRENT_SCALE;
}

float ATM90E36Driver::getCurrentN() {
    uint16_t val = readRegister(REG_I_RMS_N);
    return (float)val / CURRENT_SCALE;
}

// ============================================================================
// ACTIVE POWER READINGS (32-bit)
// ============================================================================

float ATM90E36Driver::getActivePowerA() {
    int32_t val = (int32_t)read32BitRegister(REG_P_MEAN_A, REG_P_MEAN_A_LSB);
    return (float)val * 0.00032;  // Power scaling from V1.0
}

float ATM90E36Driver::getActivePowerB() {
    int32_t val = (int32_t)read32BitRegister(REG_P_MEAN_B, REG_P_MEAN_B_LSB);
    return (float)val * 0.00032;
}

float ATM90E36Driver::getActivePowerC() {
    int32_t val = (int32_t)read32BitRegister(REG_P_MEAN_C, REG_P_MEAN_C_LSB);
    return (float)val * 0.00032;
}

float ATM90E36Driver::getActivePowerTotal() {
    int32_t val = (int32_t)read32BitRegister(REG_P_MEAN_T, REG_P_MEAN_T_LSB);
    return (float)val * 0.00032;
}

// ============================================================================
// REACTIVE POWER READINGS (32-bit)
// ============================================================================

float ATM90E36Driver::getReactivePowerA() {
    int32_t val = (int32_t)read32BitRegister(REG_Q_MEAN_A, REG_Q_MEAN_A_LSB);
    return (float)val * 0.00032;
}

float ATM90E36Driver::getReactivePowerB() {
    int32_t val = (int32_t)read32BitRegister(REG_Q_MEAN_B, REG_Q_MEAN_B_LSB);
    return (float)val * 0.00032;
}

float ATM90E36Driver::getReactivePowerC() {
    int32_t val = (int32_t)read32BitRegister(REG_Q_MEAN_C, REG_Q_MEAN_C_LSB);
    return (float)val * 0.00032;
}

float ATM90E36Driver::getReactivePowerTotal() {
    int32_t val = (int32_t)read32BitRegister(REG_Q_MEAN_T, REG_Q_MEAN_T_LSB);
    return (float)val * 0.00032;
}

// ============================================================================
// APPARENT POWER READINGS (32-bit)
// ============================================================================

float ATM90E36Driver::getApparentPowerA() {
    int32_t val = (int32_t)read32BitRegister(REG_S_MEAN_A, 0xBA);  // SmeanALSB not in map
    return (float)val * 0.00032;
}

float ATM90E36Driver::getApparentPowerB() {
    int32_t val = (int32_t)read32BitRegister(REG_S_MEAN_B, 0xBA);  // SmeanBLSB not in map
    return (float)val * 0.00032;
}

float ATM90E36Driver::getApparentPowerC() {
    int32_t val = (int32_t)read32BitRegister(REG_S_MEAN_C, 0xBB);  // SmeanCLSB not in map
    return (float)val * 0.00032;
}

float ATM90E36Driver::getApparentPowerTotal() {
    int32_t val = (int32_t)read32BitRegister(REG_S_MEAN_T, 0xB8);  // SAmeanTLSB not in map
    return (float)val * 0.00032;
}

// ============================================================================
// POWER FACTOR READINGS (16-bit signed)
// ============================================================================

float ATM90E36Driver::getPowerFactorA() {
    int16_t val = (int16_t)readRegister(REG_PF_MEAN_A);
    return (float)val / PF_SCALE;
}

float ATM90E36Driver::getPowerFactorB() {
    int16_t val = (int16_t)readRegister(REG_PF_MEAN_B);
    return (float)val / PF_SCALE;
}

float ATM90E36Driver::getPowerFactorC() {
    int16_t val = (int16_t)readRegister(REG_PF_MEAN_C);
    return (float)val / PF_SCALE;
}

float ATM90E36Driver::getPowerFactorTotal() {
    int16_t val = (int16_t)readRegister(REG_PF_MEAN_T);
    return (float)val / PF_SCALE;
}

// ============================================================================
// PHASE ANGLE READINGS
// ============================================================================

float ATM90E36Driver::getPhaseAngleA() {
    int16_t val = (int16_t)readRegister(REG_P_ANGLE_A);
    return (float)val / ANGLE_SCALE;
}

float ATM90E36Driver::getPhaseAngleB() {
    int16_t val = (int16_t)readRegister(REG_P_ANGLE_B);
    return (float)val / ANGLE_SCALE;
}

float ATM90E36Driver::getPhaseAngleC() {
    int16_t val = (int16_t)readRegister(REG_P_ANGLE_C);
    return (float)val / ANGLE_SCALE;
}

// ============================================================================
// THD READINGS
// ============================================================================

float ATM90E36Driver::getVoltageTHDA() {
    uint16_t val = readRegister(REG_THD_NU_A);
    return (float)val / THD_SCALE;
}

float ATM90E36Driver::getVoltageTHDB() {
    uint16_t val = readRegister(REG_THD_NU_B);
    return (float)val / THD_SCALE;
}

float ATM90E36Driver::getVoltageTHDC() {
    uint16_t val = readRegister(REG_THD_NU_C);
    return (float)val / THD_SCALE;
}

float ATM90E36Driver::getCurrentTHDA() {
    uint16_t val = readRegister(REG_THD_NI_A);
    return (float)val / THD_SCALE;
}

float ATM90E36Driver::getCurrentTHDB() {
    uint16_t val = readRegister(REG_THD_NI_B);
    return (float)val / THD_SCALE;
}

float ATM90E36Driver::getCurrentTHDC() {
    uint16_t val = readRegister(REG_THD_NI_C);
    return (float)val / THD_SCALE;
}

// ============================================================================
// FREQUENCY & TEMPERATURE
// ============================================================================

float ATM90E36Driver::getFrequency() {
    uint16_t val = readRegister(REG_FREQUENCY);
    return (float)val / FREQUENCY_SCALE;
}

float ATM90E36Driver::getTemperature() {
    int16_t val = (int16_t)readRegister(REG_TEMPERATURE);
    return (float)val;  // Temperature in °C (direct reading)
}

// ============================================================================
// ENERGY COUNTERS (32-bit, clear-on-read)
// ============================================================================

uint32_t ATM90E36Driver::getFwdActiveEnergyA() {
    uint16_t val = readRegister(REG_AP_ENERGY_A);
    return (uint32_t)val;
}

uint32_t ATM90E36Driver::getFwdActiveEnergyB() {
    uint16_t val = readRegister(REG_AP_ENERGY_B);
    return (uint32_t)val;
}

uint32_t ATM90E36Driver::getFwdActiveEnergyC() {
    uint16_t val = readRegister(REG_AP_ENERGY_C);
    return (uint32_t)val;
}

uint32_t ATM90E36Driver::getFwdActiveEnergyTotal() {
    uint16_t val = readRegister(REG_AP_ENERGY_T);
    return (uint32_t)val;
}

uint32_t ATM90E36Driver::getRevActiveEnergyA() {
    uint16_t val = readRegister(REG_AN_ENERGY_A);
    return (uint32_t)val;
}

uint32_t ATM90E36Driver::getRevActiveEnergyB() {
    uint16_t val = readRegister(REG_AN_ENERGY_B);
    return (uint32_t)val;
}

uint32_t ATM90E36Driver::getRevActiveEnergyC() {
    uint16_t val = readRegister(REG_AN_ENERGY_C);
    return (uint32_t)val;
}

uint32_t ATM90E36Driver::getRevActiveEnergyTotal() {
    uint16_t val = readRegister(REG_AN_ENERGY_T);
    return (uint32_t)val;
}
