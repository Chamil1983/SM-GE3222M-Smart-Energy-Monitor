# SM-GE3222M V2.0 Architecture

## Overview
This is the V2.0 firmware for the SM-GE3222M Smart Energy Monitor, featuring a complete architectural redesign from the V1.0 monolithic structure to a modern, layered, production-grade system.

## Key Improvements Over V1.0
- **Thread-Safe Design**: FreeRTOS-based with proper task management and mutex protection
- **Layered Architecture**: Clear separation between HAL, drivers, services, and application layers
- **Unified Configuration**: NVS-based configuration replacing scattered EEPROM management
- **Modern Protocols**: JSON-based V2 protocol alongside backward-compatible V1 Tag:Value protocol
- **Robust Error Handling**: Comprehensive logging, watchdog, and system monitoring
- **Extended Connectivity**: WiFi + Ethernet + Modbus RTU/TCP + MQTT + WebSocket
- **Event-Driven**: Pub/sub event bus for loose coupling between components

## Directory Structure

Arduino IDE requires a flat file structure where all source files are in the same directory:

```
SM_GE3222M_V2/
├── ARCHITECTURE.md             # This file
├── README.md                   # Setup and usage guide
├── SM_GE3222M_V2.ino          # Main Arduino sketch file (entry point)
├── PinMap.h                   # GPIO/SPI/I2C pin definitions
├── RegisterMap.h              # ATM90E36 register addresses
├── ModbusMap.h                # Modbus register mapping
├── Version.h                  # Firmware version metadata
├── DataTypes.h                # Shared data structures
├── TaskManager.h              # FreeRTOS task lifecycle
├── TaskManager.cpp
├── EventBus.h                 # Publish/subscribe events
├── EventBus.cpp
├── SPIBus.h                   # Thread-safe SPI manager
├── SPIBus.cpp
├── I2CBus.h                   # Thread-safe I2C manager
├── I2CBus.cpp
├── GPIOManager.h              # LED/relay/button control
├── GPIOManager.cpp
├── ATM90E36Driver.h           # Low-level IC driver
├── ATM90E36Driver.cpp
├── EnergyMeter.h              # Filtering & validation
├── EnergyMeter.cpp
├── EnergyAccumulator.h        # kWh accumulation & persistence
├── EnergyAccumulator.cpp
├── CalibrationManager.h       # Calibration data management
├── CalibrationManager.cpp
├── TCPDataServer.h            # V1-compatible Tag:Value TCP
├── TCPDataServer.cpp
├── ProtocolV2.h               # JSON structured protocol
├── ProtocolV2.cpp
├── WebServerManager.h         # REST API + WebSocket
├── WebServerManager.cpp
├── ModbusServer.h             # Unified RTU + TCP Modbus
├── ModbusServer.cpp
├── MQTTPublisher.h            # MQTT with HA discovery
├── MQTTPublisher.cpp
├── NetworkManager.h           # WiFi STA/AP management
├── NetworkManager.cpp
├── OTAManager.h               # Firmware updates
├── OTAManager.cpp
├── NTPSync.h                  # Time synchronization
├── NTPSync.cpp
├── ConfigManager.h            # NVS-based configuration
├── ConfigManager.cpp
├── SPIFFSManager.h            # Filesystem management
├── SPIFFSManager.cpp
├── DataLogger.h               # On-device data logging
├── DataLogger.cpp
├── Logger.h                   # Leveled logging
├── Logger.cpp
├── SystemMonitor.h            # Heap/CPU/temp monitoring
├── SystemMonitor.cpp
├── WatchdogManager.h          # Hardware watchdog
├── WatchdogManager.cpp
└── data/                      # SPIFFS web assets
    ├── index.html
    ├── dashboard.js
    └── style.css
```

**Note:** Arduino IDE requires all `.h` and `.cpp` files to be in the same folder as the `.ino` file. The flat structure is mandatory for Arduino IDE compatibility.

## Component Layers

### Layer 1: Hardware Abstraction (HAL)
- **SPIBus**: Mutex-protected SPI with 200kHz, MODE3 for ATM90E36
- **I2CBus**: Mutex-protected I2C for MCP23017 and sensors
- **GPIOManager**: Centralized pin management for LEDs, relays, buttons

### Layer 2: Device Drivers
- **ATM90E36Driver**: Low-level register R/W, calibration sequence, checksum verification
- **NetworkManager**: WiFi connection, AP mode, auto-reconnect

### Layer 3: Services
- **EnergyMeter**: High-level energy reading with moving average filter
- **EnergyAccumulator**: Time-based kWh accumulation with NVS persistence
- **ConfigManager**: Unified configuration storage in NVS namespaces
- **MQTTPublisher**: Telemetry publishing with auto-reconnect

### Layer 4: Communication Protocols
- **TCPDataServer**: V1.0 backward-compatible Tag:Value ASCII protocol
- **ProtocolV2**: Modern JSON protocol with ACK/NACK and sequencing
- **WebServer**: REST API + WebSocket for real-time dashboard
- **ModbusServer**: IEEE754 float encoding, unified RTU+TCP

### Layer 5: Application
- **TaskManager**: 7 FreeRTOS tasks with core affinity
- **EventBus**: Decoupled event-driven architecture
- **SM_GE3222M_V2.ino**: Main sketch file with 6-phase boot sequence

## Boot Sequence

1. **Phase 1: HAL Initialization**
   - Logger, GPIO, SPI, I2C, SystemMonitor, Watchdog

2. **Phase 2: Storage**
   - SPIFFS mount, NVS init, load configuration

3. **Phase 3: Energy Metering**
   - ATM90E36 init with calibration, accumulator restore

4. **Phase 4: Network**
   - WiFi STA/AP, mDNS, NTP sync

