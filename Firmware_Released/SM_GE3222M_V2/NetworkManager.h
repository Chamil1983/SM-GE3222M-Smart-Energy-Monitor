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

class NetworkManager {
public:
    static NetworkManager& getInstance();
    
    bool init(const WiFiConfig& config);
    bool startSTA();
    bool startAP(const char* ssid, const char* password);
    void stop();
    
    bool isConnected() const;
    String getLocalIP() const;
    int getRSSI() const;
    String getMACAddress() const;
    String getSSID() const;
    
    void handle();
    void setAutoReconnect(bool enable);
    
private:
    NetworkManager();
    ~NetworkManager();
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;
    
    static void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
    
    bool applyStaticConfig();
    void attemptReconnect();
    String getRSSIQuality(int rssi) const;
    
    WiFiConfig _config;
    bool _initialized;
    bool _autoReconnect;
    bool _isSTAMode;
    bool _isAPMode;
    
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
