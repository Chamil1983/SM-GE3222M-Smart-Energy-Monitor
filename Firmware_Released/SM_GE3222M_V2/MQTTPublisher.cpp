/**
 * @file MQTTPublisher.cpp
 * @brief Implementation of MQTT publisher with Home Assistant discovery
 */

#include "MQTTPublisher.h"
#include <algorithm>
#include <ArduinoJson.h>

MQTTPublisher& MQTTPublisher::getInstance() {
    static MQTTPublisher instance;
    return instance;
}

MQTTPublisher::MQTTPublisher()
    : _mqttClient(_wifiClient),
      _initialized(false),
      _discoveryPublished(false),
      _lastPublishTime(0),
      _lastReconnectAttempt(0),
      _reconnectDelay(MIN_RECONNECT_DELAY_MS) {
}

MQTTPublisher::~MQTTPublisher() {
    if (_mqttClient.connected()) {
        _mqttClient.disconnect();
    }
}

bool MQTTPublisher::begin(const MQTTConfig& config) {
    _config = config;
    
    if (!config.enabled) {
        Logger::getInstance().info("MQTTPublisher: Disabled in config");
        return false;
    }
    
    Logger::getInstance().info("MQTTPublisher: Starting (broker=%s:%d, clientID=%s)",
        config.broker, config.port, config.clientID);
    
    _mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
    _mqttClient.setServer(config.broker, config.port);
    
    _initialized = true;
    
    if (connect()) {
        if (config.useHomeAssistant) {
            publishHomeAssistantDiscovery();
        }
        return true;
    }
    
    return false;
}

bool MQTTPublisher::connect() {
    if (!_initialized || WiFi.status() != WL_CONNECTED) {
        return false;
    }
    
    Logger::getInstance().info("MQTTPublisher: Connecting to broker %s:%d", 
        _config.broker, _config.port);
    
    String willTopic = String(_config.baseTopic) + "/status";
    
    bool connected = false;
    if (strlen(_config.username) > 0) {
        connected = _mqttClient.connect(_config.clientID, 
                                       _config.username, 
                                       _config.password,
                                       willTopic.c_str(), 
                                       1, 
                                       true, 
                                       "offline");
    } else {
        connected = _mqttClient.connect(_config.clientID,
                                       willTopic.c_str(), 
                                       1, 
                                       true, 
                                       "offline");
    }
    
    if (connected) {
        Logger::getInstance().info("MQTTPublisher: Connected to broker");
        _mqttClient.publish(willTopic.c_str(), "online", true);
        _reconnectDelay = MIN_RECONNECT_DELAY_MS;
        return true;
    } else {
        Logger::getInstance().error("MQTTPublisher: Connection failed, rc=%d", 
            _mqttClient.state());
        return false;
    }
}

void MQTTPublisher::reconnect() {
    unsigned long now = millis();
    
    if (now - _lastReconnectAttempt < _reconnectDelay) {
        return;
    }
    
    _lastReconnectAttempt = now;
    
    if (connect()) {
        if (_config.useHomeAssistant && !_discoveryPublished) {
            publishHomeAssistantDiscovery();
        }
    } else {
        _reconnectDelay = std::min<uint16_t>(static_cast<uint16_t>(_reconnectDelay * RECONNECT_BACKOFF_MULTIPLIER),
                             static_cast<uint16_t>(MAX_RECONNECT_DELAY_MS));
        Logger::getInstance().warn("MQTTPublisher: Next reconnect in %d ms", _reconnectDelay);
    }
}

void MQTTPublisher::handle() {
    if (!_initialized || !_config.enabled) return;
    
    if (!_mqttClient.connected()) {
        reconnect();
    } else {
        _mqttClient.loop();
    }
}

