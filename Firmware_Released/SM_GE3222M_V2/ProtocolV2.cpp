#include "ProtocolV2.h"
#include "Logger.h"
#include "EnergyMeter.h"
#include "SystemMonitor.h"
#include "ConfigManager.h"
#include "CalibrationManager.h"
#include "DataLogger.h"

ProtocolV2::ProtocolV2() {
}

bool ProtocolV2::parseRequest(const String& json, String& command, JsonDocument& params) {
    StaticJsonDocument<JSON_DOC_SIZE> doc;
    
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Logger::getInstance().error("ProtocolV2 JSON parse error: %s", error.c_str());
        return false;
    }
    
    if (!doc.containsKey("cmd")) {
        Logger::getInstance().error("ProtocolV2 Missing 'cmd' field");
        return false;
    }
    
    command = doc["cmd"].as<String>();
    
    if (doc.containsKey("data")) {
        params = doc["data"];
    }
    
    return true;
}

String ProtocolV2::buildResponse(ResponseStatus status, const String& message) {
    StaticJsonDocument<512> doc;
    
    switch (status) {
        case ResponseStatus::OK:
            doc["status"] = "ok";
            break;
        case ResponseStatus::ERROR:
            doc["status"] = "error";
            break;
        case ResponseStatus::INVALID_COMMAND:
            doc["status"] = "error";
            doc["message"] = "Invalid command";
            return "";
        case ResponseStatus::INVALID_PARAMS:
            doc["status"] = "error";
            doc["message"] = "Invalid parameters";
            return "";
        case ResponseStatus::NOT_AUTHORIZED:
            doc["status"] = "error";
            doc["message"] = "Not authorized";
            return "";
    }
    
    if (message.length() > 0) {
        doc["message"] = message;
    }
    
    String output;
    serializeJson(doc, output);
    return output;
}

String ProtocolV2::buildResponse(ResponseStatus status, const JsonDocument& data) {
    DynamicJsonDocument doc(JSON_DOC_SIZE);
    
    if (status == ResponseStatus::OK) {
        doc["status"] = "ok";
        doc["data"] = data;
    } else {
        doc["status"] = "error";
        if (data.containsKey("message")) {
            doc["message"] = data["message"];
        }
    }
    
    String output;
    serializeJson(doc, output);
    return output;
}

String ProtocolV2::handleCommand(const String& command, const JsonDocument& params) {
    Logger::getInstance().debug("ProtocolV2 Command: %s", command.c_str());
    
    if (command == "getMeterData") {
        return handleGetMeterData(params);
    } else if (command == "getSystemStatus") {
        return handleGetSystemStatus(params);
    } else if (command == "getConfig") {
        return handleGetConfig(params);
    } else if (command == "setConfig") {
        return handleSetConfig(params);
    } else if (command == "getCalibration") {
        return handleGetCalibration(params);
    } else if (command == "setCalibration") {
        return handleSetCalibration(params);
    } else if (command == "getLogs") {
        return handleGetLogs(params);
    } else if (command == "reset") {
        return handleReset(params);
    } else if (command == "factoryReset") {
        return handleFactoryReset(params);
    }
    
    return buildResponse(ResponseStatus::INVALID_COMMAND);
}

String ProtocolV2::handleGetMeterData(const JsonDocument& params) {
    // Trigger an update to get the latest readings, then return snapshot
    if (!EnergyMeter::getInstance().update()) {
        return buildResponse(ResponseStatus::ERROR, "Failed to read meter data");
    }
    MeterData data = EnergyMeter::getInstance().getSnapshot();
    DynamicJsonDocument doc(JSON_DOC_SIZE);
    meterDataToJson(data, doc);
    return buildResponse(ResponseStatus::OK, doc);
}

String ProtocolV2::handleGetSystemStatus(const JsonDocument& params) {
    SystemStatus status = SystemMonitor::getInstance().getSystemStatus();
    DynamicJsonDocument doc(JSON_DOC_SIZE);
    systemStatusToJson(status, doc);
    return buildResponse(ResponseStatus::OK, doc);
}

String ProtocolV2::handleGetConfig(const JsonDocument& params) {
    DynamicJsonDocument doc(JSON_DOC_SIZE);
    configToJson(doc);
    
    return buildResponse(ResponseStatus::OK, doc);
}

