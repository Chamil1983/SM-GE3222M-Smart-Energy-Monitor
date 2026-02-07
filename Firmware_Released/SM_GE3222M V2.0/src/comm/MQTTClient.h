/**
 * @file MQTTClient.h
 * @brief MQTT 3.1.1 client implementation for smart meter data publishing
 *
 * SM-GE3222M Smart Energy Monitor V2.0
 * Handles MQTT connectivity, publishing measurements, and command subscriptions
 * Supports Home Assistant auto-discovery
 */

#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "../../include/GlobalTypes.h"

class MQTTClient {
public:
    // Singleton instance
    static MQTTClient& getInstance();

    // Initialization and connection
    bool init(const MQTTConfig& config);
    bool connect();
    void disconnect();
    bool isConnected() const;

    // Publishing
    bool publish(const char* topic, const char* payload, bool retain = false, uint8_t qos = 0);
    bool publishMeterData(const MeterData& meterData);
    bool publishHomeAssistantDiscovery();
    bool publishStatus(const char* status);

    // Subscribing
    bool subscribe(const char* topic, std::function<void(const char*, const uint8_t*, unsigned int)> callback);
    bool unsubscribe(const char* topic);

    // Main loop update
    void update();

    // Configuration
    void setPublishInterval(uint32_t intervalMs);
    uint32_t getPublishInterval() const { return publishInterval; }
    void setConfig(const MQTTConfig& config);
    const MQTTConfig& getConfig() const { return config; }

    // Status
    uint32_t getLastPublishTime() const { return lastPublishTime; }
    uint32_t getReconnectAttempts() const { return reconnectAttempts; }

private:
    MQTTClient();
    ~MQTTClient();
    MQTTClient(const MQTTClient&) = delete;
    MQTTClient& operator=(const MQTTClient&) = delete;

    // Internal methods
    void reconnect();
    void setupSubscriptions();
    void publishSingleValue(const char* subtopic, float value, bool retain = false);
    void buildTopic(char* buffer, size_t bufferSize, const char* subtopic);
    static void messageCallback(char* topic, uint8_t* payload, unsigned int length);
    void handleIncomingMessage(const char* topic, const uint8_t* payload, unsigned int length);

    // Home Assistant discovery
    void publishHADiscoverySensor(const char* name, const char* subtopic, const char* unit,
        const char* deviceClass = nullptr, const char* icon = nullptr);
    void publishHADiscoveryBinarySensor(const char* name, const char* subtopic,
        const char* deviceClass = nullptr);
    void publishHADiscoverySwitch(const char* name, const char* commandSubtopic,
        const char* stateSubtopic = nullptr);

    // Member variables
    MQTTConfig config;
    WiFiClient wifiClient;
    PubSubClient mqttClient;

    uint32_t publishInterval;
    uint32_t lastPublishTime;
    uint32_t lastReconnectAttempt;
    uint32_t reconnectAttempts;
    uint32_t reconnectDelay;

    bool initialized;
    bool haDiscoveryPublished;

    // Command callbacks
    std::function<void(const char*, const uint8_t*, unsigned int)> userCallback;

    // Constants
    static constexpr uint32_t DEFAULT_PUBLISH_INTERVAL = 5000;  // 5 seconds
    static constexpr uint32_t RECONNECT_DELAY_MIN = 5000;       // 5 seconds
    static constexpr uint32_t RECONNECT_DELAY_MAX = 300000;     // 5 minutes
    static constexpr uint16_t MQTT_PACKET_SIZE_MAX = 512;
    static constexpr uint8_t MQTT_KEEP_ALIVE_SEC = 60;
};