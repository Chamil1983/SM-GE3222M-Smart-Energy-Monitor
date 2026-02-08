# SM-GE3222M V2.0 - Implementation Guide

This guide provides detailed instructions for completing the V2.0 implementation based on the skeleton architecture provided.

## Current Implementation Status

### ✅ Phase 1: Foundation - COMPLETE
The foundation is fully implemented with:
- Directory structures for ESP32 firmware and VB.NET application
- Complete pin mappings, register maps, and Modbus layout
- All data structures and configuration models
- 6-phase boot sequence framework
- Protocol parser architecture
- Comprehensive documentation

### 🚧 Phases 2-9: To Be Implemented
The remaining phases require full implementation of the modules marked with TODO comments in `main.cpp` and the VB.NET project structure.

---

## ESP32 Firmware Implementation Guide

### Phase 2: HAL Layer Implementation

#### 1. SPIBus (src/hal/SPIBus.h/.cpp)
**Purpose**: Thread-safe SPI communication for ATM90E36

**Key Features**:
- FreeRTOS mutex for thread safety
- 200kHz, Mode3, MSB-first for ATM90E36
- Byte-swapping for address and data
- CS LOW → 10µs → Address → 4µs → Data → CS HIGH timing

**Implementation Steps**:
1. Create FreeRTOS mutex in constructor
2. Implement `init(sck, miso, mosi, frequency)`
3. Port `CommEnergyIC()` logic from V1.0 `EnergyATM90E36.cpp`:
   - Set R/W bit (bit 15): READ=1, WRITE=0
   - Swap MSB/LSB of address and data
   - Implement timing delays (10µs, 4µs)
4. Implement `readRegister(csPin, address)`
5. Implement `writeRegister(csPin, address, value)`
6. Implement `read32BitRegister(csPin, highAddr, lowAddr)` for energy registers

**Reference**: `Firmware_Released/SM_GE3222M V1.0/EnergyATM90E36.cpp` lines 50-120

#### 2. I2CBus (src/hal/I2CBus.h/.cpp)
**Purpose**: Thread-safe I2C for MCP23017 and sensors

**Implementation Steps**:
1. Create FreeRTOS mutex
2. Implement `init(sda, scl, frequency)`
3. Implement `readBytes(address, register, buffer, length)`
4. Implement `writeBytes(address, register, buffer, length)`
5. Implement `scanBus()` for device discovery

**Reference**: Standard ESP32 Wire library with mutex protection

#### 3. GPIOManager (src/hal/GPIOManager.h/.cpp)
**Purpose**: LED, relay, button management via MCP23017

**Implementation Steps**:
1. Initialize MCP23017 on I2C
2. Configure Port A as outputs (relays, LEDs)
3. Configure Port B as inputs (IRQs, pulses)
4. Implement `setLED(pin, state)`, `toggleLED(pin)`
5. Implement `setRelay(pin, state)`
6. Implement `readButton(pin)` with debounce
7. Implement LED patterns (boot, running, error, wifi)

**Reference**: `Firmware_Released/SM_GE3222M V1.0/GE3222M_Hardware V1.ino`

### Phase 3: Energy IC Driver Implementation

#### 1. ATM90E36Driver (src/energy/ATM90E36Driver.cpp)
**Purpose**: Complete driver for ATM90E36 energy IC

**Header exists at**: `src/energy/ATM90E36Driver.h`

**Implementation Steps**:
1. Port `CommEnergyIC()` → use SPIBus singleton
2. Port `begin()` → `init(CalibrationConfig& config)`:
   - ConfigStart (0x5678)
   - PLconstH/L
   - MMode0 (lineFreq), MMode1 (pgaGain)
   - Thresholds (PStartTh, QStartTh, SStartTh, PPhaseTh, QPhaseTh, SPhaseTh)
   - CSZero (0x4741)
   - CalStart (0x5678)
   - Gain/Phi for each phase
   - CSOne (calculated checksum)
   - HarmStart (0x5678)
   - Fundamental power offsets/gains
   - CSTwo
   - AdjStart (0x5678)
   - Voltage/Current gains/offsets per phase + neutral
   - CSThree
