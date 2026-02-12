#pragma once

// SM-GE3222M V2.0 - TCP Data Server (V1.0 Protocol Compatible)
// Singleton TCP server for backward compatibility with V1.0 Tag:Value protocol
//
// NOTE (ESP32 Arduino 3.x / ESP-IDF 5.x):
// AsyncTCP based servers can trigger LWIP core-lock asserts in some toolchains
// if the installed AsyncTCP build is not core-lock compatible.
// This implementation uses the socket-based WiFiServer/WiFiClient API to avoid
// tcp_alloc core-lock asserts ("Required to lock TCPIP core functionality!").
//
// Supports up to 4 simultaneous clients. Each client may send newline-terminated
// commands. Responses are V1.0 compatible.

#include <Arduino.h>
#include <WiFi.h>
#include "DataTypes.h"

class TCPDataServer {
public:
    static TCPDataServer& getInstance() {
        static TCPDataServer instance;
        return instance;
    }

    bool begin(uint16_t port = 8088);
    void stop();

    // Processing (call from task)
    void handle();

    bool isRunning() const { return _running; }
    uint16_t getPort() const { return _port; }
    uint8_t getClientCount() const { return _clientCount; }

private:
    TCPDataServer();
    ~TCPDataServer();

    TCPDataServer(const TCPDataServer&) = delete;
    TCPDataServer& operator=(const TCPDataServer&) = delete;

    struct ClientState {
        WiFiClient client;
        bool inUse = false;
        uint32_t lastActivityTime = 0;
        char rxBuffer[256];
        uint16_t rxBufferLen = 0;

        void reset() {
            if (client) client.stop();
            inUse = false;
            lastActivityTime = 0;
            rxBufferLen = 0;
            memset(rxBuffer, 0, sizeof(rxBuffer));
        }
    };

    // Protocol handling
    void processCommand(ClientState& state, const char* command);
    void sendMeterData(WiFiClient& client);
    void sendSystemStatus(WiFiClient& client);
    void sendConfig(WiFiClient& client);
    void sendHelp(WiFiClient& client);

    // V1.0 Tag:Value format builders
    String buildV1MeterData();
    String buildV1SystemInfo();

    // Client management
    void acceptNewClients();
    void pollClients();
    void cleanupInactiveClients();

    // Server instance
    WiFiServer* _server;
    bool _running;
    uint16_t _port;

    // Client tracking
    static const uint8_t MAX_CLIENTS = 4;
    ClientState _clients[MAX_CLIENTS];
    uint8_t _clientCount;

    // Timing
    static const uint32_t CLIENT_TIMEOUT_MS = 300000;   // 5 minutes
    static const uint32_t CLEANUP_INTERVAL_MS = 60000;  // 1 minute
    uint32_t _lastCleanupTime;
};
