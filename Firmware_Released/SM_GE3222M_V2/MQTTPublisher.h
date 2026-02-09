/**
 * @file MQTTPublisher.h
 * @brief MQTT publisher with Home Assistant discovery support for SM-GE3222M V2.0
 * 
 * Publishes meter data to MQTT broker with automatic reconnection and
 * Home Assistant MQTT discovery integration.
 */

#ifndef MQTTPUBLISHER_H
#define MQTTPUBLISHER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DataTypes.h"
#include "Logger.h"

class MQTTPublisher {
public:
    static MQTTPublisher& getInstance();
    
    bool begin(const MQTTConfig& config);
    void handle();
    bool publish(const MeterData& data);
    bool publishHomeAssistantDiscovery();
    
    bool isConnected();
    void disconnect();
    
private:
    MQTTPublisher();
    ~MQTTPublisher();
    MQTTPublisher(const MQTTPublisher&) = delete;
    MQTTPublisher& operator=(const MQTTPublisher&) = delete;
    
    bool connect();
    void reconnect();
    void publishDiscoverySensor(const char* sensorName, const char* deviceClass,
                               const char* unit, const char* valueTemplate,
                               const char* stateClass = nullptr);
    
    String buildStateTopic() const;
    String buildDiscoveryTopic(const char* sensorId) const;
    String buildDeviceJson() const;
    String getDeviceId() const;
    
    WiFiClient _wifiClient;
    PubSubClient _mqttClient;
    MQTTConfig _config;
    
    bool _initialized;
    bool _discoveryPublished;
    unsigned long _lastPublishTime;
    unsigned long _lastReconnectAttempt;
    uint16_t _reconnectDelay;
    
    static constexpr uint16_t MIN_RECONNECT_DELAY_MS = 1000;
    static constexpr uint16_t MAX_RECONNECT_DELAY_MS = 60000;
    static constexpr uint16_t RECONNECT_BACKOFF_MULTIPLIER = 2;
    static constexpr uint16_t MQTT_BUFFER_SIZE = 2048;
};

#endif // MQTTPUBLISHER_H
