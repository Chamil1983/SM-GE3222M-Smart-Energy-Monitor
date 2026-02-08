# V2.0 Architecture Modernization - Implementation Guide

## Executive Summary

This document provides a **complete blueprint** for implementing the V2.0 modernization of the SM-GE3222M Smart Energy Monitor system. The foundation architecture is complete, including:

- ✅ **85KB of comprehensive documentation**
- ✅ **Complete directory structures**
- ✅ **Core header files with data structures**
- ✅ **PlatformIO configuration**
- ✅ **Boot sequence specification**
- ✅ **Protocol specifications**
- ✅ **Migration guides**

---

## Project Scope

### What's Being Modernized

**ESP32 Firmware (V1.0 → V2.0):**
- From: 24KB monolithic .ino + 650+ #defines
- To: Layered architecture with HAL, drivers, services

**VB.NET Desktop App (V1.0 → V2.0):**
- From: 200KB god-class with blocking TCP
- To: MVVM-like architecture with async/await

### What's NOT Changing

✅ **Hardware:** Same ESP32 + ATM90E36 + MCP23017  
✅ **Pin Assignments:** All GPIO mappings preserved  
✅ **TCP Protocol:** V1.0 protocol maintained for compatibility  
✅ **ATM90E36 Initialization:** Register sequences preserved exactly  
✅ **V1.0 Code:** Never modified, kept for reference  

---

## Architecture Overview

### ESP32 Firmware Layers

```
Application Layer (main.cpp, TaskManager)
           ↓
Service Layer (EnergyMeter, NetworkManager, ConfigManager)
           ↓
Communication Layer (TCPDataServer, WebServer, Modbus, MQTT)
           ↓
HAL Layer (SPIBus, I2CBus, GPIOManager)
           ↓
Driver Layer (ATM90E36Driver)
           ↓
Hardware (ATM90E36, MCP23017, ESP32)
```

### VB.NET Application Layers

```
Presentation Layer (MainDashboard, Views, Controls)
           ↓
Service Layer (MeterDataService, CalibrationService, DataLoggerService)
           ↓
Communication Layer (MeterConnection, ProtocolParser, CommandSender)
           ↓
Model Layer (PhaseData, MeterSnapshot, CalibrationData)
           ↓
Network (TCP Socket to ESP32 Firmware)
```

---

## Implementation Phases

### Phase 1: ESP32 Core Infrastructure (Week 1-2)

**Goal:** Establish foundation for logging, error handling, and task management.

**Tasks:**
1. **ErrorHandler Implementation**
   ```cpp
   // src/core/ErrorHandler.h/cpp
   class ErrorHandler {
   public:
       static ErrorHandler& getInstance();
       void reportError(ErrorCode code, ErrorSeverity severity, 
                       const char* module, const char* message);
       // Ring buffer for last 50 errors
       // Recovery action triggers
   };
   ```

2. **Logger Implementation**
   ```cpp
   // src/diagnostics/Logger.h/cpp
   class Logger {
   public:
       static Logger& getInstance();
       void log(LogLevel level, const char* module, const char* format, ...);
       // Multi-destination (Serial, SPIFFS, WebSocket)
       // Ring buffer for last 200 entries
   };
   ```

3. **TaskManager Implementation**
   ```cpp
   // src/core/TaskManager.h/cpp
   class TaskManager {
   public:
       static TaskManager& getInstance();
       void createAllTasks();
       // EnergyTask (Core1, 8KB, pri5)
       // NetworkTask (Core0, 8KB, pri3)
       // DiagTask (Core1, 4KB, pri1)
       // OTATask (Core0, 4KB, pri2)
   };
   ```

4. **WatchdogManager Implementation**
   ```cpp
   // src/core/WatchdogManager.h/cpp
   class WatchdogManager {
   public:
       static WatchdogManager& getInstance();
       void init(uint32_t timeoutSeconds);
       void feed();
       // HW + SW dual watchdog
   };
   ```

**Validation:**
- ✅ Serial logging works
- ✅ Errors tracked in ring buffer
- ✅ Tasks created successfully
- ✅ Watchdog doesn't trigger during normal operation

**Time Estimate:** 3-5 days

---

### Phase 2: ESP32 HAL Layer (Week 2-3)

**Goal:** Hardware abstraction for SPI, I2C, GPIO.

**Tasks:**
1. **SPIBus Implementation**
   ```cpp
   // src/hal/SPIBus.h/cpp
   class SPIBus {
   public:
       static SPIBus& getInstance();
       void init();
       uint16_t transfer(uint8_t csPin, uint16_t* txData, uint16_t* rxData, size_t len);
       // Mutex protection
       // Retry logic (3x)
   };
   ```

