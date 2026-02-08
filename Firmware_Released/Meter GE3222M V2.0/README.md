# SM-GE3222M V2.0 - Desktop Application

## Overview

Modern VB.NET desktop application for SM-GE3222M Smart Energy Monitor with support for both V1 and V2 firmware protocols.

## Architecture

### Project Structure

```
Meter GE3222M V2.0/
├── MeterGE3222M.Core/          # Shared library (models, services, communication)
│   ├── Models/
│   │   ├── PhaseData.vb       # ✅ Single phase data model
│   │   ├── MeterSnapshot.vb   # ✅ Complete meter snapshot with V1/V2 parsers
│   │   ├── CalibrationData.vb # 🚧 Calibration data model
│   │   ├── MeterConfig.vb     # 🚧 Configuration model
│   │   └── LogEntry.vb        # 🚧 Log entry model
│   ├── Communication/
│   │   ├── IMeterConnection.vb      # 🚧 Transport interface
│   │   ├── TCPTransport.vb          # 🚧 Async TCP transport
│   │   ├── SerialTransport.vb       # 🚧 COM port transport
│   │   ├── WebSocketTransport.vb    # 🚧 WebSocket transport (V2)
│   │   ├── ProtocolParser.vb        # 🚧 V1/V2 auto-detect
│   │   ├── ProtocolV1Parser.vb      # 🚧 Tag:Value parser
│   │   ├── ProtocolV2Parser.vb      # 🚧 JSON parser
│   │   └── CommandSender.vb         # 🚧 Command with ACK/retry
│   ├── Services/
│   │   ├── MeterDataService.vb      # 🚧 Real-time data management
│   │   ├── CalibrationService.vb    # 🚧 Calibration workflow
│   │   ├── ConfigurationService.vb  # 🚧 Config read/write
│   │   └── DataLoggerService.vb     # 🚧 CSV/JSON export
│   └── Helpers/
│       ├── FloatConverter.vb        # 🚧 IEEE754 ↔ Modbus
│       └── ChecksumHelper.vb        # 🚧 CRC/checksum utilities
├── MeterGE3222M.App/           # WinForms Application
│   ├── ViewModels/
│   │   ├── DashboardViewModel.vb    # 🚧 Main dashboard VM
│   │   ├── CalibrationViewModel.vb  # 🚧 Calibration VM
│   │   ├── SettingsViewModel.vb     # 🚧 Settings VM
│   │   └── DataLogViewModel.vb      # 🚧 Data logging VM
│   ├── Views/
│   │   ├── MainDashboard.vb         # 🚧 MDI parent with navigation
│   │   ├── LiveMonitorView.vb       # 🚧 Real-time gauges
│   │   ├── TrendChartView.vb        # 🚧 Historical trends
│   │   ├── PhaseDetailView.vb       # 🚧 Per-phase deep dive
│   │   ├── EnergyView.vb            # 🚧 Energy accumulators
│   │   ├── CalibrationWizard.vb     # 🚧 Step-by-step calibration
│   │   ├── SettingsView.vb          # 🚧 Tabbed settings
│   │   ├── DataLogView.vb           # 🚧 Log viewer + export
│   │   └── ConnectionView.vb        # 🚧 Connection manager
│   ├── Controls/
│   │   ├── PhaseGauge.vb            # 🚧 Phase gauge control
│   │   ├── PowerBar.vb              # 🚧 Power bar control
│   │   ├── ConnectionIndicator.vb   # 🚧 Connection status
│   │   └── RegisterEditor.vb        # 🚧 Register editor
│   └── Program.vb                   # 🚧 Application entry point
└── MeterGE3222M.Tests/         # Unit Tests
    ├── ProtocolParserTests.vb       # 🚧 Protocol parsing tests
    ├── MeterDataServiceTests.vb     # 🚧 Service tests
    └── FloatConverterTests.vb       # 🚧 Converter tests
```

## Key Features

### ✅ Implemented
- **PhaseData Model**: Complete single-phase data structure
- **MeterSnapshot Model**: 3-phase snapshot with:
  - `FromTagValueMap()` - Tag-based V1 parsing (replaces line-number indexing)
  - `FromJson()` - V2 JSON parsing
  - `ParseData()` - Auto-detect V1 vs V2 protocol
  - Regex-based tag extraction for robust parsing

### 🚧 To Be Implemented
- **Transport Layer**: TCP, Serial, WebSocket
- **Service Layer**: Data management, calibration, configuration
- **UI Layer**: WinForms views with modern controls
- **Data Logging**: CSV/JSON export with timestamp
- **Calibration Wizard**: Step-by-step calibration workflow

## Protocol Support

### V1 Protocol (TCP Port 8088)
**Backward Compatible with V1.0 Firmware**

Tag:Value ASCII protocol:
```
AE1230.5
AP11150.0
...
```

**Key Improvement**: Uses tag-based dictionary parsing instead of V1.0's fragile line-number indexing:
```vb
' OLD V1.0 (brittle):
Dim voltage = TBox.Lines(58).Substring(0,3)  ' Fixed line 58!

' NEW V2.0 (robust):
Dim tagMap = ParseTagValueData(data)
Dim voltage = tagMap("AP4")  ' Tag-based lookup
```

