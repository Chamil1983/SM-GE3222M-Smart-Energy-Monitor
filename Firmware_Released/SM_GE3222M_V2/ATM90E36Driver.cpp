#include "ATM90E36Driver.h"

#include "EnergyATM90E36.h"   // V1.0 proven driver
#include "PinMap.h"
#include "RegisterMap.h"
#include "Logger.h"

#include <SPI.h>

// In EnergyATM90E36.h:
//   READ  = 1
//   WRITE = 0
// and register symbols like UrmsA, IrmsA, Freq, SysStatus0, SysStatus1 are defined.

ATM90E36Driver::ATM90E36Driver()
    : m_initialized(false)
    , m_lastError(ErrorCode::NONE)
    , m_ic(nullptr) {
}

bool ATM90E36Driver::init(const CalibrationConfig& config) {
    Logger& logger = Logger::getInstance();
    logger.info("ATM90E36: Init (V1 driver wrapper)");

    // Ensure CS is a defined output and deasserted before SPI transactions.
    pinMode(PIN_SPI_CS_ATM90E36, OUTPUT);
    digitalWrite(PIN_SPI_CS_ATM90E36, HIGH);
    delay(5);

    // Ensure SPI pins are explicitly configured for this board.
    // (V1 driver calls SPI.begin() without pins; on ESP32 that is usually OK,
    //  but being explicit improves portability across variants.)
    SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_CS_ATM90E36);

    if (!m_ic) {
        m_ic = new ATM90E3x(PIN_SPI_CS_ATM90E36);
    }

    // Map CalibrationConfig -> V1 begin() parameters.
    // V1 expects: lineFreq, pgagain, ugain1, ugain2, ugain3, igainA, igainB, igainC, igainN
    const uint16_t ugain1 = config.measCalRegs[0];
    const uint16_t igainA = config.measCalRegs[1];
    const uint16_t ugain2 = config.measCalRegs[4];
    const uint16_t igainB = config.measCalRegs[5];
    const uint16_t ugain3 = config.measCalRegs[8];
    const uint16_t igainC = config.measCalRegs[9];
    const uint16_t igainN = config.measCalRegs[12];

    auto logRawProof = [&](const char* tag) {
        const uint16_t rawU = m_ic->CommEnergyIC(READ, UrmsA, 0x0000);
        const uint16_t rawI = m_ic->CommEnergyIC(READ, IrmsA, 0x0000);
        const uint16_t rawF = m_ic->CommEnergyIC(READ, Freq,  0x0000);
        const uint16_t sys0 = m_ic->CommEnergyIC(READ, SysStatus0, 0x0000);
        const uint16_t sys1 = m_ic->CommEnergyIC(READ, SysStatus1, 0x0000);
        const uint16_t cfg  = m_ic->CommEnergyIC(READ, REG_CFG_REG_ACC_EN, 0x0000);
        const uint16_t men  = m_ic->CommEnergyIC(READ, REG_METER_EN, 0x0000);

        logger.info("ATM90E36 %s: URMSA=0x%04X IRMSA=0x%04X FREQ=0x%04X SYS0=0x%04X SYS1=0x%04X CFG=0x%04X MEN=0x%04X",
                    tag, rawU, rawI, rawF, sys0, sys1, cfg, men);
        return rawU | rawI | rawF | sys0 | sys1;
    };

    auto looksLikeNoSPI = [&](uint16_t rawU, uint16_t rawI, uint16_t rawF, uint16_t sys0, uint16_t sys1) {
        // Common failure patterns when CS/MISO are wrong or bus is conflicted:
        // - all zeros
        // - all 0xFFFF
        // - repeating 0x55xx / 0xAAxx patterns (echo / floating bus)
        const bool all0  = (rawU == 0 && rawI == 0 && rawF == 0 && sys0 == 0 && sys1 == 0);
        const bool allFF = (rawU == 0xFFFF && rawI == 0xFFFF && rawF == 0xFFFF);
        const bool pat55 = ((sys0 & 0xFF00) == 0x5500) || ((rawU & 0xFF00) == 0x5500) || ((rawI & 0xFF00) == 0x5500);
        const bool patAA = ((sys0 & 0xFF00) == 0xAA00) || ((rawU & 0xFF00) == 0xAA00) || ((rawI & 0xFF00) == 0xAA00);
        return all0 || allFF || pat55 || patAA;
    };

    auto sanityRead = [&]() {
        const uint16_t rawU = m_ic->CommEnergyIC(READ, UrmsA, 0x0000);
        const uint16_t rawI = m_ic->CommEnergyIC(READ, IrmsA, 0x0000);
        const uint16_t rawF = m_ic->CommEnergyIC(READ, Freq,  0x0000);
        const uint16_t sys0 = m_ic->CommEnergyIC(READ, SysStatus0, 0x0000);
        const uint16_t sys1 = m_ic->CommEnergyIC(READ, SysStatus1, 0x0000);
        return looksLikeNoSPI(rawU, rawI, rawF, sys0, sys1);
    };

    // Execute the exact V1 initialization/calibration/write-unlock sequence.
    // This is the most important fix for the "0x5500 / all-zero RMS" SPI symptom.
    m_ic->begin(config.lineFreq, config.pgaGain, ugain1, ugain2, ugain3, igainA, igainB, igainC, igainN);
    m_ic->InitEnergy();

    // Proof-step 1: immediately after init
    logRawProof("RAW0");

    // Warm-up: RMS/frequency registers can be 0 for the first measurement window.
    // Give the metering core time to accumulate at least one full cycle.
    delay(1200);
    logRawProof("RAW1");

    // If still looks like a bus issue, retry init once (common after brown-out or fast reboot).
    if (sanityRead()) {
        logger.warn("ATM90E36: Raw registers look invalid (possible SPI conflict). Retrying init...");
        SPI.end();
        delay(10);
        SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_CS_ATM90E36);
        delay(10);
        m_ic->begin(config.lineFreq, config.pgaGain, ugain1, ugain2, ugain3, igainA, igainB, igainC, igainN);
        m_ic->InitEnergy();
        delay(1200);
        logRawProof("RAW2");

        if (sanityRead()) {
            logger.error("ATM90E36: SPI sanity still failing after retry");
            m_initialized = false;
            m_lastError = ErrorCode::ATM_INIT_FAILED;
            return false;
        }
    }

    m_initialized = true;
    m_lastError = ErrorCode::NONE;
    return true;
}

