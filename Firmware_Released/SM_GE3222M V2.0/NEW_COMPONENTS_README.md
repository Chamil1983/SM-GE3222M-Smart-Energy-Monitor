# New V2.0 Firmware Components

This document describes the 6 new production-quality components implemented for the SM-GE3222M V2.0 firmware.

## Components Overview

### 1. WebSocketServer (`src/comm/WebSocketServer.{h,cpp}`)

**Purpose**: Real-time meter data streaming to web clients via WebSocket protocol.

**Key Features**:
- Pushes meter data at 1Hz to all connected clients
- JSON formatted data output
- Maximum 4 concurrent clients
- Automatic connection management
- Integration with WebServerManager
- Ping/pong support for connection health

**Usage**:
```cpp
WebSocketServer::getInstance().init(wsPointer);
WebSocketServer::getInstance().begin();
WebSocketServer::getInstance().pushMeterData(meterData);
```

**Data Format**:
```json
{
  "timestamp": 12345678,
  "phaseA": {"voltage": 230.5, "current": 5.2, "power": 1198.6, "pf": 0.98},
  "phaseB": {"voltage": 231.2, "current": 4.8, "power": 1109.8, "pf": 0.97},
  "phaseC": {"voltage": 229.8, "current": 5.0, "power": 1149.0, "pf": 0.99},
  "totals": {"power": 3457.4, "reactive": 234.5, "apparent": 3465.3, "pf": 0.98},
  "frequency": 50.0,
  "temperature": 25.3,
  "energy": {"importTotal": 1234.56, "exportTotal": 12.34}
}
```

---

### 2. TCPDataServer (`src/comm/TCPDataServer.{h,cpp}`)

**Purpose**: Legacy TCP protocol server for VB.NET desktop application compatibility.

**Key Features**:
- Port 8088 (configurable)
- V1.0 protocol compatibility
- /SYNC/ handshake protocol
- Support for all V1.0 commands
- Support for all V1.0 data tags
- JSON calibration data exchange
- Maximum 4 concurrent clients

**Supported Commands**:
- `Code_NewReq` - Request meter data
- `Code_Cal1` to `Code_Cal4` - Request calibration data (1-4)
- `Cal1_RST` to `Cal4_RST` - Reset calibration (1-4)
- `ConfigSave` - Save configuration
- `ConfigRst` - Reset configuration
- `Reboot` - Reboot device

**Supported Data Tags**:
- **Energy**: AE1-AE5 (Active), BE1-BE5 (Reactive), CE1-CE5 (Apparent)
- **System**: TE1-TE6 (Temperature, Frequency, Neutral Current, etc.)
- **Power**: AP1-AP10 (Active), BP1-BP10 (Reactive), CP1-CP10 (Apparent)
- **Quality**: TP1-TP10 (Power Factor, THD)
- **Display**: DP1-DP3 (Voltages)
- **Metering**: MD1-MD4 (Currents, Frequency)
- **Status**: ST1-ST15 (Phase Angles)

**Protocol Flow**:
```
Client connects → Server sends "/SYNC/\n"
Client sends "/SYNC/\n" → Server sends "Code_NewReq\n"
Client sends "Code_NewReq\n" → Server sends all data tags
```

---

### 3. DisplayManager (`src/ui/DisplayManager.{h,cpp}`)

**Purpose**: Thread-safe unified display management layer.

**Key Features**:
- Wraps LCDManager with higher-level interface
- Multiple display pages (boot, phases, totals, energy, network, system, error, custom)
- Auto page rotation support
- Thread-safe operations with mutex
- Error message queue with timeout
- Boot splash and initialization status display

**Display Pages**:
- BOOT - Splash screen with version
- PHASE_A/B/C - Individual phase measurements
- TOTALS - Total power measurements
- ENERGY - Energy counters
- NETWORK - IP, RSSI, MQTT status
- SYSTEM - Uptime, temperature, heap, errors
- ERROR - Error messages
- CUSTOM - Custom messages

