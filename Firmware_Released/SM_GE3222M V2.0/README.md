# SM-GE3222M Smart Energy Monitor - V2.0 Architecture Modernization

## Project Overview

This directory contains the **V2.0 modernization** of the SM-GE3222M Smart Energy Monitor system, representing a complete architectural upgrade from the V1.0 monolithic codebase to a production-grade, layered, maintainable system.

**Status:** 🚧 Architecture & Foundation Complete - Implementation In Progress

---

## What's Included

### ✅ Complete (Foundation)

1. **Architecture Documentation** (`ARCHITECTURE.md`)
   - Complete system architecture diagrams
   - Data flow architecture
   - FreeRTOS task architecture
   - Boot sequence specification
   - TCP protocol specification (V1.0 compatible)
   - ATM90E36 register programming sequence
   - Configuration management approach
   - Error handling architecture
   - Migration guide from V1.0

2. **Header Files** (`include/`)
   - `Version.h` - Firmware version and build information
   - `PinMap.h` - Complete GPIO pin assignments from V1.0 hardware
   - `RegisterMap.h` - Full ATM90E36 register map
   - `ErrorCodes.h` - Centralized error code enumeration
   - `GlobalTypes.h` - Data structures (PhaseData, MeterData, WiFiConfig, etc.)

3. **Project Configuration**
   - `platformio.ini` - PlatformIO build configuration with all dependencies

4. **Directory Structure**
   - All directories created and organized per specification
   - Follows modern embedded C++ best practices

### 🔨 To Be Implemented

The architecture is complete, and the following components need implementation following the patterns established in `ARCHITECTURE.md`:

#### Core Layer (`src/core/`)
- `ErrorHandler.cpp/.h` - Centralized error tracking with ring buffer
- `TaskManager.cpp/.h` - FreeRTOS task creation and management
- `WatchdogManager.cpp/.h` - Hardware + software watchdog

#### HAL Layer (`src/hal/`)
- `SPIBus.cpp/.h` - SPI bus manager with mutex
- `I2CBus.cpp/.h` - I2C bus manager with device scanner
- `GPIOManager.cpp/.h` - LED/button/relay control
- `NetworkManager.cpp/.h` - Unified WiFi + Ethernet management

#### Energy Layer (`src/energy/`)
- `ATM90E36Driver.cpp/.h` - Port from V1.0 `EnergyATM90E36.cpp` (50KB)
- `EnergyMeter.cpp/.h` - High-level measurement interface
- `EnergyAccumulator.cpp/.h` - NVS-persisted energy tracking

#### Communication Layer (`src/comm/`)
- `TCPDataServer.cpp/.h` - V1.0 protocol compatible TCP server
- `WebServer.cpp/.h` - REST API + WebSocket
- `ModbusServer.cpp/.h` - Port from V1.0 `Modbus_Serial.cpp`
- `MQTTPublisher.cpp/.h` - MQTT integration

#### Storage Layer (`src/storage/`)
- `ConfigManager.cpp/.h` - JSON-based configuration (replaces EEPROM approach)
- `SPIFFSManager.cpp/.h` - SPIFFS filesystem management
- `EEPROMBridge.cpp/.h` - External I2C EEPROM for factory calibration

#### Diagnostics Layer (`src/diagnostics/`)
- `Logger.cpp/.h` - Multi-level logging with ring buffer
- `SystemMonitor.cpp/.h` - Health checks and monitoring
- `DiagnosticReport.cpp/.h` - JSON diagnostic report generation

#### Application Entry (`src/`)
- `main.cpp` - Entry point with boot sequence from ARCHITECTURE.md

#### Web Interface (`data/`)
- `index.html` - Modern SPA dashboard
- `app.js` - JavaScript application logic
- `style.css` - Responsive CSS with theme support
- `config.json` - Default configuration template

#### Tests (`test/`)
- `test_energy_meter.cpp` - Energy measurement validation tests
- `test_config_manager.cpp` - Configuration serialization tests
- `test_protocol_parser.cpp` - TCP protocol parsing tests

---

## Key Improvements Over V1.0