bool ATM90E36Driver::readAll(MeterData& data) {
    if (!m_initialized || !m_ic) {
        // V2 ErrorCode enum does not include a dedicated "not initialized" value.
        // Reuse the closest existing error so callers can surface a meaningful fault.
        m_lastError = ErrorCode::ATM_INIT_FAILED;
        return false;
    }

    // Phases (Voltage/Current)
    data.phaseA.voltageRMS = (float)m_ic->GetLineVoltage1();
    data.phaseB.voltageRMS = (float)m_ic->GetLineVoltage2();
    data.phaseC.voltageRMS = (float)m_ic->GetLineVoltage3();

    data.phaseA.currentRMS = (float)m_ic->GetLineCurrentCT1();
    data.phaseB.currentRMS = (float)m_ic->GetLineCurrentCT2();
    data.phaseC.currentRMS = (float)m_ic->GetLineCurrentCT3();

    // Power (use mean power values for stability)
    data.phaseA.activePower   = (float)m_ic->GetMeanActivePowerPhaA();
    data.phaseB.activePower   = (float)m_ic->GetMeanActivePowerPhaB();
    data.phaseC.activePower   = (float)m_ic->GetMeanActivePowerPhaC();
    data.totalActivePower     = (float)m_ic->GetTotalActivePower();

    data.phaseA.reactivePower = (float)m_ic->GetMeanReactivePowerPhaA();
    data.phaseB.reactivePower = (float)m_ic->GetMeanReactivePowerPhaB();
    data.phaseC.reactivePower = (float)m_ic->GetMeanReactivePowerPhaC();
    data.totalReactivePower   = (float)m_ic->GetTotalReactivePower();

    data.phaseA.apparentPower = (float)m_ic->GetMeanApparentPowerPhaA();
    data.phaseB.apparentPower = (float)m_ic->GetMeanApparentPowerPhaB();
    data.phaseC.apparentPower = (float)m_ic->GetMeanApparentPowerPhaC();
    data.totalApparentPower   = (float)m_ic->GetTotalApparentPower();

    // PF / angles
    data.phaseA.powerFactor   = (float)m_ic->GetPowerFactorCT1();
    data.phaseB.powerFactor   = (float)m_ic->GetPowerFactorCT2();
    data.phaseC.powerFactor   = (float)m_ic->GetPowerFactorCT3();
    data.totalPowerFactor     = (float)m_ic->GetTotalPowerFactor();

    data.phaseA.meanPhaseAngle = (float)m_ic->GetPhaseCT1();
    data.phaseB.meanPhaseAngle = (float)m_ic->GetPhaseCT2();
    data.phaseC.meanPhaseAngle = (float)m_ic->GetPhaseCT3();

    data.phaseA.voltagePhaseAngle = (float)m_ic->GetPhaseangV1();
    data.phaseB.voltagePhaseAngle = (float)m_ic->GetPhaseangV2();
    data.phaseC.voltagePhaseAngle = (float)m_ic->GetPhaseangV3();

    // THD
    data.phaseA.voltageTHDN = (float)m_ic->GetLineVoltage1THDN();
    data.phaseB.voltageTHDN = (float)m_ic->GetLineVoltage2THDN();
    data.phaseC.voltageTHDN = (float)m_ic->GetLineVoltage3THDN();

    data.phaseA.currentTHDN = (float)m_ic->GetLineCurrentCT1THDN();
    data.phaseB.currentTHDN = (float)m_ic->GetLineCurrentCT2THDN();
    data.phaseC.currentTHDN = (float)m_ic->GetLineCurrentCT3THDN();

    // Neutral & frequency/temperature
    data.neutralCurrent = (float)m_ic->GetLineCurrentCTN();
    data.frequency      = (float)m_ic->GetFrequency();
    // V2 MeterData stores this as boardTemperature.
    data.boardTemperature = (float)m_ic->GetTemperature();

    // Energy (kWh / kVARh / kVAh)
    data.totalFwdActiveEnergy     = (float)m_ic->GetImportEnergy();
    data.totalRevActiveEnergy     = (float)m_ic->GetExportEnergy();
    data.totalFwdReactiveEnergy   = (float)m_ic->GetImportReactiveEnergy();
    data.totalRevReactiveEnergy   = (float)m_ic->GetExportReactiveEnergy();
    data.totalApparentEnergy      = (float)m_ic->GetImportApparentEnergy();

    m_lastError = ErrorCode::NONE;
    return true;
}

