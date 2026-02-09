#include "TCPDataServer.h"
#include "Logger.h"
#include "EnergyMeter.h"
#include "SystemMonitor.h"
#include "ConfigManager.h"
#include <cstring>

TCPDataServer::TCPDataServer() 
    : _server(nullptr), _running(false), _port(8088), 
      _clientCount(0), _lastPublishTime(0), _lastCleanupTime(0) {
}

TCPDataServer::~TCPDataServer() {
    stop();
}

bool TCPDataServer::begin(uint16_t port) {
    if (_running) {
        Logger::getInstance().warn("[TCP] Server already running");return true;
    }
    
    _port = port;
    _server = new AsyncServer(_port);
    
    if (!_server) {
        Logger::getInstance().error("[TCP] Failed to create server");return false;
    }
    
    // Set up callbacks
    _server->onClient([this](void* arg, AsyncClient* client) {
        onConnect(this, client);
    }, this);
    
    // Start server
    _server->begin();
    _running = true;
    
    Logger::getInstance().info("[TCP] Server started on port %u", _port);
    return true;
}

void TCPDataServer::stop() {
    if (!_running) {
        return;
    }
    
    // Disconnect all clients
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        if (_clients[i].client) {
            _clients[i].client->close(true);
            _clients[i].client = nullptr;
        }
    }
    _clientCount = 0;
    
    // Stop server
    if (_server) {
        _server->end();
        delete _server;
        _server = nullptr;
    }
    
    _running = false;
    Logger::getInstance().info("[TCP] Server stopped");}

void TCPDataServer::handle() {
    if (!_running) {
        return;
    }
    
    // Cleanup inactive clients periodically
    uint32_t now = millis();
    if (now - _lastCleanupTime > CLEANUP_INTERVAL_MS) {
        cleanupInactiveClients();
        _lastCleanupTime = now;
    }
}

void TCPDataServer::onConnect(void* arg, AsyncClient* client) {
    TCPDataServer* server = static_cast<TCPDataServer*>(arg);
    
    if (server->_clientCount >= MAX_CLIENTS) {
        Logger::getInstance().warn("[TCP] Max clients reached, rejecting connection");client->close(true);
        return;
    }
    
    String ip = client->remoteIP().toString();
    Logger::getInstance().info("[TCP] Client connected: %s", ip.c_str());
    
    server->addClient(client);
    
    // Set up client callbacks
    client->onDisconnect([server](void* arg, AsyncClient* c) {
        onDisconnect(server, c);
    }, server);
    
    client->onData([server](void* arg, AsyncClient* c, void* data, size_t len) {
        onData(server, c, data, len);
    }, server);
    
    client->onError([server](void* arg, AsyncClient* c, int8_t error) {
        onError(server, c, error);
    }, server);
    
    client->onTimeout([server](void* arg, AsyncClient* c, uint32_t time) {
        onTimeout(server, c, time);
    }, server);
}

void TCPDataServer::onDisconnect(void* arg, AsyncClient* client) {
    TCPDataServer* server = static_cast<TCPDataServer*>(arg);
    String ip = client->remoteIP().toString();
    Logger::getInstance().info("[TCP] Client disconnected: %s", ip.c_str());
    server->removeClient(client);
}

void TCPDataServer::onData(void* arg, AsyncClient* client, void* data, size_t len) {
    TCPDataServer* server = static_cast<TCPDataServer*>(arg);
    ClientState* state = server->findClientState(client);
    
    if (!state) {
        return;
    }
    
    state->lastActivityTime = millis();
    
    // Process incoming data
    char* cmdData = static_cast<char*>(data);
    for (size_t i = 0; i < len; i++) {
        char c = cmdData[i];
        
        if (c == '\n' || c == '\r') {
            if (state->rxBufferLen > 0) {
                state->rxBuffer[state->rxBufferLen] = '\0';
                server->processCommand(state, state->rxBuffer);
                state->rxBufferLen = 0;
            }
        } else if (state->rxBufferLen < sizeof(state->rxBuffer) - 1) {
            state->rxBuffer[state->rxBufferLen++] = c;
        }
    }
}

void TCPDataServer::onError(void* arg, AsyncClient* client, int8_t error) {
    Logger::getInstance().error("[TCP] Client error: %d", (int)error);
}

void TCPDataServer::onTimeout(void* arg, AsyncClient* client, uint32_t time) {
    Logger::getInstance().warn("[TCP] Client timeout");client->close(true);
}

void TCPDataServer::processCommand(ClientState* state, const char* command) {
    String cmd = String(command);
    cmd.trim();
    cmd.toLowerCase();
    
    if (cmd.length() == 0) {
        return;
    }
    
    Logger::getInstance().debug("[TCP] Command: %s", cmd);
    
    if (cmd == "data" || cmd == "getreadings") {
        sendMeterData(state->client);
    } else if (cmd == "status" || cmd == "getmeterinfo") {
        sendSystemStatus(state->client);
    } else if (cmd == "config" || cmd == "getconfig") {
        sendConfig(state->client);
    } else if (cmd == "help" || cmd == "?") {
        sendHelp(state->client);
    } else if (cmd == "reset") {
        state->client->write("OK\r\n");
        delay(100);
        ESP.restart();
    } else {
        state->client->write("ERROR: Unknown command\r\n");
    }
}

