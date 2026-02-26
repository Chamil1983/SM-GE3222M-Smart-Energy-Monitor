#include "SMNetworkManager.h"
#include "Logger.h"

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────────────────────
SMNetworkManager::SMNetworkManager()
    : _apMode(false),
      _staMode(false),
      _wifiConnected(false),
      _currentMac("") {

    _cfg.ssid    = DEF_STA_SSID;
    _cfg.pass    = DEF_STA_PASS;
    _cfg.useDHCP = DEF_STA_USE_DHCP;
    _cfg.ip      = DEF_STA_IP;
    _cfg.gw      = DEF_STA_GW;
    _cfg.sn      = DEF_STA_SN;
    _cfg.dns1    = DEF_STA_DNS1;
    _cfg.dns2    = DEF_STA_DNS2;
}   // <-- constructor closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  begin()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::begin() {
    Serial.println(F("[NetMgr] Initialising..."));
    Logger::getInstance().info("NetworkManager: Initialising");
    _prefs.begin(NVS_NAMESPACE_WIFI, false);
    loadConfigFromNVS();
    if (!startSTAMode()) {
        startAPMode();
    }
}   // <-- begin() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  loop()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::loop() {
    if (_apMode) {
        _dnsServer.processNextRequest();
    }
}   // <-- loop() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  parseMac()
// ─────────────────────────────────────────────────────────────────────────────
bool SMNetworkManager::parseMac(const char* macStr, uint8_t* macBytes) {
    if (!macStr || strlen(macStr) != 17) return false;
    int v[6];
    if (sscanf(macStr, "%x:%x:%x:%x:%x:%x",
               &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]) != 6) return false;
    for (int i = 0; i < 6; i++) macBytes[i] = (uint8_t)v[i];
    return true;
}   // <-- parseMac() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  applyCustomAPMac()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::applyCustomAPMac() {
    uint8_t macBytes[6];
    const char* src = WIFI_AP_MAC;
    if (!parseMac(src, macBytes)) {
        Serial.println(F("[MAC] Invalid AP MAC, using default."));
        return;
    }
    esp_err_t e1 = esp_iface_mac_addr_set(macBytes, ESP_MAC_WIFI_SOFTAP);
    esp_err_t e2 = esp_wifi_set_mac(WIFI_IF_AP, macBytes);
    if (e1 == ESP_OK && e2 == ESP_OK) {
        Serial.printf("[MAC] AP MAC → %02X:%02X:%02X:%02X:%02X:%02X\n",
                      macBytes[0], macBytes[1], macBytes[2],
                      macBytes[3], macBytes[4], macBytes[5]);
    } else {
        Serial.printf("[MAC] AP MAC failed (netif=%d, wifi=%d)\n",
                      (int)e1, (int)e2);
    }
}   // <-- applyCustomAPMac() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  applyCustomSTAMac()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::applyCustomSTAMac() {
    uint8_t macBytes[6];
    const char* src = WIFI_STA_MAC;
    if (!parseMac(src, macBytes)) {
        Serial.println(F("[MAC] Invalid STA MAC, using default."));
        return;
    }
    esp_err_t e1 = esp_iface_mac_addr_set(macBytes, ESP_MAC_WIFI_STA);
    esp_err_t e2 = esp_wifi_set_mac(WIFI_IF_STA, macBytes);
    if (e1 == ESP_OK && e2 == ESP_OK) {
        Serial.printf("[MAC] STA MAC → %02X:%02X:%02X:%02X:%02X:%02X\n",
                      macBytes[0], macBytes[1], macBytes[2],
                      macBytes[3], macBytes[4], macBytes[5]);
    } else {
        Serial.printf("[MAC] STA MAC failed (netif=%d, wifi=%d)\n",
                      (int)e1, (int)e2);
    }
}   // <-- applyCustomSTAMac() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  ensureDNSDefaults()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::ensureDNSDefaults() {
    if (_cfg.dns1.isEmpty()) _cfg.dns1 = DEF_STA_DNS1;
    if (_cfg.dns2.isEmpty()) _cfg.dns2 = DEF_STA_DNS2;
}   // <-- ensureDNSDefaults() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  startAPMode()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::startAPMode() {
    Serial.println(F("[NetMgr] Starting Soft-AP..."));
    Logger::getInstance().warn("NetworkManager: Starting SoftAP fallback");

    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_AP);
    delay(50);

    applyCustomAPMac();

    IPAddress apIP, apGW, apSN;
    apIP.fromString(AP_IP_ADDR);
    apGW.fromString(AP_GATEWAY);
    apSN.fromString(AP_SUBNET);
    WiFi.softAPConfig(apIP, apGW, apSN);
    WiFi.softAP(AP_SSID, AP_PASS);

    _apMode        = true;
    _staMode       = false;
    _wifiConnected = true;
    _currentMac    = WiFi.softAPmacAddress();

    Serial.println(F("[NetMgr] Failed to connect as client. Starting AP Mode."));
    Serial.print  (F("[NetMgr] AP SSID : ")); Serial.println(F(AP_SSID));
    Serial.print  (F("[NetMgr] AP IP   : ")); Serial.println(WiFi.softAPIP());
    Serial.print  (F("[NetMgr] AP MAC  : ")); Serial.println(_currentMac);

    _dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    Serial.println(F("[NetMgr] Captive portal DNS active (port 53)."));
}   // <-- startAPMode() closing brace  ← THIS WAS MISSING

