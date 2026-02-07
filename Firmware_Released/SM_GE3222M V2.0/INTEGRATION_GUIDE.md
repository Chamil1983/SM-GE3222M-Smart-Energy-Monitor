# SM-GE3222M V2.0 - New Components Integration Guide

This document provides integration examples for the newly implemented components.

## Table of Contents
1. [WebSocketServer Integration](#websocketserver-integration)
2. [TCPDataServer Integration](#tcpdataserver-integration)
3. [DisplayManager Integration](#displaymanager-integration)
4. [ButtonHandler Integration](#buttonhandler-integration)
5. [ExportManager Integration](#exportmanager-integration)
6. [AccessControl Integration](#accesscontrol-integration)

---

## WebSocketServer Integration

### Basic Setup
```cpp
#include "src/comm/WebSocketServer.h"
#include "src/comm/WebServerManager.h"

// In your initialization code:
void setup() {
    // Initialize web server first
    WebServerManager& webServer = WebServerManager::getInstance();
    webServer.init();
    webServer.begin();
    
    // Initialize WebSocket server with web server's WebSocket
    WebSocketServer& wsServer = WebSocketServer::getInstance();
    wsServer.init(webServer.getWebSocket());  // You may need to add getWebSocket() to WebServerManager
    wsServer.begin();
}

// In your main loop:
void loop() {
    // Update WebSocket server
    WebSocketServer::getInstance().update();
    
    // Push meter data periodically
    if (/* meter data ready */) {
        MeterData data;
        // ... populate data ...
        WebSocketServer::getInstance().pushMeterData(data);
    }
}
```

### WebSocket Client Example (JavaScript)
```javascript
// Connect to WebSocket
const ws = new WebSocket('ws://192.168.1.100/ws');

ws.onopen = () => {
    console.log('Connected to SM-GE3222M');
};

ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    
    if (data.type === 'welcome') {
        console.log('Client ID:', data.clientId);
        console.log('Push rate:', data.pushRate, 'ms');
    } else {
        // Real-time meter data
        console.log('Phase A Voltage:', data.phaseA.voltage, 'V');
        console.log('Total Power:', data.totals.power, 'W');
        console.log('Frequency:', data.frequency, 'Hz');
    }
};

// Send ping command
ws.send(JSON.stringify({ cmd: 'ping' }));
```

---

## TCPDataServer Integration

### Basic Setup
```cpp
#include "src/comm/TCPDataServer.h"

void setup() {
    // Initialize TCP server on port 8088 (or custom port)
    TCPDataServer& tcpServer = TCPDataServer::getInstance();
    tcpServer.init(8088);  // Default port is 8088
    tcpServer.begin();
}

void loop() {
    // Update TCP server
    TCPDataServer::getInstance().update();
    
    // Update meter data for transmission
    if (/* meter data ready */) {
        MeterData data;
        // ... populate data ...
        TCPDataServer::getInstance().updateMeterData(data);
    }
}
```

### VB.NET Client Example
```vb
' Create TCP client
Dim client As New TcpClient()
client.Connect("192.168.1.100", 8088)

Dim stream As NetworkStream = client.GetStream()
Dim writer As New StreamWriter(stream)
Dim reader As New StreamReader(stream)

' Wait for sync handshake
Dim line As String = reader.ReadLine()
If line.Contains("/SYNC/") Then
    ' Send handshake is automatic, server sends Code_NewReq
End If

' Wait for Code_NewReq
line = reader.ReadLine()
If line = "Code_NewReq" Then
    ' Read meter data
    While stream.DataAvailable
        line = reader.ReadLine()
        
        ' Parse data tags (e.g., "AE1:1234.56")
        If line.Contains(":") Then
            Dim parts() As String = line.Split(":"c)
            Dim tag As String = parts(0)
            Dim value As Single = Single.Parse(parts(1))
            
            ' Process tag
            Select Case tag
                Case "AE1"
                    ' Total import active energy
                Case "DP1"
                    ' Phase A voltage
                ' ... etc
            End Select
        End If
    End While
End If

' Send calibration request
writer.WriteLine("Code_Cal1")
writer.Flush()

' Read JSON calibration data
Dim jsonData As String = reader.ReadLine()
Dim cal As Object = JsonConvert.DeserializeObject(jsonData, GetType(Object))

' Send commands
writer.WriteLine("ConfigSave")   ' Save configuration
writer.WriteLine("ConfigRst")    ' Reset to defaults
writer.WriteLine("Cal1_RST")     ' Reset calibration 1
writer.WriteLine("Reboot")       ' Reboot device
```

---

## DisplayManager Integration

### Basic Setup
```cpp
#include "src/ui/DisplayManager.h"

void setup() {
    // Initialize display manager
    DisplayManager& display = DisplayManager::getInstance();
    display.init();
    
    // Show boot splash
    display.showBootSplash("2.0.1");
    
    // Show initialization status
    display.showInitStatus("WiFi", true);
    delay(500);  // Now caller controls timing
    display.showInitStatus("Modbus", true);
    delay(500);
    display.showInitStatus("ATM90E36", true);
    delay(500);
    
    // Enable auto page rotation (5 second interval)
    display.setAutoRotation(true, 5000);
}

void loop() {
    DisplayManager& display = DisplayManager::getInstance();
    
    // Update display
    display.update();
    
    // Update meter data for display
    if (/* meter data ready */) {
        MeterData data;
        // ... populate data ...
        display.updateMeterData(data);
    }
    
    // Update network info
    display.setNetworkInfo("192.168.1.100", -65, true);
    
    // Update system info
    display.setSystemInfo(
        millis() / 1000,        // uptime
        25.5,                   // temperature
        ESP.getFreeHeap(),      // free heap
        0                       // error count
    );
    
    // Show error if needed
    if (/* error condition */) {
        display.showError(
            ErrorCode::WIFI_DISCONNECTED,
            "WiFi Lost",
            5000  // Auto-clear after 5 seconds
        );
    }
}
```

### Thread-Safe Usage
```cpp
// For use from ISR or other thread
void IRAM_ATTR networkEventHandler() {
    DisplayManager& display = DisplayManager::getInstance();
    
    // Lock before accessing display
    display.lock();
    display.displayMessage(3, "Network Event!");
    display.unlock();
}
```

---

## ButtonHandler Integration

### Basic Setup
```cpp
#include "src/ui/ButtonHandler.h"
#include "src/ui/DisplayManager.h"

void setup() {
    // Initialize button handler
    ButtonHandler& buttons = ButtonHandler::getInstance();
    buttons.init();
    
    // Set custom callback (optional - defaults are provided)
    buttons.setCallback([](ButtonHandler::Button btn, ButtonHandler::Event evt) {
        DisplayManager& display = DisplayManager::getInstance();
        
        if (btn == ButtonHandler::Button::MODE) {
            if (evt == ButtonHandler::Event::SHORT_PRESS) {
                // Next page
                display.nextPage();
            }
            else if (evt == ButtonHandler::Event::LONG_PRESS) {
                // Toggle auto-rotation
                display.setAutoRotation(!display.isAutoRotationEnabled());
            }
            else if (evt == ButtonHandler::Event::FACTORY_RESET) {
                // Factory reset initiated
                display.clear();
                display.displayMessage(0, "Factory Reset!");
                display.displayMessage(1, "Resetting...");
            }
        }
        else if (btn == ButtonHandler::Button::SET) {
            if (evt == ButtonHandler::Event::SHORT_PRESS) {
                // Previous page
                display.previousPage();
            }
            else if (evt == ButtonHandler::Event::LONG_PRESS) {
                // Custom action (e.g., toggle WiFi)
                WiFiManager::getInstance().toggleAP();
            }
        }
    });
    
    // Set custom thresholds (optional)
    buttons.setLongPressThreshold(2000);     // 2 seconds for long press
    buttons.setFactoryResetThreshold(10000); // 10 seconds for factory reset
}

void loop() {
    // Update button handler
    ButtonHandler::getInstance().update();
}
```

---

## ExportManager Integration

### Basic Setup
```cpp
#include "src/storage/ExportManager.h"

void setup() {
    // Initialize export manager
    ExportManager& exporter = ExportManager::getInstance();
    exporter.init();
    
    // Set source file (optional, defaults to /datalog.csv)
    exporter.setSourceFile("/datalog.csv");
}

// Export to file
void exportToFile() {
    ExportManager& exporter = ExportManager::getInstance();
    
    // Configure export options
    ExportManager::ExportOptions options;
    options.format = ExportManager::Format::CSV;
    options.startTime = 1640000000;  // Unix timestamp (0 = from beginning)
    options.endTime = 0;             // 0 = to end
    options.fieldMask = 0xFFFF;      // All fields
    options.includeHeader = true;
    options.maxRecords = 1000;       // Limit to 1000 records (0 = unlimited)
    
    // Export
    ExportManager::ExportResult result = exporter.exportToFile("/export.csv", options);
    
    if (result.success) {
        Serial.printf("Exported %d records, %d bytes\n", 
                     result.recordCount, result.bytesWritten);
    } else {
        Serial.printf("Export failed: %s\n", result.errorMessage);
    }
}

// Export to string (for small datasets)
void exportToString() {
    ExportManager& exporter = ExportManager::getInstance();
    
    ExportManager::ExportOptions options;
    options.format = ExportManager::Format::JSON;
    options.maxRecords = 100;  // Limit for string export
    
    String output;
    ExportManager::ExportResult result = exporter.exportToString(output, options);
    
    if (result.success) {
        Serial.println(output);
    }
}

// Get available date range
void checkDateRange() {
    ExportManager& exporter = ExportManager::getInstance();
    
    unsigned long startTime, endTime;
    if (exporter.getAvailableRange(startTime, endTime)) {
        Serial.printf("Data available from %lu to %lu\n", startTime, endTime);
        
        // Estimate export size
        ExportManager::ExportOptions options;
        options.format = ExportManager::Format::CSV;
        size_t size = exporter.estimateExportSize(options);
        Serial.printf("Estimated export size: %d bytes\n", size);
    }
}
```

### Web Endpoint Integration
```cpp
// In WebServerManager or custom handler
void handleDataExport(AsyncWebServerRequest* request) {
    ExportManager& exporter = ExportManager::getInstance();
    
    // Parse query parameters
    String format = request->hasParam("format") ? 
                   request->getParam("format")->value() : "csv";
    
    ExportManager::ExportOptions options;
    options.format = (format == "json") ? 
                     ExportManager::Format::JSON : 
                     ExportManager::Format::CSV;
    
    if (request->hasParam("start")) {
        options.startTime = request->getParam("start")->value().toInt();
    }
    if (request->hasParam("end")) {
        options.endTime = request->getParam("end")->value().toInt();
    }
    if (request->hasParam("limit")) {
        options.maxRecords = request->getParam("limit")->value().toInt();
    }
    
    // Stream response
    AsyncWebServerResponse* response = request->beginChunkedResponse(
        format == "json" ? "application/json" : "text/csv",
        [options](uint8_t* buffer, size_t maxLen, size_t index) -> size_t {
            // Stream export data in chunks
            // Implementation depends on AsyncWebServer API
            return 0;
        }
    );
    
    request->send(response);
}
```

---

## AccessControl Integration

### Basic Setup
```cpp
#include "src/security/AccessControl.h"

void setup() {
    // Initialize access control
    AccessControl& ac = AccessControl::getInstance();
    ac.init();
    
    // Create users (first run)
    ac.createUser("admin", "admin123", UserRole::ADMIN);
    ac.createUser("operator", "oper123", UserRole::USER);
    ac.createUser("guest", "guest123", UserRole::VIEWER);
    
    // Or load from NVS (subsequent runs)
    ac.loadUsers();
}

// Check permissions
void checkPermissions() {
    AccessControl& ac = AccessControl::getInstance();
    
    // Check by username
    if (ac.hasPermission("operator", Permission::WRITE_CONFIG)) {
        // Allow config write
    }
    
    // Check by role
    if (ac.roleHasPermission(UserRole::VIEWER, Permission::SYSTEM_REBOOT)) {
        // Allow reboot (will be false for viewer)
    }
    
    // Get user role
    UserRole role = ac.getUserRole("operator");
    Serial.printf("User role: %s\n", AccessControl::getRoleName(role));
}

// User management
void manageUsers() {
    AccessControl& ac = AccessControl::getInstance();
    
    // Set user role
    ac.setUserRole("operator", UserRole::ADMIN);
    
    // Enable/disable user
    ac.setUserEnabled("guest", false);
    
    // Delete user
    ac.deleteUser("olduser");
    
    // Get user info
    const UserAccount* account = ac.getUser("operator");
    if (account) {
        Serial.printf("Username: %s\n", account->username.c_str());
        Serial.printf("Role: %s\n", AccessControl::getRoleName(account->role));
        Serial.printf("Enabled: %s\n", account->enabled ? "Yes" : "No");
        Serial.printf("Login count: %d\n", account->loginCount);
    }
    
    // List all users
    std::vector<String> users = ac.getUserList();
    for (const String& username : users) {
        Serial.println(username);
    }
}

// Record login
void handleLogin(const String& username) {
    AccessControl& ac = AccessControl::getInstance();
    ac.recordLogin(username);
}
```

### Web API Integration
```cpp
// In WebServerManager API handlers
void handleAPIRequest(AsyncWebServerRequest* request) {
    AccessControl& ac = AccessControl::getInstance();
    
    // Check permission for endpoint
    if (!ac.checkRequestPermission(request, Permission::READ_CONFIG)) {
        request->send(403, "application/json", "{\"error\":\"Forbidden\"}");
        return;
    }
    
    // Process request
    // ...
}

// Protected endpoint example
void setupProtectedEndpoints() {
    // GET /api/config - Requires READ_CONFIG
    server.on("/api/config", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (!AccessControl::getInstance().checkRequestPermission(
                request, Permission::READ_CONFIG)) {
            request->send(403, "application/json", "{\"error\":\"Forbidden\"}");
            return;
        }
        // Return config...
    });
    
    // POST /api/config - Requires WRITE_CONFIG
    server.on("/api/config", HTTP_POST, [](AsyncWebServerRequest* request) {
        if (!AccessControl::getInstance().checkRequestPermission(
                request, Permission::WRITE_CONFIG)) {
            request->send(403, "application/json", "{\"error\":\"Forbidden\"}");
            return;
        }
        // Update config...
    });
    
    // POST /api/system/reboot - Requires SYSTEM_REBOOT
    server.on("/api/system/reboot", HTTP_POST, [](AsyncWebServerRequest* request) {
        if (!AccessControl::getInstance().checkRequestPermission(
                request, Permission::SYSTEM_REBOOT)) {
            request->send(403, "application/json", "{\"error\":\"Forbidden\"}");
            return;
        }
        ESP.restart();
    });
}
```

---

## Complete Integration Example

Here's a complete example showing all components working together:

```cpp
#include <Arduino.h>
#include "src/comm/WebSocketServer.h"
#include "src/comm/TCPDataServer.h"
#include "src/ui/DisplayManager.h"
#include "src/ui/ButtonHandler.h"
#include "src/storage/ExportManager.h"
#include "src/security/AccessControl.h"

void setup() {
    Serial.begin(115200);
    
    // 1. Initialize Access Control first
    AccessControl::getInstance().init();
    
    // 2. Initialize Display
    DisplayManager& display = DisplayManager::getInstance();
    display.init();
    display.showBootSplash("2.0.1");
    
    // 3. Initialize Button Handler
    ButtonHandler::getInstance().init();
    display.showInitStatus("Buttons", true);
    delay(300);
    
    // 4. Initialize WebSocket Server
    // (Assumes WebServerManager is already initialized)
    WebSocketServer::getInstance().init(/* ws pointer */);
    WebSocketServer::getInstance().begin();
    display.showInitStatus("WebSocket", true);
    delay(300);
    
    // 5. Initialize TCP Data Server
    TCPDataServer::getInstance().init(8088);
    TCPDataServer::getInstance().begin();
    display.showInitStatus("TCP Server", true);
    delay(300);
    
    // 6. Initialize Export Manager
    ExportManager::getInstance().init();
    display.showInitStatus("Export", true);
    delay(300);
    
    // Enable auto page rotation
    display.setAutoRotation(true, 5000);
    
    Serial.println("All components initialized!");
}

void loop() {
    // Update all components
    DisplayManager::getInstance().update();
    ButtonHandler::getInstance().update();
    WebSocketServer::getInstance().update();
    TCPDataServer::getInstance().update();
    
    // Update meter data periodically
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate >= 1000) {
        lastUpdate = millis();
        
        // Get meter data from energy meter
        MeterData data;
        // ... populate from ATM90E36 ...
        
        // Push to all services
        DisplayManager::getInstance().updateMeterData(data);
        WebSocketServer::getInstance().pushMeterData(data);
        TCPDataServer::getInstance().updateMeterData(data);
    }
    
    // Update status info
    static unsigned long lastStatusUpdate = 0;
    if (millis() - lastStatusUpdate >= 5000) {
        lastStatusUpdate = millis();
        
        DisplayManager& display = DisplayManager::getInstance();
        display.setNetworkInfo("192.168.1.100", -65, true);
        display.setSystemInfo(
            millis() / 1000,
            25.5,
            ESP.getFreeHeap(),
            0
        );
    }
}
```

---

## Notes

1. **Initialization Order**: Initialize components in this order:
   - AccessControl (security foundation)
   - DisplayManager (user feedback)
   - ButtonHandler (user input)
   - WebSocketServer (real-time web data)
   - TCPDataServer (legacy protocol)
   - ExportManager (data export)

2. **Thread Safety**: DisplayManager uses mutexes for thread-safe operations. When calling from ISRs or other threads, use lock()/unlock() or rely on the built-in mutex protection.

3. **Memory Management**: 
   - WebSocketServer limits to 4 clients
   - TCPDataServer limits to 4 clients
   - ExportManager can stream large datasets to avoid memory issues

4. **Error Handling**: All components use the Logger system. Check logs for detailed error information.

5. **Performance**: 
   - WebSocketServer pushes at 1Hz (configurable)
   - TCPDataServer responds on-demand
   - DisplayManager updates are non-blocking
   - ButtonHandler uses hardware interrupts for responsiveness

---

## Troubleshooting

### WebSocket Issues
- **Client can't connect**: Check firewall, verify WebServerManager is running
- **No data received**: Ensure pushMeterData() is being called with valid data
- **Disconnects**: Check client count (max 4), network stability

### TCP Server Issues
- **VB.NET can't connect**: Verify port 8088 is open, check IP address
- **Wrong data**: Ensure updateMeterData() is called before client requests
- **Handshake fails**: Check for /SYNC/ in client

### Display Issues
- **Blank screen**: Check I2C connections, verify LCDManager init
- **Page doesn't change**: Check button handler, verify update() is called
- **Frozen display**: Avoid blocking operations, use non-blocking timing

### Button Issues
- **No response**: Check pin definitions, verify ISR is attached
- **Multiple triggers**: Adjust debounce threshold
- **Factory reset too easy**: Increase threshold (default 10s)

### Export Issues
- **Out of memory**: Use file export instead of string, limit maxRecords
- **Empty export**: Check source file path, verify data logger is writing
- **Wrong date range**: Ensure timestamps in log file are valid Unix timestamps

### Access Control Issues
- **Permission denied**: Check user role, verify permission mapping
- **Can't login**: Check credentials, ensure user is enabled
- **Lost admin access**: Use serial console to create new admin user

---

## API Reference Summary

See individual header files for complete API documentation:
- `src/comm/WebSocketServer.h`
- `src/comm/TCPDataServer.h`
- `src/ui/DisplayManager.h`
- `src/ui/ButtonHandler.h`
- `src/storage/ExportManager.h`
- `src/security/AccessControl.h`
