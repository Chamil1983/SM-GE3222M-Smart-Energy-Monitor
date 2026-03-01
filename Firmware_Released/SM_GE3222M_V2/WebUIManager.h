#pragma once

#include <Arduino.h>

#ifndef __has_include
#define __has_include(x) 0
#endif

#if defined(ARDUINO_ARCH_ESP32) && __has_include(<ESPAsyncWebServer.h>) && __has_include(<AsyncTCP.h>)
  #define WEBUI_ASYNC_ENABLED 1
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#else
  #define WEBUI_ASYNC_ENABLED 0
  #include <WebServer.h>
#endif

class WebUIManager {
public:
    static WebUIManager& getInstance();
    bool begin(uint16_t port = 80);
    void loop();
    bool isRunning() const { return _running; }
    bool isAsyncEnabled() const { return WEBUI_ASYNC_ENABLED != 0; }

private:
    WebUIManager();

    void setupRoutes();
    void scheduleSTAReconnect(uint32_t delayMs = 300);

    String buildMeterJson(bool refreshMeter);
    String buildStatusJson();
    String buildConfigJson();
    bool applyConfigJson(const String& body);

    String buildWiFiSetupPage();

#if WEBUI_ASYNC_ENABLED
    void setupWebSocket();
    void handleWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                              AwsEventType type, void* arg, uint8_t* data, size_t len);
    void handleAsyncStaticFile(AsyncWebServerRequest* request, const String& path, const String& contentType);
    void handleAsyncSaveForm(AsyncWebServerRequest* request);
    void handleAsyncConfigPostBody(AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total);
    void sendAsyncJson(AsyncWebServerRequest* request, int code, const String& json);
    void sendAsyncCaptiveRedirect(AsyncWebServerRequest* request);

    AsyncWebServer _server;
    AsyncWebSocket _ws;
    uint32_t _lastWsBroadcastMs;
    String _configPostBodyBuf;
#else
    void handleRoot();
    void handleIndex();
    void handleStaticFile(const String& path, const String& contentType);
    void handleApiMeter();
    void handleApiStatus();
    void handleApiConfigGet();
    void handleApiConfigPost();
    void handleSaveForm();
    void handleCaptiveRedirect();
    void handleNotFound();
    void sendJson(int code, const String& json);

    WebServer _server;
#endif

    bool _running;
    uint16_t _port;
    uint32_t _deferredStaReconnectAtMs;
};
