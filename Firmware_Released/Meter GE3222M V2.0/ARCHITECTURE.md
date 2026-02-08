# SM-GE3222M VB.NET Desktop Application V2.0 Architecture

## Overview

The V2.0 desktop application represents a complete refactoring from the monolithic V1.0 codebase to a modern, layered Model-View-ViewModel (MVVM-like) architecture. It replaces fragile line-index parsing with robust tag-dictionary parsing and implements async/await patterns to eliminate UI freezing.

---

## Application Architecture

```
┌──────────────────────────────────────────────────────────────────┐
│                        Presentation Layer                        │
│  ┌──────────────────────────────────────────────────────────────┐│
│  │              MainDashboard (MDI Parent)                      ││
│  │  ┌──────────────┐ ┌──────────────┐ ┌────────────────────┐  ││
│  │  │ MenuStrip    │ │ StatusStrip  │ │ TabControl         │  ││
│  │  │ - File       │ │ - Connection │ │ - LiveMonitor      │  ││
│  │  │ - Tools      │ │ - IP/Signal  │ │ - EnergyDetail     │  ││
│  │  │ - Help       │ │ - Heap/Time  │ │ - PowerDetail      │  ││
│  │  └──────────────┘ └──────────────┘ │ - Harmonics        │  ││
│  │                                     │ - Calibration      │  ││
│  │                                     │ - DataLogger       │  ││
│  │                                     │ - Diagnostics      │  ││
│  │                                     └────────────────────┘  ││
│  └──────────────────────────────────────────────────────────────┘│
└──────────────────────────────────────────────────────────────────┘
                              ↓
┌──────────────────────────────────────────────────────────────────┐
│                         Service Layer                            │
│  ┌────────────────────┐  ┌──────────────────────────────────┐   │
│  │ MeterDataService   │  │ CalibrationService               │   │
│  │ - Data coordinator │  │ - Cal read/write/validate        │   │
│  └────────────────────┘  └──────────────────────────────────┘   │
│  ┌────────────────────┐  ┌──────────────────────────────────┐   │
│  │ DataLoggerService  │  │ AlertService                     │   │
│  │ - SQLite logging   │  │ - Threshold monitoring           │   │
│  └────────────────────┘  └──────────────────────────────────┘   │
│  ┌────────────────────┐  ┌──────────────────────────────────┐   │
│  │ ConfigService      │  │ ReportService                    │   │
│  │ - App settings     │  │ - PDF/Excel export               │   │
│  └────────────────────┘  └──────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────────┘
                              ↓
┌──────────────────────────────────────────────────────────────────┐
│                      Communication Layer                         │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │             MeterConnection (Async TCP Client)             │ │
│  │  - State machine (Disconnected/Connecting/Connected/...)   │ │
│  │  - Auto-reconnect with exponential backoff                 │ │
│  │  - Event-driven (OnDataReceived, OnError, etc.)            │ │
│  └────────────────────────────────────────────────────────────┘ │
│  ┌──────────────┐  ┌──────────────┐  ┌────────────────────┐   │
│  │ProtocolParser│  │ CommandSender│  │ ReconnectManager   │   │
│  │- Tag→Dict    │  │- Async send  │  │- Exp. backoff      │   │
│  └──────────────┘  └──────────────┘  └────────────────────┘   │
└──────────────────────────────────────────────────────────────────┘
                              ↓
┌──────────────────────────────────────────────────────────────────┐
│                          Data Models                             │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │ PhaseData │ MeterSnapshot │ CalibrationData │ MeterConfig│   │
│  │ (All implement INotifyPropertyChanged for data binding)   │   │
│  └──────────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────────┘
```

---

## Key Problems Solved from V1.0

### Problem 1: Monolithic God-Class (FrmSMGE_3222MLoging.vb - 200KB)

**V1.0 Issues:**
- Single form with ALL logic: TCP, parsing, UI, calibration, logging
- 5000+ lines of spaghetti code
- Impossible to test or maintain

**V2.0 Solution:**
- **Separation of Concerns:** Models, Views, Services, Communication
- Each class has single responsibility
- ~20 focused classes replacing 1 monolith

### Problem 2: Fragile Line-Index Parsing

**V1.0 Approach (FRAGILE!):**
```vb
' Hardcoded line numbers - breaks if protocol changes!
StrSerialInRam = TBox.Lines(51).Substring(0, 3)
If StrSerialInRam = "TP1" Then
    TP_1 = TBox.Lines(51)  ' Magic number 51
End If

StrSerialInRam = TBox.Lines(52).Substring(0, 3)
If StrSerialInRam = "TP2" Then
    TP_2 = TBox.Lines(52)  ' Magic number 52
End If
' ...repeated 70+ times with hardcoded indices
```