String ProtocolV2::handleSetConfig(const JsonDocument& params) {
    if (!params.containsKey("config")) {
        return buildResponse(ResponseStatus::INVALID_PARAMS, "Missing 'config' field");
    }
    
    if (!jsonToConfig(params["config"])) {
        return buildResponse(ResponseStatus::ERROR, "Failed to apply configuration");
    }
    
    return buildResponse(ResponseStatus::OK, "Configuration updated");
}

String ProtocolV2::handleGetCalibration(const JsonDocument& params) {
    CalibrationConfig cal;
    if (!CalibrationManager::getInstance().getCalibration(cal)) {
        return buildResponse(ResponseStatus::ERROR, "Failed to read calibration");
    }
    
    DynamicJsonDocument doc(JSON_DOC_SIZE);
    
    doc["lineFreq"] = cal.lineFreq;
    doc["pgaGain"] = cal.pgaGain;
    
    JsonArray calRegs = doc.createNestedArray("calRegs");
    for (int i = 0; i < 13; i++) {
        calRegs.add(cal.calRegs[i]);
    }
    
    JsonArray harCalRegs = doc.createNestedArray("harCalRegs");
    for (int i = 0; i < 7; i++) {
        harCalRegs.add(cal.harCalRegs[i]);
    }
    
    JsonArray measCalRegs = doc.createNestedArray("measCalRegs");
    for (int i = 0; i < 15; i++) {
        measCalRegs.add(cal.measCalRegs[i]);
    }
    
    return buildResponse(ResponseStatus::OK, doc);
}

String ProtocolV2::handleSetCalibration(const JsonDocument& params) {
    if (!params.containsKey("calibration")) {
        return buildResponse(ResponseStatus::INVALID_PARAMS, "Missing 'calibration' field");
    }
    
    CalibrationConfig cal;
    JsonObjectConst calObj = params["calibration"].as<JsonObjectConst>();
    
    if (calObj.containsKey("lineFreq")) {
        cal.lineFreq = calObj["lineFreq"];
    }
    if (calObj.containsKey("pgaGain")) {
        cal.pgaGain = calObj["pgaGain"];
    }
    
    if (calObj.containsKey("calRegs")) {
        JsonArrayConst arr = calObj["calRegs"].as<JsonArrayConst>();
        for (int i = 0; i < 13 && i < arr.size(); i++) {
            cal.calRegs[i] = arr[i];
        }
    }
    
    if (!CalibrationManager::getInstance().setCalibration(cal)) {
        return buildResponse(ResponseStatus::ERROR, "Failed to apply calibration");
    }
    
    return buildResponse(ResponseStatus::OK, "Calibration updated");
}

String ProtocolV2::handleGetLogs(const JsonDocument& params) {
    int count = params.containsKey("count") ? params["count"].as<int>() : 50;
    
    DynamicJsonDocument doc(JSON_DOC_SIZE);
    JsonArray logs = doc.createNestedArray("logs");
    
    // Get recent log entries
    // This is a placeholder - actual implementation depends on DataLogger
    
    return buildResponse(ResponseStatus::OK, doc);
}

String ProtocolV2::handleReset(const JsonDocument& params) {
    DynamicJsonDocument response(256);
    response["message"] = "Device will reset in 1 second";
    
    String result = buildResponse(ResponseStatus::OK, response);
    
    // Schedule reset
    delay(100);
    ESP.restart();
    
    return result;
}

String ProtocolV2::handleFactoryReset(const JsonDocument& params) {
    // Verify authorization
    if (!params.containsKey("confirm") || !params["confirm"].as<bool>()) {
        return buildResponse(ResponseStatus::INVALID_PARAMS, "Factory reset requires confirmation");
    }
    
    // Perform factory reset
    ConfigManager::getInstance().factoryReset();
    
    DynamicJsonDocument response(256);
    response["message"] = "Factory reset complete. Device will restart.";
    
    String result = buildResponse(ResponseStatus::OK, response);
    
    delay(100);
    ESP.restart();
    
    return result;
}

