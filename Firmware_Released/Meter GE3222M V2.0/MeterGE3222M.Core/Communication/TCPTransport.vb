Option Strict On

' SM-GE3222M V2.0 - TCP Transport Implementation
' Async TCP client with auto-reconnect for meter communication

Imports System.Net.Sockets
Imports System.Text
Imports System.Threading
Imports System.Threading.Tasks

Namespace Communication
    ''' <summary>
    ''' TCP transport implementation with automatic reconnection
    ''' Thread-safe using SynchronizationContext
    ''' </summary>
    Public Class TCPTransport
        Implements IMeterConnection
        Implements IDisposable
        
        ' TCP Client and Stream
        Private _tcpClient As TcpClient
        Private _stream As NetworkStream
        Private _reconnectTimer As Timer
        
        ' Connection State
        Private _state As ConnectionState = ConnectionState.Disconnected
        Private _lastError As String = String.Empty
        Private _connectionString As String = String.Empty
        Private _host As String = String.Empty
        Private _port As Integer = 0
        
        ' Thread Synchronization
        Private ReadOnly _syncContext As SynchronizationContext
        Private ReadOnly _stateLock As New Object()
        Private _receiveTask As Task
        Private _cancellationTokenSource As CancellationTokenSource
        
        ' Auto-reconnect Settings
        Private _autoReconnect As Boolean = True
        Private _reconnectInterval As Integer = 5000 ' 5 seconds
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
                       _tcpClient IsNot Nothing AndAlso 
                       _tcpClient.Connected
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
        Public Sub New(Optional autoReconnect As Boolean = True)
            _syncContext = SynchronizationContext.Current
            _autoReconnect = autoReconnect
        End Sub
        
        ''' <summary>
        ''' Connect to TCP server asynchronously
        ''' </summary>
        ''' <param name="connectionString">Format: "host:port" (e.g., "192.168.1.100:8088")</param>
        ''' <param name="timeout">Connection timeout in milliseconds</param>
        ''' <returns>True if connected successfully</returns>
        Public Async Function ConnectAsync(connectionString As String, Optional timeout As Integer = 10000) As Task(Of Boolean) Implements IMeterConnection.ConnectAsync
            Try
                ' Parse connection string
                Dim parts = connectionString.Split(":"c)
                If parts.Length <> 2 Then
                    SetError("Invalid connection string. Expected format: 'host:port'")
                    Return False
                End If
                
                _host = parts(0)
                If Not Integer.TryParse(parts(1), _port) Then
                    SetError("Invalid port number")
                    Return False
                End If
                
                _connectionString = connectionString
                
                ' Change state to connecting
                ChangeState(ConnectionState.Connecting)
                
                ' Dispose existing connection
                CleanupConnection()
                
                ' Create cancellation token source
                _cancellationTokenSource = New CancellationTokenSource()
                
                ' Connect with timeout
                _tcpClient = New TcpClient()
                Dim connectTask = _tcpClient.ConnectAsync(_host, _port)
                Dim timeoutTask = Task.Delay(timeout)
                
                Dim completedTask = Await Task.WhenAny(connectTask, timeoutTask)
                
                If completedTask Is timeoutTask Then
                    SetError($"Connection timeout after {timeout}ms")
                    Return False
                End If
                
                ' Get network stream
                _stream = _tcpClient.GetStream()
                
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
        ''' Disconnect from TCP server
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
                Dim bytes = Encoding.UTF8.GetBytes(commandToSend)
                
                ' Send command
                Await _stream.WriteAsync(bytes, 0, bytes.Length)
                Await _stream.FlushAsync()
                
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
        ''' Receive data loop (runs in background)
        ''' </summary>
        Private Async Function ReceiveDataLoop() As Task
            Dim buffer(4095) As Byte
            Dim dataBuffer As New StringBuilder()
            
            Try
                While Not _cancellationTokenSource.Token.IsCancellationRequested
                    If Not _tcpClient.Connected Then
                        ' Connection lost, trigger reconnect
                        HandleDisconnection()
                        Exit While
                    End If
                    
                    ' Read data from stream
                    Dim bytesRead = Await _stream.ReadAsync(buffer, 0, buffer.Length, _cancellationTokenSource.Token)
                    
                    If bytesRead = 0 Then
                        ' Connection closed by remote host
                        HandleDisconnection()
                        Exit While
                    End If
                    
                    ' Convert bytes to string
                    Dim data = Encoding.UTF8.GetString(buffer, 0, bytesRead)
                    dataBuffer.Append(data)
                    
                    ' Process complete messages (lines ending with newline)
                    Dim messages = dataBuffer.ToString().Split(New String() {vbLf}, StringSplitOptions.RemoveEmptyEntries)
                    
                    If dataBuffer.ToString().EndsWith(vbLf) Then
                        ' All messages complete
                        For Each message In messages
                            If Not String.IsNullOrWhiteSpace(message) Then
                                RaiseDataReceivedEvent(message.Trim())
                            End If
                        Next
                        dataBuffer.Clear()
                    Else
                        ' Last message incomplete, keep in buffer
                        If messages.Length > 1 Then
                            For i = 0 To messages.Length - 2
                                If Not String.IsNullOrWhiteSpace(messages(i)) Then
                                    RaiseDataReceivedEvent(messages(i).Trim())
                                End If
                            Next
                        End If
                        dataBuffer.Clear()
                        dataBuffer.Append(messages(messages.Length - 1))
                    End If
                Next
                
            Catch ex As OperationCanceledException
                ' Normal cancellation
            Catch ex As Exception
                If Not _cancellationTokenSource.Token.IsCancellationRequested Then
                    RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs($"Receive error: {ex.Message}", ex))
                    HandleDisconnection()
                End If
            End Try
        End Function
        
        ''' <summary>
        ''' Handle disconnection and trigger auto-reconnect
        ''' </summary>
        Private Sub HandleDisconnection()
            If _state = ConnectionState.Disconnected Then
                Return ' Already handling disconnection
            End If
            
            ChangeState(ConnectionState.Disconnected)
            
            ' Trigger auto-reconnect if enabled
            If _autoReconnect AndAlso Not String.IsNullOrEmpty(_connectionString) Then
                StartReconnectTimer()
            End If
        End Sub
        
        ''' <summary>
        ''' Start reconnect timer
        ''' </summary>
        Private Sub StartReconnectTimer()
            If _reconnectTimer IsNot Nothing Then
                _reconnectTimer.Dispose()
            End If
            
            _reconnectTimer = New Timer(Sub(state)
                                            Task.Run(Async Function()
                                                         Await ReconnectAsync()
                                                     End Function)
                                        End Sub, Nothing, _reconnectInterval, Timeout.Infinite)
        End Sub
        
        ''' <summary>
        ''' Attempt to reconnect
        ''' </summary>
        Private Async Function ReconnectAsync() As Task
            If _state = ConnectionState.Connected Then
                Return ' Already connected
            End If
            
            Try
                Dim success = Await ConnectAsync(_connectionString)
                If Not success Then
                    ' Retry after interval
                    StartReconnectTimer()
                End If
            Catch ex As Exception
                RaiseEvent ErrorOccurred(Me, New ErrorOccurredEventArgs($"Reconnect error: {ex.Message}", ex))
                StartReconnectTimer()
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
                
                ' Close stream
                If _stream IsNot Nothing Then
                    _stream.Close()
                    _stream.Dispose()
                    _stream = Nothing
                End If
                
                ' Close TCP client
                If _tcpClient IsNot Nothing Then
                    _tcpClient.Close()
                    _tcpClient.Dispose()
                    _tcpClient = Nothing
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
        ''' Raise data received event on synchronization context
        ''' </summary>
        Private Sub RaiseDataReceivedEvent(data As String)
            Dim eventArgs As New DataReceivedEventArgs(data)
            
            If _syncContext IsNot Nothing Then
                _syncContext.Post(Sub(state) RaiseEvent DataReceived(Me, eventArgs), Nothing)
            Else
                RaiseEvent DataReceived(Me, eventArgs)
            End If
        End Sub
        
        ''' <summary>
        ''' Dispose resources
        ''' </summary>
        Public Sub Dispose() Implements IDisposable.Dispose
            If _disposed Then
                Return
            End If
            
            _disposed = True
            _autoReconnect = False
            
            ' Dispose reconnect timer
            If _reconnectTimer IsNot Nothing Then
                _reconnectTimer.Dispose()
                _reconnectTimer = Nothing
            End If
            
            ' Cleanup connection
            CleanupConnection()
            
            GC.SuppressFinalize(Me)
        End Sub
    End Class
End Namespace
