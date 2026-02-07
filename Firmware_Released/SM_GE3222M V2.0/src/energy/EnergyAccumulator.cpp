/**
 * @file EnergyAccumulator.cpp
 * @brief Energy tracking and persistence implementation
 */

#include "EnergyAccumulator.h"
#include <time.h>

const char* EnergyAccumulator::NVS_NAMESPACE = "energy";

EnergyAccumulator::EnergyAccumulator()
    : initialized(false),
      persistInterval(DEFAULT_PERSIST_INTERVAL),
      lastSaveTime(0),
      lastUpdateTime(0),
      currentDay(0),
      currentWeek(0),
      currentMonth(0),
      todayEnergy(0),
      yesterdayEnergy(0),
      thisWeekEnergy(0),
      thisMonthEnergy(0),
      dayStartEnergy(0),
      weekStartEnergy(0),
      monthStartEnergy(0) {
    // Initialize energy arrays
    for (int i = 0; i < 4; i++) {
        importActiveEnergy[i] = 0;
        exportActiveEnergy[i] = 0;
        importReactiveEnergy[i] = 0;
        exportReactiveEnergy[i] = 0;
        apparentEnergy[i] = 0;
        prevImportActiveEnergy[i] = 0;
        prevExportActiveEnergy[i] = 0;
        prevImportReactiveEnergy[i] = 0;
        prevExportReactiveEnergy[i] = 0;
        prevApparentEnergy[i] = 0;
    }
}

EnergyAccumulator::~EnergyAccumulator() {
    if (initialized) {
        save();
        preferences.end();
    }
}

EnergyAccumulator& EnergyAccumulator::getInstance() {
    static EnergyAccumulator instance;
    return instance;
}

bool EnergyAccumulator::init(uint32_t persistInterval) {
    if (!preferences.begin(NVS_NAMESPACE, false)) {
        return false;
    }
    
    this->persistInterval = persistInterval;
    
    // Try to load existing data
    load();
    
    // Initialize time tracking
    time_t now;
    time(&now);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    
    currentDay = timeinfo.tm_mday;
    currentWeek = timeinfo.tm_yday / 7;
    currentMonth = timeinfo.tm_mon;
    
    lastSaveTime = millis();
    lastUpdateTime = millis();
    initialized = true;
    
    return true;
}

bool EnergyAccumulator::update(const MeterData& meterData) {
    if (!initialized) {
        return false;
    }
    
    unsigned long now = millis();
    
    // Check for daily rollover
    checkDailyRollover();
    
    // Update energy values from meter (accumulate deltas)
    for (int i = 0; i < 4; i++) {
        // Calculate deltas from meter readings
        float importActiveDelta = meterData.importActiveEnergy[i] - prevImportActiveEnergy[i];
        float exportActiveDelta = meterData.exportActiveEnergy[i] - prevExportActiveEnergy[i];
        float importReactiveDelta = meterData.importReactiveEnergy[i] - prevImportReactiveEnergy[i];
        float exportReactiveDelta = meterData.exportReactiveEnergy[i] - prevExportReactiveEnergy[i];
        float apparentDelta = meterData.apparentEnergy[i] - prevApparentEnergy[i];
        
        // Only accumulate positive deltas (prevents rollback on meter reset)
        if (importActiveDelta > 0 && importActiveDelta < 1000) {
            importActiveEnergy[i] += importActiveDelta;
        }
        if (exportActiveDelta > 0 && exportActiveDelta < 1000) {
            exportActiveEnergy[i] += exportActiveDelta;
        }
        if (importReactiveDelta > 0 && importReactiveDelta < 1000) {
            importReactiveEnergy[i] += importReactiveDelta;
        }
        if (exportReactiveDelta > 0 && exportReactiveDelta < 1000) {
            exportReactiveEnergy[i] += exportReactiveDelta;
        }
        if (apparentDelta > 0 && apparentDelta < 1000) {
            apparentEnergy[i] += apparentDelta;
        }
        
        // Store current meter readings for next delta calculation
        prevImportActiveEnergy[i] = meterData.importActiveEnergy[i];
        prevExportActiveEnergy[i] = meterData.exportActiveEnergy[i];
        prevImportReactiveEnergy[i] = meterData.importReactiveEnergy[i];
        prevExportReactiveEnergy[i] = meterData.exportReactiveEnergy[i];
        prevApparentEnergy[i] = meterData.apparentEnergy[i];
    }
    
    // Update period totals
    updatePeriodTotals();
    
    lastUpdateTime = now;
    
    // Check if periodic save is needed
    if (needsPeriodicSave()) {
        save();
    }
    
    return true;
}

bool EnergyAccumulator::checkDailyRollover() {
    time_t now;
    time(&now);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    
    uint8_t day = timeinfo.tm_mday;
    uint8_t week = timeinfo.tm_yday / 7;
    uint8_t month = timeinfo.tm_mon;
    
    bool rollover = false;
    
    // Day changed
    if (day != currentDay) {
        yesterdayEnergy = todayEnergy;
        todayEnergy = 0;
        dayStartEnergy = importActiveEnergy[0];
        currentDay = day;
        rollover = true;
    }
    
    // Week changed
    if (week != currentWeek) {
        thisWeekEnergy = 0;
        weekStartEnergy = importActiveEnergy[0];
        currentWeek = week;
        rollover = true;
    }
    
    // Month changed
    if (month != currentMonth) {
        thisMonthEnergy = 0;
        monthStartEnergy = importActiveEnergy[0];
        currentMonth = month;
        rollover = true;
    }
    
    if (rollover) {
        save();
    }
    
    return rollover;
}

