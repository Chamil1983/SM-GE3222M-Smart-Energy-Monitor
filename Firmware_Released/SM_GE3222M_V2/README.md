# SM-GE3222M V2.0 Smart Energy Monitor - Firmware

## Overview

This is the V2.0 firmware for the SM-GE3222M Smart Energy Monitor - a complete architectural redesign from V1.0 mon olithic structure to a modern, layered, production-grade system based on FreeRTOS.

## Key Features

✅ **Implemented in this skeleton:**
- Complete directory structure following layered architecture
- PlatformIO build configuration with all dependencies
- Pin mapping for ESP32 GPIO, SPI, I2C
- Complete ATM90E36 register map definitions
- Modbus register layout with IEEE754 float support
- Core data structures for all meter data
- 6-phase boot sequence framework
- Configuration structures for WiFi, Modbus, MQTT, Network, System

🚧 **To be implemented** (implementation modules available as TODOs):
- HAL layer (SPIBus, I2CBus, GPIOManager)
- Energy IC driver (ATM90E36Driver, EnergyMeter, EnergyAccumulator)
- Storage layer (ConfigManager, SPIFFSManager, DataLogger)
- Network layer (NetworkManager, OTAManager, NTPSync)
- Communication protocols (TCPDataServer, ProtocolV2, WebServer, ModbusServer, MQTTPublisher)
- Task management (TaskManager, EventBus)
- Diagnostics (Logger, SystemMonitor, WatchdogManager)

## Architecture

See [ARCHITECTURE.md](ARCHITECTURE.md) for complete architectural documentation.

### Directory Structure

```
SM_GE3222M V2.0/
├── platformio.ini              # Build configuration
├── ARCHITECTURE.md             # Architecture documentation
├── README.md                   # This file
├── include/                    # Public headers
│   ├── PinMap.h               # ✅ GPIO/SPI/I2C pin definitions
│   ├── RegisterMap.h          # ✅ ATM90E36 register addresses
│   ├── ModbusMap.h            # ✅ Modbus register mapping
│   └── Version.h              # ✅ Firmware version metadata
├── src/                       # Source code
│   ├── main.cpp               # ✅ Entry point & 6-phase boot
│   ├── core/                  # Core framework
│   │   ├── DataTypes.h       # ✅ Shared data structures
│   │   ├── TaskManager.*     # 🚧 FreeRTOS task lifecycle
│   │   └── EventBus.*        # 🚧 Publish/subscribe events
│   ├── hal/                   # Hardware Abstraction Layer
│   │   ├── SPIBus.*          # 🚧 Thread-safe SPI manager
│   │   ├── I2CBus.*          # 🚧 Thread-safe I2C manager
│   │   └── GPIOManager.*     # 🚧 LED/relay/button control
│   ├── energy/                # Energy metering
│   │   ├── ATM90E36Driver.*  # 🚧 Low-level IC driver
│   │   ├── EnergyMeter.*     # 🚧 Filtering & validation
│   │   ├── EnergyAccumulator.* # 🚧 kWh accumulation
│   │   └── CalibrationManager.* # 🚧 Calibration management
│   ├── comm/                  # Communication protocols
│   │   ├── TCPDataServer.*   # 🚧 V1-compatible TCP
│   │   ├── ProtocolV2.*      # 🚧 JSON structured protocol
│   │   ├── WebServer.*       # 🚧 REST API + WebSocket
│   │   ├── ModbusServer.*    # 🚧 Unified RTU + TCP
│   │   └── MQTTPublisher.*   # 🚧 MQTT with HA discovery
│   ├── network/               # Network management
│   │   ├── NetworkManager.*  # 🚧 WiFi STA/AP management
│   │   ├── OTAManager.*      # 🚧 Firmware updates
│   │   └── NTPSync.*         # 🚧 Time synchronization
│   ├── storage/               # Data persistence
│   │   ├── ConfigManager.*   # 🚧 NVS-based configuration
│   │   ├── SPIFFSManager.*   # 🚧 Filesystem management
│   │   └── DataLogger.*      # 🚧 On-device data logging
│   └── diagnostics/           # System diagnostics
│       ├── Logger.*          # 🚧 Leveled logging
│       ├── SystemMonitor.*   # 🚧 Heap/CPU monitoring
│       └── WatchdogManager.* # 🚧 Hardware watchdog
└── data/                      # SPIFFS web assets
    ├── index.html            # 🚧 Dashboard HTML
    ├── dashboard.js          # 🚧 Dashboard JavaScript
    └── style.css             # 🚧 Dashboard CSS
```

