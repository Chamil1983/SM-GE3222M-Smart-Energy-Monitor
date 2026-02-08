# SM-GE3222M VB.NET Desktop Application - V2.0

## Overview

This directory contains the **V2.0 modernization** of the VB.NET desktop monitoring application, representing a complete refactoring from the monolithic V1.0 codebase to a clean, maintainable, async/await-based architecture.

**Status:** 🚧 Architecture Complete - Implementation Pending

---

## Key Improvements Over V1.0

### Problem: Monolithic God-Class (200KB)
**V1.0:** `FrmSMGE_3222MLoging.vb` contained ALL logic in one 5000+ line file.

**V2.0:** Separated into ~20 focused classes:
- **Models:** Data structures with INotifyPropertyChanged
- **Services:** Business logic (data coordination, logging, alerts)
- **Communication:** Async TCP with state machine
- **Views:** Focused UI forms
- **Controls:** Reusable custom controls

### Problem: Fragile Line-Index Parsing
**V1.0:** Hardcoded line numbers that break if protocol changes:
```vb
StrSerialInRam = TBox.Lines(51).Substring(0, 3)  ' Magic number 51!
If StrSerialInRam = "TP1" Then TP_1 = TBox.Lines(51)
```

**V2.0:** Robust tag-dictionary parsing:
```vb
Dim tags = ProtocolParser.ParseFrame(rawData)
Dim snapshot = MeterSnapshot.FromTagDictionary(tags)
' Position-independent, extensible, validated
```

### Problem: Blocking TCP (UI Freezes)
**V1.0:** Synchronous calls on UI thread causing 15+ second freezes:
```vb
client.Connect(hostname, port)  ' UI frozen!
```

**V2.0:** Async/await pattern keeps UI responsive:
```vb
Await connection.ConnectAsync(ipAddress, port)  ' UI stays responsive
```

### Problem: Dual TCP Implementations
**V1.0:** `TCP.vb` AND `TCPControl.vb` - inconsistent behavior.

**V2.0:** Single `MeterConnection.vb` with event-driven architecture.

---

## Architecture

See `ARCHITECTURE.md` for complete details:
- Communication state machine
- Data models with INotifyPropertyChanged
- Service layer architecture
- Async/await patterns
- Custom control implementations

---

## Project Structure

```
Meter GE3222M V2.0/
├── Meter_GE3222M_V2.0.sln
├── Meter_GE3222M_V2.0.vbproj
├── ARCHITECTURE.md              # Complete architecture specification
├── README.md                    # This file
│
├── Models/                      # Data models
│   ├── PhaseData.vb
│   ├── MeterSnapshot.vb
│   ├── CalibrationData.vb
│   ├── MeterConfig.vb
│   └── ConnectionInfo.vb
│
├── Communication/               # TCP client and protocol
│   ├── MeterConnection.vb       # Async TCP client with state machine
│   ├── ProtocolParser.vb        # Tag-dictionary parser
│   ├── CommandSender.vb         # Command sender
│   └── ReconnectManager.vb      # Exponential backoff reconnection
│
├── Services/                    # Business logic
│   ├── MeterDataService.vb      # Central data coordinator
│   ├── CalibrationService.vb    # Calibration management
│   ├── DataLoggerService.vb     # SQLite logging
│   ├── AlertService.vb          # Threshold monitoring
│   ├── ConfigService.vb         # App settings
│   └── ReportService.vb         # PDF/Excel export
│
├── Views/                       # UI forms
│   ├── MainDashboard.vb         # MDI parent
│   ├── LiveMonitorView.vb       # Real-time display
│   ├── EnergyDetailView.vb      # Energy tracking
│   ├── PowerDetailView.vb       # Power analysis
│   ├── HarmonicsView.vb         # THD analysis
│   ├── CalibrationWizard.vb     # Calibration wizard
│   ├── SettingsView.vb          # Settings
│   ├── DataLoggerView.vb        # Historical data
│   └── DiagnosticsView.vb       # Device health
│
├── Controls/                    # Custom controls
│   ├── GaugeControl.vb          # Circular gauge
│   ├── PhaseBarControl.vb       # 3-phase balance bar
│   ├── ConnectionStatusBar.vb   # Connection status
│   └── DoubleBufferedPanel.vb   # Flicker-free panel
│
├── Utilities/                   # Helper classes
│   ├── MeterConstants.vb        # Protocol constants
│   ├── UnitConverter.vb         # Unit conversions
│   ├── ValidationHelper.vb      # Input validation
│   └── ExportHelper.vb          # CSV/Excel export
│
└── Resources/                   # Assets
    ├── Icons/                   # Application icons
    │   ├── app_icon.ico
    │   ├── connected.png
    │   ├── disconnected.png
    │   └── connecting.png
    └── Themes/                  # Color schemes
        ├── LightTheme.xml
        └── DarkTheme.xml
```

---

## Dependencies

### NuGet Packages
```xml
<packages>
  <!-- SQLite for data logging -->
  <package id="System.Data.SQLite" version="1.0.118" targetFramework="net48" />
  
  <!-- JSON serialization -->
  <package id="Newtonsoft.Json" version="13.0.3" targetFramework="net48" />
  
  <!-- Excel export -->
  <package id="EPPlus" version="6.2.4" targetFramework="net48" />
  
  <!-- PDF generation -->
  <package id="iTextSharp" version="5.5.13.3" targetFramework="net48" />
</packages>
```