2. **I2CBus Implementation**
   ```cpp
   // src/hal/I2CBus.h/cpp
   class I2CBus {
   public:
       static I2CBus& getInstance();
       void init(uint8_t sda, uint8_t scl);
       bool scanDevice(uint8_t address);
       bool write(uint8_t address, uint8_t* data, size_t len);
       // Device scanner
       // Retry logic
   };
   ```

3. **GPIOManager Implementation**
   ```cpp
   // src/hal/GPIOManager.h/cpp
   class GPIOManager {
   public:
       static GPIOManager& getInstance();
       void init();
       void setLED(LED_ID led, bool state);
       void setRelay(RELAY_ID relay, bool state);
       bool readButton(uint8_t pin);
       // MCP23017 interface
       // Debouncing for buttons
   };
   ```

4. **NetworkManager Implementation**
   ```cpp
   // src/hal/NetworkManager.h/cpp
   class NetworkManager {
   public:
       static NetworkManager& getInstance();
       bool init(const WiFiConfig& config);
       void maintain();  // Keep-alive, reconnect
       String getIPAddress();
       // WiFi STA mode
       // AP mode fallback
       // mDNS "ge3222m.local"
   };
   ```

**Validation:**
- ✅ SPI communication with ATM90E36 works
- ✅ I2C devices detected (MCP23017, LCD, EEPROM)
- ✅ LEDs controllable
- ✅ WiFi connects or enters AP mode

**Time Estimate:** 5-7 days

---

### Phase 3: ESP32 Energy Measurement (Week 3-4)

**Goal:** Port ATM90E36 driver and implement energy metering.

**Tasks:**
1. **ATM90E36Driver Implementation**
   ```cpp
   // src/energy/ATM90E36Driver.h/cpp
   // Port from V1.0 EnergyATM90E36.cpp (50KB)
   class ATM90E36Driver {
   public:
       static ATM90E36Driver& getInstance();
       bool init(const CalibrationConfig& calibration);
       uint16_t readRegister(uint16_t address);
       void writeRegister(uint16_t address, uint16_t value);
       // Full init sequence with checksums
       // All measurement getters
   };
   ```

2. **EnergyMeter Implementation**
   ```cpp
   // src/energy/EnergyMeter.h/cpp
   class EnergyMeter {
   public:
       static EnergyMeter& getInstance();
       bool readAll(MeterData& data);
       // Moving average filter (5 samples)
       // Validation (voltage 0-500V, current 0-200A, freq 45-65Hz)
   };
   ```

3. **EnergyAccumulator Implementation**
   ```cpp
   // src/energy/EnergyAccumulator.h/cpp
   class EnergyAccumulator {
   public:
       static EnergyAccumulator& getInstance();
       void init(uint32_t persistIntervalSeconds);
       void update(const MeterData& data);
       // NVS persistence
       // Daily/weekly/monthly rollover
   };
   ```

**Validation:**
- ✅ ATM90E36 initialized successfully
- ✅ Checksums validated
- ✅ Voltage/current/power readings match V1.0
- ✅ Energy accumulates correctly
- ✅ Data persists across reboots

**Time Estimate:** 7-10 days (largest porting effort)

---

### Phase 4: ESP32 Storage & Configuration (Week 4-5)

**Goal:** Replace EEPROM approach with JSON configuration.

**Tasks:**
1. **SPIFFSManager Implementation**
   ```cpp
   // src/storage/SPIFFSManager.h/cpp
   class SPIFFSManager {
   public:
       static SPIFFSManager& getInstance();
       bool init();
       bool fileExists(const char* path);
       size_t getUsedSpace();
       // Mount/format
       // File operations
   };
   ```

2. **ConfigManager Implementation**
   ```cpp
   // src/storage/ConfigManager.h/cpp
   class ConfigManager {
   public:
       static ConfigManager& getInstance();
       bool init();
       WiFiConfig getWiFiConfig();
       MQTTConfig getMQTTConfig();
       // JSON serialization/deserialization
       // Schema validation
       // Auto-backup to /config.json.bak
   };
   ```

3. **EEPROMBridge Implementation**
   ```cpp
   // src/storage/EEPROMBridge.h/cpp
   // External I2C EEPROM (AT24C64 at 0x50)
   class EEPROMBridge {
   public:
       static EEPROMBridge& getInstance();
       uint8_t read8(uint16_t address);
       void write8(uint16_t address, uint8_t value);
       // Factory calibration snapshot only
   };
   ```

**Validation:**
- ✅ SPIFFS mounts successfully
- ✅ Config loads from /config.json
- ✅ Config saves persist across reboots
- ✅ Backup file created on save
- ✅ External EEPROM readable

**Time Estimate:** 3-5 days

---

### Phase 5: ESP32 Communication Protocols (Week 5-6)

**Goal:** Implement TCP, Web, Modbus, MQTT.

