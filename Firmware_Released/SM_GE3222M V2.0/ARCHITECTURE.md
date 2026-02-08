# SM-GE3222M Firmware V2.0 Architecture

## Overview

The V2.0 firmware represents a complete modernization from the monolithic V1.0 architecture to a layered, modular, production-grade embedded system. It maintains full backward compatibility with the V1.0 TCP protocol while adding modern features like MQTT, REST API, and improved diagnostics.

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        Application Layer                        │
│  ┌─────────────┐  ┌──────────────┐  ┌────────────────────────┐ │
│  │ main.cpp    │  │ TaskManager  │  │ WatchdogManager       │ │
│  │ (Setup/Loop)│  │ (FreeRTOS)   │  │ (HW/SW Watchdog)      │ │
│  └─────────────┘  └──────────────┘  └────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                       Service Layer                             │
│  ┌───────────────┐  ┌────────────────┐  ┌───────────────────┐ │
│  │ EnergyMeter   │  │ NetworkManager │  │ ConfigManager     │ │
│  │ Accumulator   │  │ WiFi/Ethernet  │  │ (JSON/SPIFFS)     │ │
│  └───────────────┘  └────────────────┘  └───────────────────┘ │
│  ┌───────────────────────────────────────────────────────────┐ │
│  │ SystemMonitor │ Logger │ DiagnosticReport                 │ │
│  └───────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                    Communication Layer                          │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────────────┐  │
│  │ TCPDataServer│  │ WebServer    │  │ ModbusServer        │  │
│  │ (Port 8088)  │  │ (Port 80)    │  │ (RTU/TCP Port 502)  │  │
│  └──────────────┘  └──────────────┘  └─────────────────────┘  │
│  ┌────────────────────────────────────────────────────────────┐│
│  │ MQTTPublisher (Configurable Broker/Topic)                  ││
│  └────────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                     Hardware Abstraction Layer                  │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────────────┐  │
│  │ SPIBus       │  │ I2CBus       │  │ GPIOManager         │  │
│  │ (ATM90E36)   │  │ (EEPROM/LCD) │  │ (LED/Button/Relay)  │  │
│  └──────────────┘  └──────────────┘  └─────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                         Driver Layer                            │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │ ATM90E36Driver (SPI Communication, Register Programming)   │ │
│  └────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
```

---

## Boot Sequence

### Phase 1: Core Initialization (0-2 seconds)
```cpp
1. Serial.begin(115200)
2. Logger.init()            // Initialize logging subsystem
3. ErrorHandler.init()      // Initialize error tracking
4. WatchdogManager.init(30) // 30-second HW+SW watchdog
5. GPIOManager.init()       // Initialize GPIO, LED Red ON
```

### Phase 2: Storage Initialization (2-4 seconds)
```cpp
6. SPIBus.init()            // Initialize SPI with mutex
7. I2CBus.init(21, 22)      // Initialize I2C (SDA=21, SCL=22)
8. SPIFFSManager.init()     // Mount SPIFFS filesystem
9. ConfigManager.init()     // Load config.json or create defaults
```

### Phase 3: Energy IC Initialization (4-7 seconds)
```cpp
10. ATM90E36Driver.init(calibrationConfig)
    - Load calibration from ConfigManager
    - Write ConfigStart (0x30 = 0x5678)
    - Program PLconstH/L, MMode0/1, thresholds
    - Write CalStart (0x40 = 0x5678)
    - Program offset/gain/phi per phase
    - Write HarmStart (0x50 = 0x5678)
    - Program harmonic calibration
    - Write AdjStart (0x60 = 0x5678)
    - Program measurement gains/offsets
    - Verify checksums (CS0/CS1/CS2/CS3)
    - Lock registers
11. EnergyMeter.init(filterWindow=5)
12. EnergyAccumulator.init(persistInterval=60)
```

### Phase 4: Network Initialization (7-22 seconds)
```cpp
13. NetworkManager.init()
    - WiFi STA mode (15s timeout)
    - On failure: AP mode "GE3222M_Setup"
    - mDNS "ge3222m.local"
    - (Optional) Ethernet W5500 initialization