void EnergyAccumulator::updatePeriodTotals() {
    todayEnergy = importActiveEnergy[0] - dayStartEnergy;
    thisWeekEnergy = importActiveEnergy[0] - weekStartEnergy;
    thisMonthEnergy = importActiveEnergy[0] - monthStartEnergy;
}

bool EnergyAccumulator::needsPeriodicSave() const {
    unsigned long now = millis();
    return ((now - lastSaveTime) >= (persistInterval * 1000));
}

bool EnergyAccumulator::save() {
    if (!initialized) {
        return false;
    }
    
    // Save energy accumulators
    preferences.putBytes("impAct", importActiveEnergy, sizeof(importActiveEnergy));
    preferences.putBytes("expAct", exportActiveEnergy, sizeof(exportActiveEnergy));
    preferences.putBytes("impReact", importReactiveEnergy, sizeof(importReactiveEnergy));
    preferences.putBytes("expReact", exportReactiveEnergy, sizeof(exportReactiveEnergy));
    preferences.putBytes("apparent", apparentEnergy, sizeof(apparentEnergy));
    
    // Save daily tracking
    preferences.putUChar("day", currentDay);
    preferences.putUChar("week", currentWeek);
    preferences.putUChar("month", currentMonth);
    preferences.putFloat("todayEn", todayEnergy);
    preferences.putFloat("yesterEn", yesterdayEnergy);
    preferences.putFloat("weekEn", thisWeekEnergy);
    preferences.putFloat("monthEn", thisMonthEnergy);
    preferences.putFloat("dayStart", dayStartEnergy);
    preferences.putFloat("weekStart", weekStartEnergy);
    preferences.putFloat("monthStart", monthStartEnergy);
    
    lastSaveTime = millis();
    return true;
}

bool EnergyAccumulator::load() {
    if (!initialized) {
        if (!preferences.begin(NVS_NAMESPACE, true)) {
            return false;
        }
    }
    
    // Load energy accumulators
    preferences.getBytes("impAct", importActiveEnergy, sizeof(importActiveEnergy));
    preferences.getBytes("expAct", exportActiveEnergy, sizeof(exportActiveEnergy));
    preferences.getBytes("impReact", importReactiveEnergy, sizeof(importReactiveEnergy));
    preferences.getBytes("expReact", exportReactiveEnergy, sizeof(exportReactiveEnergy));
    preferences.getBytes("apparent", apparentEnergy, sizeof(apparentEnergy));
    
    // Load daily tracking
    currentDay = preferences.getUChar("day", 0);
    currentWeek = preferences.getUChar("week", 0);
    currentMonth = preferences.getUChar("month", 0);
    todayEnergy = preferences.getFloat("todayEn", 0);
    yesterdayEnergy = preferences.getFloat("yesterEn", 0);
    thisWeekEnergy = preferences.getFloat("weekEn", 0);
    thisMonthEnergy = preferences.getFloat("monthEn", 0);
    dayStartEnergy = preferences.getFloat("dayStart", 0);
    weekStartEnergy = preferences.getFloat("weekStart", 0);
    monthStartEnergy = preferences.getFloat("monthStart", 0);
    
    if (!initialized) {
        preferences.end();
    }
    
    return true;
}

bool EnergyAccumulator::reset(uint32_t authCode) {
    if (authCode != RESET_AUTH_CODE) {
        return false;
    }
    
    // Reset all accumulators
    for (int i = 0; i < 4; i++) {
        importActiveEnergy[i] = 0;
        exportActiveEnergy[i] = 0;
        importReactiveEnergy[i] = 0;
        exportReactiveEnergy[i] = 0;
        apparentEnergy[i] = 0;
        prevImportActiveEnergy[i] = 0;
        prevExportActiveEnergy[i] = 0;
        prevImportReactiveEnergy[i] = 0;
        prevExportReactiveEnergy[i] = 0;
        prevApparentEnergy[i] = 0;
    }
    
    // Reset daily tracking
    todayEnergy = 0;
    yesterdayEnergy = 0;
    thisWeekEnergy = 0;
    thisMonthEnergy = 0;
    dayStartEnergy = 0;
    weekStartEnergy = 0;
    monthStartEnergy = 0;
    
    return save();
}

float EnergyAccumulator::getImportActiveEnergy(uint8_t phase) const {
    if (phase > 3) return 0;
    return importActiveEnergy[phase];
}

float EnergyAccumulator::getExportActiveEnergy(uint8_t phase) const {
    if (phase > 3) return 0;
    return exportActiveEnergy[phase];
}

float EnergyAccumulator::getImportReactiveEnergy(uint8_t phase) const {
    if (phase > 3) return 0;
    return importReactiveEnergy[phase];
}

float EnergyAccumulator::getExportReactiveEnergy(uint8_t phase) const {
    if (phase > 3) return 0;
    return exportReactiveEnergy[phase];
}

float EnergyAccumulator::getApparentEnergy(uint8_t phase) const {
    if (phase > 3) return 0;
    return apparentEnergy[phase];
}

void EnergyAccumulator::setPersistInterval(uint32_t intervalSeconds) {
    persistInterval = intervalSeconds;
}

uint32_t EnergyAccumulator::getTimeSinceLastSave() const {
    return (millis() - lastSaveTime) / 1000;
}
