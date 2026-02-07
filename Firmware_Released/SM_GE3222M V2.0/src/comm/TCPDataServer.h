/**
 * @file TCPDataServer.h
 * @brief TCP protocol server for VB.NET application compatibility
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Implements V1.0 TCP protocol for legacy VB.NET meter application
 */

#pragma once

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <vector>
#include "../../include/GlobalTypes.h"

/**
 * @class TCPDataServer
 * @brief TCP server implementing V1.0 protocol for VB.NET application
 * 
 * Features:
 * - Port 8088 (configurable)
 * - V1.0 command support (Code_NewReq, Code_Cal1-4, etc.)
 * - V1.0 data tags (AE1-AE5, BE1-BE5, CE1-CE5, etc.)
 * - JSON calibration data exchange
 * - /SYNC/ handshake protocol
 * - Max 4 concurrent clients
 */
class TCPDataServer {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to TCPDataServer instance
     */
    static TCPDataServer& getInstance();

    /**
     * @brief Initialize TCP server
     * @param port Server port (default 8088)
     * @return true if successful, false on failure
     */
    bool init(uint16_t port = 8088);

    /**
     * @brief Start TCP server
     */
    void begin();

    /**
     * @brief Stop TCP server
     */
    void stop();

    /**
     * @brief Update function (call from main loop)
     */
    void update();

    /**
     * @brief Update meter data for transmission
     * @param data Reference to MeterData
     */
    void updateMeterData(const MeterData& data);

    /**
     * @brief Set port number
     * @param port Port number
     */
    void setPort(uint16_t port) { _port = port; }

    /**
     * @brief Get port number
     * @return Port number
     */
    uint16_t getPort() const { return _port; }

    /**
     * @brief Get number of connected clients
     * @return Client count
     */
    uint8_t getClientCount() const { return _clientCount; }

    /**
     * @brief Check if server is running
     * @return true if running
     */
    bool isRunning() const { return _running; }

private:
    TCPDataServer();
    ~TCPDataServer() = default;
    TCPDataServer(const TCPDataServer&) = delete;
    TCPDataServer& operator=(const TCPDataServer&) = delete;

    /**
     * @brief Handle new client connection
     * @param client Pointer to AsyncClient
     */
    static void handleNewClient(void* arg, AsyncClient* client);

    /**
     * @brief Handle client data reception
     * @param client Pointer to AsyncClient
     * @param data Data buffer
     * @param len Data length
     */
    static void handleClientData(void* arg, AsyncClient* client, void* data, size_t len);

    /**
     * @brief Handle client disconnection
     * @param client Pointer to AsyncClient
     */
    static void handleClientDisconnect(void* arg, AsyncClient* client);

    /**
     * @brief Handle client error
     * @param client Pointer to AsyncClient
     * @param error Error code
     */
    static void handleClientError(void* arg, AsyncClient* client, int8_t error);

    /**
     * @brief Process received command
     * @param client Pointer to client
     * @param command Command string
     */
    void processCommand(AsyncClient* client, const String& command);

    /**
     * @brief Send meter data in V1.0 format
     * @param client Pointer to client
     */
    void sendMeterData(AsyncClient* client);

    /**
     * @brief Send calibration data as JSON
     * @param client Pointer to client
     * @param calType Calibration type (1-4)
     */
    void sendCalibrationData(AsyncClient* client, uint8_t calType);

    /**
     * @brief Reset calibration data
     * @param calType Calibration type (1-4)
     */
    void resetCalibration(uint8_t calType);

    /**
     * @brief Save configuration
     */
    void saveConfig();

    /**
     * @brief Reset configuration to defaults
     */
    void resetConfig();

    /**
     * @brief Reboot device
     */
    void reboot();

    /**
     * @brief Format meter data tag (e.g., "AE1:value")
     * @param tag Tag name
     * @param value Value
     * @return Formatted string
     */
    String formatDataTag(const char* tag, float value);

    /**
     * @brief Find client in list
     * @param client Client pointer
     * @return Index or -1 if not found
     */
    int findClientIndex(AsyncClient* client);

    /**
     * @brief Remove client from list
     * @param client Client pointer
     */
    void removeClient(AsyncClient* client);

    AsyncServer* _server;
    uint16_t _port;
    bool _initialized;
    bool _running;
    uint8_t _clientCount;

    std::vector<AsyncClient*> _clients;
    MeterData _meterData;
    unsigned long _lastDataUpdate;

    static const uint8_t MAX_CLIENTS = 4;
    static const size_t BUFFER_SIZE = 1024;
    static const char* SYNC_HANDSHAKE;
};
