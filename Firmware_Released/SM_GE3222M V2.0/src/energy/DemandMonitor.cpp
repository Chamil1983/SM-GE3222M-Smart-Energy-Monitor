/**
 * @file DemandMonitor.cpp
 * @brief Implementation of DemandMonitor
 */

#include "DemandMonitor.h"
#include "../diagnostics/Logger.h"
#include <ArduinoJson.h>
#include <time.h>

DemandMonitor& DemandMonitor::getInstance() {
    static DemandMonitor instance;
    return instance;
}

bool DemandMonitor::init(uint16_t intervalMinutes) {
    if (initialized) {
        LOG_WARN("DemandMonitor", "Already initialized");
        return true;
    }
    
    demandData.intervalMinutes = intervalMinutes;
    demandData.intervalStartTime = millis();
    demandData.nextIntervalTime = demandData.intervalStartTime + 
                                   (unsigned long)intervalMinutes * 60000;
    
    // Initialize TOU schedule (default: peak 8AM-8PM, shoulder 6-8AM & 8-10PM)
    touPeakStart = 8;
    touPeakEnd = 20;
    touShoulderStart1 = 6;
    touShoulderEnd1 = 8;
    touShoulderStart2 = 20;
    touShoulderEnd2 = 22;
    
    // Initialize rolling demand buffer
    for (int i = 0; i < DEMAND_ROLLING_SAMPLES; i++) {
        rollingDemandBuffer[i] = 0;
    }
    rollingDemandIndex = 0;
    
    // Reset accumulators
    powerAccumTotal = 0;
    powerAccumPhaseA = 0;
    powerAccumPhaseB = 0;
    powerAccumPhaseC = 0;
    sampleCount = 0;
    intervalComplete = false;
    
    // Load saved peak demand from NVS
    load();
    
    initialized = true;
    LOG_INFO("DemandMonitor", "Initialized with %d minute intervals", intervalMinutes);
    
    return true;
}

void DemandMonitor::update(const MeterData& data) {
    if (!initialized) {
        return;
    }
    
    intervalComplete = false;
    
    // Check if interval has elapsed
    unsigned long now = millis();
    if (now >= demandData.nextIntervalTime) {
        calculateIntervalDemand();
        updatePeakDemand();
        updateRollingDemand();
        
        // Start new interval
        demandData.intervalStartTime = now;
        demandData.nextIntervalTime = now + 
                                       (unsigned long)demandData.intervalMinutes * 60000;
        
        powerAccumTotal = 0;
        powerAccumPhaseA = 0;
        powerAccumPhaseB = 0;
        powerAccumPhaseC = 0;
        sampleCount = 0;
        
        intervalComplete = true;
        
        LOG_DEBUG("DemandMonitor", "Interval complete - Demand: %.2f kW", 
                  demandData.currentDemandTotal);
    }
    
    // Accumulate power readings
    powerAccumTotal += data.totalData.activePowerTotal;
    powerAccumPhaseA += data.phaseA.activePower;
    powerAccumPhaseB += data.phaseB.activePower;
    powerAccumPhaseC += data.phaseC.activePower;
    sampleCount++;
    
    // Update TOU period
    demandData.currentTOUPeriod = getCurrentTOUPeriod();
}

void DemandMonitor::calculateIntervalDemand() {
    if (sampleCount == 0) {
        demandData.currentDemandTotal = 0;
        demandData.currentDemandPhaseA = 0;
        demandData.currentDemandPhaseB = 0;
        demandData.currentDemandPhaseC = 0;
        return;
    }
    
    // Calculate average power during interval (in kW)
    demandData.currentDemandTotal = (powerAccumTotal / sampleCount) / 1000.0;
    demandData.currentDemandPhaseA = (powerAccumPhaseA / sampleCount) / 1000.0;
    demandData.currentDemandPhaseB = (powerAccumPhaseB / sampleCount) / 1000.0;
    demandData.currentDemandPhaseC = (powerAccumPhaseC / sampleCount) / 1000.0;
    
    // Calculate energy consumed in this interval (kWh)
    float intervalHours = demandData.intervalMinutes / 60.0;
    float energyKWh = demandData.currentDemandTotal * intervalHours;
    
    // Update TOU energy accumulators
    updateTOUEnergy(energyKWh);
}

