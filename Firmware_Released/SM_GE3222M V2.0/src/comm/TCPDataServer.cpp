/**
 * @file TCPDataServer.cpp
 * @brief TCP protocol server implementation for VB.NET application
 */

#include "TCPDataServer.h"
#include "../diagnostics/Logger.h"
#include "../storage/ConfigManager.h"
#include "../energy/EnergyAccumulator.h"
#include <ESP.h>

const char* TCPDataServer::SYNC_HANDSHAKE = "/SYNC/";

TCPDataServer& TCPDataServer::getInstance() {
    static TCPDataServer instance;
    return instance;
}

TCPDataServer::TCPDataServer()
    : _server(nullptr)
    , _port(8088)
    , _initialized(false)
    , _running(false)
    , _clientCount(0)
    , _lastDataUpdate(0) {
}

bool TCPDataServer::init(uint16_t port) {
    if (_initialized) {
        return true;
    }

    _port = port;
    _server = new AsyncServer(_port);
    
    if (_server == nullptr) {
        Logger::error("TCPDataServer", "Failed to create server");
        return false;
    }

    _server->onClient(&handleNewClient, this);
    
    _initialized = true;
    
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Initialized on port %u", _port);
    Logger::info("TCPDataServer", logMsg);
    
    return true;
}

void TCPDataServer::begin() {
    if (!_initialized) {
        Logger::error("TCPDataServer", "Not initialized");
        return;
    }

    _server->begin();
    _running = true;
    
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Started on port %u", _port);
    Logger::info("TCPDataServer", logMsg);
}

void TCPDataServer::stop() {
    if (!_running) {
        return;
    }

    // Close all clients
    for (auto client : _clients) {
        if (client) {
            client->close(true);
        }
    }
    _clients.clear();
    _clientCount = 0;

    _server->end();
    _running = false;
    Logger::info("TCPDataServer", "Stopped");
}

void TCPDataServer::update() {
    // Main update handled by AsyncTCP callbacks
    // Could add periodic maintenance here if needed
}

void TCPDataServer::updateMeterData(const MeterData& data) {
    _meterData = data;
    _lastDataUpdate = millis();
}

void TCPDataServer::handleNewClient(void* arg, AsyncClient* client) {
    TCPDataServer* server = static_cast<TCPDataServer*>(arg);
    
    if (server->_clientCount >= MAX_CLIENTS) {
        Logger::warn("TCPDataServer", "Max clients reached, rejecting connection");
        client->close(true);
        delete client;
        return;
    }

    // Set up client callbacks
    client->onData(&handleClientData, server);
    client->onDisconnect(&handleClientDisconnect, server);
    client->onError(&handleClientError, server);
    
    // Add to client list
    server->_clients.push_back(client);
    server->_clientCount++;

    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Client connected (Total: %d)", server->_clientCount);
    Logger::info("TCPDataServer", logMsg);

    // Send sync handshake
    client->write(SYNC_HANDSHAKE);
    client->write("\n");
}

void TCPDataServer::handleClientData(void* arg, AsyncClient* client, void* data, size_t len) {
    TCPDataServer* server = static_cast<TCPDataServer*>(arg);
    
    // Convert data to string
    String command;
    command.reserve(len + 1);
    for (size_t i = 0; i < len; i++) {
        char c = ((char*)data)[i];
        if (c == '\n' || c == '\r') {
            if (command.length() > 0) {
                server->processCommand(client, command);
                command = "";
            }
        } else {
            command += c;
        }
    }
    
    // Process remaining command
    if (command.length() > 0) {
        server->processCommand(client, command);
    }
}

void TCPDataServer::handleClientDisconnect(void* arg, AsyncClient* client) {
    TCPDataServer* server = static_cast<TCPDataServer*>(arg);
    server->removeClient(client);
    
    char logMsg[64];
    snprintf(logMsg, sizeof(logMsg), "Client disconnected (Total: %d)", server->_clientCount);
    Logger::info("TCPDataServer", logMsg);
}

void TCPDataServer::handleClientError(void* arg, AsyncClient* client, int8_t error) {
    Logger::error("TCPDataServer", "Client error occurred");
    TCPDataServer* server = static_cast<TCPDataServer*>(arg);
    server->removeClient(client);
}