**Tasks:**
1. **TCPDataServer Implementation**
   ```cpp
   // src/comm/TCPDataServer.h/cpp
   class TCPDataServer {
   public:
       static TCPDataServer& getInstance();
       void begin(uint16_t port);
       void update();
       // V1.0 protocol compatible
       // Max 4 clients
       // /SYNC/ handshake
       // Tag:Value format
   };
   ```

2. **WebServer Implementation**
   ```cpp
   // src/comm/WebServer.h/cpp
   class WebServer {
   public:
       static WebServer& getInstance();
       void begin(uint16_t port);
       // ESPAsyncWebServer
       // REST API endpoints
       // WebSocket /ws
   };
   ```

3. **ModbusServer Implementation**
   ```cpp
   // src/comm/ModbusServer.h/cpp
   // Port from V1.0 Modbus_Serial.cpp
   class ModbusServer {
   public:
       static ModbusServer& getInstance();
       void begin(const ModbusConfig& config);
       // RTU on Serial2
       // TCP on port 502
       // FC03, FC04, FC06, FC16
   };
   ```

4. **MQTTPublisher Implementation**
   ```cpp
   // src/comm/MQTTPublisher.h/cpp
   class MQTTPublisher {
   public:
       static MQTTPublisher& getInstance();
       void begin(const MQTTConfig& config);
       void update();
       // AsyncMqttClient
       // JSON publish
       // Auto-reconnect
   };
   ```

**Validation:**
- ✅ V1.0 VB.NET app connects successfully
- ✅ All data tags transmitted correctly
- ✅ Calibration commands work
- ✅ Web interface accessible
- ✅ Modbus queries respond correctly
- ✅ MQTT publishes if enabled

**Time Estimate:** 7-10 days

---

### Phase 6: ESP32 Web Interface (Week 6-7)

**Goal:** Create modern SPA dashboard.

**Files:**
```
data/
├── index.html  # Single-page application
├── app.js      # JavaScript logic
├── style.css   # Responsive CSS
└── config.json # Default configuration
```

**Features:**
- Real-time WebSocket updates
- Gauge/chart rendering
- Configuration editor
- Diagnostics viewer
- Dark/light theme

**Validation:**
- ✅ Web interface loads
- ✅ Real-time data updates
- ✅ Configuration editor works
- ✅ Responsive on mobile/tablet

**Time Estimate:** 5-7 days

---

### Phase 7: VB.NET Models & Communication (Week 7-8)

**Goal:** Implement data models and async TCP client.

**Tasks:**
1. **PhaseData Model**
   ```vb
   Public Class PhaseData
       Implements INotifyPropertyChanged
       Public Property Voltage As Single
       Public Property Current As Single
       ' ... with PropertyChanged events
   End Class
   ```

2. **MeterSnapshot Model**
   ```vb
   Public Class MeterSnapshot
       Public Property PhaseA/B/C As PhaseData
       Public Property TotalActivePower As Single
       Public Shared Function FromTagDictionary(tags As Dictionary) As MeterSnapshot
   End Class
   ```

3. **ProtocolParser**
   ```vb
   Public Class ProtocolParser
       Public Function ParseFrame(rawData As String) As Dictionary(Of String, Single)
           ' Tag-dictionary parsing (position-independent)
       End Function
   End Class
   ```

4. **MeterConnection**
   ```vb
   Public Class MeterConnection
       Public Async Function ConnectAsync(ip As String, port As Integer) As Task(Of Boolean)
           ' Async TCP client
           ' State machine
           ' Event-driven
       End Function
   End Class
   ```

**Validation:**
- ✅ Models data-bind to UI
- ✅ ProtocolParser handles all tags
- ✅ Async connection doesn't freeze UI
- ✅ Reconnection works

**Time Estimate:** 5-7 days

---

### Phase 8: VB.NET Services & Views (Week 8-9)

**Goal:** Implement services and UI forms.

**Services:**
- MeterDataService (coordinator)
- DataLoggerService (SQLite)
- AlertService (threshold monitoring)
- CalibrationService (cal management)
- ReportService (PDF/Excel export)

**Views:**
- MainDashboard (MDI parent)
- LiveMonitorView (real-time display)
- EnergyDetailView (energy tracking)
- CalibrationWizard (step-by-step calibration)
- DataLoggerView (historical data)

**Controls:**
- GaugeControl (circular gauge)
- PhaseBarControl (3-phase balance)
- ConnectionStatusBar

**Validation:**
- ✅ All views functional
- ✅ Data logging works
- ✅ Alerts trigger correctly
- ✅ Calibration wizard flows properly
- ✅ Export to PDF/Excel works

**Time Estimate:** 10-14 days

---

### Phase 9: Testing & Integration (Week 10)

**ESP32 Unit Tests:**
```cpp
// test/test_energy_meter.cpp
void test_voltage_validation() {
    // Test validation logic
}

void test_moving_average() {
    // Test filter
}
```