void DemandMonitor::updatePeakDemand() {
    unsigned long now = millis();
    
    // Update total peak
    if (demandData.currentDemandTotal > demandData.peakDemandTotal) {
        demandData.peakDemandTotal = demandData.currentDemandTotal;
        demandData.peakDemandTimestamp = now;
        LOG_INFO("DemandMonitor", "New peak demand (total): %.2f kW", 
                 demandData.peakDemandTotal);
    }
    
    // Update phase A peak
    if (demandData.currentDemandPhaseA > demandData.peakDemandPhaseA) {
        demandData.peakDemandPhaseA = demandData.currentDemandPhaseA;
        demandData.peakDemandTimestampA = now;
    }
    
    // Update phase B peak
    if (demandData.currentDemandPhaseB > demandData.peakDemandPhaseB) {
        demandData.peakDemandPhaseB = demandData.currentDemandPhaseB;
        demandData.peakDemandTimestampB = now;
    }
    
    // Update phase C peak
    if (demandData.currentDemandPhaseC > demandData.peakDemandPhaseC) {
        demandData.peakDemandPhaseC = demandData.currentDemandPhaseC;
        demandData.peakDemandTimestampC = now;
    }
}

void DemandMonitor::updateTOUEnergy(float energyKWh) {
    TOUPeriod period = getCurrentTOUPeriod();
    
    switch (period) {
        case TOUPeriod::PEAK:
            demandData.touEnergyPeak += energyKWh;
            break;
        case TOUPeriod::SHOULDER:
            demandData.touEnergyShoulder += energyKWh;
            break;
        case TOUPeriod::OFF_PEAK:
        default:
            demandData.touEnergyOffPeak += energyKWh;
            break;
    }
}

void DemandMonitor::updateRollingDemand() {
    // Add current demand to rolling buffer
    rollingDemandBuffer[rollingDemandIndex] = demandData.currentDemandTotal;
    rollingDemandIndex = (rollingDemandIndex + 1) % DEMAND_ROLLING_SAMPLES;
    
    // Calculate rolling average
    float sum = 0;
    for (int i = 0; i < DEMAND_ROLLING_SAMPLES; i++) {
        sum += rollingDemandBuffer[i];
    }
    demandData.rollingDemandTotal = sum / DEMAND_ROLLING_SAMPLES;
}

TOUPeriod DemandMonitor::getCurrentTOUPeriod() const {
    // Get current hour (0-23)
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    uint8_t hour = timeinfo->tm_hour;
    
    // Check peak period
    if (hour >= touPeakStart && hour < touPeakEnd) {
        return TOUPeriod::PEAK;
    }
    
    // Check shoulder periods
    if ((hour >= touShoulderStart1 && hour < touShoulderEnd1) ||
        (hour >= touShoulderStart2 && hour < touShoulderEnd2)) {
        return TOUPeriod::SHOULDER;
    }
    
    // Off-peak otherwise
    return TOUPeriod::OFF_PEAK;
}

unsigned long DemandMonitor::getTimeUntilNextInterval() const {
    unsigned long now = millis();
    if (now >= demandData.nextIntervalTime) {
        return 0;
    }
    return demandData.nextIntervalTime - now;
}

void DemandMonitor::resetPeakDemand() {
    demandData.peakDemandTotal = 0;
    demandData.peakDemandPhaseA = 0;
    demandData.peakDemandPhaseB = 0;
    demandData.peakDemandPhaseC = 0;
    demandData.peakDemandTimestamp = 0;
    demandData.peakDemandTimestampA = 0;
    demandData.peakDemandTimestampB = 0;
    demandData.peakDemandTimestampC = 0;
    
    LOG_INFO("DemandMonitor", "Peak demand reset");
    save();
}

void DemandMonitor::resetTOUEnergy() {
    demandData.touEnergyOffPeak = 0;
    demandData.touEnergyShoulder = 0;
    demandData.touEnergyPeak = 0;
    
    LOG_INFO("DemandMonitor", "TOU energy reset");
    save();
}

void DemandMonitor::resetAll() {
    resetPeakDemand();
    resetTOUEnergy();
    
    demandData.currentDemandTotal = 0;
    demandData.currentDemandPhaseA = 0;
    demandData.currentDemandPhaseB = 0;
    demandData.currentDemandPhaseC = 0;
    demandData.rollingDemandTotal = 0;
    
    for (int i = 0; i < DEMAND_ROLLING_SAMPLES; i++) {
        rollingDemandBuffer[i] = 0;
    }
    rollingDemandIndex = 0;
    
    LOG_INFO("DemandMonitor", "All demand data reset");
    save();
}

