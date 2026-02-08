/**
 * @file NTPSync.h
 * @brief NTP time synchronization manager
 * @details Singleton class for managing ESP32 NTP time synchronization
 */

#ifndef NTPSYNC_H
#define NTPSYNC_H

#include <Arduino.h>
#include <time.h>

class NTPSync {
public:
    static NTPSync& getInstance();
    
    bool init(const char* ntpServer, int16_t timezoneOffsetMinutes = 0);
    bool sync(uint32_t timeoutMs = 5000);
    
    bool isTimeSynced() const;
    String getTimeString(const char* format = "%Y-%m-%d %H:%M:%S") const;
    uint32_t getTimestamp() const;
    
    void setTimezoneOffset(int16_t minutes);
    int16_t getTimezoneOffset() const { return _timezoneOffsetMinutes; }
    
    struct DateTime {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t weekday;
    };
    
    DateTime getDateTime() const;
    
private:
    NTPSync();
    ~NTPSync();
    NTPSync(const NTPSync&) = delete;
    NTPSync& operator=(const NTPSync&) = delete;
    
    bool _initialized;
    bool _synced;
    char _ntpServer[65];
    int16_t _timezoneOffsetMinutes;
    uint32_t _lastSyncTime;
    
    static const uint32_t RESYNC_INTERVAL = 3600000; // 1 hour
};

#endif // NTPSYNC_H