3. Port all getter functions (60+ methods from V1.0)
4. Implement `readAll(MeterData&)` bulk read for efficiency
5. Implement `verifyChecksums()` - read CS0/CS1/CS2/CS3
6. Implement `hasCalibrationError()` - check SysStatus0

**Reference**: `Firmware_Released/SM_GE3222M V1.0/EnergyATM90E36.cpp` (entire file ~1500 lines)

**Critical Details**:
- SPI timing: 200kHz, Mode3, MSB-first
- Byte swapping on all R/W operations
- Checksum calculation algorithm from V1.0
- Scaling factors from `RegisterMap.h`

#### 2. EnergyMeter (src/energy/EnergyMeter.h/.cpp)
**Purpose**: High-level meter with filtering

**Implementation Steps**:
1. Create moving average filter buffers (window of 5 samples)
2. Implement `init(filterWindow)`
3. Implement `update()`:
   - Call ATM90E36Driver::readAll()
   - Apply moving average filter
   - Validate ranges (V: 0-500V, I: 0-200A, F: 45-65Hz)
   - Update thread-safe snapshot with mutex
4. Implement `getSnapshot()` - mutex-protected copy
5. Implement `getRawReading()` - unfiltered data

#### 3. EnergyAccumulator (src/energy/EnergyAccumulator.h/.cpp)
**Purpose**: kWh accumulation with NVS persistence

**Implementation Steps**:
1. Implement time-based integration (∫ P dt)
2. Implement `init(persistIntervalSeconds)`
3. Implement `update(MeterData&)`:
   - Calculate energy from power × time
   - Accumulate forward/reverse active/reactive per phase
4. Implement `persist()` - save to NVS every 60 seconds
5. Implement `loadFromNVS()` - restore on boot
6. Track daily/monthly rollovers
7. Track max demand (15-minute window)

#### 4. CalibrationManager (src/energy/CalibrationManager.h/.cpp)
**Purpose**: Manage calibration data

**Implementation Steps**:
1. Implement `loadCalibration()` - read from NVS namespace "calibration"
2. Implement `saveCalibration(CalibrationConfig&)` - write to NVS
3. Implement `applyCalibration(CalibrationConfig&)` - reinit ATM90E36
4. Implement `getDefaultCalibration()` - factory defaults from V1.0

**Reference**: `Firmware_Released/SM_GE3222M V1.0/Settings.ino` + `config.cpp`

### Phase 4: Storage Layer Implementation

#### 1. ConfigManager (src/storage/ConfigManager.h/.cpp)
**Purpose**: Unified NVS configuration management

**Implementation Steps**:
1. Define NVS namespaces:
   - "wifi" → WiFiConfig
   - "network" → NetworkConfig
   - "modbus" → ModbusConfig
   - "mqtt" → MQTTConfig
   - "calibration" → CalibrationConfig
   - "energy" → Energy accumulator state
   - "system" → SystemConfig
2. Implement getters/setters for each config type
3. Implement `exportAllAsJson()` - serialize to JSON
4. Implement `importFromJson(String)` - deserialize and save
5. Implement `resetToDefaults()` - factory reset
6. Implement `getBootCount()` - auto-incrementing

**Reference**: Replace EEPROM_Defs.h scattered access with clean NVS API

#### 2. SPIFFSManager (src/storage/SPIFFSManager.h/.cpp)
**Implementation Steps**:
1. Implement `init()` - mount SPIFFS, format if needed
2. Implement `readFile(path)`, `writeFile(path, content)`
3. Implement `deleteFile(path)`, `listFiles()`
4. Implement `getUsedSpace()`, `getTotalSpace()`

#### 3. DataLogger (src/storage/DataLogger.h/.cpp)
**Purpose**: Ring-buffer logging to SPIFFS

**Implementation Steps**:
1. Create log file with timestamp
2. Implement `logSnapshot(MeterData&)` - append CSV row
3. Implement `getLogEntries(startTime, endTime)`
4. Implement auto-rotation when file > 1MB
5. Keep last N files (ring buffer)

### Phase 5: Network Layer Implementation

#### 1. NetworkManager (src/network/NetworkManager.h/.cpp)
**Purpose**: WiFi management with reconnect

