#pragma once

// SM-GE3222M V2.0 - Core Data Types
// Shared data structures across all modules

#include <cstdint>
#include <cstring>

// ============================================================================
// LOG LEVEL ENUMERATION
// ============================================================================
enum class LogLevel : uint8_t {
    ERROR = 0,
    WARN  = 1,
    INFO  = 2,
    DEBUG = 3,
    TRACE = 4
};

// ============================================================================
// ERROR CODE ENUMERATION
// ============================================================================
enum class ErrorCode : uint16_t {
    NONE = 0,
    
    // ATM90E36 Errors (1-19)
    ATM_INIT_FAILED         = 1,
    ATM_CHECKSUM_FAILED     = 2,
    ATM_SPI_COMM_ERROR      = 3,
    ATM_CALIBRATION_ERROR   = 4,
    ATM_READ_TIMEOUT        = 5,
    
    // SPI Bus Errors (20-29)
    SPI_BUS_FAULT           = 20,
    SPI_MUTEX_TIMEOUT       = 21,
    SPI_INIT_FAILED         = 22,
    
    // I2C Bus Errors (30-39)
    I2C_BUS_FAULT           = 30,
    I2C_MUTEX_TIMEOUT       = 31,
    I2C_INIT_FAILED         = 32,
    I2C_DEVICE_NOT_FOUND    = 33,
    
    // WiFi/Network Errors (40-59)
    WIFI_CONNECT_FAILED     = 40,
    WIFI_TIMEOUT            = 41,
    WIFI_LOST_CONNECTION    = 42,
    NETWORK_INIT_FAILED     = 43,
    MQTT_CONNECT_FAILED     = 44,
    NTP_SYNC_FAILED         = 45,
    
    // Storage Errors (60-79)
    NVS_INIT_FAILED         = 60,
    NVS_READ_ERROR          = 61,
    NVS_WRITE_ERROR         = 62,
    SPIFFS_MOUNT_FAILED     = 63,
    SPIFFS_READ_ERROR       = 64,
    SPIFFS_WRITE_ERROR      = 65,
    CONFIG_CORRUPTED        = 66,
    
    // Task/System Errors (80-99)
    TASK_CREATE_FAILED      = 80,
    TASK_STACK_OVERFLOW     = 81,
    HEAP_EXHAUSTED          = 82,
    WATCHDOG_TRIGGERED      = 83,
    SYSTEM_PANIC            = 84,
    
    // Communication Errors (100-119)
    TCP_SERVER_ERROR        = 100,
    MODBUS_INIT_ERROR       = 101,
    MODBUS_COMM_ERROR       = 102,
    WEBSOCKET_ERROR         = 103,
    OTA_FAILED              = 104,
    
    // General (200+)
    UNKNOWN_ERROR           = 255
};

// ============================================================================
// PHASE DATA STRUCTURE
// ============================================================================
struct PhaseData {
    // RMS Values
    float voltageRMS;           // Voltage RMS (V)
    float currentRMS;           // Current RMS (A)
    
    // Power
    float activePower;          // Active Power (W)
    float reactivePower;        // Reactive Power (VAR)
    float apparentPower;        // Apparent Power (VA)
    float powerFactor;          // Power Factor (0.0 to 1.0)
    
    // Phase Angles
    float meanPhaseAngle;       // Mean Phase Angle (degrees)
    float voltagePhaseAngle;    // Voltage Phase Angle (degrees)
    
    // THD
    float voltageTHDN;          // Voltage THD (%)
    float currentTHDN;          // Current THD (%)
    
    // Fundamental/Harmonic Power
    float fundamentalPower;     // Fundamental Power (W)
    float harmonicPower;        // Harmonic Power (W)
    
    // Energy Accumulators
    float fwdActiveEnergy;      // Forward Active Energy (kWh)
    float revActiveEnergy;      // Reverse Active Energy (kWh)
    float fwdReactiveEnergy;    // Forward Reactive Energy (kVARh)
    float revReactiveEnergy;    // Reverse Reactive Energy (kVARh)
    float apparentEnergy;       // Apparent Energy (kVAh)
    
    // Initialize with zeros
    PhaseData() {
        memset(this, 0, sizeof(PhaseData));
    }
};

// ============================================================================
// METER DATA STRUCTURE (Complete Snapshot)
// ============================================================================
struct MeterData {
    // Three Phases
    PhaseData phaseA;
    PhaseData phaseB;
    PhaseData phaseC;
    
    // Totals
    float totalActivePower;     // Total Active Power (W)
    float totalReactivePower;   // Total Reactive Power (VAR)
    float totalApparentPower;   // Total Apparent Power (VA)
    float totalPowerFactor;     // Total Power Factor
    float totalFwdActiveEnergy; // Total Forward Active Energy (kWh)
    float totalRevActiveEnergy; // Total Reverse Active Energy (kWh)
    float totalFwdReactiveEnergy; // Total Forward Reactive Energy (kVARh)
    float totalRevReactiveEnergy; // Total Reverse Reactive Energy (kVARh)
    float totalApparentEnergy;  // Total Apparent Energy (kVAh)
    
