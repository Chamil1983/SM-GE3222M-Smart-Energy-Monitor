Option Strict On

' SM-GE3222M V2.0 - Meter Configuration Model

Namespace Models
    Public Class MeterConfig
        Public Property IpAddress As String = "192.168.1.100"
        Public Property Port As Integer = 8088
        Public Property SubnetMask As String = "255.255.255.0"
        Public Property Gateway As String = "192.168.1.1"
        Public Property BaudRate As Integer = 115200
        Public Property DataBits As Integer = 8
        Public Property Parity As String = "None"
        Public Property StopBits As String = "One"
        Public Property LineFrequency As Integer = 50
        Public Property VoltageSensorType As String = "PT"
        Public Property CurrentSensorType As String = "CT"
        Public Property SamplingRate As Integer = 1000
        Public Property DataUpdateInterval As Integer = 1000
        Public Property DisplayUnits As String = "Metric"
        Public Property DecimalPlaces As Integer = 2
        Public Property LoggingEnabled As Boolean = True
        Public Property LoggingInterval As Integer = 60
        Public Property MaxLogEntries As Integer = 10000
        Public Property OverVoltageThreshold As Single = 260.0F
        Public Property UnderVoltageThreshold As Single = 180.0F
        Public Property OverCurrentThreshold As Single = 100.0F
        Public Property OverTemperatureThreshold As Single = 80.0F
        
        Public Sub New()
        End Sub
        
        Public Function Validate() As Boolean
            If Port < 1 OrElse Port > 65535 Then Return False
            If LineFrequency <> 50 AndAlso LineFrequency <> 60 Then Return False
            If SamplingRate < 100 OrElse SamplingRate > 10000 Then Return False
            Return True
        End Function
        
        Public Function Clone() As MeterConfig
            Return DirectCast(Me.MemberwiseClone(), MeterConfig)
        End Function
    End Class
End Namespace
