#pragma once

/**
 * SM-GE3222M V2.0 - ATM90E36 Energy IC Driver (Compatibility Wrapper)
 *
 * This wrapper keeps the V2 firmware interfaces (EnergyMeter, Web UI, Modbus, etc.)
 * while delegating all ATM90E36 SPI transactions and register sequencing to the
 * proven V1.0 driver (EnergyATM90E36.{h,cpp}).
 *
 * Key goals:
 * - Use the original V1.0 CommEnergyIC() (200kHz, MODE3, byte-wise transfer, byte swap)
 * - Use the original V1.0 begin() + InitEnergy() sequences
 * - Keep the V2 MeterData/PhaseData structures and APIs intact
 */

#include <Arduino.h>
#include "DataTypes.h"

// Forward declaration (defined in EnergyATM90E36.h)
class ATM90E3x;

class ATM90E36Driver {
public:
    static ATM90E36Driver& getInstance() {
        static ATM90E36Driver instance;
        return instance;
    }

    bool init(const CalibrationConfig& config);
    bool readAll(MeterData& data);

    bool verifyChecksums();
    bool hasCalibrationError();
    bool softReset();

    // Individual getters (used by some legacy parts)
    float getVoltageA();
    float getVoltageB();
    float getVoltageC();
    float getCurrentA();
    float getCurrentB();
    float getCurrentC();
    float getCurrentN();

    float getActivePowerA();
    float getActivePowerB();
    float getActivePowerC();
    float getActivePowerTotal();

    float getReactivePowerA();
    float getReactivePowerB();
    float getReactivePowerC();
    float getReactivePowerTotal();

    float getApparentPowerA();
    float getApparentPowerB();
    float getApparentPowerC();
    float getApparentPowerTotal();

    float getPowerFactorA();
    float getPowerFactorB();
    float getPowerFactorC();
    float getPowerFactorTotal();

    float getPhaseAngleA();
    float getPhaseAngleB();
    float getPhaseAngleC();

    float getVoltageTHDA();
    float getVoltageTHDB();
    float getVoltageTHDC();
    float getCurrentTHDA();
    float getCurrentTHDB();
    float getCurrentTHDC();

    float getFrequency();
    float getTemperature();

    // Raw energy (not currently used by V2, kept for API completeness)
    uint32_t getFwdActiveEnergyA();
    uint32_t getFwdActiveEnergyB();
    uint32_t getFwdActiveEnergyC();
    uint32_t getFwdActiveEnergyTotal();
    uint32_t getRevActiveEnergyA();
    uint32_t getRevActiveEnergyB();
    uint32_t getRevActiveEnergyC();
    uint32_t getRevActiveEnergyTotal();

    ErrorCode getLastError() const { return m_lastError; }

private:
    ATM90E36Driver();
    ~ATM90E36Driver() = default;
    ATM90E36Driver(const ATM90E36Driver&) = delete;
    ATM90E36Driver& operator=(const ATM90E36Driver&) = delete;

private:
    bool m_initialized;
    ErrorCode m_lastError;

    // Underlying proven V1.0 driver
    ATM90E3x* m_ic;
};
