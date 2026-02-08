Option Strict On

' SM-GE3222M V2.0 - WebSocket Transport Implementation
' WebSocket client for meter communication with binary/text support

Imports System.Net.WebSockets
Imports System.Text
Imports System.Threading
Imports System.Threading.Tasks

Namespace Communication
    ''' <summary>
    ''' WebSocket transport implementation
    ''' Supports binary and text message formats
    ''' </summary>
    Public Class WebSocketTransport
        Implements IMeterConnection
        Implements IDisposable
        
        ' WebSocket Client
        Private _webSocket As ClientWebSocket
        Private _uri As Uri
        
        ' Connection State
        Private _state As ConnectionState = ConnectionState.Disconnected
        Private _lastError As String = String.Empty
        Private _connectionString As String = String.Empty
        
        ' Thread Synchronization
        Private ReadOnly _stateLock As New Object()
        Private _receiveTask As Task
        Private _cancellationTokenSource As CancellationTokenSource
        Private _disposed As Boolean = False
        
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
                       _webSocket IsNot Nothing AndAlso 
                       _webSocket.State = WebSocketState.Open
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
        Public Sub New()
            ' No specific initialization needed
        End Sub
        
        ''' <summary>
        ''' Connect to WebSocket server asynchronously
        ''' </summary>
        ''' <param name="connectionString">Format: "ws://host:port/path" or "wss://host:port/path"</param>
        ''' <param name="timeout">Connection timeout in milliseconds</param>
        ''' <returns>True if connected successfully</returns>
        Public Async Function ConnectAsync(connectionString As String, Optional timeout As Integer = 10000) As Task(Of Boolean) Implements IMeterConnection.ConnectAsync
            Try
                ' Parse connection string to URI
                If Not connectionString.StartsWith("ws://") AndAlso Not connectionString.StartsWith("wss://") Then
                    connectionString = "ws://" & connectionString
                End If
                
                _uri = New Uri(connectionString)
                _connectionString = connectionString
                
                ' Change state to connecting
                ChangeState(ConnectionState.Connecting)
                
                ' Dispose existing connection
                CleanupConnection()
                
                ' Create cancellation token source
                _cancellationTokenSource = New CancellationTokenSource()
                
                ' Create WebSocket client
                _webSocket = New ClientWebSocket()
                _webSocket.Options.KeepAliveInterval = TimeSpan.FromSeconds(30)
                
                ' Connect with timeout
                Dim connectTask = _webSocket.ConnectAsync(_uri, CancellationToken.None)
                Dim timeoutTask = Task.Delay(timeout)
                
                Dim completedTask = Await Task.WhenAny(connectTask, timeoutTask)
                
                If completedTask Is timeoutTask Then
                    SetError($"Connection timeout after {timeout}ms")
                    Return False
                End If
                
                ' Start receiving data
                _receiveTask = Task.Run(AddressOf ReceiveDataLoop, _cancellationTokenSource.Token)
                
                ' Change state to connected
                ChangeState(ConnectionState.Connected)
                
                Return True
                
            Catch ex As Exception
                SetError($"Connection error: {ex.Message}", ex)
                Return False
            End Try
        End Function
        
        ''' <summary>
        ''' Disconnect from WebSocket server
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
                ' Convert command to bytes
                Dim bytes = Encoding.UTF8.GetBytes(command)
                Dim buffer As New ArraySegment(Of Byte)(bytes)
                
                ' Send as text message
                Await _webSocket.SendAsync(buffer, WebSocketMessageType.Text, True, CancellationToken.None)
                
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
        ''' Send binary data to meter asynchronously
        ''' </summary>
        ''' <param name="data">Binary data to send</param>
        ''' <returns>Task</returns>
        Public Async Function SendBinaryAsync(data As Byte()) As Task
            If Not IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                Dim buffer As New ArraySegment(Of Byte)(data)
                Await _webSocket.SendAsync(buffer, WebSocketMessageType.Binary, True, CancellationToken.None)
                
            Catch ex As Exception
                RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs($"Send binary error: {ex.Message}", ex))
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Receive data loop (runs in background)
        ''' </summary>
        Private Async Function ReceiveDataLoop() As Task
            Dim buffer(4095) As Byte
            
            Try
                While _webSocket.State = WebSocketState.Open AndAlso Not _cancellationTokenSource.Token.IsCancellationRequested
                    ' Receive message
                    Dim result = Await _webSocket.ReceiveAsync(New ArraySegment(Of Byte)(buffer), _cancellationTokenSource.Token)
                    
                    If result.MessageType = WebSocketMessageType.Close Then
                        ' Server closed connection
                        Await _webSocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None)
                        ChangeState(ConnectionState.Disconnected)
                        Exit While
                    End If
                    
                    ' Convert bytes to string
                    Dim data As String
                    
                    If result.MessageType = WebSocketMessageType.Text Then
                        data = Encoding.UTF8.GetString(buffer, 0, result.Count)
                    ElseIf result.MessageType = WebSocketMessageType.Binary Then
                        ' For binary messages, convert to hex string or handle differently
                        data = BitConverter.ToString(buffer, 0, result.Count).Replace("-", " ")
                    Else
                        Continue While
                    End If
                    
                    ' Raise data received event
                    If Not String.IsNullOrWhiteSpace(data) Then
                        RaiseEvent DataReceived(Me, New DataReceivedEventArgs(data.Trim()))
                    End If
                Next
                
            Catch ex As OperationCanceledException
                ' Normal cancellation
            Catch ex As WebSocketException
                If Not _cancellationTokenSource.Token.IsCancellationRequested Then
                    RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs($"WebSocket error: {ex.Message}", ex))
                    ChangeState(ConnectionState.Error)
                End If
            Catch ex As Exception
                If Not _cancellationTokenSource.Token.IsCancellationRequested Then
                    RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs($"Receive error: {ex.Message}", ex))
                    ChangeState(ConnectionState.Error)
                End If
            End Try
        End Function
        
        ''' <summary>
        ''' Cleanup connection resources
        ''' </summary>
        Private Sub CleanupConnection()
            Try
                ' Cancel receive task
                If _cancellationTokenSource IsNot Nothing Then
                    _cancellationTokenSource.Cancel()
                    _cancellationTokenSource.Dispose()
                    _cancellationTokenSource = Nothing
                End If
                
                ' Close WebSocket
                If _webSocket IsNot Nothing Then
                    If _webSocket.State = WebSocketState.Open Then
                        Try
                            _webSocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None).Wait(1000)
                        Catch
                            ' Ignore close errors
                        End Try
                    End If
                    
                    _webSocket.Dispose()
                    _webSocket = Nothing
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