**Implementation Steps**:
1. Port WiFi logic from `WiFi_OTA.cpp`
2. Implement `init(WiFiConfig&)` - non-blocking
3. Implement `startSTA()`:
   - WiFi.begin(ssid, password)
   - Static IP or DHCP
   - 15s timeout with retry
4. Implement `startAP(ssid, password)`:
   - WiFi.softAP()
   - Default IP 192.168.4.1
5. Implement WiFi event handlers (connect/disconnect)
6. Implement auto-reconnect task
7. Implement mDNS "ge3222m.local"

**Reference**: `Firmware_Released/SM_GE3222M V1.0/WiFi_OTA.cpp`

#### 2. OTAManager (src/network/OTAManager.h/.cpp)
**Implementation Steps**:
1. Setup ArduinoOTA with hostname and password
2. Implement progress callbacks for LED indication
3. Implement HTTP OTA endpoint for binary upload
4. Add OTA handle to main loop

**Reference**: `WiFi_OTA.cpp` OTA sections

#### 3. NTPSync (src/network/NTPSync.h/.cpp)
**Implementation Steps**:
1. Initialize NTPClient library
2. Implement `init(ntpServer, timezoneOffset)`
3. Implement `getEpochTime()`, `getFormattedTime()`
4. Implement auto-sync every hour

### Phase 6: Communications Implementation

#### 1. TCPDataServer (src/comm/TCPDataServer.h/.cpp)
**Purpose**: V1-compatible Tag:Value TCP protocol

**Implementation Steps**:
1. Port from `Ethernet_Interface.ino`
2. Implement `begin(port)` - default 8088
3. Support max 4 concurrent clients
4. Implement V1 protocol:
   ```cpp
   client.print("AE1"); client.println(phaseA.fwdActiveEnergy);
   client.print("AP1"); client.println(phaseA.activePower);
   // ... all tags
   ```
5. Implement commands:
   - `PC_ENABLED` → trigger data send
   - `Req_End` → signal end
   - `ConfigSave` → JSON config receive
   - `Code_Cal1-4` → calibration data
   - `Reboot` → restart
6. Implement `/SYNC/` handshake

**Tag List** (must match V1.0):
- AE1-5: Phase A Energy
- AP1-10: Phase A Power/V/I/PF/Angles
- BE1-5, BP1-10: Phase B
- CE1-5, CP1-10: Phase C
- TE1-6: Total Energy
- TP1-10: Total Power
- DP1-3: Board data

**Reference**: `Firmware_Released/SM_GE3222M V1.0/Ethernet_Interface.ino`

#### 2. ProtocolV2 (src/comm/ProtocolV2.h/.cpp)
**Purpose**: Modern JSON protocol with ACK/NACK

**Implementation Steps**:
1. Create JSON format:
   ```json
   {
     "type": "data",
     "seq": 123,
     "timestamp": 1234567890,
     "phases": {
       "A": {"V": 230.5, "I": 10.2, ...}
     }
   }
   ```
2. Implement commands:
   - `get_config`, `set_config`
   - `calibrate`, `reset_energy`, `reboot`
3. Implement ACK/NACK responses with sequence numbers
4. Use ArduinoJson library

#### 3. WebServer (src/comm/WebServer.h/.cpp)
**Purpose**: REST API + WebSocket dashboard

**Implementation Steps**:
1. Use ESPAsyncWebServer library
2. Implement REST endpoints:
   - `GET /api/data` → MeterData as JSON
   - `GET /api/config` → Configuration
   - `POST /api/config` → Update config
   - `GET /api/status` → System status
   - `POST /api/calibration` → Apply calibration
3. Implement WebSocket `/ws`:
   - Push MeterData every 1 second
   - Handle client connections (max 10)
4. Serve SPIFFS files (index.html, dashboard.js, style.css)

#### 4. ModbusServer (src/comm/ModbusServer.h/.cpp)
**Purpose**: Unified Modbus RTU + TCP

**Critical**: Eliminate V1.0's duplication (identical register setup repeated for RTU and TCP)

**Implementation Steps**:
1. Use eModbus library
2. Create single register setup function
3. Implement IEEE754 float encoding (2 registers per float):
   ```cpp
   union { float f; uint32_t i; } val;
   val.f = voltage;
   regs[addr] = (uint16_t)(val.i >> 16);  // High word
   regs[addr+1] = (uint16_t)(val.i);      // Low word
   ```
