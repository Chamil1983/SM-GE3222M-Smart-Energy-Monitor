/**
 * @file NetworkManager.cpp
 * @brief WiFi Network Manager implementation
 */

#include "NetworkManager.h"
#include "Logger.h"
#include <esp_wifi.h>

NetworkManager& NetworkManager::getInstance() {
    static NetworkManager instance;
    return instance;
}

NetworkManager::NetworkManager() 
    : _initialized(false)
    , _autoReconnect(true)
    , _isSTAMode(false)
    , _isAPMode(false)
    , _lastReconnectAttempt(0)
    , _reconnectDelay(MIN_RECONNECT_DELAY)
    , _reconnectAttempts(0)
    , _dnsServerRunning(false) {
}

NetworkManager::~NetworkManager() {
    stop();
}

bool NetworkManager::init(const WiFiConfig& config) {
    Logger& logger = Logger::getInstance();
    
    if (_initialized) {
        logger.warn("NetworkManager: Already initialized");
        return true;
    }
    
    _config = config;
    
    if (!_config.enabled) {
        logger.info("NetworkManager: WiFi disabled in config");
        return false;
    }
    
    WiFi.onEvent(onWiFiEvent);
    WiFi.mode(WIFI_OFF);
    delay(100);
    
    String hostname = String(_config.hostname);
    if (hostname.isEmpty()) {
        uint8_t mac[6];
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
        hostname = "GE3222M-" + String(mac[4], HEX) + String(mac[5], HEX);
    }
    WiFi.setHostname(hostname.c_str());
    
    _initialized = true;
    logger.info("NetworkManager: Initialized, hostname: %s", hostname.c_str());
    
    return true;
}

bool NetworkManager::startSTA() {
    Logger& logger = Logger::getInstance();
    
    if (!_initialized) {
        logger.error("NetworkManager: Not initialized");
        return false;
    }
    
    if (strlen(_config.ssid) == 0) {
        logger.error("NetworkManager: No SSID configured");
        return false;
    }
    
    stop();
    
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(_autoReconnect);
    
    if (!_config.useDHCP) {
        if (!applyStaticConfig()) {
            logger.error("NetworkManager: Failed to apply static IP config");
            return false;
        }
    }
    
    logger.info("NetworkManager: Connecting to SSID: %s", _config.ssid);
    
    WiFi.begin(_config.ssid, _config.password);
    
    _isSTAMode = true;
    _isAPMode = false;
    _reconnectAttempts = 0;
    
    return true;
}

