/**
 * @file NTPSync.cpp
 * @brief NTP time synchronization implementation
 */

#include "NTPSync.h"
#include "Logger.h"
#include <sys/time.h>

NTPSync& NTPSync::getInstance() {
    static NTPSync instance;
    return instance;
}

NTPSync::NTPSync()
    : _initialized(false)
    , _synced(false)
    , _timezoneOffsetMinutes(0)
    , _lastSyncTime(0) {
    memset(_ntpServer, 0, sizeof(_ntpServer));
}

NTPSync::~NTPSync() {
}

bool NTPSync::init(const char* ntpServer, int16_t timezoneOffsetMinutes) {
    Logger& logger = Logger::getInstance();
    
    if (_initialized) {
        logger.warn("NTPSync: Already initialized");
        return true;
    }
    
    if (!ntpServer || strlen(ntpServer) == 0) {
        logger.error("NTPSync: Invalid NTP server");
        return false;
    }
    
    strncpy(_ntpServer, ntpServer, sizeof(_ntpServer) - 1);
    _timezoneOffsetMinutes = timezoneOffsetMinutes;
    
    long gmtOffset_sec = _timezoneOffsetMinutes * 60;
    int daylightOffset_sec = 0;
    
    configTime(gmtOffset_sec, daylightOffset_sec, _ntpServer);
    
    _initialized = true;
    
    logger.info("NTPSync: Initialized with server: %s, offset: %d minutes", 
               _ntpServer, _timezoneOffsetMinutes);
    
    return true;
}

bool NTPSync::sync(uint32_t timeoutMs) {
    Logger& logger = Logger::getInstance();
    
    if (!_initialized) {
        logger.error("NTPSync: Not initialized");
        return false;
    }
    
    logger.info("NTPSync: Starting time synchronization...");
    
    struct tm timeinfo;
    uint32_t startTime = millis();
    
    while (millis() - startTime < timeoutMs) {
        if (getLocalTime(&timeinfo)) {
            if (timeinfo.tm_year > (2020 - 1900)) {
                _synced = true;
                _lastSyncTime = millis();
                
                char timeStr[64];
                strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
                
                logger.info("NTPSync: Time synchronized - %s", timeStr);
                return true;
            }
        }
        delay(100);
    }
    
    logger.error("NTPSync: Time synchronization timeout");
    return false;
}

bool NTPSync::isTimeSynced() const {
    if (!_initialized || !_synced) {
        return false;
    }
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return false;
    }
    
    return timeinfo.tm_year > (2020 - 1900);
}

String NTPSync::getTimeString(const char* format) const {
    struct tm timeinfo;
    
    if (!getLocalTime(&timeinfo)) {
        return "N/A";
    }
    
    char buffer[128];
    strftime(buffer, sizeof(buffer), format, &timeinfo);
    
    return String(buffer);
}

uint32_t NTPSync::getTimestamp() const {
    time_t now;
    time(&now);
    return (uint32_t)now;
}

void NTPSync::setTimezoneOffset(int16_t minutes) {
    _timezoneOffsetMinutes = minutes;
    
    if (_initialized) {
        long gmtOffset_sec = _timezoneOffsetMinutes * 60;
        int daylightOffset_sec = 0;
        
        configTime(gmtOffset_sec, daylightOffset_sec, _ntpServer);
        
        Logger::getInstance().info("NTPSync: Timezone offset updated to %d minutes", 
                                  _timezoneOffsetMinutes);
    }
}

NTPSync::DateTime NTPSync::getDateTime() const {
    DateTime dt = {};
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        dt.year = timeinfo.tm_year + 1900;
        dt.month = timeinfo.tm_mon + 1;
        dt.day = timeinfo.tm_mday;
        dt.hour = timeinfo.tm_hour;
        dt.minute = timeinfo.tm_min;
        dt.second = timeinfo.tm_sec;
        dt.weekday = timeinfo.tm_wday;
    }
    
    return dt;
}
