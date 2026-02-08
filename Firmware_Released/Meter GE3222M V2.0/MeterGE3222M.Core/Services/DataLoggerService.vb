Option Strict On

' SM-GE3222M V2.0 - Data Logger Service
' Circular buffer data logging with export to CSV/JSON
' Auto-logging at configurable intervals

Imports System.Collections.Generic
Imports System.IO
Imports System.Text
Imports System.Text.Json
Imports System.Threading
Imports System.Threading.Tasks
Imports MeterGE3222M.Core.Models

Namespace Services
    ''' <summary>
    ''' Data logger service with circular buffer
    ''' Supports CSV and JSON export formats
    ''' </summary>
    Public Class DataLoggerService
        Implements IDisposable
        
        Private ReadOnly _logEntries As Queue(Of LogEntry)
        Private ReadOnly _lockObject As New Object()
        Private ReadOnly _maxEntries As Integer
        
        Private _isLogging As Boolean = False
        Private _loggingTimer As Timer
        Private _loggingInterval As Integer = 1000 ' Default 1 second
        Private _dataSource As Func(Of MeterSnapshot)
        Private _disposed As Boolean = False
        
        ''' <summary>
        ''' Current number of log entries
        ''' </summary>
        Public ReadOnly Property Count As Integer
            Get
                SyncLock _lockObject
                    Return _logEntries.Count
                End SyncLock
            End Get
        End Property
        
        ''' <summary>
        ''' Maximum number of log entries (circular buffer size)
        ''' </summary>
        Public ReadOnly Property MaxEntries As Integer
            Get
                Return _maxEntries
            End Get
        End Property
        
        ''' <summary>
        ''' Check if logging is active
        ''' </summary>
        Public ReadOnly Property IsLogging As Boolean
            Get
                Return _isLogging
            End Get
        End Property
        
        ''' <summary>
        ''' Current logging interval in milliseconds
        ''' </summary>
        Public Property LoggingInterval As Integer
            Get
                Return _loggingInterval
            End Get
            Set(value As Integer)
                If value < 100 Then
                    Throw New ArgumentException("Logging interval must be at least 100ms")
                End If
                _loggingInterval = value
            End Set
        End Property
        
        ''' <summary>
        ''' Event raised when new entry is logged
        ''' </summary>
        Public Event EntryLogged As EventHandler(Of EntryLoggedEventArgs)
        
        ''' <summary>
        ''' Event raised when buffer is full and old entries are discarded
        ''' </summary>
        Public Event BufferOverflow As EventHandler(Of BufferOverflowEventArgs)
        
        ''' <summary>
        ''' Event raised when logging error occurs
        ''' </summary>
        Public Event LoggingError As EventHandler(Of LoggingErrorEventArgs)
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        ''' <param name="maxEntries">Maximum number of log entries (circular buffer size)</param>
        Public Sub New(Optional maxEntries As Integer = 10000)
            If maxEntries < 1 Then
                Throw New ArgumentException("Max entries must be at least 1")
            End If
            
            _maxEntries = maxEntries
            _logEntries = New Queue(Of LogEntry)(maxEntries)
        End Sub
        
        ''' <summary>
        ''' Start auto-logging with data source
        ''' </summary>
        ''' <param name="dataSource">Function that returns current MeterSnapshot</param>
        ''' <param name="interval">Logging interval in milliseconds</param>
        Public Sub StartLogging(dataSource As Func(Of MeterSnapshot), Optional interval As Integer = 1000)
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(DataLoggerService))
            End If
            
            If dataSource Is Nothing Then
                Throw New ArgumentNullException(NameOf(dataSource))
            End If
            
            If _isLogging Then
                Return
            End If
            
            _dataSource = dataSource
            _loggingInterval = interval
            _isLogging = True
            
            ' Create timer for auto-logging
            _loggingTimer = New Timer(AddressOf LoggingTimerCallback, Nothing, 0, _loggingInterval)
        End Sub
        
        ''' <summary>
        ''' Stop auto-logging
        ''' </summary>
        Public Sub StopLogging()
            If Not _isLogging Then
                Return
            End If
            
            _isLogging = False
            
            If _loggingTimer IsNot Nothing Then
                _loggingTimer.Dispose()
                _loggingTimer = Nothing
            End If
            
            _dataSource = Nothing
        End Sub
        
        ''' <summary>
        ''' Add log entry manually
        ''' </summary>
        ''' <param name="entry">Log entry to add</param>
        Public Sub AddEntry(entry As LogEntry)
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(DataLoggerService))
            End If
            
            If entry Is Nothing Then
                Throw New ArgumentNullException(NameOf(entry))
            End If
            
            SyncLock _lockObject
                ' Check if buffer is full
                If _logEntries.Count >= _maxEntries Then
                    ' Remove oldest entry (circular buffer)
                    Dim removedEntry = _logEntries.Dequeue()
                    RaiseBufferOverflow(removedEntry)
                End If
                
                ' Add new entry
                _logEntries.Enqueue(entry)
            End SyncLock
            
            ' Raise event
            RaiseEntryLogged(entry)
        End Sub
        
        ''' <summary>
        ''' Add log entry from snapshot
        ''' </summary>
        ''' <param name="snapshot">Meter snapshot to log</param>
        ''' <param name="message">Optional message</param>
        ''' <param name="entryType">Entry type</param>
        Public Sub AddEntry(snapshot As MeterSnapshot, Optional message As String = "", Optional entryType As LogEntryType = LogEntryType.Data)
            Dim entry As New LogEntry(snapshot, message, entryType)
            AddEntry(entry)
        End Sub
        
        ''' <summary>
        ''' Clear all log entries
        ''' </summary>
        Public Sub ClearLog()
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(DataLoggerService))
            End If
            
            SyncLock _lockObject
                _logEntries.Clear()
            End SyncLock
        End Sub
        
        ''' <summary>
        ''' Get all log entries as array
        ''' </summary>
        ''' <returns>Array of log entries</returns>
        Public Function GetEntries() As LogEntry()
            SyncLock _lockObject
                Return _logEntries.ToArray()
            End SyncLock
        End Function
        
        ''' <summary>
        ''' Get log entries within time range
        ''' </summary>
        ''' <param name="startTime">Start time</param>
        ''' <param name="endTime">End time</param>
        ''' <returns>Filtered log entries</returns>
        Public Function GetEntries(startTime As DateTime, endTime As DateTime) As LogEntry()
            SyncLock _lockObject
                Return _logEntries.Where(Function(e) e.Timestamp >= startTime AndAlso e.Timestamp <= endTime).ToArray()
            End SyncLock
        End Function
        
        ''' <summary>
        ''' Export log to CSV file
        ''' </summary>
        ''' <param name="filePath">Output file path</param>
        Public Async Function ExportToCsvAsync(filePath As String) As Task
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(DataLoggerService))
            End If
            
            If String.IsNullOrWhiteSpace(filePath) Then
                Throw New ArgumentException("File path cannot be empty", NameOf(filePath))
            End If
            
            Try
                Dim entries() As LogEntry
                SyncLock _lockObject
                    entries = _logEntries.ToArray()
                End SyncLock
                
                Using writer As New StreamWriter(filePath, False, Encoding.UTF8)
                    ' Write header
                    Await writer.WriteLineAsync(LogEntry.GetCsvHeader())
                    
                    ' Write entries
                    For Each entry In entries
                        Await writer.WriteLineAsync(entry.ToCsvString())
                    Next
                End Using
                
            Catch ex As Exception
                RaiseLoggingError($"Failed to export CSV: {ex.Message}", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Export log to JSON file
        ''' </summary>
        ''' <param name="filePath">Output file path</param>
        Public Async Function ExportToJsonAsync(filePath As String) As Task
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(DataLoggerService))
            End If
            
            If String.IsNullOrWhiteSpace(filePath) Then
                Throw New ArgumentException("File path cannot be empty", NameOf(filePath))
            End If
            
            Try
                Dim entries() As LogEntry
                SyncLock _lockObject
                    entries = _logEntries.ToArray()
                End SyncLock
                
                ' Create JSON options for pretty printing
                Dim jsonOptions As New JsonSerializerOptions() With {
                    .WriteIndented = True,
                    .PropertyNamingPolicy = JsonNamingPolicy.CamelCase
                }
                
                ' Serialize to JSON
                Dim jsonString = JsonSerializer.Serialize(entries, jsonOptions)
                
                ' Write to file
                Await File.WriteAllTextAsync(filePath, jsonString, Encoding.UTF8)
                
            Catch ex As Exception
                RaiseLoggingError($"Failed to export JSON: {ex.Message}", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Export log entries within time range to CSV
        ''' </summary>
        ''' <param name="filePath">Output file path</param>
        ''' <param name="startTime">Start time</param>
        ''' <param name="endTime">End time</param>
        Public Async Function ExportToCsvAsync(filePath As String, startTime As DateTime, endTime As DateTime) As Task
            If _disposed Then
                Throw New ObjectDisposedException(NameOf(DataLoggerService))
            End If
            
            If String.IsNullOrWhiteSpace(filePath) Then
                Throw New ArgumentException("File path cannot be empty", NameOf(filePath))
            End If
            
            Try
                Dim entries = GetEntries(startTime, endTime)
                
                Using writer As New StreamWriter(filePath, False, Encoding.UTF8)
                    ' Write header
                    Await writer.WriteLineAsync(LogEntry.GetCsvHeader())
                    
                    ' Write entries
                    For Each entry In entries
                        Await writer.WriteLineAsync(entry.ToCsvString())
                    Next
                End Using
                
            Catch ex As Exception
                RaiseLoggingError($"Failed to export CSV: {ex.Message}", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Timer callback for auto-logging
        ''' </summary>
        Private Sub LoggingTimerCallback(state As Object)
            If Not _isLogging OrElse _dataSource Is Nothing Then
                Return
            End If
            
            Try
                ' Get current snapshot from data source
                Dim snapshot = _dataSource()
                
                If snapshot IsNot Nothing AndAlso snapshot.Valid Then
                    AddEntry(snapshot, "Auto-logged", LogEntryType.Data)
                End If
                
            Catch ex As Exception
                RaiseLoggingError($"Auto-logging error: {ex.Message}", ex)
            End Try
        End Sub
        
        ''' <summary>
        ''' Raise EntryLogged event
        ''' </summary>
        Private Sub RaiseEntryLogged(entry As LogEntry)
            RaiseEvent EntryLogged(Me, New EntryLoggedEventArgs(entry))
        End Sub
        
        ''' <summary>
        ''' Raise BufferOverflow event
        ''' </summary>
        Private Sub RaiseBufferOverflow(removedEntry As LogEntry)
            RaiseEvent BufferOverflow(Me, New BufferOverflowEventArgs(removedEntry))
        End Sub
        
        ''' <summary>
        ''' Raise LoggingError event
        ''' </summary>
        Private Sub RaiseLoggingError(message As String, ex As Exception)
            RaiseEvent LoggingError(Me, New LoggingErrorEventArgs(message, ex))
        End Sub
        
        ''' <summary>
        ''' Dispose resources
        ''' </summary>
        Public Sub Dispose() Implements IDisposable.Dispose
            If _disposed Then Return
            
            StopLogging()
            
            SyncLock _lockObject
                _logEntries.Clear()
            End SyncLock
            
            _disposed = True
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for entry logged event
    ''' </summary>
    Public Class EntryLoggedEventArgs
        Inherits EventArgs
        
        Public Property Entry As LogEntry
        Public Property Timestamp As DateTime
        
        Public Sub New(entry As LogEntry)
            Me.Entry = entry
            Me.Timestamp = DateTime.Now
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for buffer overflow event
    ''' </summary>
    Public Class BufferOverflowEventArgs
        Inherits EventArgs
        
        Public Property RemovedEntry As LogEntry
        Public Property Timestamp As DateTime
        
        Public Sub New(removedEntry As LogEntry)
            Me.RemovedEntry = removedEntry
            Me.Timestamp = DateTime.Now
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for logging error event
    ''' </summary>
    Public Class LoggingErrorEventArgs
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