### V2 Protocol (TCP Port 8089)
**New Modern Protocol**

JSON structured with ACK/NACK:
```json
{
  "type": "data",
  "seq": 123,
  "timestamp": 1234567890,
  "phases": {
    "A": {"V": 230.5, "I": 10.2, "P": 1150, ...}
  }
}
```

## Getting Started

### Prerequisites
- Visual Studio 2019 or later
- .NET Framework 4.7.2 or .NET 5.0+
- SM-GE3222M hardware with V1.0 or V2.0 firmware

### Build

1. Open `MeterGE3222M.sln` in Visual Studio
2. Restore NuGet packages
3. Build solution (Ctrl+Shift+B)
4. Run (F5)

### Configuration

#### TCP Connection
1. Select "TCP" transport
2. Enter meter IP address
3. Enter port (8088 for V1, 8089 for V2)
4. Click "Connect"

#### Serial Connection
1. Select "Serial" transport
2. Select COM port
3. Set baudrate (115200 default)
4. Click "Connect"

## Migration from V1.0 Application

### Key Improvements

1. **Robust Parsing**
   - V1.0: Fixed line-number parsing (`TBox.Lines(58)`) - breaks if format changes
   - V2.0: Tag-based dictionary parsing - resilient to reordering

2. **Protocol Detection**
   - Automatically detects V1 (Tag:Value) or V2 (JSON)
   - Seamless support for both firmware versions

3. **Async Communication**
   - Non-blocking TCP/Serial operations
   - No UI freezing during communication

4. **Modern Architecture**
   - MVVM pattern for testability
   - Separation of concerns (Model/View/Service)
   - Unit testable components

### Migration Steps

1. **Install V2.0 Application**
2. **Configure Connection**
   - TCP to V1 firmware: Port 8088
   - TCP to V2 firmware: Port 8089
   - Application auto-detects protocol

3. **Import V1 Settings**
   - Export config from V1 app
   - Import JSON to V2 app

## Data Models

### PhaseData
Single phase measurement with:
- RMS values (voltage, current)
- Power (active, reactive, apparent, PF)
- Phase angles
- THD
- Fundamental/Harmonic power
- Energy accumulators (forward/reverse, active/reactive)

### MeterSnapshot
Complete 3-phase snapshot:
- PhaseA, PhaseB, PhaseC data
- Total power and energy
- Neutral current
- Frequency
- Board temperature, humidity
- Status registers
- Timestamp, sequence number

## Communication

### Tag:Value Protocol Parser
```vb
' Parse V1 protocol
Dim data As String = GetDataFromMeter()
Dim snapshot As MeterSnapshot = MeterSnapshot.ParseData(data)

' Access phase data
Dim voltageA = snapshot.PhaseA.VoltageRMS
Dim powerA = snapshot.PhaseA.ActivePower
```

### JSON Protocol Parser
```vb
' Parse V2 protocol
Dim json As String = GetJsonFromMeter()
Dim snapshot As MeterSnapshot = MeterSnapshot.FromJson(json)

' Same access pattern
Dim voltageA = snapshot.PhaseA.VoltageRMS
```

## Development

### Adding New Features

1. **New Model**: Add to `MeterGE3222M.Core/Models/`
2. **New Service**: Add to `MeterGE3222M.Core/Services/`
3. **New View**: Add to `MeterGE3222M.App/Views/`
4. **New ViewModel**: Add to `MeterGE3222M.App/ViewModels/`

### Coding Conventions
- Use `Option Strict On`
- XML documentation for public members
- Async/await for I/O operations
- INotifyPropertyChanged for data binding
- Dispose pattern for resources

### Testing
```bash
# Run all tests
dotnet test

# Run specific test
dotnet test --filter ProtocolParserTests
```

## Troubleshooting

### Connection Issues
- Verify meter IP address
- Check firewall settings
- Ensure correct port (8088 or 8089)
- Try ping to verify network connectivity

### Parsing Errors
- Check firmware version (V1 or V2)
- Verify protocol format
- Enable debug logging

### Data Not Updating
- Check connection status
- Verify meter is sending data
- Check publish interval in meter settings

## Known Limitations

- Maximum 1 active connection per transport
- TCP timeout: 10 seconds
- Serial baudrate: 115200 max
- Data update rate: 1Hz typical

## Roadmap

### Version 2.0
- [x] Core models (PhaseData, MeterSnapshot)
- [x] Protocol parsers (V1/V2)
- [ ] Complete transport layer
- [ ] Service layer implementation
- [ ] UI views and controls
- [ ] Data logging

### Version 2.1
- [ ] Advanced charting (trends, harmonics)
- [ ] Report generation (PDF, Excel)
- [ ] Multi-meter support
- [ ] Cloud synchronization

## Support

For issues:
- Check firmware version compatibility
- Review connection settings
- Enable debug logging
- Create GitHub issue with logs

## License

Same as main repository.

## Credits

- Original V1.0 application: Microcode Engineering
- V2.0 modernization: Architecture improvement initiative
