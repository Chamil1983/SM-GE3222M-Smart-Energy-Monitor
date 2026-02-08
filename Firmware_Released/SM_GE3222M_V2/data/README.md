# SM-GE3222M V2.0 Dashboard Files

Modern responsive web dashboard for the 3-phase SM-GE3222M energy monitor.

## Files

### index.html
- Responsive 3-phase energy monitor dashboard
- Real-time display with WebSocket updates
- Mobile-first responsive design
- Includes:
  - 3 voltage gauges with color-coded phases
  - Current, power, and power factor displays
  - Energy counters (import/export)
  - System status (uptime, heap, WiFi)

### dashboard.js
- WebSocket connection to `/ws` endpoint
- Automatic reconnection with exponential backoff
- Parses JSON meter data from WebSocket
- Falls back to `/api/meter` HTTP endpoint on load
- Updates dashboard every 1 second via WebSocket
- Handles connection/disconnection gracefully

### style.css
- Modern dark theme with gradient backgrounds
- Phase color coding:
  - Phase A: Red (#e74c3c)
  - Phase B: Yellow (#f39c12)
  - Phase C: Blue (#3498db)
- CSS Grid and Flexbox layout
- Mobile responsive breakpoints
- Smooth animations and transitions
- Card-based design with hover effects

## Features

- **Real-time monitoring** via WebSocket
- **3-phase voltage gauges** with visual arc representation
- **Power metrics**: Active, Reactive, Apparent power for all phases
- **Energy counters**: Import/Export tracking
- **System info**: Frequency, temperature, uptime, WiFi status
- **Responsive design**: Works on desktop, tablet, and mobile
- **Auto-reconnect**: Handles connection drops gracefully

## Usage

1. Upload these files to the ESP32's SPIFFS filesystem in the `/data` directory
2. Access the dashboard via the device's IP address
3. WebSocket will connect automatically and display real-time data

## Data Format

The dashboard expects JSON data with fields like:
- `UrmsA`, `UrmsB`, `UrmsC` - Phase voltages (V)
- `IrmsA`, `IrmsB`, `IrmsC` - Phase currents (A)
- `PmeanA`, `PmeanB`, `PmeanC`, `PmeanT` - Active power (kW)
- `QmeanA`, `QmeanB`, `QmeanC`, `QmeanT` - Reactive power (kVAR)
- `SmeanA`, `SmeanB`, `SmeanC`, `SAmeanT` - Apparent power (kVA)
- `PFmeanA`, `PFmeanB`, `PFmeanC`, `PFmeanT` - Power factor
- `Freq` - Frequency (Hz)
- `Temp` - Temperature (°C)
- `APenergyT`, `ANenergyT` - Energy counters (kWh)
- And more...

Compatible with SM-GE3222M V1.0 data format.
