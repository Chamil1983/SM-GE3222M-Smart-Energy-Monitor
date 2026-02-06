#pragma once

#include <WiFi.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include <esp_wifi.h>

// WiFi configuration structure
struct WiFiConfig {
    String ssid;
    String password;
    String hostname;
    bool useStaticIP;
    IPAddress staticIP;
    IPAddress gateway;
    IPAddress subnet;
    IPAddress dns1;
    IPAddress dns2;
};

/**
 * @brief WiFiManager - Singleton class for managing WiFi connectivity
 * 
 * Features:
 * - STA mode with auto-reconnect (exponential backoff)
 * - AP mode fallback with captive portal
 * - AP+STA concurrent mode for setup
 * - mDNS responder
 * - NTP time synchronization
 * - Signal strength monitoring
 */
class WiFiManager {
public:
    static WiFiManager& getInstance();
    
    // Initialize WiFi with configuration
    bool init(const WiFiConfig& config);
    
    // Connection management
    bool connect();
    void disconnect();
    bool startAP();
    void stopAP();
    
    // Status queries
    bool isConnected() const;
    bool isAPMode() const;
    String getSSID() const;
    int getRSSI() const;
    IPAddress getIP() const;
    String getMACAddress() const;
    String getHostname() const;
    
    // mDNS setup
    bool setupMDNS(const char* hostname = "smartmeter");
    
    // NTP time sync
    void configureNTP(const char* ntpServer = "pool.ntp.org", long gmtOffset = 0, int daylightOffset = 0);
    
    // Main loop update (call from loop)
    void update();
    
    // Get signal strength description
    String getRSSIDescription() const;
    
private:
    WiFiManager();
    ~WiFiManager() = default;
    WiFiManager(const WiFiManager&) = delete;
    WiFiManager& operator=(const WiFiManager&) = delete;
    
    // WiFi event handlers
    static void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
    void handleWiFiEvent(WiFiEvent_t event);
    
    // Auto-reconnect logic
    void attemptReconnect();
    uint32_t getNextReconnectDelay();
    
    // AP mode helpers
    String generateAPSSID();
    void setupCaptivePortal();
    
    WiFiConfig config_;
    DNSServer dnsServer_;
    
    // State tracking
    bool initialized_;
    bool apModeActive_;
    bool staConnected_;
    
    // Reconnection logic
    uint32_t reconnectAttempts_;
    unsigned long lastReconnectAttempt_;
    unsigned long reconnectDelay_;
    static constexpr uint32_t MAX_RECONNECT_DELAY = 300000; // 5 minutes cap
    static constexpr uint32_t BASE_RECONNECT_DELAY = 1000;  // 1 second base
    
    // mDNS
    bool mdnsStarted_;
    
    // Event handler ID
    wifi_event_id_t eventHandlerId_;
};
