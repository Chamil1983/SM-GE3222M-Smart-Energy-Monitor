' SM-GE3222M V2.0 - Meter Snapshot Model
' Complete snapshot of 3-phase energy meter data

Imports System.Text.Json
Imports System.Text.RegularExpressions

Namespace Models
    ''' <summary>
    ''' Complete snapshot of meter data (all 3 phases + totals + board data)
    ''' </summary>
    Public Class MeterSnapshot
        ' Three Phases
        Public Property PhaseA As PhaseData
        Public Property PhaseB As PhaseData
        Public Property PhaseC As PhaseData
        
        ' Totals
        Public Property TotalActivePower As Single       ' Total Active Power (W)
        Public Property TotalReactivePower As Single     ' Total Reactive Power (VAR)
        Public Property TotalApparentPower As Single     ' Total Apparent Power (VA)
        Public Property TotalPowerFactor As Single       ' Total Power Factor
        Public Property TotalFwdActiveEnergy As Single   ' Total Forward Active Energy (kWh)
        Public Property TotalRevActiveEnergy As Single   ' Total Reverse Active Energy (kWh)
        Public Property TotalFwdReactiveEnergy As Single ' Total Forward Reactive Energy (kVARh)
        Public Property TotalRevReactiveEnergy As Single ' Total Reverse Reactive Energy (kVARh)
        Public Property TotalApparentEnergy As Single    ' Total Apparent Energy (kVAh)
        
        ' Neutral & Frequency
        Public Property NeutralCurrent As Single    ' Neutral Current (A)
        Public Property Frequency As Single         ' Line Frequency (Hz)
        
        ' Board Data
        Public Property BoardTemperature As Single  ' PCB Temperature (°C)
        Public Property AmbientTemperature As Single ' Ambient Temperature (°C)
        Public Property AmbientHumidity As Single   ' Ambient Humidity (%)
        
        ' Status
        Public Property MeteringStatus0 As UShort   ' ATM90E36 Status Register 0
        Public Property MeteringStatus1 As UShort   ' ATM90E36 Status Register 1
        Public Property Timestamp As UInteger       ' Unix timestamp (seconds)
        Public Property SequenceNumber As UInteger  ' Incrementing sequence number
        Public Property Valid As Boolean            ' Data validity flag
        
        ''' <summary>
        ''' Initialize with empty phase data
        ''' </summary>
        Public Sub New()
            PhaseA = New PhaseData()
            PhaseB = New PhaseData()
            PhaseC = New PhaseData()
            Valid = False
        End Sub
        
        ''' <summary>
        ''' Parse V1 Tag:Value protocol data
        ''' This replaces the old fixed line-number parsing with tag-based dictionary
        ''' </summary>
        ''' <param name="tagValueMap">Dictionary of tag -> value pairs</param>
        ''' <returns>Parsed MeterSnapshot</returns>
        Public Shared Function FromTagValueMap(tagValueMap As Dictionary(Of String, Single)) As MeterSnapshot
            Dim snapshot As New MeterSnapshot()
            snapshot.Valid = True
            
            ' Phase A Energy (AE1-AE5)
            If tagValueMap.ContainsKey("AE1") Then snapshot.PhaseA.FwdActiveEnergy = tagValueMap("AE1")
            If tagValueMap.ContainsKey("AE2") Then snapshot.PhaseA.RevActiveEnergy = tagValueMap("AE2")
            If tagValueMap.ContainsKey("AE3") Then snapshot.PhaseA.FwdReactiveEnergy = tagValueMap("AE3")
            If tagValueMap.ContainsKey("AE4") Then snapshot.PhaseA.RevReactiveEnergy = tagValueMap("AE4")
            If tagValueMap.ContainsKey("AE5") Then snapshot.PhaseA.ApparentEnergy = tagValueMap("AE5")
            
            ' Phase A Power (AP1-AP10)
            If tagValueMap.ContainsKey("AP1") Then snapshot.PhaseA.ActivePower = tagValueMap("AP1")
            If tagValueMap.ContainsKey("AP2") Then snapshot.PhaseA.ReactivePower = tagValueMap("AP2")
            If tagValueMap.ContainsKey("AP3") Then snapshot.PhaseA.ApparentPower = tagValueMap("AP3")
            If tagValueMap.ContainsKey("AP4") Then snapshot.PhaseA.VoltageRMS = tagValueMap("AP4")
            If tagValueMap.ContainsKey("AP5") Then snapshot.PhaseA.CurrentRMS = tagValueMap("AP5")
            If tagValueMap.ContainsKey("AP6") Then snapshot.PhaseA.PowerFactor = tagValueMap("AP6")
            If tagValueMap.ContainsKey("AP7") Then snapshot.PhaseA.MeanPhaseAngle = tagValueMap("AP7")
            If tagValueMap.ContainsKey("AP8") Then snapshot.PhaseA.FundamentalPower = tagValueMap("AP8")
            If tagValueMap.ContainsKey("AP9") Then snapshot.PhaseA.HarmonicPower = tagValueMap("AP9")
            If tagValueMap.ContainsKey("AP10") Then snapshot.PhaseA.VoltagePhaseAngle = tagValueMap("AP10")
            
            ' Phase B Energy (BE1-BE5)
            If tagValueMap.ContainsKey("BE1") Then snapshot.PhaseB.FwdActiveEnergy = tagValueMap("BE1")
            If tagValueMap.ContainsKey("BE2") Then snapshot.PhaseB.RevActiveEnergy = tagValueMap("BE2")
            If tagValueMap.ContainsKey("BE3") Then snapshot.PhaseB.FwdReactiveEnergy = tagValueMap("BE3")
            If tagValueMap.ContainsKey("BE4") Then snapshot.PhaseB.RevReactiveEnergy = tagValueMap("BE4")
            If tagValueMap.ContainsKey("BE5") Then snapshot.PhaseB.ApparentEnergy = tagValueMap("BE5")
            
            ' Phase B Power (BP1-BP10)
            If tagValueMap.ContainsKey("BP1") Then snapshot.PhaseB.ActivePower = tagValueMap("BP1")
            If tagValueMap.ContainsKey("BP2") Then snapshot.PhaseB.ReactivePower = tagValueMap("BP2")
            If tagValueMap.ContainsKey("BP3") Then snapshot.PhaseB.ApparentPower = tagValueMap("BP3")
            If tagValueMap.ContainsKey("BP4") Then snapshot.PhaseB.VoltageRMS = tagValueMap("BP4")
            If tagValueMap.ContainsKey("BP5") Then snapshot.PhaseB.CurrentRMS = tagValueMap("BP5")
            If tagValueMap.ContainsKey("BP6") Then snapshot.PhaseB.PowerFactor = tagValueMap("BP6")
            If tagValueMap.ContainsKey("BP7") Then snapshot.PhaseB.MeanPhaseAngle = tagValueMap("BP7")
            If tagValueMap.ContainsKey("BP8") Then snapshot.PhaseB.FundamentalPower = tagValueMap("BP8")
            If tagValueMap.ContainsKey("BP9") Then snapshot.PhaseB.HarmonicPower = tagValueMap("BP9")
            If tagValueMap.ContainsKey("BP10") Then snapshot.PhaseB.VoltagePhaseAngle = tagValueMap("BP10")
            
            ' Phase C Energy (CE1-CE5)
            If tagValueMap.ContainsKey("CE1") Then snapshot.PhaseC.FwdActiveEnergy = tagValueMap("CE1")
            If tagValueMap.ContainsKey("CE2") Then snapshot.PhaseC.RevActiveEnergy = tagValueMap("CE2")
            If tagValueMap.ContainsKey("CE3") Then snapshot.PhaseC.FwdReactiveEnergy = tagValueMap("CE3")
            If tagValueMap.ContainsKey("CE4") Then snapshot.PhaseC.RevReactiveEnergy = tagValueMap("CE4")
            If tagValueMap.ContainsKey("CE5") Then snapshot.PhaseC.ApparentEnergy = tagValueMap("CE5")
            
            ' Phase C Power (CP1-CP10)
            If tagValueMap.ContainsKey("CP1") Then snapshot.PhaseC.ActivePower = tagValueMap("CP1")
            If tagValueMap.ContainsKey("CP2") Then snapshot.PhaseC.ReactivePower = tagValueMap("CP2")
            If tagValueMap.ContainsKey("CP3") Then snapshot.PhaseC.ApparentPower = tagValueMap("CP3")
            If tagValueMap.ContainsKey("CP4") Then snapshot.PhaseC.VoltageRMS = tagValueMap("CP4")
            If tagValueMap.ContainsKey("CP5") Then snapshot.PhaseC.CurrentRMS = tagValueMap("CP5")
            If tagValueMap.ContainsKey("CP6") Then snapshot.PhaseC.PowerFactor = tagValueMap("CP6")
            If tagValueMap.ContainsKey("CP7") Then snapshot.PhaseC.MeanPhaseAngle = tagValueMap("CP7")
            If tagValueMap.ContainsKey("CP8") Then snapshot.PhaseC.FundamentalPower = tagValueMap("CP8")
            If tagValueMap.ContainsKey("CP9") Then snapshot.PhaseC.HarmonicPower = tagValueMap("CP9")
            If tagValueMap.ContainsKey("CP10") Then snapshot.PhaseC.VoltagePhaseAngle = tagValueMap("CP10")
            
            ' Totals (TP1-TP10, TE1-TE6)
            If tagValueMap.ContainsKey("TP1") Then snapshot.TotalActivePower = tagValueMap("TP1")
            If tagValueMap.ContainsKey("TP2") Then snapshot.TotalReactivePower = tagValueMap("TP2")
            If tagValueMap.ContainsKey("TP3") Then snapshot.TotalApparentPower = tagValueMap("TP3")
            If tagValueMap.ContainsKey("TP4") Then snapshot.Frequency = tagValueMap("TP4")
            If tagValueMap.ContainsKey("TP5") Then snapshot.NeutralCurrent = tagValueMap("TP5")
            If tagValueMap.ContainsKey("TP6") Then snapshot.TotalPowerFactor = tagValueMap("TP6")
            
            If tagValueMap.ContainsKey("TE1") Then snapshot.TotalFwdActiveEnergy = tagValueMap("TE1")
            If tagValueMap.ContainsKey("TE2") Then snapshot.TotalRevActiveEnergy = tagValueMap("TE2")
            If tagValueMap.ContainsKey("TE3") Then snapshot.TotalFwdReactiveEnergy = tagValueMap("TE3")
            If tagValueMap.ContainsKey("TE4") Then snapshot.TotalRevReactiveEnergy = tagValueMap("TE4")
            If tagValueMap.ContainsKey("TE5") Then snapshot.TotalApparentEnergy = tagValueMap("TE5")
            
            ' Board Data (DP1-DP3)
            If tagValueMap.ContainsKey("DP1") Then snapshot.AmbientHumidity = tagValueMap("DP1")
            If tagValueMap.ContainsKey("DP2") Then snapshot.AmbientTemperature = tagValueMap("DP2")
            If tagValueMap.ContainsKey("DP3") Then snapshot.BoardTemperature = tagValueMap("DP3")
            
            Return snapshot
        End Function
        
        ''' <summary>
        ''' Parse V2 JSON protocol data
        ''' </summary>
        ''' <param name="jsonString">JSON string from V2 protocol</param>
        ''' <returns>Parsed MeterSnapshot</returns>
        Public Shared Function FromJson(jsonString As String) As MeterSnapshot
            Try
                Dim snapshot As MeterSnapshot = JsonSerializer.Deserialize(Of MeterSnapshot)(jsonString)
                snapshot.Valid = True
                Return snapshot
            Catch ex As Exception
                ' Return invalid snapshot on parse error
                Return New MeterSnapshot() With {.Valid = False}
            End Try
        End Function
        
        ''' <summary>
        ''' Parse incoming text data and auto-detect V1 or V2 protocol
        ''' </summary>
        ''' <param name="data">Raw text data from meter</param>
        ''' <returns>Parsed MeterSnapshot</returns>
        Public Shared Function ParseData(data As String) As MeterSnapshot
            ' Auto-detect protocol
            If data.TrimStart().StartsWith("{") Then
                ' V2 JSON protocol
                Return FromJson(data)
            Else
                ' V1 Tag:Value protocol
                Dim tagMap As New Dictionary(Of String, Single)
                Dim regex As New Regex("^([A-Z]{2}\d{1,2})([-.\d]+)", RegexOptions.Multiline)
                Dim matches = regex.Matches(data)
                
                For Each match As Match In matches
                    If match.Success Then
                        Dim tag As String = match.Groups(1).Value
                        Dim value As Single
                        If Single.TryParse(match.Groups(2).Value, value) Then
                            tagMap(tag) = value
                        End If
                    End If
                Next
                
                Return FromTagValueMap(tagMap)
            End If
        End Function
        
        ''' <summary>
        ''' Check if snapshot has valid data
        ''' </summary>
        Public Function IsValid() As Boolean
            Return Valid AndAlso (PhaseA.IsValid() OrElse PhaseB.IsValid() OrElse PhaseC.IsValid())
        End Function
    End Class
End Namespace
