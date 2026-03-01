# DHT22 Integration Patch Notes

This patch adds DHT22 ambient sensor support on GPIO4 and integrates it into:
- Web dashboard (ambient temperature + humidity widgets + status)
- WebSocket `/ws` payload and `/api/meter`
- Meter snapshot (`ambientTemperature`, `ambientHumidity`)
- MQTT payloads (already wired via MeterData)
- Modbus mapping updates (already wired via MeterData)
- DataLogger ring buffer capture (now logged from EnergyTask)
- Diagnostics logging + dedicated `DHTTask`

## DHT settings (SystemConfig / /api/config)
- `system.dhtEnabled` (bool)
- `system.dhtReadIntervalMs` (min 2000 ms)
- `system.dhtDebugLogging` (bool)

Note: DHT config is loaded at boot in this patch (reboot after changing DHT settings).
