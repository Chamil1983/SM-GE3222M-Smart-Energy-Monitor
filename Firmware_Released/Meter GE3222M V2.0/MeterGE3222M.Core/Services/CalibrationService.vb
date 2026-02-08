Option Strict On

' SM-GE3222M V2.0 - Calibration Service
' Manages calibration operations for ATM90E36 meter
' Read/write calibration registers, calculate gains, validation

Imports System.Threading.Tasks
Imports MeterGE3222M.Core.Communication
Imports MeterGE3222M.Core.Models

Namespace Services
    ''' <summary>
    ''' Calibration service for ATM90E36 energy meter
    ''' Handles calibration register operations and gain calculations
    ''' </summary>
    Public Class CalibrationService
        Private ReadOnly _commandSender As CommandSender
        Private ReadOnly _connection As IMeterConnection
        
        ' ATM90E36 Register addresses (for reference)
        Private Const REG_UGAIN_A As UShort = &H61
        Private Const REG_UGAIN_B As UShort = &H62
        Private Const REG_UGAIN_C As UShort = &H63
        Private Const REG_IGAIN_A As UShort = &H64
        Private Const REG_IGAIN_B As UShort = &H65
        Private Const REG_IGAIN_C As UShort = &H66
        Private Const REG_UOFFSET_A As UShort = &H67
        Private Const REG_UOFFSET_B As UShort = &H68
        Private Const REG_UOFFSET_C As UShort = &H69
        Private Const REG_IOFFSET_A As UShort = &H6A
        Private Const REG_IOFFSET_B As UShort = &H6B
        Private Const REG_IOFFSET_C As UShort = &H6C
        Private Const REG_POFFSET_A As UShort = &H6D
        Private Const REG_POFFSET_B As UShort = &H6E
        Private Const REG_POFFSET_C As UShort = &H6F
        Private Const REG_QOFFSET_A As UShort = &H70
        Private Const REG_QOFFSET_B As UShort = &H71
        Private Const REG_QOFFSET_C As UShort = &H72
        
        ''' <summary>
        ''' Event raised when calibration operation progresses
        ''' </summary>
        Public Event CalibrationProgress As EventHandler(Of CalibrationProgressEventArgs)
        
        ''' <summary>
        ''' Event raised when calibration error occurs
        ''' </summary>
        Public Event CalibrationError As EventHandler(Of CalibrationErrorEventArgs)
        
        ''' <summary>
        ''' Constructor
        ''' </summary>
        ''' <param name="connection">Meter connection instance</param>
        Public Sub New(connection As IMeterConnection)
            If connection Is Nothing Then
                Throw New ArgumentNullException(NameOf(connection))
            End If
            
            _connection = connection
            _commandSender = New CommandSender(connection)
        End Sub
        
        ''' <summary>
        ''' Read complete calibration data from meter
        ''' </summary>
        ''' <returns>CalibrationData object with current values</returns>
        Public Async Function ReadCalibrationAsync() As Task(Of CalibrationData)
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                RaiseProgress("Reading calibration data...", 0)
                
                Dim calData As New CalibrationData()
                
                ' Read voltage gains
                RaiseProgress("Reading voltage gains...", 10)
                calData.UgainA = Await ReadRegisterValueAsync(REG_UGAIN_A)
                calData.UgainB = Await ReadRegisterValueAsync(REG_UGAIN_B)
                calData.UgainC = Await ReadRegisterValueAsync(REG_UGAIN_C)
                
                ' Read current gains
                RaiseProgress("Reading current gains...", 30)
                calData.IgainA = Await ReadRegisterValueAsync(REG_IGAIN_A)
                calData.IgainB = Await ReadRegisterValueAsync(REG_IGAIN_B)
                calData.IgainC = Await ReadRegisterValueAsync(REG_IGAIN_C)
                
                ' Read voltage offsets
                RaiseProgress("Reading voltage offsets...", 50)
                calData.UoffsetA = CShort(Await ReadRegisterValueAsync(REG_UOFFSET_A))
                calData.UoffsetB = CShort(Await ReadRegisterValueAsync(REG_UOFFSET_B))
                calData.UoffsetC = CShort(Await ReadRegisterValueAsync(REG_UOFFSET_C))
                
                ' Read current offsets
                RaiseProgress("Reading current offsets...", 70)
                calData.IoffsetA = CShort(Await ReadRegisterValueAsync(REG_IOFFSET_A))
                calData.IoffsetB = CShort(Await ReadRegisterValueAsync(REG_IOFFSET_B))
                calData.IoffsetC = CShort(Await ReadRegisterValueAsync(REG_IOFFSET_C))
                
                ' Read power offsets
                RaiseProgress("Reading power offsets...", 90)
                calData.PoffsetA = CShort(Await ReadRegisterValueAsync(REG_POFFSET_A))
                calData.PoffsetB = CShort(Await ReadRegisterValueAsync(REG_POFFSET_B))
                calData.PoffsetC = CShort(Await ReadRegisterValueAsync(REG_POFFSET_C))
                calData.QoffsetA = CShort(Await ReadRegisterValueAsync(REG_QOFFSET_A))
                calData.QoffsetB = CShort(Await ReadRegisterValueAsync(REG_QOFFSET_B))
                calData.QoffsetC = CShort(Await ReadRegisterValueAsync(REG_QOFFSET_C))
                
                RaiseProgress("Calibration data read successfully", 100)
                Return calData
                
            Catch ex As Exception
                RaiseError("Failed to read calibration data", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Write complete calibration data to meter
        ''' </summary>
        ''' <param name="calData">Calibration data to write</param>
        Public Async Function WriteCalibrationAsync(calData As CalibrationData) As Task
            If calData Is Nothing Then
                Throw New ArgumentNullException(NameOf(calData))
            End If
            
            If Not _connection.IsConnected Then
                Throw New InvalidOperationException("Not connected to meter")
            End If
            
            Try
                RaiseProgress("Writing calibration data...", 0)
                
                ' Write voltage gains
                RaiseProgress("Writing voltage gains...", 10)
                Await WriteRegisterValueAsync(REG_UGAIN_A, calData.UgainA)
                Await WriteRegisterValueAsync(REG_UGAIN_B, calData.UgainB)
                Await WriteRegisterValueAsync(REG_UGAIN_C, calData.UgainC)
                
                ' Write current gains
                RaiseProgress("Writing current gains...", 30)
                Await WriteRegisterValueAsync(REG_IGAIN_A, calData.IgainA)
                Await WriteRegisterValueAsync(REG_IGAIN_B, calData.IgainB)
                Await WriteRegisterValueAsync(REG_IGAIN_C, calData.IgainC)
                
                ' Write voltage offsets
                RaiseProgress("Writing voltage offsets...", 50)
                Await WriteRegisterValueAsync(REG_UOFFSET_A, CUShort(calData.UoffsetA))
                Await WriteRegisterValueAsync(REG_UOFFSET_B, CUShort(calData.UoffsetB))
                Await WriteRegisterValueAsync(REG_UOFFSET_C, CUShort(calData.UoffsetC))
                
                ' Write current offsets
                RaiseProgress("Writing current offsets...", 70)
                Await WriteRegisterValueAsync(REG_IOFFSET_A, CUShort(calData.IoffsetA))
                Await WriteRegisterValueAsync(REG_IOFFSET_B, CUShort(calData.IoffsetB))
                Await WriteRegisterValueAsync(REG_IOFFSET_C, CUShort(calData.IoffsetC))
                
                ' Write power offsets
                RaiseProgress("Writing power offsets...", 90)
                Await WriteRegisterValueAsync(REG_POFFSET_A, CUShort(calData.PoffsetA))
                Await WriteRegisterValueAsync(REG_POFFSET_B, CUShort(calData.PoffsetB))
                Await WriteRegisterValueAsync(REG_POFFSET_C, CUShort(calData.PoffsetC))
                Await WriteRegisterValueAsync(REG_QOFFSET_A, CUShort(calData.QoffsetA))
                Await WriteRegisterValueAsync(REG_QOFFSET_B, CUShort(calData.QoffsetB))
                Await WriteRegisterValueAsync(REG_QOFFSET_C, CUShort(calData.QoffsetC))
                
                RaiseProgress("Calibration data written successfully", 100)
                
            Catch ex As Exception
                RaiseError("Failed to write calibration data", ex)
                Throw
            End Try
        End Function
        
        ''' <summary>
        ''' Validate calibration data
        ''' </summary>
        ''' <param name="calData">Calibration data to validate</param>
        ''' <returns>True if valid, False otherwise</returns>
        Public Function ValidateCalibration(calData As CalibrationData) As Boolean
            If calData Is Nothing Then
                Return False
            End If
            
            ' Check voltage gains are non-zero
            If calData.UgainA = 0 OrElse calData.UgainB = 0 OrElse calData.UgainC = 0 Then
                Return False
            End If
            
            ' Check current gains are non-zero
            If calData.IgainA = 0 OrElse calData.IgainB = 0 OrElse calData.IgainC = 0 Then
                Return False
            End If
            
            ' Check checksum if available
            If calData.Checksum <> 0 Then
                Dim calculatedChecksum = CalculateChecksum(calData)
                If calculatedChecksum <> calData.Checksum Then
                    Return False
                End If
            End If
            
            Return True
        End Function
        
        ''' <summary>
        ''' Calculate voltage gain from reference measurement
        ''' </summary>
        ''' <param name="measuredVoltage">Measured voltage from meter (V RMS)</param>
        ''' <param name="referenceVoltage">Reference voltage from calibrator (V RMS)</param>
        ''' <param name="currentGain">Current gain value</param>
        ''' <returns>New voltage gain value</returns>
        Public Function CalculateVoltageGain(measuredVoltage As Single, referenceVoltage As Single, currentGain As UShort) As UShort
            If measuredVoltage <= 0 OrElse referenceVoltage <= 0 Then
                Throw New ArgumentException("Voltages must be positive values")
            End If
            
            Dim ratio As Single = referenceVoltage / measuredVoltage
            Dim newGain As UShort = CUShort(currentGain * ratio)
            
            Return newGain
        End Function
        
        ''' <summary>
        ''' Calculate current gain from reference measurement
        ''' </summary>
        ''' <param name="measuredCurrent">Measured current from meter (A RMS)</param>
        ''' <param name="referenceCurrent">Reference current from calibrator (A RMS)</param>
        ''' <param name="currentGain">Current gain value</param>
        ''' <returns>New current gain value</returns>
        Public Function CalculateCurrentGain(measuredCurrent As Single, referenceCurrent As Single, currentGain As UShort) As UShort
            If measuredCurrent <= 0 OrElse referenceCurrent <= 0 Then
                Throw New ArgumentException("Currents must be positive values")
            End If
            
            Dim ratio As Single = referenceCurrent / measuredCurrent
            Dim newGain As UShort = CUShort(currentGain * ratio)
            
            Return newGain
        End Function
        
        ''' <summary>
        ''' Calculate power gain from reference measurement
        ''' </summary>
        ''' <param name="measuredPower">Measured power from meter (W)</param>
        ''' <param name="referencePower">Reference power from calibrator (W)</param>
        ''' <param name="currentGain">Current gain value</param>
        ''' <returns>New power gain value</returns>
        Public Function CalculatePowerGain(measuredPower As Single, referencePower As Single, currentGain As UShort) As UShort
            If measuredPower <= 0 OrElse referencePower <= 0 Then
                Throw New ArgumentException("Power values must be positive")
            End If
            
            Dim ratio As Single = referencePower / measuredPower
            Dim newGain As UShort = CUShort(currentGain * ratio)
            
            Return newGain
        End Function
        
        ''' <summary>
        ''' Read register value as UShort
        ''' </summary>
        Private Async Function ReadRegisterValueAsync(registerAddress As UShort) As Task(Of UShort)
            Dim response = Await _commandSender.ReadRegisterAsync(registerAddress)
            
            ' Parse response (format: "REG:0xXXXX=0xYYYY" or "0xYYYY")
            Dim value As UShort = 0
            
            If response.Contains("="c) Then
                Dim parts = response.Split("="c)
                If parts.Length >= 2 Then
                    response = parts(1).Trim()
                End If
            End If
            
            ' Parse hex value
            response = response.Trim().Replace("0x", "").Replace("0X", "")
            If UShort.TryParse(response, Globalization.NumberStyles.HexNumber, Nothing, value) Then
                Return value
            End If
            
            Return 0
        End Function
        
        ''' <summary>
        ''' Write register value
        ''' </summary>
        Private Async Function WriteRegisterValueAsync(registerAddress As UShort, value As UShort) As Task
            Await _commandSender.WriteRegisterAsync(registerAddress, value)
        End Function
        
        ''' <summary>
        ''' Calculate checksum for calibration data
        ''' </summary>
        Private Function CalculateChecksum(calData As CalibrationData) As UShort
            Dim sum As UInteger = 0
            
            sum += calData.UgainA
            sum += calData.UgainB
            sum += calData.UgainC
            sum += calData.IgainA
            sum += calData.IgainB
            sum += calData.IgainC
            
            Return CUShort(sum And &HFFFF)
        End Function
        
        ''' <summary>
        ''' Raise calibration progress event
        ''' </summary>
        Private Sub RaiseProgress(message As String, percentage As Integer)
            RaiseEvent CalibrationProgress(Me, New CalibrationProgressEventArgs(message, percentage))
        End Sub
        
        ''' <summary>
        ''' Raise calibration error event
        ''' </summary>
        Private Sub RaiseError(message As String, ex As Exception)
            RaiseEvent CalibrationError(Me, New CalibrationErrorEventArgs(message, ex))
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for calibration progress
    ''' </summary>
    Public Class CalibrationProgressEventArgs
        Inherits EventArgs
        
        Public Property Message As String
        Public Property Percentage As Integer
        Public Property Timestamp As DateTime
        
        Public Sub New(message As String, percentage As Integer)
            Me.Message = message
            Me.Percentage = percentage
            Me.Timestamp = DateTime.Now
        End Sub
    End Class
    
    ''' <summary>
    ''' Event args for calibration error
    ''' </summary>
    Public Class CalibrationErrorEventArgs
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
