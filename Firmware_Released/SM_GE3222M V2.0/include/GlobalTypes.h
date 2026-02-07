/**
 * @file GlobalTypes.h
 * @brief Centralized type definitions, structures, and enums
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * All global types used across the firmware
 */

#pragma once

#include <Arduino.h>

// ============================================================
// System States
// ============================================================

enum class SystemState {
    BOOTING,
    INITIALIZING,
    RUNNING,
    ERROR,
    MAINTENANCE
};

// ============================================================
// Error Codes
// ============================================================

enum class ErrorCode {
    NONE = 0,
    SPI_COMM_FAIL,
    SPI_INIT_FAILED,
    WIFI_DISCONNECTED,
    NVS_CORRUPT,
    ATM_INIT_FAIL,
    MODBUS_TIMEOUT,
    SPIFFS_MOUNT_FAIL,
    I2C_DEVICE_MISSING,
    HEAP_LOW,
    WATCHDOG_RESET,
    CALIBRATION_ERROR,
    MQTT_DISCONNECTED,
    ETHERNET_LINK_DOWN,
    ETHERNET_INIT_FAILED,
    ETHERNET_DHCP_FAILED,
    LCD_INIT_FAIL,
    MCP23017_INIT_FAIL,
    CONFIG_LOAD_FAIL,
    CONFIG_SAVE_FAIL,
    ENERGY_READ_FAIL,
    DHT_READ_FAIL
};

// ============================================================
// Log Levels
// ============================================================

enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
};

// ============================================================
// Phase Data Structure
// ============================================================

struct PhaseData {
    float voltage;              // RMS Voltage (V)
    float current;              // RMS Current (A)
    float activePower;          // Active Power (W)
    float reactivePower;        // Reactive Power (VAR)
    float apparentPower;        // Apparent Power (VA)
    float powerFactor;          // Power Factor
    float phaseAngle;           // Phase Angle (degrees)
    float voltagePhaseAngle;    // Voltage Phase Angle (degrees)
    float voltageTHDN;          // Voltage THD+N (%)
    float currentTHDN;          // Current THD+N (%)
    
    // Constructor
    PhaseData() : voltage(0), current(0), activePower(0), reactivePower(0),
                  apparentPower(0), powerFactor(0), phaseAngle(0),
                  voltagePhaseAngle(0), voltageTHDN(0), currentTHDN(0) {}
};

// ============================================================
// Complete Meter Data Structure
// ============================================================

struct MeterData {
    // Per-phase data
    PhaseData phaseA;
    PhaseData phaseB;
    PhaseData phaseC;
    
    // Totals
    float totalActivePower;        // Total Active Power (W)
    float totalReactivePower;      // Total Reactive Power (VAR)
    float totalApparentPower;      // Total Apparent Power (VA)
    float totalPowerFactor;        // Total Power Factor
    
    // System parameters
    float frequency;               // Line Frequency (Hz)
    float temperature;             // Chip Temperature (°C)
    float neutralCurrentCalc;      // Calculated Neutral Current (A)
    float neutralCurrentSampled;   // Sampled Neutral Current (A)
    
    // Energy accumulators [Total, A, B, C]
    float importActiveEnergy[4];   // Import Active Energy (kWh)
    float exportActiveEnergy[4];   // Export Active Energy (kWh)
    float importReactiveEnergy[4]; // Import Reactive Energy (kVARh)
    float exportReactiveEnergy[4]; // Export Reactive Energy (kVARh)
    float apparentEnergy[4];       // Apparent Energy (kVAh)
    
    // Fundamental/Harmonic Power
    float totalActiveFundPower;    // Total Active Fundamental Power (W)
    float totalActiveHarPower;     // Total Active Harmonic Power (W)
    
    // Timestamp
    unsigned long readTimestamp;   // millis() when data was read
    
    // Constructor
    MeterData() : totalActivePower(0), totalReactivePower(0), totalApparentPower(0),
                  totalPowerFactor(0), frequency(0), temperature(0),
                  neutralCurrentCalc(0), neutralCurrentSampled(0),
                  totalActiveFundPower(0), totalActiveHarPower(0), readTimestamp(0) {
        // Initialize energy arrays
        for (int i = 0; i < 4; i++) {
            importActiveEnergy[i] = 0;
            exportActiveEnergy[i] = 0;
            importReactiveEnergy[i] = 0;
            exportReactiveEnergy[i] = 0;
            apparentEnergy[i] = 0;
        }
    }
};

