#include "EnergyAccumulator.h"
#include <nvs_flash.h>

// Preferences.begin() can fail on devices with corrupted NVS or unexpected partition state.
// This helper attempts a single NVS repair (erase+init) and retries once.
static bool beginPrefsSafe(Preferences& prefs, const char* ns, bool readOnly) {
    if (prefs.begin(ns, readOnly)) {
        return true;
    }
    Logger::getInstance().error("[NVS] Failed to open namespace '%s' (attempting NVS repair)", ns);

    esp_err_t err = nvs_flash_erase();
    if (err != ESP_OK) {
        Logger::getInstance().error("[NVS] nvs_flash_erase failed: %d", (int)err);
        return false;
    }
    err = nvs_flash_init();
    if (err != ESP_OK) {
        Logger::getInstance().error("[NVS] nvs_flash_init after erase failed: %d", (int)err);
        return false;
    }
    if (!prefs.begin(ns, readOnly)) {
        Logger::getInstance().error("[NVS] Failed to open namespace '%s' after repair", ns);
        return false;
    }
    Logger::getInstance().warn("[NVS] Namespace '%s' opened after repair", ns);
    return true;
}


EnergyAccumulator::EnergyAccumulator()
    : m_initialized(false)
    , m_persistInterval(300)
    , m_lastPersistTime(0)
    , m_lastUpdateTime(0)
    , m_mutex(nullptr)
{
}

bool EnergyAccumulator::init(uint32_t persistIntervalSec) {
    if (m_initialized) {
        Logger::getInstance().warn("EnergyAccumulator: Already initialized");
        return true;
    }

    m_persistInterval = persistIntervalSec;
    
    m_mutex = xSemaphoreCreateMutex();
    if (m_mutex == nullptr) {
        Logger::getInstance().error("EnergyAccumulator: Failed to create mutex");
        return false;
    }

    loadFromNVS();
    
    m_lastPersistTime = millis() / 1000;
    m_lastUpdateTime = millis() / 1000;
    m_initialized = true;
    
    Logger::getInstance().info("EnergyAccumulator", "Initialized with persist interval: " + 
                              String(m_persistInterval) + "s");
    
    return true;
}

void EnergyAccumulator::update(const MeterData& data) {
    if (!m_initialized) {
        Logger::getInstance().error("EnergyAccumulator: Not initialized");
        return;
    }

    uint32_t currentTime = millis() / 1000;
    
    if (m_lastUpdateTime == 0) {
        m_lastUpdateTime = currentTime;
        return;
    }

    float deltaTime = (currentTime - m_lastUpdateTime) / 3600.0f;
    
    if (deltaTime <= 0 || deltaTime > 3600) {
        Logger::getInstance().warn("EnergyAccumulator", "Invalid time delta: " + String(deltaTime));
        m_lastUpdateTime = currentTime;
        return;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        accumulatePhase(m_energy.phaseA, data.phaseA.activePower, 
                       data.phaseA.reactivePower, deltaTime);
        accumulatePhase(m_energy.phaseB, data.phaseB.activePower, 
                       data.phaseB.reactivePower, deltaTime);
        accumulatePhase(m_energy.phaseC, data.phaseC.activePower, 
                       data.phaseC.reactivePower, deltaTime);
        
        m_energy.total.activeEnergyImport = m_energy.phaseA.activeEnergyImport + 
                                            m_energy.phaseB.activeEnergyImport + 
                                            m_energy.phaseC.activeEnergyImport;
        m_energy.total.activeEnergyExport = m_energy.phaseA.activeEnergyExport + 
                                            m_energy.phaseB.activeEnergyExport + 
                                            m_energy.phaseC.activeEnergyExport;
        m_energy.total.reactiveEnergyImport = m_energy.phaseA.reactiveEnergyImport + 
                                              m_energy.phaseB.reactiveEnergyImport + 
                                              m_energy.phaseC.reactiveEnergyImport;
        m_energy.total.reactiveEnergyExport = m_energy.phaseA.reactiveEnergyExport + 
                                              m_energy.phaseB.reactiveEnergyExport + 
                                              m_energy.phaseC.reactiveEnergyExport;
        
        m_energy.lastUpdateTime = currentTime;
        
        xSemaphoreGive(m_mutex);
    }

    m_lastUpdateTime = currentTime;
    
    if (m_persistInterval > 0 && (currentTime - m_lastPersistTime) >= m_persistInterval) {
        saveToNVS();
        m_lastPersistTime = currentTime;
    }
}

