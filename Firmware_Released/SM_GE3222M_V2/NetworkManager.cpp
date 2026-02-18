/**
 * @file NetworkManager.cpp
 * @brief WiFi Network Manager implementation
 * @note  WiFi AP connection fix: proper initialization order, explicit softAP
 *        configuration with channel/auth mode, and deferred custom MAC setting.
 */

#include "NetworkManager.h"
#include "Logger.h"
#include <esp_wifi.h>
#include <algorithm>
#include "esp_mac.h"

 // Fixed SoftAP settings (no configurable AP params in WiFiConfig)
static constexpr const char* kSoftApSsid = "SM_GE3222M_Setup";
static constexpr const char* kSoftApPass = "12345678";
static constexpr const char* kSoftApMac = "C8:2E:A3:F5:7D:DC";

static bool parseMac(const char* str, uint8_t out[6]) {
    if (!str || !out) return false;
    int v[6];
    if (sscanf(str, "%x:%x:%x:%x:%x:%x", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]) != 6) return false;
    for (int i = 0; i < 6; ++i) out[i] = (uint8_t)(v[i] & 0xFF);
    return true;
}

/**
 * Set custom MAC address for SoftAP interface.
 * IMPORTANT: Must be called BEFORE WiFi.mode() or AFTER WiFi.softAP() completes,
 * NOT between WiFi.mode(WIFI_AP) and WiFi.softAP().
 */
static bool setCustomWiFiApMac(const char* macStr) {
    uint8_t macBytes[6];
    if (!parseMac(macStr ? macStr : kSoftApMac, macBytes)) {
        Serial.println("[MAC] Invalid WIFI_AP_MAC format!");
        return false;
    }

    // Set at netif layer (this is safe to call early)
    esp_err_t err1 = esp_iface_mac_addr_set(macBytes, ESP_MAC_WIFI_SOFTAP);

    // Also set at WiFi driver layer
    esp_err_t err2 = esp_wifi_set_mac(WIFI_IF_AP, macBytes);

    if (err1 == ESP_OK && err2 == ESP_OK) {
        Serial.printf("[MAC] WiFi AP MAC set to: %02X:%02X:%02X:%02X:%02X:%02X\n",
            macBytes[0], macBytes[1], macBytes[2], macBytes[3], macBytes[4], macBytes[5]);
        return true;
    }
    else {
        Serial.printf("[MAC] Failed to set WiFi AP MAC (netif=%d, wifi=%d)\n", (int)err1, (int)err2);
        return false;
    }
}


SMNetworkManager& SMNetworkManager::getInstance() {
    static SMNetworkManager instance;
    return instance;
}

SMNetworkManager::SMNetworkManager()
    : _initialized(false)
    , _autoReconnect(true)
    , _isSTAMode(false)
    , _isAPMode(false)
    , _connectInProgress(false)
    , _connectStartMs(0)
    , _lastReconnectAttempt(0)
    , _reconnectDelay(MIN_RECONNECT_DELAY)
    , _reconnectAttempts(0)
    , _dnsServerRunning(false) {
}

SMNetworkManager::~SMNetworkManager() {
    stop();
}

bool SMNetworkManager::init(const WiFiConfig& config) {
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
        esp_wifi_get_mac(WIFI_IF_STA, mac);
        hostname = "GE3222M-" + String(mac[4], HEX) + String(mac[5], HEX);
    }
    WiFi.setHostname(hostname.c_str());

    _initialized = true;
    logger.info("NetworkManager: Initialized, hostname: %s", hostname.c_str());

    return true;
}

