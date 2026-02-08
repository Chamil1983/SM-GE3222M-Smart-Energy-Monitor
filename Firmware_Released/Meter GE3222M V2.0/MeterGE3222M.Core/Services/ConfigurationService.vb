Option Strict On

' SM-GE3222M V2.0 - Configuration Service
' Manages meter configuration operations
' Read/write configuration registers, validate settings, network config

Imports System.Threading.Tasks
Imports MeterGE3222M.Core.Communication
Imports MeterGE3222M.Core.Models

Namespace Services
    ''' <summary>
    ''' Configuration service for meter settings
    ''' Handles configuration register operations and validation
    ''' </summary>
    Public Class ConfigurationService
        Private ReadOnly _commandSender As CommandSender
        Private ReadOnly _connection As IMeterConnection
        
        ' Configuration register addresses (example addresses)
        Private Const REG_IP_ADDR_BASE As UShort = &H100
        Private Const REG_PORT As UShort = &H104
        Private Const REG_SUBNET_BASE As UShort = &H105
        Private Const REG_GATEWAY_BASE As UShort = &H109
        Private Const REG_LINE_FREQ As UShort = &H10D
        Private Const REG_SAMPLE_RATE As UShort = &H10E
        Private Const REG_UPDATE_INTERVAL As UShort = &H10F
        Private Const REG_LOGGING_ENABLED As UShort = &H110
        Private Const REG_LOGGING_INTERVAL As UShort = &H111
        
        ''' <summary>
        ''' Event raised when configuration operation progresses
        ''' </summary>
        Public Event ConfigurationProgress As EventHandler(Of ConfigurationProgressEventArgs)
        
        ''' <summary>
        ''' Event raised when configuration error occurs
        ''' </summary>
        Public Event ConfigurationError As EventHandler(Of ConfigurationErrorEventArgs)
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        ''' <param name="connection">Meter connection instance</param>
        Public Sub New(connection As IMeterConnection)
            If connection Is Nothing Then
                Throw New ArgumentNullException(NameOf(connection))
            End If
            
            _connection = connection
            _commandSender = New CommandSender(connection)
        End Sub
        
        ''' <summary>
        ''' Read complete configuration from meter
        ''' </summary>
        ''' <returns>MeterConfig object with current values</returns>
        Public Async Function ReadConfigAsync() As Task(Of MeterConfig)
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                RaiseProgress("Reading configuration...", 0)
                
                Dim config As New MeterConfig()
                
                ' Read network configuration
                RaiseProgress("Reading network configuration...", 20)
                Dim ipBytes(3) As Byte
                For i As Integer = 0 To 3
                    ipBytes(i) = CByte(Await ReadRegisterValueAsync(CUShort(REG_IP_ADDR_BASE + i)))
                Next
                config.IpAddress = $"{ipBytes(0)}.{ipBytes(1)}.{ipBytes(2)}.{ipBytes(3)}"
                
                config.Port = CInt(Await ReadRegisterValueAsync(REG_PORT))
                
                ' Read subnet mask
                Dim subnetBytes(3) As Byte
                For i As Integer = 0 To 3
                    subnetBytes(i) = CByte(Await ReadRegisterValueAsync(CUShort(REG_SUBNET_BASE + i)))
                Next
                config.SubnetMask = $"{subnetBytes(0)}.{subnetBytes(1)}.{subnetBytes(2)}.{subnetBytes(3)}"
                
                ' Read gateway
                Dim gatewayBytes(3) As Byte
                For i As Integer = 0 To 3
                    gatewayBytes(i) = CByte(Await ReadRegisterValueAsync(CUShort(REG_GATEWAY_BASE + i)))
                Next
                config.Gateway = $"{gatewayBytes(0)}.{gatewayBytes(1)}.{gatewayBytes(2)}.{gatewayBytes(3)}"
                
                ' Read measurement configuration
                RaiseProgress("Reading measurement configuration...", 60)
                config.LineFrequency = CInt(Await ReadRegisterValueAsync(REG_LINE_FREQ))
                config.SamplingRate = CInt(Await ReadRegisterValueAsync(REG_SAMPLE_RATE))
                config.DataUpdateInterval = CInt(Await ReadRegisterValueAsync(REG_UPDATE_INTERVAL))
                
                ' Read logging configuration
                RaiseProgress("Reading logging configuration...", 80)
                config.LoggingEnabled = (Await ReadRegisterValueAsync(REG_LOGGING_ENABLED)) <> 0
                config.LoggingInterval = CInt(Await ReadRegisterValueAsync(REG_LOGGING_INTERVAL))
                
                RaiseProgress("Configuration read successfully", 100)
                Return config
                
            Catch ex As Exception
                RaiseError("Failed to read configuration", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Write complete configuration to meter
        ''' </summary>
        ''' <param name="config">Configuration to write</param>
        Public Async Function WriteConfigAsync(config As MeterConfig) As Task
            If config Is Nothing Then
                Throw New ArgumentNullException(NameOf(config))
            End If
            
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            ' Validate configuration first
            If Not ValidateConfig(config) Then
                Throw New ArgumentException("Invalid configuration data")
            End If
            
            Try
                RaiseProgress("Writing configuration...", 0)
                
                ' Write network configuration
                RaiseProgress("Writing network configuration...", 20)
                Await SetNetworkConfigAsync(config.IpAddress, config.Port, config.SubnetMask, config.Gateway)
                
                ' Write measurement configuration
                RaiseProgress("Writing measurement configuration...", 60)
                Await SetMeasurementConfigAsync(config.LineFrequency, config.SamplingRate, config.DataUpdateInterval)
                
                ' Write logging configuration
                RaiseProgress("Writing logging configuration...", 80)
                Await WriteRegisterValueAsync(REG_LOGGING_ENABLED, If(config.LoggingEnabled, CUShort(1), CUShort(0)))
                Await WriteRegisterValueAsync(REG_LOGGING_INTERVAL, CUShort(config.LoggingInterval))
                
                RaiseProgress("Configuration written successfully", 100)
                
            Catch ex As Exception
                RaiseError("Failed to write configuration", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Validate configuration data
        ''' </summary>
        ''' <param name="config">Configuration to validate</param>
        ''' <returns>True if valid, False otherwise</returns>
        Public Function ValidateConfig(config As MeterConfig) As Boolean
            If config Is Nothing Then
                Return False
            End If
            
            ' Use built-in validation
            Return config.Validate()
        End Function
        
        ''' <summary>
        ''' Set network configuration
        ''' </summary>
        ''' <param name="ipAddress">IP address (e.g., "192.168.1.100")</param>
        ''' <param name="port">Port number (1-65535)</param>
        ''' <param name="subnetMask">Subnet mask (e.g., "255.255.255.0")</param>
        ''' <param name="gateway">Gateway address (e.g., "192.168.1.1")</param>
        Public Async Function SetNetworkConfigAsync(ipAddress As String, port As Integer, subnetMask As String, gateway As String) As Task
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                ' Validate IP address
                Dim ipParts = ipAddress.Split("."c)
                If ipParts.Length <> 4 Then
                    Throw New ArgumentException("Invalid IP address format")
                End If
                
                ' Write IP address bytes
                For i As Integer = 0 To 3
                    Dim ipByte As Byte = Byte.Parse(ipParts(i))
                    Await WriteRegisterValueAsync(CUShort(REG_IP_ADDR_BASE + i), ipByte)
                Next
                
                ' Write port
                If port < 1 OrElse port > 65535 Then
                    Throw New ArgumentException("Port must be between 1 and 65535")
                End If
                Await WriteRegisterValueAsync(REG_PORT, CUShort(port))
                
                ' Write subnet mask
                Dim subnetParts = subnetMask.Split("."c)
                If subnetParts.Length <> 4 Then
                    Throw New ArgumentException("Invalid subnet mask format")
                End If
                For i As Integer = 0 To 3
                    Dim subnetByte As Byte = Byte.Parse(subnetParts(i))
                    Await WriteRegisterValueAsync(CUShort(REG_SUBNET_BASE + i), subnetByte)
                Next
                
                ' Write gateway
                Dim gatewayParts = gateway.Split("."c)
                If gatewayParts.Length <> 4 Then
                    Throw New ArgumentException("Invalid gateway format")
                End If
                For i As Integer = 0 To 3
                    Dim gatewayByte As Byte = Byte.Parse(gatewayParts(i))
                    Await WriteRegisterValueAsync(CUShort(REG_GATEWAY_BASE + i), gatewayByte)
                Next
                
            Catch ex As Exception
                RaiseError("Failed to set network configuration", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Set measurement configuration
        ''' </summary>
        ''' <param name="lineFrequency">Line frequency (50 or 60 Hz)</param>
        ''' <param name="samplingRate">Sampling rate (Hz)</param>
        ''' <param name="updateInterval">Data update interval (ms)</param>
        Public Async Function SetMeasurementConfigAsync(lineFrequency As Integer, samplingRate As Integer, updateInterval As Integer) As Task
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                ' Validate line frequency
                If lineFrequency <> 50 AndAlso lineFrequency <> 60 Then
                    Throw New ArgumentException("Line frequency must be 50 or 60 Hz")
                End If
                Await WriteRegisterValueAsync(REG_LINE_FREQ, CUShort(lineFrequency))
                
                ' Validate sampling rate
                If samplingRate < 100 OrElse samplingRate > 10000 Then
                    Throw New ArgumentException("Sampling rate must be between 100 and 10000 Hz")
                End If
                Await WriteRegisterValueAsync(REG_SAMPLE_RATE, CUShort(samplingRate))
                
                ' Write update interval
                If updateInterval < 100 OrElse updateInterval > 60000 Then
                    Throw New ArgumentException("Update interval must be between 100 and 60000 ms")
                End If
                Await WriteRegisterValueAsync(REG_UPDATE_INTERVAL, CUShort(updateInterval))
                
            Catch ex As Exception
                RaiseError("Failed to set measurement configuration", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Reset configuration to factory defaults
        ''' </summary>
        Public Async Function ResetToDefaultsAsync() As Task
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                RaiseProgress("Resetting to factory defaults...", 0)
                
                ' Send reset command
                Await _connection.SendCommandAsync("RESET:CONFIG", True, 10000)
                
                ' Wait for meter to restart
                Await Task.Delay(2000)
                
                RaiseProgress("Reset completed", 100)
                
            Catch ex As Exception
                RaiseError("Failed to reset configuration", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Read register value as UShort
        ''' </summary>
        Private Async Function ReadRegisterValueAsync(registerAddress As UShort) As Task(Of UShort)
            Dim response = Await _commandSender.ReadRegisterAsync(registerAddress)
            
            ' Parse response (format: "REG:0xXXXX=0xYYYY" or "0xYYYY")
            Dim value As UShort = 0
            
            If response.Contains("="c) Then
                Dim parts = response.Split("="c)
                If parts.Length >= 2 Then
                    response = parts(1).Trim()
                End If
            End If
            
            ' Parse hex value
            response = response.Trim().Replace("0x", "").Replace("0X", "")
            If UShort.TryParse(response, Globalization.NumberStyles.HexNumber, Nothing, value) Then
                Return value
            End If
            
            Return 0
        End Function
        
        ''' <summary>
        ''' Write register value
        ''' </summary>
        Private Async Function WriteRegisterValueAsync(registerAddress As UShort, value As UShort) As Task
            Await _commandSender.WriteRegisterAsync(registerAddress, value)
        End Function
        
        ''' <summary>
        ''' Raise configuration progress event
        ''' </summary>
        Private Sub RaiseProgress(message As String, percentage As Integer)
            RaiseEvent ConfigurationProgress(Me, New ConfigurationProgressEventArgs(message, percentage))
        End Sub
        
        ''' <summary>
        ''' Raise configuration error event
        ''' </summary>
        Private Sub RaiseError(message As String, ex As Exception)
            RaiseEvent ConfigurationError(Me, New ConfigurationErrorEventArgs(message, ex))
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for configuration progress
    ''' </summary>
    Public Class ConfigurationProgressEventArgs
        Inherits EventArgs
        
        Public Property Message As String
        Public Property Percentage As Integer
        Public Property Timestamp As DateTime
        
        Public Sub New(message As String, percentage As Integer)
            Me.Message = message
            Me.Percentage = percentage
            Me.Timestamp = DateTime.Now
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for configuration error
    ''' </summary>
    Public Class ConfigurationErrorEventArgs
        Inherits EventArgs
        
        Public Property ErrorMessage As String
        Public Property Exception As Exception
        Public Property Timestamp As DateTime
        
        Public Sub New(errorMessage As String, Optional ex As Exception = Nothing)
            Me.ErrorMessage = errorMessage
            Me.Exception = ex
            Me.Timestamp = DateTime.Now
        End Sub
    End Class
End Namespace
