Option Strict On

' SM-GE3222M V2.0 - Serial Port Transport Implementation
' Async serial port communication for meter

Imports System.IO.Ports
Imports System.Text
Imports System.Threading.Tasks

Namespace Communication
    ''' <summary>
    ''' Serial port transport implementation
    ''' Supports async read/write operations
    ''' </summary>
    Public Class SerialTransport
        Implements IMeterConnection
        Implements IDisposable
        
        ' Serial Port
        Private _serialPort As SerialPort
        
        ' Connection State
        Private _state As ConnectionState = ConnectionState.Disconnected
        Private _lastError As String = String.Empty
        Private _portName As String = String.Empty
        
        ' Thread Synchronization
        Private ReadOnly _stateLock As New Object()
        Private _disposed As Boolean = False
        
        ' Serial Settings
        Private _baudRate As Integer = 115200
        Private _dataBits As Integer = 8
        Private _parity As Parity = Parity.None
        Private _stopBits As StopBits = StopBits.One
        
        ''' <summary>
        ''' Current connection state
        ''' </summary>
        Public ReadOnly Property State As ConnectionState Implements IMeterConnection.State
            Get
                SyncLock _stateLock
                    Return _state
                End SyncLock
            End Get
        End Property
        
        ''' <summary>
        ''' Check if currently connected
        ''' </summary>
        Public ReadOnly Property IsConnected As Boolean Implements IMeterConnection.IsConnected
            Get
                Return State = ConnectionState.Connected AndAlso 
                       _serialPort IsNot Nothing AndAlso 
                       _serialPort.IsOpen
            End Get
        End Property
        
        ''' <summary>
        ''' Last error message
        ''' </summary>
        Public ReadOnly Property LastError As String Implements IMeterConnection.LastError
            Get
                SyncLock _stateLock
                    Return _lastError
                End SyncLock
            End Get
        End Property
        
        ''' <summary>
        ''' Event raised when data is received
        ''' </summary>
        Public Event DataReceived As EventHandler(Of DataReceivedEventArgs) Implements IMeterConnection.DataReceived
        
        ''' <summary>
        ''' Event raised when connection state changes
        ''' </summary>
        Public Event ConnectionStateChanged As EventHandler(Of ConnectionStateChangedEventArgs) Implements IMeterConnection.ConnectionStateChanged
        
        ''' <summary>
        ''' Event raised when an error occurs
        ''' </summary>
        Public Event ErrorOccurred As EventHandler(Of ErrorOccurredEventArgs) Implements IMeterConnection.ErrorOccurred
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        Public Sub New(Optional baudRate As Integer = 115200)
            _baudRate = baudRate
        End Sub
        
        ''' <summary>
        ''' Connect to serial port asynchronously
        ''' </summary>
        ''' <param name="connectionString">Format: "COM3" or "COM3:115200:8:N:1"</param>
        ''' <param name="timeout">Connection timeout in milliseconds (unused for serial)</param>
        ''' <returns>True if connected successfully</returns>
        Public Async Function ConnectAsync(connectionString As String, Optional timeout As Integer = 10000) As Task(Of Boolean) Implements IMeterConnection.ConnectAsync
            Try
                ' Parse connection string
                ParseConnectionString(connectionString)
                
                ' Change state to connecting
                ChangeState(ConnectionState.Connecting)
                
                ' Close existing port
                CleanupConnection()
                
                ' Create and configure serial port
                _serialPort = New SerialPort(_portName, _baudRate, _parity, _dataBits, _stopBits) With {
                    .ReadTimeout = 5000,
                    .WriteTimeout = 5000,
                    .Encoding = Encoding.UTF8,
                    .NewLine = vbLf
                }
                
                ' Subscribe to data received event
                AddHandler _serialPort.DataReceived, AddressOf SerialPort_DataReceived
                AddHandler _serialPort.ErrorReceived, AddressOf SerialPort_ErrorReceived
                
                ' Open port (run on thread pool to avoid blocking)
                Await Task.Run(Sub() _serialPort.Open())
                
                ' Change state to connected
                ChangeState(ConnectionState.Connected)
                
                Return True
                
            Catch ex As Exception
                SetError($"Connection error: {ex.Message}", ex)
                Return False
            End Try
        End Function
        
        ''' <summary>
        ''' Disconnect from serial port
        ''' </summary>
        Public Sub Disconnect() Implements IMeterConnection.Disconnect
            CleanupConnection()
            ChangeState(ConnectionState.Disconnected)
        End Sub
        
        ''' <summary>
        ''' Send command to meter asynchronously
        ''' </summary>
        ''' <param name="command">Command string to send</param>
        ''' <param name="waitForAck">Wait for ACK response</param>
        ''' <param name="timeout">Command timeout in milliseconds</param>
        ''' <returns>Response string (if any)</returns>
        Public Async Function SendCommandAsync(command As String, Optional waitForAck As Boolean = False, Optional timeout As Integer = 5000) As Task(Of String) Implements IMeterConnection.SendCommandAsync
            If Not IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                ' Add newline if not present
                Dim commandToSend = If(command.EndsWith(vbLf), command, command & vbLf)
                
                ' Send command (run on thread pool)
                Await Task.Run(Sub() _serialPort.Write(commandToSend))
                
                ' Wait for ACK if requested
                If waitForAck Then
                    ' TODO: Implement ACK waiting with timeout
                    ' For now, just delay
                    Await Task.Delay(100)
                End If
                
                Return String.Empty
                
            Catch ex As Exception
                RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs($"Send error: {ex.Message}", ex))
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Parse connection string
        ''' </summary>
        Private Sub ParseConnectionString(connectionString As String)
            Dim parts = connectionString.Split(":"c)
            
            ' Port name (required)
            _portName = parts(0).Trim()
            
            ' Baud rate (optional)
            If parts.Length > 1 AndAlso Integer.TryParse(parts(1), _baudRate) Then
                ' Parsed successfully
            End If
            
            ' Data bits (optional)
            If parts.Length > 2 AndAlso Integer.TryParse(parts(2), _dataBits) Then
                ' Parsed successfully
            End If
            
            ' Parity (optional)
            If parts.Length > 3 Then
                Select Case parts(3).ToUpperInvariant()
                    Case "N", "NONE"
                        _parity = Parity.None
                    Case "E", "EVEN"
                        _parity = Parity.Even
                    Case "O", "ODD"
                        _parity = Parity.Odd
                    Case "M", "MARK"
                        _parity = Parity.Mark
                    Case "S", "SPACE"
                        _parity = Parity.Space
                End Select
            End If
            
            ' Stop bits (optional)
            If parts.Length > 4 Then
                Select Case parts(4)
                    Case "1"
                        _stopBits = StopBits.One
                    Case "1.5"
                        _stopBits = StopBits.OnePointFive
                    Case "2"
                        _stopBits = StopBits.Two
                End Select
            End If
        End Sub
        
        ''' <summary>
        ''' Serial port data received event handler
        ''' </summary>
        Private Sub SerialPort_DataReceived(sender As Object, e As SerialDataReceivedEventArgs)
            Try
                If _serialPort Is Nothing OrElse Not _serialPort.IsOpen Then
                    Return
                End If
                
                ' Read available data
                Dim data = _serialPort.ReadExisting()
                
                If Not String.IsNullOrEmpty(data) Then
                    ' Split by newlines and raise event for each line
                    Dim lines = data.Split(New String() {vbLf, vbCrLf}, StringSplitOptions.RemoveEmptyEntries)
                    
                    For Each line In lines
                        If Not String.IsNullOrWhiteSpace(line) Then
                            RaiseEvent DataReceived(Me, New DataReceivedEventArgs(line.Trim()))
                        End If
                    Next
                End If
                
            Catch ex As Exception
                RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs($"Data receive error: {ex.Message}", ex))
            End Try
        End Sub
        
        ''' <summary>
        ''' Serial port error received event handler
        ''' </summary>
        Private Sub SerialPort_ErrorReceived(sender As Object, e As SerialErrorReceivedEventArgs)
            Dim errorMessage As String
            
            Select Case e.EventType
                Case SerialError.Frame
                    errorMessage = "Serial frame error"
                Case SerialError.Overrun
                    errorMessage = "Serial buffer overrun"
                Case SerialError.RXOver
                    errorMessage = "Serial receive buffer overflow"
                Case SerialError.RXParity
                    errorMessage = "Serial parity error"
                Case SerialError.TXFull
                    errorMessage = "Serial transmit buffer full"
                Case Else
                    errorMessage = $"Serial error: {e.EventType}"
            End Select
            
            RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs(errorMessage))
        End Sub
        
        ''' <summary>
        ''' Cleanup connection resources
        ''' </summary>
        Private Sub CleanupConnection()
            Try
                If _serialPort IsNot Nothing Then
                    RemoveHandler _serialPort.DataReceived, AddressOf SerialPort_DataReceived
                    RemoveHandler _serialPort.ErrorReceived, AddressOf SerialPort_ErrorReceived
                    
                    If _serialPort.IsOpen Then
                        _serialPort.Close()
                    End If
                    
                    _serialPort.Dispose()
                    _serialPort = Nothing
                End If
                
            Catch ex As Exception
                ' Ignore cleanup errors
            End Try
        End Sub
        
        ''' <summary>
        ''' Change connection state and raise event
        ''' </summary>
        Private Sub ChangeState(newState As ConnectionState)
            Dim oldState As ConnectionState
            
            SyncLock _stateLock
                oldState = _state
                _state = newState
                
                If newState <> ConnectionState.Error Then
                    _lastError = String.Empty
                End If
            End SyncLock
            
            If oldState <> newState Then
                RaiseEvent ConnectionStateChanged(Me, New ConnectionStateChangedEventArgs(oldState, newState))
            End If
        End Sub
        
        ''' <summary>
        ''' Set error state
        ''' </summary>
        Private Sub SetError(errorMessage As String, Optional ex As Exception = Nothing)
            SyncLock _stateLock
                _state = ConnectionState.Error
                _lastError = errorMessage
            End SyncLock
            
            RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs(errorMessage, ex))
        End Sub
        
        ''' <summary>
        ''' Dispose resources
        ''' </summary>
        Public Sub Dispose() Implements IDisposable.Dispose
            If _disposed Then
                Return
            End If
            
            _disposed = True
            CleanupConnection()
            
            GC.SuppressFinalize(Me)
        End Sub
    End Class
End Namespace
