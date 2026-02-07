/**
 * @file DisplayManager.cpp
 * @brief Unified display interface implementation
 */

#include "DisplayManager.h"
#include "../core/Logger.h"

DisplayManager& DisplayManager::getInstance() {
    static DisplayManager instance;
    return instance;
}

DisplayManager::DisplayManager()
    : _initialized(false)
    , _currentPage(Page::BOOT)
    , _previousPage(Page::BOOT)
    , _autoRotationEnabled(false)
    , _rotationInterval(5000)
    , _lastRotationTime(0)
    , _errorActive(false)
    , _errorCode(ErrorCode::NONE)
    , _errorStartTime(0)
    , _errorDuration(0)
    , _networkRSSI(-1)
    , _networkMQTT(false)
    , _systemUptime(0)
    , _systemTemp(0)
    , _systemFreeHeap(0)
    , _systemErrorCount(0)
    , _dataUpdated(false) {
    
    strcpy(_errorMessage, "");
    strcpy(_networkIP, "0.0.0.0");
}

bool DisplayManager::init() {
    std::lock_guard<std::mutex> lock(_mutex);

    if (_initialized) {
        return true;
    }

    // Initialize LCD manager
    if (!LCDManager::getInstance().init()) {
        Logger::error("DisplayManager", "Failed to initialize LCDManager");
        return false;
    }

    _initialized = true;
    _currentPage = Page::BOOT;
    _lastRotationTime = millis();

    Logger::info("DisplayManager", "Initialized successfully");
    return true;
}

void DisplayManager::update() {
    if (!_initialized) {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    unsigned long now = millis();

    // Handle error timeout
    if (_errorActive) {
        handleErrorTimeout();
    }

    // Handle auto rotation
    if (_autoRotationEnabled && !_errorActive) {
        if (now - _lastRotationTime >= _rotationInterval) {
            nextPage();
            _lastRotationTime = now;
        }
    }

    // Update underlying LCD
    LCDManager::getInstance().update();
}

void DisplayManager::setPage(Page page) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (page == _currentPage) {
        return;
    }

    _previousPage = _currentPage;
    _currentPage = page;

    // Clear error if switching manually
    if (_errorActive && page != Page::ERROR) {
        _errorActive = false;
    }

    // Update LCD page
    LCDManager::getInstance().setPage(pageToDisplayPage(page));
    
    Logger::debug("DisplayManager", ("Page changed to " + String(static_cast<int>(page))).c_str());
}

void DisplayManager::nextPage() {
    Page nextPage;
    
    switch (_currentPage) {
        case Page::BOOT:
            nextPage = Page::PHASE_A;
            break;
        case Page::PHASE_A:
            nextPage = Page::PHASE_B;
            break;
        case Page::PHASE_B:
            nextPage = Page::PHASE_C;
            break;
        case Page::PHASE_C:
            nextPage = Page::TOTALS;
            break;
        case Page::TOTALS:
            nextPage = Page::ENERGY;
            break;
        case Page::ENERGY:
            nextPage = Page::NETWORK;
            break;
        case Page::NETWORK:
            nextPage = Page::SYSTEM;
            break;
        case Page::SYSTEM:
            nextPage = Page::PHASE_A;  // Loop back
            break;
        default:
            nextPage = Page::PHASE_A;
            break;
    }

    setPage(nextPage);
}

void DisplayManager::previousPage() {
    Page prevPage;
    
    switch (_currentPage) {
        case Page::PHASE_A:
            prevPage = Page::SYSTEM;
            break;
        case Page::PHASE_B:
            prevPage = Page::PHASE_A;
            break;
        case Page::PHASE_C:
            prevPage = Page::PHASE_B;
            break;
        case Page::TOTALS:
            prevPage = Page::PHASE_C;
            break;
        case Page::ENERGY:
            prevPage = Page::TOTALS;
            break;
        case Page::NETWORK:
            prevPage = Page::ENERGY;
            break;
        case Page::SYSTEM:
            prevPage = Page::NETWORK;
            break;
        default:
            prevPage = Page::PHASE_A;
            break;
    }

    setPage(prevPage);
}

void DisplayManager::updateMeterData(const MeterData& data) {
    std::lock_guard<std::mutex> lock(_mutex);
    _meterData = data;
    _dataUpdated = true;
    
    // Update LCD with new data
    LCDManager::getInstance().displayMeterData(data);
}

void DisplayManager::setNetworkInfo(const char* ip, int rssi, bool mqttConnected) {
    std::lock_guard<std::mutex> lock(_mutex);
    
    if (ip != nullptr) {
        strncpy(_networkIP, ip, sizeof(_networkIP) - 1);
        _networkIP[sizeof(_networkIP) - 1] = '\0';
    }
    _networkRSSI = rssi;
    _networkMQTT = mqttConnected;
    
    LCDManager::getInstance().setNetworkInfo(_networkIP, _networkRSSI, _networkMQTT);
}

