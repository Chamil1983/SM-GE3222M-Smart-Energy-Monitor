# ModbusServer and MQTTPublisher Usage Guide

## ModbusServer

### Overview
The ModbusServer module provides unified Modbus RTU and TCP server functionality with IEEE754 float encoding for energy meter data.

### Features
- Singleton pattern for easy access
- Simultaneous RTU (Serial2) and TCP (port 502) operation
- IEEE754 float encoding (2 registers per float value)
- Complete register map support:
  - Input registers (0-399): Meter readings (voltages, currents, power, energy, etc.)
  - Holding registers (0-99): Configuration and control
  - Coils (0-9): Relay and LED control
  - Discrete inputs (0-9): Button and status inputs

### Basic Usage

```cpp
#include "ModbusServer.h"

// Get singleton instance
ModbusServer& modbus = ModbusServer::getInstance();

// Configure Modbus
ModbusConfig config;
config.rtuEnabled = true;
config.tcpEnabled = true;
config.slaveID = 1;
config.baudrate = 9600;
config.parity = 'N';
config.tcpPort = 502;

// Initialize
if (modbus.begin(config)) {
    Logger::getInstance().info("Modbus started successfully");
}

// In main loop or task
void loop() {
    modbus.handle();  // Process Modbus requests
}

// Update meter data
void onMeterDataUpdate(const MeterData& data) {
    modbus.updateMeterData(data);
}

// Update system status
void onSystemStatusUpdate(const SystemStatus& status) {
    modbus.updateSystemStatus(status);
}
```

### Register Map Examples

**Reading Voltage (Phase A)** - Input Registers 0-1:
```
Register 0: High word of IEEE754 float
Register 1: Low word of IEEE754 float
Combined: 32-bit float voltage value (e.g., 230.5V)
```

**Reading Power (Phase A)** - Input Registers 12-13:
```
Register 12: High word of active power
Register 13: Low word of active power
Combined: 32-bit float power value (e.g., 1520.3W)
```

**Reading Energy (Phase A Forward)** - Input Registers 100-101:
```
Register 100: High word of energy
Register 101: Low word of energy
Combined: 32-bit float energy value (e.g., 123.45kWh)
```

### Control Examples

**Setting Relay State** - Coil 0-1:
```cpp
modbus.setCoil(MB_COIL_RELAY_1, true);  // Turn on Relay 1
modbus.setCoil(MB_COIL_RELAY_2, false); // Turn off Relay 2
```

**Reading Button State** - Discrete Input 0-1:
```cpp
bool setPressed = modbus.getCoil(MB_DI_BUTTON_SET);
bool modePressed = modbus.getCoil(MB_DI_BUTTON_MODE);
```

---

## MQTTPublisher

### Overview
The MQTTPublisher module handles MQTT connectivity with automatic reconnection and Home Assistant MQTT discovery support.

### Features
- Singleton pattern
- Auto-reconnect with exponential backoff
- JSON-based state publishing
- Home Assistant MQTT discovery
- QoS 1 for reliable delivery
- Retained messages for status
- Configurable publish intervals

### Basic Usage

```cpp
#include "MQTTPublisher.h"

// Get singleton instance
MQTTPublisher& mqtt = MQTTPublisher::getInstance();

// Configure MQTT
MQTTConfig config;
config.enabled = true;
strcpy(config.broker, "192.168.1.100");
config.port = 1883;
strcpy(config.username, "mqtt_user");
strcpy(config.password, "mqtt_pass");
strcpy(config.clientID, "ge3222m_01");
strcpy(config.baseTopic, "energy/meter1");
config.publishInterval = 10;  // 10 seconds
config.useHomeAssistant = true;
config.useTLS = false;

// Initialize
if (mqtt.begin(config)) {
    Logger::getInstance().info("MQTT started successfully");
}

// In main loop or task
void loop() {
    mqtt.handle();  // Process MQTT loop and reconnect
}

// Publish meter data
void onMeterDataUpdate(const MeterData& data) {
    if (mqtt.isConnected()) {
        mqtt.publish(data);
    }
}
```

### Published Topics

**State Topic** (published every interval):
```
Topic: {baseTopic}/state
Payload: JSON with complete meter data
QoS: 1
Retained: false
```

**Status Topic** (LWT - Last Will Testament):
```
Topic: {baseTopic}/status
Payload: "online" or "offline"
QoS: 1
Retained: true
```

### JSON State Format