bool DemandMonitor::save() {
    NVStorage& storage = NVStorage::getInstance();
    
    bool success = true;
    success &= storage.putFloat(NVS_NAMESPACE_ENERGY, "peak_total", demandData.peakDemandTotal);
    success &= storage.putFloat(NVS_NAMESPACE_ENERGY, "peak_a", demandData.peakDemandPhaseA);
    success &= storage.putFloat(NVS_NAMESPACE_ENERGY, "peak_b", demandData.peakDemandPhaseB);
    success &= storage.putFloat(NVS_NAMESPACE_ENERGY, "peak_c", demandData.peakDemandPhaseC);
    success &= storage.putULong(NVS_NAMESPACE_ENERGY, "peak_time", demandData.peakDemandTimestamp);
    
    success &= storage.putFloat(NVS_NAMESPACE_ENERGY, "tou_offpeak", demandData.touEnergyOffPeak);
    success &= storage.putFloat(NVS_NAMESPACE_ENERGY, "tou_shoulder", demandData.touEnergyShoulder);
    success &= storage.putFloat(NVS_NAMESPACE_ENERGY, "tou_peak", demandData.touEnergyPeak);
    
    if (success) {
        LOG_DEBUG("DemandMonitor", "Demand data saved to NVS");
    } else {
        LOG_ERROR("DemandMonitor", "Failed to save demand data to NVS");
    }
    
    return success;
}

bool DemandMonitor::load() {
    NVStorage& storage = NVStorage::getInstance();
    
    demandData.peakDemandTotal = storage.getFloat(NVS_NAMESPACE_ENERGY, "peak_total", 0);
    demandData.peakDemandPhaseA = storage.getFloat(NVS_NAMESPACE_ENERGY, "peak_a", 0);
    demandData.peakDemandPhaseB = storage.getFloat(NVS_NAMESPACE_ENERGY, "peak_b", 0);
    demandData.peakDemandPhaseC = storage.getFloat(NVS_NAMESPACE_ENERGY, "peak_c", 0);
    demandData.peakDemandTimestamp = storage.getULong(NVS_NAMESPACE_ENERGY, "peak_time", 0);
    
    demandData.touEnergyOffPeak = storage.getFloat(NVS_NAMESPACE_ENERGY, "tou_offpeak", 0);
    demandData.touEnergyShoulder = storage.getFloat(NVS_NAMESPACE_ENERGY, "tou_shoulder", 0);
    demandData.touEnergyPeak = storage.getFloat(NVS_NAMESPACE_ENERGY, "tou_peak", 0);
    
    LOG_INFO("DemandMonitor", "Loaded peak demand: %.2f kW", demandData.peakDemandTotal);
    
    return true;
}

void DemandMonitor::setTOUSchedule(uint8_t peakStartHour, uint8_t peakEndHour,
                                    uint8_t shoulderStartHour1, uint8_t shoulderEndHour1,
                                    uint8_t shoulderStartHour2, uint8_t shoulderEndHour2) {
    touPeakStart = peakStartHour;
    touPeakEnd = peakEndHour;
    touShoulderStart1 = shoulderStartHour1;
    touShoulderEnd1 = shoulderEndHour1;
    touShoulderStart2 = shoulderStartHour2;
    touShoulderEnd2 = shoulderEndHour2;
    
    LOG_INFO("DemandMonitor", "TOU schedule updated - Peak: %02d:00-%02d:00", 
             peakStartHour, peakEndHour);
}

String DemandMonitor::toJSON() const {
    StaticJsonDocument<512> doc;
    
    doc["currentTotal"] = demandData.currentDemandTotal;
    doc["currentPhaseA"] = demandData.currentDemandPhaseA;
    doc["currentPhaseB"] = demandData.currentDemandPhaseB;
    doc["currentPhaseC"] = demandData.currentDemandPhaseC;
    
    doc["peakTotal"] = demandData.peakDemandTotal;
    doc["peakPhaseA"] = demandData.peakDemandPhaseA;
    doc["peakPhaseB"] = demandData.peakDemandPhaseB;
    doc["peakPhaseC"] = demandData.peakDemandPhaseC;
    
    doc["rollingTotal"] = demandData.rollingDemandTotal;
    
    doc["touOffPeak"] = demandData.touEnergyOffPeak;
    doc["touShoulder"] = demandData.touEnergyShoulder;
    doc["touPeak"] = demandData.touEnergyPeak;
    
    const char* periodStr = "off-peak";
    if (demandData.currentTOUPeriod == TOUPeriod::PEAK) periodStr = "peak";
    else if (demandData.currentTOUPeriod == TOUPeriod::SHOULDER) periodStr = "shoulder";
    doc["currentPeriod"] = periodStr;
    
    doc["intervalMinutes"] = demandData.intervalMinutes;
    doc["timeUntilNext"] = getTimeUntilNextInterval();
    
    String output;
    serializeJson(doc, output);
    return output;
}