5. **Phase 5: Communications**
   - TCP server, WebServer, Modbus, MQTT, OTA

6. **Phase 6: Task Launch**
   - Create all FreeRTOS tasks with proper priorities

## FreeRTOS Tasks

| Task | Core | Priority | Period | Stack | Purpose |
|------|------|----------|--------|-------|---------|
| EnergyTask | 1 | 5 | 500ms | 4096 | Read ATM90E36, update snapshot |
| AccumulatorTask | 1 | 4 | 1000ms | 3072 | Accumulate energy, persist every 60s |
| ModbusTask | 1 | 3 | 10ms | 4096 | Poll Modbus RTU & TCP |
| TCPServerTask | 0 | 3 | Event | 4096 | Handle TCP clients, send data |
| WebServerTask | 0 | 2 | 10ms | 4096 | WebSocket updates |
| MQTTTask | 0 | 2 | Config | 3072 | MQTT publishing |
| DiagnosticsTask | 0 | 1 | 5000ms | 2048 | Feed watchdog, monitor heap |

## Communication Protocols

### V1 Tag:Value Protocol (TCP Port 8088)
Backward compatible ASCII protocol:
```
AE1<value>\n      # Phase A Forward Active Energy
AP1<value>\n      # Phase A Active Power
...
```

### V2 JSON Protocol (TCP Port 8089)
Modern structured protocol with ACK/NACK:
```json
{
  "type": "data",
  "seq": 123,
  "timestamp": 1234567890,
  "phases": {
    "A": {"V": 230.5, "I": 10.2, ...},
    "B": {...},
    "C": {...}
  },
  "totals": {...},
  "energy": {...}
}
```

### REST API (HTTP)
- `GET /api/data` - Current meter snapshot
- `GET /api/config` - Configuration
- `POST /api/config` - Update configuration
- `GET /api/status` - System status
- `POST /api/calibration` - Apply calibration

### WebSocket (ws://<ip>/ws)
Real-time data push every 1s

### Modbus (RTU: Serial2, TCP: Port 502)
IEEE754 float encoding (2 registers per value)

### MQTT
- Topic: `ge3222m/<deviceId>/state`
- Home Assistant auto-discovery

## Data Flow

```
ATM90E36 IC
    ↓ (SPI, 500ms)
ATM90E36Driver
    ↓
EnergyMeter (filtering)
    ↓
EnergyAccumulator (kWh)
    ↓ (EventBus: METER_DATA_UPDATED)
    ├→ TCPDataServer (Tag:Value)
    ├→ WebServer (WebSocket)
    ├→ ModbusServer (registers)
    └→ MQTTPublisher (JSON)
```

## Configuration Management

All config stored in NVS with namespaces:
- `wifi` - SSID, password, IP config
- `network` - Hostname, mDNS, NTP
- `modbus` - RTU/TCP settings
- `mqtt` - Broker, topics, credentials
- `calibration` - ATM90E36 cal registers
- `energy` - Accumulator state
- `system` - Boot count, flags

## Error Handling

- **Logger**: 5 levels (ERROR, WARN, INFO, DEBUG, TRACE)
- **Watchdog**: 30s timeout, auto-reboot on hang
- **SystemMonitor**: Heap low-water-mark alerts
- **EventBus**: ERROR_OCCURRED events

## Memory Management

- **Heap**: ~150KB free after init
- **Stack**: Per-task allocation (2-4KB)
- **PSRAM**: Optional for large buffers
- **Flash**: 1.5MB program, 1.5MB SPIFFS

## Backward Compatibility

V2.0 maintains full backward compatibility with V1.0:
- V1 Tag:Value protocol on port 8088
- Same calibration workflow
- Compatible Modbus register layout (extended)
- VB.NET app can connect to both V1 and V2 firmware

## Performance

- Energy reading: 500ms cycle
- TCP response: <50ms
- WebSocket latency: <100ms
- Modbus response: <20ms
- Web dashboard: 60fps animation

## Security Considerations

- WiFi WPA2/WPA3 support
- OTA password protection
- Web server authentication (optional)
- MQTT TLS support (optional)

## Future Extensions

- Phase 2.1: Advanced power quality analysis
- Phase 2.2: Multi-device aggregation
- Phase 2.3: Cloud connectivity
- Phase 2.4: Predictive maintenance

## Migration from V1.0

1. Backup V1.0 config via VB.NET app
2. Flash V2.0 firmware
3. Use V1 protocol initially
4. Upgrade to V2 protocol when ready
5. Update VB.NET app to V2.0

## Development

### Build System
- **Platform**: Arduino IDE 2.0+ (or Arduino IDE 1.8.x)
- **Board Support**: ESP32 by Espressif Systems
- **Build Process**: Arduino IDE automatically compiles all `.cpp` files in sketch folder
- **Board Config**: ESP32 Dev Module, 4MB Flash, Default partition scheme
- **Upload Method**: Serial (UART0) at 921600 baud or OTA

### Arduino IDE Compatibility Requirements
1. **Flat File Structure**: All `.h` and `.cpp` files must be in same directory as `.ino` file
2. **Header Guards**: Use `#pragma once` (recommended) or traditional include guards
3. **Include Paths**: Direct includes without path prefixes (e.g., `#include "PinMap.h"`)
4. **Library Dependencies**: All libraries must be installed via Library Manager or manually
5. **SPIFFS Upload**: Requires ESP32 Sketch Data Upload plugin

### Compilation Notes
- Arduino IDE processes `.ino` file first, then compiles all `.cpp` files
- Function prototypes in `.ino` file are automatically generated
- Libraries are linked in order of inclusion
- ESP32 Arduino Core provides FreeRTOS support natively

## License

Same as main repository
