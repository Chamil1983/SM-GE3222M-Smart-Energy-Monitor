#pragma once

// SM-GE3222M V2.0 - TCP Data Server (V1.0 Protocol Compatible)
// Singleton TCP server for backward compatibility with V1.0 Tag:Value protocol
// Multi-client support using AsyncTCP

#include <Arduino.h>
#include <AsyncTCP.h>
#include "DataTypes.h"

class TCPDataServer {
public:
    static TCPDataServer& getInstance() {
        static TCPDataServer instance;
        return instance;
    }
    
    // Initialization
    bool begin(uint16_t port = 8088);
    void stop();
    
    // Processing (call from task)
    void handle();
    
    // Status
    bool isRunning() const { return _running; }
    uint16_t getPort() const { return _port; }
    uint8_t getClientCount() const { return _clientCount; }
    
private:
    TCPDataServer();
    ~TCPDataServer();
    
    // Prevent copying
    TCPDataServer(const TCPDataServer&) = delete;
    TCPDataServer& operator=(const TCPDataServer&) = delete;
    
    // Client handling
    struct ClientState {
        AsyncClient* client;
        uint32_t lastActivityTime;
        char rxBuffer[256];
        uint16_t rxBufferLen;
        bool authenticated;
        
        ClientState() : client(nullptr), lastActivityTime(0), 
                       rxBufferLen(0), authenticated(true) {
            memset(rxBuffer, 0, sizeof(rxBuffer));
        }
    };
    
    // Async TCP callbacks
    static void onConnect(void* arg, AsyncClient* client);
    static void onDisconnect(void* arg, AsyncClient* client);
    static void onData(void* arg, AsyncClient* client, void* data, size_t len);
    static void onError(void* arg, AsyncClient* client, int8_t error);
    static void onTimeout(void* arg, AsyncClient* client, uint32_t time);
    
    // Protocol handling
    void processCommand(ClientState* state, const char* command);
    void sendMeterData(AsyncClient* client);
    void sendSystemStatus(AsyncClient* client);
    void sendConfig(AsyncClient* client);
    void sendHelp(AsyncClient* client);
    
    // V1.0 Tag:Value format builders
    String buildV1MeterData();
    String buildV1SystemInfo();
    
    // Client management
    void addClient(AsyncClient* client);
    void removeClient(AsyncClient* client);
    ClientState* findClientState(AsyncClient* client);
    void cleanupInactiveClients();
    
    // Server instance
    AsyncServer* _server;
    bool _running;
    uint16_t _port;
    
    // Client tracking
    static const uint8_t MAX_CLIENTS = 4;
    ClientState _clients[MAX_CLIENTS];
    uint8_t _clientCount;
    
    // Timing
    uint32_t _lastPublishTime;
    static const uint32_t CLIENT_TIMEOUT_MS = 300000;  // 5 minutes
    static const uint32_t CLEANUP_INTERVAL_MS = 60000;  // 1 minute
    uint32_t _lastCleanupTime;
};
