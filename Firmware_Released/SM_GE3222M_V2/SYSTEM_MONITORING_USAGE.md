# System Monitoring Usage Guide

## Overview

The SystemMonitor and WatchdogManager modules provide comprehensive system health monitoring and watchdog protection for the SM-GE3222M V2.0 firmware.

## SystemMonitor

### Features
- Real-time system metrics tracking
- Heap memory monitoring (current and minimum)
- CPU frequency and temperature monitoring
- Uptime tracking with formatted output
- Error counting and tracking
- WiFi RSSI monitoring via NetworkManager
- Task stack watermark monitoring
- Boot count persistence

### Initialization

```cpp
#include "SystemMonitor.h"

void setup() {
    SystemMonitor& monitor = SystemMonitor::getInstance();
    if (!monitor.init()) {
        Serial.println("Failed to initialize SystemMonitor");
    }
}
```

### Updating Metrics

Call `update()` every 5 seconds from the DiagnosticsTask:

```cpp
void diagnosticsTaskFunc(void* param) {
    while (true) {
        SystemMonitor::getInstance().update();
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
```

### Retrieving System Status

```cpp
SystemStatus status = SystemMonitor::getInstance().getSystemStatus();

Serial.printf("Uptime: %u seconds\n", status.uptime);
Serial.printf("Free Heap: %u bytes\n", status.freeHeap);
Serial.printf("Min Free Heap: %u bytes\n", status.minFreeHeap);
Serial.printf("CPU Freq: %u MHz\n", status.cpuFreqMHz);
Serial.printf("Error Count: %u\n", status.errorCount);
Serial.printf("WiFi: %s\n", status.wifiConnected ? "Connected" : "Disconnected");
```

### Formatted Uptime

```cpp
String uptime = SystemMonitor::getInstance().getUptimeString();
Serial.printf("Uptime: %s\n", uptime.c_str());
// Output examples: "5d 3h 42m", "12h 30m", "45m"
```

### Error Tracking

```cpp
// Increment error count
SystemMonitor::getInstance().incrementErrorCount();
SystemMonitor::getInstance().setLastError(ErrorCode::WIFI_TIMEOUT);

// Reset error count
SystemMonitor::getInstance().resetErrorCount();
```

### Task Stack Monitoring

```cpp
TaskHandle_t energyTask = TaskManager::getInstance().getEnergyTaskHandle();
UBaseType_t watermark = SystemMonitor::getInstance().getTaskStackWatermark(energyTask);
Serial.printf("Energy Task Stack Watermark: %u bytes\n", watermark);
```

## WatchdogManager

### Features
- ESP32 Task Watchdog Timer (TWDT) management
- Configurable timeout (default 30 seconds)
- Task subscription for multi-task watchdog
- Panic handler on timeout
- Automatic system restart on watchdog trigger

### Initialization

```cpp
#include "WatchdogManager.h"

void setup() {
    WatchdogManager& wdt = WatchdogManager::getInstance();
    
    // Initialize with 30-second timeout
    if (!wdt.init(30)) {
        Serial.println("Failed to initialize WatchdogManager");
    }
}
```

### Feeding the Watchdog

Must be called regularly from main loop:

```cpp
void loop() {
    WatchdogManager::getInstance().feed();
    
    // Your main loop code here
    
    delay(100);  // Don't delay too long!
}
```

### Subscribing Tasks

Subscribe critical tasks to the watchdog:

```cpp
void criticalTask(void* param) {
    TaskHandle_t taskHandle = xTaskGetCurrentTaskHandle();
    WatchdogManager::getInstance().subscribeTask(taskHandle, "CriticalTask");
    
    while (true) {
        // Do work
        
        // Reset watchdog for this task
        esp_task_wdt_reset();
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Unsubscribing Tasks

```cpp
TaskHandle_t taskHandle = xTaskGetCurrentTaskHandle();
WatchdogManager::getInstance().unsubscribeTask(taskHandle);
```

## Integration Example

### Phase 1: HAL Initialization (from main .ino)

```cpp
void setup() {
    Serial.begin(115200);
    
    // Phase 1: HAL Initialization
    printBootPhase("1", "HAL Initialization");
    
    // Initialize Logger
    Logger::getInstance().init(LogLevel::INFO, true, false);
    
    // Initialize SystemMonitor
    if (!checkBootStatus("SystemMonitor", SystemMonitor::getInstance().init())) {
        return;
    }
    
    // Initialize Watchdog (30 second timeout)
    if (!checkBootStatus("WatchdogManager", WatchdogManager::getInstance().init(30))) {
        return;
    }
    
    // Continue with other initialization...
}
```

### Main Loop with Watchdog Feeding

```cpp
void loop() {
    // Feed watchdog to prevent timeout
    WatchdogManager::getInstance().feed();
    
    // Handle events, OTA, etc.
    EventBus::getInstance().dispatch();
    OTAManager::getInstance().handle();
    
    delay(10);
}
```

### Diagnostics Task with Monitoring

```cpp
void diagnosticsTaskFunc(void* param) {
    // Subscribe this task to watchdog
    TaskHandle_t taskHandle = xTaskGetCurrentTaskHandle();
    WatchdogManager::getInstance().subscribeTask(taskHandle, "DiagnosticsTask");
    
    while (true) {
        // Update system metrics
        SystemMonitor::getInstance().update();
        
        // Log system status periodically
        SystemStatus status = SystemMonitor::getInstance().getSystemStatus();
        Logger::getInstance().info("Diagnostics: Uptime=%s, Heap=%u, Errors=%u",
            SystemMonitor::getInstance().getUptimeString().c_str(),
            status.freeHeap,
            status.errorCount);
        
        // Check for low heap condition
        if (status.freeHeap < 10000) {
            Logger::getInstance().warn("Diagnostics: Low heap warning!");
        }
        
        // Reset this task's watchdog
        esp_task_wdt_reset();
        
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
```

## Best Practices

1. **Watchdog Feeding**: Always feed the watchdog in the main loop at least once per timeout period
2. **Task Subscription**: Subscribe critical tasks that must run continuously
3. **Monitoring Interval**: Call `SystemMonitor::update()` every 5 seconds from DiagnosticsTask
4. **Error Tracking**: Use `incrementErrorCount()` and `setLastError()` for all error conditions
5. **Stack Monitoring**: Periodically check task stack watermarks to prevent overflows
6. **Heap Monitoring**: Monitor free heap and trigger warnings below critical thresholds

## Troubleshooting

### Watchdog Timeout
If the system restarts due to watchdog timeout:
1. Check the task that's blocking (shown in panic message)
2. Ensure all subscribed tasks call `esp_task_wdt_reset()` regularly
3. Increase timeout if necessary for legitimate long operations
4. Remove task subscription if it's not critical

### Low Heap Memory
If heap memory drops critically low:
1. Check `minFreeHeap` to see the lowest point
2. Review memory allocations in your code
3. Reduce buffer sizes or optimize data structures
4. Enable heap poisoning for debugging (`CONFIG_HEAP_POISONING_COMPREHENSIVE`)

### High Error Count
If error count keeps increasing:
1. Check `lastError` to identify the error type
2. Review logs for error patterns
3. Fix the root cause of the errors
4. Use `resetErrorCount()` after resolving issues
