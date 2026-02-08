Option Strict On

' SM-GE3222M V2.0 - Command Sender
' Sends commands to meter (register read/write, calibration, config)

Imports System.Threading.Tasks

Namespace Communication
    ''' <summary>
    ''' Command sender for meter communication
    ''' Handles register read/write, calibration, and configuration commands
    ''' </summary>
    Public Class CommandSender
        Private ReadOnly _connection As IMeterConnection
        Private ReadOnly _commandTimeout As Integer = 5000 ' 5 seconds default
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        ''' <param name="connection">Meter connection instance</param>
        Public Sub New(connection As IMeterConnection)
            If connection Is Nothing Then
                Throw New ArgumentNullException(NameOf(connection))
            End If
            
            _connection = connection
        End Sub
        
        ''' <summary>
        ''' Constructor with custom timeout
        ''' </summary>
        ''' <param name="connection">Meter connection instance</param>
        ''' <param name="commandTimeout">Command timeout in milliseconds</param>
        Public Sub New(connection As IMeterConnection, commandTimeout As Integer)
            If connection Is Nothing Then
                Throw New ArgumentNullException(NameOf(connection))
            End If
            
            _connection = connection
            _commandTimeout = commandTimeout
        End Sub
        
        #Region "Register Commands"
        
        ''' <summary>
        ''' Read register from meter
        ''' </summary>
        ''' <param name="registerAddress">Register address (hex format: 0xXXXX)</param>
        ''' <param name="timeout">Command timeout in milliseconds</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function ReadRegisterAsync(registerAddress As UShort, Optional timeout As Integer = 0) As Task(Of String)
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            ' Format: R:0xXXXX
            Dim command = $"R:0x{registerAddress:X4}"
            
            Dim actualTimeout = If(timeout > 0, timeout, _commandTimeout)
            Return Await _connection.SendCommandAsync(command, True, actualTimeout)
        End Function
        
        ''' <summary>
        ''' Read register by hex string address
        ''' </summary>
        ''' <param name="registerAddressHex">Register address as hex string (e.g., "0x1234" or "1234")</param>
        ''' <param name="timeout">Command timeout in milliseconds</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function ReadRegisterAsync(registerAddressHex As String, Optional timeout As Integer = 0) As Task(Of String)
            Dim address = ParseHexAddress(registerAddressHex)
            Return Await ReadRegisterAsync(address, timeout)
        End Function
        
        ''' <summary>
        ''' Write register to meter
        ''' </summary>
        ''' <param name="registerAddress">Register address (hex format: 0xXXXX)</param>
        ''' <param name="value">Value to write (hex format: 0xYYYY)</param>
        ''' <param name="timeout">Command timeout in milliseconds</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function WriteRegisterAsync(registerAddress As UShort, value As UShort, Optional timeout As Integer = 0) As Task(Of String)
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            ' Format: W:0xXXXX:0xYYYY
            Dim command = $"W:0x{registerAddress:X4}:0x{value:X4}"
            
            Dim actualTimeout = If(timeout > 0, timeout, _commandTimeout)
            Return Await _connection.SendCommandAsync(command, True, actualTimeout)
        End Function
        
        ''' <summary>
        ''' Write register by hex string addresses
        ''' </summary>
        ''' <param name="registerAddressHex">Register address as hex string</param>
        ''' <param name="valueHex">Value as hex string</param>
        ''' <param name="timeout">Command timeout in milliseconds</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function WriteRegisterAsync(registerAddressHex As String, valueHex As String, Optional timeout As Integer = 0) As Task(Of String)
            Dim address = ParseHexAddress(registerAddressHex)
            Dim value = ParseHexAddress(valueHex)
            Return Await WriteRegisterAsync(address, value, timeout)
        End Function
        
        #End Region
        
        #Region "Calibration Commands"
        
        ''' <summary>
        ''' Start calibration mode
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function StartCalibrationAsync() As Task(Of String)
            Return Await SendCommandAsync("CAL:START")
        End Function
        
        ''' <summary>
        ''' Stop calibration mode
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function StopCalibrationAsync() As Task(Of String)
            Return Await SendCommandAsync("CAL:STOP")
        End Function
        
        ''' <summary>
        ''' Calibrate voltage gain for specific phase
        ''' </summary>
        ''' <param name="phase">Phase (A, B, or C)</param>
        ''' <param name="gain">Gain value</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function CalibrateVoltageGainAsync(phase As Char, gain As Single) As Task(Of String)
            If Not "ABC".Contains(Char.ToUpperInvariant(phase)) Then
                Throw New ArgumentException("Phase must be A, B, or C", NameOf(phase))
            End If
            
            Dim command = $"CAL:VGAIN:{Char.ToUpperInvariant(phase)}:{gain:F6}"
            Return Await SendCommandAsync(command)
        End Function
        
        ''' <summary>
        ''' Calibrate current gain for specific phase
        ''' </summary>
        ''' <param name="phase">Phase (A, B, or C)</param>
        ''' <param name="gain">Gain value</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function CalibrateCurrentGainAsync(phase As Char, gain As Single) As Task(Of String)
            If Not "ABC".Contains(Char.ToUpperInvariant(phase)) Then
                Throw New ArgumentException("Phase must be A, B, or C", NameOf(phase))
            End If
            
            Dim command = $"CAL:IGAIN:{Char.ToUpperInvariant(phase)}:{gain:F6}"
            Return Await SendCommandAsync(command)
        End Function
        
        ''' <summary>
        ''' Calibrate power gain for specific phase
        ''' </summary>
        ''' <param name="phase">Phase (A, B, or C)</param>
        ''' <param name="gain">Gain value</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function CalibratePowerGainAsync(phase As Char, gain As Single) As Task(Of String)
            If Not "ABC".Contains(Char.ToUpperInvariant(phase)) Then
                Throw New ArgumentException("Phase must be A, B, or C", NameOf(phase))
            End If
            
            Dim command = $"CAL:PGAIN:{Char.ToUpperInvariant(phase)}:{gain:F6}"
            Return Await SendCommandAsync(command)
        End Function
        
        ''' <summary>
        ''' Calibrate voltage offset for specific phase
        ''' </summary>
        ''' <param name="phase">Phase (A, B, or C)</param>
        ''' <param name="offset">Offset value</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function CalibrateVoltageOffsetAsync(phase As Char, offset As Single) As Task(Of String)
            If Not "ABC".Contains(Char.ToUpperInvariant(phase)) Then
                Throw New ArgumentException("Phase must be A, B, or C", NameOf(phase))
            End If
            
            Dim command = $"CAL:VOFFSET:{Char.ToUpperInvariant(phase)}:{offset:F6}"
            Return Await SendCommandAsync(command)
        End Function
        
        ''' <summary>
        ''' Calibrate current offset for specific phase
        ''' </summary>
        ''' <param name="phase">Phase (A, B, or C)</param>
        ''' <param name="offset">Offset value</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function CalibrateCurrentOffsetAsync(phase As Char, offset As Single) As Task(Of String)
            If Not "ABC".Contains(Char.ToUpperInvariant(phase)) Then
                Throw New ArgumentException("Phase must be A, B, or C", NameOf(phase))
            End If
            
            Dim command = $"CAL:IOFFSET:{Char.ToUpperInvariant(phase)}:{offset:F6}"
            Return Await SendCommandAsync(command)
        End Function
        
        ''' <summary>
        ''' Save calibration data to EEPROM
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function SaveCalibrationAsync() As Task(Of String)
            Return Await SendCommandAsync("CAL:SAVE")
        End Function
        
        ''' <summary>
        ''' Load calibration data from EEPROM
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function LoadCalibrationAsync() As Task(Of String)
            Return Await SendCommandAsync("CAL:LOAD")
        End Function
        
        ''' <summary>
        ''' Reset calibration to factory defaults
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function ResetCalibrationAsync() As Task(Of String)
            Return Await SendCommandAsync("CAL:RESET")
        End Function
        
        #End Region
        
        #Region "Configuration Commands"
        
        ''' <summary>
        ''' Set meter device ID
        ''' </summary>
        ''' <param name="deviceId">Device ID (0-255)</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function SetDeviceIdAsync(deviceId As Byte) As Task(Of String)
            Return Await SendCommandAsync($"CFG:DEVID:{deviceId}")
        End Function
        
        ''' <summary>
        ''' Set meter baud rate
        ''' </summary>
        ''' <param name="baudRate">Baud rate (e.g., 9600, 115200)</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function SetBaudRateAsync(baudRate As Integer) As Task(Of String)
            Return Await SendCommandAsync($"CFG:BAUD:{baudRate}")
        End Function
        
        ''' <summary>
        ''' Set meter IP address
        ''' </summary>
        ''' <param name="ipAddress">IP address (e.g., "192.168.1.100")</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function SetIpAddressAsync(ipAddress As String) As Task(Of String)
            Return Await SendCommandAsync($"CFG:IP:{ipAddress}")
        End Function
        
        ''' <summary>
        ''' Set meter port number
        ''' </summary>
        ''' <param name="port">Port number (1-65535)</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function SetPortAsync(port As UShort) As Task(Of String)
            Return Await SendCommandAsync($"CFG:PORT:{port}")
        End Function
        
        ''' <summary>
        ''' Enable/disable auto-streaming mode
        ''' </summary>
        ''' <param name="enable">True to enable, False to disable</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function SetAutoStreamAsync(enable As Boolean) As Task(Of String)
            Dim value = If(enable, "1", "0")
            Return Await SendCommandAsync($"CFG:STREAM:{value}")
        End Function
        
        ''' <summary>
        ''' Set streaming interval
        ''' </summary>
        ''' <param name="intervalMs">Interval in milliseconds</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function SetStreamIntervalAsync(intervalMs As UInteger) As Task(Of String)
            Return Await SendCommandAsync($"CFG:INTERVAL:{intervalMs}")
        End Function
        
        ''' <summary>
        ''' Save configuration to EEPROM
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function SaveConfigAsync() As Task(Of String)
            Return Await SendCommandAsync("CFG:SAVE")
        End Function
        
        ''' <summary>
        ''' Load configuration from EEPROM
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function LoadConfigAsync() As Task(Of String)
            Return Await SendCommandAsync("CFG:LOAD")
        End Function
        
        ''' <summary>
        ''' Reset configuration to factory defaults
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function ResetConfigAsync() As Task(Of String)
            Return Await SendCommandAsync("CFG:RESET")
        End Function
        
        #End Region
        
        #Region "System Commands"
        
        ''' <summary>
        ''' Restart/reboot meter
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function RestartAsync() As Task(Of String)
            Return Await SendCommandAsync("SYS:RESTART")
        End Function
        
        ''' <summary>
        ''' Get firmware version
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function GetVersionAsync() As Task(Of String)
            Return Await SendCommandAsync("SYS:VERSION")
        End Function
        
        ''' <summary>
        ''' Get meter status
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function GetStatusAsync() As Task(Of String)
            Return Await SendCommandAsync("SYS:STATUS")
        End Function
        
        ''' <summary>
        ''' Ping meter (connectivity test)
        ''' </summary>
        ''' <returns>Response string from meter</returns>
        Public Async Function PingAsync() As Task(Of String)
            Return Await SendCommandAsync("SYS:PING")
        End Function
        
        #End Region
        
        #Region "Helper Methods"
        
        ''' <summary>
        ''' Send custom command to meter
        ''' </summary>
        ''' <param name="command">Command string</param>
        ''' <param name="waitForAck">Wait for ACK response</param>
        ''' <param name="timeout">Command timeout in milliseconds</param>
        ''' <returns>Response string from meter</returns>
        Public Async Function SendCommandAsync(command As String, Optional waitForAck As Boolean = True, Optional timeout As Integer = 0) As Task(Of String)
            If String.IsNullOrWhiteSpace(command) Then
                Throw New ArgumentException("Command cannot be empty", NameOf(command))
            End If
            
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Dim actualTimeout = If(timeout > 0, timeout, _commandTimeout)
            Return Await _connection.SendCommandAsync(command, waitForAck, actualTimeout)
        End Function
        
        ''' <summary>
        ''' Parse hex address string to UShort
        ''' </summary>
        ''' <param name="hexString">Hex string (e.g., "0x1234" or "1234")</param>
        ''' <returns>Parsed UShort value</returns>
        Private Shared Function ParseHexAddress(hexString As String) As UShort
            If String.IsNullOrWhiteSpace(hexString) Then
                Throw New ArgumentException("Hex string cannot be empty", NameOf(hexString))
            End If
            
            ' Remove "0x" prefix if present
            Dim cleanHex = hexString.Trim()
            If cleanHex.StartsWith("0x", StringComparison.OrdinalIgnoreCase) Then
                cleanHex = cleanHex.Substring(2)
            End If
            
            ' Parse hex string
            Dim value As UShort
            If Not UShort.TryParse(cleanHex, Globalization.NumberStyles.HexNumber, Globalization.CultureInfo.InvariantCulture, value) Then
                Throw New ArgumentException($"Invalid hex string: {hexString}", NameOf(hexString))
            End If
            
            Return value
        End Function
        
        #End Region
    End Class
End Namespace
