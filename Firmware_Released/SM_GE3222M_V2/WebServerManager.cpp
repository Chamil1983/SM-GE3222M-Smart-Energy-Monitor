#include "WebServerManager.h"
#include "Logger.h"
#include "EnergyMeter.h"
#include "ConfigManager.h"
#include "DataLogger.h"
#include "ProtocolV2.h"
#include "SPIFFSManager.h"
#include <ArduinoJson.h>

const char* WebServerManager::WS_PATH = "/ws";

WebServerManager::WebServerManager() 
    : _server(nullptr), _ws(nullptr), _running(false), 
      _port(80), _lastBroadcastTime(0) {
}

WebServerManager::~WebServerManager() {
    stop();
}

bool WebServerManager::begin(uint16_t port) {
    if (_running) {
        Logger::log(LogLevel::WARN, "WebServer", "Server already running");
        return true;
    }
    
    _port = port;
    
    // Create server instance
    _server = new AsyncWebServer(_port);
    if (!_server) {
        Logger::log(LogLevel::ERROR, "WebServer", "Failed to create server");
        return false;
    }
    
    // Create WebSocket instance
    _ws = new AsyncWebSocket(WS_PATH);
    if (!_ws) {
        Logger::log(LogLevel::ERROR, "WebServer", "Failed to create WebSocket");
        delete _server;
        _server = nullptr;
        return false;
    }
    
    // Setup WebSocket and routes
    setupWebSocket();
    setupRoutes();
    
    // Start server
    _server->begin();
    _running = true;
    
    Logger::log(LogLevel::INFO, "WebServer", "Server started on port " + String(_port));
    return true;
}

void WebServerManager::stop() {
    if (!_running) {
        return;
    }
    
    if (_ws) {
        _ws->closeAll();
        delete _ws;
        _ws = nullptr;
    }
    
    if (_server) {
        _server->end();
        delete _server;
        _server = nullptr;
    }
    
    _running = false;
    Logger::log(LogLevel::INFO, "WebServer", "Server stopped");
}

void WebServerManager::setupWebSocket() {
    _ws->onEvent(onWsEvent);
    _server->addHandler(_ws);
}

void WebServerManager::setupRoutes() {
    // Serve root index.html
    _server->on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleRoot(request);
    });
    
    // API endpoints
    _server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleGetStatus(request);
    });
    
    _server->on("/api/meter", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleGetMeter(request);
    });
    
    _server->on("/api/config", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleGetConfig(request);
    });
    
    _server->on("/api/config", HTTP_POST, 
        [](AsyncWebServerRequest* request) {
            request->send(200);
        },
        nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            handlePostConfig(request, data, len);
        }
    );
    
    _server->on("/api/logs", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleGetLogs(request);
    });
    
    // Serve static files from SPIFFS
    _server->serveStatic("/", SPIFFS, "/data/")
        .setDefaultFile("index.html")
        .setCacheControl("max-age=600");
    
    // 404 handler
    _server->onNotFound([this](AsyncWebServerRequest* request) {
        handleNotFound(request);
    });
}

void WebServerManager::handle() {
    if (!_running) {
        return;
    }
    
    // Cleanup WebSocket clients
    _ws->cleanupClients();
    
    // Auto-broadcast meter data every second
    uint32_t now = millis();
    if (now - _lastBroadcastTime >= BROADCAST_INTERVAL_MS) {
        if (_ws->count() > 0) {
            broadcastMeterData();
        }
        _lastBroadcastTime = now;
    }
}

void WebServerManager::handleRoot(AsyncWebServerRequest* request) {
    if (SPIFFS.exists("/data/index.html")) {
        request->send(SPIFFS, "/data/index.html", "text/html");
    } else {
        String html = "<html><head><title>SM-GE3222M V2</title></head>";
        html += "<body><h1>SM-GE3222M Smart Energy Monitor V2.0</h1>";
        html += "<p>Web interface not installed. Upload web files to SPIFFS /data/ directory.</p>";
        html += "<p>API Endpoints:</p><ul>";
        html += "<li><a href='/api/status'>/api/status</a> - System Status</li>";
        html += "<li><a href='/api/meter'>/api/meter</a> - Meter Data</li>";
        html += "<li><a href='/api/config'>/api/config</a> - Configuration</li>";
        html += "<li><a href='/api/logs'>/api/logs</a> - Recent Logs</li>";
        html += "<li>WebSocket: ws://" + request->host() + "/ws</li>";
        html += "</ul></body></html>";
        request->send(200, "text/html", html);
    }
}

void WebServerManager::handleGetStatus(AsyncWebServerRequest* request) {
    String json = getSystemStatusJson();
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", json);
    addCORSHeaders(response);
    request->send(response);
}

void WebServerManager::handleGetMeter(AsyncWebServerRequest* request) {
    String json = getMeterDataJson();
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", json);
    addCORSHeaders(response);
    request->send(response);
}

void WebServerManager::handleGetConfig(AsyncWebServerRequest* request) {
    String json = getConfigJson();
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", json);
    addCORSHeaders(response);
    request->send(response);
}

