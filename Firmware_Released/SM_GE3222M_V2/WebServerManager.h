#pragma once

// SM-GE3222M V2.0 - Web Server Manager
// Singleton web server with WebSocket support
// Serves static files from SPIFFS and provides REST API

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include "DataTypes.h"

class WebServerManager {
public:
    static WebServerManager& getInstance() {
        static WebServerManager instance;
        return instance;
    }
    
    // Initialization
    bool begin(uint16_t port = 80);
    void stop();
    
    // Processing (call from loop/task)
    void handle();
    
    // WebSocket broadcasting
    void broadcastMeterData();
    void broadcastSystemStatus();
    void broadcastEvent(const String& event, const String& data);
    
    // Status
    bool isRunning() const { return _running; }
    uint16_t getPort() const { return _port; }
    size_t getClientCount() const;
    
private:
    WebServerManager();
    ~WebServerManager();
    
    // Prevent copying
    WebServerManager(const WebServerManager&) = delete;
    WebServerManager& operator=(const WebServerManager&) = delete;
    
    // Server setup
    void setupRoutes();
    void setupWebSocket();
    
    // HTTP Route Handlers
    void handleRoot(AsyncWebServerRequest* request);
    void handleGetStatus(AsyncWebServerRequest* request);
    void handleGetMeter(AsyncWebServerRequest* request);
    void handleGetConfig(AsyncWebServerRequest* request);
    void handlePostConfig(AsyncWebServerRequest* request, uint8_t* data, size_t len);
    void handleGetLogs(AsyncWebServerRequest* request);
    void handleNotFound(AsyncWebServerRequest* request);
    
    // WebSocket Handlers
    static void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, 
                         AwsEventType type, void* arg, uint8_t* data, size_t len);
    void handleWsMessage(AsyncWebSocketClient* client, const String& message);
    
    // Helper functions
    String getMeterDataJson();
    String getSystemStatusJson();
    String getConfigJson();
    String getLogsJson(int count = 50);
    void addCORSHeaders(AsyncWebServerResponse* response);
    
    // Server instances
    AsyncWebServer* _server;
    AsyncWebSocket* _ws;
    
    // State
    bool _running;
    uint16_t _port;
    
    // Timing for auto-broadcast
    uint32_t _lastBroadcastTime;
    static const uint32_t BROADCAST_INTERVAL_MS = 1000;  // 1 second
    
    // WebSocket path
    static const char* WS_PATH;
};
