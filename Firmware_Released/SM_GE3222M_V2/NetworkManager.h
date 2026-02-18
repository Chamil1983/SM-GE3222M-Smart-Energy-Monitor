/**
 * @file NetworkManager.h
 * @brief WiFi Network Manager with auto-reconnect and static IP support
 * @details Singleton class for managing WiFi connections in both STA and AP modes
 */

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include "DataTypes.h"

 // NOTE: ESP32 Arduino core also defines a global `NetworkManager` class.
 // To avoid a symbol collision (and a hard compile error), our project-specific
 // WiFi manager is named `SMNetworkManager`.
class SMNetworkManager {
public:
    static SMNetworkManager& getInstance();

    bool init(const WiFiConfig& config);
    bool startSTA();
    bool startAP(const char* ssid = nullptr, const char* password = nullptr);
    void stop();

    bool isConnected() const;
    // Mode helpers (used by WebServerManager and UI status)
    bool isSTAMode() const;
    bool isAPMode() const;
    String getLocalIP() const;
    String getAPIP() const;
    int getRSSI() const;
    String getMACAddress() const;
    String getSSID() const;

    void handle();
    void setAutoReconnect(bool enable);

private:
    SMNetworkManager();
    ~SMNetworkManager();
    SMNetworkManager(const SMNetworkManager&) = delete;
    SMNetworkManager& operator=(const SMNetworkManager&) = delete;

    static void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

    bool applyStaticConfig();
    void attemptReconnect();
    String getRSSIQuality(int rssi) const;

    // Internal AP start helper — attempts softAP with optional custom MAC
    bool startAPInternal(const char* apSSID, const char* apPass, const char* apMAC, bool useCustomMac);

    WiFiConfig _config;
    bool _initialized;
    bool _autoReconnect;
    bool _isSTAMode;
    bool _isAPMode;

    // Connection state (prevents repeated esp_wifi_connect() calls while already connecting)
    bool _connectInProgress;
    unsigned long _connectStartMs;

    unsigned long _lastReconnectAttempt;
    uint16_t _reconnectDelay;
    uint8_t _reconnectAttempts;

    static const uint16_t MIN_RECONNECT_DELAY = 1000;
    static const uint16_t MAX_RECONNECT_DELAY = 30000;
    static const uint8_t MAX_RECONNECT_ATTEMPTS = 10;

    DNSServer _dnsServer;
    bool _dnsServerRunning;
};

#endif // NETWORKMANAGER_H