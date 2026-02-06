/**
 * @file PowerQuality.cpp
 * @brief Power quality analysis implementation
 */

#include "PowerQuality.h"
#include <cmath>

PowerQuality::PowerQuality()
    : initialized(false),
      nominalVoltage(230.0f),
      nominalFrequency(50.0f),
      voltageSagThreshold(0.9f),
      voltageSwellThreshold(1.1f),
      thdThreshold(5.0f),
      imbalanceThreshold(2.0f),
      minFrequency(47.0f),
      maxFrequency(53.0f),
      voltageSagActive(false),
      voltageSwellActive(false),
      thdAlarmActive(false),
      frequencyOutOfRange(false),
      currentImbalance(0.0f) {
    // Initialize debounce timers
    for (int i = 0; i < 5; i++) {
        lastEventTime[i] = 0;
    }
}

PowerQuality::~PowerQuality() {
}

PowerQuality& PowerQuality::getInstance() {
    static PowerQuality instance;
    return instance;
}

bool PowerQuality::init(float nominalVoltage, float nominalFrequency) {
    this->nominalVoltage = nominalVoltage;
    this->nominalFrequency = nominalFrequency;
    
    // Set frequency range based on nominal
    if (nominalFrequency > 55.0f) {
        // 60Hz system
        minFrequency = 57.0f;
        maxFrequency = 63.0f;
    } else {
        // 50Hz system
        minFrequency = 47.0f;
        maxFrequency = 53.0f;
    }
    
    events.reserve(MAX_EVENTS);
    initialized = true;
    return true;
}

uint8_t PowerQuality::analyze(const MeterData& meterData) {
    if (!initialized) {
        return 0;
    }
    
    uint8_t eventCount = 0;
    
    // Reset status flags
    voltageSagActive = false;
    voltageSwellActive = false;
    thdAlarmActive = false;
    frequencyOutOfRange = false;
    
    // Detect voltage sag/swell
    detectVoltageSagSwell(meterData);
    
    // Calculate phase imbalance
    currentImbalance = calculatePhaseImbalance(meterData);
    if (currentImbalance > imbalanceThreshold) {
        unsigned long now = millis();
        if (now - lastEventTime[2] > EVENT_DEBOUNCE_MS) {
            PowerQualityEvent event;
            event.type = PowerQualityEvent::Type::PHASE_IMBALANCE;
            event.phase = 0;
            event.value = currentImbalance;
            event.timestamp = now;
            logEvent(event);
            lastEventTime[2] = now;
            eventCount++;
        }
    }
    
    // Check THD
    checkTHD(meterData);
    
    // Check frequency
    checkFrequency(meterData);
    
    return eventCount;
}

void PowerQuality::detectVoltageSagSwell(const MeterData& meterData) {
    float voltages[3] = {
        meterData.phaseA.voltage,
        meterData.phaseB.voltage,
        meterData.phaseC.voltage
    };
    
    for (uint8_t i = 0; i < 3; i++) {
        float perUnit = voltages[i] / nominalVoltage;
        
        // Check sag
        if (perUnit < voltageSagThreshold && perUnit > 0.1f) {
            voltageSagActive = true;
            unsigned long now = millis();
            if (now - lastEventTime[0] > EVENT_DEBOUNCE_MS) {
                PowerQualityEvent event;
                event.type = PowerQualityEvent::Type::VOLTAGE_SAG;
                event.phase = i + 1;
                event.value = perUnit;
                event.timestamp = now;
                logEvent(event);
                lastEventTime[0] = now;
            }
        }
        
        // Check swell
        if (perUnit > voltageSwellThreshold) {
            voltageSwellActive = true;
            unsigned long now = millis();
            if (now - lastEventTime[1] > EVENT_DEBOUNCE_MS) {
                PowerQualityEvent event;
                event.type = PowerQualityEvent::Type::VOLTAGE_SWELL;
                event.phase = i + 1;
                event.value = perUnit;
                event.timestamp = now;
                logEvent(event);
                lastEventTime[1] = now;
            }
        }
    }
}

