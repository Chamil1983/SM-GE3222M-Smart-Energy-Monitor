DHT22 Web Sync Fix + Runtime Settings Patch

Root cause fixed:
- EnergyMeter::update() overwrote MeterData snapshot with fresh ATM90E36 rawData every 500ms.
- DHT22 values are injected by DHTSensorManager into MeterData asynchronously, so they were being reset to 0.0 between DHT reads.
- Result on dashboard: DHT values briefly show real values, then revert to 0.0 until next DHT poll.

Fixes included:
1) Preserve ambientTemperature / ambientHumidity fields across EnergyMeter updates.
2) WebUI JSON now prefers live DHT snapshot values when valid.
3) Dashboard JS caches last-good DHT values to avoid UI flicker on transient invalid reads.
4) Runtime DHT config apply via /api/config (enable/interval/debug) without reboot.
5) DHT settings panel added to dashboard UI.