void WebServerManager::handlePostConfig(AsyncWebServerRequest* request, uint8_t* data, size_t len) {
    String body = String((char*)data).substring(0, len);
    
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
        return;
    }
    
    // Use ProtocolV2 to handle config update
    String command = "setConfig";
    String response = ProtocolV2::getInstance().handleSetConfig(doc);
    
    AsyncWebServerResponse* resp = request->beginResponse(200, "application/json", response);
    addCORSHeaders(resp);
    request->send(resp);
}

void WebServerManager::handleGetLogs(AsyncWebServerRequest* request) {
    int count = 50;
    if (request->hasParam("count")) {
        count = request->getParam("count")->value().toInt();
    }
    
    String json = getLogsJson(count);
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", json);
    addCORSHeaders(response);
    request->send(response);
}

void WebServerManager::handleNotFound(AsyncWebServerRequest* request) {
    request->send(404, "application/json", "{\"status\":\"error\",\"message\":\"Not found\"}");
}

void WebServerManager::onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, 
                                 AwsEventType type, void* arg, uint8_t* data, size_t len) {
    WebServerManager* instance = &WebServerManager::getInstance();
    
    switch (type) {
        case WS_EVT_CONNECT:
            Logger::log(LogLevel::INFO, "WebSocket", "Client #" + String(client->id()) + 
                       " connected from " + client->remoteIP().toString());
            // Send initial data to newly connected client
            instance->broadcastMeterData();
            break;
            
        case WS_EVT_DISCONNECT:
            Logger::log(LogLevel::INFO, "WebSocket", "Client #" + String(client->id()) + " disconnected");
            break;
            
        case WS_EVT_DATA: {
            AwsFrameInfo* info = (AwsFrameInfo*)arg;
            if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
                data[len] = 0;
                String message = String((char*)data);
                instance->handleWsMessage(client, message);
            }
            break;
        }
        
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void WebServerManager::handleWsMessage(AsyncWebSocketClient* client, const String& message) {
    Logger::log(LogLevel::DEBUG, "WebSocket", "Message: " + message);
    
    // Try to parse as JSON command
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, message);
    
    if (!error && doc.containsKey("cmd")) {
        String command = doc["cmd"].as<String>();
        String response;
        
        if (command == "getMeterData") {
            response = getMeterDataJson();
        } else if (command == "getStatus") {
            response = getSystemStatusJson();
        } else if (command == "getConfig") {
            response = getConfigJson();
        } else {
            // Use ProtocolV2 for complex commands
            response = ProtocolV2::getInstance().handleCommand(command, doc);
        }
        
        if (response.length() > 0) {
            client->text(response);
        }
    } else {
        // Legacy text commands (V1.0 compatibility)
        String cmd = message;
        cmd.trim();
        cmd.toLowerCase();
        
        if (cmd == "getreadings" || cmd == "getmeterdata") {
            client->text(getMeterDataJson());
        } else if (cmd == "getmeterinfo" || cmd == "getstatus") {
            client->text(getSystemStatusJson());
        } else if (cmd == "getconfig") {
            client->text(getConfigJson());
        }
    }
}

void WebServerManager::broadcastMeterData() {
    if (!_running || !_ws || _ws->count() == 0) {
        return;
    }
    
    String json = getMeterDataJson();
    _ws->textAll(json);
}

void WebServerManager::broadcastSystemStatus() {
    if (!_running || !_ws || _ws->count() == 0) {
        return;
    }
    
    String json = getSystemStatusJson();
    _ws->textAll(json);
}

void WebServerManager::broadcastEvent(const String& event, const String& data) {
    if (!_running || !_ws || _ws->count() == 0) {
        return;
    }
    
    StaticJsonDocument<512> doc;
    doc["event"] = event;
    doc["data"] = data;
    doc["timestamp"] = millis();
    
    String json;
    serializeJson(doc, json);
    _ws->textAll(json);
}

size_t WebServerManager::getClientCount() const {
    if (_ws) {
        return _ws->count();
    }
    return 0;
}

String WebServerManager::getMeterDataJson() {
    MeterData data;
    if (!EnergyMeter::getInstance().getMeterData(data)) {
        return "{\"status\":\"error\",\"message\":\"Failed to read meter data\"}";
    }
    
    DynamicJsonDocument doc(4096);
    ProtocolV2::getInstance().meterDataToJson(data, doc);
    
    String output;
    serializeJson(doc, output);
    return output;
}

String WebServerManager::getSystemStatusJson() {
    SystemStatus status;
    EnergyMeter::getInstance().getSystemStatus(status);
    
    DynamicJsonDocument doc(2048);
    ProtocolV2::getInstance().systemStatusToJson(status, doc);
    
    String output;
    serializeJson(doc, output);
    return output;
}

String WebServerManager::getConfigJson() {
    DynamicJsonDocument doc(4096);
    ProtocolV2::getInstance().configToJson(doc);
    
    String output;
    serializeJson(doc, output);
    return output;
}

String WebServerManager::getLogsJson(int count) {
    DynamicJsonDocument doc(4096);
    JsonArray logs = doc.createNestedArray("logs");
    
    // Get recent logs from DataLogger
    // This is a placeholder - actual implementation depends on DataLogger API
    
    doc["count"] = logs.size();
    doc["timestamp"] = millis();
    
    String output;
    serializeJson(doc, output);
    return output;
}

void WebServerManager::addCORSHeaders(AsyncWebServerResponse* response) {
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
}