bool SMNetworkManager::startSTA() {
    Logger& logger = Logger::getInstance();

    if (!_initialized) {
        logger.error("NetworkManager: Not initialized");
        return false;
    }

    if (strlen(_config.ssid) == 0) {
        logger.error("NetworkManager: No SSID configured");
        return false;
    }

    // Stop any previous WiFi state first
    stop();

    // STA-only mode (per Phase 4 plan):
    //  - startSTA() is STA-only (no management AP)
    //  - no WiFi.mode switching after start
    // Avoid aggressive power-save that can cause reconnect loops on some routers.
    WiFi.persistent(false);
    delay(20);

    // STA only
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    esp_wifi_set_ps(WIFI_PS_NONE);
    // Max TX power helps stability on weak links.
    WiFi.setTxPower(WIFI_POWER_19_5dBm);

    // Ensure AP/DNS are fully stopped in STA-only operation
    _isAPMode = false;
    if (_dnsServerRunning) {
        _dnsServer.stop();
        _dnsServerRunning = false;
    }

    WiFi.setAutoReconnect(_autoReconnect);
    if (!_config.useDHCP) {
        if (!applyStaticConfig()) {
            logger.error("NetworkManager: Failed to apply static IP config");
            return false;
        }
    }

    logger.info("NetworkManager: Connecting to SSID: %s", _config.ssid);

    WiFi.begin(_config.ssid, _config.password);

    _connectInProgress = true;
    _connectStartMs = millis();

    _isSTAMode = true;
    _reconnectAttempts = 0;

    return true;
}

bool SMNetworkManager::startAP(const char* ssid, const char* password) {
    Logger& logger = Logger::getInstance();

    if (!_initialized) {
        logger.error("NetworkManager: Not initialized");
        return false;
    }

    // Stop any active interface before starting AP-only.
    stop();

    // ===== SoftAP start logic (FIXED) =====
    // Key fixes for "can't connect to this network":
    //  1. Set custom MAC BEFORE WiFi.mode(WIFI_AP) via netif layer only
    //  2. Explicitly configure softAP IP/gateway/subnet before softAP()
    //  3. Call WiFi.softAP() with explicit channel (1) and auth mode (WPA2_PSK)
    //  4. Set WiFi driver MAC AFTER softAP is fully started
    WiFi.persistent(false);

    // Ensure we are not connected as STA
    WiFi.disconnect(true);
    delay(100);

    // Determine AP settings (prefer config, then params, then defaults)
    const char* apSSID = (ssid && ssid[0]) ? ssid : (_config.apSsid[0] ? _config.apSsid : kSoftApSsid);
    const char* apPASS = (password && password[0]) ? password : (_config.apPassword[0] ? _config.apPassword : kSoftApPass);
    const char* apMAC = (_config.apMac[0] ? _config.apMac : kSoftApMac);

    // Enforce WPA2 minimum length if password is provided
    String apPassStr = String(apPASS ? apPASS : "");
    if (!apPassStr.isEmpty() && apPassStr.length() < 8) {
        logger.warn("NetworkManager: AP password too short, using default");
        apPassStr = kSoftApPass;
    }

    // Step 1: Set custom MAC at netif layer BEFORE entering AP mode
    // This ensures the MAC is applied when the AP interface initializes
    {
        uint8_t macBytes[6];
        if (parseMac(apMAC, macBytes)) {
            esp_err_t err = esp_iface_mac_addr_set(macBytes, ESP_MAC_WIFI_SOFTAP);
            if (err == ESP_OK) {
                logger.info("NetworkManager: Pre-set AP MAC via netif: %s", apMAC);
            }
            else {
                logger.warn("NetworkManager: Pre-set AP MAC failed (err=%d), will use default", (int)err);
            }
        }
    }

    // Step 2: Enter AP mode
    WiFi.mode(WIFI_AP);
    delay(100);  // Allow mode switch to stabilize

    // Disable WiFi power-save for AP stability
    WiFi.setSleep(false);
    esp_wifi_set_ps(WIFI_PS_NONE);

    // Step 3: Explicitly configure AP IP addressing BEFORE softAP()
    // This ensures the DHCP server is properly initialized
    IPAddress apIP(192, 168, 4, 1);
    IPAddress apGateway(192, 168, 4, 1);
    IPAddress apSubnet(255, 255, 255, 0);
    if (!WiFi.softAPConfig(apIP, apGateway, apSubnet)) {
        logger.error("NetworkManager: Failed to configure AP IP/subnet");
        // Continue anyway - softAP may still work with defaults
    }
    delay(50);

    // Step 4: Start SoftAP with EXPLICIT channel and auth mode parameters
    // Using channel 1 (widely compatible) and explicit max_connection
    // The 2-parameter form can cause auth mode issues on ESP32 Arduino 3.x
    bool result;
    if (apPassStr.isEmpty()) {
        // Open network: channel 1, max 4 connections, not hidden
        result = WiFi.softAP(apSSID, NULL, 1, 0, 4);
    }
    else {
        // WPA2 protected: channel 1, not hidden, max 4 connections
        result = WiFi.softAP(apSSID, apPassStr.c_str(), 1, 0, 4);
    }

    if (result) {
        // Step 5: After softAP is running, set MAC at driver level too
        delay(100);  // Let AP fully start
        {
            uint8_t macBytes[6];
            if (parseMac(apMAC, macBytes)) {
                esp_err_t err = esp_wifi_set_mac(WIFI_IF_AP, macBytes);
                if (err == ESP_OK) {
                    logger.info("NetworkManager: AP driver MAC set: %s", apMAC);
                }
                else {
                    logger.warn("NetworkManager: AP driver MAC set failed (err=%d), using auto MAC", (int)err);
                }
            }
        }

        // Step 6: Start DNS server for captive portal
        _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        _dnsServer.start(53, "*", WiFi.softAPIP());
        _dnsServerRunning = true;

        _isAPMode = true;
        _isSTAMode = false;
        _connectInProgress = false;
        _connectStartMs = 0;

        // Log actual AP config for debugging
        wifi_config_t cfg;
        if (esp_wifi_get_config(WIFI_IF_AP, &cfg) == ESP_OK) {
            logger.info("NetworkManager: AP cfg (channel=%u, authmode=%u, max_conn=%u, ssid_len=%u)",
                (unsigned)cfg.ap.channel, (unsigned)cfg.ap.authmode,
                (unsigned)cfg.ap.max_connection, (unsigned)cfg.ap.ssid_len);
        }

        logger.info("NetworkManager: AP started - SSID: %s, IP: %s",
            apSSID, WiFi.softAPIP().toString().c_str());
        logger.info("NetworkManager: AP MAC: %s", WiFi.softAPmacAddress().c_str());
    }
    else {
        logger.error("NetworkManager: Failed to start AP (SSID=%s)", apSSID);

        // Retry once without custom MAC (fallback)
        logger.warn("NetworkManager: Retrying AP start without custom MAC...");
        WiFi.mode(WIFI_OFF);
        delay(200);
        WiFi.mode(WIFI_AP);
        delay(100);
        WiFi.softAPConfig(apIP, apGateway, apSubnet);
        delay(50);

        if (apPassStr.isEmpty()) {
            result = WiFi.softAP(apSSID, NULL, 1, 0, 4);
        }
        else {
            result = WiFi.softAP(apSSID, apPassStr.c_str(), 1, 0, 4);
        }

        if (result) {
            _dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
            _dnsServer.start(53, "*", WiFi.softAPIP());
            _dnsServerRunning = true;
            _isAPMode = true;
            _isSTAMode = false;
            _connectInProgress = false;
            _connectStartMs = 0;

            logger.info("NetworkManager: AP started (fallback, no custom MAC) - SSID: %s, IP: %s",
                apSSID, WiFi.softAPIP().toString().c_str());
        }
        else {
            logger.error("NetworkManager: AP start failed even on retry");
        }
    }

    return result;
}