    // Neutral & Frequency
    float neutralCurrent;       // Neutral Current (A)
    float frequency;            // Line Frequency (Hz)
    
    // Board Data
    float boardTemperature;     // PCB Temperature (°C)
    float ambientTemperature;   // Ambient Temperature (°C)
    float ambientHumidity;      // Ambient Humidity (%)
    
    // Status
    uint16_t meteringStatus0;   // ATM90E36 Status Register 0
    uint16_t meteringStatus1;   // ATM90E36 Status Register 1
    uint32_t timestamp;         // Unix timestamp (seconds)
    uint32_t sequenceNumber;    // Incrementing sequence number
    bool     valid;             // Data validity flag
    
    // Initialize with zeros
    MeterData() {
        memset(this, 0, sizeof(MeterData));
    }
};

// ============================================================================
// CALIBRATION CONFIGURATION STRUCTURE
// ============================================================================
struct CalibrationConfig {
    // Configuration Registers
    uint16_t lineFreq;          // 0=50Hz, 1=60Hz
    uint16_t pgaGain;           // PGA Gain (0=1x, 1=4x, 2=8x, 3=16x)
    uint16_t pmpga;             // Partial Measurement PGA
    
    // Thresholds
    uint16_t pStartTh;          // Active Power Startup Threshold
    uint16_t qStartTh;          // Reactive Power Startup Threshold
    uint16_t sStartTh;          // Apparent Power Startup Threshold
    uint16_t pPhaseTh;          // Active Power Phase Threshold
    uint16_t qPhaseTh;          // Reactive Power Phase Threshold
    uint16_t sPhaseTh;          // Apparent Power Phase Threshold
    
    // Calibration Registers (Phase A, B, C)
    // Format: [A_POffset, A_QOffset, A_Gain, A_Phi, B_POffset, B_QOffset, B_Gain, B_Phi, C_POffset, C_QOffset, C_Gain, C_Phi, CSOne]
    uint16_t calRegs[13];       // Power calibration registers
    
    // Harmonic Calibration
    // Format: [AF_POffset, BF_POffset, CF_POffset, AF_PGain, BF_PGain, CF_PGain, CSTwo]
    uint16_t harCalRegs[7];     // Harmonic calibration registers
    
    // Measurement Calibration
    // Format: [A_UGain, A_IGain, A_UOffset, A_IOffset, B_UGain, B_IGain, B_UOffset, B_IOffset, C_UGain, C_IGain, C_UOffset, C_IOffset, N_IGain, N_IOffset, CSThree]
    uint16_t measCalRegs[15];   // Measurement calibration registers
    
    // Initialize with default values
    CalibrationConfig() {
        lineFreq = 0;  // 50Hz
        pgaGain = 0;   // 1x
        pmpga = 0;
        pStartTh = 0x08BD;
        qStartTh = 0x0AEC;
        sStartTh = 0x0AEC;
        pPhaseTh = 0x00D8;
        qPhaseTh = 0x00D8;
        sPhaseTh = 0x00D8;
        
        memset(calRegs, 0, sizeof(calRegs));
        memset(harCalRegs, 0, sizeof(harCalRegs));
        memset(measCalRegs, 0, sizeof(measCalRegs));
    }
};

// ============================================================================
// WIFI CONFIGURATION STRUCTURE
// ============================================================================
struct WiFiConfig {
    bool    enabled;
    bool    apMode;             // true=AP mode, false=STA mode
    char    ssid[33];           // SSID (max 32 chars + null)
    char    password[64];       // Password (max 63 chars + null)
    char    hostname[33];       // Hostname
    
    // Static IP (if not DHCP)
    bool    useDHCP;
    uint8_t staticIP[4];        // Static IP address
    uint8_t gateway[4];         // Gateway address
    uint8_t subnet[4];          // Subnet mask
    uint8_t dns1[4];            // Primary DNS
    uint8_t dns2[4];            // Secondary DNS
    
    // AP Mode settings
    char    apSSID[33];
    char    apPassword[64];
    uint8_t apChannel;
    
    WiFiConfig() {
        enabled = true;
        apMode = false;
        useDHCP = true;
        strcpy(ssid, "");
        strcpy(password, "");
        strcpy(hostname, "ge3222m");
        strcpy(apSSID, "SM-GE3222M-Setup");
        strcpy(apPassword, "12345678");
        apChannel = 1;
        memset(staticIP, 0, sizeof(staticIP));
        memset(gateway, 0, sizeof(gateway));
        memset(subnet, 0, sizeof(subnet));
        memset(dns1, 0, sizeof(dns1));
        memset(dns2, 0, sizeof(dns2));
    }
};

