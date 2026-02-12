#pragma once

// SM-GE3222M V2.0 - Web Server Manager
// Socket-based HTTP server (ESP32 Arduino WebServer).
//
// NOTE: This build avoids ESPAsyncWebServer/AsyncTCP to prevent LWIP core-lock
// asserts on some ESP32 Arduino 3.x toolchains.
//
// - Serves a minimal index page (or /data/index.html from SPIFFS if present)
// - Provides REST API:
//     GET  /api/status
//     GET  /api/meter
//     GET  /api/config
//     POST /api/config   (JSON body)
//     GET  /api/logs?count=50
//
// WebSocket streaming is disabled in this compatibility build.
// The web UI can poll /api/meter once per second.

#include <Arduino.h>
#include <WebServer.h>
#include "DataTypes.h"

class WebServerManager {
public:
    static WebServerManager& getInstance() {
        static WebServerManager instance;
        return instance;
    }

    bool begin(uint16_t port = 80);
    void stop();

    // Processing (call from loop/task)
    void handle();

    // Compatibility no-ops (WebSocket was removed)
    void broadcastMeterData() {}
    void broadcastSystemStatus() {}
    void broadcastEvent(const String& /*event*/, const String& /*data*/) {}

    bool isRunning() const { return _running; }
    uint16_t getPort() const { return _port; }
    size_t getClientCount() const { return 0; } // no websocket clients

private:
    WebServerManager();
    ~WebServerManager();

    WebServerManager(const WebServerManager&) = delete;
    WebServerManager& operator=(const WebServerManager&) = delete;

    void setupRoutes();

    // HTTP handlers
    void handleRoot();
    void handleGetStatus();
    void handleGetMeter();
    void handleGetConfig();
    void handlePostConfig();
    void handleGetLogs();
    void handleNotFound();

    // Helpers
    String getMeterDataJson();
    String getSystemStatusJson();
    String getConfigJson();
    String getLogsJson(int count = 50);

    void addCORSHeaders();
    bool streamFile(const char* path, const char* contentType);

    WebServer* _server;
    bool _running;
    uint16_t _port;
};