**Usage**:
```cpp
DisplayManager::getInstance().init();
DisplayManager::getInstance().showBootSplash("2.0.1");
DisplayManager::getInstance().setAutoRotation(true, 5000);
DisplayManager::getInstance().updateMeterData(data);
DisplayManager::getInstance().showError(ErrorCode::WIFI_DISCONNECTED, "WiFi Lost", 5000);
```

---

### 4. ButtonHandler (`src/ui/ButtonHandler.{h,cpp}`)

**Purpose**: Physical button handling with debouncing and advanced press detection.

**Key Features**:
- Debounced reading (50ms default)
- Short press detection (< 1s)
- Long press detection (1-10s, configurable)
- Factory reset detection (> 10s hold)
- ISR-based for responsiveness
- Callback support for custom actions
- Default behaviors if no callback set

**Buttons**:
- **MODE** (PIN_MODE_BUTTON): Page navigation, auto-rotation toggle, factory reset
- **SET** (PIN_SET_BUTTON): Previous page, custom actions

**Default Actions**:
- MODE short press → Next page
- MODE long press → Toggle auto-rotation
- MODE 10s hold → Factory reset
- SET short press → Previous page

**Usage**:
```cpp
ButtonHandler::getInstance().init();
ButtonHandler::getInstance().setCallback([](Button btn, Event evt) {
    // Custom handler
});
```

---

### 5. ExportManager (`src/storage/ExportManager.{h,cpp}`)

**Purpose**: Data export functionality for logged meter data.

**Key Features**:
- Export to CSV or JSON format
- Date/time range filtering
- Field selection via bitmask
- Export to file, string, or stream
- Progress tracking
- Size estimation
- Web endpoint integration ready

**Export Formats**:
- **CSV**: Comma-separated values with optional header
- **JSON**: Array of objects with optional metadata

**Export Options**:
- Format (CSV/JSON)
- Start/End time (Unix timestamps)
- Field mask (select specific fields)
- Include header/metadata
- Maximum records limit

**Usage**:
```cpp
ExportManager::getInstance().init();

ExportManager::ExportOptions options;
options.format = ExportManager::Format::CSV;
options.startTime = 1640000000;
options.endTime = 0;  // To end
options.maxRecords = 1000;

ExportManager::ExportResult result = 
    ExportManager::getInstance().exportToFile("/export.csv", options);
```

---

### 6. AccessControl (`src/security/AccessControl.{h,cpp}`)

**Purpose**: Role-based access control system.

**Key Features**:
- Three user roles: Admin, User, Viewer
- Comprehensive permission system
- User account management
- Role persistence in NVS
- Password hashing with salt
- Session integration with AuthManager
- Request-based permission checking

**User Roles**:
- **Admin**: Full system access
  - All read permissions
  - All write permissions
  - System management (reboot, update, factory reset)
  - User and role management

- **User**: Operational access
  - All read permissions
  - Configuration writes
  - Energy reset
  - Relay control
  - No system management or user management

- **Viewer**: Read-only access
  - Read status, measurements, energy, logs, network
  - No write permissions
  - No system management

**Permissions**:
- Read: STATUS, MEASUREMENTS, ENERGY, CONFIG, LOGS, NETWORK
- Write: CONFIG, CALIBRATION, RESET_ENERGY, CONTROL_RELAYS
- System: REBOOT, UPDATE, FACTORY_RESET
- Management: MANAGE_USERS, MANAGE_ROLES

**Usage**:
```cpp
AccessControl::getInstance().init();
AccessControl::getInstance().createUser("operator", "pass123", UserRole::USER);

// Check permission
if (AccessControl::getInstance().hasPermission("operator", Permission::WRITE_CONFIG)) {
    // Allow
}

// Check via HTTP request
if (AccessControl::getInstance().checkRequestPermission(request, Permission::SYSTEM_REBOOT)) {
    ESP.restart();
}
```