float PowerQuality::calculatePhaseImbalance(const MeterData& meterData) {
    // NEMA definition: max deviation from average / average * 100
    float voltages[3] = {
        meterData.phaseA.voltage,
        meterData.phaseB.voltage,
        meterData.phaseC.voltage
    };
    
    // Calculate average
    float avg = (voltages[0] + voltages[1] + voltages[2]) / 3.0f;
    
    if (avg < 10.0f) {
        return 0.0f; // Avoid division by zero
    }
    
    // Find max deviation
    float maxDeviation = 0.0f;
    for (int i = 0; i < 3; i++) {
        float deviation = abs(voltages[i] - avg);
        if (deviation > maxDeviation) {
            maxDeviation = deviation;
        }
    }
    
    return (maxDeviation / avg) * 100.0f;
}

void PowerQuality::checkTHD(const MeterData& meterData) {
    float maxTHD = 0.0f;
    uint8_t maxPhase = 0;
    
    if (meterData.phaseA.voltageTHDN > maxTHD) {
        maxTHD = meterData.phaseA.voltageTHDN;
        maxPhase = 1;
    }
    if (meterData.phaseB.voltageTHDN > maxTHD) {
        maxTHD = meterData.phaseB.voltageTHDN;
        maxPhase = 2;
    }
    if (meterData.phaseC.voltageTHDN > maxTHD) {
        maxTHD = meterData.phaseC.voltageTHDN;
        maxPhase = 3;
    }
    
    if (maxTHD > thdThreshold) {
        thdAlarmActive = true;
        unsigned long now = millis();
        if (now - lastEventTime[3] > EVENT_DEBOUNCE_MS) {
            PowerQualityEvent event;
            event.type = PowerQualityEvent::Type::THD_ALARM;
            event.phase = maxPhase;
            event.value = maxTHD;
            event.timestamp = now;
            logEvent(event);
            lastEventTime[3] = now;
        }
    }
}

void PowerQuality::checkFrequency(const MeterData& meterData) {
    if (meterData.frequency < minFrequency || meterData.frequency > maxFrequency) {
        frequencyOutOfRange = true;
        unsigned long now = millis();
        if (now - lastEventTime[4] > EVENT_DEBOUNCE_MS) {
            PowerQualityEvent event;
            event.type = PowerQualityEvent::Type::FREQUENCY_OUT_OF_RANGE;
            event.phase = 0;
            event.value = meterData.frequency;
            event.timestamp = now;
            logEvent(event);
            lastEventTime[4] = now;
        }
    }
}

void PowerQuality::logEvent(const PowerQualityEvent& event) {
    // Circular buffer - remove oldest if at capacity
    if (events.size() >= MAX_EVENTS) {
        events.erase(events.begin());
    }
    
    events.push_back(event);
}

std::vector<PowerQualityEvent> PowerQuality::getEvents(uint8_t count) const {
    std::vector<PowerQualityEvent> result;
    
    if (events.empty()) {
        return result;
    }
    
    // Return last N events
    size_t startIdx = 0;
    if (events.size() > count) {
        startIdx = events.size() - count;
    }
    
    for (size_t i = startIdx; i < events.size(); i++) {
        result.push_back(events[i]);
    }
    
    return result;
}

std::vector<PowerQualityEvent> PowerQuality::getAllEvents() const {
    return events;
}

void PowerQuality::clearEvents() {
    events.clear();
}

void PowerQuality::setVoltageSagThreshold(float threshold) {
    voltageSagThreshold = threshold;
}

void PowerQuality::setVoltageSwellThreshold(float threshold) {
    voltageSwellThreshold = threshold;
}

void PowerQuality::setTHDThreshold(float threshold) {
    thdThreshold = threshold;
}

void PowerQuality::setImbalanceThreshold(float threshold) {
    imbalanceThreshold = threshold;
}