| Aspect | V1.0 | V2.0 |
|--------|------|------|
| **Architecture** | Monolithic flat | Layered modular |
| **Code Files** | 24KB main .ino + 650+ #defines | Clean separation: Models, HAL, Drivers, Services |
| **Configuration** | 43KB `EEPROM_Defs.h` with sequential offsets | JSON on SPIFFS with schema validation |
| **Task Management** | Single `loop()` | FreeRTOS dual-core with priority scheduling |
| **Error Handling** | Ad-hoc `Serial.println()` | Centralized ErrorHandler with recovery actions |
| **Logging** | Serial only | Multi-destination (Serial/SPIFFS/WebSocket) with ring buffer |
| **TCP Protocol** | ✅ Text-based tags | ✅ **Fully backward compatible** |
| **Web Interface** | Basic embedded HTML | Modern SPA with WebSocket real-time updates |
| **REST API** | ❌ None | ✅ Full JSON API |
| **MQTT** | ❌ None | ✅ Configurable broker with TLS |
| **Modbus TCP** | ❌ None | ✅ Port 502 |
| **Memory Safety** | ❌ No checks | Heap monitoring with warnings/alerts |
| **Watchdog** | ❌ None | HW + SW dual watchdog (30s timeout) |
| **Unit Tests** | ❌ None | ✅ PlatformIO native tests |

---

## V1.0 Protocol Compatibility

**Critical:** V2.0 firmware maintains **100% backward compatibility** with V1.0 TCP protocol on port 8088.

- Existing V1.0 VB.NET desktop app works with V2.0 firmware
- Tag format unchanged: `TAG:VALUE\n`
- All V1.0 commands supported (Code_NewReq, Code_Cal1-4, ConfigSave, etc.)
- Handshake sequence identical (/SYNC/)

This allows gradual migration:
1. **Phase 1:** Upgrade firmware to V2.0 → Use V1.0 app
2. **Phase 2:** Upgrade app to V2.0 → Gain full feature set

---

## Quick Start (Implementation Guide)

### 1. Development Environment Setup

```bash
# Install PlatformIO
pip install platformio

# Clone repository
git clone https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor.git
cd SM-GE3222M-Smart-Energy-Monitor/Firmware_Released/SM_GE3222M\ V2.0/

# Install dependencies
pio lib install

# Build project
pio run

# Upload via USB
pio run --target upload

# Monitor serial output
pio device monitor
```

### 2. Implementation Priority

Follow this order to build incrementally:

**Week 1: Core Infrastructure**
1. Implement `ErrorHandler` (singleton, ring buffer)
2. Implement `Logger` (multi-level, multi-destination)
3. Create basic `main.cpp` with boot sequence phases 1-2
4. Test: Verify serial logging and error tracking

**Week 2: HAL Layer**
1. Implement `SPIBus` (mutex, retry logic)
2. Implement `I2CBus` (device scanner, retry)
3. Implement `GPIOManager` (LED control, MCP23017 interface)
4. Test: Verify SPI/I2C communication, LED blink

**Week 3: Energy IC Driver**
1. Port `ATM90E36Driver` from V1.0 `EnergyATM90E36.cpp`
   - CommEnergyIC() with mutex
   - Full init sequence with checksum validation
   - All measurement getters
2. Implement `EnergyMeter` (moving average, validation)
3. Test: Verify register reads, calibration sequence

**Week 4: Network & Storage**
1. Implement `NetworkManager` (WiFi STA/AP modes)
2. Implement `SPIFFSManager` + `ConfigManager` (JSON config)
3. Test: WiFi connection, config load/save

**Week 5: Communication Protocols**
1. Implement `TCPDataServer` (V1.0 protocol, port 8088)
2. Port `ModbusServer` from V1.0 `Modbus_Serial.cpp`
3. Test: Connect with V1.0 VB.NET app, verify data tags

**Week 6: Advanced Features**
1. Implement `WebServer` (REST API + WebSocket)
2. Implement `MQTTPublisher`
3. Create web interface (HTML/JS/CSS)
4. Test: Full system integration

**Week 7: Tasks & Diagnostics**
1. Implement `TaskManager` (EnergyTask, NetworkTask, DiagTask)
2. Implement `WatchdogManager`
3. Implement `SystemMonitor` + `DiagnosticReport`
4. Test: Long-duration stability (24+ hours)

**Week 8: Polish & Testing**
1. Write unit tests
2. Code review and refactoring
3. Performance optimization
4. Documentation updates

### 3. Porting from V1.0

When porting V1.0 code, follow these patterns:

