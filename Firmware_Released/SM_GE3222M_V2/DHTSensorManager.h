#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "DataTypes.h"

class DHTSensorManager {
public:
    struct Snapshot {
        bool initialized;
        bool enabled;
        bool valid;
        uint8_t pin;
        float temperatureC;
        float humidityPct;
        uint32_t lastReadMs;
        uint32_t lastGoodReadMs;
        uint32_t successCount;
        uint32_t failCount;
        uint32_t consecutiveFailures;

        Snapshot()
            : initialized(false), enabled(false), valid(false), pin(0),
              temperatureC(0.0f), humidityPct(0.0f),
              lastReadMs(0), lastGoodReadMs(0),
              successCount(0), failCount(0), consecutiveFailures(0) {}
    };

    static DHTSensorManager& getInstance();

    bool init();
    void poll();          // non-blocking scheduler wrapper (reads when due)
    bool forceRead();     // immediate read attempt
    bool reloadConfig(bool forceImmediateRead = false); // apply updated SystemConfig at runtime
    Snapshot getSnapshot() const;
    bool isEnabled() const;

private:
    DHTSensorManager();
    ~DHTSensorManager() = default;
    DHTSensorManager(const DHTSensorManager&) = delete;
    DHTSensorManager& operator=(const DHTSensorManager&) = delete;

    bool readNow(uint32_t nowMs, bool forced);
    uint32_t getConfiguredIntervalMs() const;
    bool isConfiguredEnabled() const;
    bool isConfiguredDebugLogging() const;

    mutable SemaphoreHandle_t _mutex;
    Snapshot _snap;
    uint32_t _nextReadMs;
    uint32_t _lastVerboseLogMs;
    uint32_t _cfgIntervalMs;
    bool _cfgEnabled;
    bool _cfgDebugLogging;
};
