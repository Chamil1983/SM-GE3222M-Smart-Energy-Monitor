/**
 * @file MQTTClient.cpp
 * @brief MQTT client implementation
 *
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "MQTTClient.h"
#include "../core/ErrorHandler.h"
#include <ArduinoJson.h>

 // Singleton instance
MQTTClient& MQTTClient::getInstance() {
    static MQTTClient instance;
    return instance;
}

// Constructor
MQTTClient::MQTTClient()
    : mqttClient(wifiClient),
    publishInterval(DEFAULT_PUBLISH_INTERVAL),
    lastPublishTime(0),
    lastReconnectAttempt(0),
    reconnectAttempts(0),
    reconnectDelay(RECONNECT_DELAY_MIN),
    initialized(false),
    haDiscoveryPublished(false) {

    mqttClient.setBufferSize(MQTT_PACKET_SIZE_MAX);
    mqttClient.setKeepAlive(MQTT_KEEP_ALIVE_SEC);
    mqttClient.setCallback(messageCallback);
}

// Destructor
MQTTClient::~MQTTClient() {
    disconnect();
}

// Initialize MQTT client
bool MQTTClient::init(const MQTTConfig& cfg) {
    config = cfg;

    if (!config.enabled) {
        return false;
    }

    if (strlen(config.server) == 0) {
        ErrorHandler::getInstance().setError(ErrorCode::MQTT_DISCONNECTED, "MQTT server not configured");
        return false;
    }

    mqttClient.setServer(config.server, config.port);
    initialized = true;

    return true;
}

// Connect to MQTT broker
bool MQTTClient::connect() {
    if (!initialized || !config.enabled) {
        return false;
    }

    // Build client ID
    char clientID[32];
    snprintf(clientID, sizeof(clientID), "SM-GE3222M-%06X", (uint32_t)ESP.getEfuseMac());

    // Build LWT topic
    char lwtTopic[128];
    buildTopic(lwtTopic, sizeof(lwtTopic), "status");

    // Attempt connection
    bool connected = false;
    if (strlen(config.user) > 0) {
        connected = mqttClient.connect(clientID, config.user, config.password,
            lwtTopic, 1, true, "offline");
    }
    else {
        connected = mqttClient.connect(clientID, lwtTopic, 1, true, "offline");
    }

    if (connected) {
        reconnectAttempts = 0;
        reconnectDelay = RECONNECT_DELAY_MIN;

        // Publish online status
        publishStatus("online");

        // Setup subscriptions
        setupSubscriptions();

        // Publish Home Assistant discovery (once)
        if (!haDiscoveryPublished) {
            publishHomeAssistantDiscovery();
            haDiscoveryPublished = true;
        }

        ErrorHandler::getInstance().clearError(ErrorCode::MQTT_DISCONNECTED);
    }
    else {
        reconnectAttempts++;
        ErrorHandler::getInstance().setError(ErrorCode::MQTT_DISCONNECTED, "MQTT connection failed");
    }

    return connected;
}

// Disconnect from MQTT broker
void MQTTClient::disconnect() {
    if (mqttClient.connected()) {
        publishStatus("offline");
        mqttClient.disconnect();
    }
}

// Check if connected
bool MQTTClient::isConnected() const {
    return mqttClient.connected();
}

// Publish message
bool MQTTClient::publish(const char* topic, const char* payload, bool retain, uint8_t qos) {
    if (!mqttClient.connected()) {
        return false;
    }

    return mqttClient.publish(topic, payload, retain);
}

// Publish meter data
bool MQTTClient::publishMeterData(const MeterData& meterData) {
    if (!mqttClient.connected()) {
        return false;
    }

    // Check if it's time to publish
    if (millis() - lastPublishTime < publishInterval) {
        return true;
    }

    lastPublishTime = millis();

    // Publish voltage
    publishSingleValue("voltage/phase_a", meterData.phaseA.voltage);
    publishSingleValue("voltage/phase_b", meterData.phaseB.voltage);
    publishSingleValue("voltage/phase_c", meterData.phaseC.voltage);

    // Publish current
    publishSingleValue("current/phase_a", meterData.phaseA.current);
    publishSingleValue("current/phase_b", meterData.phaseB.current);
    publishSingleValue("current/phase_c", meterData.phaseC.current);

    // Publish active power
    publishSingleValue("power/active/total", meterData.totalActivePower);
    publishSingleValue("power/active/phase_a", meterData.phaseA.activePower);
    publishSingleValue("power/active/phase_b", meterData.phaseB.activePower);
    publishSingleValue("power/active/phase_c", meterData.phaseC.activePower);

    // Publish reactive power
    publishSingleValue("power/reactive/total", meterData.totalReactivePower);
    publishSingleValue("power/reactive/phase_a", meterData.phaseA.reactivePower);
    publishSingleValue("power/reactive/phase_b", meterData.phaseB.reactivePower);
    publishSingleValue("power/reactive/phase_c", meterData.phaseC.reactivePower);

    // Publish apparent power
    publishSingleValue("power/apparent/total", meterData.totalApparentPower);
    publishSingleValue("power/apparent/phase_a", meterData.phaseA.apparentPower);
    publishSingleValue("power/apparent/phase_b", meterData.phaseB.apparentPower);
    publishSingleValue("power/apparent/phase_c", meterData.phaseC.apparentPower);

    // Publish energy (import)
    publishSingleValue("energy/import/total", meterData.importActiveEnergy[0], true);
    publishSingleValue("energy/import/phase_a", meterData.importActiveEnergy[1], true);
    publishSingleValue("energy/import/phase_b", meterData.importActiveEnergy[2], true);
    publishSingleValue("energy/import/phase_c", meterData.importActiveEnergy[3], true);

    // Publish power factor
    publishSingleValue("power_factor/total", meterData.totalPowerFactor);
    publishSingleValue("power_factor/phase_a", meterData.phaseA.powerFactor);
    publishSingleValue("power_factor/phase_b", meterData.phaseB.powerFactor);
    publishSingleValue("power_factor/phase_c", meterData.phaseC.powerFactor);

    // Publish frequency and temperature
    publishSingleValue("frequency", meterData.frequency);
    publishSingleValue("temperature", meterData.temperature);

    // Publish THD
    publishSingleValue("thd/voltage/phase_a", meterData.phaseA.voltageTHDN);
    publishSingleValue("thd/voltage/phase_b", meterData.phaseB.voltageTHDN);
    publishSingleValue("thd/voltage/phase_c", meterData.phaseC.voltageTHDN);
    publishSingleValue("thd/current/phase_a", meterData.phaseA.currentTHDN);
    publishSingleValue("thd/current/phase_b", meterData.phaseB.currentTHDN);
    publishSingleValue("thd/current/phase_c", meterData.phaseC.currentTHDN);

    return true;
}

// Publish status
bool MQTTClient::publishStatus(const char* status) {
    char topic[128];
    buildTopic(topic, sizeof(topic), "status");
    return publish(topic, status, true, 1);
}

// Publish single value
void MQTTClient::publishSingleValue(const char* subtopic, float value, bool retain) {
    char topic[128];
    char payload[16];

    buildTopic(topic, sizeof(topic), subtopic);
    snprintf(payload, sizeof(payload), "%.2f", value);

    mqttClient.publish(topic, payload, retain);
}

// Build full topic path
void MQTTClient::buildTopic(char* buffer, size_t bufferSize, const char* subtopic) {
    snprintf(buffer, bufferSize, "%s/%s", config.topicPrefix, subtopic);
}

// Setup command subscriptions
void MQTTClient::setupSubscriptions() {
    char topic[128];

    // Subscribe to relay commands
    buildTopic(topic, sizeof(topic), "cmd/relay/1");
    mqttClient.subscribe(topic, 1);

    buildTopic(topic, sizeof(topic), "cmd/relay/2");
    mqttClient.subscribe(topic, 1);

    // Subscribe to energy reset command
    buildTopic(topic, sizeof(topic), "cmd/reset/energy");
    mqttClient.subscribe(topic, 1);

    // Subscribe to config update command
    buildTopic(topic, sizeof(topic), "cmd/config");
    mqttClient.subscribe(topic, 1);
}

// Static message callback
void MQTTClient::messageCallback(char* topic, uint8_t* payload, unsigned int length) {
    getInstance().handleIncomingMessage(topic, payload, length);
}

// Handle incoming MQTT message
void MQTTClient::handleIncomingMessage(const char* topic, const uint8_t* payload, unsigned int length) {
    // Convert payload to string
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';

    // Parse topic to determine command type
    if (strstr(topic, "cmd/relay/1") != nullptr) {
        // Handle relay 1 command
        // TODO: Integrate with GPIO manager
    }
    else if (strstr(topic, "cmd/relay/2") != nullptr) {
        // Handle relay 2 command
        // TODO: Integrate with GPIO manager
    }
    else if (strstr(topic, "cmd/reset/energy") != nullptr) {
        // Handle energy reset command
        // TODO: Integrate with energy meter
    }
    else if (strstr(topic, "cmd/config") != nullptr) {
        // Handle config update command
        // TODO: Integrate with config manager
    }

    // Call user callback if set
    if (userCallback) {
        userCallback(topic, payload, length);
    }
}

// Subscribe with custom callback
bool MQTTClient::subscribe(const char* topic, std::function<void(const char*, const uint8_t*, unsigned int)> callback) {
    userCallback = callback;
    return mqttClient.subscribe(topic);
}

// Unsubscribe
bool MQTTClient::unsubscribe(const char* topic) {
    return mqttClient.unsubscribe(topic);
}

// Main loop update
void MQTTClient::update() {
    if (!initialized || !config.enabled) {
        return;
    }

    // Reconnect if disconnected
    if (!mqttClient.connected()) {
        uint32_t now = millis();
        if (now - lastReconnectAttempt >= reconnectDelay) {
            lastReconnectAttempt = now;

            if (connect()) {
                reconnectDelay = RECONNECT_DELAY_MIN;
            }
            else {
                // Exponential backoff
                reconnectDelay = min(reconnectDelay * 2, RECONNECT_DELAY_MAX);
            }
        }
        return;
    }

    // Process incoming messages
    mqttClient.loop();
}

// Set publish interval
void MQTTClient::setPublishInterval(uint32_t intervalMs) {
    publishInterval = intervalMs;
}

// Set configuration
void MQTTClient::setConfig(const MQTTConfig& cfg) {
    bool needsReconnect = config.enabled &&
        (strcmp(config.server, cfg.server) != 0 ||
            config.port != cfg.port);

    config = cfg;

    if (needsReconnect && mqttClient.connected()) {
        disconnect();
    }

    if (config.enabled) {
        mqttClient.setServer(config.server, config.port);
    }
}

// Publish Home Assistant auto-discovery
bool MQTTClient::publishHomeAssistantDiscovery() {
    if (!mqttClient.connected()) {
        return false;
    }

    // Device identifier
    char deviceID[32];
    snprintf(deviceID, sizeof(deviceID), "sm_ge3222m_%06X", (uint32_t)ESP.getEfuseMac());

    // Voltage sensors
    publishHADiscoverySensor("Phase A Voltage", "voltage/phase_a", "V", "voltage", "mdi:flash");
    publishHADiscoverySensor("Phase B Voltage", "voltage/phase_b", "V", "voltage", "mdi:flash");
    publishHADiscoverySensor("Phase C Voltage", "voltage/phase_c", "V", "voltage", "mdi:flash");

    // Current sensors
    publishHADiscoverySensor("Phase A Current", "current/phase_a", "A", "current", "mdi:current-ac");
    publishHADiscoverySensor("Phase B Current", "current/phase_b", "A", "current", "mdi:current-ac");
    publishHADiscoverySensor("Phase C Current", "current/phase_c", "A", "current", "mdi:current-ac");

    // Power sensors
    publishHADiscoverySensor("Total Active Power", "power/active/total", "W", "power", "mdi:flash");
    publishHADiscoverySensor("Phase A Active Power", "power/active/phase_a", "W", "power", "mdi:flash");
    publishHADiscoverySensor("Phase B Active Power", "power/active/phase_b", "W", "power", "mdi:flash");
    publishHADiscoverySensor("Phase C Active Power", "power/active/phase_c", "W", "power", "mdi:flash");

    // Energy sensors
    publishHADiscoverySensor("Total Import Energy", "energy/import/total", "kWh", "energy", "mdi:counter");
    publishHADiscoverySensor("Phase A Import Energy", "energy/import/phase_a", "kWh", "energy", "mdi:counter");
    publishHADiscoverySensor("Phase B Import Energy", "energy/import/phase_b", "kWh", "energy", "mdi:counter");
    publishHADiscoverySensor("Phase C Import Energy", "energy/import/phase_c", "kWh", "energy", "mdi:counter");

    // Power factor sensors
    publishHADiscoverySensor("Total Power Factor", "power_factor/total", "", "power_factor", "mdi:angle-acute");
    publishHADiscoverySensor("Phase A Power Factor", "power_factor/phase_a", "", "power_factor", "mdi:angle-acute");
    publishHADiscoverySensor("Phase B Power Factor", "power_factor/phase_b", "", "power_factor", "mdi:angle-acute");
    publishHADiscoverySensor("Phase C Power Factor", "power_factor/phase_c", "", "power_factor", "mdi:angle-acute");

    // Frequency and temperature
    publishHADiscoverySensor("Frequency", "frequency", "Hz", "frequency", "mdi:sine-wave");
    publishHADiscoverySensor("Temperature", "temperature", "°C", "temperature", "mdi:thermometer");

    return true;
}

// Publish Home Assistant discovery sensor
void MQTTClient::publishHADiscoverySensor(const char* name, const char* subtopic,
    const char* unit, const char* deviceClass, const char* icon) {
    StaticJsonDocument<512> doc;
    char topic[256];
    char stateTopic[128];
    char uniqueID[64];
    char deviceID[32];

    snprintf(deviceID, sizeof(deviceID), "sm_ge3222m_%06X", (uint32_t)ESP.getEfuseMac());
    snprintf(uniqueID, sizeof(uniqueID), "%s_%s", deviceID, subtopic);

    // Replace slashes with underscores for unique ID
    for (char* p = uniqueID; *p; p++) {
        if (*p == '/') *p = '_';
    }

    buildTopic(stateTopic, sizeof(stateTopic), subtopic);
    snprintf(topic, sizeof(topic), "homeassistant/sensor/%s/%s/config", deviceID, uniqueID);

    doc["name"] = name;
    doc["unique_id"] = uniqueID;
    doc["state_topic"] = stateTopic;
    doc["unit_of_measurement"] = unit;

    if (deviceClass) {
        doc["device_class"] = deviceClass;
    }

    if (icon) {
        doc["icon"] = icon;
    }

    JsonObject device = doc.createNestedObject("device");
    device["identifiers"][0] = deviceID;
    device["name"] = "SM-GE3222M Smart Energy Monitor";
    device["model"] = "SM-GE3222M V2.0";
    device["manufacturer"] = "Smart Meter Solutions";

    char buffer[512];
    serializeJson(doc, buffer, sizeof(buffer));

    mqttClient.publish(topic, buffer, true);
}

// Publish Home Assistant discovery binary sensor
void MQTTClient::publishHADiscoveryBinarySensor(const char* name, const char* subtopic, const char* deviceClass) {
    // Similar implementation to publishHADiscoverySensor but for binary sensors
}

// Publish Home Assistant discovery switch
void MQTTClient::publishHADiscoverySwitch(const char* name, const char* commandSubtopic, const char* stateSubtopic) {
    // Similar implementation to publishHADiscoverySensor but for switches
}