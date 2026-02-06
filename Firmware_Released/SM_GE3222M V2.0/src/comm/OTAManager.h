/**
 * @file OTAManager.h
 * @brief Over-The-Air firmware update manager
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Handles HTTP OTA and ArduinoOTA firmware updates
 */

#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include <functional>

class OTAManager {
public:
    // Singleton instance
    static OTAManager& getInstance();
    
    // Initialization
    bool init();
    bool begin();
    void stop();
    
    // Main loop update
    void update();
    
    // Configuration
    void setPassword(const char* password);
    void setHostname(const char* hostname);
    void setPort(uint16_t port);
    
    // Status
    bool isInitialized() const { return initialized; }
    bool isUpdating() const { return updating; }
    uint8_t getProgress() const { return progress; }
    const char* getError() const { return errorMessage; }
    
    // HTTP OTA
    bool beginUpdate(size_t fileSize);
    bool writeUpdate(uint8_t* data, size_t len);
    bool endUpdate();
    void abortUpdate();
    
    // Callbacks
    void onStart(std::function<void()> callback);
    void onProgress(std::function<void(uint8_t)> callback);
    void onEnd(std::function<void()> callback);
    void onError(std::function<void(const char*)> callback);
    
    // Utility
    const char* getVersionString() const;
    bool checkVersion(const char* newVersion);
    
private:
    OTAManager();
    ~OTAManager();
    OTAManager(const OTAManager&) = delete;
    OTAManager& operator=(const OTAManager&) = delete;
    
    // Internal methods
    void setupArduinoOTA();
    void handleOTAStart();
    void handleOTAEnd();
    void handleOTAProgress(unsigned int progress, unsigned int total);
    void handleOTAError(ota_error_t error);
    bool backupFirmware();
    void disableWatchdog();
    void enableWatchdog();
    
    // Member variables
    char hostname[32];
    char password[32];
    uint16_t otaPort;
    
    bool initialized;
    bool updating;
    uint8_t progress;
    char errorMessage[128];
    
    // Callbacks
    std::function<void()> startCallback;
    std::function<void(uint8_t)> progressCallback;
    std::function<void()> endCallback;
    std::function<void(const char*)> errorCallback;
    
    // Constants
    static constexpr uint16_t DEFAULT_OTA_PORT = 3232;
    static constexpr size_t UPDATE_BUFFER_SIZE = 1024;
};

#endif // OTAMANAGER_H