---

## Integration

See [INTEGRATION_GUIDE.md](./INTEGRATION_GUIDE.md) for detailed integration examples, including:
- Basic setup for each component
- Complete working examples
- Web API integration
- Client examples (JavaScript, VB.NET)
- Thread-safety guidelines
- Troubleshooting tips

## Dependencies

All components integrate with existing V2.0 firmware:
- **Logger** (diagnostics): All components use Logger for consistent logging
- **ConfigManager** (storage): Configuration persistence
- **NVStorage** (storage): Key-value storage for settings
- **AuthManager** (security): Authentication and session management
- **CryptoHelper** (security): Password hashing and cryptography
- **LCDManager** (display): Low-level LCD control
- **WebServerManager** (comm): Base web server

## Design Patterns

All components follow consistent patterns:
- **Singleton pattern** for manager classes
- **Thread-safe operations** where needed (DisplayManager)
- **Comprehensive error handling** with Logger
- **Doxygen documentation** for all public APIs
- **Const correctness** and memory safety
- **Resource management** (RAII principles)

## File Structure

```
src/
├── comm/
│   ├── WebSocketServer.h      # WebSocket server header
│   ├── WebSocketServer.cpp    # WebSocket server implementation
│   ├── TCPDataServer.h        # TCP server header
│   └── TCPDataServer.cpp      # TCP server implementation
├── ui/
│   ├── DisplayManager.h       # Display manager header
│   ├── DisplayManager.cpp     # Display manager implementation
│   ├── ButtonHandler.h        # Button handler header
│   └── ButtonHandler.cpp      # Button handler implementation
├── storage/
│   ├── ExportManager.h        # Export manager header
│   └── ExportManager.cpp      # Export manager implementation
└── security/
    ├── AccessControl.h        # Access control header
    └── AccessControl.cpp      # Access control implementation
```

## Testing

Each component includes:
- Initialization validation
- Error handling for invalid inputs
- Resource cleanup on shutdown
- Thread-safety where applicable
- Logging of all significant events

## Performance Characteristics

- **WebSocketServer**: 1Hz push rate, ~2KB JSON per push
- **TCPDataServer**: On-demand responses, ~5KB per request
- **DisplayManager**: Non-blocking updates, ~100ms per page render
- **ButtonHandler**: ISR-based, <1ms response time
- **ExportManager**: Streaming export, memory-efficient for large datasets
- **AccessControl**: O(1) permission checks, persistent storage

## Security Considerations

- **WebSocketServer**: Max 4 clients prevents DoS
- **TCPDataServer**: Max 4 clients prevents DoS
- **AccessControl**: Salted password hashing (SHA-256)
- **AccessControl**: Role-based permissions prevent privilege escalation
- **AccessControl**: Default admin account (change password on first use!)

## Known Limitations

1. **WebSocketServer**: Binary data not supported (JSON only)
2. **TCPDataServer**: V1.0 protocol only, no V2.0 extensions
3. **DisplayManager**: 20x4 LCD only (hardware limitation)
4. **ButtonHandler**: Two buttons only (hardware limitation)
5. **ExportManager**: Large exports may take time, use streaming
6. **AccessControl**: Username from request requires AuthManager session

## Future Enhancements

Potential improvements for future versions:
- WebSocket binary protocol support
- TCP protocol V2.0 extensions
- OLED display support in DisplayManager
- Touchscreen support in ButtonHandler
- Real-time streaming in ExportManager
- OAuth2 support in AccessControl

---

## Questions or Issues?

For questions or issues with these components:
1. Check the [INTEGRATION_GUIDE.md](./INTEGRATION_GUIDE.md)
2. Review component header files for API documentation
3. Check Logger output for error messages
4. Verify initialization order
5. Check hardware connections (buttons, display)

---

## License

All components are part of the SM-GE3222M Smart Energy Monitor V2.0 firmware.
See LICENSE file for details.
