#pragma once

#include <Arduino.h>
#include <WebServer.h>

class WebUIManager {
public:
    static WebUIManager& getInstance();
    bool begin(uint16_t port = 80);
    void loop();
    bool isRunning() const { return _running; }

private:
    WebUIManager();
    void setupRoutes();
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
    String buildWiFiSetupPage();
    void sendJson(int code, const String& json);

    WebServer _server;
    bool _running;
    uint16_t _port;
};