```

### Phase 5: Communication Services (22-25 seconds)
```cpp
14. TCPDataServer.begin(8088)     // V1.0 protocol compatible
15. WebServer.begin(80)           // REST API + WebSocket
16. ModbusServer.begin()          // RTU Serial2 + TCP port 502
17. MQTTPublisher.begin()         // If enabled in config
18. ArduinoOTA.begin()            // OTA updates
```

### Phase 6: Task Creation (25-27 seconds)
```cpp
19. TaskManager.createAllTasks()
    - EnergyTask (Core 1, 8192 stack, priority 5)
    - NetworkTask (Core 0, 8192 stack, priority 3)
    - DiagTask (Core 1, 4096 stack, priority 1)
    - OTATask (Core 0, 4096 stack, priority 2)
20. LED Green ON
21. Log "V2.0 initialized"
```

**Total Boot Time:** ~27 seconds (includes 15s WiFi connection timeout)

---

## FreeRTOS Task Architecture

### Task Overview

| Task | Core | Stack | Priority | Period | Purpose |
|------|------|-------|----------|--------|---------|
| **EnergyTask** | 1 | 8192 | 5 (High) | 1000ms | Read ATM90E36, update accumulators |
| **NetworkTask** | 0 | 8192 | 3 (Med) | 10ms | Handle TCP/Web/Modbus/MQTT |
| **DiagTask** | 1 | 4096 | 1 (Low) | 5000ms | System health monitoring, watchdog feed |
| **OTATask** | 0 | 4096 | 2 (Med-Low) | 100ms | OTA update handling |

### Task Details

#### EnergyTask (Core 1, Priority 5)
```cpp
void energyTask(void* param) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(1000);
    
    while(1) {
        // Read all measurements from ATM90E36
        MeterData data;
        if (energyMeter.readAll(data)) {
            // Update persistent energy accumulators
            energyAccumulator.update(data);
            
            // Write to shared buffer with mutex protection
            if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(100))) {
                memcpy(&sharedMeterData, &data, sizeof(MeterData));
                xSemaphoreGive(dataMutex);
            }
        }
        
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
```

#### NetworkTask (Core 0, Priority 3)
```cpp
void networkTask(void* param) {
    const TickType_t xPeriod = pdMS_TO_TICKS(10);
    
    while(1) {
        networkManager.maintain();      // WiFi keep-alive, reconnect
        tcpServer.update();             // Process TCP clients
        webServer.handleClient();       // Process web requests
        modbusServer.update();          // Process Modbus requests
        mqttPublisher.update();         // Publish MQTT if interval elapsed
        ArduinoOTA.handle();            // Handle OTA updates
        
        vTaskDelay(xPeriod);
    }
}
```

#### DiagTask (Core 1, Priority 1)
```cpp
void diagTask(void* param) {
    const TickType_t xPeriod = pdMS_TO_TICKS(5000);
    
    while(1) {
        systemMonitor.update();  // Check heap, RSSI, temperature, etc.
        watchdogManager.feed();  // Feed HW+SW watchdog
        logger.flush();          // Flush log buffer to SPIFFS if needed
        
        // Check for critical conditions
        if (systemMonitor.isCritical()) {
            errorHandler.reportError(ErrorCode::SYSTEM_CRITICAL_MEMORY);
        }
        
        vTaskDelay(xPeriod);
    }
}
```

---

## Data Flow Architecture

### Energy Measurement Flow
```
┌─────────────┐    SPI Read     ┌──────────────────┐
│  ATM90E36   │ ─────────────>  │ ATM90E36Driver   │
│  (Hardware) │                 │ (Low-level SPI)  │
└─────────────┘                 └──────────────────┘
                                         │
                                         │ Register values
                                         ↓
                                ┌──────────────────┐
                                │  EnergyMeter     │
                                │ - Moving average │
                                │ - Validation     │
                                │ - Scaling        │
                                └──────────────────┘
                                         │
                                         │ MeterData struct
                                         ↓
                                ┌──────────────────┐
                                │EnergyAccumulator │
                                │ - NVS persist    │
                                │ - Daily rollover │
                                └──────────────────┘
                                         │
                                         │ Accumulated energy
                                         ↓
                   ┌────────────────────────────────────────┐
                   │        Shared Data Buffer (Mutex)      │
                   └────────────────────────────────────────┘
                                         │
                ┌────────────────────────┼────────────────────────┐
                │                        │                        │
                ↓                        ↓                        ↓
        ┌──────────────┐        ┌──────────────┐        ┌──────────────┐
        │ TCPDataServer│        │  WebServer   │        │ MQTTPublisher│
        │  (Port 8088) │        │  (Port 80)   │        │ (Broker pub) │
        └──────────────┘        └──────────────┘        └──────────────┘