bool MQTTPublisher::publish(const MeterData& data) {
    if (!_mqttClient.connected()) {
        return false;
    }
    
    unsigned long now = millis();
    if (now - _lastPublishTime < (_config.publishInterval * 1000)) {
        return true;
    }
    
    _lastPublishTime = now;
    
    StaticJsonDocument<2048> doc;
    
    doc["timestamp"] = data.timestamp;
    doc["sequence"] = data.sequenceNumber;
    
    JsonObject phaseA = doc.createNestedObject("phase_a");
    phaseA["voltage"] = data.phaseA.voltageRMS;
    phaseA["current"] = data.phaseA.currentRMS;
    phaseA["power"] = data.phaseA.activePower;
    phaseA["reactive_power"] = data.phaseA.reactivePower;
    phaseA["apparent_power"] = data.phaseA.apparentPower;
    phaseA["power_factor"] = data.phaseA.powerFactor;
    phaseA["phase_angle"] = data.phaseA.meanPhaseAngle;
    phaseA["voltage_thd"] = data.phaseA.voltageTHDN;
    phaseA["current_thd"] = data.phaseA.currentTHDN;
    phaseA["energy_import"] = data.phaseA.fwdActiveEnergy;
    phaseA["energy_export"] = data.phaseA.revActiveEnergy;
    
    JsonObject phaseB = doc.createNestedObject("phase_b");
    phaseB["voltage"] = data.phaseB.voltageRMS;
    phaseB["current"] = data.phaseB.currentRMS;
    phaseB["power"] = data.phaseB.activePower;
    phaseB["reactive_power"] = data.phaseB.reactivePower;
    phaseB["apparent_power"] = data.phaseB.apparentPower;
    phaseB["power_factor"] = data.phaseB.powerFactor;
    phaseB["phase_angle"] = data.phaseB.meanPhaseAngle;
    phaseB["voltage_thd"] = data.phaseB.voltageTHDN;
    phaseB["current_thd"] = data.phaseB.currentTHDN;
    phaseB["energy_import"] = data.phaseB.fwdActiveEnergy;
    phaseB["energy_export"] = data.phaseB.revActiveEnergy;
    
    JsonObject phaseC = doc.createNestedObject("phase_c");
    phaseC["voltage"] = data.phaseC.voltageRMS;
    phaseC["current"] = data.phaseC.currentRMS;
    phaseC["power"] = data.phaseC.activePower;
    phaseC["reactive_power"] = data.phaseC.reactivePower;
    phaseC["apparent_power"] = data.phaseC.apparentPower;
    phaseC["power_factor"] = data.phaseC.powerFactor;
    phaseC["phase_angle"] = data.phaseC.meanPhaseAngle;
    phaseC["voltage_thd"] = data.phaseC.voltageTHDN;
    phaseC["current_thd"] = data.phaseC.currentTHDN;
    phaseC["energy_import"] = data.phaseC.fwdActiveEnergy;
    phaseC["energy_export"] = data.phaseC.revActiveEnergy;
    
    JsonObject totals = doc.createNestedObject("totals");
    totals["power"] = data.totalActivePower;
    totals["reactive_power"] = data.totalReactivePower;
    totals["apparent_power"] = data.totalApparentPower;
    totals["power_factor"] = data.totalPowerFactor;
    totals["energy_import"] = data.totalFwdActiveEnergy;
    totals["energy_export"] = data.totalRevActiveEnergy;
    
    doc["frequency"] = data.frequency;
    doc["neutral_current"] = data.neutralCurrent;
    doc["board_temperature"] = data.boardTemperature;
    doc["ambient_temperature"] = data.ambientTemperature;
    doc["ambient_humidity"] = data.ambientHumidity;
    
    String stateTopic = buildStateTopic();
    char buffer[2048];
    size_t len = serializeJson(doc, buffer);
    
    bool result = _mqttClient.publish(stateTopic.c_str(), buffer, len);
    
    if (result) {
        Logger::getInstance().debug("MQTTPublisher: Published %d bytes to %s", len, stateTopic.c_str());
    } else {
        Logger::getInstance().error("MQTTPublisher: Publish failed");
    }
    
    return result;
}

