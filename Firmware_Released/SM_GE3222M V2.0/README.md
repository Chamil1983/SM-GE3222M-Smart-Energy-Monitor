# SM-GE3222M Smart Energy Monitor V2.0 Firmware

## Overview

Production-grade firmware for the SM-GE3222M 3-Phase Smart Energy Monitor with ATM90E36 IC. Complete architecture overhaul from V1.0 with modern design patterns, comprehensive error handling, and industrial-quality code.

## Hardware Support

- **MCU**: ESP32 (WROOM-32 module)
- **Energy IC**: ATM90E36 (3-phase energy metering IC via SPI)
- **I/O Expander**: MCP23017 (I2C address 0x20)
- **Display**: LCD 20×4 (I2C address 0x27)
- **Ethernet**: W5500 (SPI)
- **RS-485**: MAX485 (UART2)
- **Sensor**: DHT22 (GPIO4)
- **Relays**: 2× via MCP23017
- **LEDs**: WiFi, RUN, FLT, MODBUS via MCP23017

## Architecture

### Core Subsystems
- **SystemManager**: Main orchestrator with state machine
- **TaskScheduler**: Non-blocking cooperative scheduler with priorities
- **ErrorHandler**: Centralized error management with recovery
- **TimerManager**: Hardware timer abstraction with watchdog

### Hardware Abstraction Layer (HAL)
- **BoardConfig**: Pin maps and hardware identification
- **SPIBus**: Thread-safe SPI with mutex for ATM90E36/W5500
- **I2CBus**: I2C management with scanning and timeout
- **GPIOManager**: LED patterns, button state machine, relay control

### Energy Subsystem
- **ATM90E36Driver**: Low-level driver (86 methods, 100% V1.0 parity)
- **EnergyMeter**: High-level measurement API with JSON export
- **Calibration**: Configuration management with NVS persistence
- **PowerQuality**: Sag/swell/THD/imbalance detection
- **EnergyAccumulator**: Long-term tracking with daily/weekly/monthly stats

### Storage Subsystem
- **NVStorage**: ESP32 NVS wrapper with CRC32
- **ConfigManager**: JSON config with validation
- **DataLogger**: Circular CSV logging with auto-rotation
- **SPIFFSManager**: File system management

### Communication Stack
- **WiFiManager**: STA/AP modes with exponential backoff reconnect
- **EthernetManager**: W5500 with DHCP/static IP
- **WebServerManager**: AsyncWebServer + WebSocket + REST API (9 endpoints)
- **MQTTClient**: Full MQTT 3.1.1 with Home Assistant auto-discovery
- **ModbusRTUServer**: RS-485 slave (FC03/04/06/16)
- **ModbusTCPServer**: TCP slave on port 502
- **OTAManager**: Firmware updates (HTTP + ArduinoOTA)

### Display & Diagnostics
- **LCDManager**: 8-page display with auto-scroll
- **LEDIndicator**: Status LED patterns
- **Logger**: 6 log levels, multi-output
- **SelfTest**: 8 hardware tests at boot
- **HealthMonitor**: Runtime health tracking

### Security
- **AuthManager**: SHA-256, session/API keys, rate limiting

### Web Dashboard
- Modern responsive UI with real-time gauges
- Dark/light theme
- WebSocket live updates
- Configuration interface

## Features

✅ **100% V1.0 Feature Parity** - All measurements and functionality preserved  
✅ **Thread-Safe** - Mutex-protected SPI/I2C communication  
✅ **Non-Blocking** - Zero `delay()` calls in main loop  
✅ **Error Recovery** - Automatic recovery from transient failures  
✅ **Comprehensive Logging** - Multi-level, multi-output  
✅ **Power Quality Analysis** - Sag/swell/THD detection  
✅ **Home Assistant Integration** - MQTT auto-discovery  
✅ **Modbus RTU/TCP** - Full register map (200+ registers)  
✅ **Web Dashboard** - Real-time monitoring and configuration  
✅ **OTA Updates** - Firmware updates over WiFi/Ethernet  
✅ **Data Logging** - Historical data with CSV export  

## Dependencies

### Required Libraries
```
- Arduino core for ESP32
- Wire (I2C)
- SPI
- WiFi
- Ethernet
- SPIFFS
- Preferences (NVS)
- ESPmDNS
- Update
- mbedtls (SHA-256)
```