// ─────────────────────────────────────────────────────────────────────────────
//  startSTAMode()
// ─────────────────────────────────────────────────────────────────────────────
bool SMNetworkManager::startSTAMode() {
    if (_cfg.ssid.isEmpty()) {
        Serial.println(F("[NetMgr] No STA SSID configured, skipping STA mode."));
        return false;
    }

    Serial.println(F("[NetMgr] Starting Station mode..."));
    Logger::getInstance().info("NetworkManager: Starting STA mode");

    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_STA);
    delay(50);

    applyCustomSTAMac();

    if (_cfg.useDHCP) {
        Serial.println(F("[NetMgr] DHCP mode — IP assigned automatically."));
    } else {
        ensureDNSDefaults();
        IPAddress ip, gw, sn, d1, d2;
        ip.fromString (_cfg.ip);
        gw.fromString (_cfg.gw);
        sn.fromString (_cfg.sn);
        d1.fromString (_cfg.dns1);
        d2.fromString (_cfg.dns2);
        if (!WiFi.config(ip, gw, sn, d1, d2)) {
            Serial.println(F("[NetMgr] Static IP configuration failed!"));
            return false;
        }
        Serial.println(F("[NetMgr] Static IP mode:"));
        Serial.print  (F("  IP      : ")); Serial.println(_cfg.ip);
        Serial.print  (F("  Gateway : ")); Serial.println(_cfg.gw);
        Serial.print  (F("  Subnet  : ")); Serial.println(_cfg.sn);
        Serial.print  (F("  DNS1    : ")); Serial.println(_cfg.dns1);
        Serial.print  (F("  DNS2    : ")); Serial.println(_cfg.dns2);
    }

    WiFi.begin(_cfg.ssid.c_str(), _cfg.pass.c_str());
    Serial.print(F("[NetMgr] Connecting to: "));
    Serial.println(_cfg.ssid);

    uint32_t t0 = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - t0 >= WIFI_CONNECT_TIMEOUT_MS) {
            Serial.println(F("\n[NetMgr] Connection timed out."));
            WiFi.disconnect(true);
            return false;
        }
        delay(500);
        Serial.print('.');
    }
    Serial.println();

    _staMode       = true;
    _apMode        = false;
    _wifiConnected = true;
    _currentMac    = WiFi.macAddress();

    Serial.println(F("[NetMgr] Station connected!"));
    Serial.print  (F("[NetMgr] IP      : ")); Serial.println(WiFi.localIP());
    Serial.print  (F("[NetMgr] Gateway : ")); Serial.println(WiFi.gatewayIP());
    Serial.print  (F("[NetMgr] MAC     : ")); Serial.println(_currentMac);
    return true;
}   // <-- startSTAMode() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  reconnectSTA()
// ─────────────────────────────────────────────────────────────────────────────
bool SMNetworkManager::reconnectSTA() {
    Serial.println(F("[NetMgr] Attempting reconnect..."));
    if (!startSTAMode()) {
        startAPMode();
        return false;
    }
    return true;
}   // <-- reconnectSTA() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  loadConfigFromNVS()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::loadConfigFromNVS() {
    Serial.println(F("[NetMgr] Loading WiFi config from NVS..."));

    _cfg.ssid    = _prefs.getString("ssid", DEF_STA_SSID);
    _cfg.pass    = _prefs.getString("pass", DEF_STA_PASS);
    _cfg.useDHCP = _prefs.getBool  ("dhcp", DEF_STA_USE_DHCP);
    _cfg.ip      = _prefs.getString("ip",   DEF_STA_IP);
    _cfg.gw      = _prefs.getString("gw",   DEF_STA_GW);
    _cfg.sn      = _prefs.getString("sn",   DEF_STA_SN);
    _cfg.dns1    = _prefs.getString("d1",   DEF_STA_DNS1);
    _cfg.dns2    = _prefs.getString("d2",   DEF_STA_DNS2);

    Serial.print(F("[NetMgr] SSID    : ")); Serial.println(_cfg.ssid);
    Serial.print(F("[NetMgr] useDHCP : ")); Serial.println(_cfg.useDHCP ? "true" : "false");
}   // <-- loadConfigFromNVS() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  saveConfigToNVS()
// ─────────────────────────────────────────────────────────────────────────────
void SMNetworkManager::saveConfigToNVS() {
    Serial.println(F("[NetMgr] Saving WiFi config to NVS..."));

    _prefs.putString("ssid", _cfg.ssid);
    _prefs.putString("pass", _cfg.pass);
    _prefs.putBool  ("dhcp", _cfg.useDHCP);
    _prefs.putString("ip",   _cfg.ip);
    _prefs.putString("gw",   _cfg.gw);
    _prefs.putString("sn",   _cfg.sn);
    _prefs.putString("d1",   _cfg.dns1);
    _prefs.putString("d2",   _cfg.dns2);
}   // <-- saveConfigToNVS() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  getConfig() / applyConfig()
// ─────────────────────────────────────────────────────────────────────────────
WiFiConfig SMNetworkManager::getConfig() const {
    return _cfg;
}   // <-- getConfig() closing brace

void SMNetworkManager::applyConfig(const WiFiConfig& cfg, bool saveToFlash) {
    _cfg = cfg;
    if (saveToFlash) {
        saveConfigToNVS();
    }
}   // <-- applyConfig() closing brace

// ─────────────────────────────────────────────────────────────────────────────
//  Getters
// ─────────────────────────────────────────────────────────────────────────────
bool   SMNetworkManager::isAPMode()      const { return _apMode;        }
bool   SMNetworkManager::isSTAMode()     const { return _staMode;       }
bool   SMNetworkManager::isConnected()   const { return _wifiConnected; }
String SMNetworkManager::getMacAddress() const { return _currentMac;    }

String SMNetworkManager::getIPAddress() const {
    return _apMode ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
}   // <-- getIPAddress() closing brace

String SMNetworkManager::getSSID() const {
    if (_apMode) return String(F(AP_SSID));
    return WiFi.SSID();
}   // <-- getSSID() closing brace

// Global singleton
SMNetworkManager networkManager;