bool MQTTPublisher::publishHomeAssistantDiscovery() {
    if (!_mqttClient.connected()) {
        return false;
    }
    
    Logger::getInstance().info("MQTTPublisher: Publishing Home Assistant discovery");
    
    publishDiscoverySensor("voltage_a", "voltage", "V", "{{ value_json.phase_a.voltage }}", "measurement");
    publishDiscoverySensor("voltage_b", "voltage", "V", "{{ value_json.phase_b.voltage }}", "measurement");
    publishDiscoverySensor("voltage_c", "voltage", "V", "{{ value_json.phase_c.voltage }}", "measurement");
    
    publishDiscoverySensor("current_a", "current", "A", "{{ value_json.phase_a.current }}", "measurement");
    publishDiscoverySensor("current_b", "current", "A", "{{ value_json.phase_b.current }}", "measurement");
    publishDiscoverySensor("current_c", "current", "A", "{{ value_json.phase_c.current }}", "measurement");
    
    publishDiscoverySensor("power_a", "power", "W", "{{ value_json.phase_a.power }}", "measurement");
    publishDiscoverySensor("power_b", "power", "W", "{{ value_json.phase_b.power }}", "measurement");
    publishDiscoverySensor("power_c", "power", "W", "{{ value_json.phase_c.power }}", "measurement");
    publishDiscoverySensor("power_total", "power", "W", "{{ value_json.totals.power }}", "measurement");
    
    publishDiscoverySensor("power_factor_a", "power_factor", "", "{{ value_json.phase_a.power_factor }}", "measurement");
    publishDiscoverySensor("power_factor_b", "power_factor", "", "{{ value_json.phase_b.power_factor }}", "measurement");
    publishDiscoverySensor("power_factor_c", "power_factor", "", "{{ value_json.phase_c.power_factor }}", "measurement");
    publishDiscoverySensor("power_factor_total", "power_factor", "", "{{ value_json.totals.power_factor }}", "measurement");
    
    publishDiscoverySensor("energy_import_a", "energy", "kWh", "{{ value_json.phase_a.energy_import }}", "total_increasing");
    publishDiscoverySensor("energy_import_b", "energy", "kWh", "{{ value_json.phase_b.energy_import }}", "total_increasing");
    publishDiscoverySensor("energy_import_c", "energy", "kWh", "{{ value_json.phase_c.energy_import }}", "total_increasing");
    publishDiscoverySensor("energy_import_total", "energy", "kWh", "{{ value_json.totals.energy_import }}", "total_increasing");
    
    publishDiscoverySensor("energy_export_a", "energy", "kWh", "{{ value_json.phase_a.energy_export }}", "total_increasing");
    publishDiscoverySensor("energy_export_b", "energy", "kWh", "{{ value_json.phase_b.energy_export }}", "total_increasing");
    publishDiscoverySensor("energy_export_c", "energy", "kWh", "{{ value_json.phase_c.energy_export }}", "total_increasing");
    publishDiscoverySensor("energy_export_total", "energy", "kWh", "{{ value_json.totals.energy_export }}", "total_increasing");
    
    publishDiscoverySensor("frequency", "frequency", "Hz", "{{ value_json.frequency }}", "measurement");
    publishDiscoverySensor("neutral_current", "current", "A", "{{ value_json.neutral_current }}", "measurement");
    publishDiscoverySensor("board_temperature", "temperature", "°C", "{{ value_json.board_temperature }}", "measurement");
    publishDiscoverySensor("ambient_temperature", "temperature", "°C", "{{ value_json.ambient_temperature }}", "measurement");
    publishDiscoverySensor("ambient_humidity", "humidity", "%", "{{ value_json.ambient_humidity }}", "measurement");
    
    _discoveryPublished = true;
    Logger::getInstance().info("MQTTPublisher: Home Assistant discovery complete");
    
    return true;
}

void MQTTPublisher::publishDiscoverySensor(const char* sensorName, const char* deviceClass,
                                          const char* unit, const char* valueTemplate,
                                          const char* stateClass) {
    StaticJsonDocument<768> doc;
    
    String deviceId = getDeviceId();
    String objectId = deviceId + "_" + String(sensorName);
    String uniqueId = "ge3222m_" + deviceId + "_" + String(sensorName);
    
    doc["name"] = String(sensorName);
    doc["unique_id"] = uniqueId;
    doc["object_id"] = objectId;
    doc["state_topic"] = buildStateTopic();
    doc["value_template"] = valueTemplate;
    
    if (deviceClass && strlen(deviceClass) > 0) {
        doc["device_class"] = deviceClass;
    }
    
    if (unit && strlen(unit) > 0) {
        doc["unit_of_measurement"] = unit;
    }
    
    if (stateClass && strlen(stateClass) > 0) {
        doc["state_class"] = stateClass;
    }
    
    JsonObject device = doc.createNestedObject("device");
    device["identifiers"][0] = deviceId;
    device["name"] = "SM-GE3222M Energy Monitor";
    device["model"] = "GE3222M";
    device["manufacturer"] = "SM";
    device["sw_version"] = "2.0.0";
    
    String discoveryTopic = buildDiscoveryTopic(sensorName);
    char buffer[768];
    size_t len = serializeJson(doc, buffer);
    
    _mqttClient.publish(discoveryTopic.c_str(), reinterpret_cast<const uint8_t*>(buffer), len, true);
    
    delay(50);
}

String MQTTPublisher::buildStateTopic() const {
    return String(_config.baseTopic) + "/state";
}

String MQTTPublisher::buildDiscoveryTopic(const char* sensorId) const {
    String deviceId = getDeviceId();
    return "homeassistant/sensor/" + deviceId + "/" + String(sensorId) + "/config";
}

String MQTTPublisher::getDeviceId() const {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[13];
    snprintf(macStr, sizeof(macStr), "%02x%02x%02x%02x%02x%02x", 
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

bool MQTTPublisher::isConnected() {
    return _mqttClient.connected();
}

void MQTTPublisher::disconnect() {
    if (_mqttClient.connected()) {
        String willTopic = String(_config.baseTopic) + "/status";
        _mqttClient.publish(willTopic.c_str(), "offline", true);
        _mqttClient.disconnect();
        Logger::getInstance().info("MQTTPublisher: Disconnected from broker");
    }
}