void TCPDataServer::processCommand(AsyncClient* client, const String& command) {
    Logger::getInstance().debug("TCPDataServer", ("Received: " + command).c_str());

    // Check for SYNC handshake
    if (command.indexOf(SYNC_HANDSHAKE) >= 0) {
        client->write("Code_NewReq\n");
        return;
    }

    // Handle commands
    if (command == "Code_NewReq") {
        sendMeterData(client);
    }
    else if (command == "Code_Cal1") {
        sendCalibrationData(client, 1);
    }
    else if (command == "Code_Cal2") {
        sendCalibrationData(client, 2);
    }
    else if (command == "Code_Cal3") {
        sendCalibrationData(client, 3);
    }
    else if (command == "Code_Cal4") {
        sendCalibrationData(client, 4);
    }
    else if (command == "Cal1_RST") {
        resetCalibration(1);
        client->write("OK\n");
    }
    else if (command == "Cal2_RST") {
        resetCalibration(2);
        client->write("OK\n");
    }
    else if (command == "Cal3_RST") {
        resetCalibration(3);
        client->write("OK\n");
    }
    else if (command == "Cal4_RST") {
        resetCalibration(4);
        client->write("OK\n");
    }
    else if (command == "ConfigSave") {
        saveConfig();
        client->write("OK\n");
    }
    else if (command == "ConfigRst") {
        resetConfig();
        client->write("OK\n");
    }
    else if (command == "Reboot") {
        client->write("OK\n");
        delay(100);
        reboot();
    }
}