### External Libraries (install via Library Manager)
```
- Adafruit MCP23X17 (v2.3.0+)
- LiquidCrystal_I2C (v1.1.2+)
- DHT sensor library (v1.4.4+)
- ESPAsyncWebServer (v1.2.3+)
- AsyncTCP (v1.1.1+)
- ArduinoJson (v6.21.0+)
- PubSubClient (v2.8.0+)
- ModbusRTU (v4.1.0+)
- ArduinoOTA (v3.0.0+)
```

## Compilation

### Arduino IDE
1. Install ESP32 board support
2. Install all required libraries
3. Open `SM_GE3222M_V2.ino`
4. Select board: "ESP32 Dev Module"
5. Upload

### PlatformIO
```bash
pio run -t upload
```

## Configuration

### Default Credentials
- **Web UI**: admin / admin (change on first login)
- **OTA**: admin

### WiFi Configuration
- STA mode: Configure via web UI or config file
- AP mode fallback: SSID "SM-GE3222M-XXXX" (password: 12345678)

### Modbus
- RTU: Slave ID 1, 9600 baud, no parity
- TCP: Port 502

### MQTT
- Configure via web UI
- Topic prefix: "smartmeter/"

## API Endpoints

### REST API v1
- `GET /api/v1/readings` - Current measurements (JSON)
- `GET /api/v1/energy` - Energy accumulators (JSON)
- `GET /api/v1/config` - Configuration (JSON)
- `PUT /api/v1/config` - Update config
- `GET /api/v1/status` - System status
- `POST /api/v1/calibrate` - Calibration commands
- `GET /api/v1/history` - Data log (CSV)
- `POST /api/v1/reset/energy` - Reset counters
- `GET /api/v1/network` - Network info

### WebSocket
- Endpoint: `ws://device-ip/ws`
- Real-time meter data push (1 Hz)

## Task Schedule

| Task | Interval | Priority |
|------|----------|----------|
| EnergyRead | 100ms | CRITICAL |
| ModbusRTU/TCP | 50ms | CRITICAL |
| WebSocketPush | 1s | HIGH |
| LCDUpdate | 500ms | MEDIUM |
| MQTTUpdate | 5s | MEDIUM |
| WiFiCheck | 5s | LOW |
| HealthCheck | 10s | LOW |
| DataLog | 30s | IDLE |
| EnergyPersist | 60s | IDLE |

## File Structure

```
SM_GE3222M V2.0/
├── SM_GE3222M_V2.ino          # Main entry point
├── src/                        # Source code
│   ├── core/                   # Core subsystem
│   ├── hal/                    # Hardware abstraction
│   ├── energy/                 # Energy metering
│   ├── storage/                # NVS + SPIFFS
│   ├── comm/                   # Communication stack
│   ├── display/                # LCD + LEDs
│   ├── diagnostics/            # Logging + monitoring
│   └── security/               # Authentication
├── include/                    # Headers
│   ├── Version.h
│   ├── PinMap.h
│   ├── GlobalTypes.h
│   ├── RegisterMap.h
│   └── ModbusRegMap.h
└── data/                       # Web files (upload to SPIFFS)
    ├── index.html
    ├── config.html
    ├── style.css
    └── app.js
```

## Memory Usage (Estimated)

- Flash: ~1.2 MB (with libraries)
- RAM: ~80 KB (heap + stack)
- SPIFFS: 1 MB (web files + data logs)

## Troubleshooting

### No measurements
- Check ATM90E36 SPI connection (GPIO5 CS)
- Run self-test (results on LCD/Serial)
- Verify calibration values

### WiFi not connecting
- Check credentials in config
- AP mode fallback: Connect to "SM-GE3222M-XXXX"
- Configure via web UI at 192.168.4.1

### Modbus not responding
- Check RS-485 wiring (A, B, GND)
- Verify slave ID and baud rate
- Use Modbus scanner tool

### Web UI not loading
- Upload data/ folder to SPIFFS first
- Check SPIFFS mount status in logs
- Verify web server started (port 80)

## Version History

### V2.0.0 (2024)
- Complete architecture overhaul
- Non-blocking cooperative scheduler
- Thread-safe communication
- Comprehensive error handling
- Modern web dashboard
- Home Assistant integration
- Power quality analysis
- Data logging and export
- OTA firmware updates

### V1.0 (2024)
- Initial development prototype

## License

Copyright © 2024 Microcode Engineering. All rights reserved.

## Support

For issues and questions, refer to the repository wiki and issue tracker.
