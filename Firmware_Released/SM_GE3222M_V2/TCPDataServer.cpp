#include "TCPDataServer.h"
#include "Logger.h"
#include "EnergyMeter.h"
#include "SystemMonitor.h"
#include "ConfigManager.h"
#include <cstring>

TCPDataServer::TCPDataServer()
    : _server(nullptr),
      _running(false),
      _port(8088),
      _clientCount(0),
      _lastCleanupTime(0) {
}

TCPDataServer::~TCPDataServer() {
    stop();
}

bool TCPDataServer::begin(uint16_t port) {
    if (_running) {
        Logger::getInstance().warn("[TCP] Server already running");
        return true;
    }

    _port = port;

    // Create server instance (socket based)
    _server = new WiFiServer(_port);
    if (!_server) {
        Logger::getInstance().error("[TCP] Failed to create WiFiServer");
        return false;
    }

    _server->begin();
    _server->setNoDelay(true);

    // Reset client slots
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        _clients[i].reset();
    }
    _clientCount = 0;

    _running = true;
    _lastCleanupTime = millis();

    Logger::getInstance().info("[TCP] Server started on port %u", (unsigned)_port);
    return true;
}

void TCPDataServer::stop() {
    if (!_running) {
        return;
    }

    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        _clients[i].reset();
    }
    _clientCount = 0;

    if (_server) {
        _server->stop();
        delete _server;
        _server = nullptr;
    }

    _running = false;
    Logger::getInstance().info("[TCP] Server stopped");
}

void TCPDataServer::handle() {
    if (!_running || !_server) {
        return;
    }

    acceptNewClients();
    pollClients();

    uint32_t now = millis();
    if (now - _lastCleanupTime > CLEANUP_INTERVAL_MS) {
        cleanupInactiveClients();
        _lastCleanupTime = now;
    }
}

void TCPDataServer::acceptNewClients() {
    // Accept as many pending connections as possible (up to MAX_CLIENTS)
    while (_server->hasClient()) {
        WiFiClient newClient = _server->accept();

        if (!newClient) {
            // Nothing valid
            return;
        }

        // Find free slot
        int slot = -1;
        for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
            if (!_clients[i].inUse) {
                slot = i;
                break;
            }
        }

        if (slot < 0) {
            Logger::getInstance().warn("[TCP] Max clients reached, rejecting connection");
            newClient.stop();
            continue;
        }

        _clients[slot].client = newClient;
        _clients[slot].inUse = true;
        _clients[slot].lastActivityTime = millis();
        _clients[slot].rxBufferLen = 0;
        memset(_clients[slot].rxBuffer, 0, sizeof(_clients[slot].rxBuffer));

        _clientCount++;

        Logger::getInstance().info("[TCP] Client connected: %s", newClient.remoteIP().toString().c_str());
        // greet
        _clients[slot].client.print("SM-GE3222M V2.0 TCP Server\r\nType 'help' for commands.\r\n");
    }
}

void TCPDataServer::pollClients() {
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        if (!_clients[i].inUse) continue;

        WiFiClient& c = _clients[i].client;

        if (!c.connected()) {
            _clients[i].reset();
            if (_clientCount > 0) _clientCount--;
            continue;
        }

        // Read available data
        while (c.available()) {
            int ch = c.read();
            if (ch < 0) break;
            char cch = (char)ch;

            _clients[i].lastActivityTime = millis();

            if (cch == '\n' || cch == '\r') {
                if (_clients[i].rxBufferLen > 0) {
                    _clients[i].rxBuffer[_clients[i].rxBufferLen] = '\0';
                    processCommand(_clients[i], _clients[i].rxBuffer);
                    _clients[i].rxBufferLen = 0;
                }
            } else {
                if (_clients[i].rxBufferLen < sizeof(_clients[i].rxBuffer) - 1) {
                    _clients[i].rxBuffer[_clients[i].rxBufferLen++] = cch;
                }
            }
        }
    }
}

void TCPDataServer::cleanupInactiveClients() {
    uint32_t now = millis();
    for (uint8_t i = 0; i < MAX_CLIENTS; i++) {
        if (!_clients[i].inUse) continue;
        if (!(_clients[i].client) || !_clients[i].client.connected()) {
            _clients[i].reset();
            if (_clientCount > 0) _clientCount--;
            continue;
        }
        if ((now - _clients[i].lastActivityTime) > CLIENT_TIMEOUT_MS) {
            Logger::getInstance().warn("[TCP] Client timeout -> disconnect");
            _clients[i].reset();
            if (_clientCount > 0) _clientCount--;
        }
    }
}

void TCPDataServer::processCommand(ClientState& state, const char* command) {
    String cmd = String(command);
    cmd.trim();
    cmd.toLowerCase();

    if (cmd.length() == 0) {
        return;
    }

    Logger::getInstance().debug("[TCP] Command: %s", cmd.c_str());

    if (cmd == "data" || cmd == "getreadings") {
        sendMeterData(state.client);
    } else if (cmd == "status" || cmd == "getmeterinfo") {
        sendSystemStatus(state.client);
    } else if (cmd == "config" || cmd == "getconfig") {
        sendConfig(state.client);
    } else if (cmd == "help" || cmd == "?") {
        sendHelp(state.client);
    } else if (cmd == "reset") {
        state.client.print("OK\r\n");
        state.client.flush();
        delay(100);
        ESP.restart();
    } else {
        state.client.print("ERROR: Unknown command\r\n");
    }
}

void TCPDataServer::sendMeterData(WiFiClient& client) {
    if (!client || !client.connected()) return;
    String data = buildV1MeterData();
    client.print(data);
}

void TCPDataServer::sendSystemStatus(WiFiClient& client) {
    if (!client || !client.connected()) return;
    String data = buildV1SystemInfo();
    client.print(data);
}

void TCPDataServer::sendConfig(WiFiClient& client) {
    if (!client || !client.connected()) return;

    String response = "CONFIG:\r\n";

    SystemConfig sysCfg;
    if (ConfigManager::getInstance().getSystemConfig(sysCfg)) {
        response += "ReadInterval:" + String(sysCfg.readInterval) + "\r\n";
        response += "PublishInterval:" + String(sysCfg.publishInterval) + "\r\n";
        response += "WebPort:" + String(sysCfg.webServerPort) + "\r\n";
    } else {
        response += "ReadInterval:500\r\nPublishInterval:1000\r\nWebPort:80\r\n";
    }

    response += "END\r\n";
    client.print(response);
}

void TCPDataServer::sendHelp(WiFiClient& client) {
    if (!client || !client.connected()) return;

    String help = "SM-GE3222M V2.0 TCP Server\r\n";
    help += "Commands:\r\n";
    help += "  data       - Get meter readings (V1.0 format)\r\n";
    help += "  status     - Get system status\r\n";
    help += "  config     - Get configuration\r\n";
    help += "  reset      - Reset device\r\n";
    help += "  help       - This help message\r\n";

    client.print(help);
}

// The remaining V1.0 builders are unchanged from the previous Async implementation.


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
    //output += "WiFi:" + String(status.wifiConnected ? "1" : "0") + "\r\n";
    //output += "MQTT:" + String(status.mqttConnected ? "1" : "0") + "\r\n";
    output += "Errors:" + String(status.errorCount) + "\r\n";
    
    return output;
}