```

### Configuration Flow
```
SPIFFS:/config.json  ────read────>  ConfigManager
                                         │
                                         │ Parse/Validate
                                         ↓
                           ┌─────────────────────────────┐
                           │ In-Memory Config Structures │
                           │ - WiFiConfig                │
                           │ - MQTTConfig                │
                           │ - ModbusConfig              │
                           │ - CalibrationConfig         │
                           └─────────────────────────────┘
                                         │
        ┌────────────────────────────────┼────────────────────────────────┐
        │                                │                                │
        ↓                                ↓                                ↓
  NetworkManager                  ATM90E36Driver                   TCPDataServer
  (WiFi settings)                 (Calibration)                    (Protocol config)
```

---

## TCP Data Protocol (V1.0 Compatible)

### Connection Sequence
```
Client                                  Server (ESP32)
  │                                          │
  │──────── TCP Connect to 8088 ────────────>│
  │                                          │
  │<────────── /SYNC/ ───────────────────────│
  │                                          │
  │──────── Code_NewReq ─────────────────────>│
  │                                          │
  │<─── AE1:123.45 ──────────────────────────│
  │<─── AE2:678.90 ──────────────────────────│
  │<─── ... (all tags) ──────────────────────│
  │<─── Code_End ────────────────────────────│
  │                                          │
  │──────── Code_Cal1 ────────────────────────>│
  │                                          │
  │<─── JSON Calibration Data ───────────────│
  │                                          │
```

### Data Tags (V1.0 Protocol)

#### Energy Tags
- **AE1-AE5**: Phase A (Forward Active, Reverse Active, Forward Reactive, Reverse Reactive, Apparent)
- **BE1-BE5**: Phase B (same structure)
- **CE1-CE5**: Phase C (same structure)
- **TE1-TE6**: Totals + System (Frequency, Temperature, Neutral currents, Free heap, Uptime)

#### Power Tags
- **AP1-AP10**: Phase A power data (Total, Per-phase, Fundamental, Harmonic)
- **BP1-BP10**: Phase B power data
- **CP1-CP10**: Phase C power data
- **TP1-TP10**: Total/Combined power, THD, Power factors

#### Display Tags
- **DP1-DP3**: Voltages for display

#### Metering Tags
- **MD1-MD4**: Currents + neutral

### Commands
- **Code_NewReq**: Request current data
- **Code_Cal1/2/3/4**: Request calibration data
- **ConfigSave**: Update config without reboot
- **ConfigRst**: Update config and reboot
- **Reboot**: Immediate reboot

### Tag Format
```
<TAG><VALUE>\n

Example:
AE1<1234.5678>
TP1<0.9876>
TE1<60.02>
```

---

## ATM90E36 Register Programming Sequence

### Initialization Sequence (from V1.0)
```cpp
1. SoftReset (0x00 = 0x789A)
   - Wait 100ms for reset completion

2. ConfigStart (0x30 = 0x5678) // Unlock configuration
   
3. PLconstH (0x31) = MSB of pulse constant
   PLconstL (0x32) = LSB of pulse constant
   
4. MMode0 (0x33) = Metering mode configuration
   MMode1 (0x34) = PGA gain configuration
   
5. PStartTh (0x35) = Active power startup threshold
   QStartTh (0x36) = Reactive power startup threshold
   SStartTh (0x37) = Apparent power startup threshold
   
6. PPhaseTh (0x38) = Active power phase threshold
   QPhaseTh (0x39) = Reactive power phase threshold
   SPhaseTh (0x3A) = Apparent power phase threshold
   
7. Verify Checksum CS0 (0x3B)

8. CalStart (0x40 = 0x5678) // Unlock calibration

9. For each phase (A, B, C):
   - PoffsetX (0x41/0x43/0x45) = Active power offset
   - QoffsetX (0x42/0x44/0x46) = Reactive power offset
   - GainX (0x47/0x49/0x4B) = Power gain
   - PhiX (0x48/0x4A/0x4C) = Phase calibration angle

10. Verify Checksum CS1 (0x4D)

11. HarmStart (0x50 = 0x5678) // Unlock harmonic cal

12. For each phase (A, B, C):
    - POffsetXF (0x51/0x52/0x53) = Fundamental power offset
    - PGainXF (0x54/0x55/0x56) = Fundamental power gain

13. Verify Checksum CS2 (0x57)

14. AdjStart (0x60 = 0x5678) // Unlock measurement cal