// ============================================================
// Calibration Configuration Structure
// ============================================================

struct CalibrationConfig {
    // Basic configuration
    uint16_t lineFreq;             // Line frequency register value
    uint16_t pgaGain;              // PGA gain configuration
    
    // Metering mode
    uint16_t mmode0;               // Metering mode 0
    uint16_t mmode1;               // Metering mode 1
    
    // Voltage gains
    uint16_t voltageGain[3];       // Voltage gain for phases A, B, C
    
    // Current gains
    uint16_t currentGain[3];       // Current gain for phases A, B, C
    uint16_t currentGainN;         // Neutral current gain
    
    // Power calibration offsets
    uint16_t pOffsetA;             // Phase A active power offset
    uint16_t qOffsetA;             // Phase A reactive power offset
    uint16_t pOffsetB;             // Phase B active power offset
    uint16_t qOffsetB;             // Phase B reactive power offset
    uint16_t pOffsetC;             // Phase C active power offset
    uint16_t qOffsetC;             // Phase C reactive power offset
    
    // Calibration gains and angles
    uint16_t gainA;                // Phase A calibration gain
    uint16_t phiA;                 // Phase A calibration angle
    uint16_t gainB;                // Phase B calibration gain
    uint16_t phiB;                 // Phase B calibration angle
    uint16_t gainC;                // Phase C calibration gain
    uint16_t phiC;                 // Phase C calibration angle
    
    // Fundamental power offsets
    uint16_t pOffsetAF;            // Phase A fundamental active power offset
    uint16_t pOffsetBF;            // Phase B fundamental active power offset
    uint16_t pOffsetCF;            // Phase C fundamental active power offset
    
    // Fundamental power gains
    uint16_t pGainAF;              // Phase A fundamental active power gain
    uint16_t pGainBF;              // Phase B fundamental active power gain
    uint16_t pGainCF;              // Phase C fundamental active power gain
    
    // Measurement calibration
    uint16_t uOffsetA;             // Phase A voltage offset
    uint16_t iOffsetA;             // Phase A current offset
    uint16_t uOffsetB;             // Phase B voltage offset
    uint16_t iOffsetB;             // Phase B current offset
    uint16_t uOffsetC;             // Phase C voltage offset
    uint16_t iOffsetC;             // Phase C current offset
    uint16_t iOffsetN;             // Neutral current offset
    
    // Checksums
    uint16_t csZero;               // Checksum 0
    uint16_t csOne;                // Checksum 1
    uint16_t csTwo;                // Checksum 2
    uint16_t csThree;              // Checksum 3
    
    // Power line constant
    uint16_t plConstH;             // PL constant high word
    uint16_t plConstL;             // PL constant low word
    
    // Startup thresholds
    uint16_t pStartTh;             // Active power startup threshold
    uint16_t qStartTh;             // Reactive power startup threshold
    uint16_t sStartTh;             // Apparent power startup threshold
    uint16_t pPhaseTh;             // Active power accumulation threshold
    uint16_t qPhaseTh;             // Reactive power accumulation threshold
    uint16_t sPhaseTh;             // Apparent power accumulation threshold
    
    // Constructor with default values
    CalibrationConfig() : lineFreq(389), pgaGain(0x5555), mmode0(0), mmode1(0),
                          currentGainN(33500),
                          pOffsetA(0), qOffsetA(0), pOffsetB(0), qOffsetB(0), pOffsetC(0), qOffsetC(0),
                          gainA(0), phiA(0), gainB(0), phiB(0), gainC(0), phiC(0),
                          pOffsetAF(0), pOffsetBF(0), pOffsetCF(0),
                          pGainAF(0), pGainBF(0), pGainCF(0),
                          uOffsetA(0), iOffsetA(0), uOffsetB(0), iOffsetB(0), uOffsetC(0), iOffsetC(0), iOffsetN(0),
                          csZero(0), csOne(0), csTwo(0), csThree(0),
                          plConstH(0), plConstL(0),
                          pStartTh(0), qStartTh(0), sStartTh(0),
                          pPhaseTh(0), qPhaseTh(0), sPhaseTh(0) {
        // Initialize arrays with default values
        voltageGain[0] = 7143;
        voltageGain[1] = 7171;
        voltageGain[2] = 7180;
        currentGain[0] = 45578;
        currentGain[1] = 45578;
        currentGain[2] = 45578;
    }
};