// ============================================================================
// MODBUS CONFIGURATION STRUCTURE
// ============================================================================
struct ModbusConfig {
    bool     rtuEnabled;
    bool     tcpEnabled;
    uint8_t  slaveID;
    uint32_t baudrate;
    uint8_t  dataBits;
    uint8_t  stopBits;
    char     parity;            // 'N', 'E', 'O'
    uint16_t tcpPort;
    
    ModbusConfig() {
        rtuEnabled = true;
        tcpEnabled = true;
        slaveID = 1;
        baudrate = 9600;
        dataBits = 8;
        stopBits = 1;
        parity = 'N';
        tcpPort = 502;
    }
};

// ============================================================================
// MQTT CONFIGURATION STRUCTURE
// ============================================================================
struct MQTTConfig {
    bool    enabled;
    char    broker[65];         // MQTT broker hostname/IP
    uint16_t port;              // MQTT broker port
    char    username[33];
    char    password[65];
    char    clientID[33];
    char    baseTopic[65];      // Base topic (e.g., "ge3222m/device1")
    uint16_t publishInterval;   // Publish interval (seconds)
    bool    useHomeAssistant;   // Enable Home Assistant discovery
    bool    useTLS;             // Enable TLS/SSL
    
    MQTT Config() {
        enabled = false;
        port = 1883;
        publishInterval = 10;
        useHomeAssistant = true;
        useTLS = false;
        strcpy(broker, "");
        strcpy(username, "");
        strcpy(password, "");
        strcpy(clientID, "ge3222m");
        strcpy(baseTopic, "ge3222m");
    }
};

// ============================================================================
// NETWORK CONFIGURATION STRUCTURE
// ============================================================================
struct NetworkConfig {
    char    hostname[33];
    bool    mdnsEnabled;
    char    mdnsName[33];
    bool    ntpEnabled;
    char    ntpServer[65];
    int16_t timezoneOffset;     // Timezone offset in minutes
    
    NetworkConfig() {
        strcpy(hostname, "ge3222m");
        mdnsEnabled = true;
        strcpy(mdnsName, "ge3222m");
        ntpEnabled = true;
        strcpy(ntpServer, "pool.ntp.org");
        timezoneOffset = 0;
    }
};

// ============================================================================
// SYSTEM CONFIGURATION STRUCTURE
// ============================================================================
struct SystemConfig {
    uint16_t readInterval;      // Energy reading interval (ms)
    uint16_t publishInterval;   // Data publish interval (seconds)
    bool     webServerEnabled;
    uint16_t webServerPort;
    bool     otaEnabled;
    char     otaPassword[33];
    LogLevel logLevel;
    bool     watchdogEnabled;
    uint16_t watchdogTimeout;   // Watchdog timeout (seconds)
    
    SystemConfig() {
        readInterval = 500;
        publishInterval = 1;
        webServerEnabled = true;
        webServerPort = 80;
        otaEnabled = true;
        strcpy(otaPassword, "admin");
        logLevel = LogLevel::INFO;
        watchdogEnabled = true;
        watchdogTimeout = 30;
    }
};

// ============================================================================
// EVENT TYPES ENUMERATION
// ============================================================================
enum class EventType : uint8_t {
    METER_DATA_UPDATED = 0,
    CONFIG_CHANGED,
    CALIBRATION_APPLIED,
    ERROR_OCCURRED,
    WIFI_CONNECTED,
    WIFI_DISCONNECTED,
    MQTT_CONNECTED,
    MQTT_DISCONNECTED,
    OTA_STARTED,
    OTA_COMPLETED,
    SYSTEM_REBOOT
};

// ============================================================================
// LOG ENTRY STRUCTURE
// ============================================================================
struct LogEntry {
    uint32_t timestamp;
    LogLevel level;
    char     module[16];
    char     message[128];
    
    LogEntry() {
        timestamp = 0;
        level = LogLevel::INFO;
        memset(module, 0, sizeof(module));
        memset(message, 0, sizeof(message));
    }
};

// ============================================================================
// SYSTEM STATUS STRUCTURE
// ============================================================================
struct SystemStatus {
    uint32_t uptime;            // Uptime in seconds
    uint32_t freeHeap;          // Free heap memory (bytes)
    uint32_t minFreeHeap;       // Minimum free heap seen (bytes)
    uint8_t  cpuFreqMHz;        // CPU frequency (MHz)
    float    cpuTemperature;    // CPU temperature (°C)
    uint16_t errorCount;        // Total error count
    ErrorCode lastError;        // Last error code
    uint32_t bootCount;         // Number of boots
    bool     wifiConnected;
    bool     mqttConnected;
    bool     modbusActive;
    
    SystemStatus() {
        memset(this, 0, sizeof(SystemStatus));
    }
};