void ProtocolV2::meterDataToJson(const MeterData& data, JsonDocument& doc) {
    // Phase A
    JsonObject phaseA = doc.createNestedObject("phaseA");
    phaseDataToJson(data.phaseA, phaseA);
    
    // Phase B
    JsonObject phaseB = doc.createNestedObject("phaseB");
    phaseDataToJson(data.phaseB, phaseB);
    
    // Phase C
    JsonObject phaseC = doc.createNestedObject("phaseC");
    phaseDataToJson(data.phaseC, phaseC);
    
    // Totals
    JsonObject totals = doc.createNestedObject("totals");
    totals["activePower"] = data.totalActivePower;
    totals["reactivePower"] = data.totalReactivePower;
    totals["apparentPower"] = data.totalApparentPower;
    totals["powerFactor"] = data.totalPowerFactor;
    totals["fwdActiveEnergy"] = data.totalFwdActiveEnergy;
    totals["revActiveEnergy"] = data.totalRevActiveEnergy;
    totals["fwdReactiveEnergy"] = data.totalFwdReactiveEnergy;
    totals["revReactiveEnergy"] = data.totalRevReactiveEnergy;
    totals["apparentEnergy"] = data.totalApparentEnergy;
    
    // System
    doc["neutralCurrent"] = data.neutralCurrent;
    doc["frequency"] = data.frequency;
    doc["boardTemperature"] = data.boardTemperature;
    doc["ambientTemperature"] = data.ambientTemperature;
    doc["ambientHumidity"] = data.ambientHumidity;
    doc["timestamp"] = data.timestamp;
    doc["sequenceNumber"] = data.sequenceNumber;
}

void ProtocolV2::phaseDataToJson(const PhaseData& phase, JsonObject& obj) {
    obj["voltageRMS"] = phase.voltageRMS;
    obj["currentRMS"] = phase.currentRMS;
    obj["activePower"] = phase.activePower;
    obj["reactivePower"] = phase.reactivePower;
    obj["apparentPower"] = phase.apparentPower;
    obj["powerFactor"] = phase.powerFactor;
    obj["meanPhaseAngle"] = phase.meanPhaseAngle;
    obj["voltagePhaseAngle"] = phase.voltagePhaseAngle;
    obj["voltageTHDN"] = phase.voltageTHDN;
    obj["currentTHDN"] = phase.currentTHDN;
    obj["fundamentalPower"] = phase.fundamentalPower;
    obj["harmonicPower"] = phase.harmonicPower;
    obj["fwdActiveEnergy"] = phase.fwdActiveEnergy;
    obj["revActiveEnergy"] = phase.revActiveEnergy;
    obj["fwdReactiveEnergy"] = phase.fwdReactiveEnergy;
    obj["revReactiveEnergy"] = phase.revReactiveEnergy;
    obj["apparentEnergy"] = phase.apparentEnergy;
}

void ProtocolV2::systemStatusToJson(const SystemStatus& status, JsonDocument& doc) {
    doc["uptime"] = status.uptime;
    doc["freeHeap"] = status.freeHeap;
    doc["minFreeHeap"] = status.minFreeHeap;
    doc["cpuFreqMHz"] = status.cpuFreqMHz;
    doc["cpuTemperature"] = status.cpuTemperature;
    doc["errorCount"] = status.errorCount;
    doc["lastError"] = static_cast<uint16_t>(status.lastError);
    doc["bootCount"] = status.bootCount;
    doc["wifiConnected"] = status.wifiConnected;
    doc["mqttConnected"] = status.mqttConnected;
    doc["modbusActive"] = status.modbusActive;
}