void TCPDataServer::sendMeterData(AsyncClient* client) {
    // Energy data - Active Energy Import (AE1-AE5)
    client->write((formatDataTag("AE1", _meterData.importActiveEnergy[0]) + "\n").c_str());
    client->write((formatDataTag("AE2", _meterData.importActiveEnergy[1]) + "\n").c_str());
    client->write((formatDataTag("AE3", _meterData.importActiveEnergy[2]) + "\n").c_str());
    client->write((formatDataTag("AE4", _meterData.importActiveEnergy[3]) + "\n").c_str());
    client->write((formatDataTag("AE5", _meterData.exportActiveEnergy[0]) + "\n").c_str());

    // Energy data - Reactive Energy Import (BE1-BE5)
    client->write((formatDataTag("BE1", _meterData.importReactiveEnergy[0]) + "\n").c_str());
    client->write((formatDataTag("BE2", _meterData.importReactiveEnergy[1]) + "\n").c_str());
    client->write((formatDataTag("BE3", _meterData.importReactiveEnergy[2]) + "\n").c_str());
    client->write((formatDataTag("BE4", _meterData.importReactiveEnergy[3]) + "\n").c_str());
    client->write((formatDataTag("BE5", _meterData.exportReactiveEnergy[0]) + "\n").c_str());

    // Energy data - Apparent Energy (CE1-CE5)
    client->write((formatDataTag("CE1", _meterData.apparentEnergy[0]) + "\n").c_str());
    client->write((formatDataTag("CE2", _meterData.apparentEnergy[1]) + "\n").c_str());
    client->write((formatDataTag("CE3", _meterData.apparentEnergy[2]) + "\n").c_str());
    client->write((formatDataTag("CE4", _meterData.apparentEnergy[3]) + "\n").c_str());
    client->write((formatDataTag("CE5", 0.0) + "\n").c_str());  // Reserved

    // Temperature and system (TE1-TE6)
    client->write((formatDataTag("TE1", _meterData.temperature) + "\n").c_str());
    client->write((formatDataTag("TE2", _meterData.frequency) + "\n").c_str());
    client->write((formatDataTag("TE3", _meterData.neutralCurrentSampled) + "\n").c_str());
    client->write((formatDataTag("TE4", _meterData.neutralCurrentCalc) + "\n").c_str());
    client->write((formatDataTag("TE5", (float)ESP.getFreeHeap() / 1024.0) + "\n").c_str());
    client->write((formatDataTag("TE6", (float)millis() / 1000.0) + "\n").c_str());

    // Active Power (AP1-AP10)
    client->write((formatDataTag("AP1", _meterData.totalActivePower) + "\n").c_str());
    client->write((formatDataTag("AP2", _meterData.phaseA.activePower) + "\n").c_str());
    client->write((formatDataTag("AP3", _meterData.phaseB.activePower) + "\n").c_str());
    client->write((formatDataTag("AP4", _meterData.phaseC.activePower) + "\n").c_str());
    client->write((formatDataTag("AP5", _meterData.totalActiveFundPower) + "\n").c_str());
    client->write((formatDataTag("AP6", _meterData.phaseA.activePower) + "\n").c_str());  // Fund A
    client->write((formatDataTag("AP7", _meterData.phaseB.activePower) + "\n").c_str());  // Fund B
    client->write((formatDataTag("AP8", _meterData.phaseC.activePower) + "\n").c_str());  // Fund C
    client->write((formatDataTag("AP9", _meterData.totalActiveHarPower) + "\n").c_str());
    client->write((formatDataTag("AP10", 0.0) + "\n").c_str());  // Reserved

    // Reactive Power (BP1-BP10)
    client->write((formatDataTag("BP1", _meterData.totalReactivePower) + "\n").c_str());
    client->write((formatDataTag("BP2", _meterData.phaseA.reactivePower) + "\n").c_str());
    client->write((formatDataTag("BP3", _meterData.phaseB.reactivePower) + "\n").c_str());
    client->write((formatDataTag("BP4", _meterData.phaseC.reactivePower) + "\n").c_str());
    client->write((formatDataTag("BP5", 0.0) + "\n").c_str());  // Reserved
    client->write((formatDataTag("BP6", 0.0) + "\n").c_str());
    client->write((formatDataTag("BP7", 0.0) + "\n").c_str());
    client->write((formatDataTag("BP8", 0.0) + "\n").c_str());
    client->write((formatDataTag("BP9", 0.0) + "\n").c_str());
    client->write((formatDataTag("BP10", 0.0) + "\n").c_str());

    // Apparent Power (CP1-CP10)
    client->write((formatDataTag("CP1", _meterData.totalApparentPower) + "\n").c_str());
    client->write((formatDataTag("CP2", _meterData.phaseA.apparentPower) + "\n").c_str());
    client->write((formatDataTag("CP3", _meterData.phaseB.apparentPower) + "\n").c_str());
    client->write((formatDataTag("CP4", _meterData.phaseC.apparentPower) + "\n").c_str());
    client->write((formatDataTag("CP5", 0.0) + "\n").c_str());
    client->write((formatDataTag("CP6", 0.0) + "\n").c_str());
    client->write((formatDataTag("CP7", 0.0) + "\n").c_str());
    client->write((formatDataTag("CP8", 0.0) + "\n").c_str());
    client->write((formatDataTag("CP9", 0.0) + "\n").c_str());
    client->write((formatDataTag("CP10", 0.0) + "\n").c_str());

    // Power Factor and THD (TP1-TP10)
    client->write((formatDataTag("TP1", _meterData.totalPowerFactor) + "\n").c_str());
    client->write((formatDataTag("TP2", _meterData.phaseA.powerFactor) + "\n").c_str());
    client->write((formatDataTag("TP3", _meterData.phaseB.powerFactor) + "\n").c_str());
    client->write((formatDataTag("TP4", _meterData.phaseC.powerFactor) + "\n").c_str());
    client->write((formatDataTag("TP5", _meterData.phaseA.voltageTHDN) + "\n").c_str());
    client->write((formatDataTag("TP6", _meterData.phaseB.voltageTHDN) + "\n").c_str());
    client->write((formatDataTag("TP7", _meterData.phaseC.voltageTHDN) + "\n").c_str());
    client->write((formatDataTag("TP8", _meterData.phaseA.currentTHDN) + "\n").c_str());
    client->write((formatDataTag("TP9", _meterData.phaseB.currentTHDN) + "\n").c_str());
    client->write((formatDataTag("TP10", _meterData.phaseC.currentTHDN) + "\n").c_str());

    // Display data - Voltage and Current (DP1-DP3)
    client->write((formatDataTag("DP1", _meterData.phaseA.voltage) + "\n").c_str());
    client->write((formatDataTag("DP2", _meterData.phaseB.voltage) + "\n").c_str());
    client->write((formatDataTag("DP3", _meterData.phaseC.voltage) + "\n").c_str());

    // Metering Data (MD1-MD4)
    client->write((formatDataTag("MD1", _meterData.phaseA.current) + "\n").c_str());
    client->write((formatDataTag("MD2", _meterData.phaseB.current) + "\n").c_str());
    client->write((formatDataTag("MD3", _meterData.phaseC.current) + "\n").c_str());
    client->write((formatDataTag("MD4", _meterData.frequency) + "\n").c_str());

    // Status data (ST1-ST15) - Phase angles
    client->write((formatDataTag("ST1", _meterData.phaseA.phaseAngle) + "\n").c_str());
    client->write((formatDataTag("ST2", _meterData.phaseB.phaseAngle) + "\n").c_str());
    client->write((formatDataTag("ST3", _meterData.phaseC.phaseAngle) + "\n").c_str());
    client->write((formatDataTag("ST4", _meterData.phaseA.voltagePhaseAngle) + "\n").c_str());
    client->write((formatDataTag("ST5", _meterData.phaseB.voltagePhaseAngle) + "\n").c_str());
    client->write((formatDataTag("ST6", _meterData.phaseC.voltagePhaseAngle) + "\n").c_str());
    
    // ST7-ST15 reserved
    for (int i = 7; i <= 15; i++) {
        char tag[8];
        snprintf(tag, sizeof(tag), "ST%d", i);
        client->write((formatDataTag(tag, 0.0) + "\n").c_str());
    }
}

