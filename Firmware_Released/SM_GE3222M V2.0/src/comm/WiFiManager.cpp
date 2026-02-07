#include "WiFiManager.h"
#include "../core/ErrorHandler.h"
#include <esp_mac.h>

// Singleton instance
WiFiManager& WiFiManager::getInstance() {
    static WiFiManager instance;
    return instance;
}

WiFiManager::WiFiManager()
    : initialized_(false),
    apModeActive_(false),
    staConnected_(false),
    reconnectAttempts_(0),
    lastReconnectAttempt_(0),
    reconnectDelay_(BASE_RECONNECT_DELAY),
    mdnsStarted_(false),
    eventHandlerId_(0) {
}

bool WiFiManager::init(const WiFiManagerConfig& config) {
    if (initialized_) {
        return true;
    }

    config_ = config;

    // Set WiFi mode to STA
    WiFi.mode(WIFI_STA);

    // Set hostname if provided
    if (!config_.hostname.isEmpty()) {
        WiFi.setHostname(config_.hostname.c_str());
    }

    // Register WiFi event handler
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        getInstance().handleWiFiEvent(event);
        });

    initialized_ = true;

    ErrorHandler::getInstance().logInfo("WiFiManager initialized");
    return true;
}

bool WiFiManager::connect() {
    if (!initialized_ || config_.ssid.isEmpty()) {
        ErrorHandler::getInstance().logError(ErrorCode::WIFI_INIT_FAILED, "WiFi not initialized or no SSID configured");
        return false;
    }

    disconnect(); // Clean disconnect first

    // Configure static IP if requested
    if (config_.useStaticIP) {
        if (!WiFi.config(config_.staticIP, config_.gateway, config_.subnet,
            config_.dns1, config_.dns2)) {
            ErrorHandler::getInstance().logWarning("Failed to configure static IP");
        }
    }

    // Begin connection
    WiFi.begin(config_.ssid.c_str(), config_.password.c_str());

    ErrorHandler::getInstance().logInfo("Connecting to WiFi: " + config_.ssid);

    // Wait for connection (non-blocking timeout)
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(100);
    }

    if (WiFi.status() == WL_CONNECTED) {
        staConnected_ = true;
        reconnectAttempts_ = 0;
        reconnectDelay_ = BASE_RECONNECT_DELAY;
        ErrorHandler::getInstance().logInfo("WiFi connected: " + WiFi.localIP().toString());
        return true;
    }

    ErrorHandler::getInstance().logWarning("WiFi connection timeout");
    return false;
}

void WiFiManager::disconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect(true);
        staConnected_ = false;
    }
}

bool WiFiManager::startAP() {
    String apSSID = generateAPSSID();

    // Configure AP mode
    WiFi.mode(WIFI_AP_STA);

    IPAddress apIP(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);

    if (!WiFi.softAPConfig(apIP, apIP, subnet)) {
        ErrorHandler::getInstance().logError(ErrorCode::WIFI_AP_START_FAILED, "Failed to configure AP");
        return false;
    }

    // Start AP with random channel
    int channel = (random(3) * 5) + 1;
    if (!WiFi.softAP(apSSID.c_str(), "", channel)) {
        ErrorHandler::getInstance().logError(ErrorCode::WIFI_AP_START_FAILED, "Failed to start AP");
        return false;
    }

    setupCaptivePortal();
    apModeActive_ = true;

    ErrorHandler::getInstance().logInfo("AP started: " + apSSID + " IP: " + WiFi.softAPIP().toString());
    return true;
}

void WiFiManager::stopAP() {
    if (apModeActive_) {
        dnsServer_.stop();
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
        apModeActive_ = false;
        ErrorHandler::getInstance().logInfo("AP stopped");
    }
}

bool WiFiManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiManager::isAPMode() const {
    return apModeActive_;
}

String WiFiManager::getSSID() const {
    return WiFi.SSID();
}

int WiFiManager::getRSSI() const {
    return WiFi.RSSI();
}

IPAddress WiFiManager::getIP() const {
    if (apModeActive_) {
        return WiFi.softAPIP();
    }
    return WiFi.localIP();
}