void DisplayManager::setSystemInfo(uint32_t uptime, float temp, uint32_t freeHeap, uint32_t errorCount) {
    std::lock_guard<std::mutex> lock(_mutex);
    
    _systemUptime = uptime;
    _systemTemp = temp;
    _systemFreeHeap = freeHeap;
    _systemErrorCount = errorCount;
    
    LCDManager::getInstance().setSystemInfo(_systemUptime, _systemTemp, _systemFreeHeap, _systemErrorCount);
}

void DisplayManager::displayMessage(uint8_t line, const char* message) {
    if (!_initialized || message == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    LCDManager::getInstance().displayMessage(line, message);
}

void DisplayManager::showError(ErrorCode errorCode, const char* message, unsigned long duration) {
    if (!_initialized) {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    _errorActive = true;
    _errorCode = errorCode;
    _errorStartTime = millis();
    _errorDuration = duration;

    if (message != nullptr) {
        strncpy(_errorMessage, message, sizeof(_errorMessage) - 1);
        _errorMessage[sizeof(_errorMessage) - 1] = '\0';
    } else {
        strcpy(_errorMessage, "Error occurred");
    }

    // Display error on LCD
    LCDManager& lcd = LCDManager::getInstance();
    lcd.clear();
    lcd.displayMessage(0, "*** ERROR ***");
    lcd.displayMessage(1, _errorMessage);
    
    char errorCodeStr[20];
    snprintf(errorCodeStr, sizeof(errorCodeStr), "Code: %d", static_cast<int>(errorCode));
    lcd.displayMessage(2, errorCodeStr);
    
    if (duration > 0) {
        lcd.displayMessage(3, "Auto-clear...");
    }

    Logger::error("DisplayManager", _errorMessage);
}

void DisplayManager::clearError() {
    std::lock_guard<std::mutex> lock(_mutex);

    if (_errorActive) {
        _errorActive = false;
        _errorCode = ErrorCode::NONE;
        strcpy(_errorMessage, "");
        
        // Restore previous page
        LCDManager::getInstance().setPage(pageToDisplayPage(_previousPage));
    }
}

void DisplayManager::setAutoRotation(bool enabled, uint32_t intervalMs) {
    std::lock_guard<std::mutex> lock(_mutex);
    
    _autoRotationEnabled = enabled;
    _rotationInterval = intervalMs;
    _lastRotationTime = millis();
    
    LCDManager::getInstance().setAutoScroll(enabled, intervalMs);
    
    Logger::info("DisplayManager", enabled ? "Auto-rotation enabled" : "Auto-rotation disabled");
}

void DisplayManager::clear() {
    if (!_initialized) {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    LCDManager::getInstance().clear();
}

void DisplayManager::showBootSplash(const char* version) {
    if (!_initialized) {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    LCDManager& lcd = LCDManager::getInstance();
    lcd.clear();
    lcd.displayMessage(0, "  SM-GE3222M V2.0  ");
    lcd.displayMessage(1, "Smart Energy Monitor");
    
    if (version != nullptr) {
        char versionLine[21];
        snprintf(versionLine, sizeof(versionLine), "FW: %s", version);
        lcd.displayMessage(2, versionLine);
    }
    
    lcd.displayMessage(3, "Initializing...");
    
    _currentPage = Page::BOOT;
}

void DisplayManager::showInitStatus(const char* component, bool success) {
    if (!_initialized || component == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    LCDManager& lcd = LCDManager::getInstance();
    
    char statusLine[21];
    snprintf(statusLine, sizeof(statusLine), "%s: %s", 
             component, success ? "OK" : "FAIL");
    
    lcd.displayMessage(3, statusLine);
    
    delay(500);  // Brief delay to show status
}

DisplayPage DisplayManager::pageToDisplayPage(Page page) {
    switch (page) {
        case Page::BOOT:
            return DisplayPage::SPLASH;
        case Page::PHASE_A:
            return DisplayPage::PHASE_A;
        case Page::PHASE_B:
            return DisplayPage::PHASE_B;
        case Page::PHASE_C:
            return DisplayPage::PHASE_C;
        case Page::TOTALS:
            return DisplayPage::TOTALS;
        case Page::ENERGY:
            return DisplayPage::ENERGY;
        case Page::NETWORK:
            return DisplayPage::NETWORK;
        case Page::SYSTEM:
            return DisplayPage::SYSTEM;
        default:
            return DisplayPage::SPLASH;
    }
}

void DisplayManager::handleErrorTimeout() {
    if (_errorDuration == 0) {
        return;  // Persistent error
    }

    unsigned long now = millis();
    if (now - _errorStartTime >= _errorDuration) {
        clearError();
    }
}
