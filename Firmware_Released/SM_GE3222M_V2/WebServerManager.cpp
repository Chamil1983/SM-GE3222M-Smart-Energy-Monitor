#include "WebServerManager.h"
#include "Logger.h"
#include "EnergyMeter.h"
#include "SystemMonitor.h"
#include "ConfigManager.h"
#include "DataLogger.h"
#include "ProtocolV2.h"
#include "SPIFFSManager.h"
#include "NetworkManager.h"
#include <ArduinoJson.h>

WebServerManager::WebServerManager()
    : _server(nullptr), _running(false), _port(80) {
}

WebServerManager::~WebServerManager() {
    stop();
}

bool WebServerManager::begin(uint16_t port) {
    if (_running) {
        Logger::getInstance().warn("[WebServer] Server already running");
        return true;
    }

    _port = port;

    _server = new WebServer(_port);
    if (!_server) {
        Logger::getInstance().error("[WebServer] Failed to create WebServer");
        return false;
    }

    setupRoutes();
    _server->begin();
    _running = true;

    Logger::getInstance().info("[WebServer] Server started on port %u", (unsigned)_port);
    Logger::getInstance().info("[WebServer] WebSocket disabled (poll /api/meter)");
    return true;
}

void WebServerManager::stop() {
    if (!_running) return;

    if (_server) {
        _server->stop();
        delete _server;
        _server = nullptr;
    }

    _running = false;
    Logger::getInstance().info("[WebServer] Server stopped");
}

void WebServerManager::handle() {
    if (!_running || !_server) return;
    _server->handleClient();
}

void WebServerManager::setupRoutes() {
    // Root
    _server->on("/", HTTP_GET, [this]() { handleRoot(); });

    // API endpoints
    _server->on("/api/status", HTTP_GET, [this]() { handleGetStatus(); });
    _server->on("/api/meter", HTTP_GET, [this]() { handleGetMeter(); });
    _server->on("/api/config", HTTP_GET, [this]() { handleGetConfig(); });
    _server->on("/api/config", HTTP_POST, [this]() { handlePostConfig(); });
    _server->on("/api/logs", HTTP_GET, [this]() { handleGetLogs(); });

    // Preflight CORS
    _server->on("/api/status", HTTP_OPTIONS, [this]() { addCORSHeaders(); _server->send(204); });
    _server->on("/api/meter", HTTP_OPTIONS, [this]() { addCORSHeaders(); _server->send(204); });
    _server->on("/api/config", HTTP_OPTIONS, [this]() { addCORSHeaders(); _server->send(204); });
    _server->on("/api/logs", HTTP_OPTIONS, [this]() { addCORSHeaders(); _server->send(204); });

    // Static file passthrough for /data/*
    _server->onNotFound([this]() { handleNotFound(); });
}

bool WebServerManager::streamFile(const char* path, const char* contentType) {
    if (!SPIFFS.exists(path)) {
        return false;
    }
    File f = SPIFFS.open(path, "r");
    if (!f) return false;
    addCORSHeaders();
    _server->streamFile(f, contentType);
    f.close();
    return true;
}

void WebServerManager::handleRoot() {
    // Prefer SPIFFS hosted UI
    if (streamFile("/data/index.html", "text/html")) {
        return;
    }

    String html;
    html.reserve(1024);
    html += "<html><head><title>SM-GE3222M V2</title></head><body>";
    html += "<h1>SM-GE3222M Smart Energy Monitor V2.0</h1>";
    html += "<p>Web UI not installed. Upload web files to SPIFFS under <code>/data/</code>.</p>";
    html += "<p>API Endpoints:</p><ul>";
    html += "<li><a href='/api/status'>/api/status</a> - System Status</li>";
    html += "<li><a href='/api/meter'>/api/meter</a> - Meter Data</li>";
    html += "<li><a href='/api/config'>/api/config</a> - Configuration</li>";
    html += "<li><a href='/api/logs'>/api/logs</a> - Recent Logs</li>";
    html += "</ul>";
    html += "<p><b>Note:</b> WebSocket streaming is disabled in this build. Poll <code>/api/meter</code>.</p>";
    html += "</body></html>";

    addCORSHeaders();
    _server->send(200, "text/html", html);
}

void WebServerManager::handleGetStatus() {
    addCORSHeaders();
    _server->send(200, "application/json", getSystemStatusJson());
}

void WebServerManager::handleGetMeter() {
    addCORSHeaders();
    _server->send(200, "application/json", getMeterDataJson());
}