**V2.0 Solution (ROBUST!):**
```vb
' ProtocolParser.vb - Position-independent, extensible
Public Function ParseFrame(rawData As String) As Dictionary(Of String, Single)
    Dim values As New Dictionary(Of String, Single)
    
    For Each line In rawData.Split(vbLf)
        Dim parts = line.Trim().Split(":"c, 2)
        If parts.Length = 2 Then
            Dim tag As String = parts(0).Trim()
            Dim value As Single
            If Single.TryParse(parts(1), value) Then
                values(tag) = value
            End If
        End If
    Next
    
    Return values  ' Tag→Value dictionary
End Function

' MeterSnapshot.vb - Factory method
Public Shared Function FromTagDictionary(tags As Dictionary(Of String, Single)) As MeterSnapshot
    Dim snapshot As New MeterSnapshot With {
        .PhaseA = New PhaseData With {
            .Voltage = GetTagValue(tags, "AE1"),
            .Current = GetTagValue(tags, "AE2"),
            ' ...
        },
        .TotalActivePower = GetTagValue(tags, "AP1"),
        .Frequency = GetTagValue(tags, "TE1"),
        .Timestamp = DateTime.Now
    }
    Return snapshot
End Function
```

### Problem 3: Blocking TCP (UI Freezes)

**V1.0 Issue:**
```vb
' Synchronous blocking call on UI thread!
client.Connect(hostname, port)  ' UI freezes for 15+ seconds
```

**V2.0 Solution:**
```vb
' MeterConnection.vb - Async/Await pattern
Public Async Function ConnectAsync(ipAddress As String, port As Integer) As Task(Of Boolean)
    Try
        _state = ConnectionState.Connecting
        RaiseEvent OnStateChanged(_state)
        
        _client = New TcpClient()
        Dim connectTask = _client.ConnectAsync(ipAddress, port)
        Dim timeoutTask = Task.Delay(TimeSpan.FromSeconds(10))
        
        Dim completedTask = Await Task.WhenAny(connectTask, timeoutTask)
        
        If completedTask Is timeoutTask Then
            ' Timeout occurred
            _client.Close()
            _state = ConnectionState.Error
            RaiseEvent OnError(ErrorCode.TCP_CONNECT_TIMEOUT, "Connection timeout")
            Return False
        End If
        
        ' Connection successful
        _state = ConnectionState.Syncing
        RaiseEvent OnConnected()
        
        ' Start receive loop on background thread
        Task.Run(AddressOf ReceiveLoop)
        
        Return True
        
    Catch ex As Exception
        _state = ConnectionState.Error
        RaiseEvent OnError(ErrorCode.TCP_CONNECT_FAILED, ex.Message)
        Return False
    End Try
End Function

' UI remains responsive throughout!
```

### Problem 4: Dual TCP Implementations

**V1.0 Issue:**
- `TCP.vb` AND `TCPControl.vb` - two different TCP implementations!
- Inconsistent behavior
- Code duplication

**V2.0 Solution:**
- Single `MeterConnection.vb` - one source of truth
- Event-driven architecture
- Consistent error handling

---

## Communication State Machine

```
┌──────────────┐
│ DISCONNECTED │ ◀────────────────────────────────────┐
└──────────────┘                                       │
       │                                               │
       │ User clicks "Connect"                         │
       ↓                                               │
┌──────────────┐                                       │
│  CONNECTING  │ ──timeout/error──────────────────────┤
└──────────────┘                                       │
       │                                               │
       │ TCP connected                                 │
       ↓                                               │
┌──────────────┐                                       │
│   SYNCING    │ ──no /SYNC/ received─────────────────┤
└──────────────┘                                       │
       │                                               │
       │ Received /SYNC/                               │
       ↓                                               │
┌──────────────┐                                       │
│  CONNECTED   │ ◀───────┐                             │
└──────────────┘         │                             │
       │                 │                             │
       │ Send NewReq     │ Received Code_End           │
       ↓                 │                             │
┌──────────────┐         │                             │
│RECEIVING_DATA│ ────────┘                             │
└──────────────┘                                       │
       │                                               │
       │ Error/Disconnect                              │
       ↓                                               │
┌──────────────┐                                       │
│    ERROR     │                                       │
└──────────────┘                                       │
       │                                               │
       │ Auto-retry enabled                            │
       ↓                                               │
┌──────────────┐                                       │
│   RETRYING   │ ──max retries exceeded────────────────┘
└──────────────┘
       │
       │ Exponential backoff (1s→2s→4s→8s→16s max)
       │
       └──────────────────────────────────────────────┘
```

### Reconnection Strategy