15. For each phase (A, B, C):
    - UgainX (0x61/0x65/0x69) = Voltage RMS gain
    - IgainX (0x62/0x66/0x6A) = Current RMS gain
    - UoffsetX (0x63/0x67/0x6B) = Voltage offset
    - IoffsetX (0x64/0x68/0x6C) = Current offset

16. IgainN (0x6D) = Neutral current gain
    IoffsetN (0x6E) = Neutral current offset

17. Verify Checksum CS3 (0x6F)

18. Lock all registers:
    - ConfigStart (0x30 = 0x8765)
    - CalStart (0x40 = 0x8765)
    - HarmStart (0x50 = 0x8765)
    - AdjStart (0x60 = 0x8765)
```

### SPI Communication Protocol
```cpp
SPISettings settings(200000, MSBFIRST, SPI_MODE3);

Read Register:
1. CS LOW
2. Delay 10µs
3. Transfer address with READ bit (address | 0x8000)
   - MSB/LSB byte-swap required
4. Delay 4µs
5. Transfer 0x0000 to clock in data (2 bytes)
   - MSB/LSB byte-swap on received data
6. CS HIGH
7. Delay 10µs

Write Register:
1. CS LOW
2. Delay 10µs
3. Transfer address with WRITE bit (address & 0x7FFF)
   - MSB/LSB byte-swap required
4. Delay 4µs
5. Transfer data (2 bytes)
   - MSB/LSB byte-swap required
6. CS HIGH
7. Delay 10µs

