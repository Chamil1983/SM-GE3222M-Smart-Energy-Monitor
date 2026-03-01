#include "DHTSensorManager.h"

#include <math.h>
#include <DHT.h>

#include "PinMap.h"
#include "ConfigManager.h"
#include "Logger.h"
#include "EnergyMeter.h"

#ifndef DHTTYPE
#define DHTTYPE DHT22
#endif

namespace {
DHT g_dht(PIN_DHT, DHTTYPE);
}

DHTSensorManager& DHTSensorManager::getInstance() {
    static DHTSensorManager instance;
    return instance;
}

DHTSensorManager::DHTSensorManager()
    : _mutex(xSemaphoreCreateMutex()), _nextReadMs(0), _lastVerboseLogMs(0), _cfgIntervalMs(3000), _cfgEnabled(true), _cfgDebugLogging(false) {
    _snap.pin = PIN_DHT;
}

bool DHTSensorManager::init() {
    if (_mutex == nullptr) {
        Logger::getInstance().error("DHT22: Failed to create mutex");
        return false;
    }

    g_dht.begin();

    _cfgEnabled = isConfiguredEnabled();
    _cfgIntervalMs = getConfiguredIntervalMs();
    _cfgDebugLogging = isConfiguredDebugLogging();

    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
        _snap.initialized = true;
        _snap.enabled = _cfgEnabled;
        _snap.pin = PIN_DHT;
        _snap.valid = false;
        _nextReadMs = millis() + 1200; // first read after startup settles
        xSemaphoreGive(_mutex);
    }

    Logger::getInstance().info("DHT22: Initialized on GPIO%u (enabled=%s, interval=%lums)",
                               (unsigned)PIN_DHT,
                               _cfgEnabled ? "true" : "false",
                               (unsigned long)_cfgIntervalMs);
    return true;
}

bool DHTSensorManager::isConfiguredEnabled() const {
    SystemConfig cfg;
    if (ConfigManager::getInstance().getSystemConfig(cfg)) {
        return cfg.dhtEnabled;
    }
    return true;
}

bool DHTSensorManager::isConfiguredDebugLogging() const {
    SystemConfig cfg;
    if (ConfigManager::getInstance().getSystemConfig(cfg)) {
        return cfg.dhtDebugLogging;
    }
    return false;
}

uint32_t DHTSensorManager::getConfiguredIntervalMs() const {
    SystemConfig cfg;
    if (ConfigManager::getInstance().getSystemConfig(cfg)) {
        uint32_t v = cfg.dhtReadIntervalMs;
        if (v < 2000UL) v = 2000UL;
        return v;
    }
    return 3000UL;
}

bool DHTSensorManager::reloadConfig(bool forceImmediateRead) {
    const bool newEnabled = isConfiguredEnabled();
    const uint32_t newInterval = getConfiguredIntervalMs();
    const bool newDebug = isConfiguredDebugLogging();

    bool changed = (newEnabled != _cfgEnabled) || (newInterval != _cfgIntervalMs) || (newDebug != _cfgDebugLogging);

    _cfgEnabled = newEnabled;
    _cfgIntervalMs = (newInterval < 2000UL ? 2000UL : newInterval);
    _cfgDebugLogging = newDebug;

    if (_mutex && xSemaphoreTake(_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
        _snap.enabled = _cfgEnabled;
        if (!_cfgEnabled) {
            _snap.valid = false; // keep last values but indicate disabled/not-valid for UI status
        }
        xSemaphoreGive(_mutex);
    }

    const uint32_t now = millis();
    _nextReadMs = forceImmediateRead ? now : (now + 200UL);

    if (changed || forceImmediateRead) {
        Logger::getInstance().info("DHT22: Config reload (enabled=%s, interval=%lums, debug=%s, immediate=%s)",
                                   _cfgEnabled ? "true" : "false",
                                   (unsigned long)_cfgIntervalMs,
                                   _cfgDebugLogging ? "true" : "false",
                                   forceImmediateRead ? "true" : "false");
    }

    if (forceImmediateRead && _cfgEnabled) {
        return readNow(now, true);
    }
    return true;
}

bool DHTSensorManager::isEnabled() const {
    return isConfiguredEnabled();
}

void DHTSensorManager::poll() {
    const uint32_t now = millis();

    static uint32_t s_lastCfgRefreshMs = 0;
    if ((uint32_t)(now - s_lastCfgRefreshMs) >= 5000UL) {
        s_lastCfgRefreshMs = now;
        // Low-cost safety refresh in case config changed via another code path.
        (void)reloadConfig(false);
    }

    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        _snap.enabled = _cfgEnabled;
        xSemaphoreGive(_mutex);
    }

    if (!_cfgEnabled) {
        return;
    }

    if ((int32_t)(now - _nextReadMs) >= 0) {
        readNow(now, false);
    }
}

bool DHTSensorManager::forceRead() {
    return readNow(millis(), true);
}

bool DHTSensorManager::readNow(uint32_t nowMs, bool forced) {
    const uint32_t intervalMs = _cfgIntervalMs;
    _nextReadMs = nowMs + intervalMs;

    if (!_cfgEnabled) {
        if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
            _snap.enabled = false;
            xSemaphoreGive(_mutex);
        }
        return false;
    }

    float h = g_dht.readHumidity();
    float t = g_dht.readTemperature(); // Celsius

    bool ok = !isnan(h) && !isnan(t) && h >= 0.0f && h <= 100.0f && t > -50.0f && t < 125.0f;

    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(50)) != pdTRUE) {
        Logger::getInstance().warn("DHT22: Mutex timeout during read result update");
        return false;
    }

    _snap.initialized = true;
    _snap.enabled = true;
    _snap.lastReadMs = nowMs;

    if (ok) {
        _snap.temperatureC = t;
        _snap.humidityPct = h;
        _snap.valid = true;
        _snap.lastGoodReadMs = nowMs;
        _snap.successCount++;
        _snap.consecutiveFailures = 0;
    } else {
        _snap.failCount++;
        _snap.consecutiveFailures++;
        // keep last good values but flag status invalid until next success
        _snap.valid = false;
    }

    Snapshot snapCopy = _snap;
    xSemaphoreGive(_mutex);

    if (ok) {
        // Publish into meter snapshot for MQTT/Web/Modbus/DataLogger consumers.
        EnergyMeter::getInstance().setAmbientReadings(t, h, true);

        const bool verbose = _cfgDebugLogging;
        if (forced || verbose || (uint32_t)(nowMs - _lastVerboseLogMs) >= 30000UL) {
            _lastVerboseLogMs = nowMs;
            Logger::getInstance().info("DHT22: T=%.1fC RH=%.1f%% (ok=%lu fail=%lu)",
                                       t, h,
                                       (unsigned long)snapCopy.successCount,
                                       (unsigned long)snapCopy.failCount);
        }
    } else {
        if (forced || snapCopy.consecutiveFailures == 1 || (snapCopy.consecutiveFailures % 5) == 0) {
            Logger::getInstance().warn("DHT22: Read failed (NaN/invalid) [consecutive=%lu totalFail=%lu]",
                                       (unsigned long)snapCopy.consecutiveFailures,
                                       (unsigned long)snapCopy.failCount);
        }
    }

    return ok;
}

DHTSensorManager::Snapshot DHTSensorManager::getSnapshot() const {
    Snapshot s;
    if (_mutex && xSemaphoreTake(_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
        s = _snap;
        xSemaphoreGive(_mutex);
    }
    return s;
}
