Option Strict On

Namespace Models
    Public Enum LogEntryType
        Data = 0
        [Event] = 1
        Alarm = 2
        [Error] = 3
        SystemInfo = 4
    End Enum
    
    Public Class LogEntry
        Public Property Timestamp As DateTime
        Public Property EntryType As LogEntryType
        Public Property Snapshot As MeterSnapshot
        Public Property Message As String
        Public Property Severity As Integer
        
        Public Sub New()
            Timestamp = DateTime.Now
            EntryType = LogEntryType.Data
            Message = String.Empty
            Severity = 0
        End Sub
        
        Public Sub New(snapshot As MeterSnapshot, Optional message As String = "", Optional entryType As LogEntryType = LogEntryType.Data)
            Me.Timestamp = DateTime.Now
            Me.Snapshot = snapshot
            Me.Message = message
            Me.EntryType = entryType
        End Sub
        
        Public Function ToCsvString() As String
            If Snapshot Is Nothing Then
                Return $"{Timestamp:yyyy-MM-dd HH:mm:ss},{EntryType},{Message}"
            End If
            
            Return $"{Timestamp:yyyy-MM-dd HH:mm:ss},{Snapshot.PhaseA.VoltageRMS:F2},{Snapshot.PhaseA.CurrentRMS:F3}," &
                   $"{Snapshot.PhaseA.ActivePower:F2},{Snapshot.PhaseB.VoltageRMS:F2},{Snapshot.PhaseB.CurrentRMS:F3}," &
                   $"{Snapshot.PhaseB.ActivePower:F2},{Snapshot.PhaseC.VoltageRMS:F2},{Snapshot.PhaseC.CurrentRMS:F3}," &
                   $"{Snapshot.PhaseC.ActivePower:F2},{Snapshot.TotalActivePower:F2},{Snapshot.Frequency:F2}"
        End Function
        
        Public Shared Function GetCsvHeader() As String
            Return "Timestamp,VA,IA,PA,VB,IB,PB,VC,IC,PC,TotalPower,Frequency"
        End Function
    End Class
End Namespace