bool EnergyAccumulator::loadFromNVS() {
    if (!beginPrefsSafe(m_preferences, NVS_NAMESPACE, false)) {
        Logger::getInstance().error("EnergyAccumulator: Failed to open NVS namespace");
        return false;
    }

    bool exists = m_preferences.isKey("phaseA_AEI");
    
    if (!exists) {
        Logger::getInstance().info("EnergyAccumulator: No saved energy data in NVS");
        m_preferences.end();
        return false;
    }

    m_energy.phaseA.activeEnergyImport = m_preferences.getDouble("phaseA_AEI", 0.0);
    m_energy.phaseA.activeEnergyExport = m_preferences.getDouble("phaseA_AEE", 0.0);
    m_energy.phaseA.reactiveEnergyImport = m_preferences.getDouble("phaseA_REI", 0.0);
    m_energy.phaseA.reactiveEnergyExport = m_preferences.getDouble("phaseA_REE", 0.0);
    
    m_energy.phaseB.activeEnergyImport = m_preferences.getDouble("phaseB_AEI", 0.0);
    m_energy.phaseB.activeEnergyExport = m_preferences.getDouble("phaseB_AEE", 0.0);
    m_energy.phaseB.reactiveEnergyImport = m_preferences.getDouble("phaseB_REI", 0.0);
    m_energy.phaseB.reactiveEnergyExport = m_preferences.getDouble("phaseB_REE", 0.0);
    
    m_energy.phaseC.activeEnergyImport = m_preferences.getDouble("phaseC_AEI", 0.0);
    m_energy.phaseC.activeEnergyExport = m_preferences.getDouble("phaseC_AEE", 0.0);
    m_energy.phaseC.reactiveEnergyImport = m_preferences.getDouble("phaseC_REI", 0.0);
    m_energy.phaseC.reactiveEnergyExport = m_preferences.getDouble("phaseC_REE", 0.0);
    
    m_energy.total.activeEnergyImport = m_preferences.getDouble("total_AEI", 0.0);
    m_energy.total.activeEnergyExport = m_preferences.getDouble("total_AEE", 0.0);
    m_energy.total.reactiveEnergyImport = m_preferences.getDouble("total_REI", 0.0);
    m_energy.total.reactiveEnergyExport = m_preferences.getDouble("total_REE", 0.0);
    
    m_energy.lastUpdateTime = m_preferences.getUInt("lastUpdate", 0);
    
    m_preferences.end();
    
    Logger::getInstance().info("EnergyAccumulator: Loaded energy data from NVS");
    Logger::getInstance().debug("EnergyAccumulator", "Total Import: " + 
                               String(m_energy.total.activeEnergyImport, 3) + " kWh");
    
    return true;
}

bool EnergyAccumulator::saveToNVS() {
    if (!beginPrefsSafe(m_preferences, NVS_NAMESPACE, false)) {
        Logger::getInstance().error("EnergyAccumulator: Failed to open NVS namespace for write");
        return false;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        m_preferences.putDouble("phaseA_AEI", m_energy.phaseA.activeEnergyImport);
        m_preferences.putDouble("phaseA_AEE", m_energy.phaseA.activeEnergyExport);
        m_preferences.putDouble("phaseA_REI", m_energy.phaseA.reactiveEnergyImport);
        m_preferences.putDouble("phaseA_REE", m_energy.phaseA.reactiveEnergyExport);
        
        m_preferences.putDouble("phaseB_AEI", m_energy.phaseB.activeEnergyImport);
        m_preferences.putDouble("phaseB_AEE", m_energy.phaseB.activeEnergyExport);
        m_preferences.putDouble("phaseB_REI", m_energy.phaseB.reactiveEnergyImport);
        m_preferences.putDouble("phaseB_REE", m_energy.phaseB.reactiveEnergyExport);
        
        m_preferences.putDouble("phaseC_AEI", m_energy.phaseC.activeEnergyImport);
        m_preferences.putDouble("phaseC_AEE", m_energy.phaseC.activeEnergyExport);
        m_preferences.putDouble("phaseC_REI", m_energy.phaseC.reactiveEnergyImport);
        m_preferences.putDouble("phaseC_REE", m_energy.phaseC.reactiveEnergyExport);
        
        m_preferences.putDouble("total_AEI", m_energy.total.activeEnergyImport);
        m_preferences.putDouble("total_AEE", m_energy.total.activeEnergyExport);
        m_preferences.putDouble("total_REI", m_energy.total.reactiveEnergyImport);
        m_preferences.putDouble("total_REE", m_energy.total.reactiveEnergyExport);
        
        m_preferences.putUInt("lastUpdate", m_energy.lastUpdateTime);
        
        xSemaphoreGive(m_mutex);
    }
    
    m_preferences.end();
    
    Logger::getInstance().debug("EnergyAccumulator: Saved energy data to NVS");
    
    return true;
}

EnergyData EnergyAccumulator::getAccumulatedEnergy() {
    EnergyData data;
    
    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        data = m_energy;
        xSemaphoreGive(m_mutex);
    } else {
        Logger::getInstance().warn("EnergyAccumulator: Mutex timeout in getAccumulatedEnergy");
    }
    
    return data;
}

void EnergyAccumulator::reset() {
    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        m_energy = EnergyData();
        xSemaphoreGive(m_mutex);
    }
    
    saveToNVS();
    Logger::getInstance().info("EnergyAccumulator: Reset all accumulated energy");
}

void EnergyAccumulator::accumulatePhase(EnergyData::PhaseEnergy& energy, 
                                        float activePower, float reactivePower, 
                                        float deltaTime) {
    float activeEnergy = (activePower / 1000.0f) * deltaTime;
    float reactiveEnergy = (reactivePower / 1000.0f) * deltaTime;
    
    if (activePower > 0) {
        energy.activeEnergyImport += activeEnergy;
    } else if (activePower < 0) {
        energy.activeEnergyExport += -activeEnergy;
    }
    
    if (reactivePower > 0) {
        energy.reactiveEnergyImport += reactiveEnergy;
    } else if (reactivePower < 0) {
        energy.reactiveEnergyExport += -reactiveEnergy;
    }
}