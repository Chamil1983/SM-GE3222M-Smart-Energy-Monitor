#include "WebServerManager.h"
#include "../core/ErrorHandler.h"
#include "../storage/ConfigManager.h"
#include "../energy/EnergyMeter.h"
#include "../storage/DataLogger.h"

// Singleton instance
WebServerManager& WebServerManager::getInstance() {
    static WebServerManager instance;
    return instance;
}

WebServerManager::WebServerManager()
    : server_(80),
      ws_("/ws"),
      initialized_(false),
      running_(false),
      lastRateLimitCleanup_(0) {
}

bool WebServerManager::init() {
    if (initialized_) {
        return true;
    }
    
    // Initialize SPIFFS if not already done
    if (!SPIFFS.begin(true)) {
        ErrorHandler::getInstance().logError(ErrorCode::SPIFFS_MOUNT_FAIL, "Failed to mount SPIFFS");
        return false;
    }
    
    setupWebSocket();
    setupAPIEndpoints();
    
    // Serve static files
    server_.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    
    // Handle 404
    server_.onNotFound([](AsyncWebServerRequest* request) {
        request->send(404, "application/json", "{\"error\":\"Not found\"}");
    });
    
    initialized_ = true;
    ErrorHandler::getInstance().logInfo("WebServerManager initialized");
    return true;
}

void WebServerManager::begin() {
    if (!initialized_) {
        ErrorHandler::getInstance().logError(ErrorCode::WEB_SERVER_START_FAILED, "WebServer not initialized");
        return;
    }
    
    server_.begin();
    running_ = true;
    ErrorHandler::getInstance().logInfo("Web server started on port 80");
}

void WebServerManager::stop() {
    if (running_) {
        server_.end();
        running_ = false;
        ErrorHandler::getInstance().logInfo("Web server stopped");
    }
}

void WebServerManager::setupWebSocket() {
    ws_.onEvent(onWebSocketEvent);
    server_.addHandler(&ws_);
    ErrorHandler::getInstance().logInfo("WebSocket endpoint /ws configured");
}

void WebServerManager::pushData(const String& jsonData) {
    if (running_) {
        ws_.textAll(jsonData);
    }
}

void WebServerManager::pushData(const JsonDocument& doc) {
    String output;
    serializeJson(doc, output);
    pushData(output);
}

void WebServerManager::serveStatic(const char* uri, const char* path) {
    server_.serveStatic(uri, SPIFFS, path);
}

void WebServerManager::registerHandler(const char* uri, WebRequestMethod method,
                                       void (*handler)(AsyncWebServerRequest*)) {
    server_.on(uri, method, handler);
}

void WebServerManager::update() {
    // Clean WebSocket clients
    ws_.cleanupClients();
    
    // Cleanup rate limits periodically (every minute)
    unsigned long now = millis();
    if (now - lastRateLimitCleanup_ >= 60000) {
        cleanupRateLimits();
        lastRateLimitCleanup_ = now;
    }
}

// Private methods - Setup API endpoints

void WebServerManager::setupAPIEndpoints() {
    // GET /api/v1/readings - Current measurements
    server_.on("/api/v1/readings", HTTP_GET, handleGetReadings);
    
    // GET /api/v1/energy - Energy accumulators
    server_.on("/api/v1/energy", HTTP_GET, handleGetEnergy);
    
    // GET /api/v1/config - Configuration
    server_.on("/api/v1/config", HTTP_GET, handleGetConfig);
    
    // PUT /api/v1/config - Update config
    server_.on("/api/v1/config", HTTP_PUT, handlePutConfig);
    
    // GET /api/v1/status - System status
    server_.on("/api/v1/status", HTTP_GET, handleGetStatus);
    
    // POST /api/v1/calibrate - Calibration
    server_.on("/api/v1/calibrate", HTTP_POST, handlePostCalibrate);
    
    // GET /api/v1/history - Data log download
    server_.on("/api/v1/history", HTTP_GET, handleGetHistory);
    
    // POST /api/v1/reset/energy - Reset energy counters
    server_.on("/api/v1/reset/energy", HTTP_POST, handleResetEnergy);
    
    // GET /api/v1/network - Network info
    server_.on("/api/v1/network", HTTP_GET, handleGetNetwork);
    
    ErrorHandler::getInstance().logInfo("REST API endpoints configured");
}

// API Handlers

