Imports System.IO
Imports System.Data
Imports System.IO.Ports
Imports System.Net
Imports Newtonsoft.Json
Imports Newtonsoft.Json.Linq


Public Class FrmSMGE_3222M_CommonCalibrationWindow

    Dim Config As Calibrations2
    Dim ConfigStr As String
    Dim Ack1 As Boolean = False
    Public CalMode As Boolean = False
    Dim Page_name As String

    Private Sub CmdSave_Click(sender As Object, e As EventArgs) Handles CmdSave.Click

        'New add ===================================================
        FrmSMGE_3222MLoging.TimerSync.Enabled = False
        FrmSMGE_3222MLoging.TimerSync.Stop()
        FrmSMGE_3222MLoging.TxtComTerm.Clear()
        'New add ===================================================

        Dim intResponse As Integer



        intResponse = MsgBox("Are you sure you want to save settings?", vbYesNo + vbQuestion, "New Settings Save")



        If intResponse = vbYes Then

            CmdSave.Enabled = False
            'FrmSMGE_3222MLoging.TimerMonitor.Start()

            CalMode = True
            'TimerHandler1_Client.Start()
            SendCalibrationToHost()



        End If

    End Sub

    Function SendCalibrationToHost()

        Dim SetState As Boolean = FrmSMGE_3222MLoging.CalMode

        If SetState = True Then

            Try


                Config = New Calibrations2

                ''//Configuration Registers
                'Config.MMode0 = FrmSMGE_3222M_Configuration.TxtMMode0.Text
                'Config.MMode1 = FrmSMGE_3222M_Configuration.TxtMMode1.Text
                'Config.PMPGA = FrmSMGE_3222M_Configuration.TxtPMPGA.Text
                'Config.PStartTh = FrmSMGE_3222M_Configuration.TxtPStartTh.Text
                'Config.QStartTh = FrmSMGE_3222M_Configuration.TxtQStartTh.Text
                'Config.SStartTh = FrmSMGE_3222M_Configuration.TxtSStartTh.Text
                'Config.PPhaseTh = FrmSMGE_3222M_Configuration.TxtPPhaseTh.Text
                'Config.QPhaseTh = FrmSMGE_3222M_Configuration.TxtQPhaseTh.Text
                'Config.SPhaseTh = FrmSMGE_3222M_Configuration.TxtSPhaseTh.Text

                '//Common Calibration Registers
                Config.Page_Name = "Cal_2"
                Config.PoffsetA = TxtPoffsetA.Text
                Config.QoffsetA = TxtQoffsetA.Text
                Config.POffsetB = TxtPOffsetB.Text
                Config.QOffsetB = TxtQOffsetB.Text
                Config.POffsetC = TxtPOffsetC.Text
                Config.QOffsetC = TxtQOffsetC.Text
                Config.GainA = TxtGainA.Text
                Config.PhiA = TxtPhiA.Text
                Config.GainB = TxtGainB.Text
                Config.PhiB = TxtPhiB.Text
                Config.GainC = TxtGainC.Text
                Config.PhiC = TxtPhiC.Text

                '//Fundamental/ Harmonic Energy Calibration registers
                'Config.POffsetAF = FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPOffsetAF.Text
                'Config.POffsetBF = FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPOffsetBF.Text
                'Config.POffsetCF = FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPOffsetCF.Text
                'Config.PGainAF = FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPGainAF.Text
                'Config.PGainBF = FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPGainBF.Text
                'Config.PGainCF = FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPGainCF.Text

                ''//Measurement Calibration
                'Config.UgainA = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUgainA.Text
                'Config.IgainA = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainA.Text
                'Config.UoffsetA = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUoffsetA.Text
                'Config.IoffsetA = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetA.Text
                'Config.UgainB = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUgainB.Text
                'Config.IgainB = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainB.Text
                'Config.UoffsetB = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUoffsetB.Text
                'Config.IoffsetB = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetB.Text
                'Config.UgainC = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUgainC.Text
                'Config.IgainC = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainC.Text
                'Config.UoffsetC = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUoffsetC.Text
                'Config.IoffsetC = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetC.Text
                'Config.IgainN = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainN.Text
                'Config.IoffsetN = FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetN.Text




                ConfigStr = JsonConvert.SerializeObject(Config, Formatting.Indented)
                'MsgBox(ConfigStr)



                If FrmSMGE_3222MLoging.SerialPort.IsOpen = False Then
                    Dim intResponse As Integer

                    intResponse = MsgBox("Do you want to reset the Meter?", vbYesNo + vbQuestion, "Reset Meter")



                    If intResponse = vbNo Then

                        Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, "Code_Cal2" + vbLf)

                    ElseIf intResponse = vbYes Then

                        Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, "Cal2_RST" + vbLf)

                    End If
                End If




                If String.IsNullOrEmpty(FrmSMGE_3222MLoging.TxtComTerm.Text) Then
                    If FrmSMGE_3222MLoging.SerialPort.IsOpen Then
                        FrmSMGE_3222MLoging.SerialPort.WriteLine("Code_Cal2")

                    End If


                Else
                    FrmSMGE_3222MLoging.TxtComTerm.Clear()
                    'FrmSMGE_3222MLoging.SerialPort.WriteLine("ConfigSave")
                    'FrmSMGE_3222MLoging.TimerCommands.Enabled = True
                    'FrmSMGE_3222MLoging.TimerCommands.Start()
                End If



                'File.WriteAllText("C:\Users\chami\Documents\Arduino\FileSys\ConfigESPTCP\data\config.txt", JsonConvert.SerializeObject(Config, Formatting.Indented))
                File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\Calibration_2.txt", JsonConvert.SerializeObject(Config, Formatting.Indented))


                Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, ConfigStr + vbLf)

                If FrmSMGE_3222MLoging.SerialPort.IsOpen Then
                    FrmSMGE_3222MLoging.SerialPort.WriteLine(ConfigStr + vbLf)
                    FrmSMGE_3222MLoging.TimerCommands.Enabled = True
                    FrmSMGE_3222MLoging.TimerCommands.Start()
                    '    'MsgBox(FrmSMGE_3222MLoging.SerialPort.ReadExisting(ConfigStr + vbLf))

                End If




            Catch ex As Exception

            Finally

                'Staying on TCP conncetion
                If FrmSMGE_3222MLoging.SerialPort.IsOpen = False Then

                    FrmSMGE_3222MLoging.Show()
                    FrmSMGE_3222MLoging.CalMode = False
                    FrmSMGE_3222MLoging.TimerHandler1_Client.Start()

                    FrmSMGE_3222MLoging.TimerMonitor.Start()
                    FrmSMGE_3222MLoging.TimerMonitor.Enabled = True
                    FrmSMGE_3222MLoging.TimerCommands.Enabled = False
                    FrmSMGE_3222MLoging.TimerCommands.Stop()
                    FrmSMGE_3222MLoging.TimerSync.Enabled = False
                    FrmSMGE_3222MLoging.TimerSync.Stop()
                    Me.Close()
                    FrmSMGE_3222M_CalibrationMainWindow.Close()



                End If

            End Try
        End If

        Return True
    End Function

    Private Sub TimerHandler1_Client_Tick(sender As Object, e As EventArgs) Handles TimerHandler1_Client.Tick

        Try
            Dim StrTerminal As String

            StrTerminal = FrmSMGE_3222MLoging.TxtComTerm.Text

            If StrTerminal.Contains("/SYNC/") Then
                Ack1 = True
                FrmSMGE_3222MLoging.TxtComTerm.Clear()
                'TBox.Clear()

                'Dim Comstatus As Boolean = FrmSMGE_3222MLoging.Ack

                If Ack1 = True Then

                    CmdSave.Enabled = True

                Else
                    CmdSave.Enabled = False

                End If


            Else
                Ack1 = False
            End If


        Catch ex As Exception

        End Try
    End Sub

    Private Sub FrmSMGE_3222M_Configuration_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        'CmdSave.Enabled = False
    End Sub

    Private Sub CmdLoad_Click(sender As Object, e As EventArgs) Handles CmdLoad.Click

        Dim Settings As New Calibrations2

        Dim SRSerial As StreamReader

        Try

            SRSerial = New StreamReader("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration2.txt")
            Dim ConfigSerializer As New JsonSerializer()
            Settings = ConfigSerializer.Deserialize(SRSerial, Settings.GetType)
            SRSerial.Close()



            ''//Configuration Registers
            'FrmSMGE_3222M_Configuration.TxtMMode0.Text = CInt(Settings.MMode0)
            'FrmSMGE_3222M_Configuration.TxtMMode1.Text = CInt(Settings.MMode1)
            'FrmSMGE_3222M_Configuration.TxtPMPGA.Text = CInt(Settings.PMPGA)
            'FrmSMGE_3222M_Configuration.TxtPStartTh.Text = CInt(Settings.PStartTh)
            'FrmSMGE_3222M_Configuration.TxtQStartTh.Text = CInt(Settings.QStartTh)
            'FrmSMGE_3222M_Configuration.TxtSStartTh.Text = CInt(Settings.SStartTh)
            'FrmSMGE_3222M_Configuration.TxtPPhaseTh.Text = CInt(Settings.PPhaseTh)
            'FrmSMGE_3222M_Configuration.TxtQPhaseTh.Text = CInt(Settings.QPhaseTh)
            'FrmSMGE_3222M_Configuration.TxtSPhaseTh.Text = CInt(Settings.SPhaseTh)



            '//Common Calibration Registers
            Page_name = Settings.Page_Name
            TxtPoffsetA.Text = CInt(Settings.PoffsetA)
            TxtQoffsetA.Text = CInt(Settings.QoffsetA)
            TxtPOffsetB.Text = CInt(Settings.POffsetB)
            TxtQOffsetB.Text = CInt(Settings.QOffsetB)
            TxtPOffsetC.Text = CInt(Settings.POffsetC)
            TxtQOffsetC.Text = CInt(Settings.QOffsetC)
            TxtGainA.Text = CInt(Settings.GainA)
            TxtPhiA.Text = CInt(Settings.PhiA)
            TxtGainB.Text = CInt(Settings.GainB)
            TxtPhiB.Text = CInt(Settings.PhiB)
            TxtGainC.Text = CInt(Settings.GainC)
            TxtPhiC.Text = CInt(Settings.PhiC)



            ''//Fundamental/ Harmonic Energy Calibration registers
            'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPOffsetAF.Text = CInt(Settings.POffsetAF)
            'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPOffsetBF.Text = CInt(Settings.POffsetBF)
            'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPOffsetCF.Text = CInt(Settings.POffsetCF)
            'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPGainAF.Text = CInt(Settings.PGainAF)
            'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPGainBF.Text = CInt(Settings.PGainBF)
            'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TxtPGainCF.Text = CInt(Settings.PGainCF)




            ''//Measurement Calibration
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUgainA.Text = CInt(Settings.UgainA)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainA.Text = CInt(Settings.IgainA)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUoffsetA.Text = CInt(Settings.UoffsetA)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetA.Text = CInt(Settings.IoffsetA)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUgainB.Text = CInt(Settings.UgainB)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainB.Text = CInt(Settings.IgainB)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUoffsetB.Text = CInt(Settings.UoffsetB)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetB.Text = CInt(Settings.IoffsetB)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUgainC.Text = CInt(Settings.UgainC)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainC.Text = CInt(Settings.IgainC)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtUoffsetC.Text = CInt(Settings.UoffsetC)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetC.Text = CInt(Settings.IoffsetC)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIgainN.Text = CInt(Settings.IgainN)
            'FrmSMGE_3222M_MeasurementCalibrationWindow.TxtIoffsetN.Text = CInt(Settings.IoffsetN)

        Catch ex As Exception


        End Try








    End Sub

    Private Sub CmdCancel_Click(sender As Object, e As EventArgs) Handles CmdCancel.Click
        Me.Hide()
        TimerHandler1_Client.Stop()

        FrmSMGE_3222M_CalibrationMainWindow.Show()
    End Sub
End Class