void ProtocolV2::configToJson(JsonDocument& doc) {
    SystemConfig sysCfg;
    WiFiConfig wifiCfg;
    ModbusConfig modbusCfg;
    MQTTConfig mqttCfg;
    NetworkConfig netCfg;
    
    ConfigManager& cfg = ConfigManager::getInstance();
    
    if (cfg.getSystemConfig(sysCfg)) {
        JsonObject sys = doc.createNestedObject("system");
        sys["readInterval"] = sysCfg.readInterval;
        sys["publishInterval"] = sysCfg.publishInterval;
        sys["webServerEnabled"] = sysCfg.webServerEnabled;
        sys["webServerPort"] = sysCfg.webServerPort;
        sys["otaEnabled"] = sysCfg.otaEnabled;
        sys["logLevel"] = static_cast<uint8_t>(sysCfg.logLevel);
        sys["watchdogEnabled"] = sysCfg.watchdogEnabled;
        sys["watchdogTimeout"] = sysCfg.watchdogTimeout;
    }
    
    if (cfg.getWiFiConfig(wifiCfg)) {
        JsonObject wifi = doc.createNestedObject("wifi");
        wifi["enabled"] = wifiCfg.enabled;
        wifi["apMode"] = wifiCfg.apMode;
        wifi["ssid"] = wifiCfg.ssid;
        wifi["hostname"] = wifiCfg.hostname;
        wifi["useDHCP"] = wifiCfg.useDHCP;
    }
    
    if (cfg.getModbusConfig(modbusCfg)) {
        JsonObject modbus = doc.createNestedObject("modbus");
        modbus["rtuEnabled"] = modbusCfg.rtuEnabled;
        modbus["tcpEnabled"] = modbusCfg.tcpEnabled;
        modbus["slaveID"] = modbusCfg.slaveID;
        modbus["baudrate"] = modbusCfg.baudrate;
        modbus["tcpPort"] = modbusCfg.tcpPort;
    }
    
    if (cfg.getMQTTConfig(mqttCfg)) {
        JsonObject mqtt = doc.createNestedObject("mqtt");
        mqtt["enabled"] = mqttCfg.enabled;
        mqtt["broker"] = mqttCfg.broker;
        mqtt["port"] = mqttCfg.port;
        mqtt["clientID"] = mqttCfg.clientID;
        mqtt["baseTopic"] = mqttCfg.baseTopic;
        mqtt["publishInterval"] = mqttCfg.publishInterval;
    }
}

bool ProtocolV2::jsonToConfig(const JsonDocument& doc) {
    ConfigManager& cfg = ConfigManager::getInstance();
    bool success = true;
    
    if (doc.containsKey("system")) {
        SystemConfig sysCfg;
        if (cfg.getSystemConfig(sysCfg)) {
            JsonObjectConst sys = doc["system"].as<JsonObjectConst>();
            if (sys.containsKey("readInterval")) sysCfg.readInterval = sys["readInterval"];
            if (sys.containsKey("publishInterval")) sysCfg.publishInterval = sys["publishInterval"];
            if (sys.containsKey("webServerEnabled")) sysCfg.webServerEnabled = sys["webServerEnabled"];
            if (sys.containsKey("webServerPort")) sysCfg.webServerPort = sys["webServerPort"];
            if (sys.containsKey("otaEnabled")) sysCfg.otaEnabled = sys["otaEnabled"];
            if (sys.containsKey("watchdogEnabled")) sysCfg.watchdogEnabled = sys["watchdogEnabled"];
            
            success &= cfg.setSystemConfig(sysCfg);
        }
    }
    
    if (doc.containsKey("wifi")) {
        WiFiConfig wifiCfg;
        if (cfg.getWiFiConfig(wifiCfg)) {
            JsonObjectConst wifi = doc["wifi"].as<JsonObjectConst>();
            if (wifi.containsKey("enabled")) wifiCfg.enabled = wifi["enabled"];
            if (wifi.containsKey("ssid")) strncpy(wifiCfg.ssid, wifi["ssid"], sizeof(wifiCfg.ssid));
            if (wifi.containsKey("password")) strncpy(wifiCfg.password, wifi["password"], sizeof(wifiCfg.password));
            if (wifi.containsKey("hostname")) strncpy(wifiCfg.hostname, wifi["hostname"], sizeof(wifiCfg.hostname));
            
            success &= cfg.setWiFiConfig(wifiCfg);
        }
    }
    
    if (doc.containsKey("mqtt")) {
        MQTTConfig mqttCfg;
        if (cfg.getMQTTConfig(mqttCfg)) {
            JsonObjectConst mqtt = doc["mqtt"].as<JsonObjectConst>();
            if (mqtt.containsKey("enabled")) mqttCfg.enabled = mqtt["enabled"];
            if (mqtt.containsKey("broker")) strncpy(mqttCfg.broker, mqtt["broker"], sizeof(mqttCfg.broker));
            if (mqtt.containsKey("port")) mqttCfg.port = mqtt["port"];
            if (mqtt.containsKey("username")) strncpy(mqttCfg.username, mqtt["username"], sizeof(mqttCfg.username));
            if (mqtt.containsKey("password")) strncpy(mqttCfg.password, mqtt["password"], sizeof(mqttCfg.password));
            
            success &= cfg.setMQTTConfig(mqttCfg);
        }
    }
    
    return success;
}