void WebServerManager::handleGetConfig() {
    addCORSHeaders();
    _server->send(200, "application/json", getConfigJson());
}

void WebServerManager::handlePostConfig() {
    addCORSHeaders();

    if (!_server->hasArg("plain")) {
        _server->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing body\"}");
        return;
    }

    String body = _server->arg("plain");

    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        _server->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
        return;
    }

    // Use ProtocolV2 to handle config update
    String response = ProtocolV2::getInstance().handleSetConfig(doc);
    _server->send(200, "application/json", response);
}

void WebServerManager::handleGetLogs() {
    int count = 50;
    if (_server->hasArg("count")) {
        count = _server->arg("count").toInt();
        if (count < 1) count = 1;
        if (count > 500) count = 500;
    }

    addCORSHeaders();
    _server->send(200, "application/json", getLogsJson(count));
}

void WebServerManager::handleNotFound() {
    // Serve SPIFFS static files under /data/
    String uri = _server->uri();
    if (uri.startsWith("/data/")) {
        // content-type guess
        const char* type = "text/plain";
        if (uri.endsWith(".html")) type = "text/html";
        else if (uri.endsWith(".css")) type = "text/css";
        else if (uri.endsWith(".js")) type = "application/javascript";
        else if (uri.endsWith(".json")) type = "application/json";
        else if (uri.endsWith(".png")) type = "image/png";
        else if (uri.endsWith(".jpg") || uri.endsWith(".jpeg")) type = "image/jpeg";
        else if (uri.endsWith(".svg")) type = "image/svg+xml";

        if (streamFile(uri.c_str(), type)) {
            return;
        }
    }

    addCORSHeaders();
    _server->send(404, "application/json", "{\"status\":\"error\",\"message\":\"Not found\"}");
}