```json
{
  "timestamp": 1234567890,
  "sequence": 123,
  "phase_a": {
    "voltage": 230.5,
    "current": 10.2,
    "power": 2350.1,
    "reactive_power": 150.0,
    "apparent_power": 2354.9,
    "power_factor": 0.998,
    "phase_angle": 0.5,
    "voltage_thd": 2.1,
    "current_thd": 3.5,
    "energy_import": 123.45,
    "energy_export": 0.0
  },
  "phase_b": { ... },
  "phase_c": { ... },
  "totals": {
    "power": 7050.3,
    "reactive_power": 450.0,
    "apparent_power": 7064.7,
    "power_factor": 0.998,
    "energy_import": 370.35,
    "energy_export": 0.0
  },
  "frequency": 50.0,
  "neutral_current": 0.5,
  "board_temperature": 35.2,
  "ambient_temperature": 25.5,
  "ambient_humidity": 45.0
}
```

### Home Assistant Discovery

When `useHomeAssistant` is enabled, the publisher automatically sends discovery messages for all sensors:

**Discovery Topics**:
```
homeassistant/sensor/{device_id}/voltage_a/config
homeassistant/sensor/{device_id}/current_a/config
homeassistant/sensor/{device_id}/power_a/config
homeassistant/sensor/{device_id}/energy_import_a/config
... (and many more)
```

**Discovery Payload Example**:
```json
{
  "name": "voltage_a",
  "unique_id": "ge3222m_abc123_voltage_a",
  "object_id": "abc123_voltage_a",
  "state_topic": "energy/meter1/state",
  "value_template": "{{ value_json.phase_a.voltage }}",
  "device_class": "voltage",
  "unit_of_measurement": "V",
  "state_class": "measurement",
  "device": {
    "identifiers": ["abc123"],
    "name": "SM-GE3222M Energy Monitor",
    "model": "GE3222M",
    "manufacturer": "SM",
    "sw_version": "2.0.0"
  }
}
```

### Sensor Classes Published

- **Voltage**: Phase A, B, C (V)
- **Current**: Phase A, B, C, Neutral (A)
- **Power**: Phase A, B, C, Total (W)
- **Power Factor**: Phase A, B, C, Total
- **Energy Import**: Phase A, B, C, Total (kWh)
- **Energy Export**: Phase A, B, C, Total (kWh)
- **Frequency**: Line frequency (Hz)
- **Temperature**: Board, Ambient (°C)
- **Humidity**: Ambient (%)

---

## Integration Example

Complete example showing both modules working together:

```cpp
#include "ModbusServer.h"
#include "MQTTPublisher.h"
#include "EnergyMeter.h"

ModbusServer& modbus = ModbusServer::getInstance();
MQTTPublisher& mqtt = MQTTPublisher::getInstance();
EnergyMeter& meter = EnergyMeter::getInstance();

void setup() {
    // Initialize Modbus
    ModbusConfig modbusConfig;
    modbusConfig.rtuEnabled = true;
    modbusConfig.tcpEnabled = true;
    modbusConfig.slaveID = 1;
    modbusConfig.baudrate = 9600;
    modbus.begin(modbusConfig);
    
    // Initialize MQTT
    MQTTConfig mqttConfig;
    mqttConfig.enabled = true;
    strcpy(mqttConfig.broker, "192.168.1.100");
    strcpy(mqttConfig.baseTopic, "energy/meter1");
    mqttConfig.publishInterval = 10;
    mqttConfig.useHomeAssistant = true;
    mqtt.begin(mqttConfig);
    
    // Initialize energy meter
    meter.begin();
}

void loop() {
    // Handle Modbus requests
    modbus.handle();
    
    // Handle MQTT loop
    mqtt.handle();
    
    // Read meter data periodically
    static unsigned long lastRead = 0;
    if (millis() - lastRead >= 500) {
        lastRead = millis();
        
        MeterData data;
        if (meter.readData(data)) {
            // Update both Modbus and MQTT
            modbus.updateMeterData(data);
            mqtt.publish(data);
        }
    }
}
```

## Required Libraries

Add these to your `platformio.ini` or Arduino IDE Library Manager:

```ini
lib_deps =
    emelianov/modbus-esp8266 @ ^4.1.0    ; ModbusRTU library
    knolleary/PubSubClient @ ^2.8         ; MQTT client
    bblanchon/ArduinoJson @ ^6.21.3       ; JSON serialization
```

## Troubleshooting

### Modbus
- **No RTU response**: Check Serial2 wiring (TX=17, RX=16, DE=27)
- **No TCP connection**: Verify WiFi is connected and port 502 is not blocked
- **Wrong readings**: Verify IEEE754 float decoding in client

### MQTT
- **Connection failed**: Check broker IP, port, credentials
- **Not publishing**: Verify `config.enabled = true` and WiFi connected
- **HA not discovering**: Check `config.useHomeAssistant = true` and topic format

## Performance Notes

- **Modbus RTU**: ~20ms response time at 9600 baud
- **Modbus TCP**: <50ms response time (4 concurrent clients max)
- **MQTT publish**: Configurable interval (default 10s)
- **Memory usage**: ~8KB RAM for both modules combined