```vb
' ReconnectManager.vb
Public Class ReconnectManager
    Private _initialDelay As Integer = 1000  ' 1 second
    Private _maxDelay As Integer = 16000     ' 16 seconds
    Private _maxRetries As Integer = 5
    Private _currentRetry As Integer = 0
    
    Public Async Function TryReconnectAsync(connectFunc As Func(Of Task(Of Boolean))) As Task(Of Boolean)
        _currentRetry = 0
        
        While _currentRetry < _maxRetries
            _currentRetry += 1
            Dim delay = CalculateDelay(_currentRetry)
            
            RaiseEvent OnRetrying(_currentRetry, delay)
            
            Await Task.Delay(delay)
            
            Dim success = Await connectFunc()
            If success Then
                Return True
            End If
        End While
        
        RaiseEvent OnMaxRetriesExceeded()
        Return False
    End Function
    
    Private Function CalculateDelay(attempt As Integer) As Integer
        ' Exponential backoff: 2^(attempt-1) * initialDelay
        ' 1: 1s, 2: 2s, 3: 4s, 4: 8s, 5: 16s
        Dim delay = _initialDelay * Math.Pow(2, attempt - 1)
        Return Math.Min(CInt(delay), _maxDelay)
    End Function
End Class
```

---

## Data Models

### PhaseData.vb
```vb
Public Class PhaseData
    Implements INotifyPropertyChanged
    
    ' Measurement properties
    Private _voltage As Single
    Public Property Voltage As Single
        Get
            Return _voltage
        End Get
        Set(value As Single)
            If _voltage <> value Then
                _voltage = value
                OnPropertyChanged(NameOf(Voltage))
            End If
        End Set
    End Property
    
    Private _current As Single
    Public Property Current As Single
        Get
            Return _current
        End Get
        Set(value As Single)
            If _current <> value Then
                _current = value
                OnPropertyChanged(NameOf(Current))
            End If
        End Set
    End Property
    
    ' ...more properties with INotifyPropertyChanged pattern
    
    Public Event PropertyChanged As PropertyChangedEventHandler Implements INotifyPropertyChanged.PropertyChanged
    
    Protected Sub OnPropertyChanged(propertyName As String)
        RaiseEvent PropertyChanged(Me, New PropertyChangedEventArgs(propertyName))
    End Sub
End Class
```

### MeterSnapshot.vb
```vb
Public Class MeterSnapshot
    Public Property PhaseA As PhaseData
    Public Property PhaseB As PhaseData
    Public Property PhaseC As PhaseData
    
    Public Property TotalActivePower As Single
    Public Property TotalReactivePower As Single
    Public Property TotalApparentPower As Single
    Public Property TotalPowerFactor As Single
    
    Public Property Frequency As Single
    Public Property Temperature As Single
    Public Property NeutralCurrentCalc As Single
    Public Property NeutralCurrentSampled As Single
    
    Public Property FreeHeapKB As Single
    Public Property UptimeSeconds As UInteger
    
    Public Property Timestamp As DateTime
    
    ' Factory method from tag dictionary
    Public Shared Function FromTagDictionary(tags As Dictionary(Of String, Single)) As MeterSnapshot
        Dim snapshot As New MeterSnapshot()
        
        ' Phase A
        snapshot.PhaseA = New PhaseData With {
            .Voltage = GetTagValue(tags, "DP1", 0),
            .Current = GetTagValue(tags, "MD1", 0),
            .ActivePower = GetTagValue(tags, "AP2", 0),
            .ReactivePower = GetTagValue(tags, "BP2", 0),
            .ApparentPower = GetTagValue(tags, "CP2", 0),
            .PowerFactor = GetTagValue(tags, "TP2", 0),
            .VoltageTHDN = GetTagValue(tags, "TP5", 0),
            .CurrentTHDN = GetTagValue(tags, "TP8", 0)
        }
        
        ' Phase B, Phase C (similar pattern)
        ' ...
        
        ' Totals
        snapshot.TotalActivePower = GetTagValue(tags, "AP1", 0)
        snapshot.Frequency = GetTagValue(tags, "TE1", 0)
        snapshot.Temperature = GetTagValue(tags, "TE2", 0)
        snapshot.Timestamp = DateTime.Now
        
        Return snapshot
    End Function
    
    Private Shared Function GetTagValue(tags As Dictionary(Of String, Single), key As String, defaultValue As Single) As Single
        If tags.ContainsKey(key) Then
            Return tags(key)
        Else
            Return defaultValue
        End If
    End Function
End Class
```

---

## Services Layer