void WebServerManager::addCORSHeaders() {
    if (!_server) return;
    _server->sendHeader("Access-Control-Allow-Origin", "*");
    _server->sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
    _server->sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

// ============================================================================
// JSON Builders (largely unchanged)
// ============================================================================

String WebServerManager::getMeterDataJson() {
    StaticJsonDocument<1024> doc;
    MeterData data = EnergyMeter::getInstance().getSnapshot();

    doc["valid"] = data.valid;
    doc["timestamp"] = data.timestamp;
    // Frequency is a single value reported by the metering IC (shared across phases)
    doc["frequency"] = data.frequency;

    JsonObject a = doc.createNestedObject("phaseA");
    // Keep the JSON field names stable, map to RMS fields in PhaseData
    a["voltage"] = data.phaseA.voltageRMS;
    a["current"] = data.phaseA.currentRMS;
    a["activePower"] = data.phaseA.activePower;
    a["reactivePower"] = data.phaseA.reactivePower;
    a["apparentPower"] = data.phaseA.apparentPower;
    a["powerFactor"] = data.phaseA.powerFactor;
    a["frequency"] = data.frequency;

    JsonObject b = doc.createNestedObject("phaseB");
    b["voltage"] = data.phaseB.voltageRMS;
    b["current"] = data.phaseB.currentRMS;
    b["activePower"] = data.phaseB.activePower;
    b["reactivePower"] = data.phaseB.reactivePower;
    b["apparentPower"] = data.phaseB.apparentPower;
    b["powerFactor"] = data.phaseB.powerFactor;
    b["frequency"] = data.frequency;

    JsonObject c = doc.createNestedObject("phaseC");
    c["voltage"] = data.phaseC.voltageRMS;
    c["current"] = data.phaseC.currentRMS;
    c["activePower"] = data.phaseC.activePower;
    c["reactivePower"] = data.phaseC.reactivePower;
    c["apparentPower"] = data.phaseC.apparentPower;
    c["powerFactor"] = data.phaseC.powerFactor;
    c["frequency"] = data.frequency;

    JsonObject t = doc.createNestedObject("total");
    t["activePower"] = data.totalActivePower;
    t["reactivePower"] = data.totalReactivePower;
    t["apparentPower"] = data.totalApparentPower;
    t["powerFactor"] = data.totalPowerFactor;
    t["frequency"] = data.frequency;

    String out;
    serializeJson(doc, out);
    return out;
}

String WebServerManager::getSystemStatusJson() {
    StaticJsonDocument<1024> doc;
    SystemStatus status = SystemMonitor::getInstance().getSystemStatus();

    doc["uptime"] = status.uptime;
    doc["freeHeap"] = status.freeHeap;
    doc["minFreeHeap"] = status.minFreeHeap;
    // Keep JSON field names stable while mapping to current SystemStatus fields
    doc["cpuFreqMHz"] = status.cpuFreqMHz;
    doc["cpuTemp"] = status.cpuTemperature;
    doc["wifiConnected"] = status.wifiConnected;
    doc["mqttConnected"] = status.mqttConnected;
    doc["modbusActive"] = status.modbusActive;
    doc["bootCount"] = status.bootCount;
    doc["errorCount"] = status.errorCount;

    // Network details come from SMNetworkManager (WiFi). Ethernet is not implemented in this build.
    int rssi = 0;
    String ip = "";
    String mac = "";
    if (SMNetworkManager::getInstance().isConnected()) {
        rssi = SMNetworkManager::getInstance().getRSSI();
        ip = SMNetworkManager::getInstance().getLocalIP();
        mac = SMNetworkManager::getInstance().getMACAddress();
    }
    doc["wifiRSSI"] = rssi;
    doc["ipAddress"] = ip;
    doc["macAddress"] = mac;

    doc["lastError"] = (int)status.lastError;

    String out;
    serializeJson(doc, out);
    return out;
}

String WebServerManager::getConfigJson() {
    StaticJsonDocument<1024> doc;

    WiFiConfig wifi;
    ModbusConfig modbus;
    MQTTConfig mqtt;
    NetworkConfig net;
    SystemConfig sys;

    ConfigManager::getInstance().getWiFiConfig(wifi);
    ConfigManager::getInstance().getModbusConfig(modbus);
    ConfigManager::getInstance().getMQTTConfig(mqtt);
    ConfigManager::getInstance().getNetworkConfig(net);
    ConfigManager::getInstance().getSystemConfig(sys);

    JsonObject jw = doc.createNestedObject("wifi");
    jw["enabled"] = wifi.enabled;
    jw["apMode"] = wifi.apMode;
    jw["useDHCP"] = wifi.useDHCP;
    jw["ssid"] = wifi.ssid;
    jw["hostname"] = wifi.hostname;
    jw["apSSID"] = wifi.apSSID;

    JsonObject jm = doc.createNestedObject("modbus");
    jm["rtuEnabled"] = modbus.rtuEnabled;
    jm["tcpEnabled"] = modbus.tcpEnabled;
    jm["slaveID"] = modbus.slaveID;
    jm["baudrate"] = modbus.baudrate;
    jm["tcpPort"] = modbus.tcpPort;

    JsonObject jq = doc.createNestedObject("mqtt");
    jq["enabled"] = mqtt.enabled;
    jq["broker"] = mqtt.broker;
    jq["port"] = mqtt.port;
    jq["baseTopic"] = mqtt.baseTopic;
    jq["publishInterval"] = mqtt.publishInterval;

    JsonObject jn = doc.createNestedObject("network");
    jn["hostname"] = net.hostname;
    jn["mdnsEnabled"] = net.mdnsEnabled;
    jn["mdnsName"] = net.mdnsName;
    jn["ntpEnabled"] = net.ntpEnabled;
    jn["ntpServer"] = net.ntpServer;
    jn["timezoneOffset"] = net.timezoneOffset;

    JsonObject js = doc.createNestedObject("system");
    js["readInterval"] = sys.readInterval;
    js["publishInterval"] = sys.publishInterval;
    js["webServerPort"] = sys.webServerPort;
    js["otaEnabled"] = sys.otaEnabled;

    String out;
    serializeJson(doc, out);
    return out;
}

String WebServerManager::getLogsJson(int count) {
    StaticJsonDocument<2048> doc;
    JsonArray arr = doc.createNestedArray("logs");

    std::vector<LoggedReading> readings = DataLogger::getInstance().getRecentReadings(count);
    for (const auto& r : readings) {
        JsonObject o = arr.createNestedObject();
        o["ts"] = r.timestamp;
        o["va"] = r.data.phaseA.voltageRMS;
        o["ia"] = r.data.phaseA.currentRMS;
        o["pa"] = r.data.phaseA.activePower;
        o["vb"] = r.data.phaseB.voltageRMS;
        o["ib"] = r.data.phaseB.currentRMS;
        o["pb"] = r.data.phaseB.activePower;
        o["vc"] = r.data.phaseC.voltageRMS;
        o["ic"] = r.data.phaseC.currentRMS;
        o["pc"] = r.data.phaseC.activePower;
    }

    String out;
    serializeJson(doc, out);
    return out;
}