Mutex Protection:
- All SPI operations wrapped in mutex (3x retry on failure)
- Prevents conflicts with other SPI devices (W5500 if used)
```

---

## Configuration Management

### Old V1.0 Approach (EEPROM_Defs.h)
```cpp
// 650+ sequential #defines - FRAGILE!
#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_LENGTH 32
#define EEPROM_WIFI_PASSWORD_START 32
// ... 600+ more lines
```

### New V2.0 Approach (JSON on SPIFFS)
```json
{
  "wifi": {
    "ssid": "MyNetwork",
    "password": "MyPassword",
    "hostname": "ge3222m",
    "useDHCP": true,
    "staticIP": "192.168.1.100",
    "gateway": "192.168.1.1",
    "subnet": "255.255.255.0",
    "dns1": "8.8.8.8",
    "dns2": "8.8.4.4",
    "apMode": true,
    "apSSID": "GE3222M_Setup",
    "apPassword": "12345678"
  },
  "mqtt": {
    "enabled": false,
    "server": "mqtt.example.com",
    "port": 1883,
    "username": "",
    "password": "",
    "topicPrefix": "energy/ge3222m",
    "publishInterval": 60,
    "useTLS": false
  },
  "modbus": {
    "rtuEnabled": true,
    "tcpEnabled": false,
    "baudRate": 9600,
    "slaveId": 1,
    "parity": "N",
    "tcpPort": 502
  },
  "calibration": {
    "lineFreq": 60,
    "pgaGain": 21,
    "voltageGain": [30000, 30000, 30000],
    "currentGain": [30000, 30000, 30000, 30000],
    "calRegisters": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    "harCalRegisters": [0, 0, 0, 0, 0, 0, 0],
    "measurCalRegisters": [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
  }
}
```

**Benefits:**
- Human-readable
- Easy to backup/restore
- Schema validation
- Extensible
- Auto-backup to `/config.json.bak`

---

## Error Handling Architecture

### Centralized Error Handler
```cpp
class ErrorHandler {
public:
    static ErrorHandler& getInstance();
    
    void reportError(ErrorCode code, ErrorSeverity severity, 
                     const char* module, const char* message);
    
    // Error retrieval
    uint16_t getErrorCount() const;
    bool getRecentErrors(ErrorEntry* buffer, size_t maxCount);
    
    // Recovery actions
    void triggerRecovery(ErrorCode code);
    
private:
    struct ErrorEntry {
        ErrorCode code;
        ErrorSeverity severity;
        uint32_t timestamp;
        char module[16];
        char message[64];
    };
    
    ErrorEntry ringBuffer[50];  // Last 50 errors
    size_t writeIndex;
    uint16_t totalErrors;
};
```

### Error Categories & Recovery Actions

| Category | Recovery Action |
|----------|----------------|
| **ATM Errors** | Reset ATM90E36, reinitialize |
| **WiFi Errors** | Reconnect, fallback to AP mode |
| **TCP Errors** | Close connection, restart server |
| **Config Errors** | Load backup, restore factory defaults |
| **Energy Errors** | Use last valid data, log warning |
| **System Errors** | Reboot if critical, continue if warning |

---

## Logging Architecture

### Multi-Level Logger
```cpp
enum class LogLevel {
    TRACE,   // Verbose debug information
    DEBUG,   // Debug messages
    INFO,    // General information
    WARN,    // Warning messages
    ERROR,   // Error messages
    FATAL    // Fatal errors (system cannot continue)
};

class Logger {
public:
    static Logger& getInstance();
    
    void log(LogLevel level, const char* module, const char* format, ...);
    
    // Output destinations
    void enableSerial(bool enable);
    void enableSPIFFS(bool enable, const char* path = "/log.txt");
    void enableWebSocket(bool enable);
    
    // Ring buffer
    bool getRecentLogs(char* buffer, size_t maxSize);
    
private:
    struct LogEntry {
        uint32_t timestamp;
        LogLevel level;
        char module[16];
        char message[128];
    };
    
    LogEntry ringBuffer[200];  // Last 200 log entries
};
```

### Usage Example
```cpp
Logger::getInstance().log(LogLevel::INFO, "MAIN", "System initialized");
Logger::getInstance().log(LogLevel::ERROR, "ATM", "Checksum mismatch: 0x%04X", value);
Logger::getInstance().log(LogLevel::WARN, "WIFI", "Weak signal: %d dBm", rssi);
```

---

## Migration Guide: V1.0 → V2.0

### For End Users
1. **Backup V1.0 configuration** via VB.NET app
2. **Flash V2.0 firmware** via USB or OTA
3. **First boot:** Device creates AP "GE3222M_Setup"
4. **Connect to AP** and navigate to `http://192.168.4.1`
5. **Configure WiFi** via web interface
6. **Import calibration** data from backup (JSON format)
7. **Test connection** with V1.0 or V2.0 desktop app

### Protocol Compatibility
- **V1.0 App → V2.0 Firmware:** ✅ Fully compatible (TCP port 8088)
- **V2.0 App → V1.0 Firmware:** ⚠️ Limited (no REST/MQTT/WebSocket)
- **V2.0 App → V2.0 Firmware:** ✅ Full feature set

### Configuration Migration
```cpp
// Pseudo-code for migration helper
if (EEPROM.read(0) == 0xAA) {  // V1.0 signature
    // Read V1.0 EEPROM data
    WiFiConfig wifiCfg;
    strcpy(wifiCfg.ssid, readEEPROMString(EEPROM_WIFI_SSID_START, 32));
    strcpy(wifiCfg.password, readEEPROMString(EEPROM_WIFI_PASSWORD_START, 64));
    // ... read other settings
    
    // Write to V2.0 config.json
    ConfigManager::getInstance().saveConfig(wifiCfg, mqttCfg, modbusConfig, calCfg);
    
    // Mark migration complete
    EEPROM.write(0, 0xBB);  // V2.0 signature
}
```

---

## Feature Comparison Matrix

| Feature | V1.0 | V2.0 |
|---------|------|------|
| **Architecture** | Monolithic flat | Layered modular |
| **Configuration** | EEPROM (650+ #defines) | JSON on SPIFFS |
| **Task Management** | Single loop() | FreeRTOS multi-core |
| **Error Handling** | Ad-hoc | Centralized with codes |
| **Logging** | Serial only | Multi-destination + ring buffer |
| **TCP Protocol** | Custom text-based | ✅ Same (backward compatible) |
| **Web Interface** | Basic HTML | Modern SPA with WebSocket |
| **REST API** | ❌ None | ✅ Full JSON API |
| **MQTT** | ❌ None | ✅ Configurable broker/TLS |
| **Modbus TCP** | ❌ None | ✅ Port 502 |
| **Modbus RTU** | ✅ Serial | ✅ Serial (improved) |
| **OTA Updates** | ✅ Basic | ✅ Enhanced with rollback |
| **Diagnostics** | Minimal | Comprehensive system health |
| **Watchdog** | ❌ None | HW + SW dual watchdog |
| **Memory Safety** | ❌ No checks | Heap monitoring + alerts |
| **Energy Accumulation** | Volatile | NVS persistent with rollover |
| **Unit Tests** | ❌ None | ✅ PlatformIO native tests |

---

## Testing Strategy

### Unit Tests (PlatformIO)
```cpp
// test/test_energy_meter.cpp
void test_voltage_validation() {
    EnergyMeter meter;
    MeterData data;
    
    data.phaseA.voltage = 250.0;  // Valid
    TEST_ASSERT_TRUE(meter.validateMeasurements(data));
    
    data.phaseA.voltage = 600.0;  // Invalid (> 500V)
    TEST_ASSERT_FALSE(meter.validateMeasurements(data));
}

// test/test_config_manager.cpp
void test_json_serialization() {
    WiFiConfig cfg;
    strcpy(cfg.ssid, "TestNetwork");
    
    String json = ConfigManager::serializeWiFi(cfg);
    WiFiConfig loaded = ConfigManager::deserializeWiFi(json);
    
    TEST_ASSERT_EQUAL_STRING(cfg.ssid, loaded.ssid);
}
```

### Integration Tests
- Connect to actual ATM90E36 hardware
- Verify register read/write with known values
- Test calibration sequence end-to-end
- Validate TCP protocol with V1.0 client

### System Tests
- Long-duration stability test (24+ hours)
- WiFi reconnection stress test
- Memory leak detection (heap watermark monitoring)
- Watchdog timeout recovery test

---

## Performance Characteristics

### Memory Usage (Typical)
- **Heap at boot:** ~220 KB free (of ~280 KB)
- **After all tasks:** ~180 KB free
- **Critical threshold:** 20 KB (warning), 10 KB (critical)
- **Persistent storage:** 1.5 MB SPIFFS (config + logs + web files)

### CPU Usage (Typical)
- **EnergyTask:** 5% (1Hz acquisition)
- **NetworkTask:** 15% (10ms polling)
- **DiagTask:** <1% (5s interval)
- **OTATask:** <1% (idle unless updating)
- **Idle:** ~78%

### Response Times
- **TCP data request:** <50ms (typical), <100ms (max)
- **Web API request:** <30ms (cached), <80ms (database query)
- **Modbus RTU:** <20ms (per register)
- **MQTT publish:** Async, non-blocking

### Power Consumption
- **Active (WiFi on):** ~120 mA @ 5V
- **Idle (WiFi connected):** ~80 mA @ 5V
- **AP mode:** ~100 mA @ 5V

---

## Development Workflow

### PlatformIO Build
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

lib_deps =
    AsyncTCP
    ESPAsyncWebServer
    ArduinoJson
    AsyncMqttClient
    ezTime
    
upload_speed = 921600
monitor_speed = 115200

build_flags =
    -DCORE_DEBUG_LEVEL=3
    -DENABLE_MQTT=1
    
test_build_project_src = true
```

### Build Commands
```bash
# Build firmware
pio run

# Upload via USB
pio run --target upload

# Upload via OTA
pio run --target upload --upload-port ge3222m.local

# Run unit tests
pio test

# Monitor serial output
pio device monitor

# Clean build
pio run --target clean
```

### Code Style
- **Naming:** PascalCase for classes, camelCase for methods/variables
- **Documentation:** Doxygen-style comments (/** ... */)
- **Singletons:** Meyer's singleton pattern (static local instance)
- **Error handling:** Always check return values, use ErrorHandler
- **Logging:** Use Logger with appropriate levels

---

## Future Enhancements (V2.1+)

### Planned Features
- [ ] **Demand response integration** (MQTT commands)
- [ ] **Load shedding control** (relay automation)
- [ ] **Time-of-use tariff tracking** (configurable rates)
- [ ] **Solar PV integration** (bi-directional energy tracking)
- [ ] **Harmonic analysis dashboard** (FFT visualization)
- [ ] **Alexa/Google Home integration** (voice queries)
- [ ] **InfluxDB/Grafana export** (time-series database)
- [ ] **Telegram bot notifications** (alerts and queries)
- [ ] **LoRaWAN support** (long-range wireless)
- [ ] **BLE configuration** (smartphone app setup)

---

## Support & Contribution

### Getting Help
- **Documentation:** This file + inline code comments
- **Issues:** GitHub issue tracker
- **Email:** support@microcodeeng.com

### Contributing
1. Fork the repository
2. Create feature branch (`feature/my-feature`)
3. Write tests for new functionality
4. Ensure all tests pass (`pio test`)
5. Submit pull request with description

### License
- **Hardware:** Open Source Hardware (OSHW)
- **Firmware:** MIT License
- **Desktop App:** MIT License

---

**Document Version:** 1.0  
**Last Updated:** 2026-02-08  
**Author:** Microcode Engineering
