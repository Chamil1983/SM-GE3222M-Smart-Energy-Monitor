/**
 * @file WebSocketServer.h
 * @brief Real-time WebSocket data push service
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Pushes meter data at 1Hz to connected clients via WebSocket
 */

#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <vector>
#include "../../include/GlobalTypes.h"

/**
 * @class WebSocketServer
 * @brief Manages WebSocket connections for real-time meter data streaming
 * 
 * Features:
 * - Real-time meter data push at 1Hz
 * - JSON formatted data
 * - Max 4 concurrent clients
 * - Automatic client management
 * - Thread-safe operations
 * - Integration with WebServerManager
 */
class WebSocketServer {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to WebSocketServer instance
     */
    static WebSocketServer& getInstance();

    /**
     * @brief Initialize WebSocket server
     * @param ws Pointer to AsyncWebSocket from WebServerManager
     * @return true if successful, false on failure
     */
    bool init(AsyncWebSocket* ws);

    /**
     * @brief Start WebSocket service
     */
    void begin();

    /**
     * @brief Stop WebSocket service
     */
    void stop();

    /**
     * @brief Update function (call from main loop)
     * Handles periodic data push at 1Hz
     */
    void update();

    /**
     * @brief Push meter data to all connected clients
     * @param data Reference to MeterData structure
     */
    void pushMeterData(const MeterData& data);

    /**
     * @brief Push generic JSON data to all clients
     * @param jsonData JSON string to send
     */
    void pushData(const String& jsonData);

    /**
     * @brief Push JSON document to all clients
     * @param doc ArduinoJson document
     */
    void pushData(const JsonDocument& doc);

    /**
     * @brief Get number of connected clients
     * @return Number of active clients
     */
    uint8_t getClientCount() const { return _clientCount; }

    /**
     * @brief Check if at max capacity
     * @return true if max clients reached
     */
    bool isAtCapacity() const { return _clientCount >= MAX_CLIENTS; }

    /**
     * @brief Get running status
     * @return true if running
     */
    bool isRunning() const { return _running; }

private:
    WebSocketServer();
    ~WebSocketServer() = default;
    WebSocketServer(const WebSocketServer&) = delete;
    WebSocketServer& operator=(const WebSocketServer&) = delete;

    /**
     * @brief WebSocket event handler
     */
    static void onWebSocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client,
                                AwsEventType type, void* arg, uint8_t* data, size_t len);

    /**
     * @brief Handle client connection
     * @param client Pointer to client
     */
    void handleConnect(AsyncWebSocketClient* client);

    /**
     * @brief Handle client disconnection
     * @param client Pointer to client
     */
    void handleDisconnect(AsyncWebSocketClient* client);

    /**
     * @brief Handle incoming message from client
     * @param client Pointer to client
     * @param data Message data
     * @param len Message length
     */
    void handleMessage(AsyncWebSocketClient* client, uint8_t* data, size_t len);

    /**
     * @brief Convert MeterData to JSON string
     * @param data MeterData reference
     * @return JSON string
     */
    String meterDataToJson(const MeterData& data);

    /**
     * @brief Cleanup stale connections
     */
    void cleanupClients();

    AsyncWebSocket* _ws;
    bool _initialized;
    bool _running;
    uint8_t _clientCount;
    unsigned long _lastPushTime;
    unsigned long _lastCleanupTime;

    MeterData _cachedData;
    bool _hasNewData;

    static const uint8_t MAX_CLIENTS = 4;
    static const unsigned long PUSH_INTERVAL_MS = 1000;  // 1Hz
    static const unsigned long CLEANUP_INTERVAL_MS = 60000;  // 1 minute
};