void TCPDataServer::sendCalibrationData(AsyncClient* client, uint8_t calType) {
    CalibrationConfig cal = ConfigManager::getInstance().getCalibrationConfig();
    
    DynamicJsonDocument doc(1024);
    doc["type"] = calType;
    
    switch (calType) {
        case 1:  // Basic calibration
            doc["lineFreq"] = cal.lineFreq;
            doc["pgaGain"] = cal.pgaGain;
            doc["mmode0"] = cal.mmode0;
            doc["mmode1"] = cal.mmode1;
            break;
            
        case 2:  // Voltage/Current gains
            doc["voltageGainA"] = cal.voltageGain[0];
            doc["voltageGainB"] = cal.voltageGain[1];
            doc["voltageGainC"] = cal.voltageGain[2];
            doc["currentGainA"] = cal.currentGain[0];
            doc["currentGainB"] = cal.currentGain[1];
            doc["currentGainC"] = cal.currentGain[2];
            doc["currentGainN"] = cal.currentGainN;
            break;
            
        case 3:  // Power offsets
            doc["pOffsetA"] = cal.pOffsetA;
            doc["qOffsetA"] = cal.qOffsetA;
            doc["pOffsetB"] = cal.pOffsetB;
            doc["qOffsetB"] = cal.qOffsetB;
            doc["pOffsetC"] = cal.pOffsetC;
            doc["qOffsetC"] = cal.qOffsetC;
            break;
            
        case 4:  // Advanced calibration
            doc["gainA"] = cal.gainA;
            doc["phiA"] = cal.phiA;
            doc["gainB"] = cal.gainB;
            doc["phiB"] = cal.phiB;
            doc["gainC"] = cal.gainC;
            doc["phiC"] = cal.phiC;
            break;
    }
    
    String jsonStr;
    serializeJson(doc, jsonStr);
    client->write((jsonStr + "\n").c_str());
}

void TCPDataServer::resetCalibration(uint8_t calType) {
    Logger::info("TCPDataServer", ("Resetting calibration type " + String(calType)).c_str());
    
    CalibrationConfig cal = ConfigManager::getInstance().getCalibrationConfig();
    CalibrationConfig defaultCal;  // Use default constructor values
    
    switch (calType) {
        case 1:
            cal.lineFreq = defaultCal.lineFreq;
            cal.pgaGain = defaultCal.pgaGain;
            cal.mmode0 = defaultCal.mmode0;
            cal.mmode1 = defaultCal.mmode1;
            break;
        case 2:
            for (int i = 0; i < 3; i++) {
                cal.voltageGain[i] = defaultCal.voltageGain[i];
                cal.currentGain[i] = defaultCal.currentGain[i];
            }
            cal.currentGainN = defaultCal.currentGainN;
            break;
        case 3:
            cal.pOffsetA = cal.qOffsetA = 0;
            cal.pOffsetB = cal.qOffsetB = 0;
            cal.pOffsetC = cal.qOffsetC = 0;
            break;
        case 4:
            cal.gainA = cal.phiA = 0;
            cal.gainB = cal.phiB = 0;
            cal.gainC = cal.phiC = 0;
            break;
    }
    
    ConfigManager::getInstance().setCalibrationConfig(cal);
}

void TCPDataServer::saveConfig() {
    Logger::info("TCPDataServer", "Saving configuration");
    ConfigManager::getInstance().save();
}

void TCPDataServer::resetConfig() {
    Logger::info("TCPDataServer", "Resetting configuration");
    ConfigManager::getInstance().restoreDefaults();
    ConfigManager::getInstance().save();
}

void TCPDataServer::reboot() {
    Logger::info("TCPDataServer", "Rebooting device");
    delay(500);
    ESP.restart();
}

String TCPDataServer::formatDataTag(const char* tag, float value) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%s:%.4f", tag, value);
    return String(buffer);
}

int TCPDataServer::findClientIndex(AsyncClient* client) {
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i] == client) {
            return i;
        }
    }
    return -1;
}

void TCPDataServer::removeClient(AsyncClient* client) {
    int index = findClientIndex(client);
    if (index >= 0) {
        _clients.erase(_clients.begin() + index);
        if (_clientCount > 0) {
            _clientCount--;
        }
    }
}