**ATM90E36 Driver:**
```cpp
// V1.0: EnergyATM90E36.cpp
unsigned short ATM90E36::CommEnergyIC(unsigned char RW, unsigned short address, unsigned short val) {
    // Original SPI communication code...
}

// V2.0: ATM90E36Driver.cpp
uint16_t ATM90E36Driver::commEnergyIC(bool read, uint16_t address, uint16_t value) {
    // Wrap with mutex and retry logic
    if (xSemaphoreTake(_spiMutex, pdMS_TO_TICKS(100))) {
        for (int retry = 0; retry < 3; retry++) {
            uint16_t result = _spiTransaction(read, address, value);
            if (result != 0xFFFF || retry == 2) {
                xSemaphoreGive(_spiMutex);
                return result;
            }
            delay(10);
        }
        xSemaphoreGive(_spiMutex);
    }
    
    errorHandler.reportError(ErrorCode::ATM_READ_TIMEOUT, ...);
    return 0xFFFF;
}
```

**Configuration Migration:**
```cpp
// V1.0: EEPROM_Defs.h (AVOID THIS!)
#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_LENGTH 32
// ...650+ more lines

// V2.0: ConfigManager.cpp (USE THIS!)
String ConfigManager::serializeToJSON() {
    DynamicJsonDocument doc(4096);
    
    doc["wifi"]["ssid"] = _wifiConfig.ssid;
    doc["wifi"]["password"] = _wifiConfig.password;
    // ...
    
    String output;
    serializeJsonPretty(doc, output);
    return output;
}
```

### 4. Testing Checklist

- [ ] Boot sequence completes in <30 seconds
- [ ] ATM90E36 initialization successful (checksums validated)
- [ ] WiFi connects to network or falls back to AP mode
- [ ] TCP server accepts connections on port 8088
- [ ] V1.0 VB.NET app can connect and receive data
- [ ] All data tags present and formatted correctly
- [ ] Web interface accessible at http://ge3222m.local
- [ ] REST API endpoints respond correctly
- [ ] MQTT publishes if enabled
- [ ] Modbus RTU/TCP responds to queries
- [ ] Configuration persists across reboots
- [ ] OTA updates work correctly
- [ ] Watchdog resets on hang
- [ ] Memory usage stays below 200KB heap
- [ ] No memory leaks over 24+ hours
- [ ] Error logging works correctly

---

## Reference V1.0 Files (DO NOT MODIFY!)

For porting reference, see:
- `../SM_GE3222M V1.0/SM_GE3222M V1.0.ino` - Main setup/loop
- `../SM_GE3222M V1.0/EnergyATM90E36.cpp/h` - ATM90E36 driver (50KB)
- `../SM_GE3222M V1.0/Modbus_Serial.cpp/h` - Modbus implementation
- `../SM_GE3222M V1.0/Modbus_Defs.h` - Modbus register map
- `../SM_GE3222M V1.0/Ethernet_Interface.ino` - TCP protocol implementation
- `../SM_GE3222M V1.0/Definitions.h` - Pin assignments and constants
- `../SM_GE3222M V1.0/EEPROM_Defs.h` - Configuration approach (to be replaced)

**Important:** V1.0 code must remain unchanged for reference and rollback purposes.

---

## VB.NET Desktop Application V2.0

See `../Meter GE3222M V2.0/ARCHITECTURE.md` for complete VB.NET refactoring specification.

**Key Features:**
- Async/await TCP client (no UI freezing)
- Tag-dictionary parsing (robust, position-independent)
- Modern MVVM-like architecture
- SQLite data logging
- Real-time alerts
- PDF/Excel export
- Multiple meter support (future)

---

## Documentation

- **`ARCHITECTURE.md`** - Complete firmware architecture specification
- **`../Meter GE3222M V2.0/ARCHITECTURE.md`** - VB.NET app architecture
- **In-code documentation** - All classes/methods documented with Doxygen comments

---

## Contributing

### Code Style
- **Naming:** PascalCase for classes, camelCase for methods/variables
- **Singletons:** Meyer's singleton pattern (static local instance)
- **Error Handling:** Always use ErrorHandler, never bare Serial.println()
- **Logging:** Use Logger with appropriate levels
- **Documentation:** Doxygen-style comments for all public methods

### Workflow
1. Create feature branch from `main`
2. Implement following ARCHITECTURE.md specification
3. Write unit tests for new functionality
4. Ensure `pio test` passes
5. Submit pull request with description

---

## Support

- **Issues:** GitHub issue tracker
- **Documentation:** This README + ARCHITECTURE.md
- **Email:** support@microcodeeng.com

---

## License

- **Hardware:** Open Source Hardware (OSHW)
- **Firmware:** MIT License
- **Desktop App:** MIT License

---

**Project Status:** Foundation Complete - Ready for Implementation  
**Version:** 2.0.0-alpha  
**Last Updated:** 2026-02-08  
**Maintainer:** Microcode Engineering
