Option Strict On

' SM-GE3222M V2.0 - Calibration Data Model
' Stores calibration parameters for ATM90E36

Namespace Models
    ''' <summary>
    ''' Calibration data for ATM90E36 energy metering IC
    ''' </summary>
    Public Class CalibrationData
        ' PGA Gain Settings (per phase)
        Public Property PgaGainA As UShort = &H0000
        Public Property PgaGainB As UShort = &H0000
        Public Property PgaGainC As UShort = &H0000
        
        ' Voltage Gain (per phase)
        Public Property UgainA As UShort = &H0000
        Public Property UgainB As UShort = &H0000
        Public Property UgainC As UShort = &H0000
        
        ' Current Gain (per phase)
        Public Property IgainA As UShort = &H0000
        Public Property IgainB As UShort = &H0000
        Public Property IgainC As UShort = &H0000
        
        ' Phase Calibration
        Public Property UoffsetA As Short = 0
        Public Property UoffsetB As Short = 0
        Public Property UoffsetC As Short = 0
        Public Property IoffsetA As Short = 0
        Public Property IoffsetB As Short = 0
        Public Property IoffsetC As Short = 0
        
        ' Power Offset
        Public Property PoffsetA As Short = 0
        Public Property PoffsetB As Short = 0
        Public Property PoffsetC As Short = 0
        Public Property QoffsetA As Short = 0
        Public Property QoffsetB As Short = 0
        Public Property QoffsetC As Short = 0
        
        ' Meter Configuration
        Public Property MMode0 As UShort = &H1087  ' Default mode
        Public Property MMode1 As UShort = &H1500
        Public Property PStartTh As UShort = &H08BD
        Public Property QStartTh As UShort = &H0AEC
        Public Property PPhaseTh As UShort = &H00BC
        Public Property QPhaseTh As UShort = &H0AEC
        
        ' Checksum
        Public Property CS1 As UShort = 0
        Public Property CS2 As UShort = 0
        Public Property CS3 As UShort = 0
        
        ''' <summary>
        ''' Create default calibration data
        ''' </summary>
        Public Sub New()
            ' Initialize with default values
        End Sub
        
        ''' <summary>
        ''' Validate calibration data checksum
        ''' </summary>
        Public Function ValidateChecksum() As Boolean
            ' TODO: Implement checksum validation
            Return True
        End Function
        
        ''' <summary>
        ''' Clone calibration data
        ''' </summary>
        Public Function Clone() As CalibrationData
            Return DirectCast(Me.MemberwiseClone(), CalibrationData)
        End Function
    End Class
End Namespace