bool NetworkManager::startAP(const char* ssid, const char* password) {
    Logger& logger = Logger::getInstance();
    
    if (!_initialized) {
        logger.error("NetworkManager: Not initialized");
        return false;
    }
    
    stop();
    
    WiFi.mode(WIFI_AP);
    
    String apSSID = ssid ? String(ssid) : String(_config.apSSID);
    String apPassword = password ? String(password) : String(_config.apPassword);
    
    if (apSSID.isEmpty()) {
        uint8_t mac[6];
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
        apSSID = "GE3222M-" + String(mac[4], HEX) + String(mac[5], HEX);
    }
    
    IPAddress apIP(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    
    if (!WiFi.softAPConfig(apIP, apIP, subnet)) {
        logger.error("NetworkManager: Failed to configure AP");
        return false;
    }
    
    uint8_t channel = _config.apChannel > 0 ? _config.apChannel : 1;
    
    bool result = WiFi.softAP(apSSID.c_str(), apPassword.c_str(), channel);
    
    if (result) {
        _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        _dnsServer.start(53, "*", apIP);
        _dnsServerRunning = true;
        
        logger.info("NetworkManager: AP started - SSID: %s, IP: %s", 
                   apSSID.c_str(), apIP.toString().c_str());
        
        _isAPMode = true;
        _isSTAMode = false;
    } else {
        logger.error("NetworkManager: Failed to start AP");
    }
    
    return result;
}

void NetworkManager::stop() {
    Logger& logger = Logger::getInstance();
    
    if (_dnsServerRunning) {
        _dnsServer.stop();
        _dnsServerRunning = false;
    }
    
    if (_isAPMode) {
        WiFi.softAPdisconnect(true);
        logger.info("NetworkManager: AP stopped");
    }
    
    if (_isSTAMode) {
        WiFi.disconnect(true);
        logger.info("NetworkManager: STA disconnected");
    }
    
    WiFi.mode(WIFI_OFF);
    
    _isSTAMode = false;
    _isAPMode = false;
}

bool NetworkManager::isConnected() const {
    return _isSTAMode && WiFi.isConnected();
}

String NetworkManager::getLocalIP() const {
    if (_isSTAMode) {
        return WiFi.localIP().toString();
    } else if (_isAPMode) {
        return WiFi.softAPIP().toString();
    }
    return "0.0.0.0";
}

int NetworkManager::getRSSI() const {
    if (isConnected()) {
        return WiFi.RSSI();
    }
    return -100;
}

String NetworkManager::getMACAddress() const {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

String NetworkManager::getSSID() const {
    if (_isSTAMode) {
        return WiFi.SSID();
    } else if (_isAPMode) {
        return String(_config.apSSID);
    }
    return "";
}

void NetworkManager::handle() {
    if (_dnsServerRunning) {
        _dnsServer.processNextRequest();
    }
    
    if (_autoReconnect && _isSTAMode && !isConnected()) {
        attemptReconnect();
    }
}

void NetworkManager::setAutoReconnect(bool enable) {
    _autoReconnect = enable;
    if (_isSTAMode) {
        WiFi.setAutoReconnect(enable);
    }
}

bool NetworkManager::applyStaticConfig() {
    IPAddress ip(_config.staticIP[0], _config.staticIP[1], 
                 _config.staticIP[2], _config.staticIP[3]);
    IPAddress gateway(_config.gateway[0], _config.gateway[1], 
                     _config.gateway[2], _config.gateway[3]);
    IPAddress subnet(_config.subnet[0], _config.subnet[1], 
                    _config.subnet[2], _config.subnet[3]);
    IPAddress dns1(_config.dns1[0], _config.dns1[1], 
                  _config.dns1[2], _config.dns1[3]);
    IPAddress dns2(_config.dns2[0], _config.dns2[1], 
                  _config.dns2[2], _config.dns2[3]);
    
    if (dns1 == IPAddress(0, 0, 0, 0)) {
        dns1 = gateway;
    }
    
    if (dns2 == IPAddress(0, 0, 0, 0)) {
        dns2 = IPAddress(8, 8, 4, 4);
    }
    
    return WiFi.config(ip, gateway, subnet, dns1, dns2);
}

void NetworkManager::attemptReconnect() {
    unsigned long now = millis();
    
    if (now - _lastReconnectAttempt < _reconnectDelay) {
        return;
    }
    
    _lastReconnectAttempt = now;
    _reconnectAttempts++;
    
    if (_reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
        Logger::getInstance().error("NetworkManager: Max reconnect attempts reached");
        _reconnectDelay = MAX_RECONNECT_DELAY;
        _reconnectAttempts = 0;
        return;
    }
    
    _reconnectDelay = min(_reconnectDelay * 2, MAX_RECONNECT_DELAY);
    
    Logger::getInstance().info("NetworkManager: Reconnect attempt %d (delay: %dms)", 
                              _reconnectAttempts, _reconnectDelay);
    
    WiFi.reconnect();
}

String NetworkManager::getRSSIQuality(int rssi) const {
    if (rssi >= -30) return "Excellent";
    if (rssi >= -50) return "Very Good";
    if (rssi >= -60) return "Good";
    if (rssi >= -70) return "Fair";
    if (rssi >= -80) return "Weak";
    return "Very Weak";
}

void NetworkManager::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    Logger& logger = Logger::getInstance();
    NetworkManager& nm = NetworkManager::getInstance();
    
    switch (event) {
        case ARDUINO_EVENT_WIFI_READY:
            logger.debug("NetworkManager: WiFi ready");
            break;
            
        case ARDUINO_EVENT_WIFI_STA_START:
            logger.debug("NetworkManager: STA started");
            break;
            
        case ARDUINO_EVENT_WIFI_STA_STOP:
            logger.debug("NetworkManager: STA stopped");
            break;
            
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            logger.info("NetworkManager: Connected to SSID: %s", WiFi.SSID().c_str());
            nm._reconnectAttempts = 0;
            nm._reconnectDelay = MIN_RECONNECT_DELAY;
            break;
            
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            logger.warn("NetworkManager: Disconnected from WiFi");
            break;
            
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            logger.info("NetworkManager: Got IP: %s", WiFi.localIP().toString().c_str());
            logger.info("NetworkManager: RSSI: %d dBm (%s)", 
                       WiFi.RSSI(), nm.getRSSIQuality(WiFi.RSSI()).c_str());
            break;
            
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            logger.warn("NetworkManager: Lost IP address");
            break;
            
        case ARDUINO_EVENT_WIFI_AP_START:
            logger.info("NetworkManager: AP started");
            break;
            
        case ARDUINO_EVENT_WIFI_AP_STOP:
            logger.info("NetworkManager: AP stopped");
            break;
            
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            logger.info("NetworkManager: Client connected to AP");
            break;
            
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            logger.info("NetworkManager: Client disconnected from AP");
            break;
            
        default:
            break;
    }
}