// ============================================================
// WiFi Configuration
// ============================================================

struct WiFiConfig {
    char ssid[32];
    char password[64];
    char hostname[32];
    bool useDHCP;
    char staticIP[16];
    char gateway[16];
    char subnet[16];
    char dns1[16];
    char dns2[16];
    bool apMode;
    char apSSID[32];
    char apPassword[64];
    
    WiFiConfig() : useDHCP(true), apMode(false) {
        strcpy(ssid, "");
        strcpy(password, "");
        strcpy(hostname, "SM-GE3222M");
        strcpy(staticIP, "192.168.1.100");
        strcpy(gateway, "192.168.1.1");
        strcpy(subnet, "255.255.255.0");
        strcpy(dns1, "8.8.8.8");
        strcpy(dns2, "8.8.4.4");
        strcpy(apSSID, "SM-GE3222M-AP");
        strcpy(apPassword, "12345678");
    }
};

// ============================================================
// MQTT Configuration
// ============================================================

struct MQTTConfig {
    bool enabled;
    char server[64];
    uint16_t port;
    char user[32];
    char password[64];
    char topicPrefix[32];
    uint32_t publishInterval;
    bool useTLS;
    
    MQTTConfig() : enabled(false), port(1883), publishInterval(5000), useTLS(false) {
        strcpy(server, "");
        strcpy(user, "");
        strcpy(password, "");
        strcpy(topicPrefix, "smartmeter");
    }
};

// ============================================================
// Modbus Configuration
// ============================================================

struct ModbusConfig {
    bool rtuEnabled;
    uint8_t rtuSlaveID;
    uint32_t rtuBaudRate;
    uint8_t rtuParity;  // 0=None, 1=Even, 2=Odd
    
    bool tcpEnabled;
    uint16_t tcpPort;
    uint8_t tcpMaxConnections;
    
    ModbusConfig() : rtuEnabled(true), rtuSlaveID(1), rtuBaudRate(9600), rtuParity(0),
                     tcpEnabled(false), tcpPort(502), tcpMaxConnections(4) {}
};

// ============================================================
// Task Priorities
// ============================================================

enum class TaskPriority {
    PRIORITY_CRITICAL = 0,   // Highest priority
    PRIORITY_HIGH = 1,
    PRIORITY_MEDIUM = 2,
    PRIORITY_LOW = 3,
    PRIORITY_IDLE = 4        // Lowest priority
};

// ============================================================
// Task Structure
// ============================================================

struct Task {
    const char* name;
    void (*callback)();
    unsigned long interval;
    unsigned long lastRun;
    TaskPriority priority;
    bool enabled;
    unsigned long executionTime;
    
    Task() : name(""), callback(nullptr), interval(0), lastRun(0),
             priority(TaskPriority::PRIORITY_MEDIUM), enabled(false), executionTime(0) {}
};

// ============================================================
// Health Monitor Data
// ============================================================

struct HealthData {
    uint32_t freeHeap;
    uint32_t minFreeHeap;
    uint32_t uptime;
    uint32_t restartCount;
    float cpuLoad;
    uint32_t taskOverruns;
    int wifiRSSI;
    bool wifiConnected;
    bool mqttConnected;
    bool ethernetLinked;
    uint32_t spiErrors;
    uint32_t i2cErrors;
    
    HealthData() : freeHeap(0), minFreeHeap(0xFFFFFFFF), uptime(0), restartCount(0),
                   cpuLoad(0), taskOverruns(0), wifiRSSI(0), wifiConnected(false),
                   mqttConnected(false), ethernetLinked(false), spiErrors(0), i2cErrors(0) {}
};

// ============================================================
// Power Quality Event
// ============================================================

struct PowerQualityEvent {
    enum class Type {
        VOLTAGE_SAG,
        VOLTAGE_SWELL,
        PHASE_IMBALANCE,
        THD_ALARM,
        FREQUENCY_OUT_OF_RANGE
    };
    
    Type type;
    uint8_t phase;  // 0=All, 1=A, 2=B, 3=C
    float value;
    unsigned long timestamp;
    
    PowerQualityEvent() : type(Type::VOLTAGE_SAG), phase(0), value(0), timestamp(0) {}
};