**VB.NET Unit Tests:**
```vb
<TestMethod>
Public Sub ParseFrame_ValidData_ReturnsCorrectDictionary()
    ' Test protocol parser
End Sub
```

**Integration Tests:**
- End-to-end data flow
- Long-duration stability (24+ hours)
- Memory leak detection
- Reconnection scenarios
- Calibration workflow
- Export functionality

**Validation:**
- ✅ All unit tests pass
- ✅ No memory leaks
- ✅ System stable for 24+ hours
- ✅ V1.0 compatibility verified
- ✅ Performance targets met

**Time Estimate:** 7-10 days

---

### Phase 10: Documentation & Release (Week 11)

**Tasks:**
- Update all documentation
- Create user manual
- Record demo videos
- Package installer
- Create GitHub release

**Deliverables:**
- Firmware binary (.bin)
- Desktop app installer (.msi)
- User manual (.pdf)
- Migration guide
- Demo videos
- Source code ZIP

**Time Estimate:** 3-5 days

---

## Total Timeline Estimate

**Conservative Estimate:** 11-12 weeks (3 months)
**Optimistic Estimate:** 8-9 weeks (2 months)
**Realistic Estimate:** 10 weeks with 1 developer full-time

---

## Risk Mitigation

### Risk 1: ATM90E36 Driver Complexity
**Mitigation:** 
- Port carefully from V1.0
- Test each register write
- Verify checksums at each step
- Keep V1.0 side-by-side for comparison

### Risk 2: Protocol Compatibility Issues
**Mitigation:**
- Test with V1.0 VB.NET app continuously
- Use protocol analyzer to compare traffic
- Maintain unit tests for parser

### Risk 3: Memory Constraints
**Mitigation:**
- Monitor heap usage throughout development
- Use PROGMEM for large constant arrays
- Implement heap warnings early
- Profile memory usage regularly

### Risk 4: Async/Await Learning Curve (VB.NET)
**Mitigation:**
- Create simple examples first
- Use ConfigureAwait(False) correctly
- Test thoroughly for race conditions
- Code review async patterns

---

## Success Criteria

### Functional Requirements
- [ ] Boots in <30 seconds
- [ ] Connects to WiFi or enters AP mode
- [ ] Reads ATM90E36 successfully
- [ ] V1.0 VB.NET app can connect
- [ ] All data tags transmitted correctly
- [ ] Calibration commands work
- [ ] Web interface accessible
- [ ] MQTT publishes
- [ ] Modbus responds
- [ ] Configuration persists
- [ ] OTA updates work
- [ ] VB.NET app connects without freezing
- [ ] Data logging works
- [ ] Alerts trigger
- [ ] Export to PDF/Excel works

### Performance Requirements
- [ ] Heap usage <200KB
- [ ] No memory leaks over 24+ hours
- [ ] Data update rate ≥1Hz
- [ ] TCP response time <100ms
- [ ] VB.NET UI maintains 60 FPS

### Quality Requirements
- [ ] All unit tests pass
- [ ] Code coverage >70%
- [ ] Documentation complete
- [ ] No critical security vulnerabilities
- [ ] Passes 24-hour stability test

---

## Resources

### Documentation
- **ESP32 Firmware:** `/Firmware_Released/SM_GE3222M V2.0/ARCHITECTURE.md`
- **VB.NET App:** `/Firmware_Released/Meter GE3222M V2.0/ARCHITECTURE.md`
- **This Guide:** `IMPLEMENTATION_GUIDE.md`

### Reference Code
- **V1.0 Firmware:** `/Firmware_Released/SM_GE3222M V1.0/` (DO NOT MODIFY)
- **V1.0 VB.NET:** `/Firmware_Released/Meter GE3222M V1.0/` (DO NOT MODIFY)

### Tools
- **PlatformIO:** Build system for ESP32
- **Visual Studio:** VB.NET development
- **Git:** Version control
- **GitHub:** Issue tracking, PRs

### Hardware
- SM-GE3222M board with ESP32 + ATM90E36
- USB cable for programming
- WiFi network for testing
- 3-phase power source for validation

---

## Next Steps

1. **Review this guide** with the development team
2. **Set up development environment** (PlatformIO, Visual Studio)
3. **Create project branch** from main
4. **Begin Phase 1** (Core Infrastructure)
5. **Daily standups** to track progress
6. **Weekly reviews** to validate milestones
7. **Continuous integration** with hardware testing

---

## Support & Contact

- **GitHub Issues:** For bugs and feature requests
- **Email:** support@microcodeeng.com
- **Documentation:** ARCHITECTURE.md files

---

**Document Version:** 1.0  
**Last Updated:** 2026-02-08  
**Author:** Microcode Engineering  
**Status:** Ready for Implementation