void TCPDataServer::sendMeterData(AsyncClient* client) {
    if (!client || !client->connected()) {
        return;
    }
    
    String data = buildV1MeterData();
    client->write(data.c_str(), data.length());
}

void TCPDataServer::sendSystemStatus(AsyncClient* client) {
    if (!client || !client->connected()) {
        return;
    }
    
    String data = buildV1SystemInfo();
    client->write(data.c_str(), data.length());
}

void TCPDataServer::sendConfig(AsyncClient* client) {
    if (!client || !client->connected()) {
        return;
    }
    
    String response = "CONFIG:\r\n";
    
    SystemConfig sysCfg;
    if (ConfigManager::getInstance().getSystemConfig(sysCfg)) {
        response += "ReadInterval:" + String(sysCfg.readInterval) + "\r\n";
        response += "PublishInterval:" + String(sysCfg.publishInterval) + "\r\n";
        response += "WebPort:" + String(sysCfg.webServerPort) + "\r\n";
    }
    
    response += "END\r\n";
    client->write(response.c_str(), response.length());
}

void TCPDataServer::sendHelp(AsyncClient* client) {
    if (!client || !client->connected()) {
        return;
    }
    
    String help = "SM-GE3222M V2.0 TCP Server\r\n";
    help += "Commands:\r\n";
    help += "  data       - Get meter readings (V1.0 format)\r\n";
    help += "  status     - Get system status\r\n";
    help += "  config     - Get configuration\r\n";
    help += "  reset      - Reset device\r\n";
    help += "  help       - This help message\r\n";
    
    client->write(help.c_str(), help.length());
}

String TCPDataServer::buildV1MeterData() {
    String output = "";
    
    MeterData data = EnergyMeter::getInstance().getSnapshot();
    if (!data.valid) {
        return "ERROR: No meter data available";
    }
    
    // Phase A Energy
    output += "AE1" + String(data.phaseA.fwdActiveEnergy, 3) + "\r\n";
    output += "AE2" + String(data.phaseA.revActiveEnergy, 3) + "\r\n";
    output += "AE3" + String(data.phaseA.fwdReactiveEnergy, 3) + "\r\n";
    output += "AE4" + String(data.phaseA.revReactiveEnergy, 3) + "\r\n";
    output += "AE5" + String(data.phaseA.apparentEnergy, 3) + "\r\n";
    
    // Phase B Energy
    output += "BE1" + String(data.phaseB.fwdActiveEnergy, 3) + "\r\n";
    output += "BE2" + String(data.phaseB.revActiveEnergy, 3) + "\r\n";
    output += "BE3" + String(data.phaseB.fwdReactiveEnergy, 3) + "\r\n";
    output += "BE4" + String(data.phaseB.revReactiveEnergy, 3) + "\r\n";
    output += "BE5" + String(data.phaseB.apparentEnergy, 3) + "\r\n";
    
    // Phase C Energy
    output += "CE1" + String(data.phaseC.fwdActiveEnergy, 3) + "\r\n";
    output += "CE2" + String(data.phaseC.revActiveEnergy, 3) + "\r\n";
    output += "CE3" + String(data.phaseC.fwdReactiveEnergy, 3) + "\r\n";
    output += "CE4" + String(data.phaseC.revReactiveEnergy, 3) + "\r\n";
    output += "CE5" + String(data.phaseC.apparentEnergy, 3) + "\r\n";
    
    // Total Energy
    output += "TE1" + String(data.totalFwdActiveEnergy, 3) + "\r\n";
    output += "TE2" + String(data.totalRevActiveEnergy, 3) + "\r\n";
    output += "TE3" + String(data.totalFwdReactiveEnergy, 3) + "\r\n";
    output += "TE4" + String(data.totalRevReactiveEnergy, 3) + "\r\n";
    output += "TE5" + String(data.totalApparentEnergy, 3) + "\r\n";
    output += "TE6" + String(data.totalApparentEnergy, 3) + "\r\n";
    
    // Phase A Power
    output += "AP1" + String(data.phaseA.activePower, 2) + "\r\n";
    output += "AP2" + String(data.phaseA.reactivePower, 2) + "\r\n";
    output += "AP3" + String(data.phaseA.apparentPower, 2) + "\r\n";
    output += "AP4" + String(data.phaseA.fundamentalPower, 2) + "\r\n";
    output += "AP5" + String(data.phaseA.powerFactor, 3) + "\r\n";
    output += "AP6" + String(data.phaseA.harmonicPower, 2) + "\r\n";
    output += "AP7" + String(data.phaseA.voltageRMS, 2) + "\r\n";
    output += "AP8" + String(data.phaseA.currentRMS, 2) + "\r\n";
    output += "AP9" + String(data.phaseA.meanPhaseAngle, 2) + "\r\n";
    output += "AP10" + String(data.phaseA.voltagePhaseAngle, 2) + "\r\n";
    
    // Phase B Power
    output += "BP1" + String(data.phaseB.activePower, 2) + "\r\n";
    output += "BP2" + String(data.phaseB.reactivePower, 2) + "\r\n";
    output += "BP3" + String(data.phaseB.apparentPower, 2) + "\r\n";
    output += "BP4" + String(data.phaseB.fundamentalPower, 2) + "\r\n";
    output += "BP5" + String(data.phaseB.powerFactor, 3) + "\r\n";
    output += "BP6" + String(data.phaseB.harmonicPower, 2) + "\r\n";
    output += "BP7" + String(data.phaseB.voltageRMS, 2) + "\r\n";
    output += "BP8" + String(data.phaseB.currentRMS, 2) + "\r\n";
    output += "BP9" + String(data.phaseB.meanPhaseAngle, 2) + "\r\n";
    output += "BP10" + String(data.phaseB.voltagePhaseAngle, 2) + "\r\n";
    
    // Phase C Power
    output += "CP1" + String(data.phaseC.activePower, 2) + "\r\n";
    output += "CP2" + String(data.phaseC.reactivePower, 2) + "\r\n";
    output += "CP3" + String(data.phaseC.apparentPower, 2) + "\r\n";
    output += "CP4" + String(data.phaseC.fundamentalPower, 2) + "\r\n";
    output += "CP5" + String(data.phaseC.powerFactor, 3) + "\r\n";
    output += "CP6" + String(data.phaseC.harmonicPower, 2) + "\r\n";
    output += "CP7" + String(data.phaseC.voltageRMS, 2) + "\r\n";
    output += "CP8" + String(data.phaseC.currentRMS, 2) + "\r\n";
    output += "CP9" + String(data.phaseC.meanPhaseAngle, 2) + "\r\n";
    output += "CP10" + String(data.phaseC.voltagePhaseAngle, 2) + "\r\n";
    
    // Total Power
    output += "TP1" + String(data.totalActivePower, 2) + "\r\n";
    output += "TP2" + String(data.totalReactivePower, 2) + "\r\n";
    output += "TP3" + String(data.totalApparentPower, 2) + "\r\n";
    output += "TP4" + String(data.totalPowerFactor, 3) + "\r\n";
    
    // THD
    output += "THDUA" + String(data.phaseA.voltageTHDN, 2) + "\r\n";
    output += "THDUB" + String(data.phaseB.voltageTHDN, 2) + "\r\n";
    output += "THDUC" + String(data.phaseC.voltageTHDN, 2) + "\r\n";
    output += "THDIA" + String(data.phaseA.currentTHDN, 2) + "\r\n";
    output += "THDIB" + String(data.phaseB.currentTHDN, 2) + "\r\n";
    output += "THDIC" + String(data.phaseC.currentTHDN, 2) + "\r\n";
    
    // Frequency and others
    output += "FREQ" + String(data.frequency, 2) + "\r\n";
    output += "TEMP" + String(data.boardTemperature, 1) + "\r\n";
    output += "NEUTR" + String(data.neutralCurrent, 2) + "\r\n";
    
    return output;
}

