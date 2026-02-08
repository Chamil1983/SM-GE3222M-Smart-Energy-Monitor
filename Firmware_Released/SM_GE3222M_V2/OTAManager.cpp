/**
 * @file OTAManager.cpp
 * @brief Over-The-Air update manager implementation
 */

#include "OTAManager.h"
#include "Logger.h"

OTAManager& OTAManager::getInstance() {
    static OTAManager instance;
    return instance;
}

OTAManager::OTAManager()
    : _initialized(false)
    , _running(false)
    , _onStart(nullptr)
    , _onEnd(nullptr)
    , _onProgress(nullptr)
    , _onError(nullptr) {
}

OTAManager::~OTAManager() {
    end();
}

bool OTAManager::begin(const char* hostname, const char* password) {
    Logger& logger = Logger::getInstance();
    
    if (_initialized) {
        logger.warn("OTAManager: Already initialized");
        return true;
    }
    
    if (hostname && strlen(hostname) > 0) {
        ArduinoOTA.setHostname(hostname);
        logger.info("OTAManager: Hostname set to: %s", hostname);
    }
    
    if (password && strlen(password) > 0) {
        ArduinoOTA.setPassword(password);
        logger.info("OTAManager: Password protection enabled");
    }
    
    ArduinoOTA.onStart(handleStart);
    ArduinoOTA.onEnd(handleEnd);
    ArduinoOTA.onProgress(handleProgress);
    ArduinoOTA.onError(handleError);
    
    ArduinoOTA.begin();
    
    _initialized = true;
    _running = true;
    
    logger.info("OTAManager: Started successfully");
    
    return true;
}

void OTAManager::handle() {
    if (_running) {
        ArduinoOTA.handle();
    }
}

void OTAManager::end() {
    if (_running) {
        ArduinoOTA.end();
        _running = false;
        Logger::getInstance().info("OTAManager: Stopped");
    }
}

void OTAManager::setOnStart(OTACallback callback) {
    _onStart = callback;
}

void OTAManager::setOnEnd(OTACallback callback) {
    _onEnd = callback;
}

void OTAManager::setOnProgress(OTAProgressCallback callback) {
    _onProgress = callback;
}

void OTAManager::setOnError(OTAErrorCallback callback) {
    _onError = callback;
}

void OTAManager::handleStart() {
    Logger& logger = Logger::getInstance();
    OTAManager& ota = OTAManager::getInstance();
    
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
    } else {
        type = "filesystem";
    }
    
    logger.info("OTAManager: Update started - Type: %s", type.c_str());
    
    if (ota._onStart) {
        ota._onStart();
    }
}

void OTAManager::handleEnd() {
    Logger& logger = Logger::getInstance();
    OTAManager& ota = OTAManager::getInstance();
    
    logger.info("OTAManager: Update completed successfully");
    
    if (ota._onEnd) {
        ota._onEnd();
    }
}

void OTAManager::handleProgress(unsigned int progress, unsigned int total) {
    OTAManager& ota = OTAManager::getInstance();
    
    static unsigned int lastPercent = 0;
    unsigned int percent = (progress * 100) / total;
    
    if (percent != lastPercent && percent % 10 == 0) {
        Logger::getInstance().info("OTAManager: Progress: %u%%", percent);
        lastPercent = percent;
    }
    
    if (ota._onProgress) {
        ota._onProgress(progress, total);
    }
}

void OTAManager::handleError(ota_error_t error) {
    Logger& logger = Logger::getInstance();
    OTAManager& ota = OTAManager::getInstance();
    
    const char* errorMsg = "Unknown";
    
    switch (error) {
        case OTA_AUTH_ERROR:
            errorMsg = "Auth Failed";
            break;
        case OTA_BEGIN_ERROR:
            errorMsg = "Begin Failed";
            break;
        case OTA_CONNECT_ERROR:
            errorMsg = "Connect Failed";
            break;
        case OTA_RECEIVE_ERROR:
            errorMsg = "Receive Failed";
            break;
        case OTA_END_ERROR:
            errorMsg = "End Failed";
            break;
    }
    
    logger.error("OTAManager: Error[%u]: %s", error, errorMsg);
    
    if (ota._onError) {
        ota._onError(error);
    }
}