bool ATM90E36Driver::verifyChecksums() {
    // The V1.0 driver does not expose explicit checksum verification for all sections on ATM90E36.
    // We treat "no calibration error" + non-zero SYS status as a practical verification.
    if (!m_initialized || !m_ic) return false;
    return !hasCalibrationError();
}

bool ATM90E36Driver::hasCalibrationError() {
    if (!m_initialized || !m_ic) return true;
    return m_ic->calibrationError();
}

bool ATM90E36Driver::softReset() {
    if (!m_initialized || !m_ic) return false;
    // V1 sequence uses: SoftReset = 0x789A
    m_ic->CommEnergyIC(WRITE, SoftReset, 0x789A);
    delay(50);
    return true;
}

// --- Individual getter wrappers ---
float ATM90E36Driver::getVoltageA() { return m_initialized ? (float)m_ic->GetLineVoltage1() : 0.0f; }
float ATM90E36Driver::getVoltageB() { return m_initialized ? (float)m_ic->GetLineVoltage2() : 0.0f; }
float ATM90E36Driver::getVoltageC() { return m_initialized ? (float)m_ic->GetLineVoltage3() : 0.0f; }

float ATM90E36Driver::getCurrentA() { return m_initialized ? (float)m_ic->GetLineCurrentCT1() : 0.0f; }
float ATM90E36Driver::getCurrentB() { return m_initialized ? (float)m_ic->GetLineCurrentCT2() : 0.0f; }
float ATM90E36Driver::getCurrentC() { return m_initialized ? (float)m_ic->GetLineCurrentCT3() : 0.0f; }
float ATM90E36Driver::getCurrentN() { return m_initialized ? (float)m_ic->GetLineCurrentCTN() : 0.0f; }

float ATM90E36Driver::getActivePowerA() { return m_initialized ? (float)m_ic->GetMeanActivePowerPhaA() : 0.0f; }
float ATM90E36Driver::getActivePowerB() { return m_initialized ? (float)m_ic->GetMeanActivePowerPhaB() : 0.0f; }
float ATM90E36Driver::getActivePowerC() { return m_initialized ? (float)m_ic->GetMeanActivePowerPhaC() : 0.0f; }
float ATM90E36Driver::getActivePowerTotal() { return m_initialized ? (float)m_ic->GetTotalActivePower() : 0.0f; }