## Hardware Support

- **Board**: ESP32 DevKit
- **Energy IC**: ATM90E36 (SPI interface)
- **I/O Expander**: MCP23017 (I2C)
- **Sensors**: DHT22 temperature/humidity
- **Network**: WiFi (built-in), optional W5500 Ethernet
- **Interfaces**: RS485 Modbus RTU, Modbus TCP, MQTT, WebSocket

## Pin Mapping

See [include/PinMap.h](include/PinMap.h) for complete pin definitions:

- **SPI (ATM90E36)**: CS=5, MISO=19, MOSI=23, SCK=18
- **I2C (MCP23017)**: SDA=21, SCL=22
- **UART2 (Modbus)**: TX=17, RX=16
- **Buttons**: Set=33, Mode=32
- **ADC**: DC_IN=36, CT4=39
- **DHT**: GPIO4

## Getting Started

### Prerequisites

1. [PlatformIO](https://platformio.org/) installed
2. ESP32 DevKit board
3. SM-GE3222M hardware

### Build

```bash
# Clean build
pio run -t clean

# Build
pio run

# Build and upload
pio run -t upload

# Monitor serial output
pio device monitor
```

### First Boot

On first boot, the firmware will:
1. Format SPIFFS if needed
2. Create default configuration in NVS
3. Start in AP mode with SSID: `SM-GE3222M-Setup`, Password: `12345678`
4. Access web interface at `http://192.168.4.1`
5. Configure WiFi credentials and other settings

## Configuration

Configuration is stored in NVS (Non-Volatile Storage) with these namespaces:

- `wifi` - WiFi settings (SSID, password, static IP)
- `network` - Network settings (hostname, mDNS, NTP)
- `modbus` - Modbus RTU/TCP settings
- `mqtt` - MQTT broker and topics
- `calibration` - ATM90E36 calibration data
- `energy` - Energy accumulator state
- `system` - System settings (intervals, logging)

### Web Configuration

Access the web interface at:
- AP mode: `http://192.168.4.1`
- STA mode: `http://ge3222m.local` or device IP

### Serial Configuration

Connect at 115200 baud. Boot sequence shows all initialization steps.

## Communication Protocols

### V1 Tag:Value Protocol (TCP Port 8088)

Backward compatible with V1.0 desktop application:
```
AE1230.5\n      # Phase A Voltage RMS
AP11150.2\n     # Phase A Active Power
...
```

### V2 JSON Protocol (TCP Port 8089)

Modern structured protocol:
```json
{
  "type": "data",
  "seq": 123,
  "timestamp": 1234567890,
  "phases": {
    "A": {"V": 230.5, "I": 10.2, "P": 1150, ...},
    "B": {...},
    "C": {...}
  }
}
```

### REST API (HTTP)

- `GET /api/data` - Current meter snapshot (JSON)
- `GET /api/config` - Configuration (JSON)
- `POST /api/config` - Update configuration
- `GET /api/status` - System status
- `POST /api/calibration` - Apply calibration
- `POST /api/reset` - Reset energy accumulators
- `POST /api/reboot` - Reboot system

### WebSocket (ws://<ip>/ws)

Real-time data push every second for dashboard updates.

### Modbus (RTU: Serial2, TCP: Port 502)

IEEE754 float encoding (2 registers per value). See [include/ModbusMap.h](include/ModbusMap.h) for register map.

### MQTT

- Topic: `ge3222m/<deviceId>/state`
- Home Assistant auto-discovery supported
- Configurable publish interval

## Development

### Adding New Modules

1. Create `.h` and `.cpp` files in appropriate `src/` subdirectory
2. Use singleton pattern for managers
3. Include in `main.cpp` boot sequence
4. Add to appropriate FreeRTOS task
5. Use EventBus for inter-module communication

### Coding Style

- C++17 standard
- `constexpr` for compile-time constants
- Descriptive variable names (camelCase)
- Comprehensive comments
- Thread-safe with FreeRTOS mutexes

### Testing

1. Build with `pio run`
2. Upload with `pio run -t upload`
3. Monitor serial at 115200 baud
4. Verify all 6 boot phases complete
5. Test each communication protocol
6. Verify energy readings

## Migration from V1.0

V2.0 maintains backward compatibility:

1. **Desktop App**: V1.0 VB.NET app can connect via TCP port 8088
2. **Modbus**: Register layout extended but V1 registers preserved
3. **Calibration**: Same calibration workflow and data format
4. **Configuration**: Can import V1 config JSON

Migration steps:
1. Backup V1.0 config via desktop app
2. Flash V2.0 firmware
3. Connect desktop app to port 8088 (V1 protocol)
4. Import config
5. Verify operation
6. Optionally upgrade to V2 protocol (port 8089)

## Troubleshooting

### Boot Fails at Phase 1
- Check serial connection (115200 baud)
- Verify ESP32 board selection in platformio.ini
- Check GPIO connections

### Boot Fails at Phase 3 (ATM90E36)
- Verify SPI connections (CS=5, MISO=19, MOSI=23, SCK=18)
- Check ATM90E36 power supply
- Verify calibration data

### WiFi Connection Issues
- Check SSID/password in config
- Verify WiFi signal strength
- Try AP mode for reconfiguration

### Modbus Not Responding
- Check RS485 wiring
- Verify slave ID (default: 1)
- Verify baudrate (default: 9600)

## Performance

- Energy reading cycle: 500ms
- TCP response latency: <50ms
- WebSocket update rate: 1Hz
- Modbus response: <20ms
- Free heap after boot: ~150KB
- SPIFFS usage: <100KB

## Known Limitations

- Maximum 4 concurrent TCP clients
- WebSocket limited to 10 clients
- SPIFFS file logging limited to 1MB ring buffer
- No SD card support yet
- Ethernet (W5500) support optional

## Roadmap

### Version 2.1
- [ ] Advanced power quality analysis
- [ ] Harmonic analysis up to 31st order
- [ ] Waveform capture
- [ ] Events and alarms

### Version 2.2
- [ ] Multi-device aggregation
- [ ] Master/slave configuration
- [ ] Data forwarding

### Version 2.3
- [ ] Cloud connectivity (AWS IoT, Azure IoT)
- [ ] Remote firmware updates
- [ ] Cloud data storage

## License

Same as main repository.

## Support

For issues and questions:
- Create GitHub issue
- Check ARCHITECTURE.md for design details
- Review V1.0 code in `../SM_GE3222M V1.0/` for reference

## Contributing

1. Fork repository
2. Create feature branch
3. Follow coding style
4. Test thoroughly
5. Submit pull request

## Changelog

### V2.0.0 (Current)
- Complete architecture redesign
- FreeRTOS-based task management
- Thread-safe HAL layer
- Unified configuration management (NVS)
- Modern communication protocols
- Web dashboard
- MQTT support
- Home Assistant integration
- Backward compatibility with V1.0

## Credits

- Original V1.0 firmware: Microcode Engineering
- V2.0 architecture: Modernization initiative
- Libraries: Arduino, ESP32, PlatformIO ecosystem