### MeterDataService.vb
```vb
Public Class MeterDataService
    Private _connection As MeterConnection
    Private _currentSnapshot As MeterSnapshot
    Private _dataLogger As DataLoggerService
    Private _alertService As AlertService
    
    Public Event OnDataUpdated(snapshot As MeterSnapshot)
    
    Public Sub New(connection As MeterConnection)
        _connection = connection
        _dataLogger = New DataLoggerService()
        _alertService = New AlertService()
        
        ' Subscribe to connection events
        AddHandler _connection.OnDataReceived, AddressOf HandleDataReceived
    End Sub
    
    Private Sub HandleDataReceived(snapshot As MeterSnapshot)
        ' Update current snapshot
        _currentSnapshot = snapshot
        
        ' Trigger data logging
        _dataLogger.LogMeasurement(snapshot)
        
        ' Check alert thresholds
        _alertService.CheckThresholds(snapshot)
        
        ' Notify subscribers
        RaiseEvent OnDataUpdated(snapshot)
    End Sub
    
    Public Function GetCurrentSnapshot() As MeterSnapshot
        Return _currentSnapshot
    End Function
End Class
```

### DataLoggerService.vb
```vb
Public Class DataLoggerService
    Private _dbConnection As SQLiteConnection
    Private _lastLogTime As DateTime
    Private _logInterval As TimeSpan = TimeSpan.FromSeconds(60)
    
    Public Sub New()
        ' Initialize SQLite database
        _dbConnection = New SQLiteConnection("Data Source=meter_data.db;Version=3;")
        _dbConnection.Open()
        
        ' Create table if not exists
        Dim createTable As String = "
            CREATE TABLE IF NOT EXISTS meter_readings (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                timestamp DATETIME NOT NULL,
                phase_a_voltage REAL,
                phase_a_current REAL,
                phase_a_power REAL,
                phase_b_voltage REAL,
                phase_b_current REAL,
                phase_b_power REAL,
                phase_c_voltage REAL,
                phase_c_current REAL,
                phase_c_power REAL,
                total_power REAL,
                frequency REAL,
                temperature REAL
            )"
        
        Using cmd As New SQLiteCommand(createTable, _dbConnection)
            cmd.ExecuteNonQuery()
        End Using
        
        ' Auto-rotate: delete records older than 30 days
        CleanupOldRecords()
    End Sub
    
    Public Sub LogMeasurement(snapshot As MeterSnapshot)
        ' Rate limiting - only log at specified interval
        If DateTime.Now - _lastLogTime < _logInterval Then
            Return
        End If
        
        _lastLogTime = DateTime.Now
        
        Dim insertQuery As String = "
            INSERT INTO meter_readings (
                timestamp, phase_a_voltage, phase_a_current, phase_a_power,
                phase_b_voltage, phase_b_current, phase_b_power,
                phase_c_voltage, phase_c_current, phase_c_power,
                total_power, frequency, temperature
            ) VALUES (
                @timestamp, @pa_v, @pa_i, @pa_p,
                @pb_v, @pb_i, @pb_p,
                @pc_v, @pc_i, @pc_p,
                @total_p, @freq, @temp
            )"
        
        Using cmd As New SQLiteCommand(insertQuery, _dbConnection)
            cmd.Parameters.AddWithValue("@timestamp", snapshot.Timestamp)
            cmd.Parameters.AddWithValue("@pa_v", snapshot.PhaseA.Voltage)
            cmd.Parameters.AddWithValue("@pa_i", snapshot.PhaseA.Current)
            cmd.Parameters.AddWithValue("@pa_p", snapshot.PhaseA.ActivePower)
            ' ... more parameters
            cmd.ExecuteNonQuery()
        End Using
    End Sub
    
    Private Sub CleanupOldRecords()
        Dim cutoffDate = DateTime.Now.AddDays(-30)
        Dim deleteQuery = "DELETE FROM meter_readings WHERE timestamp < @cutoff"
        
        Using cmd As New SQLiteCommand(deleteQuery, _dbConnection)
            cmd.Parameters.AddWithValue("@cutoff", cutoffDate)
            cmd.ExecuteNonQuery()
        End Using
    End Sub
    
    Public Function ExportToCSV(startDate As DateTime, endDate As DateTime, filePath As String) As Boolean
        ' Export data to CSV file
        ' Implementation details...
    End Function
End Class
```