void WebServerManager::handleGetReadings(AsyncWebServerRequest* request) {
    // Check rate limit
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    EnergyMeter& meter = EnergyMeter::getInstance();
    MeterData data = meter.getMeterData();
    
    DynamicJsonDocument doc(2048);
    doc["timestamp"] = millis();
    
    // Voltage
    JsonArray voltage = doc.createNestedArray("voltage");
    voltage.add(data.voltageA);
    voltage.add(data.voltageB);
    voltage.add(data.voltageC);
    
    // Current
    JsonArray current = doc.createNestedArray("current");
    current.add(data.currentA);
    current.add(data.currentB);
    current.add(data.currentC);
    
    // Power
    JsonArray activePower = doc.createNestedArray("activePower");
    activePower.add(data.activePowerA);
    activePower.add(data.activePowerB);
    activePower.add(data.activePowerC);
    doc["totalActivePower"] = data.totalActivePower;
    
    // Power Factor
    JsonArray powerFactor = doc.createNestedArray("powerFactor");
    powerFactor.add(data.powerFactorA);
    powerFactor.add(data.powerFactorB);
    powerFactor.add(data.powerFactorC);
    
    // Frequency
    doc["frequency"] = data.frequency;
    
    String output;
    serializeJson(doc, output);
    
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", output);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void WebServerManager::handleGetEnergy(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    EnergyMeter& meter = EnergyMeter::getInstance();
    MeterData data = meter.getMeterData();
    
    DynamicJsonDocument doc(1024);
    doc["timestamp"] = millis();
    doc["totalForwardActiveEnergy"] = data.totalForwardActiveEnergy;
    doc["totalReverseActiveEnergy"] = data.totalReverseActiveEnergy;
    doc["totalForwardReactiveEnergy"] = data.totalForwardReactiveEnergy;
    doc["totalReverseReactiveEnergy"] = data.totalReverseReactiveEnergy;
    
    String output;
    serializeJson(doc, output);
    
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", output);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void WebServerManager::handleGetConfig(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    ConfigManager& config = ConfigManager::getInstance();
    String configJson = config.exportJSON();
    
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", configJson);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void WebServerManager::handlePutConfig(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    // This needs body handling - will be called from onBody callback
    request->send(200, "application/json", "{\"status\":\"Config update requires body parser\"}");
}

void WebServerManager::handleGetStatus(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    DynamicJsonDocument doc(1024);
    doc["uptime"] = millis() / 1000;
    doc["freeHeap"] = ESP.getFreeHeap();
    doc["cpuFreq"] = ESP.getCpuFreqMHz();
    doc["chipModel"] = ESP.getChipModel();
    doc["sdkVersion"] = ESP.getSdkVersion();
    
    String output;
    serializeJson(doc, output);
    
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", output);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void WebServerManager::handlePostCalibrate(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    request->send(200, "application/json", "{\"status\":\"Calibration endpoint placeholder\"}");
}

void WebServerManager::handleGetHistory(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    DataLogger& logger = DataLogger::getInstance();
    String csvData = logger.exportCSV();
    
    AsyncWebServerResponse* response = request->beginResponse(200, "text/csv", csvData);
    response->addHeader("Content-Disposition", "attachment; filename=energy_log.csv");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void WebServerManager::handleResetEnergy(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    EnergyMeter& meter = EnergyMeter::getInstance();
    meter.resetEnergy();
    
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", 
                                                             "{\"status\":\"Energy counters reset\"}");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void WebServerManager::handleGetNetwork(AsyncWebServerRequest* request) {
    if (!getInstance().checkRateLimit(request->client()->remoteIP())) {
        request->send(429, "application/json", "{\"error\":\"Rate limit exceeded\"}");
        return;
    }
    
    DynamicJsonDocument doc(512);
    doc["ip"] = WiFi.localIP().toString();
    doc["subnet"] = WiFi.subnetMask().toString();
    doc["gateway"] = WiFi.gatewayIP().toString();
    doc["dns"] = WiFi.dnsIP().toString();
    doc["mac"] = WiFi.macAddress();
    doc["hostname"] = WiFi.getHostname();
    doc["rssi"] = WiFi.RSSI();
    doc["ssid"] = WiFi.SSID();
    
    String output;
    serializeJson(doc, output);
    
    AsyncWebServerResponse* response = request->beginResponse(200, "application/json", output);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

// WebSocket handlers

void WebServerManager::onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                                        AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            ErrorHandler::getInstance().logInfo("WebSocket client connected: " + 
                                              client->remoteIP().toString());
            break;
            
        case WS_EVT_DISCONNECT:
            ErrorHandler::getInstance().logInfo("WebSocket client disconnected");
            break;
            
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
            
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void WebServerManager::handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        String message = (char*)data;
        
        // Handle different message types
        if (message == "getReadings") {
            EnergyMeter& meter = EnergyMeter::getInstance();
            MeterData meterData = meter.getMeterData();
            
            DynamicJsonDocument doc(2048);
            doc["type"] = "readings";
            doc["voltage_a"] = meterData.voltageA;
            doc["current_a"] = meterData.currentA;
            doc["power_a"] = meterData.activePowerA;
            // Add more fields as needed
            
            getInstance().pushData(doc);
        }
    }
}

// Rate limiting

bool WebServerManager::checkRateLimit(IPAddress clientIP) {
    uint32_t ipKey = (uint32_t)clientIP;
    unsigned long now = millis();
    
    auto it = rateLimits_.find(ipKey);
    
    if (it == rateLimits_.end()) {
        // New IP
        rateLimits_[ipKey] = {now, 1};
        return true;
    }
    
    RateLimitInfo& info = it->second;
    
    // Reset counter if window expired
    if (now - info.lastReset >= RATE_LIMIT_WINDOW) {
        info.lastReset = now;
        info.requestCount = 1;
        return true;
    }
    
    // Check if limit exceeded
    if (info.requestCount >= MAX_REQUESTS_PER_MINUTE) {
        return false;
    }
    
    info.requestCount++;
    return true;
}

void WebServerManager::cleanupRateLimits() {
    unsigned long now = millis();
    
    for (auto it = rateLimits_.begin(); it != rateLimits_.end();) {
        if (now - it->second.lastReset >= RATE_LIMIT_WINDOW * 2) {
            it = rateLimits_.erase(it);
        } else {
            ++it;
        }
    }
}