String TCPDataServer::buildV1SystemInfo() {
    String output = "SYSTEM:\r\n";
    
    SystemStatus status = SystemMonitor::getInstance().getSystemStatus();
    
    output += "Uptime:" + String(status.uptime) + "\r\n";
    output += "FreeHeap:" + String(status.freeHeap) + "\r\n";
    output += "CPUTemp:" + String(status.cpuTemperature, 1) + "\r\n";
    output += "WiFi:" + String(status.wifiConnected ? "1" : "0") + "\r\n";
    output += "MQTT:" + String(status.mqttConnected ? "1" : "0") + "\r\n";
    output += "Errors:" + String(status.errorCount) + "\r\n";
    
    return output;
}

void TCPDataServer::addClient(AsyncClient* client) {
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        if (!_clients[i].client) {
            _clients[i].client = client;
            _clients[i].lastActivityTime = millis();
            _clients[i].rxBufferLen = 0;
            _clients[i].authenticated = true;
            _clientCount++;
            return;
        }
    }
}

void TCPDataServer::removeClient(AsyncClient* client) {
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        if (_clients[i].client == client) {
            _clients[i].client = nullptr;
            _clients[i].rxBufferLen = 0;
            if (_clientCount > 0) {
                _clientCount--;
            }
            return;
        }
    }
}

TCPDataServer::ClientState* TCPDataServer::findClientState(AsyncClient* client) {
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        if (_clients[i].client == client) {
            return &_clients[i];
        }
    }
    return nullptr;
}

void TCPDataServer::cleanupInactiveClients() {
    uint32_t now = millis();
    
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        if (_clients[i].client) {
            if (now - _clients[i].lastActivityTime > CLIENT_TIMEOUT_MS) {
                Logger::getInstance().info("[TCP] Disconnecting inactive client");_clients[i].client->close(true);
                _clients[i].client = nullptr;
                if (_clientCount > 0) {
                    _clientCount--;
                }
            }
        }
    }
}
