' SM-GE3222M V2.0 - Phase Data Model
' Represents single-phase energy measurement data

Namespace Models
    ''' <summary>
    ''' Represents energy data for a single phase (A, B, or C)
    ''' </summary>
    Public Class PhaseData
        ' RMS Values
        Public Property VoltageRMS As Single        ' Voltage RMS (V)
        Public Property CurrentRMS As Single        ' Current RMS (A)
        
        ' Power
        Public Property ActivePower As Single       ' Active Power (W)
        Public Property ReactivePower As Single     ' Reactive Power (VAR)
        Public Property ApparentPower As Single     ' Apparent Power (VA)
        Public Property PowerFactor As Single       ' Power Factor (0.0 to 1.0)
        
        ' Phase Angles
        Public Property MeanPhaseAngle As Single    ' Mean Phase Angle (degrees)
        Public Property VoltagePhaseAngle As Single ' Voltage Phase Angle (degrees)
        
        ' THD (Total Harmonic Distortion)
        Public Property VoltageTHDN As Single       ' Voltage THD (%)
        Public Property CurrentTHDN As Single       ' Current THD (%)
        
        ' Fundamental/Harmonic Power
        Public Property FundamentalPower As Single  ' Fundamental Power (W)
        Public Property HarmonicPower As Single     ' Harmonic Power (W)
        
        ' Energy Accumulators
        Public Property FwdActiveEnergy As Single   ' Forward Active Energy (kWh)
        Public Property RevActiveEnergy As Single   ' Reverse Active Energy (kWh)
        Public Property FwdReactiveEnergy As Single ' Forward Reactive Energy (kVARh)
        Public Property RevReactiveEnergy As Single ' Reverse Reactive Energy (kVARh)
        Public Property ApparentEnergy As Single    ' Apparent Energy (kVAh)
        
        ''' <summary>
        ''' Initialize with zero values
        ''' </summary>
        Public Sub New()
            VoltageRMS = 0
            CurrentRMS = 0
            ActivePower = 0
            ReactivePower = 0
            ApparentPower = 0
            PowerFactor = 0
            MeanPhaseAngle = 0
            VoltagePhaseAngle = 0
            VoltageTHDN = 0
            CurrentTHDN = 0
            FundamentalPower = 0
            HarmonicPower = 0
            FwdActiveEnergy = 0
            RevActiveEnergy = 0
            FwdReactiveEnergy = 0
            RevReactiveEnergy = 0
            ApparentEnergy = 0
        End Sub
        
        ''' <summary>
        ''' Clone this phase data
        ''' </summary>
        Public Function Clone() As PhaseData
            Return DirectCast(Me.MemberwiseClone(), PhaseData)
        End Function
        
        ''' <summary>
        ''' Check if data is valid (not all zeros)
        ''' </summary>
        Public Function IsValid() As Boolean
            Return VoltageRMS > 0 OrElse CurrentRMS > 0 OrElse ActivePower <> 0
        End Function
    End Class
End Namespace