void SMNetworkManager::stop() {
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
    delay(100);  // Allow WiFi stack to fully shut down

    _isSTAMode = false;
    _isAPMode = false;
    _connectInProgress = false;
    _connectStartMs = 0;
}

bool SMNetworkManager::isConnected() const {
    if (_isSTAMode) return WiFi.isConnected();
    if (_isAPMode) return true; // AP is active
    return false;
}
bool SMNetworkManager::isSTAMode() const { return _isSTAMode; }
bool SMNetworkManager::isAPMode() const { return _isAPMode; }


String SMNetworkManager::getLocalIP() const {
    if (_isSTAMode) {
        return WiFi.localIP().toString();
    }
    else if (_isAPMode) {
        return WiFi.softAPIP().toString();
    }
    return "0.0.0.0";
}

String SMNetworkManager::getAPIP() const {
    if (_isAPMode) {
        return WiFi.softAPIP().toString();
    }
    return "0.0.0.0";
}

int SMNetworkManager::getRSSI() const {
    if (isConnected()) {
        return WiFi.RSSI();
    }
    return -100;
}

String SMNetworkManager::getMACAddress() const {
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

String SMNetworkManager::getSSID() const {
    if (_isSTAMode) {
        return WiFi.SSID();
    }
    else if (_isAPMode) {
        return WiFi.softAPSSID();
    }
    return "";
}

void SMNetworkManager::handle() {
    if (_dnsServerRunning) {
        _dnsServer.processNextRequest();
    }

    if (_autoReconnect && _isSTAMode && !isConnected()) {
        attemptReconnect();
    }
}

void SMNetworkManager::setAutoReconnect(bool enable) {
    _autoReconnect = enable;
    if (_isSTAMode) {
        WiFi.setAutoReconnect(enable);
    }
}

bool SMNetworkManager::applyStaticConfig() {
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

void SMNetworkManager::attemptReconnect() {
    unsigned long now = millis();

    // If a connection attempt is already in progress, do not call WiFi.reconnect()/begin()
    // again (ESP-IDF will log: "sta is connecting, return error").
    if (_connectInProgress) {
        // Give the STA state machine time to finish association/DHCP.
        if (now - _connectStartMs < 15000) {
            return;
        }
        // Timed out -> allow a fresh attempt.
        _connectInProgress = false;
        _connectStartMs = 0;
    }

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

    _reconnectDelay = std::min<uint16_t>(static_cast<uint16_t>(_reconnectDelay * 2), MAX_RECONNECT_DELAY);

    Logger::getInstance().info("NetworkManager: Reconnect attempt %d (delay: %dms)",
        _reconnectAttempts, _reconnectDelay);

    wl_status_t st = WiFi.status();
    if (st == WL_CONNECTED) {
        return;
    }

    // Prefer reconnect() first. Some routers/firmware combos will emit
    // "wifi: sta is connecting, return error" if we call WiFi.begin() too aggressively.
    // We only force a full begin() occasionally as a fallback.
    if ((_reconnectAttempts % 3) == 0) {
        WiFi.disconnect(true);
        delay(100);
        WiFi.begin(_config.ssid, _config.password);
    }
    else {
        WiFi.reconnect();
    }
    _connectInProgress = true;
    _connectStartMs = now;
}

String SMNetworkManager::getRSSIQuality(int rssi) const {
    if (rssi >= -30) return "Excellent";
    if (rssi >= -50) return "Very Good";
    if (rssi >= -60) return "Good";
    if (rssi >= -70) return "Fair";
    if (rssi >= -80) return "Weak";
    return "Very Weak";
}

void SMNetworkManager::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    Logger& logger = Logger::getInstance();
    SMNetworkManager& nm = SMNetworkManager::getInstance();

    switch (event) {
    case ARDUINO_EVENT_WIFI_READY:
        logger.debug("NetworkManager: WiFi ready");
        break;

    case ARDUINO_EVENT_WIFI_STA_START:
        logger.debug("NetworkManager: STA started");
        nm._connectInProgress = true;
        nm._connectStartMs = millis();
        break;

    case ARDUINO_EVENT_WIFI_STA_STOP:
        logger.debug("NetworkManager: STA stopped");
        break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        logger.info("NetworkManager: Connected to SSID: %s", WiFi.SSID().c_str());

        // We are associated, but DHCP may still be running.
        // Hold off any "reconnect attempt" spam until we either get an IP or timeout.
        nm._connectInProgress = true;
        nm._connectStartMs = millis();

        nm._reconnectAttempts = 0;
        nm._reconnectDelay = MIN_RECONNECT_DELAY;
        break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        // NOTE: Reason codes are ESP-IDF specific.
        logger.warn("NetworkManager: Disconnected from WiFi (reason=%d)", (int)info.wifi_sta_disconnected.reason);

        // IMPORTANT:
        // During flapping links, ESP-IDF can emit DISCONNECTED while the STA state machine
        // is still mid-transition. Immediately calling WiFi.begin() again can produce:
        //   "sta is connecting, return error"
        // We keep a short "connect in progress" grace window so attemptReconnect() backs off.
        nm._connectInProgress = true;
        nm._connectStartMs = millis();
        break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        logger.info("NetworkManager: Got IP: %s", WiFi.localIP().toString().c_str());
        logger.info("NetworkManager: RSSI: %d dBm (%s)",
            WiFi.RSSI(), nm.getRSSIQuality(WiFi.RSSI()).c_str());
        nm._connectInProgress = false;
        nm._connectStartMs = 0;
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