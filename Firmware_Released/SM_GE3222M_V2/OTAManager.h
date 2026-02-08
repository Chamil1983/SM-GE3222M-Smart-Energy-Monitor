/**
 * @file OTAManager.h
 * @brief Over-The-Air update manager using ArduinoOTA
 * @details Singleton class for handling firmware updates over WiFi
 */

#ifndef OTAMANAGER_H
#define OTAMANAGER_H

#include <ArduinoOTA.h>
#include <functional>

typedef std::function<void()> OTACallback;
typedef std::function<void(unsigned int progress, unsigned int total)> OTAProgressCallback;
typedef std::function<void(ota_error_t error)> OTAErrorCallback;

class OTAManager {
public:
    static OTAManager& getInstance();
    
    bool begin(const char* hostname, const char* password = nullptr);
    void handle();
    void end();
    
    void setOnStart(OTACallback callback);
    void setOnEnd(OTACallback callback);
    void setOnProgress(OTAProgressCallback callback);
    void setOnError(OTAErrorCallback callback);
    
    bool isRunning() const { return _running; }
    
private:
    OTAManager();
    ~OTAManager();
    OTAManager(const OTAManager&) = delete;
    OTAManager& operator=(const OTAManager&) = delete;
    
    static void handleStart();
    static void handleEnd();
    static void handleProgress(unsigned int progress, unsigned int total);
    static void handleError(ota_error_t error);
    
    bool _initialized;
    bool _running;
    
    OTACallback _onStart;
    OTACallback _onEnd;
    OTAProgressCallback _onProgress;
    OTAErrorCallback _onError;
};

#endif // OTAMANAGER_H