### AlertService.vb
```vb
Public Class AlertService
    Private _notifyIcon As NotifyIcon
    
    ' Alert thresholds
    Public Property OverVoltageThreshold As Single = 260.0
    Public Property UnderVoltageThreshold As Single = 200.0
    Public Property OverCurrentThreshold As Single = 100.0
    Public Property LowPowerFactorThreshold As Single = 0.85
    Public Property FrequencyDeviationThreshold As Single = 0.5  ' ±0.5 Hz
    
    Public Event OnAlert(alertType As String, message As String)
    
    Public Sub New(notifyIcon As NotifyIcon)
        _notifyIcon = notifyIcon
    End Sub
    
    Public Sub CheckThresholds(snapshot As MeterSnapshot)
        ' Check overvoltage
        For Each phase In {snapshot.PhaseA, snapshot.PhaseB, snapshot.PhaseC}
            If phase.Voltage > OverVoltageThreshold Then
                RaiseAlert("Overvoltage", $"Voltage exceeded {OverVoltageThreshold}V: {phase.Voltage:F2}V")
            End If
            
            If phase.Voltage < UnderVoltageThreshold Then
                RaiseAlert("Undervoltage", $"Voltage below {UnderVoltageThreshold}V: {phase.Voltage:F2}V")
            End If
            
            If phase.Current > OverCurrentThreshold Then
                RaiseAlert("Overcurrent", $"Current exceeded {OverCurrentThreshold}A: {phase.Current:F2}A")
            End If
            
            If Math.Abs(phase.PowerFactor) < LowPowerFactorThreshold Then
                RaiseAlert("Low Power Factor", $"Power factor below {LowPowerFactorThreshold}: {phase.PowerFactor:F3}")
            End If
        Next
        
        ' Check frequency deviation
        Dim nominalFreq As Single = 60.0  ' or 50.0 depending on region
        If Math.Abs(snapshot.Frequency - nominalFreq) > FrequencyDeviationThreshold Then
            RaiseAlert("Frequency Deviation", $"Frequency deviation: {snapshot.Frequency:F2} Hz")
        End If
    End Sub
    
    Private Sub RaiseAlert(alertType As String, message As String)
        ' Desktop notification
        _notifyIcon.BalloonTipTitle = $"Meter Alert: {alertType}"
        _notifyIcon.BalloonTipText = message
        _notifyIcon.BalloonTipIcon = ToolTipIcon.Warning
        _notifyIcon.ShowBalloonTip(5000)
        
        ' Event notification
        RaiseEvent OnAlert(alertType, message)
    End Sub
End Class
```

---

## View Layer

### MainDashboard.vb (MDI Parent)
```vb
Public Class MainDashboard
    Private _connection As MeterConnection
    Private _dataService As MeterDataService
    Private _configService As ConfigService
    
    ' Child views
    Private _liveMonitorView As LiveMonitorView
    Private _energyDetailView As EnergyDetailView
    Private _calibrationWizard As CalibrationWizard
    
    Public Sub New()
        InitializeComponent()
        
        ' Initialize services
        _connection = New MeterConnection()
        _dataService = New MeterDataService(_connection)
        _configService = New ConfigService()
        
        ' Setup MDI
        Me.IsMdiContainer = True
        
        ' Create child forms
        _liveMonitorView = New LiveMonitorView(_dataService)
        _liveMonitorView.MdiParent = Me
        _liveMonitorView.Show()
        
        ' Subscribe to connection events
        AddHandler _connection.OnStateChanged, AddressOf UpdateConnectionStatus
        AddHandler _connection.OnError, AddressOf HandleConnectionError
        
        ' Load recent connections
        LoadRecentConnections()
    End Sub
    
    Private Sub MenuConnect_Click(sender As Object, e As EventArgs) Handles MenuConnect.Click
        Dim dialog As New ConnectionDialog(_configService.GetRecentConnections())
        
        If dialog.ShowDialog() = DialogResult.OK Then
            ConnectToMeterAsync(dialog.IPAddress, dialog.Port)
        End If
    End Sub
    
    Private Async Sub ConnectToMeterAsync(ipAddress As String, port As Integer)
        ' Disable UI during connection
        MenuConnect.Enabled = False
        StatusLabel.Text = "Connecting..."
        
        Try
            Dim success = Await _connection.ConnectAsync(ipAddress, port)
            
            If success Then
                StatusLabel.Text = $"Connected to {ipAddress}:{port}"
                StatusLabel.ForeColor = Color.Green
                
                ' Save to recent connections
                _configService.AddRecentConnection(ipAddress, port)
            Else
                StatusLabel.Text = "Connection failed"
                StatusLabel.ForeColor = Color.Red
            End If
            
        Finally
            MenuConnect.Enabled = True
        End Try
    End Sub
    
    Private Sub UpdateConnectionStatus(state As ConnectionState)
        ' Update status strip with connection state
        StatusStrip.Invoke(Sub()
            StatusLabel.Text = state.ToString()
            
            Select Case state
                Case ConnectionState.Connected
                    StatusIcon.Image = My.Resources.GreenIcon
                Case ConnectionState.Connecting, ConnectionState.Retrying
                    StatusIcon.Image = My.Resources.YellowIcon
                Case ConnectionState.Disconnected, ConnectionState.Error
                    StatusIcon.Image = My.Resources.RedIcon
            End Select
        End Sub)
    End Sub
End Class
```