4. Implement `begin(ModbusConfig&)`:
   - Setup RTU on Serial2
   - Setup TCP on port 502
5. Implement `update()` - poll both
6. Implement `syncRegisters(MeterData&)` - update all from snapshot

**Register Layout**: See `include/ModbusMap.h`

**Reference**: `Firmware_Released/SM_GE3222M V1.0/Modbus_Serial.cpp` (but eliminate duplication!)

#### 5. MQTTPublisher (src/comm/MQTTPublisher.h/.cpp)
**Implementation Steps**:
1. Use PubSubClient library
2. Implement `begin(MQTTConfig&)` with auto-reconnect
3. Implement `publish(MeterData&)` - publish to configured topics
4. Implement Home Assistant MQTT discovery messages
5. Subscribe to command topics

### Phase 7: Core Framework Implementation

#### 1. TaskManager (src/core/TaskManager.h/.cpp)
**Purpose**: Create and manage all FreeRTOS tasks

**Task Definitions**:
| Task | Core | Priority | Stack | Period | Function |
|------|------|----------|-------|--------|----------|
| EnergyTask | 1 | 5 | 4096 | 500ms | Read ATM90E36, update snapshot |
| AccumulatorTask | 1 | 4 | 3072 | 1000ms | Accumulate energy, persist |
| ModbusTask | 1 | 3 | 4096 | 10ms | Poll Modbus RTU/TCP |
| TCPServerTask | 0 | 3 | 4096 | Event | Handle TCP clients |
| WebServerTask | 0 | 2 | 4096 | 10ms | WebSocket updates |
| MQTTTask | 0 | 2 | 3072 | Config | MQTT publish |
| DiagnosticsTask | 0 | 1 | 2048 | 5000ms | Feed watchdog, monitor |

**Implementation Steps**:
1. Create task functions for each
2. Implement `createAllTasks()` with proper priorities/affinity
3. Implement `suspendAll()`, `resumeAll()`
4. Track task cycle counters

**Example Task**:
```cpp
void energyTask(void* params) {
    TickType_t lastWake = xTaskGetTickCount();
    while (true) {
        EnergyMeter::getInstance().update();
        MeterData data = EnergyMeter::getInstance().getSnapshot();
        EventBus::getInstance().publish(EventType::METER_DATA_UPDATED, &data);
        vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(500));
    }
}
```

#### 2. EventBus (src/core/EventBus.h/.cpp)
**Purpose**: Pub/sub for loose coupling

**Implementation Steps**:
1. Create FreeRTOS queue-based event system
2. Implement `publish(EventType, void* data)`
3. Implement `subscribe(EventType, callback)`
4. Support multiple subscribers per event type
5. Thread-safe, lock-free if possible

### Phase 8: Diagnostics Implementation

#### 1. Logger (src/diagnostics/Logger.h/.cpp)
**Purpose**: Leveled logging system

**Implementation Steps**:
1. Implement singleton with LogLevel filter
2. Implement `log(LogLevel, module, format, ...)` using va_list
3. Implement convenience: `error()`, `warn()`, `info()`, `debug()`
4. Create ring buffer (200 entries) for `getRecentLogs()`
5. Format: `[timestamp][LEVEL][MODULE] message`
6. Optional: Write to Serial and/or SPIFFS file

#### 2. SystemMonitor (src/diagnostics/SystemMonitor.h/.cpp)
**Purpose**: System health monitoring

**Implementation Steps**:
1. Implement `getFreeHeap()`, `getMinFreeHeap()`
2. Implement `getCPUFrequency()`, `getUptime()`
3. Implement `getChipInfo()` - model, revision, flash size
4. Track heap low-water-mark
5. Optional: CPU temperature if available

#### 3. WatchdogManager (src/diagnostics/WatchdogManager.h/.cpp)
**Purpose**: Hardware watchdog for reliability

**Implementation Steps**:
1. Implement `init(timeoutSeconds)` - configure ESP32 watchdog
2. Implement `feed()` - reset timer
3. Called by DiagnosticsTask every 5 seconds
4. Auto-reboot if not fed within timeout

