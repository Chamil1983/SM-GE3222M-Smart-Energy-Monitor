# System Monitoring and Watchdog Implementation

## Files Created

### SystemMonitor.h / SystemMonitor.cpp
Complete system monitoring solution with the following features:

**Key Features:**
- ✅ Singleton pattern implementation
- ✅ `init()` - Initialize monitoring system with mutex protection
- ✅ `update()` - Update all metrics (designed to be called every 5 seconds from DiagnosticsTask)
- ✅ `getSystemStatus()` - Return complete SystemStatus struct with all metrics
- ✅ `getUptimeString()` - Format uptime as "Xd Yh Zm"
- ✅ `resetErrorCount()` - Reset error counter
- ✅ Thread-safe operations with FreeRTOS mutex

**Metrics Tracked:**
- Free heap memory (`ESP.getFreeHeap()`)
- Minimum free heap seen (`ESP.getMinFreeHeap()`)
- CPU frequency (`ESP.getCpuFreqMHz()`)
- Uptime in seconds (calculated from `millis()`)
- WiFi RSSI (via NetworkManager)
- Task stack watermarks (`uxTaskGetStackHighWaterMark()` for any task)
- Error counts (increment, reset, get)
- Boot count (persisted in NVS)
- CPU temperature (for ESP32-S2/S3)
- Last error code

**Architecture:**
- Uses SystemStatus struct from DataTypes.h
- Logger integration for all logging
- NetworkManager integration for WiFi metrics
- Preferences (NVS) for boot count persistence
- FreeRTOS mutex for thread safety

### WatchdogManager.h / WatchdogManager.cpp
ESP32 Task Watchdog Timer (TWDT) management with the following features:

**Key Features:**
- ✅ Singleton pattern implementation
- ✅ `init(timeoutSeconds)` - Initialize hardware watchdog with configurable timeout
- ✅ `feed()` - Feed watchdog (must be called regularly from main loop)
- ✅ `subscribeTask()` - Subscribe FreeRTOS tasks to watchdog
- ✅ `unsubscribeTask()` - Unsubscribe tasks from watchdog
- ✅ Uses ESP32 `esp_task_wdt.h` for Task WDT
- ✅ Configurable timeout (default 30 seconds)
- ✅ Panic handler on watchdog timeout
- ✅ Automatic system restart on timeout

**Architecture:**
- Uses ESP32 Task Watchdog Timer (TWDT)
- Trigger panic on timeout (system restart)
- Multiple task subscription support
- Main loop task automatically subscribed
- Logger integration for all events

## Integration with Existing Code

The modules are designed to integrate seamlessly with the existing SM-GE3222M V2.0 architecture:

1. **SystemMonitor** tracks all system metrics and works with:
   - DataTypes.h (SystemStatus struct)
   - Logger (thread-safe logging)
   - NetworkManager (WiFi RSSI)
   - TaskManager (stack watermarks)

2. **WatchdogManager** provides system reliability and works with:
   - ESP32 Task Watchdog Timer
   - FreeRTOS tasks
   - Logger (event logging)

## Usage

See `SYSTEM_MONITORING_USAGE.md` for detailed usage examples and best practices.

## Quick Integration Example

```cpp
// In setup() - Phase 1: HAL Initialization
SystemMonitor::getInstance().init();
WatchdogManager::getInstance().init(30);

// In loop() - Feed watchdog
void loop() {
    WatchdogManager::getInstance().feed();
    delay(10);
}

// In DiagnosticsTask - Update metrics every 5 seconds
void diagnosticsTaskFunc(void* param) {
    while (true) {
        SystemMonitor::getInstance().update();
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
```

## Benefits

1. **System Health**: Real-time monitoring of heap, CPU, and network
2. **Reliability**: Watchdog ensures system recovery from hangs
3. **Diagnostics**: Comprehensive metrics for troubleshooting
4. **Thread Safety**: Mutex-protected operations for FreeRTOS
5. **Persistence**: Boot count tracking across reboots
6. **Integration**: Works seamlessly with existing architecture

## Notes

- SystemMonitor uses the existing SystemStatus struct from DataTypes.h
- Both modules follow the singleton pattern used throughout the codebase
- Logger is used for all diagnostic messages
- Thread-safe design for FreeRTOS multi-tasking environment
- Watchdog automatically subscribes the main loop task
- Additional critical tasks can be subscribed to the watchdog