### LiveMonitorView.vb
```vb
Public Class LiveMonitorView
    Private _dataService As MeterDataService
    
    ' Custom gauge controls
    Private _voltageGaugeA As GaugeControl
    Private _voltageGaugeB As GaugeControl
    Private _voltageGaugeC As GaugeControl
    Private _currentGaugeA As GaugeControl
    Private _currentGaugeB As GaugeControl
    Private _currentGaugeC As GaugeControl
    
    ' Data-bound labels
    Private _lblTotalPower As Label
    Private _lblFrequency As Label
    Private _lblPowerFactor As Label
    
    Public Sub New(dataService As MeterDataService)
        InitializeComponent()
        
        _dataService = dataService
        
        ' Subscribe to data updates
        AddHandler _dataService.OnDataUpdated, AddressOf UpdateDisplay
        
        ' Initialize gauges
        InitializeGauges()
    End Sub
    
    Private Sub InitializeGauges()
        ' Voltage gauges (0-300V range)
        _voltageGaugeA = New GaugeControl With {
            .MinValue = 0,
            .MaxValue = 300,
            .YellowZoneStart = 240,
            .RedZoneStart = 260,
            .Label = "Phase A Voltage",
            .Unit = "V"
        }
        PanelPhaseA.Controls.Add(_voltageGaugeA)
        
        ' Current gauges (0-100A range)
        _currentGaugeA = New GaugeControl With {
            .MinValue = 0,
            .MaxValue = 100,
            .YellowZoneStart = 80,
            .RedZoneStart = 90,
            .Label = "Phase A Current",
            .Unit = "A"
        }
        PanelPhaseA.Controls.Add(_currentGaugeA)
        
        ' ... similar for phases B and C
    End Sub
    
    Private Sub UpdateDisplay(snapshot As MeterSnapshot)
        ' Invoke on UI thread
        Me.Invoke(Sub()
            ' Update gauges
            _voltageGaugeA.Value = snapshot.PhaseA.Voltage
            _voltageGaugeB.Value = snapshot.PhaseB.Voltage
            _voltageGaugeC.Value = snapshot.PhaseC.Voltage
            
            _currentGaugeA.Value = snapshot.PhaseA.Current
            _currentGaugeB.Value = snapshot.PhaseB.Current
            _currentGaugeC.Value = snapshot.PhaseC.Current
            
            ' Update labels
            _lblTotalPower.Text = $"{snapshot.TotalActivePower / 1000:F2} kW"
            _lblFrequency.Text = $"{snapshot.Frequency:F2} Hz"
            _lblPowerFactor.Text = $"{snapshot.TotalPowerFactor:F3}"
            
            ' Update timestamp
            _lblLastUpdate.Text = $"Last update: {snapshot.Timestamp:HH:mm:ss}"
        End Sub)
    End Sub
End Class
```

---

## Custom Controls

### GaugeControl.vb
```vb
Public Class GaugeControl
    Inherits UserControl
    
    ' Properties
    Public Property MinValue As Single = 0
    Public Property MaxValue As Single = 100
    Public Property Value As Single
        Get
            Return _value
        End Get
        Set(v As Single)
            _value = Math.Max(MinValue, Math.Min(MaxValue, v))
            Me.Invalidate()  ' Trigger repaint
        End Set
    End Property
    
    Public Property YellowZoneStart As Single = 80
    Public Property RedZoneStart As Single = 90
    Public Property Label As String = ""
    Public Property Unit As String = ""
    
    Private _value As Single
    
    Protected Overrides Sub OnPaint(e As PaintEventArgs)
        MyBase.OnPaint(e)
        
        Dim g As Graphics = e.Graphics
        g.SmoothingMode = SmoothingMode.AntiAlias
        
        ' Calculate dimensions
        Dim centerX = Me.Width / 2
        Dim centerY = Me.Height * 0.6
        Dim radius = Math.Min(centerX, centerY) * 0.8
        
        ' Draw gauge background arc
        Dim arcRect As New RectangleF(centerX - radius, centerY - radius, radius * 2, radius * 2)
        
        ' Green zone
        Using greenBrush As New SolidBrush(Color.LimeGreen)
            g.FillPie(greenBrush, arcRect, 180, GetAngle(YellowZoneStart))
        End Using
        
        ' Yellow zone
        Using yellowBrush As New SolidBrush(Color.Yellow)
            Dim startAngle = 180 + GetAngle(YellowZoneStart)
            Dim sweepAngle = GetAngle(RedZoneStart) - GetAngle(YellowZoneStart)
            g.FillPie(yellowBrush, arcRect, startAngle, sweepAngle)
        End Using
        
        ' Red zone
        Using redBrush As New SolidBrush(Color.Red)
            Dim startAngle = 180 + GetAngle(RedZoneStart)
            Dim sweepAngle = 180 - GetAngle(RedZoneStart)
            g.FillPie(redBrush, arcRect, startAngle, sweepAngle)
        End Using
        
        ' Draw needle
        Dim needleAngle = 180 + GetAngle(Value)
        Dim needleRad = needleAngle * Math.PI / 180
        Dim needleX = centerX + radius * 0.9 * Math.Cos(needleRad)
        Dim needleY = centerY + radius * 0.9 * Math.Sin(needleRad)
        
        Using needlePen As New Pen(Color.Black, 3)
            g.DrawLine(needlePen, centerX, centerY, needleX, needleY)
        End Using
        
        ' Draw value text
        Dim valueText = $"{Value:F1} {Unit}"
        Dim valueFont As New Font("Arial", 14, FontStyle.Bold)
        Dim valueSize = g.MeasureString(valueText, valueFont)
        g.DrawString(valueText, valueFont, Brushes.Black, 
                     centerX - valueSize.Width / 2, centerY + radius * 0.3)
        
        ' Draw label
        Dim labelFont As New Font("Arial", 10)
        Dim labelSize = g.MeasureString(Label, labelFont)
        g.DrawString(Label, labelFont, Brushes.DarkGray,
                     centerX - labelSize.Width / 2, Me.Height - 20)
    End Sub
    
    Private Function GetAngle(value As Single) As Single
        ' Map value to 0-180 degrees
        Return (value - MinValue) / (MaxValue - MinValue) * 180
    End Function
End Class
```