### .NET Framework
- Target: .NET Framework 4.8
- Language: Visual Basic .NET

---

## Quick Start

### Development Setup

1. **Install Visual Studio 2019/2022**
   - Workload: ".NET desktop development"
   - Language: Visual Basic

2. **Open Solution**
   ```
   Meter_GE3222M_V2.0.sln
   ```

3. **Restore NuGet Packages**
   - Right-click solution → "Restore NuGet Packages"

4. **Build Solution**
   - Build → Build Solution (Ctrl+Shift+B)

5. **Run Application**
   - Debug → Start Debugging (F5)

### First Connection

1. Launch application
2. Click "File" → "Connect"
3. Enter meter IP address (e.g., `192.168.1.100`) and port (`8088`)
4. Click "Connect"
5. Data should start streaming to Live Monitor view

---

## Implementation Priority

### Week 1: Models & Communication
1. Create data models (PhaseData, MeterSnapshot)
2. Implement ProtocolParser (tag-dictionary parsing)
3. Implement MeterConnection (async TCP client)
4. Test: Connect to V1.0 firmware, parse data

### Week 2: Views & UI
1. Create MainDashboard (MDI parent)
2. Create LiveMonitorView with data binding
3. Implement GaugeControl
4. Test: Display live data from meter

### Week 3: Services
1. Implement MeterDataService (coordinator)
2. Implement DataLoggerService (SQLite)
3. Implement AlertService (threshold monitoring)
4. Test: Data logging and alerts

### Week 4: Advanced Features
1. Implement CalibrationWizard
2. Implement ReportService (PDF/Excel export)
3. Create remaining views (Energy, Power, Harmonics, etc.)
4. Test: Full application workflow

### Week 5: Polish & Testing
1. Add error handling throughout
2. Implement reconnection logic
3. Performance optimization
4. User acceptance testing

---

## Migration from V1.0

### Export V1.0 Configuration
1. Open V1.0 application
2. Connect to meter
3. Navigate to Calibration → Export
4. Save calibration data as JSON file

### Import to V2.0
1. Open V2.0 application
2. Navigate to Tools → Import Calibration
3. Select exported JSON file
4. Verify calibration data

### Data Migration
- Historical data from V1.0 is **not automatically migrated**
- Use V1.0 to export historical data to CSV
- Import CSV into V2.0 via Data Logger → Import

---

## Testing

### Unit Tests
```vb
<TestClass>
Public Class ProtocolParserTests
    <TestMethod>
    Public Sub ParseFrame_ValidData_ReturnsCorrectDictionary()
        ' Test implementation
    End Sub
    
    <TestMethod>
    Public Sub MeterSnapshot_FromTagDictionary_CorrectlyMapsValues()
        ' Test implementation
    End Sub
End Class
```

### Integration Tests
- Connect to actual meter hardware
- Verify data parsing accuracy
- Test all command codes
- Validate reconnection logic

### UI Tests
- Manual testing of all views
- Responsiveness during connection/disconnection
- Data refresh performance
- Memory leak detection

---

## Protocol Compatibility

### V1.0 Protocol (Maintained)
The application maintains **full backward compatibility** with V1.0 firmware:
- TCP port 8088
- Tag format: `TAG:VALUE\n`
- All command codes supported
- Handshake: `/SYNC/`

### V2.0 Enhancements (Optional)
When connected to V2.0 firmware, additional features available:
- REST API endpoints (HTTP)
- WebSocket real-time updates
- MQTT subscription
- Extended diagnostics

---

## Coding Standards

### Naming Conventions
- **Classes:** PascalCase (e.g., `MeterConnection`)
- **Methods:** PascalCase (e.g., `ConnectAsync`)
- **Properties:** PascalCase (e.g., `IPAddress`)
- **Variables:** camelCase (e.g., `ipAddress`)
- **Private fields:** underscore prefix (e.g., `_connection`)
- **Constants:** UPPER_SNAKE_CASE (e.g., `DEFAULT_PORT`)

### Documentation
- XML documentation comments for all public members
- Inline comments for complex logic
- README.md for module overview

### Error Handling
- Use Try/Catch for all async operations
- Provide meaningful error messages
- Log errors for diagnostics

### Async/Await
- All network operations must be async
- Use `Async` suffix for async methods
- Always configure await (`ConfigureAwait(False)`) in library code

---

## Performance Targets

- **Connection Time:** <5 seconds
- **Data Update Rate:** 1 Hz minimum (configurable)
- **UI Responsiveness:** 60 FPS (no freezing)
- **Memory Usage:** <100 MB typical
- **Database Growth:** ~1 MB/day at 60s intervals

---

## Future Enhancements (V2.1+)

- [ ] Multi-meter support (monitor multiple devices simultaneously)
- [ ] Cloud synchronization (Azure/AWS backup)
- [ ] Mobile companion app (Xamarin/MAUI)
- [ ] Advanced charting (LiveCharts integration)
- [ ] Energy cost calculator with tariff support
- [ ] Predictive maintenance with ML
- [ ] Automated scheduled reports (email/PDF)
- [ ] REST API client for V2.0 firmware endpoints

---

## Support

- **Issues:** GitHub issue tracker
- **Documentation:** ARCHITECTURE.md
- **Email:** support@microcodeeng.com

---

## License

MIT License - See LICENSE file for details

---

**Project Status:** Architecture Complete - Ready for Implementation  
**Version:** 2.0.0-alpha  
**Last Updated:** 2026-02-08  
**Maintainer:** Microcode Engineering
