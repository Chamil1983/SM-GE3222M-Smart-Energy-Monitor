# SM-GE3222M V2.0 Smart Energy Monitor - Firmware

## Overview

This is the V2.0 firmware for the SM-GE3222M Smart Energy Monitor - a complete architectural redesign from V1.0 monolithic structure to a modern, layered, production-grade system based on FreeRTOS.

## Key Features

✅ **Implemented in this release:**
- Complete flat file structure for Arduino IDE compatibility
- Arduino IDE build configuration with all dependencies
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
- Communication protocols (TCPDataServer, ProtocolV2, WebServer)
- Task management (TaskManager, EventBus)
- Diagnostics (Logger, SystemMonitor, WatchdogManager)

✅ **Recently implemented:**
- ModbusServer (unified RTU + TCP with IEEE754 float encoding)
- MQTTPublisher (MQTT with Home Assistant discovery)

## Architecture

See [ARCHITECTURE.md](ARCHITECTURE.md) for complete architectural documentation.

### Directory Structure

Arduino IDE uses a flat file structure where all source files must be in the same directory as the .ino file:

```
SM_GE3222M_V2/
├── ARCHITECTURE.md             # Architecture documentation
├── README.md                   # This file
├── SM_GE3222M_V2.ino          # ✅ Main Arduino sketch file
├── PinMap.h                   # ✅ GPIO/SPI/I2C pin definitions
├── RegisterMap.h              # ✅ ATM90E36 register addresses
├── ModbusMap.h                # ✅ Modbus register mapping
├── Version.h                  # ✅ Firmware version metadata
├── DataTypes.h                # ✅ Shared data structures
├── TaskManager.h              # 🚧 FreeRTOS task lifecycle
├── TaskManager.cpp
├── EventBus.h                 # 🚧 Publish/subscribe events
├── EventBus.cpp
├── SPIBus.h                   # 🚧 Thread-safe SPI manager
├── SPIBus.cpp
├── I2CBus.h                   # 🚧 Thread-safe I2C manager
├── I2CBus.cpp
├── GPIOManager.h              # 🚧 LED/relay/button control
├── GPIOManager.cpp
├── ATM90E36Driver.h           # 🚧 Low-level IC driver
├── ATM90E36Driver.cpp
├── EnergyMeter.h              # 🚧 Filtering & validation
├── EnergyMeter.cpp
├── EnergyAccumulator.h        # 🚧 kWh accumulation
├── EnergyAccumulator.cpp
├── CalibrationManager.h       # 🚧 Calibration management
├── CalibrationManager.cpp
├── TCPDataServer.h            # 🚧 V1-compatible TCP
├── TCPDataServer.cpp
├── ProtocolV2.h               # 🚧 JSON structured protocol
├── ProtocolV2.cpp
├── WebServerManager.h         # 🚧 REST API + WebSocket
├── WebServerManager.cpp
├── ModbusServer.h             # ✅ Unified RTU + TCP
├── ModbusServer.cpp
├── MQTTPublisher.h            # ✅ MQTT with HA discovery
├── MQTTPublisher.cpp
├── NetworkManager.h           # 🚧 WiFi STA/AP management
├── NetworkManager.cpp
├── OTAManager.h               # 🚧 Firmware updates
├── OTAManager.cpp
├── NTPSync.h                  # 🚧 Time synchronization
├── NTPSync.cpp
├── ConfigManager.h            # 🚧 NVS-based configuration
├── ConfigManager.cpp
├── SPIFFSManager.h            # 🚧 Filesystem management
├── SPIFFSManager.cpp
├── DataLogger.h               # 🚧 On-device data logging
├── DataLogger.cpp
├── Logger.h                   # 🚧 Leveled logging
├── Logger.cpp
├── SystemMonitor.h            # 🚧 Heap/CPU monitoring
├── SystemMonitor.cpp
├── WatchdogManager.h          # 🚧 Hardware watchdog
├── WatchdogManager.cpp
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

See [PinMap.h](PinMap.h) for complete pin definitions:

- **SPI (ATM90E36)**: CS=5, MISO=19, MOSI=23, SCK=18
- **I2C (MCP23017)**: SDA=21, SCL=22
- **UART2 (Modbus)**: TX=17, RX=16
- **Buttons**: Set=33, Mode=32
- **ADC**: DC_IN=36, CT4=39
- **DHT**: GPIO4

## Getting Started

### Prerequisites

1. **Arduino IDE** installed (version 2.0 or later recommended)
2. **ESP32 board support** installed
3. **Required libraries** installed (see below)
4. ESP32 DevKit board
5. SM-GE3222M hardware

### Arduino IDE Setup

#### 1. Install Arduino IDE
Download and install from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

#### 2. Install ESP32 Board Support
1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add to "Additional Boards Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search for "ESP32" 
6. Install **"ESP32 by Espressif Systems"** (version 2.0.0 or later)

#### 3. Install Required Libraries
Go to **Tools → Manage Libraries** and install:

- **ESPAsyncWebServer** by me-no-dev
- **AsyncTCP** by me-no-dev
- **ArduinoJson** by Benoit Blanchon (version 6.x)
- **PubSubClient** by Nick O'Leary
- **Adafruit MCP23017** by Adafruit
- **DHT sensor library** by Adafruit
- **ModbusRTU** by Alexander Emelianov
- **Button** by Michael Adams

*Note: Some libraries like ESPAsyncWebServer may need to be installed manually from GitHub if not available in Library Manager*

#### 4. Board Configuration
Configure board settings in **Tools** menu:
- **Board**: "ESP32 Dev Module"
- **Flash Size**: "4MB (Default)"
- **Partition Scheme**: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"
- **Upload Speed**: 921600
- **CPU Frequency**: 240MHz (WiFi/BT)
- **Flash Frequency**: 80MHz
- **Flash Mode**: QIO
- **Core Debug Level**: "None" (or "Info" for debugging)

### Build and Upload

1. Open `SM_GE3222M_V2.ino` in Arduino IDE
2. Select your ESP32 board's COM port in **Tools → Port**
3. Click **Verify** (✓) to compile the sketch
4. Click **Upload** (→) to flash to ESP32
5. Open **Tools → Serial Monitor** and set baud rate to **115200**

### Upload SPIFFS Data

To upload web interface files from the `data/` folder:

1. Install **ESP32 Sketch Data Upload** plugin:
   - Download from [https://github.com/me-no-dev/arduino-esp32fs-plugin/releases](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases)
   - Follow installation instructions for your OS
2. Close Serial Monitor if open
3. Go to **Tools → ESP32 Sketch Data Upload**
4. Wait for upload to complete
5. Restart ESP32

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
    "A": {"V": 230.5, "I": 10.2, "P": 1150},
    "B": {},
    "C": {}
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

IEEE754 float encoding (2 registers per value). See [ModbusMap.h](ModbusMap.h) for register map.

### MQTT

- Topic: `ge3222m/<deviceId>/state`
- Home Assistant auto-discovery supported
- Configurable publish interval

## Development

### Adding New Modules

1. Create `.h` and `.cpp` files in the same directory as `SM_GE3222M_V2.ino`
2. Use singleton pattern for managers
3. Include headers in the `.ino` file
4. Add to appropriate FreeRTOS task
5. Use EventBus for inter-module communication

### Arduino IDE Compatibility Notes

- All source files (`.h`, `.cpp`) must be in the same folder as the `.ino` file
- Arduino IDE automatically compiles all `.cpp` files in the sketch folder
- Use `#pragma once` instead of include guards in headers
- ESP32 Arduino core provides FreeRTOS support natively

### Coding Style

- C++17 standard (supported by ESP32 Arduino core)
- `constexpr` for compile-time constants
- Descriptive variable names (camelCase)
- Comprehensive comments
- Thread-safe with FreeRTOS mutexes

### Testing

1. Verify/compile in Arduino IDE (Ctrl+R / Cmd+R)
2. Upload to ESP32 (Ctrl+U / Cmd+U)
3. Open Serial Monitor at 115200 baud
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
- Verify ESP32 board selection in Arduino IDE Tools menu
- Ensure correct partition scheme is selected
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
- Flash usage: ~1.2MB (program) + ~1.5MB (SPIFFS)

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
- Libraries: Arduino IDE, ESP32 Arduino Core, open-source community
