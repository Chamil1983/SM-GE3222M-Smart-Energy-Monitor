#pragma once

// SM-GE3222M V2.0 - Protocol V2 (JSON-based)
// Singleton JSON protocol handler for V2 features
// Coexists with V1.0 protocol in TCP server

#include <Arduino.h>
#include <ArduinoJson.h>
#include "DataTypes.h"

class ProtocolV2 {
public:
    enum class ResponseStatus {
        OK,
        ERROR,
        INVALID_COMMAND,
        INVALID_PARAMS,
        NOT_AUTHORIZED
    };
    
    static ProtocolV2& getInstance() {
        static ProtocolV2 instance;
        return instance;
    }
    
    // Request parsing
    bool parseRequest(const String& json, String& command, JsonDocument& params);
    
    // Response building
    String buildResponse(ResponseStatus status, const String& message = "");
    String buildResponse(ResponseStatus status, const JsonDocument& data);
    
    // Command handlers
    String handleCommand(const String& command, const JsonDocument& params);
    
    // Specific command handlers
    String handleGetMeterData(const JsonDocument& params);
    String handleGetSystemStatus(const JsonDocument& params);
    String handleGetConfig(const JsonDocument& params);
    String handleSetConfig(const JsonDocument& params);
    String handleGetCalibration(const JsonDocument& params);
    String handleSetCalibration(const JsonDocument& params);
    String handleGetLogs(const JsonDocument& params);
    String handleReset(const JsonDocument& params);
    String handleFactoryReset(const JsonDocument& params);
    
    // Helper functions (public for WebServerManager)
    void meterDataToJson(const MeterData& data, JsonDocument& doc);
    void phaseDataToJson(const PhaseData& phase, JsonObject& obj);
    void systemStatusToJson(const SystemStatus& status, JsonDocument& doc);
    void configToJson(JsonDocument& doc);
    bool jsonToConfig(const JsonDocument& doc);
    
private:
    ProtocolV2();
    ~ProtocolV2() = default;
    
    // Prevent copying
    ProtocolV2(const ProtocolV2&) = delete;
    ProtocolV2& operator=(const ProtocolV2&) = delete;
    
    static const size_t JSON_DOC_SIZE = 4096;
};
