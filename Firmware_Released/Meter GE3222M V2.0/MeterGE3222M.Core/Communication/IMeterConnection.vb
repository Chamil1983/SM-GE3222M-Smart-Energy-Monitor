' SM-GE3222M V2.0 - Meter Connection Interface
' Defines the contract for all meter transport implementations (TCP, Serial, WebSocket)

Imports System.Threading.Tasks

Namespace Communication
    ''' <summary>
    ''' Connection state enumeration
    ''' </summary>
    Public Enum ConnectionState
        Disconnected = 0
        Connecting = 1
        Connected = 2
        Error = 3
    End Enum
    
    ''' <summary>
    ''' Interface for meter communication transports
    ''' Implemented by TCPTransport, SerialTransport, WebSocketTransport
    ''' </summary>
    Public Interface IMeterConnection
        ''' <summary>
        ''' Current connection state
        ''' </summary>
        ReadOnly Property State As ConnectionState
        
        ''' <summary>
        ''' Check if currently connected
        ''' </summary>
        ReadOnly Property IsConnected As Boolean
        
        ''' <summary>
        ''' Last error message (if State = Error)
        ''' </summary>
        ReadOnly Property LastError As String
        
        ''' <summary>
        ''' Connect to meter asynchronously
        ''' </summary>
        ''' <param name="connectionString">Connection string (e.g., "192.168.1.100:8088" or "COM3")</param>
        ''' <param name="timeout">Connection timeout in milliseconds</param>
        ''' <returns>True if connected successfully</returns>
        Function ConnectAsync(connectionString As String, Optional timeout As Integer = 10000) As Task(Of Boolean)
        
        ''' <summary>
        ''' Disconnect from meter
        ''' </summary>
        Sub Disconnect()
        
        ''' <summary>
        ''' Send command to meter asynchronously
        ''' </summary>
        ''' <param name="command">Command string to send</param>
        ''' <param name="waitForAck">Wait for ACK response</param>
        ''' <param name="timeout">Command timeout in milliseconds</param>
        ''' <returns>Response string (if any)</returns>
        Function SendCommandAsync(command As String, Optional waitForAck As Boolean = False, Optional timeout As Integer = 5000) As Task(Of String)
        
        ''' <summary>
        ''' Event raised when data is received from meter
        ''' </summary>
        Event DataReceived As EventHandler(Of DataReceivedEventArgs)
        
        ''' <summary>
        ''' Event raised when connection state changes
        ''' </summary>
        Event ConnectionStateChanged As EventHandler(Of ConnectionStateChangedEventArgs)
        
        ''' <summary>
        ''' Event raised when an error occurs
        ''' </summary>
        Event ErrorOccurred As EventHandler(Of ErrorOccurredEventArgs)
    End Interface
    
    ''' <summary>
    ''' Event args for data received event
    ''' </summary>
    Public Class DataReceivedEventArgs
        Inherits EventArgs
        
        Public Property Data As String
        Public Property Timestamp As DateTime
        Public Property BytesReceived As Integer
        
        Public Sub New(data As String)
            Me.Data = data
            Me.Timestamp = DateTime.Now
            Me.BytesReceived = Text.Encoding.UTF8.GetByteCount(data)
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for connection state changed event
    ''' </summary>
    Public Class ConnectionStateChangedEventArgs
        Inherits EventArgs
        
        Public Property OldState As ConnectionState
        Public Property NewState As ConnectionState
        Public Property Timestamp As DateTime
        
        Public Sub New(oldState As ConnectionState, newState As ConnectionState)
            Me.OldState = oldState
            Me.NewState = newState
            Me.Timestamp = DateTime.Now
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for error occurred event
    ''' </summary>
    Public Class ErrorOccurredEventArgs
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