String WiFiManager::getMACAddress() const {
    uint8_t baseMac[6];
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);

    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
        baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

    return String(macStr);
}

String WiFiManager::getHostname() const {
    return String(WiFi.getHostname());
}

bool WiFiManager::setupMDNS(const char* hostname) {
    if (mdnsStarted_) {
        MDNS.end();
    }

    if (MDNS.begin(hostname)) {
        MDNS.addService("http", "tcp", 80);
        mdnsStarted_ = true;
        ErrorHandler::getInstance().logInfo("mDNS started: " + String(hostname) + ".local");
        return true;
    }

    ErrorHandler::getInstance().logWarning("Failed to start mDNS");
    return false;
}

void WiFiManager::configureNTP(const char* ntpServer, long gmtOffset, int daylightOffset) {
    configTime(gmtOffset, daylightOffset, ntpServer);
    ErrorHandler::getInstance().logInfo("NTP configured: " + String(ntpServer));
}

void WiFiManager::update() {
    // Process DNS requests in AP mode
    if (apModeActive_) {
        dnsServer_.processNextRequest();
    }

    // Auto-reconnect logic
    if (initialized_ && !isConnected() && !config_.ssid.isEmpty()) {
        attemptReconnect();
    }
}

String WiFiManager::getRSSIDescription() const {
    int rssi = getRSSI();

    if (rssi >= -30) return "Excellent";
    if (rssi >= -50) return "Very Good";
    if (rssi >= -60) return "Good";
    if (rssi >= -70) return "Fair";
    if (rssi >= -80) return "Weak";
    if (rssi >= -90) return "Very Weak";
    return "No Signal";
}

// Private methods

void WiFiManager::handleWiFiEvent(WiFiEvent_t event) {
    switch (event) {
    case ARDUINO_EVENT_WIFI_STA_START:
        ErrorHandler::getInstance().logInfo("WiFi STA started");
        break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        ErrorHandler::getInstance().logInfo("WiFi connected to: " + config_.ssid);
        staConnected_ = true;
        reconnectAttempts_ = 0;
        reconnectDelay_ = BASE_RECONNECT_DELAY;
        break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        ErrorHandler::getInstance().logInfo("Got IP: " + WiFi.localIP().toString());
        break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        ErrorHandler::getInstance().logWarning("WiFi disconnected");
        staConnected_ = false;
        break;

    case ARDUINO_EVENT_WIFI_AP_START:
        ErrorHandler::getInstance().logInfo("WiFi AP started");
        break;

    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
        ErrorHandler::getInstance().logInfo("Client connected to AP");
        break;

    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
        ErrorHandler::getInstance().logInfo("Client disconnected from AP");
        break;

    default:
        break;
    }
}

void WiFiManager::attemptReconnect() {
    unsigned long now = millis();

    if (now - lastReconnectAttempt_ >= reconnectDelay_) {
        lastReconnectAttempt_ = now;
        reconnectAttempts_++;

        ErrorHandler::getInstance().logInfo("Reconnect attempt #" + String(reconnectAttempts_));

        WiFi.reconnect();

        // Exponential backoff
        reconnectDelay_ = min(reconnectDelay_ * 2, MAX_RECONNECT_DELAY);

        // Start AP after 3 failed attempts
        if (reconnectAttempts_ == 3 && !apModeActive_) {
            ErrorHandler::getInstance().logWarning("Multiple reconnect failures, starting AP mode");
            startAP();
        }
    }
}

uint32_t WiFiManager::getNextReconnectDelay() {
    uint32_t delay = BASE_RECONNECT_DELAY;
    for (uint32_t i = 0; i < reconnectAttempts_ && delay < MAX_RECONNECT_DELAY; i++) {
        delay *= 2;
    }
    return min(delay, MAX_RECONNECT_DELAY);
}

String WiFiManager::generateAPSSID() {
    String mac = getMACAddress();
    mac.replace(":", "");
    String last4 = mac.substring(mac.length() - 4);
    return "SM-GE3222M-" + last4;
}

void WiFiManager::setupCaptivePortal() {
    dnsServer_.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer_.start(53, "*", WiFi.softAPIP());
    ErrorHandler::getInstance().logInfo("Captive portal DNS started");
}