---

## Utilities

### MeterConstants.vb
```vb
Public Module MeterConstants
    ' TCP Protocol
    Public Const DEFAULT_PORT As Integer = 8088
    Public Const SYNC_MESSAGE As String = "/SYNC/"
    
    ' Command codes
    Public Const CMD_NEW_REQ As String = "Code_NewReq"
    Public Const CMD_CAL1 As String = "Code_Cal1"
    Public Const CMD_CAL2 As String = "Code_Cal2"
    Public Const CMD_CAL3 As String = "Code_Cal3"
    Public Const CMD_CAL4 As String = "Code_Cal4"
    Public Const CMD_CONFIG_SAVE As String = "ConfigSave"
    Public Const CMD_CONFIG_RST As String = "ConfigRst"
    Public Const CMD_REBOOT As String = "Reboot"
    Public Const CMD_END As String = "Code_End"
    
    ' Data tags (V1.0 protocol)
    ' Energy
    Public Const TAG_PHASE_A_ENERGY_FWD_ACTIVE As String = "AE1"
    Public Const TAG_PHASE_A_ENERGY_REV_ACTIVE As String = "AE2"
    Public Const TAG_PHASE_A_ENERGY_FWD_REACTIVE As String = "AE3"
    Public Const TAG_PHASE_A_ENERGY_REV_REACTIVE As String = "AE4"
    Public Const TAG_PHASE_A_ENERGY_APPARENT As String = "AE5"
    ' ... (similar for B and C)
    
    ' System
    Public Const TAG_FREQUENCY As String = "TE1"
    Public Const TAG_TEMPERATURE As String = "TE2"
    Public Const TAG_NEUTRAL_SAMPLED As String = "TE3"
    Public Const TAG_NEUTRAL_CALC As String = "TE4"
    Public Const TAG_FREE_HEAP_KB As String = "TE5"
    Public Const TAG_UPTIME_SEC As String = "TE6"
    
    ' Power
    Public Const TAG_TOTAL_ACTIVE_POWER As String = "AP1"
    Public Const TAG_PHASE_A_ACTIVE_POWER As String = "AP2"
    Public Const TAG_PHASE_B_ACTIVE_POWER As String = "AP3"
    Public Const TAG_PHASE_C_ACTIVE_POWER As String = "AP4"
    ' ... (more tags)
    
    ' Display
    Public Const TAG_VOLTAGE_A As String = "DP1"
    Public Const TAG_VOLTAGE_B As String = "DP2"
    Public Const TAG_VOLTAGE_C As String = "DP3"
    
    ' Metering
    Public Const TAG_CURRENT_A As String = "MD1"
    Public Const TAG_CURRENT_B As String = "MD2"
    Public Const TAG_CURRENT_C As String = "MD3"
    Public Const TAG_CURRENT_N As String = "MD4"
End Module
```

---

## Project Structure

