# V2.0 Project Structure

This document describes the complete directory structure for the V2.0 architecture.

```
SM_GE3222M V2.0/
│
├── platformio.ini              # PlatformIO project configuration
├── README.md                   # Project overview and quick start
├── ARCHITECTURE.md             # Complete architecture specification
├── .gitignore                  # Git ignore patterns
│
├── include/                    # Public header files
│   ├── Version.h              # Firmware version information
│   ├── PinMap.h               # GPIO pin assignments
│   ├── RegisterMap.h          # ATM90E36 register definitions
│   ├── ErrorCodes.h           # Error code enumeration
│   └── GlobalTypes.h          # Global data structures
│
├── src/                        # Source code
│   ├── main.cpp               # Application entry point
│   │
│   ├── core/                  # Core infrastructure
│   │   ├── ErrorHandler.h/cpp # Centralized error tracking
│   │   ├── TaskManager.h/cpp  # FreeRTOS task management
│   │   └── WatchdogManager.h/cpp # Hardware/software watchdog
│   │
│   ├── hal/                   # Hardware Abstraction Layer
│   │   ├── SPIBus.h/cpp       # SPI bus manager with mutex
│   │   ├── I2CBus.h/cpp       # I2C bus manager
│   │   ├── GPIOManager.h/cpp  # GPIO control (LED/button/relay)
│   │   └── NetworkManager.h/cpp # WiFi/Ethernet management
│   │
│   ├── energy/                # Energy measurement
│   │   ├── ATM90E36Driver.h/cpp # Low-level SPI driver
│   │   ├── EnergyMeter.h/cpp    # High-level measurement interface
│   │   └── EnergyAccumulator.h/cpp # Persistent energy tracking
│   │
│   ├── comm/                  # Communication protocols
│   │   ├── TCPDataServer.h/cpp  # TCP server (V1.0 compatible)
│   │   ├── WebServer.h/cpp      # REST API + WebSocket
│   │   ├── ModbusServer.h/cpp   # Modbus RTU/TCP
│   │   └── MQTTPublisher.h/cpp  # MQTT integration
│   │
│   ├── storage/               # Configuration and storage
│   │   ├── ConfigManager.h/cpp  # JSON configuration
│   │   ├── SPIFFSManager.h/cpp  # SPIFFS filesystem
│   │   └── EEPROMBridge.h/cpp   # External I2C EEPROM
│   │
│   └── diagnostics/           # Monitoring and logging
│       ├── Logger.h/cpp         # Multi-level logging
│       ├── SystemMonitor.h/cpp  # Health monitoring
│       └── DiagnosticReport.h/cpp # Diagnostic reports
│
├── data/                       # SPIFFS web files
│   ├── index.html             # Web dashboard
│   ├── app.js                 # JavaScript application
│   ├── style.css              # Stylesheet
│   └── config.json            # Default configuration
│
├── test/                       # Unit tests
│   ├── test_energy_meter.cpp  # Energy measurement tests
│   ├── test_config_manager.cpp # Configuration tests
│   └── test_protocol_parser.cpp # Protocol parsing tests
│
└── scripts/                    # Build and deployment scripts
    └── ota_upload.py          # OTA upload script
```

## Module Dependencies

```
main.cpp
  ├─> TaskManager
  │    ├─> EnergyTask
  │    ├─> NetworkTask
  │    ├─> DiagTask
  │    └─> OTATask
  │
  ├─> ErrorHandler
  ├─> WatchdogManager
  ├─> Logger
  │
  ├─> SPIBus
  │    └─> ATM90E36Driver
  │         └─> EnergyMeter
  │              └─> EnergyAccumulator
  │
  ├─> I2CBus
  │    ├─> GPIOManager (MCP23017)
  │    └─> EEPROMBridge
  │
  ├─> NetworkManager
  │    ├─> TCPDataServer
  │    ├─> WebServer
  │    ├─> ModbusServer
  │    └─> MQTTPublisher
  │
  ├─> SPIFFSManager
  │    └─> ConfigManager
  │
  └─> SystemMonitor
       └─> DiagnosticReport
```

## Implementation Status

### ✅ Complete
- [x] Directory structure created
- [x] Header files (Version, PinMap, RegisterMap, ErrorCodes, GlobalTypes)
- [x] ARCHITECTURE.md documentation
- [x] README.md with quick start
- [x] platformio.ini configuration
- [x] main.cpp skeleton with boot sequence
- [x] .gitignore file

### 🔨 To Be Implemented
- [ ] Core infrastructure (ErrorHandler, TaskManager, WatchdogManager)
- [ ] HAL layer (SPIBus, I2CBus, GPIOManager, NetworkManager)
- [ ] Energy measurement (ATM90E36Driver, EnergyMeter, EnergyAccumulator)
- [ ] Communication (TCPDataServer, WebServer, ModbusServer, MQTTPublisher)
- [ ] Storage (ConfigManager, SPIFFSManager, EEPROMBridge)
- [ ] Diagnostics (Logger, SystemMonitor, DiagnosticReport)
- [ ] Web interface (HTML/CSS/JS)
- [ ] Unit tests
- [ ] Build scripts

## Next Steps

1. **Implement Core Infrastructure** - Start with ErrorHandler and Logger
2. **Implement HAL Layer** - SPIBus and I2CBus are critical dependencies
3. **Port ATM90E36 Driver** - This is the largest piece from V1.0
4. **Implement Network Stack** - WiFi, TCP server for V1.0 compatibility
5. **Add Advanced Features** - Web server, MQTT, Modbus
6. **Create Web Interface** - Modern SPA dashboard
7. **Write Tests** - Unit tests for critical components
8. **Integration Testing** - End-to-end validation with hardware

## Coding Standards

- **Naming Conventions:**
  - Classes: PascalCase (e.g., `ErrorHandler`)
  - Methods: camelCase (e.g., `reportError`)
  - Variables: camelCase (e.g., `errorCount`)
  - Constants: UPPER_SNAKE_CASE (e.g., `MAX_RETRIES`)
  - Private members: underscore prefix (e.g., `_instance`)

- **Documentation:**
  - All public methods must have Doxygen comments
  - Complex algorithms require inline comments
  - File headers with brief description, author, date

- **Error Handling:**
  - Always use ErrorHandler for errors
  - Never use bare `Serial.println()` for errors
  - Check return values and handle failures

- **Resource Management:**
  - Use RAII principles
  - Mutexes for shared resources
  - Clean up in destructors

- **Testing:**
  - Write unit tests for new functionality
  - Test edge cases and error conditions
  - Integration tests for complete flows

## Reference Materials

- **ATM90E36 Datasheet** - Available in `/Documentation/`
- **ESP32 Technical Reference** - https://espressif.com/
- **V1.0 Source Code** - `../SM_GE3222M V1.0/` (DO NOT MODIFY)
- **TCP Protocol Spec** - See ARCHITECTURE.md section "TCP Data Protocol"
- **Pin Assignments** - See `include/PinMap.h`
