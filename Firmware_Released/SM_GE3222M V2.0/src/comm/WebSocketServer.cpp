/**
 * @file WebSocketServer.cpp
 * @brief Real-time WebSocket data push service implementation
 */

#include "WebSocketServer.h"
#include "../diagnostics/Logger.h"

WebSocketServer& WebSocketServer::getInstance() {
    static WebSocketServer instance;
    return instance;
}

WebSocketServer::WebSocketServer()
    : _ws(nullptr)
    , _initialized(false)
    , _running(false)
    , _clientCount(0)
    , _lastPushTime(0)
    , _lastCleanupTime(0)
    , _hasNewData(false) {
}

bool WebSocketServer::init(AsyncWebSocket* ws) {
    if (_initialized) {
        return true;
    }

    if (ws == nullptr) {
        Logger::error("WebSocketServer", "Invalid WebSocket pointer");
        return false;
    }

    _ws = ws;
    _ws->onEvent(onWebSocketEvent);
    
    _initialized = true;
    Logger::getInstance().info("WebSocketServer", "Initialized successfully");
    return true;
}

void WebSocketServer::begin() {
    if (!_initialized) {
        Logger::error("WebSocketServer", "Not initialized");
        return;
    }

    _running = true;
    _lastPushTime = millis();
    _lastCleanupTime = millis();
    Logger::getInstance().info("WebSocketServer", "Started");
}

void WebSocketServer::stop() {
    if (!_running) {
        return;
    }

    _ws->closeAll();
    _running = false;
    _clientCount = 0;
    Logger::getInstance().info("WebSocketServer", "Stopped");
}

void WebSocketServer::update() {
    if (!_running) {
        return;
    }

    unsigned long now = millis();

    // Push data at 1Hz if we have new data
    if (_hasNewData && (now - _lastPushTime >= PUSH_INTERVAL_MS)) {
        if (_clientCount > 0) {
            String jsonData = meterDataToJson(_cachedData);
            _ws->textAll(jsonData);
        }
        _lastPushTime = now;
        _hasNewData = false;
    }

    // Periodic cleanup
    if (now - _lastCleanupTime >= CLEANUP_INTERVAL_MS) {
        cleanupClients();
        _lastCleanupTime = now;
    }
}

void WebSocketServer::pushMeterData(const MeterData& data) {
    if (!_running) {
        return;
    }

    _cachedData = data;
    _hasNewData = true;
}

void WebSocketServer::pushData(const String& jsonData) {
    if (!_running || _clientCount == 0) {
        return;
    }

    _ws->textAll(jsonData);
}

void WebSocketServer::pushData(const JsonDocument& doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    pushData(jsonString);
}

void WebSocketServer::onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                                       AwsEventType type, void* arg, uint8_t* data, size_t len) {
    WebSocketServer& instance = getInstance();

    switch (type) {
        case WS_EVT_CONNECT:
            instance.handleConnect(client);
            break;

        case WS_EVT_DISCONNECT:
            instance.handleDisconnect(client);
            break;

        case WS_EVT_DATA:
            instance.handleMessage(client, data, len);
            break;

        case WS_EVT_ERROR:
            Logger::error("WebSocketServer", "Client error");
            break;

        case WS_EVT_PONG:
            // Pong received from client
            break;
    }
}

void WebSocketServer::handleConnect(AsyncWebSocketClient* client) {
    if (_clientCount >= MAX_CLIENTS) {
        Logger::warn("WebSocketServer", "Max clients reached, rejecting connection");
        client->close();
        return;
    }

    _clientCount++;
    
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Client connected: %u (Total: %d)", 
             client->id(), _clientCount);
    Logger::getInstance().info("WebSocketServer", logMsg);

    // Send welcome message
    StaticJsonDocument<256> doc;
    doc["type"] = "welcome";
    doc["clientId"] = client->id();
    doc["pushRate"] = PUSH_INTERVAL_MS;
    doc["maxClients"] = MAX_CLIENTS;
    
    String welcomeMsg;
    serializeJson(doc, welcomeMsg);
    client->text(welcomeMsg);
}

void WebSocketServer::handleDisconnect(AsyncWebSocketClient* client) {
    if (_clientCount > 0) {
        _clientCount--;
    }

    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Client disconnected: %u (Total: %d)", 
             client->id(), _clientCount);
    Logger::getInstance().info("WebSocketServer", logMsg);
}

void WebSocketServer::handleMessage(AsyncWebSocketClient* client, uint8_t* data, size_t len) {
    // Parse incoming message
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, data, len);

    if (error) {
        Logger::warn("WebSocketServer", "Failed to parse client message");
        return;
    }

    // Handle client commands
    const char* cmd = doc["cmd"];
    if (cmd == nullptr) {
        return;
    }

    if (strcmp(cmd, "ping") == 0) {
        // Respond to ping
        StaticJsonDocument<128> response;
        response["type"] = "pong";
        response["timestamp"] = millis();
        
        String responseStr;
        serializeJson(response, responseStr);
        client->text(responseStr);
    }
    else if (strcmp(cmd, "status") == 0) {
        // Send status
        StaticJsonDocument<128> response;
        response["type"] = "status";
        response["clients"] = _clientCount;
        response["uptime"] = millis() / 1000;
        
        String responseStr;
        serializeJson(response, responseStr);
        client->text(responseStr);
    }
}

String WebSocketServer::meterDataToJson(const MeterData& data) {
    DynamicJsonDocument doc(2048);

    doc["timestamp"] = millis();

    // Phase A
    JsonObject phaseA = doc.createNestedObject("phaseA");
    phaseA["voltage"] = data.phaseA.voltage;
    phaseA["current"] = data.phaseA.current;
    phaseA["power"] = data.phaseA.activePower;
    phaseA["pf"] = data.phaseA.powerFactor;

    // Phase B
    JsonObject phaseB = doc.createNestedObject("phaseB");
    phaseB["voltage"] = data.phaseB.voltage;
    phaseB["current"] = data.phaseB.current;
    phaseB["power"] = data.phaseB.activePower;
    phaseB["pf"] = data.phaseB.powerFactor;

    // Phase C
    JsonObject phaseC = doc.createNestedObject("phaseC");
    phaseC["voltage"] = data.phaseC.voltage;
    phaseC["current"] = data.phaseC.current;
    phaseC["power"] = data.phaseC.activePower;
    phaseC["pf"] = data.phaseC.powerFactor;

    // Totals
    JsonObject totals = doc.createNestedObject("totals");
    totals["power"] = data.totalActivePower;
    totals["reactive"] = data.totalReactivePower;
    totals["apparent"] = data.totalApparentPower;
    totals["pf"] = data.totalPowerFactor;

    // System
    doc["frequency"] = data.frequency;
    doc["temperature"] = data.temperature;

    // Energy
    JsonObject energy = doc.createNestedObject("energy");
    energy["importTotal"] = data.importActiveEnergy[0];
    energy["exportTotal"] = data.exportActiveEnergy[0];

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

void WebSocketServer::cleanupClients() {
    if (_ws == nullptr) {
        return;
    }

    _ws->cleanupClients();
    
    // Recount actual clients
    _clientCount = _ws->count();
}
