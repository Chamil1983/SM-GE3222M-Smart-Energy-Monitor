/**
 * @file GlobalTypes.h
 * @brief Global data structures and type definitions
 * @author Microcode Engineering
 * @date 2026-02-08
 */

#ifndef GLOBALTYPES_H
#define GLOBALTYPES_H

#include <cstdint>
#include <Arduino.h>

// ==================== Phase Data Structure ====================

/**
 * @brief Single phase electrical measurements
 */
struct PhaseData {
    float voltage;              ///< RMS Voltage (V)
    float current;              ///< RMS Current (A)
    float activePower;          ///< Active Power (W)
    float reactivePower;        ///< Reactive Power (VAR)
    float apparentPower;        ///< Apparent Power (VA)
    float powerFactor;          ///< Power Factor (-1.0 to 1.0)
    float phaseAngle;           ///< Phase Angle (degrees)
    float voltageTHD;           ///< Voltage Total Harmonic Distortion (%)
    float currentTHD;           ///< Current Total Harmonic Distortion (%)
    float fundamentalPower;     ///< Fundamental Active Power (W)
    float harmonicPower;        ///< Harmonic Active Power (W)
    
    // Energy accumulators
    float forwardActiveEnergy;   ///< Forward Active Energy (Wh)
    float reverseActiveEnergy;   ///< Reverse Active Energy (Wh)
    float forwardReactiveEnergy; ///< Forward Reactive Energy (VARh)
    float reverseReactiveEnergy; ///< Reverse Reactive Energy (VARh)
    float apparentEnergy;        ///< Apparent Energy (VAh)
    
    /**
     * @brief Default constructor - initializes all values to zero
     */
    PhaseData() : voltage(0), current(0), activePower(0), reactivePower(0),
                  apparentPower(0), powerFactor(0), phaseAngle(0), 
                  voltageTHD(0), currentTHD(0), fundamentalPower(0),
                  harmonicPower(0), forwardActiveEnergy(0), reverseActiveEnergy(0),
                  forwardReactiveEnergy(0), reverseReactiveEnergy(0), apparentEnergy(0) {}
};

// ==================== Complete Meter Data ====================

/**
 * @brief Complete 3-phase meter measurements
 */
struct MeterData {
    PhaseData phaseA;           ///< Phase A measurements
    PhaseData phaseB;           ///< Phase B measurements
    PhaseData phaseC;           ///< Phase C measurements
    
    // Total/System measurements
    float totalActivePower;     ///< Total Active Power (W)
    float totalReactivePower;   ///< Total Reactive Power (VAR)
    float totalApparentPower;   ///< Total Apparent Power (VA)
    float totalPowerFactor;     ///< Total Power Factor
    
    float frequency;            ///< Line Frequency (Hz)
    float temperature;          ///< IC Temperature (°C)
    float neutralCurrentCalc;   ///< Calculated Neutral Current (A)
    float neutralCurrentSampled;///< Sampled Neutral Current (A)
    
    // Total energy accumulators
    float totalForwardActiveEnergy;   ///< Total Forward Active Energy (Wh)
    float totalReverseActiveEnergy;   ///< Total Reverse Active Energy (Wh)
    float totalForwardReactiveEnergy; ///< Total Forward Reactive Energy (VARh)
    float totalReverseReactiveEnergy; ///< Total Reverse Reactive Energy (VARh)
    float totalApparentEnergy;        ///< Total Apparent Energy (VAh)
    
    uint32_t timestamp;         ///< Unix timestamp of measurement
    bool valid;                 ///< Data validity flag
    
    /**
     * @brief Default constructor
     */
    MeterData() : totalActivePower(0), totalReactivePower(0), totalApparentPower(0),
                  totalPowerFactor(0), frequency(0), temperature(0), 
                  neutralCurrentCalc(0), neutralCurrentSampled(0),
                  totalForwardActiveEnergy(0), totalReverseActiveEnergy(0),
                  totalForwardReactiveEnergy(0), totalReverseReactiveEnergy(0),
                  totalApparentEnergy(0), timestamp(0), valid(false) {}
};

// ==================== Configuration Structures ====================

/**
 * @brief WiFi configuration
 */
struct WiFiConfig {
    char ssid[64];              ///< WiFi SSID
    char password[64];          ///< WiFi password
    char hostname[32];          ///< Device hostname
    bool useDHCP;               ///< Use DHCP (true) or static IP (false)
    IPAddress staticIP;         ///< Static IP address
    IPAddress gateway;          ///< Gateway address
    IPAddress subnet;           ///< Subnet mask
    IPAddress dns1;             ///< Primary DNS
    IPAddress dns2;             ///< Secondary DNS
    bool apMode;                ///< Enable AP mode fallback
    char apSSID[32];            ///< AP mode SSID
    char apPassword[64];        ///< AP mode password
    