---

## VB.NET Application Implementation Guide

### Phase 7: VB.NET Core Library

#### 1. Complete Models
**Files to Create**:
- `Models/CalibrationData.vb` - Calibration data structure
- `Models/MeterConfig.vb` - Configuration model
- `Models/LogEntry.vb` - Log entry model

#### 2. Communication Layer
**Files to Create**:
- `Communication/TCPTransport.vb`:
  - Implement IMeterConnection
  - Async TcpClient with auto-reconnect
  - Parse incoming data line-by-line
  - Raise DataReceived events
  
- `Communication/SerialTransport.vb`:
  - Implement IMeterConnection
  - SerialPort with async reading
  - Parse incoming data
  
- `Communication/WebSocketTransport.vb`:
  - Implement IMeterConnection
  - WebSocket client for V2 protocol
  - JSON data reception

- `Communication/ProtocolV1Parser.vb`:
  - Use MeterSnapshot.FromTagValueMap()
  - Already implemented in MeterSnapshot!

- `Communication/ProtocolV2Parser.vb`:
  - Use MeterSnapshot.FromJson()
  - Already implemented in MeterSnapshot!

- `Communication/CommandSender.vb`:
  - Send command, wait for ACK
  - Retry up to 3 times with timeout
  - Return response or throw exception

#### 3. Services Layer
**Files to Create**:
- `Services/MeterDataService.vb`:
  - Subscribe to IMeterConnection.DataReceived
  - Parse data via ProtocolParser
  - Maintain history (last 1000 snapshots)
  - Raise DataUpdated event for UI binding
  
- `Services/CalibrationService.vb`:
  - Step-by-step calibration workflow
  - Send Code_Cal1-4 commands
  - Verify ACK responses
  - Apply calibration to meter

- `Services/ConfigurationService.vb`:
  - Read config via GET /api/config
  - Write config via POST /api/config
  - Serialize/deserialize JSON

- `Services/DataLoggerService.vb`:
  - CSV export with headers
  - JSON export
  - Configurable interval
  - Start/stop logging

#### 4. Helpers
- `Helpers/FloatConverter.vb`:
  - IEEE754 ↔ Modbus register conversion
  - `FloatToRegisters(value)` → [high, low]
  - `RegistersToFloat([high, low])` → value

### Phase 8: VB.NET Application UI

#### 1. ViewModels
Implement MVVM pattern with INotifyPropertyChanged:
- `DashboardViewModel` - Current readings, connection status
- `CalibrationViewModel` - Calibration wizard state
- `SettingsViewModel` - Configuration editor
- `DataLogViewModel` - Logging controls

#### 2. Views (WinForms)
- `MainDashboard` - MDI parent with menu/toolbar
- `LiveMonitorView` - Real-time gauges (use custom controls)
- `TrendChartView` - Historical charts (use MSChart or LiveCharts)
- `PhaseDetailView` - Detailed phase data
- `EnergyView` - Energy accumulator display
- `CalibrationWizard` - Step-by-step wizard
- `SettingsView` - TabControl with settings pages
- `DataLogView` - DataGridView with export buttons
- `ConnectionView` - Connection setup form

#### 3. Custom Controls
- `PhaseGauge` - Circular gauge for voltage/current
- `PowerBar` - Bar chart for power
- `ConnectionIndicator` - LED-style status indicator
- `RegisterEditor` - Hex editor for calibration registers

---

## Web Dashboard Implementation

### Phase 9: Web Assets

#### 1. index.html (data/index.html)
```html
<!DOCTYPE html>
<html>
<head>
    <title>SM-GE3222M Dashboard</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="header">
        <h1>SM-GE3222M Energy Monitor</h1>
        <span id="status">Disconnected</span>
    </div>
    <div class="phases">
        <div class="phase" id="phaseA">
            <h2>Phase A</h2>
            <div class="gauge" id="gaugeA"></div>
            <div class="readings">
                <span>Voltage: <span id="voltA">0</span> V</span>
                <span>Current: <span id="currA">0</span> A</span>
                <span>Power: <span id="powerA">0</span> W</span>
            </div>
        </div>
        <!-- Repeat for Phase B, C -->
    </div>
    <script src="dashboard.js"></script>
</body>
</html>
```