```
Meter GE3222M V2.0/
├── Meter_GE3222M_V2.0.sln
├── Meter_GE3222M_V2.0.vbproj
├── app.config
├── packages.config
│
├── Models/
│   ├── PhaseData.vb
│   ├── MeterSnapshot.vb
│   ├── CalibrationData.vb
│   ├── MeterConfig.vb
│   └── ConnectionInfo.vb
│
├── Communication/
│   ├── MeterConnection.vb
│   ├── ProtocolParser.vb
│   ├── CommandSender.vb
│   └── ReconnectManager.vb
│
├── Services/
│   ├── MeterDataService.vb
│   ├── CalibrationService.vb
│   ├── DataLoggerService.vb
│   ├── AlertService.vb
│   ├── ConfigService.vb
│   └── ReportService.vb
│
├── Views/
│   ├── MainDashboard.vb
│   ├── LiveMonitorView.vb
│   ├── EnergyDetailView.vb
│   ├── PowerDetailView.vb
│   ├── HarmonicsView.vb
│   ├── CalibrationWizard.vb
│   ├── SettingsView.vb
│   ├── DataLoggerView.vb
│   └── DiagnosticsView.vb
│
├── Controls/
│   ├── GaugeControl.vb
│   ├── PhaseBarControl.vb
│   ├── ConnectionStatusBar.vb
│   └── DoubleBufferedPanel.vb
│
├── Utilities/
│   ├── MeterConstants.vb
│   ├── UnitConverter.vb
│   ├── ValidationHelper.vb
│   └── ExportHelper.vb
│
└── Resources/
    ├── Icons/
    │   ├── app_icon.ico
    │   ├── connected.png
    │   ├── disconnected.png
    │   └── connecting.png
    └── Themes/
        ├── LightTheme.xml
        └── DarkTheme.xml
```

---

## Migration from V1.0

### Step 1: Export Configuration
In V1.0 app:
1. Connect to meter
2. Export calibration data
3. Save as JSON file

### Step 2: Install V2.0 App
1. Uninstall V1.0 (optional)
2. Install V2.0 application
3. First run: Import JSON calibration

### Step 3: Verify Compatibility
- Test connection with both apps
- Compare readings (should be identical)
- Verify calibration data

---

## Testing Strategy

### Unit Tests
```vb
<TestClass>
Public Class ProtocolParserTests
    <TestMethod>
    Public Sub ParseFrame_ValidData_ReturnsCorrectDictionary()
        ' Arrange
        Dim parser As New ProtocolParser()
        Dim testData As String = "AE1:123.45" & vbLf & "AE2:678.90" & vbLf & "TE1:60.02" & vbLf
        
        ' Act
        Dim result = parser.ParseFrame(testData)
        
        ' Assert
        Assert.AreEqual(3, result.Count)
        Assert.AreEqual(123.45F, result("AE1"), 0.01F)
        Assert.AreEqual(678.90F, result("AE2"), 0.01F)
        Assert.AreEqual(60.02F, result("TE1"), 0.01F)
    End Sub
    
    <TestMethod>
    Public Sub ParseFrame_InvalidData_SkipsInvalidLines()
        ' Arrange
        Dim parser As New ProtocolParser()
        Dim testData As String = "AE1:123.45" & vbLf & "INVALID" & vbLf & "AE2:ABC" & vbLf
        
        ' Act
        Dim result = parser.ParseFrame(testData)
        
        ' Assert
        Assert.AreEqual(1, result.Count)  ' Only valid line parsed
        Assert.IsTrue(result.ContainsKey("AE1"))
    End Sub
End Class
```

### Integration Tests
- Connect to actual meter hardware
- Verify full data flow from TCP to UI
- Test reconnection scenarios
- Validate data logging to SQLite

---

## Performance Characteristics

### Memory Usage
- **Startup:** ~50 MB
- **Connected with logging:** ~80 MB
- **SQLite database growth:** ~1 MB per day (60s intervals)

### Response Time
- **Connection establishment:** <5 seconds
- **Data update rate:** 1 Hz (configurable)
- **UI refresh:** 60 FPS (no freezing)
- **Export 1 day to CSV:** <2 seconds

---

## Future Enhancements (V2.1+)

- [ ] **Multi-meter support** (monitor multiple devices)
- [ ] **Cloud synchronization** (Azure/AWS backup)
- [ ] **Mobile companion app** (Xamarin/MAUI)
- [ ] **Advanced charting** (Oxyplot/LiveCharts integration)
- [ ] **Tariff calculator** (energy cost analysis)
- [ ] **Predictive maintenance** (ML anomaly detection)
- [ ] **Automated reports** (scheduled email/PDF)
- [ ] **REST API client** (for V2.0 firmware REST endpoints)

---

**Document Version:** 1.0  
**Last Updated:** 2026-02-08  
**Author:** Microcode Engineering
