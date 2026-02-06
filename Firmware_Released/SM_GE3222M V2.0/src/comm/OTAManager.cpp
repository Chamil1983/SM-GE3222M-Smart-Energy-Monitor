/**
 * @file OTAManager.cpp
 * @brief OTA firmware update manager implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "OTAManager.h"
#include "../core/ErrorHandler.h"
#include "../../include/Version.h"
#include <esp_task_wdt.h>

// Singleton instance
OTAManager& OTAManager::getInstance() {
    static OTAManager instance;
    return instance;
}

// Constructor
OTAManager::OTAManager()
    : otaPort(DEFAULT_OTA_PORT),
      initialized(false),
      updating(false),
      progress(0) {
    
    strcpy(hostname, "SM-GE3222M");
    strcpy(password, "admin");
    memset(errorMessage, 0, sizeof(errorMessage));
}

// Destructor
OTAManager::~OTAManager() {
    stop();
}

// Initialize OTA manager
bool OTAManager::init() {
    if (initialized) {
        return true;
    }
    
    // Setup ArduinoOTA
    setupArduinoOTA();
    
    initialized = true;
    
    return true;
}

// Start OTA service
bool OTAManager::begin() {
    if (!initialized) {
        return false;
    }
    
    // Begin ArduinoOTA
    ArduinoOTA.begin();
    
    return true;
}

// Stop OTA service
void OTAManager::stop() {
    if (initialized) {
        ArduinoOTA.end();
        initialized = false;
    }
}

// Main loop update
void OTAManager::update() {
    if (!initialized || updating) {
        return;
    }
    
    // Handle ArduinoOTA
    ArduinoOTA.handle();
}

// Set OTA password
void OTAManager::setPassword(const char* pwd) {
    if (pwd && strlen(pwd) > 0) {
        strncpy(password, pwd, sizeof(password) - 1);
        password[sizeof(password) - 1] = '\0';
        
        if (initialized) {
            ArduinoOTA.setPassword(password);
        }
    }
}

// Set hostname
void OTAManager::setHostname(const char* name) {
    if (name && strlen(name) > 0) {
        strncpy(hostname, name, sizeof(hostname) - 1);
        hostname[sizeof(hostname) - 1] = '\0';
        
        if (initialized) {
            ArduinoOTA.setHostname(hostname);
        }
    }
}

// Set OTA port
void OTAManager::setPort(uint16_t port) {
    otaPort = port;
    
    if (initialized) {
        ArduinoOTA.setPort(otaPort);
    }
}

// Setup ArduinoOTA
void OTAManager::setupArduinoOTA() {
    // Set hostname
    ArduinoOTA.setHostname(hostname);
    
    // Set password
    if (strlen(password) > 0) {
        ArduinoOTA.setPassword(password);
    }
    
    // Set port
    ArduinoOTA.setPort(otaPort);
    
    // Setup callbacks
    ArduinoOTA.onStart([this]() {
        handleOTAStart();
    });
    
    ArduinoOTA.onEnd([this]() {
        handleOTAEnd();
    });
    
    ArduinoOTA.onProgress([this](unsigned int prog, unsigned int total) {
        handleOTAProgress(prog, total);
    });
    
    ArduinoOTA.onError([this](ota_error_t error) {
        handleOTAError(error);
    });
}

// Handle OTA start
void OTAManager::handleOTAStart() {
    updating = true;
    progress = 0;
    memset(errorMessage, 0, sizeof(errorMessage));
    
    // Disable watchdog during update
    disableWatchdog();
    
    // Backup firmware if needed
    // backupFirmware();
    
    // Determine update type
    String type = "sketch";
    if (ArduinoOTA.getCommand() == U_SPIFFS) {
        type = "filesystem";
    }
    
    // Call user callback
    if (startCallback) {
        startCallback();
    }
}

// Handle OTA end
void OTAManager::handleOTAEnd() {
    updating = false;
    progress = 100;
    
    // Re-enable watchdog
    enableWatchdog();
    
    // Call user callback
    if (endCallback) {
        endCallback();
    }
}

// Handle OTA progress
void OTAManager::handleOTAProgress(unsigned int prog, unsigned int total) {
    progress = (prog * 100) / total;
    
    // Call user callback
    if (progressCallback) {
        progressCallback(progress);
    }
}

// Handle OTA error
void OTAManager::handleOTAError(ota_error_t error) {
    updating = false;
    
    // Convert error code to message
    switch (error) {
        case OTA_AUTH_ERROR:
            strcpy(errorMessage, "OTA Auth Failed");
            break;
        case OTA_BEGIN_ERROR:
            strcpy(errorMessage, "OTA Begin Failed");
            break;
        case OTA_CONNECT_ERROR:
            strcpy(errorMessage, "OTA Connect Failed");
            break;
        case OTA_RECEIVE_ERROR:
            strcpy(errorMessage, "OTA Receive Failed");
            break;
        case OTA_END_ERROR:
            strcpy(errorMessage, "OTA End Failed");
            break;
        default:
            strcpy(errorMessage, "OTA Unknown Error");
            break;
    }
    
    // Re-enable watchdog
    enableWatchdog();
    
    // Call user callback
    if (errorCallback) {
        errorCallback(errorMessage);
    }
    
    ErrorHandler::getInstance().setError(ErrorCode::NONE, errorMessage);
}

// Begin HTTP OTA update
bool OTAManager::beginUpdate(size_t fileSize) {
    if (updating) {
        return false;
    }
    
    // Disable watchdog
    disableWatchdog();
    
    // Begin update
    if (!Update.begin(fileSize)) {
        strcpy(errorMessage, "Not enough space for update");
        enableWatchdog();
        return false;
    }
    
    updating = true;
    progress = 0;
    
    // Call user callback
    if (startCallback) {
        startCallback();
    }
    
    return true;
}

// Write update data
bool OTAManager::writeUpdate(uint8_t* data, size_t len) {
    if (!updating) {
        return false;
    }
    
    size_t written = Update.write(data, len);
    
    if (written != len) {
        strcpy(errorMessage, "Update write failed");
        return false;
    }
    
    // Update progress
    progress = (Update.progress() * 100) / Update.size();
    
    if (progressCallback) {
        progressCallback(progress);
    }
    
    return true;
}

// End HTTP OTA update
bool OTAManager::endUpdate() {
    if (!updating) {
        return false;
    }
    
    if (!Update.end(true)) {
        strcpy(errorMessage, "Update end failed");
        updating = false;
        enableWatchdog();
        return false;
    }
    
    updating = false;
    progress = 100;
    
    // Re-enable watchdog
    enableWatchdog();
    
    // Call user callback
    if (endCallback) {
        endCallback();
    }
    
    return true;
}

// Abort update
void OTAManager::abortUpdate() {
    if (updating) {
        Update.abort();
        updating = false;
        progress = 0;
        enableWatchdog();
    }
}

// Set start callback
void OTAManager::onStart(std::function<void()> callback) {
    startCallback = callback;
}

// Set progress callback
void OTAManager::onProgress(std::function<void(uint8_t)> callback) {
    progressCallback = callback;
}

// Set end callback
void OTAManager::onEnd(std::function<void()> callback) {
    endCallback = callback;
}

// Set error callback
void OTAManager::onError(std::function<void(const char*)> callback) {
    errorCallback = callback;
}

// Get version string
const char* OTAManager::getVersionString() const {
    #ifdef FIRMWARE_VERSION
        return FIRMWARE_VERSION;
    #else
        return "2.0.0";
    #endif
}

// Check if new version is newer
bool OTAManager::checkVersion(const char* newVersion) {
    if (!newVersion) {
        return false;
    }
    
    // Simple version comparison
    // Format expected: "X.Y.Z"
    int newMajor = 0, newMinor = 0, newPatch = 0;
    int curMajor = 0, curMinor = 0, curPatch = 0;
    
    sscanf(newVersion, "%d.%d.%d", &newMajor, &newMinor, &newPatch);
    sscanf(getVersionString(), "%d.%d.%d", &curMajor, &curMinor, &curPatch);
    
    if (newMajor > curMajor) return true;
    if (newMajor < curMajor) return false;
    
    if (newMinor > curMinor) return true;
    if (newMinor < curMinor) return false;
    
    if (newPatch > curPatch) return true;
    
    return false;
}

// Backup current firmware
bool OTAManager::backupFirmware() {
    // TODO: Implement firmware backup to SPIFFS or external storage
    // This would copy the current firmware partition to a backup location
    return true;
}

// Disable watchdog timer during OTA
void OTAManager::disableWatchdog() {
    #ifdef CONFIG_TASK_WDT_TIMEOUT_S
        esp_task_wdt_delete(NULL);
    #endif
}

// Re-enable watchdog timer after OTA
void OTAManager::enableWatchdog() {
    #ifdef CONFIG_TASK_WDT_TIMEOUT_S
        esp_task_wdt_add(NULL);
    #endif
}
