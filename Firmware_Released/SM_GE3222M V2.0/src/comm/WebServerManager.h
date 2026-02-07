#pragma once

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <map>

/**
 * @brief WebServerManager - Singleton class for managing web server and REST API
 * 
 * Features:
 * - ESPAsyncWebServer on port 80
 * - WebSocket endpoint for real-time data push
 * - RESTful API endpoints
 * - Static file serving from SPIFFS
 * - GZIP support
 * - CORS headers
 * - Rate limiting (100 req/min per IP)
 */
class WebServerManager {
public:
    static WebServerManager& getInstance();
    
    // Initialize and start server
    bool init();
    void begin();
    void stop();
    
    // WebSocket management
    void setupWebSocket();
    void pushData(const String& jsonData);
    void pushData(const JsonDocument& doc);
    
    // Static file serving
    void serveStatic(const char* uri, const char* path);
    
    // Custom route handlers
    void registerHandler(const char* uri, WebRequestMethod method, 
                        void (*handler)(AsyncWebServerRequest*));
    
    // Main loop update
    void update();
    
private:
    WebServerManager();
    ~WebServerManager() = default;
    WebServerManager(const WebServerManager&) = delete;
    WebServerManager& operator=(const WebServerManager&) = delete;
    
    // Setup REST API endpoints
    void setupAPIEndpoints();
    
    // API handlers
    static void handleGetReadings(AsyncWebServerRequest* request);
    static void handleGetEnergy(AsyncWebServerRequest* request);
    static void handleGetConfig(AsyncWebServerRequest* request);
    static void handlePutConfig(AsyncWebServerRequest* request);
    static void handleGetStatus(AsyncWebServerRequest* request);
    static void handlePostCalibrate(AsyncWebServerRequest* request);
    static void handleGetHistory(AsyncWebServerRequest* request);
    static void handleResetEnergy(AsyncWebServerRequest* request);
    static void handleGetNetwork(AsyncWebServerRequest* request);
    
    // WebSocket handlers
    static void onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                                AwsEventType type, void* arg, uint8_t* data, size_t len);
    static void handleWebSocketMessage(void* arg, uint8_t* data, size_t len);
    
    // Rate limiting
    bool checkRateLimit(IPAddress clientIP);
    void cleanupRateLimits();
    
    AsyncWebServer server_;
    AsyncWebSocket ws_;
    
    bool initialized_;
    bool running_;
    
    // Rate limiting
    struct RateLimitInfo {
        unsigned long lastReset;
        uint16_t requestCount;
    };
    std::map<uint32_t, RateLimitInfo> rateLimits_;
    unsigned long lastRateLimitCleanup_;
    
    static constexpr uint16_t MAX_REQUESTS_PER_MINUTE = 100;
    static constexpr unsigned long RATE_LIMIT_WINDOW = 60000; // 1 minute
};
