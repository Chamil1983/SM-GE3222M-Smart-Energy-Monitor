Option Strict On

' SM-GE3222M V2.0 - Meter Data Service
' Main service for meter data operations
' Subscribes to connection events, parses data, maintains current snapshot

Imports System.Threading
Imports System.Threading.Tasks
Imports MeterGE3222M.Core.Communication
Imports MeterGE3222M.Core.Models

Namespace Services
    ''' <summary>
    ''' Main service for meter data operations
    ''' Manages data subscription, parsing, and snapshot distribution
    ''' </summary>
    Public Class MeterDataService
        Implements IDisposable
        
        Private ReadOnly _connection As IMeterConnection
        Private ReadOnly _parser As ProtocolV2Parser
        Private ReadOnly _syncContext As SynchronizationContext
        Private ReadOnly _lockObject As New Object()
        
        Private _currentSnapshot As MeterSnapshot
        Private _isSubscribed As Boolean = False
        Private _disposed As Boolean = False
        
        ''' <summary>
        ''' Current meter snapshot (thread-safe)
        ''' </summary>
        Public ReadOnly Property CurrentSnapshot As MeterSnapshot
            Get
                SyncLock _lockObject
                    Return _currentSnapshot
                End SyncLock
            End Get
        End Property
        
        ''' <summary>
        ''' Check if service is subscribed to data updates
        ''' </summary>
        Public ReadOnly Property IsSubscribed As Boolean
            Get
                Return _isSubscribed
            End Get
        End Property
        
        ''' <summary>
        ''' Event raised when new meter snapshot is available
        ''' </summary>
        Public Event DataUpdated As EventHandler(Of MeterDataUpdatedEventArgs)
        
        ''' <summary>
        ''' Event raised when parsing error occurs
        ''' </summary>
        Public Event ParsingError As EventHandler(Of ParsingErrorEventArgs)
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        ''' <param name="connection">Meter connection instance</param>
        Public Sub New(connection As IMeterConnection)
            If connection Is Nothing Then
                Throw New ArgumentNullException(NameOf(connection))
            End If
            
            _connection = connection
            _parser = New ProtocolV2Parser()
            _syncContext = SynchronizationContext.Current
            _currentSnapshot = New MeterSnapshot() With {.Valid = False}
        End Sub
        
        ''' <summary>
        ''' Subscribe to data updates from meter
        ''' </summary>
        Public Sub Subscribe()
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(MeterDataService))
            End If
            
            If _isSubscribed Then
                Return
            End If
            
            AddHandler _connection.DataReceived, AddressOf OnDataReceived
            _isSubscribed = True
        End Sub
        
        ''' <summary>
        ''' Unsubscribe from data updates
        ''' </summary>
        Public Sub Unsubscribe()
            If Not _isSubscribed Then
                Return
            End If
            
            RemoveHandler _connection.DataReceived, AddressOf OnDataReceived
            _isSubscribed = False
        End Sub
        
        ''' <summary>
        ''' Request a snapshot from the meter asynchronously
        ''' </summary>
        ''' <param name="timeout">Request timeout in milliseconds</param>
        ''' <returns>Meter snapshot or Nothing on error</returns>
        Public Async Function RequestSnapshotAsync(Optional timeout As Integer = 5000) As Task(Of MeterSnapshot)
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(MeterDataService))
            End If
            
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                ' Send snapshot request command
                Dim response = Await _connection.SendCommandAsync("GET:SNAPSHOT", True, timeout)
                
                If String.IsNullOrWhiteSpace(response) Then
                    Return New MeterSnapshot() With {.Valid = False}
                End If
                
                ' Parse response
                Dim snapshot = _parser.Parse(response)
                
                ' Update current snapshot if valid
                If snapshot.Valid Then
                    UpdateSnapshot(snapshot)
                End If
                
                Return snapshot
                
            Catch ex As Exception
                RaiseParsingError($"Failed to request snapshot: {ex.Message}", ex)
                Return New MeterSnapshot() With {.Valid = False}
            End Try
        End Function
        
        ''' <summary>
        ''' Clear current snapshot data
        ''' </summary>
        Public Sub ClearSnapshot()
            SyncLock _lockObject
                _currentSnapshot = New MeterSnapshot() With {.Valid = False}
            End SyncLock
        End Sub
        
        ''' <summary>
        ''' Handle incoming data from connection
        ''' </summary>
        Private Sub OnDataReceived(sender As Object, e As DataReceivedEventArgs)
            If _disposed Then Return
            
            Try
                ' Parse incoming data
                Dim snapshot = _parser.Parse(e.Data)
                
                If snapshot Is Nothing OrElse Not snapshot.Valid Then
                    RaiseParsingError($"Invalid data received: {e.Data}", Nothing)
                    Return
                End If
                
                ' Update current snapshot
                UpdateSnapshot(snapshot)
                
            Catch ex As Exception
                RaiseParsingError($"Error parsing data: {ex.Message}", ex)
            End Try
        End Sub
        
        ''' <summary>
        ''' Update current snapshot and raise event (thread-safe)
        ''' </summary>
        Private Sub UpdateSnapshot(snapshot As MeterSnapshot)
            If snapshot Is Nothing Then Return
            
            SyncLock _lockObject
                _currentSnapshot = snapshot
            End SyncLock
            
            ' Raise event on sync context if available
            If _syncContext IsNot Nothing Then
                _syncContext.Post(Sub(state)
                    RaiseDataUpdated(snapshot)
                End Sub, Nothing)
            Else
                RaiseDataUpdated(snapshot)
            End If
        End Sub
        
        ''' <summary>
        ''' Raise DataUpdated event
        ''' </summary>
        Private Sub RaiseDataUpdated(snapshot As MeterSnapshot)
            RaiseEvent DataUpdated(Me, New MeterDataUpdatedEventArgs(snapshot))
        End Sub
        
        ''' <summary>
        ''' Raise ParsingError event
        ''' </summary>
        Private Sub RaiseParsingError(message As String, ex As Exception)
            RaiseEvent ParsingError(Me, New ParsingErrorEventArgs(message, ex))
        End Sub
        
        ''' <summary>
        ''' Dispose resources
        ''' </summary>
        Public Sub Dispose() Implements IDisposable.Dispose
            If _disposed Then Return
            
            Unsubscribe()
            _disposed = True
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for meter data updated event
    ''' </summary>
    Public Class MeterDataUpdatedEventArgs
        Inherits EventArgs
        
        Public Property Snapshot As MeterSnapshot
        Public Property Timestamp As DateTime
        
        Public Sub New(snapshot As MeterSnapshot)
            Me.Snapshot = snapshot
            Me.Timestamp = DateTime.Now
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for parsing error event
    ''' </summary>
    Public Class ParsingErrorEventArgs
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