    /**
     * @brief Default constructor
     */
    WiFiConfig() : useDHCP(true), apMode(true) {
        strcpy(ssid, "");
        strcpy(password, "");
        strcpy(hostname, "ge3222m");
        strcpy(apSSID, "GE3222M_Setup");
        strcpy(apPassword, "12345678");
        staticIP = IPAddress(192, 168, 1, 100);
        gateway = IPAddress(192, 168, 1, 1);
        subnet = IPAddress(255, 255, 255, 0);
        dns1 = IPAddress(8, 8, 8, 8);
        dns2 = IPAddress(8, 8, 4, 4);
    }
};

/**
 * @brief MQTT configuration
 */
struct MQTTConfig {
    bool enabled;               ///< Enable MQTT
    char server[128];           ///< MQTT broker address
    uint16_t port;              ///< MQTT broker port
    char username[64];          ///< MQTT username
    char password[64];          ///< MQTT password
    char topicPrefix[64];       ///< Topic prefix (e.g., "home/energy")
    uint16_t publishInterval;   ///< Publish interval (seconds)
    bool useTLS;                ///< Use TLS encryption
    
    /**
     * @brief Default constructor
     */
    MQTTConfig() : enabled(false), port(1883), publishInterval(60), useTLS(false) {
        strcpy(server, "");
        strcpy(username, "");
        strcpy(password, "");
        strcpy(topicPrefix, "energy/ge3222m");
    }
};

/**
 * @brief Modbus configuration
 */
struct ModbusConfig {
    bool rtuEnabled;            ///< Enable Modbus RTU
    bool tcpEnabled;            ///< Enable Modbus TCP
    uint32_t baudRate;          ///< RTU baud rate
    uint8_t slaveId;            ///< Modbus slave ID
    char parity;                ///< Parity: 'N', 'E', 'O'
    uint16_t tcpPort;           ///< TCP port for Modbus TCP
    
    /**
     * @brief Default constructor
     */
    ModbusConfig() : rtuEnabled(true), tcpEnabled(false), 
                     baudRate(9600), slaveId(1), parity('N'), tcpPort(502) {}
};

/**
 * @brief Calibration configuration
 */
struct CalibrationConfig {
    uint16_t lineFreq;          ///< Line frequency (50/60 Hz)
    uint16_t pgaGain;           ///< PGA Gain setting
    
    // Voltage and current gains (per phase + neutral)
    uint16_t voltageGain[3];    ///< Voltage gain [A, B, C]
    uint16_t currentGain[4];    ///< Current gain [A, B, C, N]
    
    // Calibration register values
    uint16_t calRegisters[13];  ///< Main calibration registers
    uint16_t harCalRegisters[7];///< Harmonic calibration registers
    uint16_t measurCalRegisters[15]; ///< Measurement calibration registers
    
    /**
     * @brief Default constructor - sets factory defaults
     */
    CalibrationConfig() : lineFreq(60), pgaGain(0x015) {
        // Factory default gains
        for (int i = 0; i < 3; i++) voltageGain[i] = 0x7530;
        for (int i = 0; i < 4; i++) currentGain[i] = 0x7530;
        
        // Zero out calibration registers
        for (int i = 0; i < 13; i++) calRegisters[i] = 0;
        for (int i = 0; i < 7; i++) harCalRegisters[i] = 0;
        for (int i = 0; i < 15; i++) measurCalRegisters[i] = 0;
    }
};

// ==================== System Status ====================

/**
 * @brief System status information
 */
struct SystemStatus {
    uint32_t uptime;            ///< Uptime in seconds
    uint32_t freeHeap;          ///< Free heap memory (bytes)
    uint32_t minFreeHeap;       ///< Minimum free heap since boot
    float cpuTemperature;       ///< ESP32 temperature (°C)
    int8_t wifiRSSI;            ///< WiFi signal strength (dBm)
    uint8_t tcpClientCount;     ///< Number of connected TCP clients
    uint16_t errorCount;        ///< Total error count since boot
    uint16_t spiErrorCount;     ///< SPI communication errors
    bool atmStatus;             ///< ATM90E36 status (true=OK)
    
    /**
     * @brief Default constructor
     */
    SystemStatus() : uptime(0), freeHeap(0), minFreeHeap(0), cpuTemperature(0),
                     wifiRSSI(0), tcpClientCount(0), errorCount(0), 
                     spiErrorCount(0), atmStatus(false) {}
};

#endif // GLOBALTYPES_H
