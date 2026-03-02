#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <Preferences.h>
#include "esp_wifi.h"
#include "esp_mac.h"

// ─── Soft-AP Configuration ───────────────────────────────────────────────────
#define AP_SSID             "SM_GE3222M_Setup"
#define AP_PASS             "12345678"
#define AP_IP_ADDR          "192.168.4.1"
#define AP_GATEWAY          "192.168.4.1"
#define AP_SUBNET           "255.255.255.0"

// AP stability options (ESP32 core 3.x):
// Using low-level AP/STA MAC overrides can break DHCP/ARP on some builds.
// Keep disabled by default for reliable AP web serving.
#ifndef AP_USE_CUSTOM_MAC
#define AP_USE_CUSTOM_MAC    0
#endif

// ─── Hardware MAC Addresses ──────────────────────────────────────────────────
#define WIFI_STA_MAC        "C8:2E:A3:F5:7D:DB"
#define WIFI_AP_MAC         "C8:2E:A3:F5:7D:DC"

// ─── DNS Captive Portal ──────────────────────────────────────────────────────
#define DNS_PORT            53

// ─── Station Mode Defaults (used if nothing saved in NVS) ────────────────────
#define DEF_STA_SSID        ""
#define DEF_STA_PASS        ""
#define DEF_STA_USE_DHCP    true

#define DEF_STA_IP          "192.168.86.100"
#define DEF_STA_GW          "192.168.86.1"
#define DEF_STA_SN          "255.255.255.0"
#define DEF_STA_DNS1        "8.8.8.8"
#define DEF_STA_DNS2        "8.8.4.4"

// ─── Connection Timeout ──────────────────────────────────────────────────────
#define WIFI_CONNECT_TIMEOUT_MS   10000UL

// NVS namespace for WiFi config
#define NVS_NAMESPACE_WIFI  "wifi_cfg"

// ─────────────────────────────────────────────────────────────────────────────
//  WiFi configuration structure
// ─────────────────────────────────────────────────────────────────────────────
struct WiFiConfig {
    String ssid;
    String pass;
    bool   useDHCP;
    String ip;
    String gw;
    String sn;
    String dns1;
    String dns2;
};

// ─────────────────────────────────────────────────────────────────────────────
//  SMNetworkManager (avoids clash with core NetworkManager) [web:22]
// ─────────────────────────────────────────────────────────────────────────────
class SMNetworkManager {
public:
    SMNetworkManager();

    // Core lifecycle
    void begin();           // Init: load config from NVS, try STA → fallback AP
    void loop();            // Must be called in main loop: DNS + STA watchdog

    // Mode control
    void startAPMode();
    bool startSTAMode();
    bool reconnectSTA();

    // Status
    bool   isAPMode()      const;
    bool   isSTAMode()     const;
    bool   isConnected()   const;
    String getMacAddress() const;
    String getIPAddress()  const;
    String getSSID()       const;

    // Config access
    WiFiConfig getConfig() const;
    void       applyConfig(const WiFiConfig& cfg, bool saveToFlash);

    // NVS
    void loadConfigFromNVS();
    void saveConfigToNVS();

private:
    DNSServer   _dnsServer;
    Preferences _prefs;

    WiFiConfig  _cfg;

    bool        _apMode;
    bool        _staMode;
    bool        _wifiConnected;

    String      _currentMac;

    uint32_t    _lastApDiagMs;
    int         _lastApStaCount;

    static bool parseMac(const char* macStr, uint8_t* macBytes);
    void applyCustomAPMac();
    void applyCustomSTAMac();
    void ensureDNSDefaults();
};

// Global instance
extern SMNetworkManager networkManager;
