#include "EnergyMeter.h"

EnergyMeter::EnergyMeter() 
    : m_initialized(false)
    , m_filterSize(5)
    , m_filterIndex(0)
    , m_filterFilled(false)
    , m_mutex(nullptr)
{
    memset(m_voltageBufferA, 0, sizeof(m_voltageBufferA));
    memset(m_voltageBufferB, 0, sizeof(m_voltageBufferB));
    memset(m_voltageBufferC, 0, sizeof(m_voltageBufferC));
    memset(m_currentBufferA, 0, sizeof(m_currentBufferA));
    memset(m_currentBufferB, 0, sizeof(m_currentBufferB));
    memset(m_currentBufferC, 0, sizeof(m_currentBufferC));
}

bool EnergyMeter::init(uint8_t filterSize) {
    if (m_initialized) {
        Logger::getInstance().warn("EnergyMeter: Already initialized");
        return true;
    }

    if (filterSize == 0 || filterSize > 10) {
        Logger::getInstance().error("EnergyMeter: Invalid filter size, must be 1-10");
        return false;
    }

    m_filterSize = filterSize;
    
    m_mutex = xSemaphoreCreateMutex();
    if (m_mutex == nullptr) {
        Logger::getInstance().error("EnergyMeter: Failed to create mutex");
        return false;
    }

    m_initialized = true;
    Logger::getInstance().info("EnergyMeter", "Initialized with filter size: " + String(m_filterSize));
    
    return true;
}

bool EnergyMeter::update() {
    if (!m_initialized) {
        Logger::getInstance().error("EnergyMeter: Not initialized");
        return false;
    }

    MeterData rawData;
    ATM90E36Driver& driver = ATM90E36Driver::getInstance();
    
    if (!driver.readAll(rawData)) {
        Logger::getInstance().error("EnergyMeter: Failed to read from ATM90E36");
        return false;
    }

    applyFilter(rawData);

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        // Preserve non-ATM90E36 fields that are maintained by other subsystems/tasks (e.g. DHT22).
        const float prevAmbientTemp = m_snapshot.ambientTemperature;
        const float prevAmbientHum  = m_snapshot.ambientHumidity;
        const float prevBoardTemp   = m_snapshot.boardTemperature;
        const uint32_t prevSeq      = m_snapshot.sequenceNumber;

        m_snapshot = rawData;

        // ATM90E36 driver does not provide ambient humidity and DHT22 temperature. Keep last published values.
        m_snapshot.ambientTemperature = prevAmbientTemp;
        m_snapshot.ambientHumidity    = prevAmbientHum;

        // If a transient read returns 0 board temp, keep the previous board temp to avoid UI flicker.
        if (m_snapshot.boardTemperature == 0.0f && prevBoardTemp != 0.0f) {
            m_snapshot.boardTemperature = prevBoardTemp;
        }

        m_snapshot.sequenceNumber = prevSeq + 1;
        m_snapshot.timestamp = millis() / 1000;
        m_snapshot.valid = true;
        xSemaphoreGive(m_mutex);
    } else {
        Logger::getInstance().warn("EnergyMeter: Mutex timeout during update");
        return false;
    }

    return true;
}

MeterData EnergyMeter::getSnapshot() {
    MeterData data;
    
    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        data = m_snapshot;
        xSemaphoreGive(m_mutex);
    } else {
        Logger::getInstance().warn("EnergyMeter: Mutex timeout during getSnapshot");
        data.valid = false;
    }
    
    return data;
}

void EnergyMeter::applyFilter(MeterData& data) {
    m_voltageBufferA[m_filterIndex] = data.phaseA.voltageRMS;
    m_voltageBufferB[m_filterIndex] = data.phaseB.voltageRMS;
    m_voltageBufferC[m_filterIndex] = data.phaseC.voltageRMS;
    m_currentBufferA[m_filterIndex] = data.phaseA.currentRMS;
    m_currentBufferB[m_filterIndex] = data.phaseB.currentRMS;
    m_currentBufferC[m_filterIndex] = data.phaseC.currentRMS;
    
    m_filterIndex++;
    if (m_filterIndex >= m_filterSize) {
        m_filterIndex = 0;
        m_filterFilled = true;
    }
    
    uint8_t count = m_filterFilled ? m_filterSize : m_filterIndex;
    if (count == 0) count = 1;
    
    float sumVA = 0, sumVB = 0, sumVC = 0;
    float sumCA = 0, sumCB = 0, sumCC = 0;
    
    for (uint8_t i = 0; i < count; i++) {
        sumVA += m_voltageBufferA[i];
        sumVB += m_voltageBufferB[i];
        sumVC += m_voltageBufferC[i];
        sumCA += m_currentBufferA[i];
        sumCB += m_currentBufferB[i];
        sumCC += m_currentBufferC[i];
    }
    
    data.phaseA.voltageRMS = sumVA / count;
    data.phaseB.voltageRMS = sumVB / count;
    data.phaseC.voltageRMS = sumVC / count;
    data.phaseA.currentRMS = sumCA / count;
    data.phaseB.currentRMS = sumCB / count;
    data.phaseC.currentRMS = sumCC / count;
}


void EnergyMeter::setAmbientReadings(float ambientTempC, float ambientHumidityPct, bool valid) {
    if (!m_initialized || m_mutex == nullptr) return;

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
        if (valid) {
            m_snapshot.ambientTemperature = ambientTempC;
            m_snapshot.ambientHumidity = ambientHumidityPct;
        }
        xSemaphoreGive(m_mutex);
    } else {
        Logger::getInstance().warn("EnergyMeter: Mutex timeout during setAmbientReadings");
    }
}