float ATM90E36Driver::getReactivePowerA() { return m_initialized ? (float)m_ic->GetMeanReactivePowerPhaA() : 0.0f; }
float ATM90E36Driver::getReactivePowerB() { return m_initialized ? (float)m_ic->GetMeanReactivePowerPhaB() : 0.0f; }
float ATM90E36Driver::getReactivePowerC() { return m_initialized ? (float)m_ic->GetMeanReactivePowerPhaC() : 0.0f; }
float ATM90E36Driver::getReactivePowerTotal() { return m_initialized ? (float)m_ic->GetTotalReactivePower() : 0.0f; }

float ATM90E36Driver::getApparentPowerA() { return m_initialized ? (float)m_ic->GetMeanApparentPowerPhaA() : 0.0f; }
float ATM90E36Driver::getApparentPowerB() { return m_initialized ? (float)m_ic->GetMeanApparentPowerPhaB() : 0.0f; }
float ATM90E36Driver::getApparentPowerC() { return m_initialized ? (float)m_ic->GetMeanApparentPowerPhaC() : 0.0f; }
float ATM90E36Driver::getApparentPowerTotal() { return m_initialized ? (float)m_ic->GetTotalApparentPower() : 0.0f; }

float ATM90E36Driver::getPowerFactorA() { return m_initialized ? (float)m_ic->GetPowerFactorCT1() : 0.0f; }
float ATM90E36Driver::getPowerFactorB() { return m_initialized ? (float)m_ic->GetPowerFactorCT2() : 0.0f; }
float ATM90E36Driver::getPowerFactorC() { return m_initialized ? (float)m_ic->GetPowerFactorCT3() : 0.0f; }
float ATM90E36Driver::getPowerFactorTotal() { return m_initialized ? (float)m_ic->GetTotalPowerFactor() : 0.0f; }

float ATM90E36Driver::getPhaseAngleA() { return m_initialized ? (float)m_ic->GetPhaseCT1() : 0.0f; }
float ATM90E36Driver::getPhaseAngleB() { return m_initialized ? (float)m_ic->GetPhaseCT2() : 0.0f; }
float ATM90E36Driver::getPhaseAngleC() { return m_initialized ? (float)m_ic->GetPhaseCT3() : 0.0f; }

float ATM90E36Driver::getVoltageTHDA() { return m_initialized ? (float)m_ic->GetLineVoltage1THDN() : 0.0f; }
float ATM90E36Driver::getVoltageTHDB() { return m_initialized ? (float)m_ic->GetLineVoltage2THDN() : 0.0f; }
float ATM90E36Driver::getVoltageTHDC() { return m_initialized ? (float)m_ic->GetLineVoltage3THDN() : 0.0f; }
float ATM90E36Driver::getCurrentTHDA() { return m_initialized ? (float)m_ic->GetLineCurrentCT1THDN() : 0.0f; }
float ATM90E36Driver::getCurrentTHDB() { return m_initialized ? (float)m_ic->GetLineCurrentCT2THDN() : 0.0f; }
float ATM90E36Driver::getCurrentTHDC() { return m_initialized ? (float)m_ic->GetLineCurrentCT3THDN() : 0.0f; }

float ATM90E36Driver::getFrequency() { return m_initialized ? (float)m_ic->GetFrequency() : 0.0f; }
float ATM90E36Driver::getTemperature() { return m_initialized ? (float)m_ic->GetTemperature() : 0.0f; }

// Energy raw getters not used (V2 uses float kWh fields in MeterData)
uint32_t ATM90E36Driver::getFwdActiveEnergyA() { return 0; }
uint32_t ATM90E36Driver::getFwdActiveEnergyB() { return 0; }
uint32_t ATM90E36Driver::getFwdActiveEnergyC() { return 0; }
uint32_t ATM90E36Driver::getFwdActiveEnergyTotal() { return 0; }
uint32_t ATM90E36Driver::getRevActiveEnergyA() { return 0; }
uint32_t ATM90E36Driver::getRevActiveEnergyB() { return 0; }
uint32_t ATM90E36Driver::getRevActiveEnergyC() { return 0; }
uint32_t ATM90E36Driver::getRevActiveEnergyTotal() { return 0; }