#### 2. dashboard.js (data/dashboard.js)
```javascript
// WebSocket connection
let ws = null;

function connect() {
    ws = new WebSocket('ws://' + location.host + '/ws');
    ws.onopen = () => {
        document.getElementById('status').textContent = 'Connected';
    };
    ws.onmessage = (event) => {
        const data = JSON.parse(event.data);
        updateDashboard(data);
    };
    ws.onclose = () => {
        document.getElementById('status').textContent = 'Disconnected';
        setTimeout(connect, 5000);  // Reconnect after 5s
    };
}

function updateDashboard(data) {
    // Update Phase A
    document.getElementById('voltA').textContent = data.phaseA.voltageRMS.toFixed(1);
    document.getElementById('currA').textContent = data.phaseA.currentRMS.toFixed(2);
    document.getElementById('powerA').textContent = data.phaseA.activePower.toFixed(0);
    // ... Update Phase B, C, Totals
}

connect();
```

#### 3. style.css (data/style.css)
Modern, responsive CSS with:
- Grid layout for phases
- Circular gauges (SVG or Canvas)
- Color coding (green=good, yellow=warning, red=error)
- Mobile-responsive

---

## Testing Strategy

### 1. Unit Tests
- Protocol parsers (V1 Tag:Value, V2 JSON)
- Float converters
- Calibration calculations

### 2. Integration Tests
- SPI communication with ATM90E36 (loopback test)
- WiFi connection
- TCP/Modbus communication
- NVS read/write

### 3. System Tests
- Full boot sequence
- Energy measurement accuracy
- Data logging
- OTA updates
- Web dashboard functionality

### 4. Compatibility Tests
- V1.0 firmware with V2.0 desktop app
- V2.0 firmware with V1.0 desktop app (via TCP port 8088)

---

## Build and Deployment

### ESP32 Firmware
```bash
# Build
pio run

# Upload via USB
pio run -t upload

# Upload via OTA (after initial flash)
pio run -t upload --upload-port 192.168.1.100
```

### VB.NET Application
```bash
# Restore packages
dotnet restore

# Build
dotnet build

# Publish (Release)
dotnet publish -c Release
```

---

## Troubleshooting Implementation

### Common Issues

1. **SPI Communication Fails**
   - Check byte swapping (MSB/LSB)
   - Verify timing delays (10µs, 4µs)
   - Check CS pin control

2. **Calibration Checksums Fail**
   - Verify checksum calculation algorithm from V1.0
   - Check register write order
   - Verify magic values (0x5678, 0x4741)

3. **Modbus Not Working**
   - Test IEEE754 float conversion separately
   - Verify register addresses match ModbusMap.h
   - Check serial port settings (9600, 8N1)

4. **WiFi Won't Connect**
   - Check SSID/password
   - Verify signal strength
   - Try AP mode first for configuration

5. **Task Stack Overflow**
   - Increase stack size in TaskManager
   - Check for large local variables
   - Use heap allocation for large buffers

---

## Performance Optimization

1. **Memory**:
   - Use `const` and `PROGMEM` for constants
   - Minimize string copies
   - Reuse buffers

2. **CPU**:
   - Batch SPI reads (readAll() instead of individual getters)
   - Use DMA for SPI if available
   - Optimize task priorities

3. **Network**:
   - Limit WebSocket updates to 1Hz
   - Use JSON minification
   - Implement backpressure

---

## Next Steps

1. Start with HAL layer (SPIBus, I2CBus, GPIOManager)
2. Implement ATM90E36Driver (port from V1.0)
3. Test energy reading before proceeding
4. Implement storage and network layers
5. Add communication protocols one at a time
6. Test each protocol independently
7. Implement VB.NET transport layer
8. Create basic UI
9. Test end-to-end communication
10. Add advanced features

---

## Support and Resources

- **V1.0 Reference Code**: `Firmware_Released/SM_GE3222M V1.0/`
- **ATM90E36 Datasheet**: `Datasheets and Information/`
- **ARCHITECTURE.md**: Complete architectural documentation
- **README.md**: User guide

For questions or issues, refer to the V1.0 implementation for guidance on the electrical/protocol specifics.
