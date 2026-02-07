Imports System.Linq
Imports System.Data.SqlClient
Imports System.Data.OleDb
Imports Microsoft.Office.Core
Imports Excel = Microsoft.Office.Interop.Excel
Imports ExcelAutoFormat = Microsoft.Office.Interop.Excel.XlRangeAutoFormat
Imports Microsoft.Office.Interop
Imports System.IO
Imports System.Xml.XPath
Imports System.Data
Imports System.Xml
Imports System.IO.Ports
Imports System.Net
Imports System.Net.Sockets
Imports DevComponents.DotNetBar.Controls
Imports System.Reflection
Imports System.Security.Cryptography
Imports DevComponents.DotNetBar
Imports System.Net.Mime.MediaTypeNames
Imports Xenocode
Imports System
Imports Microsoft.VisualBasic
Imports Newtonsoft.Json
Imports Newtonsoft.Json.JsonSerializerSettings
Imports Meter_GE3222M_V1._0.ClsSettings
Imports System.Reflection.Emit
Imports Meter_GE3222M_V1._0.ClsCalibrations

Public Class FrmSMGE_3222MLoging

    Dim Settings As Configurations
    Dim Calibration As Calibrations
    Dim Calibration1 As Calibrations1
    Dim Calibration2 As Calibrations2
    Dim Calibration3 As Calibrations3
    Dim Calibration4 As Calibrations4

    Dim Cal1Filter As Boolean = False
    Dim Cal2Filter As Boolean = False
    Dim Cal3Filter As Boolean = False
    Dim Cal4Filter As Boolean = False
    Public ConState As Boolean = False
    Public Ack As Boolean = False

    Dim Cal1Filater As Boolean
    Dim Cal2Filater As Boolean
    'Dim ServerStatus As Boolean = False
    'Dim ServerTrying As Boolean = False
    'Dim Server As TcpListener
    'Dim Clients As New List(Of TcpClient)
    'Dim Client As TcpClient
    ' Dim TCPServer As Socket
    'Dim TBox As New TextBox
    Public SrtJson As String = ""
    Public SetMode As Boolean = False
    Public CalMode As Boolean = False
    Private addr() As IPAddress
    Private myAdapterName, myPhysicalAddress, myGateway, myDNS, strHostName As String
    Dim DataSerIn As String '-> Variable to hold incoming serial data
    Dim Con_state As Boolean = True
    Dim Reply_SAVE As Boolean = False
    Private stopwatch1, stopwatch2, stopwatch3, stopwatch4, stopwatch5, stopwatch6, stopwatch7, stopwatch8, stopwatch9, stopwatch10, stopwatch11, stopwatch12, stopwatch13, stopwatch14 As New Diagnostics.Stopwatch '-> Stopwatch declaration used as a timer
    Dim _sec, _min As String '-> Variable to hold the minutes and seconds of the stopwatch
    Dim StrSerialIn As String
    Dim StrSerialInRam As String
    Dim StrParse, AE_1, AE_2, AE_3, AE_4, AE_5 As String
    Dim BE_1, BE_2, BE_3, BE_4, BE_5 As String
    Dim CE_1, CE_2, CE_3, CE_4, CE_5 As String
    Dim TE_1, TE_2, TE_3, TE_4, TE_5, TE_6 As String
    Dim AP_1, AP_2, AP_3, AP_4, AP_5, AP_6, AP_7, AP_8, AP_9, AP_10 As String
    Dim BP_1, BP_2, BP_3, BP_4, BP_5, BP_6, BP_7, BP_8, BP_9, BP_10 As String

    Private Sub CmdlSettings_Click(sender As Object, e As EventArgs) Handles CmdlSettings.Click
        FrmSMGE_3222MSettings.Show()
        ' TimerHandler1_Client.Stop()

        TimerSerial.Stop()
        TimerMonitor.Stop()
        TimerMonitor.Enabled = False



        SetMode = True
        TxtComTerm.Clear()

        TimerSync.Enabled = True
        TimerSync.Start()
        'TimerMonitor.Start()
        'FrmSMGE_3222MSettings.TimerHandler1_Client.Start()

        If OptTCPIP.Checked = True Then
            FrmSMGE_3222MSettings.GrpModbusSetting.Enabled = False

        End If
    End Sub

    Private Sub TimerMonitor_Tick(sender As Object, e As EventArgs) Handles TimerMonitor.Tick

        If String.IsNullOrEmpty(TxtComTerm.Text) Then
            If SerialPort.IsOpen = True Then
                SerialPort.WriteLine("PC_ENABLED")
            End If

        Else
                TxtComTerm.Clear()
        End If



        Try

            'Dim StrSerialIn As String = SerialPort.ReadExisting
            'Dim StrSerialInRam As String

            DataSerIn = ReceiveSerialData()

            TxtComTerm.Text &= DataSerIn



            'StrSerialIn = SerialPort.ReadExisting
            Dim TB As New TextBox
            TB.Multiline = True
            TB.Text = TxtComTerm.Text


            '==================================================================================
            StrSerialInRam = TB.Lines(0).Substring(0, 3)
            If StrSerialInRam = "AE1" Then
                AE_1 = TB.Lines(0)
                AE_1L = AE_1.Length
            Else
                AE_1 = AE_1
            End If

            StrSerialInRam = TB.Lines(1).Substring(0, 3)
            If StrSerialInRam = "AE2" Then
                AE_2 = TB.Lines(1)
                AE_2L = AE_2.Length
            Else
                AE_2 = AE_2
            End If

            StrSerialInRam = TB.Lines(2).Substring(0, 3)
            If StrSerialInRam = "AE3" Then
                AE_3 = TB.Lines(2)
                AE_3L = AE_3.Length
            Else
                AE_3 = AE_3
            End If

            StrSerialInRam = TB.Lines(3).Substring(0, 3)
            If StrSerialInRam = "AE4" Then
                AE_4 = TB.Lines(3)
                AE_4L = AE_4.Length
            Else
                AE_4 = AE_4
            End If

            StrSerialInRam = TB.Lines(4).Substring(0, 3)
            If StrSerialInRam = "AE5" Then
                AE_5 = TB.Lines(4)
                AE_5L = AE_5.Length
            Else
                AE_5 = AE_5
            End If


            '==================================================================================
            StrSerialInRam = TB.Lines(5).Substring(0, 3)
            If StrSerialInRam = "BE1" Then
                BE_1 = TB.Lines(5)
                BE_1L = BE_1.Length
            Else
                BE_1 = BE_1
            End If

            StrSerialInRam = TB.Lines(6).Substring(0, 3)
            If StrSerialInRam = "BE2" Then
                BE_2 = TB.Lines(6)
                BE_2L = BE_2.Length
            Else
                BE_2 = BE_2
            End If

            StrSerialInRam = TB.Lines(7).Substring(0, 3)
            If StrSerialInRam = "BE3" Then
                BE_3 = TB.Lines(7)
                BE_3L = BE_3.Length
            Else
                BE_3 = BE_3
            End If

            StrSerialInRam = TB.Lines(8).Substring(0, 3)
            If StrSerialInRam = "BE4" Then
                BE_4 = TB.Lines(8)
                BE_4L = BE_4.Length
            Else
                BE_4 = BE_4
            End If

            StrSerialInRam = TB.Lines(9).Substring(0, 3)
            If StrSerialInRam = "BE5" Then
                BE_5 = TB.Lines(9)
                BE_5L = BE_5.Length
            Else
                BE_5 = BE_5
            End If

            '==================================================================================
            StrSerialInRam = TB.Lines(10).Substring(0, 3)
            If StrSerialInRam = "CE1" Then
                CE_1 = TB.Lines(10)
                CE_1L = CE_1.Length
            Else
                CE_1 = CE_1
            End If

            StrSerialInRam = TB.Lines(11).Substring(0, 3)
            If StrSerialInRam = "CE2" Then
                CE_2 = TB.Lines(11)
                CE_2L = CE_2.Length
            Else
                CE_2 = CE_2
            End If

            StrSerialInRam = TB.Lines(12).Substring(0, 3)
            If StrSerialInRam = "CE3" Then
                CE_3 = TB.Lines(12)
                CE_3L = CE_3.Length
            Else
                CE_3 = CE_3
            End If

            StrSerialInRam = TB.Lines(13).Substring(0, 3)
            If StrSerialInRam = "CE4" Then
                CE_4 = TB.Lines(13)
                CE_4L = CE_4.Length
            Else
                CE_4 = CE_4
            End If

            StrSerialInRam = TB.Lines(14).Substring(0, 3)
            If StrSerialInRam = "CE5" Then
                CE_5 = TB.Lines(14)
                CE_5L = CE_5.Length
            Else
                CE_5 = CE_5
            End If

            '==================================================================================
            StrSerialInRam = TB.Lines(15).Substring(0, 3)
            If StrSerialInRam = "TE1" Then
                TE_1 = TB.Lines(15)
                TE_1L = TE_1.Length
            Else
                BE_1 = BE_1
            End If

            StrSerialInRam = TB.Lines(16).Substring(0, 3)
            If StrSerialInRam = "TE2" Then
                TE_2 = TB.Lines(16)
                TE_2L = TE_2.Length
            Else
                TE_2 = TE_2
            End If

            StrSerialInRam = TB.Lines(17).Substring(0, 3)
            If StrSerialInRam = "TE3" Then
                TE_3 = TB.Lines(17)
                TE_3L = TE_3.Length
            Else
                TE_3 = TE_3
            End If

            StrSerialInRam = TB.Lines(18).Substring(0, 3)
            If StrSerialInRam = "TE4" Then
                TE_4 = TB.Lines(18)
                TE_4L = TE_4.Length
            Else
                TE_4 = TE_4
            End If

            StrSerialInRam = TB.Lines(19).Substring(0, 3)
            If StrSerialInRam = "TE5" Then
                TE_5 = TB.Lines(19)
                TE_5L = TE_5.Length
            Else
                TE_5 = TE_5
            End If

            StrSerialInRam = TB.Lines(20).Substring(0, 3)
            If StrSerialInRam = "TE6" Then
                TE_6 = TB.Lines(20)
                TE_6L = TE_6.Length
            Else
                TE_6 = TE_6
            End If






            FrmSM_GE3222M_Energy_Data.LblPhaA_ForwardActiveEnergyVal.Text = Mid(AE_1, 4, AE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ReverseActiveEnergyVal.Text = Mid(AE_2, 4, AE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ForwardReactiveEnergyVal.Text = Mid(AE_3, 4, AE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ReverseReactiveEnergyVal.Text = Mid(AE_4, 4, AE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ApparentEnergyVal.Text = Mid(AE_5, 4, AE_5L) & " KVAh"

            FrmSM_GE3222M_Energy_Data.LblPhaB_ForwardActiveEnergyVal.Text = Mid(BE_1, 4, BE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ReverseActiveEnergyVal.Text = Mid(BE_2, 4, BE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ForwardReactiveEnergyVal.Text = Mid(BE_3, 4, BE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ReverseReactiveEnergyVal.Text = Mid(BE_4, 4, BE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ApparentEnergyVal.Text = Mid(BE_5, 4, BE_5L) & " KVAh"

            FrmSM_GE3222M_Energy_Data.LblPhaC_ForwardActiveEnergyVal.Text = Mid(CE_1, 4, CE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ReverseActiveEnergyVal.Text = Mid(CE_2, 4, CE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ForwardReactiveEnergyVal.Text = Mid(CE_3, 4, CE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ReverseReactiveEnergyVal.Text = Mid(CE_4, 4, CE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ApparentEnergyVal.Text = Mid(CE_5, 4, CE_5L) & " KVAh"

            FrmSM_GE3222M_Energy_Data.LblTotalForwardActiveEnergyVal.Text = Mid(TE_1, 4, TE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblTotalReverseActiveEnergyVal.Text = Mid(TE_2, 4, TE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblTotalForwardReactiveEnergyVal.Text = Mid(TE_3, 4, TE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblTotalReverseReactiveEnergyVal.Text = Mid(TE_4, 4, TE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblTotal_ArithmeticSum_ApparentEnergyVal.Text = Mid(TE_5, 4, TE_5L) & " KVAh"
            FrmSM_GE3222M_Energy_Data.LblVectorSum_TotalApparentEnergyVal.Text = Mid(TE_6, 4, TE_6L) & " KVAh"

            '==================================================================================
            StrSerialInRam = TB.Lines(21).Substring(0, 3)
            If StrSerialInRam = "AP1" Then
                AP_1 = TB.Lines(21)
                AP_1L = AP_1.Length
            Else
                AP_1 = AP_1
            End If

            StrSerialInRam = TB.Lines(22).Substring(0, 3)
            If StrSerialInRam = "AP2" Then
                AP_2 = TB.Lines(22)
                AP_2L = AP_2.Length
            Else
                AP_2 = AP_2
            End If

            StrSerialInRam = TB.Lines(23).Substring(0, 3)
            If StrSerialInRam = "AP3" Then
                AP_3 = TB.Lines(23)
                AP_3L = AP_3.Length
            Else
                AP_3 = AP_3
            End If

            StrSerialInRam = TB.Lines(24).Substring(0, 3)
            If StrSerialInRam = "AP4" Then
                AP_4 = TB.Lines(24)
                AP_4L = AP_4.Length
            Else
                AP_4 = AP_4
            End If

            StrSerialInRam = TB.Lines(25).Substring(0, 3)
            If StrSerialInRam = "AP5" Then
                AP_5 = TB.Lines(25)
                AP_5L = AP_5.Length
            Else
                AP_5 = AP_5
            End If


            StrSerialInRam = TB.Lines(26).Substring(0, 3)
            If StrSerialInRam = "AP6" Then
                AP_6 = TB.Lines(26)
                AP_6L = AP_6.Length
            Else
                AP_6 = AP_6
            End If

            StrSerialInRam = TB.Lines(27).Substring(0, 3)
            If StrSerialInRam = "AP7" Then
                AP_7 = TB.Lines(27)
                AP_7L = AP_7.Length
            Else
                AP_7 = AP_7
            End If

            StrSerialInRam = TB.Lines(28).Substring(0, 3)
            If StrSerialInRam = "AP8" Then
                AP_8 = TB.Lines(28)
                AP_8L = AP_8.Length
            Else
                AP_8 = AP_8
            End If

            StrSerialInRam = TB.Lines(29).Substring(0, 3)
            If StrSerialInRam = "AP9" Then
                AP_9 = TB.Lines(29)
                AP_9L = AP_9.Length
            Else
                AP_9 = AP_9
            End If

            StrSerialInRam = TB.Lines(30).Substring(0, 4)
            If StrSerialInRam = "AP10" Then
                AP_10 = TB.Lines(30)
                AP_10L = AP_10.Length
            Else
                AP_10 = AP_10
            End If


            '==================================================================================
            StrSerialInRam = TB.Lines(31).Substring(0, 3)
            If StrSerialInRam = "BP1" Then
                BP_1 = TB.Lines(31)
                BP_1L = BP_1.Length
            Else
                BP_1 = BP_1
            End If

            StrSerialInRam = TB.Lines(32).Substring(0, 3)
            If StrSerialInRam = "BP2" Then
                BP_2 = TB.Lines(32)
                BP_2L = BP_2.Length
            Else
                BP_2 = BP_2
            End If

            StrSerialInRam = TB.Lines(33).Substring(0, 3)
            If StrSerialInRam = "BP3" Then
                BP_3 = TB.Lines(33)
                BP_3L = BP_3.Length
            Else
                BP_3 = BP_3
            End If

            StrSerialInRam = TB.Lines(34).Substring(0, 3)
            If StrSerialInRam = "BP4" Then
                BP_4 = TB.Lines(34)
                BP_4L = BP_4.Length
            Else
                BP_4 = BP_4
            End If

            StrSerialInRam = TB.Lines(35).Substring(0, 3)
            If StrSerialInRam = "BP5" Then
                BP_5 = TB.Lines(35)
                BP_5L = BP_5.Length
            Else
                BP_5 = BP_5
            End If


            StrSerialInRam = TB.Lines(36).Substring(0, 3)
            If StrSerialInRam = "BP6" Then
                BP_6 = TB.Lines(36)
                BP_6L = BP_6.Length
            Else
                BP_6 = BP_6
            End If

            StrSerialInRam = TB.Lines(37).Substring(0, 3)
            If StrSerialInRam = "BP7" Then
                BP_7 = TB.Lines(37)
                BP_7L = BP_7.Length
            Else
                BP_7 = BP_7
            End If

            StrSerialInRam = TB.Lines(38).Substring(0, 3)
            If StrSerialInRam = "BP8" Then
                BP_8 = TB.Lines(38)
                BP_8L = BP_8.Length
            Else
                BP_8 = BP_8
            End If

            StrSerialInRam = TB.Lines(39).Substring(0, 3)
            If StrSerialInRam = "BP9" Then
                BP_9 = TB.Lines(39)
                BP_9L = BP_9.Length
            Else
                BP_9 = BP_9
            End If

            StrSerialInRam = TB.Lines(40).Substring(0, 4)
            If StrSerialInRam = "BP10" Then
                BP_10 = TB.Lines(40)
                BP_10L = BP_10.Length
            Else
                BP_10 = BP_10
            End If



            '==================================================================================
            StrSerialInRam = TB.Lines(41).Substring(0, 3)
            If StrSerialInRam = "CP1" Then
                CP_1 = TB.Lines(41)
                CP_1L = CP_1.Length
            Else
                CP_1 = CP_1
            End If

            StrSerialInRam = TB.Lines(42).Substring(0, 3)
            If StrSerialInRam = "CP2" Then
                CP_2 = TB.Lines(42)
                CP_2L = CP_2.Length
            Else
                CP_2 = CP_2
            End If

            StrSerialInRam = TB.Lines(43).Substring(0, 3)
            If StrSerialInRam = "CP3" Then
                CP_3 = TB.Lines(43)
                CP_3L = CP_3.Length
            Else
                CP_3 = CP_3
            End If

            StrSerialInRam = TB.Lines(44).Substring(0, 3)
            If StrSerialInRam = "CP4" Then
                CP_4 = TB.Lines(44)
                CP_4L = CP_4.Length
            Else
                CP_4 = CP_4
            End If

            StrSerialInRam = TB.Lines(45).Substring(0, 3)
            If StrSerialInRam = "CP5" Then
                CP_5 = TB.Lines(45)
                CP_5L = CP_5.Length
            Else
                CP_5 = CP_5
            End If


            StrSerialInRam = TB.Lines(46).Substring(0, 3)
            If StrSerialInRam = "CP6" Then
                CP_6 = TB.Lines(46)
                CP_6L = CP_6.Length
            Else
                CP_6 = CP_6
            End If

            StrSerialInRam = TB.Lines(47).Substring(0, 3)
            If StrSerialInRam = "CP7" Then
                CP_7 = TB.Lines(47)
                CP_7L = CP_7.Length
            Else
                CP_7 = CP_7
            End If

            StrSerialInRam = TB.Lines(48).Substring(0, 3)
            If StrSerialInRam = "CP8" Then
                CP_8 = TB.Lines(48)
                CP_8L = CP_8.Length
            Else
                CP_8 = CP_8
            End If

            StrSerialInRam = TB.Lines(49).Substring(0, 3)
            If StrSerialInRam = "CP9" Then
                CP_9 = TB.Lines(49)
                CP_9L = CP_9.Length
            Else
                CP_9 = CP_9
            End If

            StrSerialInRam = TB.Lines(50).Substring(0, 4)
            If StrSerialInRam = "CP10" Then
                CP_10 = TB.Lines(50)
                CP_10L = CP_10.Length
            Else
                CP_10 = CP_10
            End If



            '==================================================================================
            StrSerialInRam = TB.Lines(51).Substring(0, 3)
            If StrSerialInRam = "TP1" Then
                TP_1 = TB.Lines(51)
                TP_1L = TP_1.Length
            Else
                TP_1 = TP_1
            End If

            StrSerialInRam = TB.Lines(52).Substring(0, 3)
            If StrSerialInRam = "TP2" Then
                TP_2 = TB.Lines(52)
                TP_2L = TP_2.Length
            Else
                TP_2 = TP_2
            End If

            StrSerialInRam = TB.Lines(53).Substring(0, 3)
            If StrSerialInRam = "TP3" Then
                TP_3 = TB.Lines(53)
                TP_3L = TP_3.Length
            Else
                TP_3 = TP_3
            End If

            StrSerialInRam = TB.Lines(54).Substring(0, 3)
            If StrSerialInRam = "TP4" Then
                TP_4 = TB.Lines(54)
                TP_4L = TP_4.Length
            Else
                TP_4 = TP_4
            End If

            StrSerialInRam = TB.Lines(55).Substring(0, 3)
            If StrSerialInRam = "TP5" Then
                TP_5 = TB.Lines(55)
                TP_5L = TP_5.Length
            Else
                TP_5 = TP_5
            End If


            StrSerialInRam = TB.Lines(56).Substring(0, 3)
            If StrSerialInRam = "TP6" Then
                TP_6 = TB.Lines(56)
                TP_6L = TP_6.Length
            Else
                TP_6 = TP_6
            End If

            StrSerialInRam = TB.Lines(57).Substring(0, 3)
            If StrSerialInRam = "TP7" Then
                TP_7 = TB.Lines(57)
                TP_7L = TP_7.Length
            Else
                TP_7 = TP_7
            End If

            StrSerialInRam = TB.Lines(58).Substring(0, 3)
            If StrSerialInRam = "TP8" Then
                TP_8 = TB.Lines(58)
                TP_8L = TP_8.Length
            Else
                TP_8 = TP_8
            End If

            StrSerialInRam = TB.Lines(59).Substring(0, 3)
            If StrSerialInRam = "TP9" Then
                TP_9 = TB.Lines(59)
                TP_9L = TP_9.Length
            Else
                TP_9 = TP_9
            End If

            StrSerialInRam = TB.Lines(60).Substring(0, 4)
            If StrSerialInRam = "TP10" Then
                TP_10 = TB.Lines(60)
                TP_10L = TP_10.Length
            Else
                TP_10 = TP_10
            End If

            StrSerialInRam = TB.Lines(61).Substring(0, 3)
            If StrSerialInRam = "DP1" Then
                DP_1 = TB.Lines(61)
                DP_1L = DP_1.Length
            Else
                DP_1 = DP_1
            End If

            StrSerialInRam = TB.Lines(62).Substring(0, 3)
            If StrSerialInRam = "DP2" Then
                DP_2 = TB.Lines(62)
                DP_2L = DP_2.Length
            Else
                DP_2 = DP_2
            End If

            StrSerialInRam = TB.Lines(63).Substring(0, 3)
            If StrSerialInRam = "DP3" Then
                DP_3 = TB.Lines(63)
                DP_3L = DP_3.Length
            Else
                DP_3 = DP_3
            End If

            'Voltages
            AP_7Log = Mid(AP_7, 4, AP_7L)
            BP_7Log = Mid(BP_7, 4, BP_7L)
            CP_7Log = Mid(CP_7, 4, CP_7L)

            'Currents
            AP_8Log = Mid(AP_8, 4, AP_8L)
            BP_8Log = Mid(BP_8, 4, BP_8L)
            CP_8Log = Mid(CP_8, 4, CP_8L)
            TP_9Log = Mid(TP_9, 4, TP_9L)
            TP_10Log = Mid(TP_10, 5, TP_10L)

            'Active Power
            AP_1Log = Mid(AP_1, 4, AP_1L)
            BP_1Log = Mid(BP_1, 4, BP_1L)
            CP_1Log = Mid(CP_1, 4, CP_1L)
            TP_1Log = Mid(TP_1, 4, TP_1L)

            'Reactive Power
            AP_2Log = Mid(AP_2, 4, AP_2L)
            BP_2Log = Mid(BP_2, 4, BP_2L)
            CP_2Log = Mid(CP_2, 4, CP_2L)
            TP_2Log = Mid(TP_2, 4, TP_2L)

            'Apperant Power
            AP_3Log = Mid(AP_3, 4, AP_3L)
            BP_3Log = Mid(BP_3, 4, BP_3L)
            CP_3Log = Mid(CP_3, 4, CP_3L)
            TP_3Log = Mid(TP_3, 4, TP_3L)

            'Power Factor
            AP_5Log = Mid(AP_5, 4, AP_5L)
            BP_5Log = Mid(BP_5, 4, BP_5L)
            CP_5Log = Mid(CP_5, 4, CP_5L)
            TP_5Log = Mid(TP_5, 4, TP_5L)

            'Phase Angle
            AP_9Log = Mid(AP_9, 4, AP_9L)
            BP_9Log = Mid(BP_9, 4, BP_9L)
            CP_9Log = Mid(CP_9, 4, CP_9L)

            'Frequency
            TP_8Log = Mid(TP_8, 4, TP_8L)

            'Active Energy Imp
            AE_1Log = Mid(AE_1, 4, AE_1L)
            BE_1Log = Mid(BE_1, 4, BE_1L)
            CE_1Log = Mid(CE_1, 4, CE_1L)
            TE_1Log = Mid(TE_1, 4, TE_1L)

            'Active Energy Exp
            AE_2Log = Mid(AE_2, 4, AE_2L)
            BE_2Log = Mid(BE_2, 4, BE_2L)
            CE_2Log = Mid(CE_2, 4, CE_2L)
            TE_2Log = Mid(TE_2, 4, TE_2L)


            'Reactive Energy Imp
            AE_3Log = Mid(AP_3, 4, AP_3L)
            BE_3Log = Mid(BP_3, 4, BP_3L)
            CE_3Log = Mid(CP_3, 4, CP_3L)
            TE_3Log = Mid(TE_3, 4, TE_3L)

            'Reactive Energy Exp
            AE_4Log = Mid(AE_4, 4, AE_4L)
            BE_4Log = Mid(BE_4, 4, BE_4L)
            CE_4Log = Mid(CE_4, 4, CE_4L)
            TE_4Log = Mid(TE_4, 4, TE_4L)


            'Apperent Energy
            AE_5Log = Mid(AE_5, 4, AE_5L)
            BE_5Log = Mid(BE_5, 4, BE_5L)
            CE_5Log = Mid(CE_5, 4, CE_5L)
            TE_5Log = Mid(TE_5, 4, TE_5L)
            TE_6Log = Mid(TE_6, 4, TE_6L)


            'Temperature and Humidity
            DP_1Log = Mid(DP_1, 4, DP_1L)
            DP_2Log = Mid(DP_2, 4, DP_2L)
            DP_3Log = Mid(DP_3, 4, DP_3L)


            FrmSMGE_3222M_BoardData.LblHumidityVal.Text = Mid(DP_1, 4, DP_1L)
            FrmSMGE_3222M_BoardData.LblTempCVal.Text = Mid(DP_2, 4, DP_2L)
            FrmSMGE_3222M_VoltageData.LblV1Val.Text = Mid(AP_7, 4, AP_7L)
            FrmSMGE_3222M_VoltageData.LblV2Val.Text = Mid(BP_7, 4, BP_7L)
            FrmSMGE_3222M_VoltageData.LblV3Val.Text = Mid(CP_7, 4, CP_7L)



            FrmVoltage.TxtVoltage.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "=============================="
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & ""
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 1 Voltage :   " & Mid(AP_7, 4, AP_7L) & "V"
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 2 Voltage :   " & Mid(BP_7, 4, BP_7L) & "V"
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 3 Voltage :   " & Mid(CP_7, 4, CP_7L) & "V"
            'FrmVoltage.TxtVoltage.Focus()
            'FrmVoltage.TxtVoltage.SelectionStart = FrmVoltage.TxtVoltage.Text.Length


            FrmSMGE_3222M_CurrentData.LblI1Val.Text = Mid(AP_8, 4, AP_8L)
            FrmSMGE_3222M_CurrentData.LblI2Val.Text = Mid(BP_8, 4, BP_8L)
            FrmSMGE_3222M_CurrentData.LblI3Val.Text = Mid(CP_8, 4, CP_8L)
            FrmSMGE_3222M_CurrentData.LB1.Text = Mid(TP_9, 4, TP_9L)
            FrmSMGE_3222M_CurrentData.LB2.Text = Mid(TP_10, 5, TP_10L)

            FrmCurrent.TxtCurrent.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "=============================="
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & ""
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 1 Current      :   " & Mid(AP_8, 4, AP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 2 Current      :   " & Mid(BP_8, 4, BP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 3 Current      :   " & Mid(CP_8, 4, CP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Neutral Current    :   " & Mid(TP_9, 4, TP_9L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Total Current      :   " & Mid(TP_10, 5, TP_10L) & "A"

            FrmSMGE_3222M_ActPowerData.TxtPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 1 Power      :   " & Mid(AP_1, 4, AP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 2 Power      :   " & Mid(BP_1, 4, BP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 3 Power      :   " & Mid(CP_1, 4, CP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Total Power      :   " & Mid(TP_1, 4, TP_1L) & "W"



            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 1 Reactive Power :   " & Mid(AP_2, 4, AP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 2 Reactive Power :   " & Mid(BP_2, 4, BP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 3 Reactive Power :   " & Mid(CP_2, 4, CP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Total Reactive Power :   " & Mid(TP_2, 4, TP_2L) & "VAR"


            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 1 Apparent Power :   " & Mid(AP_3, 4, AP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 2 Apparent Power :   " & Mid(BP_3, 4, BP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 3 Apparent Power :   " & Mid(CP_3, 4, CP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Total Apparent Power :   " & Mid(TP_3, 4, TP_3L) & "VA"


            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 1 Power Factor :   " & Mid(AP_5, 4, AP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 2 Power Factor :   " & Mid(BP_5, 4, BP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 3 Power Factor :   " & Mid(CP_5, 4, CP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Total Power Factor :   " & Mid(TP_5, 4, TP_5L)


            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 1 Angle       :   " & Mid(AP_9, 4, AP_9L) & "°"
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 2 Angle       :   " & Mid(BP_9, 4, BP_9L) & "°"
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 3 Angler      :   " & Mid(CP_9, 4, CP_9L) & "°"

            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & "Frequency   :   " & Mid(TP_8, 4, TP_8L) & "Hz"


            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Energy Imported"
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 1 Active Energy :   " & Mid(AE_1, 4, AE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 2 Active Energy :   " & Mid(BE_1, 4, BE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 3 Active Energy :   " & Mid(CE_1, 4, CE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Total Power Factor :   " & Mid(TE_1, 4, TE_1L)


            FrmSM_GE3222M_Power_Data.LblPhaA_ActivePowerVal.Text = Mid(AP_1, 4, AP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_ReactivePowerVal.Text = Mid(AP_2, 4, AP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaA_apparentpowerVal.Text = Mid(AP_3, 4, AP_3L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblPhaA_activefundamentalpowerVal.Text = Mid(AP_4, 4, AP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_powerfactorVal.Text = Mid(AP_5, 4, AP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaA_activeharmonicpowerVal.Text = Mid(AP_6, 4, AP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_voltageRMSVal.Text = Mid(AP_7, 4, AP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaA_currentRMSVal.Text = Mid(AP_8, 4, AP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaA_meanphaseangleVal.Text = Mid(AP_9, 4, AP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaA_voltagephaseangleVal.Text = Mid(AP_10, 5, AP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblPhaB_ActivePowerVal.Text = Mid(BP_1, 4, BP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_ReactivePowerVal.Text = Mid(BP_2, 4, BP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaB_apparentpowerVal.Text = Mid(BP_3, 4, BP_3L) & " VA"
            FrmSM_GE3222M_Power_Data.LblPhaB_activefundamentalpowerVal.Text = Mid(BP_4, 4, BP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_powerfactorVal.Text = Mid(BP_5, 4, BP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaB_activeharmonicpowerVal.Text = Mid(BP_6, 4, BP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_voltageRMSVal.Text = Mid(BP_7, 4, BP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaB_currentRMSVal.Text = Mid(BP_8, 4, BP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaB_meanphaseangleVal.Text = Mid(BP_9, 4, BP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaB_voltagephaseangleVal.Text = Mid(BP_10, 5, BP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblPhaC_ActivePowerVal.Text = Mid(CP_1, 4, CP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_ReactivePowerVal.Text = Mid(CP_2, 4, CP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaC_apparentpowerVal.Text = Mid(CP_3, 4, CP_3L) & " VA"
            FrmSM_GE3222M_Power_Data.LblPhaC_activefundamentalpowerVal.Text = Mid(CP_4, 4, CP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_powerfactorVal.Text = Mid(CP_5, 4, CP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaC_activeharmonicpowerVal.Text = Mid(CP_6, 4, CP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_voltageRMSVal.Text = Mid(CP_7, 4, CP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaC_currentRMSVal.Text = Mid(CP_8, 4, CP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaC_meanphaseangleVal.Text = Mid(CP_9, 4, CP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaC_voltagephaseangleVal.Text = Mid(CP_10, 5, CP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblTotal_PhasesumActivePowerVal.Text = Mid(TP_1, 4, TP_1L) & " kW"
            FrmSM_GE3222M_Power_Data.LblTotal_PhasesumReactivePowerVal.Text = Mid(TP_2, 4, TP_2L) & " kvar"
            FrmSM_GE3222M_Power_Data.LblTotal_ArithmeticSumapparentpowerVal.Text = Mid(TP_3, 4, TP_3L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblVectorSum_TotalApparentPowerVal.Text = Mid(TP_4, 4, TP_4L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblTotalpowerfactorVal.Text = Mid(TP_5, 4, TP_5L)
            FrmSM_GE3222M_Power_Data.LblTotalactivefundamentalpowerVal.Text = Mid(TP_6, 4, TP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblTotalactiveharmonicpowerVal.Text = Mid(TP_7, 4, TP_7L) & " W"
            FrmSM_GE3222M_Power_Data.LblLblFrequency.Text = Mid(TP_8, 4, TP_8L) & " Hz"
            FrmSM_GE3222M_Power_Data.LblN_Line_SampledcurrentRMSVal.Text = Mid(TP_9, 4, TP_9L) & " A"
            FrmSM_GE3222M_Power_Data.LblTotalLine_currentRMSVal.Text = Mid(TP_10, 5, TP_10L) & " A"

            SerialPort.WriteLine("Req_End")
            'SerialPort.Close()

        Catch ex As Exception

        End Try

    End Sub


    Private Sub TimerCommands_Tick(sender As Object, e As EventArgs) Handles TimerCommands.Tick



        If TxtComTerm.Text.Contains("/SYNC/") Then
            TxtComTerm.Clear()
        End If


        If SerialPort.IsOpen = True Then
            DataSerIn = ReceiveSerialData()

            TxtComTerm.Text &= DataSerIn
        End If


        'Dim StrSerialIn As String = SerialPort.ReadExisting
        'Dim StrSerialInRam As String
        '    If TxtComTerm.Text.Contains("/SAVED/") Then

        '        'TxtComTerm.Clear()
        '        MsgBox("Configuration Saved. Meter reset!", MessageBoxIcon.Information)
        '    TimerCommands.Enabled = False
        '    TimerCommands.Stop()
        'End If

        'If Not TxtComTerm.Text.Contains("Saved") Then
        '    SrtJson = FrmSMGE_3222MSettings.ConfigStr
        '    SerialPort.WriteLine(SrtJson + vbLf)
        '    ' MsgBox(SrtJson + vbLf)

        'Else

        'End If






    End Sub

    Private Sub TimerSync_Tick(sender As Object, e As EventArgs) Handles TimerSync.Tick
        If TxtComTerm.Text.Contains("/SYNC/") Then
            TxtComTerm.Clear()
            FrmSMGE_3222MSettings.CmdSave.Enabled = True
            FrmSMGE_3222M_Configuration.CmdSave.Enabled = True
            FrmSMGE_3222M_CommonCalibrationWindow.CmdSave.Enabled = True
            FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.CmdSave.Enabled = True
            FrmSMGE_3222M_MeasurementCalibrationWindow.CmdSave.Enabled = True
        End If

        If SerialPort.IsOpen = True Then
            DataSerIn = ReceiveSerialData()

            TxtComTerm.Text &= DataSerIn
        End If

    End Sub



    Function ReceiveSerialData() As String
        Dim Incoming As String
        Try
            Incoming = SerialPort.ReadExisting()
            If Incoming Is Nothing Then
                Return "nothing" & vbCrLf
            Else
                Return Incoming
            End If
        Catch ex As TimeoutException
            Return "Error: Serial Port read timed out."
        End Try

    End Function

    Private Sub CmdExit_Click(sender As Object, e As EventArgs) Handles CmdExit.Click
        ' TimerMonitor.Start()
    End Sub

    Private Sub CmdCallibrate_Click(sender As Object, e As EventArgs) Handles CmdCallibrate.Click
        'Me.Hide()
        FrmSMGE_3222M_CalibrationMainWindow.Show()

        TimerMonitor.Stop()
        TimerMonitor.Enabled = False

        TimerHandler1_Client.Stop()

        CalMode = True
        TxtComTerm.Clear()

        'TimerSync.Enabled = True
        'TimerSync.Start()



        '===============================================


    End Sub

    Private Sub TextBox1_TextChanged(sender As Object, e As EventArgs) Handles TxtComTerm.TextChanged

        Dim JString As String = TxtComTerm.Text

        Try
            If JString.Contains("/SYNC/") Then
                Ack = True
                FrmSMGE_3222MSettings.CmdSave.Enabled = True
                FrmSMGE_3222M_Configuration.CmdSave.Enabled = True
                FrmSMGE_3222M_CommonCalibrationWindow.CmdSave.Enabled = True
                FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.CmdSave.Enabled = True
                FrmSMGE_3222M_MeasurementCalibrationWindow.CmdSave.Enabled = True

            Else
                Ack = False
            End If

        Catch ex As Exception

        End Try

        'If TimerCommands.Enabled = True Then
        '    If JString.Contains("/SAVED/") Then

        '        'TxtComTerm.Clear()
        '        MsgBox("Configuration Saved. Meter reset!", MessageBoxIcon.Information)
        '        TimerCommands.Enabled = False
        '        TimerCommands.Stop()

        '        FrmSMGE_3222MSettings.CmdSave.Enabled = True
        '    End If
        'End If


        If SetMode = True Then




            Try


                Settings = New Configurations

                'Dim JString As String = TxtComTerm.Text
                Dim SavedConfig = JsonConvert.DeserializeObject(Of MeterSettings)(JString)

                Settings.Page_Name = SavedConfig.Page_Name
                Settings.COM_MODE = SavedConfig.COM_MODE
                Settings.Config_State = SavedConfig.ConfigState
                Settings.SerialBaud = SavedConfig.SerialBaud
                Settings.WIFi_Mode = SavedConfig.WIFi_Mode
                Settings.WIFi_State = SavedConfig.WIFi_State
                Settings.WIFiClient_Mode = SavedConfig.WiFiClient_Mode
                Settings.WiFiServerPort = SavedConfig.WiFiServerPort
                Settings.Publish_Interval = SavedConfig.PublishInterval
                Settings.MDBUS_BAUD = SavedConfig.MDBUSBAUD
                Settings.MDBus_TCPPort = SavedConfig.MDBusTCPPort
                Settings.Config_Mode = SavedConfig.ConfigMode
                Settings.WIFiSSID = SavedConfig.WIFiSSID
                Settings.WiFiPassword = SavedConfig.WiFiPassword
                Settings.ETH_Mode = SavedConfig.ETH_Mode
                Settings.Server_IP = SavedConfig.ServerIP
                Settings.ETH_HostIP = SavedConfig.ETHHostIP
                Settings.ETH_TCPPort = SavedConfig.ETHTCPPort
                Settings.MDBus_TCPHostIP = SavedConfig.MDBusTCPHostIP
                Settings.WiFiHost_IP = SavedConfig.WiFiHostIP
                Settings.WiFiGateway = SavedConfig.WiFiGateway
                Settings.WiFiSubnet = SavedConfig.WiFiSubnet
                Settings.DNS1 = SavedConfig.DNS1
                Settings.DNS2 = SavedConfig.DNS2
                Settings.MDBUS_Mode = SavedConfig.MDBUS_Mode
                Settings.MDBUS_ID = SavedConfig.MDBUSID
                Settings.WiFiAP_Mode = SavedConfig.WiFiAP_Mode
                Settings.WiFiSoftAP_IP = SavedConfig.WiFiSoftAP_IP
                Settings.SoftAP_SSID = SavedConfig.SoftAP_SSID
                Settings.SoftAP_Password = SavedConfig.SoftAP_Password



                If String.Equals(Settings.Config_Mode, "Saved") Then

                    If SerialPort.IsOpen = False Then
                        MsgBox("Configuration Saved!", MessageBoxIcon.Information)

                        '---------------------------------------------------------------------------
                        'Dim intResponse As Integer

                        'intResponse = MsgBox("Configuration Saved. Do you want to reset the Meter?", vbYesNo + vbQuestion, "Meter Reset")



                        'If intResponse = vbYes Then
                        '    Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, "Reboot" + vbLf)
                        '    FrmSMGE_3222MSettings.Close()
                        '    MsgBox("Please Connect to Serial Port", MessageBoxIcon.Information)
                        'End If

                        '-------------------------------------------------------------------------

                    End If


                    If (String.Equals(Settings.COM_MODE, "TCP")) And SerialPort.IsOpen = True Then
                        MsgBox("Please Connect to TCP/IP!", MessageBoxIcon.Information, "TCP Connection")
                    End If

                    'If String.Equals(Settings.COM_MODE, "TCP") Then

                    'End If


                    File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewConfig.txt", JsonConvert.SerializeObject(Settings, Newtonsoft.Json.Formatting.Indented))


                    Dim ConfigStr As String
                    ConfigStr = JsonConvert.SerializeObject(Settings, Newtonsoft.Json.Formatting.Indented)


                    MsgBox(ConfigStr, vbInformation, "Received")

                    If SerialPort.IsOpen = True Then

                        Dim intResponse As Integer

                        intResponse = MsgBox("Configuration Saved. Do you want to reset the Meter?", vbYesNo + vbQuestion, "Meter Reset")



                        If intResponse = vbYes Then



                            SerialPort.WriteLine("Reboot")
                            MsgBox("Meter reset!", MessageBoxIcon.Information)
                            SetMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222MSettings.Close()



                        Else

                            SetMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222MSettings.Close()



                        End If

                    End If




                    FrmSMGE_3222MSettings.CmdSave.Enabled = True
                End If





                'Dim SWSerial As StreamWriter
                'SWSerial = New StreamWriter("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewConfig.txt", False)
                'Dim ConfigSerializer As New JsonSerializer()
                'ConfigSerializer.Serialize(SWSerial, Settings)
                'SWSerial.Close()

                TxtComTerm.Clear()
                Reply_SAVE = False

                If Ack = True Then
                    FrmSMGE_3222MSettings.CmdSave.Enabled = True
                End If






            Catch ex As Exception

            End Try

        End If

        '=======================================================================================================================================================================================

        'Dim Cal1Filater As Boolean = FrmSMGE_3222M_Configuration.CalMode

        If JString.Contains("Cal_1") Then
            'If CalMode = True Then

            Try


                Calibration1 = New Calibrations1

                'Dim JString As String = TxtComTerm.Text
                Dim SavedConfig = JsonConvert.DeserializeObject(Of Meter_calibration1)(JString)

                '//Configuration Registers
                Calibration1.Page_Name = SavedConfig.Page_Name
                Calibration1.Trans = SavedConfig.Trans
                Calibration1.MMode0 = SavedConfig.MMode0
                Calibration1.MMode1 = SavedConfig.MMode1
                Calibration1.PMPGA = SavedConfig.PMPGA
                Calibration1.PStartTh = SavedConfig.PStartTh
                Calibration1.QStartTh = SavedConfig.QStartTh
                Calibration1.SStartTh = SavedConfig.SStartTh
                Calibration1.PPhaseTh = SavedConfig.PPhaseTh
                Calibration1.QPhaseTh = SavedConfig.QPhaseTh
                Calibration1.SPhaseTh = SavedConfig.SPhaseTh

                '=======================================================================================
                If String.Equals(Calibration1.Trans, "CAL1_SUCCESS") Then

                    If SerialPort.IsOpen = False Then
                        MsgBox("Configuration Saved.!", MessageBoxIcon.Information)
                    End If




                    File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration1.txt", JsonConvert.SerializeObject(Calibration1, Newtonsoft.Json.Formatting.Indented))


                    Dim ConfigStr As String
                    ConfigStr = JsonConvert.SerializeObject(Calibration1, Newtonsoft.Json.Formatting.Indented)


                    MsgBox(ConfigStr, vbInformation, "Received")


                    If SerialPort.IsOpen = True Then
                        Dim intResponse As Integer

                        intResponse = MsgBox("Configuration Saved. Do you want to reset the Meter?", vbYesNo + vbQuestion, "Meter Reset")



                        If intResponse = vbYes Then

                            SerialPort.WriteLine("Reboot")
                            MsgBox("Meter reset!", MessageBoxIcon.Information)
                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_Configuration.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        Else

                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_Configuration.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        End If

                    End If



                    FrmSMGE_3222M_Configuration.CmdSave.Enabled = True

                End If


                '=======================================================================================

                'Dim ConfigStr As String
                'ConfigStr = JsonConvert.SerializeObject(Calibration1, Newtonsoft.Json.Formatting.Indented)

                ''TimerMonitor.Stop()


                'MsgBox(ConfigStr)

                'File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration1.txt", JsonConvert.SerializeObject(Calibration1, Newtonsoft.Json.Formatting.Indented))



                'TxtComTerm.Clear()
                'Reply_SAVE = False

                'FrmSMGE_3222M_Configuration.CmdSave.Enabled = True
                'MsgBox("Calibration Settings Saved!", MessageBoxIcon.Information)
                TxtComTerm.Clear()
                Reply_SAVE = False

                If Ack = True Then
                    FrmSMGE_3222M_Configuration.CmdSave.Enabled = True
                End If



            Catch ex As Exception

            End Try

        End If

        '=======================================================================================================================================================================================


        If JString.Contains("Cal_2") Then
            'If Cal2Filater = True Then

            Try


                Calibration2 = New Calibrations2

                'Dim JString As String = TxtComTerm.Text
                Dim SavedConfig = JsonConvert.DeserializeObject(Of Meter_calibration2)(JString)



                '//Common Calibration Registers
                Calibration2.Page_Name = SavedConfig.Page_Name
                Calibration2.Trans = SavedConfig.Trans
                Calibration2.PoffsetA = SavedConfig.PoffsetA
                Calibration2.QoffsetA = SavedConfig.QoffsetA
                Calibration2.POffsetB = SavedConfig.POffsetB
                Calibration2.QOffsetB = SavedConfig.QOffsetB
                Calibration2.POffsetC = SavedConfig.POffsetC
                Calibration2.QOffsetC = SavedConfig.QOffsetC
                Calibration2.GainA = SavedConfig.GainA
                Calibration2.PhiA = SavedConfig.PhiA
                Calibration2.GainB = SavedConfig.GainB
                Calibration2.PhiB = SavedConfig.PhiB
                Calibration2.GainC = SavedConfig.GainC
                Calibration2.PhiC = SavedConfig.PhiC


                '=======================================================================================
                If String.Equals(Calibration2.Trans, "CAL2_SUCCESS") Then

                    If SerialPort.IsOpen = False Then
                        MsgBox("Configuration Saved.!", MessageBoxIcon.Information)
                    End If




                    File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration2.txt", JsonConvert.SerializeObject(Calibration2, Newtonsoft.Json.Formatting.Indented))


                    Dim ConfigStr As String
                    ConfigStr = JsonConvert.SerializeObject(Calibration2, Newtonsoft.Json.Formatting.Indented)


                    MsgBox(ConfigStr, vbInformation, "Received")


                    If SerialPort.IsOpen = True Then
                        Dim intResponse As Integer

                        intResponse = MsgBox("Configuration Saved. Do you want to reset the Meter?", vbYesNo + vbQuestion, "Meter Reset")



                        If intResponse = vbYes Then

                            SerialPort.WriteLine("Reboot")
                            MsgBox("Meter reset!", MessageBoxIcon.Information)
                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_CommonCalibrationWindow.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        Else

                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_CommonCalibrationWindow.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        End If

                    End If



                    FrmSMGE_3222M_CommonCalibrationWindow.CmdSave.Enabled = True

                End If


                '=======================================================================================

                TxtComTerm.Clear()
                Reply_SAVE = False

                If Ack = True Then
                    FrmSMGE_3222M_CommonCalibrationWindow.CmdSave.Enabled = True
                End If



                '=====================================================================================================================================
                'Dim ConfigStr As String
                'ConfigStr = JsonConvert.SerializeObject(Calibration2, Newtonsoft.Json.Formatting.Indented)


                'MsgBox(ConfigStr)

                'File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration2.txt", JsonConvert.SerializeObject(Calibration2, Newtonsoft.Json.Formatting.Indented))



                'TxtComTerm.Clear()
                'Reply_SAVE = False

                'FrmSMGE_3222M_CommonCalibrationWindow.CmdSave.Enabled = True
                'MsgBox("Calibration Settings Saved!", MessageBoxIcon.Information)




            Catch ex As Exception

            End Try

        End If

        '=======================================================================================================================================================================================
        If JString.Contains("Cal_3") Then

            Try


                Calibration3 = New Calibrations3

                'Dim JString As String = TxtComTerm.Text
                Dim SavedConfig = JsonConvert.DeserializeObject(Of Meter_calibration3)(JString)



                '//Fundamental/ Harmonic Energy Calibration registers
                Calibration3.Page_Name = SavedConfig.Page_Name
                Calibration3.Trans = SavedConfig.Trans
                Calibration3.POffsetAF = SavedConfig.POffsetAF
                Calibration3.POffsetBF = SavedConfig.POffsetBF
                Calibration3.POffsetCF = SavedConfig.POffsetCF
                Calibration3.PGainAF = SavedConfig.PGainAF
                Calibration3.PGainBF = SavedConfig.PGainBF
                Calibration3.PGainCF = SavedConfig.PGainCF

                '=======================================================================================
                If String.Equals(Calibration3.Trans, "CAL3_SUCCESS") Then

                    If SerialPort.IsOpen = False Then
                        MsgBox("Configuration Saved.!", MessageBoxIcon.Information)
                    End If




                    File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration3.txt", JsonConvert.SerializeObject(Calibration3, Newtonsoft.Json.Formatting.Indented))


                    Dim ConfigStr As String
                    ConfigStr = JsonConvert.SerializeObject(Calibration3, Newtonsoft.Json.Formatting.Indented)


                    MsgBox(ConfigStr, vbInformation, "Received")


                    If SerialPort.IsOpen = True Then
                        Dim intResponse As Integer

                        intResponse = MsgBox("Configuration Saved. Do you want to reset the Meter?", vbYesNo + vbQuestion, "Meter Reset")



                        If intResponse = vbYes Then

                            SerialPort.WriteLine("Reboot")
                            MsgBox("Meter reset!", MessageBoxIcon.Information)
                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        Else

                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        End If

                    End If



                    FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.CmdSave.Enabled = True

                End If


                '=======================================================================================

                TxtComTerm.Clear()
                Reply_SAVE = False

                If Ack = True Then
                    FrmSMGE_3222M_Configuration.CmdSave.Enabled = True
                End If





                'Dim ConfigStr As String
                'ConfigStr = JsonConvert.SerializeObject(Calibration3, Newtonsoft.Json.Formatting.Indented)


                'MsgBox(ConfigStr)

                'File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration3.txt", JsonConvert.SerializeObject(Calibration3, Newtonsoft.Json.Formatting.Indented))



                'TxtComTerm.Clear()
                'Reply_SAVE = False

                'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.CmdSave.Enabled = True
                'MsgBox("Calibration Settings Saved!", MessageBoxIcon.Information)




            Catch ex As Exception

            End Try

        End If

        '=======================================================================================================================================================================================
        If JString.Contains("Cal_4") Then

            Try


                Calibration4 = New Calibrations4

                'Dim JString As String = TxtComTerm.Text
                Dim SavedConfig = JsonConvert.DeserializeObject(Of Meter_calibration4)(JString)


                '//Measurement Calibration Registers
                Calibration4.Page_Name = SavedConfig.Page_Name
                Calibration4.Trans = SavedConfig.Trans
                Calibration4.UgainA = SavedConfig.UgainA
                Calibration4.IgainA = SavedConfig.IgainA
                Calibration4.UoffsetA = SavedConfig.UoffsetA
                Calibration4.IoffsetA = SavedConfig.IoffsetA
                Calibration4.UgainB = SavedConfig.UgainB
                Calibration4.IgainB = SavedConfig.IgainB
                Calibration4.UoffsetB = SavedConfig.UoffsetB
                Calibration4.IoffsetB = SavedConfig.IoffsetB
                Calibration4.UgainC = SavedConfig.UgainC
                Calibration4.IgainC = SavedConfig.IgainC
                Calibration4.UoffsetC = SavedConfig.UoffsetC
                Calibration4.IoffsetC = SavedConfig.IoffsetC
                Calibration4.IgainN = SavedConfig.IgainN
                Calibration4.IoffsetN = SavedConfig.IoffsetN


                '=======================================================================================
                If String.Equals(Calibration4.Trans, "CAL4_SUCCESS") Then

                    If SerialPort.IsOpen = False Then
                        MsgBox("Configuration Saved.!", MessageBoxIcon.Information)
                    End If




                    File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration4.txt", JsonConvert.SerializeObject(Calibration4, Newtonsoft.Json.Formatting.Indented))


                    Dim ConfigStr As String
                    ConfigStr = JsonConvert.SerializeObject(Calibration4, Newtonsoft.Json.Formatting.Indented)


                    MsgBox(ConfigStr, vbInformation, "Received")


                    If SerialPort.IsOpen = True Then
                        Dim intResponse As Integer

                        intResponse = MsgBox("Configuration Saved. Do you want to reset the Meter?", vbYesNo + vbQuestion, "Meter Reset")



                        If intResponse = vbYes Then

                            SerialPort.WriteLine("Reboot")
                            MsgBox("Meter reset!", MessageBoxIcon.Information)
                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_MeasurementCalibrationWindow.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        Else

                            CalMode = False
                            TimerHandler1_Client.Start()

                            TimerMonitor.Start()
                            TimerMonitor.Enabled = True
                            TimerCommands.Enabled = False
                            TimerSync.Enabled = False
                            TimerSync.Stop()

                            TimerCommands.Stop()
                            FrmSMGE_3222M_MeasurementCalibrationWindow.Close()
                            FrmSMGE_3222M_CalibrationMainWindow.Close()
                            Me.Show()

                        End If

                    End If



                    FrmSMGE_3222M_MeasurementCalibrationWindow.CmdSave.Enabled = True

                End If


                '=======================================================================================

                TxtComTerm.Clear()
                Reply_SAVE = False

                If Ack = True Then
                    FrmSMGE_3222M_MeasurementCalibrationWindow.CmdSave.Enabled = True
                End If



                'Dim ConfigStr As String
                'ConfigStr = JsonConvert.SerializeObject(Calibration4, Newtonsoft.Json.Formatting.Indented)


                'MsgBox(ConfigStr)

                'File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration4.txt", JsonConvert.SerializeObject(Calibration4, Newtonsoft.Json.Formatting.Indented))



                'TxtComTerm.Clear()
                'Reply_SAVE = False

                'FrmSMGE_3222M_Configuration.CmdSave.Enabled = True
                'MsgBox("Calibration Settings Saved!", MessageBoxIcon.Information)




            Catch ex As Exception

            End Try

        End If


        '=======================================================================================================================================================================================
        'If CalMode = True Then

        '    Try


        '        Calibration = New Calibrations

        '        Dim JString As String = TxtComTerm.Text
        '        Dim SavedConfig = JsonConvert.DeserializeObject(Of Meter_calibrations)(JString)

        '        '//Configuration Registers
        '        Calibration.MMode0 = SavedConfig.MMode0
        '        Calibration.MMode1 = SavedConfig.MMode1
        '        Calibration.PMPGA = SavedConfig.PMPGA
        '        Calibration.PStartTh = SavedConfig.PStartTh
        '        Calibration.QStartTh = SavedConfig.QStartTh
        '        Calibration.SStartTh = SavedConfig.SStartTh
        '        Calibration.PPhaseTh = SavedConfig.PPhaseTh
        '        Calibration.QPhaseTh = SavedConfig.QPhaseTh
        '        Calibration.SPhaseTh = SavedConfig.SPhaseTh

        '        '//Common Calibration Registers
        '        Calibration.PoffsetA = SavedConfig.PoffsetA
        '        Calibration.QoffsetA = SavedConfig.QoffsetA
        '        Calibration.POffsetB = SavedConfig.POffsetB
        '        Calibration.QOffsetB = SavedConfig.QOffsetB
        '        Calibration.POffsetC = SavedConfig.POffsetC
        '        Calibration.QOffsetC = SavedConfig.QOffsetC
        '        Calibration.GainA = SavedConfig.GainA
        '        Calibration.PhiA = SavedConfig.PhiA
        '        Calibration.GainB = SavedConfig.GainB
        '        Calibration.PhiB = SavedConfig.PhiB
        '        Calibration.GainC = SavedConfig.GainC
        '        Calibration.PhiC = SavedConfig.PhiC

        '        '//Fundamental/ Harmonic Energy Calibration registers
        '        Calibration.POffsetAF = SavedConfig.POffsetAF
        '        Calibration.POffsetBF = SavedConfig.POffsetBF
        '        Calibration.POffsetCF = SavedConfig.POffsetCF
        '        Calibration.PGainAF = SavedConfig.PGainAF
        '        Calibration.PGainBF = SavedConfig.PGainBF
        '        Calibration.PGainCF = SavedConfig.PGainCF

        '        '//Measurement Calibration Registers
        '        Calibration.UgainA = SavedConfig.UgainA
        '        Calibration.IgainA = SavedConfig.IgainA
        '        Calibration.UoffsetA = SavedConfig.UoffsetA
        '        Calibration.IoffsetA = SavedConfig.IoffsetA
        '        Calibration.UgainB = SavedConfig.UgainB
        '        Calibration.IgainB = SavedConfig.IgainB
        '        Calibration.UoffsetB = SavedConfig.UoffsetB
        '        Calibration.IoffsetB = SavedConfig.IoffsetB
        '        Calibration.UgainC = SavedConfig.UgainC
        '        Calibration.IgainC = SavedConfig.IgainC
        '        Calibration.UoffsetC = SavedConfig.UoffsetC
        '        Calibration.IoffsetC = SavedConfig.IoffsetC
        '        Calibration.IgainN = SavedConfig.IgainN
        '        Calibration.IoffsetN = SavedConfig.IoffsetN




        '        Dim ConfigStr As String
        '        ConfigStr = JsonConvert.SerializeObject(Calibration, Newtonsoft.Json.Formatting.Indented)


        '        MsgBox(ConfigStr)

        '        File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewCalibration.txt", JsonConvert.SerializeObject(Calibration, Newtonsoft.Json.Formatting.Indented))



        '        TxtComTerm.Clear()
        '        Reply_SAVE = False

        '        If FrmSMGE_3222M_Configuration.CmdSave.Enabled = True Then
        '            MsgBox("Calibration Settings Saved!", MessageBoxIcon.Information)
        '        End If



        '    Catch ex As Exception

        '    End Try

        'End If

        '=======================================================================================================================================================================================


    End Sub

    Dim CP_1, CP_2, CP_3, CP_4, CP_5, CP_6, CP_7, CP_8, CP_9, CP_10 As String
    Dim DP_1, DP_2, DP_3, MD_1, MD_2, MD_3, MD_4 As String
    Dim TP_1, TP_2, TP_3, TP_4, TP_5, TP_6, TP_7, TP_8, TP_9, TP_10 As String
    Dim AE_1L, AE_2L, AE_3L, AE_4L, AE_5L, MD_1L, MD_2L, MD_3L, MD_4L As Integer
    Dim BE_1L, BE_2L, BE_3L, BE_4L, BE_5L As Integer
    Dim CE_1L, CE_2L, CE_3L, CE_4L, CE_5L As Integer
    Dim TE_1L, TE_2L, TE_3L, TE_4L, TE_5L, TE_6L As Integer
    Dim DP_1L, DP_2L, DP_3L As Integer
    Dim AP_1L, AP_2L, AP_3L, AP_4L, AP_5L, AP_6L, AP_7L, AP_8L, AP_9L, AP_10L As Integer
    Dim BP_1L, BP_2L, BP_3L, BP_4L, BP_5L, BP_6L, BP_7L, BP_8L, BP_9L, BP_10L As Integer
    Dim CP_1L, CP_2L, CP_3L, CP_4L, CP_5L, CP_6L, CP_7L, CP_8L, CP_9L, CP_10L As Integer
    Dim TP_1L, TP_2L, TP_3L, TP_4L, TP_5L, TP_6L, TP_7L, TP_8L, TP_9L, TP_10L As Integer
    Dim Limit As Integer = 20
    Dim FilePathAndName As String
    Dim Hum_ As Decimal
    Public RecordCheck As Boolean = False
    Dim SB As Boolean = True
    Dim ShowHB As Integer
    Public AE_1Log, AE_2Log, AE_3Log, AE_4Log, AE_5Log As String
    Public BE_1Log, BE_2Log, BE_3Log, BE_4Log, BE_5Log As String
    Public CE_1Log, CE_2Log, CE_3Log, CE_4Log, CE_5Log As String
    Public TE_1Log, TE_2Log, TE_3Log, TE_4Log, TE_5Log, TE_6Log As String
    Public AP_1Log, AP_2Log, AP_3Log, AP_4Log, AP_5Log, AP_6Log, AP_7Log, AP_8Log, AP_9Log, AP_10Log As String
    Public BP_1Log, BP_2Log, BP_3Log, BP_4Log, BP_5Log, BP_6Log, BP_7Log, BP_8Log, BP_9Log, BP_10Log As String
    Public CP_1Log, CP_2Log, CP_3Log, CP_4Log, CP_5Log, CP_6Log, CP_7Log, CP_8Log, CP_9Log, CP_10Log As String
    Public TP_1Log, TP_2Log, TP_3Log, TP_4Log, TP_5Log, TP_6Log, TP_7Log, TP_8Log, TP_9Log, TP_10Log, DP_1Log, DP_2Log, DP_3Log As String
    Public DT As DateTime = Now

    Private Sub CmdTCPSettings_Click(sender As Object, e As EventArgs) Handles CmdTCPSettings.Click
        'Me.Hide()
        FrmSMGE_3222MSettings.Show()
        TimerHandler1_Client.Stop()

        TimerSerial.Stop()
        TimerMonitor.Stop()
        TimerMonitor.Enabled = False

        '==================================
        TimerSync.Enabled = True
        TimerSync.Start()
        '==================================
        SetMode = True
        TxtComTerm.Clear()
        'TimerMonitor.Start()
        FrmSMGE_3222MSettings.TimerHandler1_Client.Start()

        If OptTCPIP.Checked = True Then
            FrmSMGE_3222MSettings.GrpModbusSetting.Enabled = False

        End If


    End Sub

    Private Sub CmdTCPConnect_Click(sender As Object, e As EventArgs) Handles CmdTCPConnect.Click
        CmdTCPConnect.Enabled = False
        CmdTCPSettings.Enabled = True
        CmdTCPDisconnect.Enabled = True
        StartServer()
        TCP.StartServer()

    End Sub


    Private Sub CmdTCPDisconnect_Click(sender As Object, e As EventArgs) Handles CmdTCPDisconnect.Click
        CmdTCPConnect.Enabled = True
        CmdTCPSettings.Enabled = True
        CmdTCPDisconnect.Enabled = False
        StopServer()
        TCP.StopServer()
        TimerHandler1_Client.Enabled = False
        TimerHandler1_Client.Stop()
        'TimerMonitor.Stop()
    End Sub


    Private Sub TimerRefreshForm_Tick(sender As Object, e As EventArgs) Handles TimerRefreshForm.Tick

        If Con_state = True Then
            PictureBoxConnectionIndTCP.Visible = True
            Con_state = False

        Else
            PictureBoxConnectionIndTCP.Visible = False
            Con_state = True

        End If



    End Sub

    Sub Refresh_Form()
        TCP.TBox.Clear()
        TxtComTerm.Clear()

    End Sub

    Function StartServer()
        If ServerStatus = False Then
            ServerTrying = True
            Try
                Server = New TcpListener(IPAddress.Parse(TxtServerIP.Text), TxtTCPPort.Text)
                Server.Start()
                ServerStatus = True
                TimerHandler1_Client.Enabled = True
                TimerHandler1_Client.Start()
                'TimerHandler2_Client.Enabled = True
                'TimerHandler2_Client.Start()
                ProgressBarDataRead.Value = 0

                CmdCallibrate.Enabled = True

                TimerRefreshForm.Start()
                LblEthernetStatus.Text = "Status : Connected"
                PictureBoxConnectionIndTCP.Image = My.Resources.Green
                Threading.ThreadPool.QueueUserWorkItem(AddressOf Handler_Client)
            Catch ex As Exception
                ServerStatus = False
            End Try
            ServerTrying = False
        End If
        Return True
    End Function

    Function StopServer()

        If ServerStatus = True Then
            ServerTrying = True
            Try
                For Each Client As TcpClient In Clients
                    Client.Close()
                Next
                Server.Stop()
                LblEthernetStatus.Text = "Status : Disconnect"
                PictureBoxConnectionIndTCP.Image = My.Resources.Red
                TimerRefreshForm.Stop()
                ServerStatus = False
                TimerHandler1_Client.Enabled = False
                TimerHandler1_Client.Stop()
                'TimerHandler2_Client.Enabled = False
                'TimerHandler2_Client.Stop()

                CmdCallibrate.Enabled = False

            Catch ex As Exception
                StopServer()
            End Try
            ServerTrying = False
        End If
        Return True

        Return True
    End Function

    Function Handler_Client(ByVal state As Object)
        Dim TempClient As TcpClient = Nothing


        Try
            Using Client As TcpClient = Server.AcceptTcpClient
                ''     Console.Beep()
                ConState = True

                If ServerTrying = False Then
                    Threading.ThreadPool.QueueUserWorkItem(AddressOf Handler_Client)
                End If

                Clients.Add(Client)
                TempClient = Client

                Dim TX As New StreamWriter(Client.GetStream)
                Dim RX As New StreamReader(Client.GetStream)
                Try
                    If RX.BaseStream.CanRead = True Then


                        While RX.BaseStream.CanRead = True


                            Dim RawData As String = RX.ReadLine
                            If Client.Client.Connected = True AndAlso Client.Connected = True AndAlso Client.GetStream.CanRead = True Then

                                PictureBoxConnectionIndTCP.Image = My.Resources.Green
                                REM For some reason this seems to stop the comon tcp connection bug vvv
                                Dim RawDataLength As String = RawData.Length.ToString
                                REM ^^^^ Comment it out and test it in your own projects. Mine might be the only stupid one.



                                If SetMode = False And CalMode = False Then
                                    TxtComTerm.Text += RawData + vbNewLine
                                    TBox.Text += RawData + vbNewLine
                                    StatusLblClientIP.Text = "Client IP : " + Client.Client.RemoteEndPoint.ToString
                                    ProgressBarDataRead.Value = 10
                                End If




                                If SetMode = True Then
                                    TxtComTerm.Text += RawData + vbNewLine
                                    TBox.Text += RawData + vbNewLine
                                    'Reply_SAVE = True
                                End If

                                If CalMode = True Then
                                    TxtComTerm.Text += RawData + vbNewLine
                                    TBox.Text += RawData + vbNewLine
                                End If





                            Else Exit While
                            End If
                        End While


                    End If
                Catch ex As Exception
                    If Clients.Contains(Client) Then
                        Clients.Remove(Client)
                        Client.Close()

                    End If



                    RX.Close()
                End Try


                ''   If RX.BaseStream.CanRead = False Then
                ''   Client.Close()
                ''   Clients.Remove(Client)
                ''   End If
                ''   Console.Beep()
            End Using
            If Clients.Contains(TempClient) Then
                Clients.Remove(TempClient)
                TempClient.Close()
            End If
        Catch ex As Exception
            If Clients.Contains(TempClient) Then
                Clients.Remove(TempClient)
                TempClient.Close()

            End If
            ConState = False
        End Try

        Return True
    End Function

    Function SendToClients(ByVal Data As String)
        If ServerStatus = True Then
            If Clients.Count > 0 Then
                Try
                    REM  Broadcast data to all clients
                    REM To target one client,
                    REM USAGE: If client.client.remoteendpoint.tostring.contains(IP As String) Then
                    REM I am sorry for the lack of preparation for this project and in the video.
                    REM I wrote 99% of this from the top of my head,  no one is perfect, bound to make mistakes.
                    For Each Client As TcpClient In Clients
                        Dim TX1 As New StreamWriter(Client.GetStream)
                        ''   Dim RX1 As New StreamReader(Client.GetStream)
                        TX1.WriteLine(Data)
                        TX1.Flush()
                    Next
                Catch ex As Exception
                    SendToClients(Data)
                End Try
            End If
        End If
        Return True
    End Function


    Private Sub TimerHandler1_Client_Tick(sender As Object, e As EventArgs) Handles TimerHandler1_Client.Tick


        Dim StrTerminal As String

        StrTerminal = TBox.Text

        If StrTerminal.Contains("/SYNC/") Then
            Ack = True

            TBox.Clear()
            TxtComTerm.Clear()
            Threading.ThreadPool.QueueUserWorkItem(AddressOf SendToClients, "Code_NewReq" + vbLf)

        Else
            Ack = False
        End If
        Try

            ProgressBarDataRead.Value = 25
            StrSerialInRam = TBox.Lines(0).Substring(0, 3)
            If StrSerialInRam = "AE1" Then
                AE_1 = TBox.Lines(0)
                AE_1L = AE_1.Length
            Else
                AE_1 = AE_1
            End If

            StrSerialInRam = TBox.Lines(1).Substring(0, 3)
            If StrSerialInRam = "AE2" Then
                AE_2 = TBox.Lines(1)
                AE_2L = AE_2.Length
            Else
                AE_2 = AE_2
            End If

            StrSerialInRam = TBox.Lines(2).Substring(0, 3)
            If StrSerialInRam = "AE3" Then
                AE_3 = TBox.Lines(2)
                AE_3L = AE_3.Length
            Else
                AE_3 = AE_3
            End If
            StrSerialInRam = TBox.Lines(3).Substring(0, 3)
            If StrSerialInRam = "AE4" Then
                AE_4 = TBox.Lines(3)
                AE_4L = AE_4.Length
            Else
                AE_4 = AE_4
            End If
            StrSerialInRam = TBox.Lines(4).Substring(0, 3)
            If StrSerialInRam = "AE5" Then
                AE_5 = TBox.Lines(4)
                AE_5L = AE_5.Length
            Else
                AE_5 = AE_5
            End If

            ProgressBarDataRead.Value = 40
            '==================================================================================
            StrSerialInRam = TBox.Lines(5).Substring(0, 3)
            If StrSerialInRam = "BE1" Then
                BE_1 = TBox.Lines(5)
                BE_1L = BE_1.Length
            Else
                BE_1 = BE_1
            End If
            StrSerialInRam = TBox.Lines(6).Substring(0, 3)
            If StrSerialInRam = "BE2" Then
                BE_2 = TBox.Lines(6)
                BE_2L = BE_2.Length
            Else
                BE_2 = BE_2
            End If
            StrSerialInRam = TBox.Lines(7).Substring(0, 3)
            If StrSerialInRam = "BE3" Then
                BE_3 = TBox.Lines(7)
                BE_3L = BE_3.Length
            Else
                BE_3 = BE_3
            End If

            StrSerialInRam = TBox.Lines(8).Substring(0, 3)
            If StrSerialInRam = "BE4" Then
                BE_4 = TBox.Lines(8)
                BE_4L = BE_4.Length
            Else
                BE_4 = BE_4
            End If

            StrSerialInRam = TBox.Lines(9).Substring(0, 3)
            If StrSerialInRam = "BE5" Then
                BE_5 = TBox.Lines(9)
                BE_5L = BE_5.Length
            Else
                BE_5 = BE_5
            End If
            ProgressBarDataRead.Value = 50
            '==================================================================================
            StrSerialInRam = TBox.Lines(10).Substring(0, 3)
            If StrSerialInRam = "CE1" Then
                CE_1 = TBox.Lines(10)
                CE_1L = CE_1.Length
            Else
                CE_1 = CE_1
            End If
            StrSerialInRam = TBox.Lines(11).Substring(0, 3)
            If StrSerialInRam = "CE2" Then
                CE_2 = TBox.Lines(11)
                CE_2L = CE_2.Length
            Else
                CE_2 = CE_2
            End If
            StrSerialInRam = TBox.Lines(12).Substring(0, 3)
            If StrSerialInRam = "CE3" Then
                CE_3 = TBox.Lines(12)
                CE_3L = CE_3.Length
            Else
                CE_3 = CE_3
            End If

            StrSerialInRam = TBox.Lines(13).Substring(0, 3)
            If StrSerialInRam = "CE4" Then
                CE_4 = TBox.Lines(13)
                CE_4L = CE_4.Length
            Else
                CE_4 = CE_4
            End If
            StrSerialInRam = TBox.Lines(14).Substring(0, 3)
            If StrSerialInRam = "CE5" Then
                CE_5 = TBox.Lines(14)
                CE_5L = CE_5.Length
            Else
                CE_5 = CE_5
            End If

            ProgressBarDataRead.Value = 60
            '==================================================================================
            StrSerialInRam = TBox.Lines(15).Substring(0, 3)
            If StrSerialInRam = "TE1" Then
                TE_1 = TBox.Lines(15)
                TE_1L = TE_1.Length
            Else
                BE_1 = BE_1
            End If
            StrSerialInRam = TBox.Lines(16).Substring(0, 3)
            If StrSerialInRam = "TE2" Then
                TE_2 = TBox.Lines(16)
                TE_2L = TE_2.Length
            Else
                TE_2 = TE_2
            End If

            StrSerialInRam = TBox.Lines(17).Substring(0, 3)
            If StrSerialInRam = "TE3" Then
                TE_3 = TBox.Lines(17)
                TE_3L = TE_3.Length
            Else
                TE_3 = TE_3
            End If

            StrSerialInRam = TBox.Lines(18).Substring(0, 3)
            If StrSerialInRam = "TE4" Then
                TE_4 = TBox.Lines(18)
                TE_4L = TE_4.Length
            Else
                TE_4 = TE_4
            End If
            StrSerialInRam = TBox.Lines(19).Substring(0, 3)
            If StrSerialInRam = "TE5" Then
                TE_5 = TBox.Lines(19)
                TE_5L = TE_5.Length
            Else
                TE_5 = TE_5
            End If
            StrSerialInRam = TBox.Lines(20).Substring(0, 3)
            If StrSerialInRam = "TE6" Then
                TE_6 = TBox.Lines(20)
                TE_6L = TE_6.Length
            Else
                TE_6 = TE_6
            End If
            ProgressBarDataRead.Value = 70
            StrSerialInRam = TBox.Lines(21).Substring(0, 3)
            If StrSerialInRam = "AP1" Then
                AP_1 = TBox.Lines(21)
                AP_1L = AP_1.Length
            Else
                AP_1 = AP_1
            End If
            StrSerialInRam = TBox.Lines(22).Substring(0, 3)
            If StrSerialInRam = "AP2" Then
                AP_2 = TBox.Lines(22)
                AP_2L = AP_2.Length
            Else
                AP_2 = AP_2
            End If

            StrSerialInRam = TBox.Lines(23).Substring(0, 3)
            If StrSerialInRam = "AP3" Then
                AP_3 = TBox.Lines(23)
                AP_3L = AP_3.Length
            Else
                AP_3 = AP_3
            End If

            StrSerialInRam = TBox.Lines(24).Substring(0, 3)
            If StrSerialInRam = "AP4" Then
                AP_4 = TBox.Lines(24)
                AP_4L = AP_4.Length
            Else
                AP_4 = AP_4
            End If
            StrSerialInRam = TBox.Lines(25).Substring(0, 3)
            If StrSerialInRam = "AP5" Then
                AP_5 = TBox.Lines(25)
                AP_5L = AP_5.Length
            Else
                AP_5 = AP_5
            End If
            StrSerialInRam = TBox.Lines(26).Substring(0, 3)
            If StrSerialInRam = "AP6" Then
                AP_6 = TBox.Lines(26)
                AP_6L = AP_6.Length
            Else
                AP_6 = AP_6
            End If

            StrSerialInRam = TBox.Lines(27).Substring(0, 3)
            If StrSerialInRam = "AP7" Then
                AP_7 = TBox.Lines(27)
                AP_7L = AP_7.Length
            Else
                AP_7 = AP_7
            End If

            StrSerialInRam = TBox.Lines(28).Substring(0, 3)
            If StrSerialInRam = "AP8" Then
                AP_8 = TBox.Lines(28)
                AP_8L = AP_8.Length
            Else
                AP_8 = AP_8
            End If
            StrSerialInRam = TBox.Lines(29).Substring(0, 3)
            If StrSerialInRam = "AP9" Then
                AP_9 = TBox.Lines(29)
                AP_9L = AP_9.Length
            Else
                AP_9 = AP_9
            End If

            StrSerialInRam = TBox.Lines(30).Substring(0, 4)
            If StrSerialInRam = "AP10" Then
                AP_10 = TBox.Lines(30)
                AP_10L = AP_10.Length
            Else
                AP_10 = AP_10
            End If

            ProgressBarDataRead.Value = 75
            '==================================================================================
            StrSerialInRam = TBox.Lines(31).Substring(0, 3)
            If StrSerialInRam = "BP1" Then
                BP_1 = TBox.Lines(31)
                BP_1L = BP_1.Length
            Else
                BP_1 = BP_1
            End If

            StrSerialInRam = TBox.Lines(32).Substring(0, 3)
            If StrSerialInRam = "BP2" Then
                BP_2 = TBox.Lines(32)
                BP_2L = BP_2.Length
            Else
                BP_2 = BP_2
            End If
            StrSerialInRam = TBox.Lines(33).Substring(0, 3)
            If StrSerialInRam = "BP3" Then
                BP_3 = TBox.Lines(33)
                BP_3L = BP_3.Length
            Else
                BP_3 = BP_3
            End If
            StrSerialInRam = TBox.Lines(34).Substring(0, 3)
            If StrSerialInRam = "BP4" Then
                BP_4 = TBox.Lines(34)
                BP_4L = BP_4.Length
            Else
                BP_4 = BP_4
            End If

            StrSerialInRam = TBox.Lines(35).Substring(0, 3)
            If StrSerialInRam = "BP5" Then
                BP_5 = TBox.Lines(35)
                BP_5L = BP_5.Length
            Else
                BP_5 = BP_5
            End If


            StrSerialInRam = TBox.Lines(36).Substring(0, 3)
            If StrSerialInRam = "BP6" Then
                BP_6 = TBox.Lines(36)
                BP_6L = BP_6.Length
            Else
                BP_6 = BP_6
            End If
            StrSerialInRam = TBox.Lines(37).Substring(0, 3)
            If StrSerialInRam = "BP7" Then
                BP_7 = TBox.Lines(37)
                BP_7L = BP_7.Length
            Else
                BP_7 = BP_7
            End If
            StrSerialInRam = TBox.Lines(38).Substring(0, 3)
            If StrSerialInRam = "BP8" Then
                BP_8 = TBox.Lines(38)
                BP_8L = BP_8.Length
            Else
                BP_8 = BP_8
            End If

            StrSerialInRam = TBox.Lines(39).Substring(0, 3)
            If StrSerialInRam = "BP9" Then
                BP_9 = TBox.Lines(39)
                BP_9L = BP_9.Length
            Else
                BP_9 = BP_9
            End If

            StrSerialInRam = TBox.Lines(40).Substring(0, 4)
            If StrSerialInRam = "BP10" Then
                BP_10 = TBox.Lines(40)
                BP_10L = BP_10.Length
            Else
                BP_10 = BP_10
            End If
            ProgressBarDataRead.Value = 80
            '==================================================================================
            StrSerialInRam = TBox.Lines(41).Substring(0, 3)
            If StrSerialInRam = "CP1" Then
                CP_1 = TBox.Lines(41)
                CP_1L = CP_1.Length
            Else
                CP_1 = CP_1
            End If
            StrSerialInRam = TBox.Lines(42).Substring(0, 3)
            If StrSerialInRam = "CP2" Then
                CP_2 = TBox.Lines(42)
                CP_2L = CP_2.Length
            Else
                CP_2 = CP_2
            End If

            StrSerialInRam = TBox.Lines(43).Substring(0, 3)
            If StrSerialInRam = "CP3" Then
                CP_3 = TBox.Lines(43)
                CP_3L = CP_3.Length
            Else
                CP_3 = CP_3
            End If
            StrSerialInRam = TBox.Lines(44).Substring(0, 3)
            If StrSerialInRam = "CP4" Then
                CP_4 = TBox.Lines(44)
                CP_4L = CP_4.Length
            Else
                CP_4 = CP_4
            End If
            StrSerialInRam = TBox.Lines(45).Substring(0, 3)
            If StrSerialInRam = "CP5" Then
                CP_5 = TBox.Lines(45)
                CP_5L = CP_5.Length
            Else
                CP_5 = CP_5
            End If


            StrSerialInRam = TBox.Lines(46).Substring(0, 3)
            If StrSerialInRam = "CP6" Then
                CP_6 = TBox.Lines(46)
                CP_6L = CP_6.Length
            Else
                CP_6 = CP_6
            End If

            StrSerialInRam = TBox.Lines(47).Substring(0, 3)
            If StrSerialInRam = "CP7" Then
                CP_7 = TBox.Lines(47)
                CP_7L = CP_7.Length
            Else
                CP_7 = CP_7
            End If
            StrSerialInRam = TBox.Lines(48).Substring(0, 3)
            If StrSerialInRam = "CP8" Then
                CP_8 = TBox.Lines(48)
                CP_8L = CP_8.Length
            Else
                CP_8 = CP_8
            End If
            StrSerialInRam = TBox.Lines(49).Substring(0, 3)
            If StrSerialInRam = "CP9" Then
                CP_9 = TBox.Lines(49)
                CP_9L = CP_9.Length
            Else
                CP_9 = CP_9
            End If

            StrSerialInRam = TBox.Lines(50).Substring(0, 4)
            If StrSerialInRam = "CP10" Then
                CP_10 = TBox.Lines(50)
                CP_10L = CP_10.Length
            Else
                CP_10 = CP_10
            End If


            ProgressBarDataRead.Value = 90
            '==================================================================================
            StrSerialInRam = TBox.Lines(51).Substring(0, 3)
            If StrSerialInRam = "TP1" Then
                TP_1 = TBox.Lines(51)
                TP_1L = TP_1.Length
            Else
                TP_1 = TP_1
            End If

            StrSerialInRam = TBox.Lines(52).Substring(0, 3)
            If StrSerialInRam = "TP2" Then
                TP_2 = TBox.Lines(52)
                TP_2L = TP_2.Length
            Else
                TP_2 = TP_2
            End If
            StrSerialInRam = TBox.Lines(53).Substring(0, 3)
            If StrSerialInRam = "TP3" Then
                TP_3 = TBox.Lines(53)
                TP_3L = TP_3.Length
            Else
                TP_3 = TP_3
            End If
            StrSerialInRam = TBox.Lines(54).Substring(0, 3)
            If StrSerialInRam = "TP4" Then
                TP_4 = TBox.Lines(54)
                TP_4L = TP_4.Length
            Else
                TP_4 = TP_4
            End If

            StrSerialInRam = TBox.Lines(55).Substring(0, 3)
            If StrSerialInRam = "TP5" Then
                TP_5 = TBox.Lines(55)
                TP_5L = TP_5.Length
            Else
                TP_5 = TP_5
            End If
            StrSerialInRam = TBox.Lines(56).Substring(0, 3)
            If StrSerialInRam = "TP6" Then
                TP_6 = TBox.Lines(56)
                TP_6L = TP_6.Length
            Else
                TP_6 = TP_6
            End If

            StrSerialInRam = TBox.Lines(57).Substring(0, 3)
            If StrSerialInRam = "TP7" Then
                TP_7 = TBox.Lines(57)
                TP_7L = TP_7.Length
            Else
                TP_7 = TP_7
            End If
            StrSerialInRam = TBox.Lines(58).Substring(0, 3)
            If StrSerialInRam = "TP8" Then
                TP_8 = TBox.Lines(58)
                TP_8L = TP_8.Length
            Else
                TP_8 = TP_8
            End If
            StrSerialInRam = TBox.Lines(59).Substring(0, 3)
            If StrSerialInRam = "TP9" Then
                TP_9 = TBox.Lines(59)
                TP_9L = TP_9.Length
            Else
                TP_9 = TP_9
            End If

            StrSerialInRam = TBox.Lines(60).Substring(0, 4)
            If StrSerialInRam = "TP10" Then
                TP_10 = TBox.Lines(60)
                TP_10L = TP_10.Length
            Else
                TP_10 = TP_10
            End If

            StrSerialInRam = TBox.Lines(61).Substring(0, 3)
            If StrSerialInRam = "DP1" Then
                DP_1 = TBox.Lines(61)
                DP_1L = DP_1.Length
            Else
                DP_1 = DP_1
            End If
            StrSerialInRam = TBox.Lines(62).Substring(0, 3)
            If StrSerialInRam = "DP2" Then
                DP_2 = TBox.Lines(62)
                DP_2L = DP_2.Length
            Else
                DP_2 = DP_2
            End If
            StrSerialInRam = TBox.Lines(63).Substring(0, 3)
            If StrSerialInRam = "DP3" Then
                DP_3 = TBox.Lines(63)
                DP_3L = DP_3.Length
            Else
                DP_3 = DP_3
            End If

            StrSerialInRam = TBox.Lines(64).Substring(0, 3)
            If StrSerialInRam = "MD1" Then
                MD_1 = TBox.Lines(64)
                MD_1L = MD_1.Length
            Else
                MD_1 = MD_1
            End If

            StrSerialInRam = TBox.Lines(65).Substring(0, 3)
            If StrSerialInRam = "MD2" Then
                MD_2 = TBox.Lines(65)
                MD_2L = MD_2.Length
            Else
                MD_2 = MD_2
            End If
            StrSerialInRam = TBox.Lines(66).Substring(0, 3)
            If StrSerialInRam = "MD3" Then
                MD_3 = TBox.Lines(66)
                MD_3L = MD_3.Length
            Else
                MD_3 = MD_3
            End If
            StrSerialInRam = TBox.Lines(67).Substring(0, 3)
            If StrSerialInRam = "MD4" Then
                MD_4 = TBox.Lines(67)
                MD_4L = MD_4.Length
            Else
                MD_4 = MD_4
            End If




            'Voltages
            AP_7Log = Mid(AP_7, 4, AP_7L)
            BP_7Log = Mid(BP_7, 4, BP_7L)
            CP_7Log = Mid(CP_7, 4, CP_7L)

            'Currents
            AP_8Log = Mid(AP_8, 4, AP_8L)
            BP_8Log = Mid(BP_8, 4, BP_8L)
            CP_8Log = Mid(CP_8, 4, CP_8L)
            TP_9Log = Mid(TP_9, 4, TP_9L)
            TP_10Log = Mid(TP_10, 5, TP_10L)

            'Active Power
            AP_1Log = Mid(AP_1, 4, AP_1L)
            BP_1Log = Mid(BP_1, 4, BP_1L)
            CP_1Log = Mid(CP_1, 4, CP_1L)
            TP_1Log = Mid(TP_1, 4, TP_1L)

            'Reactive Power
            AP_2Log = Mid(AP_2, 4, AP_2L)
            BP_2Log = Mid(BP_2, 4, BP_2L)
            CP_2Log = Mid(CP_2, 4, CP_2L)
            TP_2Log = Mid(TP_2, 4, TP_2L)

            'Apperant Power
            AP_3Log = Mid(AP_3, 4, AP_3L)
            BP_3Log = Mid(BP_3, 4, BP_3L)
            CP_3Log = Mid(CP_3, 4, CP_3L)
            TP_3Log = Mid(TP_3, 4, TP_3L)

            'Power Factor
            AP_5Log = Mid(AP_5, 4, AP_5L)
            BP_5Log = Mid(BP_5, 4, BP_5L)
            CP_5Log = Mid(CP_5, 4, CP_5L)
            TP_5Log = Mid(TP_5, 4, TP_5L)

            'Phase Angle
            AP_9Log = Mid(AP_9, 4, AP_9L)
            BP_9Log = Mid(BP_9, 4, BP_9L)
            CP_9Log = Mid(CP_9, 4, CP_9L)

            'Frequency
            TP_8Log = Mid(TP_8, 4, TP_8L)

            'Active Energy Imp
            AE_1Log = Mid(AE_1, 4, AE_1L)
            BE_1Log = Mid(BE_1, 4, BE_1L)
            CE_1Log = Mid(CE_1, 4, CE_1L)
            TE_1Log = Mid(TE_1, 4, TE_1L)

            'Active Energy Exp
            AE_2Log = Mid(AE_2, 4, AE_2L)
            BE_2Log = Mid(BE_2, 4, BE_2L)
            CE_2Log = Mid(CE_2, 4, CE_2L)
            TE_2Log = Mid(TE_2, 4, TE_2L)


            'Reactive Energy Imp
            AE_3Log = Mid(AP_3, 4, AP_3L)
            BE_3Log = Mid(BP_3, 4, BP_3L)
            CE_3Log = Mid(CP_3, 4, CP_3L)
            TE_3Log = Mid(TE_3, 4, TE_3L)

            'Reactive Energy Exp
            AE_4Log = Mid(AE_4, 4, AE_4L)
            BE_4Log = Mid(BE_4, 4, BE_4L)
            CE_4Log = Mid(CE_4, 4, CE_4L)
            TE_4Log = Mid(TE_4, 4, TE_4L)


            'Apperent Energy
            AE_5Log = Mid(AE_5, 4, AE_5L)
            BE_5Log = Mid(BE_5, 4, BE_5L)
            CE_5Log = Mid(CE_5, 4, CE_5L)
            TE_5Log = Mid(TE_5, 4, TE_5L)
            TE_6Log = Mid(TE_6, 4, TE_6L)


            'Temperature and Humidity
            DP_1Log = Mid(DP_1, 4, DP_1L)
            DP_2Log = Mid(DP_2, 4, DP_2L)
            DP_3Log = Mid(DP_3, 4, DP_3L)
            ''==================================================================================================================


            FrmSM_GE3222M_Energy_Data.LblPhaA_ForwardActiveEnergyVal.Text = Mid(AE_1, 4, AE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ReverseActiveEnergyVal.Text = Mid(AE_2, 4, AE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ForwardReactiveEnergyVal.Text = Mid(AE_3, 4, AE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ReverseReactiveEnergyVal.Text = Mid(AE_4, 4, AE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ApparentEnergyVal.Text = Mid(AE_5, 4, AE_5L) & " KVAh"

            FrmSM_GE3222M_Energy_Data.LblPhaB_ForwardActiveEnergyVal.Text = Mid(BE_1, 4, BE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ReverseActiveEnergyVal.Text = Mid(BE_2, 4, BE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ForwardReactiveEnergyVal.Text = Mid(BE_3, 4, BE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ReverseReactiveEnergyVal.Text = Mid(BE_4, 4, BE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ApparentEnergyVal.Text = Mid(BE_5, 4, BE_5L) & " KVAh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ForwardActiveEnergyVal.Text = Mid(CE_1, 4, CE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ReverseActiveEnergyVal.Text = Mid(CE_2, 4, CE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ForwardReactiveEnergyVal.Text = Mid(CE_3, 4, CE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ReverseReactiveEnergyVal.Text = Mid(CE_4, 4, CE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ApparentEnergyVal.Text = Mid(CE_5, 4, CE_5L) & " KVAh"
            FrmSM_GE3222M_Energy_Data.LblTotalForwardActiveEnergyVal.Text = Mid(TE_1, 4, TE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblTotalReverseActiveEnergyVal.Text = Mid(TE_2, 4, TE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblTotalForwardReactiveEnergyVal.Text = Mid(TE_3, 4, TE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblTotalReverseReactiveEnergyVal.Text = Mid(TE_4, 4, TE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblTotal_ArithmeticSum_ApparentEnergyVal.Text = Mid(TE_5, 4, TE_5L) & " KVAh"
            FrmSM_GE3222M_Energy_Data.LblVectorSum_TotalApparentEnergyVal.Text = Mid(TE_6, 4, TE_6L) & " KVAh"
            FrmSMGE_3222M_BoardData.LblHumidityVal.Text = Mid(DP_1, 4, DP_1L)
            FrmSMGE_3222M_BoardData.LblTempCVal.Text = Mid(DP_2, 4, DP_2L)
            FrmSMGE_3222M_VoltageData.LblV1Val.Text = Mid(AP_7, 4, AP_7L)
            FrmSMGE_3222M_VoltageData.LblV2Val.Text = Mid(BP_7, 4, BP_7L)
            FrmSMGE_3222M_VoltageData.LblV3Val.Text = Mid(CP_7, 4, CP_7L)



            FrmVoltage.TxtVoltage.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "=============================="
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & ""
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 1 Voltage :   " & Mid(AP_7, 4, AP_7L) & "V"
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 2 Voltage :   " & Mid(BP_7, 4, BP_7L) & "V"
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 3 Voltage :   " & Mid(CP_7, 4, CP_7L) & "V"
            'FrmVoltage.TxtVoltage.Focus()
            'FrmVoltage.TxtVoltage.SelectionStart = FrmVoltage.TxtVoltage.Text.Length


            FrmSMGE_3222M_CurrentData.LblI1Val.Text = Mid(AP_8, 4, AP_8L)
            FrmSMGE_3222M_CurrentData.LblI2Val.Text = Mid(BP_8, 4, BP_8L)
            FrmSMGE_3222M_CurrentData.LblI3Val.Text = Mid(CP_8, 4, CP_8L)
            FrmSMGE_3222M_CurrentData.LB1.Text = Mid(TP_9, 4, TP_9L)
            FrmSMGE_3222M_CurrentData.LB2.Text = Mid(TP_10, 5, TP_10L)
            FrmCurrent.TxtCurrent.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "=============================="
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & ""
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 1 Current      :   " & Mid(AP_8, 4, AP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 2 Current      :   " & Mid(BP_8, 4, BP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 3 Current      :   " & Mid(CP_8, 4, CP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Neutral Current    :   " & Mid(TP_9, 4, TP_9L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Total Current      :   " & Mid(TP_10, 5, TP_10L) & "A"

            FrmSMGE_3222M_ActPowerData.TxtPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 1 Power      :   " & Mid(AP_1, 4, AP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 2 Power      :   " & Mid(BP_1, 4, BP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 3 Power      :   " & Mid(CP_1, 4, CP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Total Power      :   " & Mid(TP_1, 4, TP_1L) & "W"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 1 Reactive Power :   " & Mid(AP_2, 4, AP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 2 Reactive Power :   " & Mid(BP_2, 4, BP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 3 Reactive Power :   " & Mid(CP_2, 4, CP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Total Reactive Power :   " & Mid(TP_2, 4, TP_2L) & "VAR"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 1 Apparent Power :   " & Mid(AP_3, 4, AP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 2 Apparent Power :   " & Mid(BP_3, 4, BP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 3 Apparent Power :   " & Mid(CP_3, 4, CP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Total Apparent Power :   " & Mid(TP_3, 4, TP_3L) & "VA"
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 1 Power Factor :   " & Mid(AP_5, 4, AP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 2 Power Factor :   " & Mid(BP_5, 4, BP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 3 Power Factor :   " & Mid(CP_5, 4, CP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Total Power Factor :   " & Mid(TP_5, 4, TP_5L)
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 1 Angle       :   " & Mid(AP_9, 4, AP_9L) & "°"
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 2 Angle       :   " & Mid(BP_9, 4, BP_9L) & "°"
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 3 Angler      :   " & Mid(CP_9, 4, CP_9L) & "°"
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & "Frequency   :   " & Mid(TP_8, 4, TP_8L) & "Hz"
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Energy Imported"
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 1 Active Energy :   " & Mid(AE_1, 4, AE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 2 Active Energy :   " & Mid(BE_1, 4, BE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 3 Active Energy :   " & Mid(CE_1, 4, CE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Total Power Factor :   " & Mid(TE_1, 4, TE_1L)
            FrmSM_GE3222M_Power_Data.LblPhaA_ActivePowerVal.Text = Mid(AP_1, 4, AP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_ReactivePowerVal.Text = Mid(AP_2, 4, AP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaA_apparentpowerVal.Text = Mid(AP_3, 4, AP_3L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblPhaA_activefundamentalpowerVal.Text = Mid(AP_4, 4, AP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_powerfactorVal.Text = Mid(AP_5, 4, AP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaA_activeharmonicpowerVal.Text = Mid(AP_6, 4, AP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_voltageRMSVal.Text = Mid(AP_7, 4, AP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaA_currentRMSVal.Text = Mid(AP_8, 4, AP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaA_meanphaseangleVal.Text = Mid(AP_9, 4, AP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaA_voltagephaseangleVal.Text = Mid(AP_10, 5, AP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblPhaB_ActivePowerVal.Text = Mid(BP_1, 4, BP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_ReactivePowerVal.Text = Mid(BP_2, 4, BP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaB_apparentpowerVal.Text = Mid(BP_3, 4, BP_3L) & " VA"
            FrmSM_GE3222M_Power_Data.LblPhaB_activefundamentalpowerVal.Text = Mid(BP_4, 4, BP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_powerfactorVal.Text = Mid(BP_5, 4, BP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaB_activeharmonicpowerVal.Text = Mid(BP_6, 4, BP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_voltageRMSVal.Text = Mid(BP_7, 4, BP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaB_currentRMSVal.Text = Mid(BP_8, 4, BP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaB_meanphaseangleVal.Text = Mid(BP_9, 4, BP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaB_voltagephaseangleVal.Text = Mid(BP_10, 5, BP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblPhaC_ActivePowerVal.Text = Mid(CP_1, 4, CP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_ReactivePowerVal.Text = Mid(CP_2, 4, CP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaC_apparentpowerVal.Text = Mid(CP_3, 4, CP_3L) & " VA"
            FrmSM_GE3222M_Power_Data.LblPhaC_activefundamentalpowerVal.Text = Mid(CP_4, 4, CP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_powerfactorVal.Text = Mid(CP_5, 4, CP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaC_activeharmonicpowerVal.Text = Mid(CP_6, 4, CP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_voltageRMSVal.Text = Mid(CP_7, 4, CP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaC_currentRMSVal.Text = Mid(CP_8, 4, CP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaC_meanphaseangleVal.Text = Mid(CP_9, 4, CP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaC_voltagephaseangleVal.Text = Mid(CP_10, 5, CP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblTotal_PhasesumActivePowerVal.Text = Mid(TP_1, 4, TP_1L) & " kW"
            FrmSM_GE3222M_Power_Data.LblTotal_PhasesumReactivePowerVal.Text = Mid(TP_2, 4, TP_2L) & " kvar"
            FrmSM_GE3222M_Power_Data.LblTotal_ArithmeticSumapparentpowerVal.Text = Mid(TP_3, 4, TP_3L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblVectorSum_TotalApparentPowerVal.Text = Mid(TP_4, 4, TP_4L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblTotalpowerfactorVal.Text = Mid(TP_5, 4, TP_5L)
            FrmSM_GE3222M_Power_Data.LblTotalactivefundamentalpowerVal.Text = Mid(TP_6, 4, TP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblTotalactiveharmonicpowerVal.Text = Mid(TP_7, 4, TP_7L) & " W"
            FrmSM_GE3222M_Power_Data.LblLblFrequency.Text = Mid(TP_8, 4, TP_8L) & " Hz"
            FrmSM_GE3222M_Power_Data.LblN_Line_SampledcurrentRMSVal.Text = Mid(TP_9, 4, TP_9L) & " A"
            FrmSM_GE3222M_Power_Data.LblTotalLine_currentRMSVal.Text = Mid(TP_10, 5, TP_10L) & " A"


            TxtSlaveID.Text = Mid(MD_1, 4, MD_1L)
            TxtPortModbus.Text = Mid(MD_2, 4, MD_2L)
            TxtIPModbus.Text = Mid(MD_3, 4, MD_3L)
            ComboBoxModbus_BaudRate.SelectedItem = Mid(MD_4, 4, MD_4L)
            Refresh_Form()
            ProgressBarDataRead.Value = 100
        Catch ex As Exception
        End Try
    End Sub


    'Public Function SendToClients(ByVal Data As String)
    '    If ServerStatus = True Then
    '        If Clients.Count > 0 Then
    '            Try
    '                REM  Broadcast data to all clients
    '                REM To target one client,
    '                REM USAGE: If client.client.remoteendpoint.tostring.contains(IP As String) Then
    '                REM I am sorry for the lack of preparation for this project and in the video.
    '                REM I wrote 99% of this from the top of my head,  no one is perfect, bound to make mistakes.
    '                For Each Client As TcpClient In Clients
    '                    Dim TX1 As New StreamWriter(Client.GetStream)
    '                    ''   Dim RX1 As New StreamReader(Client.GetStream)
    '                    TX1.WriteLine(Data)
    '                    TX1.Flush()
    '                Next
    '            Catch ex As Exception
    '                SendToClients(Data)
    '            End Try
    '        End If
    '    End If
    '    Return True
    'End Function

    Private Sub ChkModbus_CheckedChanged(sender As Object, e As EventArgs) Handles ChkTCPModbus.CheckedChanged
        If ChkTCPModbus.Checked = True Then
            GrpConnection.Enabled = True
            GrpEthernet.Enabled = True
            PnlModbus.Enabled = True
            PnlEthernet.Enabled = False
            FrmSMGE_3222MSettings.ChkModbusMode.Checked = True
            FrmSMGE_3222MSettings.ChkModbusMode.Text = "ON"
            FrmSMGE_3222MSettings.GrpModbusSetting.Enabled = True

        Else
            PnlModbus.Enabled = False
            PnlEthernet.Enabled = True
            FrmSMGE_3222MSettings.ChkModbusMode.Checked = False
            FrmSMGE_3222MSettings.ChkModbusMode.Text = "OFF"
            FrmSMGE_3222MSettings.GrpModbusSetting.Enabled = False
        End If
    End Sub


    Private Sub CmdDataLogger_Click(sender As Object, e As EventArgs) Handles CmdDataLogger.Click
        Me.Hide()
        FrmSMGE_3222MDataLogger.Show()
    End Sub
    Private Sub TimerChartRecord_Tick(sender As Object, e As EventArgs) Handles TimerChartRecord.Tick
        FrmSMGE_3222M_BoardData.ChartHumTemp.Series("Humidity").Points.AddXY(DateTime.Now.ToLongTimeString, DP_1Log)
        If FrmSMGE_3222M_BoardData.ChartHumTemp.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_BoardData.ChartHumTemp.Series(0).Points.RemoveAt(0)
        End If


        FrmSMGE_3222M_BoardData.ChartHumTemp.Series("Temp_C").Points.AddXY(DateTime.Now.ToLongTimeString, DP_2Log)
        If FrmSMGE_3222M_BoardData.ChartHumTemp.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_BoardData.ChartHumTemp.Series(1).Points.RemoveAt(0)
        End If
        Dim RecordCheck1 As Boolean = FrmSMGE_3222M_BoardData.SR
        If RecordCheck1 = True Then
            stopwatch1.Start()
            Dim elapsed As TimeSpan = stopwatch1.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_BoardData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_BoardData.DataGridViewHumidity.Rows.Add(_min & ":" & _sec, DP_1Log, DP_2Log)
            FrmSMGE_3222M_BoardData.DataGridViewHumidity.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_BoardData.DataGridViewHumidity.RowCount - 1
            FrmSMGE_3222M_BoardData.DataGridViewHumidity.ClearSelection()
            FrmSMGE_3222M_BoardData.DataGridViewHumidity.Rows(FrmSMGE_3222M_BoardData.DataGridViewHumidity.RowCount - 1).Selected = True
            If FrmSMGE_3222M_BoardData.ChartHumidityRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_BoardData.ChartHumidityRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_BoardData.ChartHumidityRec.Series("Series1").Points.AddXY(_min & ":" & _sec, DP_1Log)
            If FrmSMGE_3222M_BoardData.ChartHumidityRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_BoardData.ChartHumidityRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_BoardData.ChartHumidityRec.Series("Series2").Points.AddXY(_min & ":" & _sec, DP_2Log)
            If FrmSMGE_3222M_BoardData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_BoardData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_BoardData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_BoardData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch1.Reset()
            stopwatch1.Stop()
        End If


        '=======================================================================================================================================

        FrmSMGE_3222M_VoltageData.ChartVoltage.Series("V1").Points.AddXY(DateTime.Now.ToLongTimeString, AP_7Log)
        If FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Points.RemoveAt(0)
        End If
        FrmSMGE_3222M_VoltageData.ChartVoltage.Series("V2").Points.AddXY(DateTime.Now.ToLongTimeString, BP_7Log)
        If FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Points.RemoveAt(0)
        End If
        FrmSMGE_3222M_VoltageData.ChartVoltage.Series("V3").Points.AddXY(DateTime.Now.ToLongTimeString, CP_7Log)
        If FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Points.RemoveAt(0)
        End If

        Dim RecordCheck2 As Boolean = FrmSMGE_3222M_VoltageData.SR
        If RecordCheck2 = True Then
            stopwatch2.Start()
            Dim elapsed As TimeSpan = stopwatch2.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_VoltageData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Rows.Add(_min & ":" & _sec, AP_7Log, BP_7Log, CP_7Log)
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_VoltageData.DataGridViewVoltage.RowCount - 1
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.ClearSelection()
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Rows(FrmSMGE_3222M_VoltageData.DataGridViewVoltage.RowCount - 1).Selected = True
            If FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AP_7Log)
            If FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BP_7Log)
            If FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CP_7Log)
            If FrmSMGE_3222M_VoltageData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_VoltageData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_VoltageData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_VoltageData.PictureBoxRecordInd.Visible = True
            End If
        Else
            stopwatch2.Reset()
            stopwatch2.Stop()



        End If


        '=======================================================================================================================================

        FrmSMGE_3222M_CurrentData.ChartCurrent.Series("I1").Points.AddXY(DateTime.Now.ToLongTimeString, AP_8Log)
        If FrmSMGE_3222M_CurrentData.ChartCurrent.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_CurrentData.ChartCurrent.Series(0).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_CurrentData.ChartCurrent.Series("I2").Points.AddXY(DateTime.Now.ToLongTimeString, BP_8Log)
        If FrmSMGE_3222M_CurrentData.ChartCurrent.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_CurrentData.ChartCurrent.Series(1).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_CurrentData.ChartCurrent.Series("I3").Points.AddXY(DateTime.Now.ToLongTimeString, CP_8Log)
        If FrmSMGE_3222M_CurrentData.ChartCurrent.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_CurrentData.ChartCurrent.Series(2).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_CurrentData.ChartCurrent.Series("I_n").Points.AddXY(DateTime.Now.ToLongTimeString, TP_9Log)
        If FrmSMGE_3222M_CurrentData.ChartCurrent.Series(3).Points.Count = Limit Then
            FrmSMGE_3222M_CurrentData.ChartCurrent.Series(3).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_CurrentData.ChartCurrent.Series("Itot").Points.AddXY(DateTime.Now.ToLongTimeString, TP_10Log)
        If FrmSMGE_3222M_CurrentData.ChartCurrent.Series(4).Points.Count = Limit Then
            FrmSMGE_3222M_CurrentData.ChartCurrent.Series(4).Points.RemoveAt(0)
        End If

        Dim RecordCheck3 As Boolean = FrmSMGE_3222M_CurrentData.SR

        If RecordCheck3 = True Then
            stopwatch3.Start()
            Dim elapsed As TimeSpan = stopwatch3.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_CurrentData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_CurrentData.DataGridViewCurrent.Rows.Add(_min & ":" & _sec, AP_7Log, BP_7Log, CP_7Log, TP_9Log, TP_10Log)
            FrmSMGE_3222M_CurrentData.DataGridViewCurrent.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_CurrentData.DataGridViewCurrent.RowCount - 1
            FrmSMGE_3222M_CurrentData.DataGridViewCurrent.ClearSelection()
            FrmSMGE_3222M_CurrentData.DataGridViewCurrent.Rows(FrmSMGE_3222M_CurrentData.DataGridViewCurrent.RowCount - 1).Selected = True



            If FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AP_8Log)

            If FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BP_8Log)

            If FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CP_8Log)

            If FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series4").Points.Count = 15 Then
                FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series4").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series4").Points.AddXY(_min & ":" & _sec, TP_9Log)

            If FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series5").Points.Count = 15 Then
                FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series5").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_CurrentData.ChartCurrentRec.Series("Series5").Points.AddXY(_min & ":" & _sec, TP_10Log)


            If FrmSMGE_3222M_CurrentData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_CurrentData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_CurrentData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_CurrentData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch3.Reset()
            stopwatch3.Stop()



        End If

        '==============================================================================================================================

        FrmSMGE_3222M_ActPowerData.ChartActPower.Series("P1").Points.AddXY(DateTime.Now.ToLongTimeString, AP_1Log)
        If FrmSMGE_3222M_ActPowerData.ChartActPower.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_ActPowerData.ChartActPower.Series(0).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_ActPowerData.ChartActPower.Series("P2").Points.AddXY(DateTime.Now.ToLongTimeString, BP_1Log)
        If FrmSMGE_3222M_ActPowerData.ChartActPower.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_ActPowerData.ChartActPower.Series(1).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_ActPowerData.ChartActPower.Series("P3").Points.AddXY(DateTime.Now.ToLongTimeString, CP_1Log)
        If FrmSMGE_3222M_ActPowerData.ChartActPower.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_ActPowerData.ChartActPower.Series(2).Points.RemoveAt(0)
        End If


        FrmSMGE_3222M_ActPowerData.ChartActPower.Series("Ptot").Points.AddXY(DateTime.Now.ToLongTimeString, TP_1Log)
        If FrmSMGE_3222M_ActPowerData.ChartActPower.Series(3).Points.Count = Limit Then
            FrmSMGE_3222M_ActPowerData.ChartActPower.Series(3).Points.RemoveAt(0)
        End If

        Dim RecordCheck4 As Boolean = FrmSMGE_3222M_ActPowerData.SR

        If RecordCheck4 = True Then
            stopwatch4.Start()
            Dim elapsed As TimeSpan = stopwatch4.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_ActPowerData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_ActPowerData.DataGridViewActPower.Rows.Add(_min & ":" & _sec, AP_1Log, BP_1Log, CP_1Log, TP_1Log)
            FrmSMGE_3222M_ActPowerData.DataGridViewActPower.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_ActPowerData.DataGridViewActPower.RowCount - 1
            FrmSMGE_3222M_ActPowerData.DataGridViewActPower.ClearSelection()
            FrmSMGE_3222M_ActPowerData.DataGridViewActPower.Rows(FrmSMGE_3222M_ActPowerData.DataGridViewActPower.RowCount - 1).Selected = True



            If FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AP_1Log)

            If FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BP_1Log)

            If FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CP_1Log)

            If FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series4").Points.Count = 15 Then
                FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series4").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActPowerData.ChartPowerRec.Series("Series4").Points.AddXY(_min & ":" & _sec, TP_1Log)




            If FrmSMGE_3222M_ActPowerData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_ActPowerData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_ActPowerData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_ActPowerData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch4.Reset()
            stopwatch4.Stop()



        End If


        '==============================================================================================================================

        FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series("Q1").Points.AddXY(DateTime.Now.ToLongTimeString, AP_2Log)
        If FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(0).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series("Q2").Points.AddXY(DateTime.Now.ToLongTimeString, BP_2Log)
        If FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(1).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series("Q3").Points.AddXY(DateTime.Now.ToLongTimeString, CP_2Log)
        If FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(2).Points.RemoveAt(0)
        End If


        FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series("Qtot").Points.AddXY(DateTime.Now.ToLongTimeString, TP_2Log)
        If FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(3).Points.Count = Limit Then
            FrmSMGE_3222M_ReactPowerData.ChartReactPower.Series(3).Points.RemoveAt(0)
        End If

        Dim RecordCheck5 As Boolean = FrmSMGE_3222M_ReactPowerData.SR

        If RecordCheck5 = True Then
            stopwatch5.Start()
            Dim elapsed As TimeSpan = stopwatch5.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_ReactPowerData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_ReactPowerData.DataGridViewReacPower.Rows.Add(_min & ":" & _sec, AP_2Log, BP_2Log, CP_2Log, TP_2Log)
            FrmSMGE_3222M_ReactPowerData.DataGridViewReacPower.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_ReactPowerData.DataGridViewReacPower.RowCount - 1
            FrmSMGE_3222M_ReactPowerData.DataGridViewReacPower.ClearSelection()
            FrmSMGE_3222M_ReactPowerData.DataGridViewReacPower.Rows(FrmSMGE_3222M_ReactPowerData.DataGridViewReacPower.RowCount - 1).Selected = True



            If FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AP_2Log)

            If FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BP_2Log)

            If FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CP_2Log)

            If FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series4").Points.Count = 15 Then
                FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series4").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ReactPowerData.ChartReacPowerRec.Series("Series4").Points.AddXY(_min & ":" & _sec, TP_2Log)




            If FrmSMGE_3222M_ReactPowerData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_ReactPowerData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_ReactPowerData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_ReactPowerData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch5.Reset()
            stopwatch5.Stop()



        End If


        '==============================================================================================================================

        FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series("S1").Points.AddXY(DateTime.Now.ToLongTimeString, AP_3Log)
        If FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(0).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series("S2").Points.AddXY(DateTime.Now.ToLongTimeString, BP_3Log)
        If FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(1).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series("S3").Points.AddXY(DateTime.Now.ToLongTimeString, CP_3Log)
        If FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(2).Points.RemoveAt(0)
        End If


        FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series("Stot").Points.AddXY(DateTime.Now.ToLongTimeString, TP_3Log)
        If FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(3).Points.Count = Limit Then
            FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(3).Points.RemoveAt(0)
        End If

        Dim RecordCheck6 As Boolean = FrmSMGE_3222M_AppntPowerData.SR

        If RecordCheck6 = True Then
            stopwatch6.Start()
            Dim elapsed As TimeSpan = stopwatch6.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_AppntPowerData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_AppntPowerData.DataGridViewApperantPower.Rows.Add(_min & ":" & _sec, AP_3Log, BP_3Log, CP_3Log, TP_3Log)
            FrmSMGE_3222M_AppntPowerData.DataGridViewApperantPower.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_AppntPowerData.DataGridViewApperantPower.RowCount - 1
            FrmSMGE_3222M_AppntPowerData.DataGridViewApperantPower.ClearSelection()
            FrmSMGE_3222M_AppntPowerData.DataGridViewApperantPower.Rows(FrmSMGE_3222M_AppntPowerData.DataGridViewApperantPower.RowCount - 1).Selected = True



            If FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AP_3Log)

            If FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BP_3Log)

            If FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CP_3Log)

            If FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series4").Points.Count = 15 Then
                FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series4").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_AppntPowerData.ChartApperantPowerRec.Series("Series4").Points.AddXY(_min & ":" & _sec, TP_3Log)




            If FrmSMGE_3222M_AppntPowerData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_AppntPowerData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_AppntPowerData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_AppntPowerData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch6.Reset()
            stopwatch6.Stop()



        End If



        '==============================================================================================================================

        FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series("PF1").Points.AddXY(DateTime.Now.ToLongTimeString, AP_5Log)
        If FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(0).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series("PF2").Points.AddXY(DateTime.Now.ToLongTimeString, BP_5Log)
        If FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_AppntPowerData.ChartApperantPower.Series(1).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series("PF3").Points.AddXY(DateTime.Now.ToLongTimeString, CP_5Log)
        If FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series(2).Points.RemoveAt(0)
        End If


        FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series("PFtot").Points.AddXY(DateTime.Now.ToLongTimeString, TP_5Log)
        If FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series(3).Points.Count = Limit Then
            FrmSMGE_3222M_PowerFactorData.ChartPowerFactor.Series(3).Points.RemoveAt(0)
        End If

        Dim RecordCheck7 As Boolean = FrmSMGE_3222M_PowerFactorData.SR

        If RecordCheck7 = True Then
            stopwatch7.Start()
            Dim elapsed As TimeSpan = stopwatch7.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_PowerFactorData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_PowerFactorData.DataGridViewPowerFactor.Rows.Add(_min & ":" & _sec, AP_5Log, BP_5Log, CP_5Log, TP_5Log)
            FrmSMGE_3222M_PowerFactorData.DataGridViewPowerFactor.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_PowerFactorData.DataGridViewPowerFactor.RowCount - 1
            FrmSMGE_3222M_PowerFactorData.DataGridViewPowerFactor.ClearSelection()
            FrmSMGE_3222M_PowerFactorData.DataGridViewPowerFactor.Rows(FrmSMGE_3222M_PowerFactorData.DataGridViewPowerFactor.RowCount - 1).Selected = True



            If FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AP_5Log)

            If FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BP_5Log)

            If FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CP_5Log)

            If FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series4").Points.Count = 15 Then
                FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series4").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_PowerFactorData.ChartPowerFactorRec.Series("Series4").Points.AddXY(_min & ":" & _sec, TP_5Log)




            If FrmSMGE_3222M_PowerFactorData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_PowerFactorData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_PowerFactorData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_PowerFactorData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch7.Reset()
            stopwatch7.Stop()



        End If


        '==============================================================================================================================

        FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series("Ang1").Points.AddXY(DateTime.Now.ToLongTimeString, AP_9Log)
        If FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series(0).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series("Ang2").Points.AddXY(DateTime.Now.ToLongTimeString, BP_9Log)
        If FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series(1).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series("Ang3").Points.AddXY(DateTime.Now.ToLongTimeString, CP_9Log)
        If FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngle.Series(2).Points.RemoveAt(0)
        End If




        Dim RecordCheck8 As Boolean = FrmSMGE_3222M_PhaseAngleData.SR

        If RecordCheck8 = True Then
            stopwatch8.Start()
            Dim elapsed As TimeSpan = stopwatch8.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_PhaseAngleData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_PhaseAngleData.DataGridViewPhaseAngle.Rows.Add(_min & ":" & _sec, AP_9Log, BP_9Log, CP_9Log)
            FrmSMGE_3222M_PhaseAngleData.DataGridViewPhaseAngle.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_PhaseAngleData.DataGridViewPhaseAngle.RowCount - 1
            FrmSMGE_3222M_PhaseAngleData.DataGridViewPhaseAngle.ClearSelection()
            FrmSMGE_3222M_PhaseAngleData.DataGridViewPhaseAngle.Rows(FrmSMGE_3222M_PhaseAngleData.DataGridViewPhaseAngle.RowCount - 1).Selected = True



            If FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AP_9Log)

            If FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BP_9Log)

            If FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_PhaseAngleData.ChartPhaseAngleRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CP_9Log)






            If FrmSMGE_3222M_PhaseAngleData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_PhaseAngleData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_PhaseAngleData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_PhaseAngleData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch8.Reset()
            stopwatch8.Stop()



        End If


        '==============================================================================================================================

        FrmSMGE_3222M_FrequencyData.ChartFrequency.Series("Freq").Points.AddXY(DateTime.Now.ToLongTimeString, TP_8Log)
        If FrmSMGE_3222M_FrequencyData.ChartFrequency.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_FrequencyData.ChartFrequency.Series(0).Points.RemoveAt(0)
        End If






        Dim RecordCheck9 As Boolean = FrmSMGE_3222M_FrequencyData.SR

        If RecordCheck9 = True Then
            stopwatch9.Start()
            Dim elapsed As TimeSpan = stopwatch9.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_FrequencyData.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_FrequencyData.DataGridViewFrequency.Rows.Add(_min & ":" & _sec, TP_8Log)
            FrmSMGE_3222M_FrequencyData.DataGridViewFrequency.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_FrequencyData.DataGridViewFrequency.RowCount - 1
            FrmSMGE_3222M_FrequencyData.DataGridViewFrequency.ClearSelection()
            FrmSMGE_3222M_FrequencyData.DataGridViewFrequency.Rows(FrmSMGE_3222M_FrequencyData.DataGridViewFrequency.RowCount - 1).Selected = True



            If FrmSMGE_3222M_FrequencyData.ChartFrequencyRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_FrequencyData.ChartFrequencyRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_FrequencyData.ChartFrequencyRec.Series("Series1").Points.AddXY(_min & ":" & _sec, TP_8Log)




            If FrmSMGE_3222M_FrequencyData.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_FrequencyData.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_FrequencyData.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_FrequencyData.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch9.Reset()
            stopwatch9.Stop()



        End If


        '==============================================================================================================================

        FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series("AE1").Points.AddXY(DateTime.Now.ToLongTimeString, AE_1Log)
        If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(0).Points.Count = Limit Then
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(0).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series("BE1").Points.AddXY(DateTime.Now.ToLongTimeString, BE_1Log)
        If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(1).Points.Count = Limit Then
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(1).Points.RemoveAt(0)
        End If

        FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series("CE1").Points.AddXY(DateTime.Now.ToLongTimeString, CE_1Log)
        If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(2).Points.Count = Limit Then
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(2).Points.RemoveAt(0)
        End If


        FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series("TE1").Points.AddXY(DateTime.Now.ToLongTimeString, TE_1Log)
        If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(3).Points.Count = Limit Then
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImp.Series(3).Points.RemoveAt(0)
        End If

        Dim RecordCheck10 As Boolean = FrmSMGE_3222M_ActiveEnergyImp.SR

        If RecordCheck10 = True Then
            stopwatch10.Start()
            Dim elapsed As TimeSpan = stopwatch10.Elapsed
            _sec = elapsed.Seconds.ToString("00")
            _min = elapsed.Minutes.ToString("00")
            FrmSMGE_3222M_ActiveEnergyImp.LblTime.Text = "Timer " & _min & ":" & _sec

            'ShowHB = _sec Mod 10


            FrmSMGE_3222M_ActiveEnergyImp.DataGridViewActiveEnergyImp.Rows.Add(_min & ":" & _sec, AE_1Log, BE_1Log, CE_1Log, TE_1Log)
            FrmSMGE_3222M_ActiveEnergyImp.DataGridViewActiveEnergyImp.FirstDisplayedScrollingRowIndex = FrmSMGE_3222M_ActiveEnergyImp.DataGridViewActiveEnergyImp.RowCount - 1
            FrmSMGE_3222M_ActiveEnergyImp.DataGridViewActiveEnergyImp.ClearSelection()
            FrmSMGE_3222M_ActiveEnergyImp.DataGridViewActiveEnergyImp.Rows(FrmSMGE_3222M_ActiveEnergyImp.DataGridViewActiveEnergyImp.RowCount - 1).Selected = True



            If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series1").Points.Count = 15 Then
                FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series1").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series1").Points.AddXY(_min & ":" & _sec, AE_1Log)

            If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series2").Points.Count = 15 Then
                FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series2").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series2").Points.AddXY(_min & ":" & _sec, BE_1Log)

            If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series3").Points.Count = 15 Then
                FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series3").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series3").Points.AddXY(_min & ":" & _sec, CE_1Log)

            If FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series4").Points.Count = 15 Then
                FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series4").Points.RemoveAt(0)
            End If
            FrmSMGE_3222M_ActiveEnergyImp.ChartActiveEnergyImpRec.Series("Series4").Points.AddXY(_min & ":" & _sec, TE_1Log)




            If FrmSMGE_3222M_ActiveEnergyImp.PictureBoxRecordInd.Visible = True Then
                FrmSMGE_3222M_ActiveEnergyImp.PictureBoxRecordInd.Visible = False
            ElseIf FrmSMGE_3222M_ActiveEnergyImp.PictureBoxRecordInd.Visible = False Then
                FrmSMGE_3222M_ActiveEnergyImp.PictureBoxRecordInd.Visible = True
            End If

        Else
            stopwatch10.Reset()
            stopwatch10.Stop()



        End If


    End Sub



    Private Sub TimerDataLogRecord_Tick(sender As Object, e As EventArgs) Handles TimerDataLogRecord.Tick





        FrmSMGE_3222MDataLogger.DataGridView1.Rows.Add(New String() {FrmSMGE_3222MDataLogger.DataGridView1.RowCount, AP_7Log, BP_7Log, CP_7Log, AP_8Log, BP_8Log, CP_8Log, TP_9Log, TP_10Log,
        AP_1Log, BP_1Log, CP_1Log, TP_1Log, AP_2Log, BP_2Log, CP_2Log, TP_2Log, AP_3Log, BP_3Log, CP_3Log, TP_3Log, AP_5Log, BP_5Log, CP_5Log, TP_5Log, AP_9Log, BP_9Log, CP_9Log, TP_8Log,
        AE_1Log, BE_1Log, CE_1Log, TE_1Log, AE_2Log, BE_2Log, CE_2Log, TE_2Log, AE_3Log, BE_3Log, CE_3Log, TE_3Log, AE_4Log, BE_4Log, CE_4Log, TE_4Log, AE_5Log, BE_5Log, CE_5Log, TE_5Log, TE_6Log, DP_1Log, DP_2Log, DP_3Log, DT.ToLongTimeString, DT.ToString("dd-MM-yyyy")})
        FrmSMGE_3222MDataLogger.DataGridView1.FirstDisplayedScrollingRowIndex = FrmSMGE_3222MDataLogger.DataGridView1.RowCount - 1






        'FrmSMGE_3222M_Humidity.ChartHumidityRec.Series("Series1").Points.AddXY(0, DP_1Log)

        If FrmSMGE_3222MDataLogger.PictureBoxRecordInd.Visible = True Then
            FrmSMGE_3222MDataLogger.PictureBoxRecordInd.Visible = False
        ElseIf FrmSMGE_3222MDataLogger.PictureBoxRecordInd.Visible = False Then
            FrmSMGE_3222MDataLogger.PictureBoxRecordInd.Visible = True
        End If
    End Sub




    Private Sub FrmSMGE_3222MLoging_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        SetMode = False
        CalMode = False

        TimerMonitor.Stop()
        TimerCommands.Enabled = False
        TimerCommands.Stop()

        TimerSerial.Enabled = False
        TimerSerial.Stop()

        TimerSync.Enabled = False
        TimerSync.Stop()

        CheckForIllegalCrossThreadCalls = False
        TBox.Multiline = True
        CmdCallibrate.Enabled = False

        CmdlSettings.Enabled = False

        'strHostName = Dns.GetHostName()
        strHostName = System.Net.Dns.GetHostName()
        Dim ipEntry As IPHostEntry = Dns.GetHostEntry(strHostName)
        addr = ipEntry.AddressList


        '=======================================================================
        'Dim strHostName As String

        Dim strIPAddress As String


        strIPAddress = System.Net.Dns.GetHostByName(strHostName).AddressList(0).ToString()


        'MessageBox.Show("Host Name: " & strHostName & "; IP Address: " & strIPAddress)




        '======================================================================


        Dim i As Integer

        For i = 0 To addr.Length - 1
            If addr(i).AddressFamily = AddressFamily.InterNetwork Then
                StatusLabeladapter.Text = String.Format("Host IP : {0}", addr(i).ToString) & " --> Host Name : " & strHostName
                Exit For

            End If

        Next





        TxtServerIP.Text = "169.254.33.20"
        TxtTCPPort.Text = "8088"
        'TxtServerIP.Text = addr(i).ToString
        TxtServerIP.Text = strIPAddress


    End Sub

    Private Sub BtnScanPort_Click(sender As Object, e As EventArgs) Handles CmdScanPort.Click
        ComboBoxPort.Items.Clear()
        Dim myPort As Array
        Dim i As Integer
        myPort = IO.Ports.SerialPort.GetPortNames()
        ComboBoxPort.Items.AddRange(myPort)
        i = ComboBoxPort.Items.Count
        i = i - i
        Try
            ComboBoxPort.SelectedIndex = i
            CmdConnect.Enabled = True
        Catch ex As Exception
            MsgBox("Com port not detected", MsgBoxStyle.Critical, "Warning !!!")
            ComboBoxPort.Text = ""
            ComboBoxPort.Items.Clear()
            CmdConnect.Enabled = False
            'ButtonStartRecording.Enabled = False
            Return
        End Try
        ComboBoxPort.DroppedDown = True
    End Sub

    Private Sub CmdConnect_Click(sender As Object, e As EventArgs) Handles CmdConnect.Click

        Try
            SerialPort.Close()
            SerialPort.BaudRate = ComboBoxBaudRate.SelectedItem
            SerialPort.PortName = ComboBoxPort.SelectedItem
            SerialPort.DataBits = 8
            SerialPort.Parity = Parity.None
            SerialPort.StopBits = StopBits.One
            SerialPort.Handshake = Handshake.None
            SerialPort.Encoding = System.Text.Encoding.Default
            SerialPort.ReadTimeout = 10000
            SerialPort.Open()

            SerialPort.DiscardOutBuffer()
            SerialPort.DiscardInBuffer()

            TimerMonitor.Start()
            TimerCommands.Enabled = False
            TimerCommands.Stop()

            CmdlSettings.Enabled = True
            CmdCallibrate.Enabled = True
            ' TimerSerial.Start()
            TimerGaugeCom.Stop()
            ComboBoxPort.Enabled = False
            LblBaudRate.Enabled = False
            ComboBoxBaudRate.Enabled = False
            CmdScanPort.Enabled = False
            CmdConnect.Enabled = False
            CmdDisconnect.Enabled = True
            CmdTCPSettings.Enabled = True

            TimerRefreshForm.Start()
            PictureBoxConnectionInd.Image = My.Resources.Green
            LblStatus.Text = "Status : Connected"

        Catch ex As Exception
            MsgBox("Failed to open Port. Ensure that Meter is connected to the computer correctly and that you have selected the correct port." & vbCrLf & "Error Message : " & ex.Message, MsgBoxStyle.Critical, "Error!")
        End Try

    End Sub

    Private Sub CmdDisconnect_Click(sender As Object, e As EventArgs) Handles CmdDisconnect.Click
        'TimerStopSerial.Enabled = True
        'TimerStopSerial.Start()

        TimerSerial.Stop()
        TimerMonitor.Stop()
        TimerGaugeCom.Start()
        PictureBoxConnectionInd.Image = My.Resources.Red
        PictureBoxConnectionInd.Visible = True
        LblStatus.Text = "Status : Disconnect"

        ComboBoxPort.Enabled = True
        LblBaudRate.Enabled = True
        ComboBoxBaudRate.Enabled = True
        CmdScanPort.Enabled = True
        CmdConnect.Enabled = True
        CmdDisconnect.Enabled = False
        CmdlSettings.Enabled = False
        CmdCallibrate.Enabled = False
        'TimerSerial.Stop()
        'TimerGaugeCom.Start()
        TimerHandler1_Client.Stop()
        SerialPort.Close()




    End Sub

    Private Sub OptSerialConnect_CheckedChanged(sender As Object, e As EventArgs) Handles OptSerialConnect.CheckedChanged
        GrpConnection.Enabled = True
        GrpEthernet.Enabled = False

        'If OptSerialConnect.Checked = True Then
        '    FrmSMGE_3222MSettings.ChkSerial.Checked = True
        '    FrmSMGE_3222MSettings.ChkSerial.Text = "ON"

        'Else
        '    FrmSMGE_3222MSettings.ChkSerial.Checked = False
        '    FrmSMGE_3222MSettings.ChkSerial.Text = "OFF"
        'End If


    End Sub

    Private Sub OptTCPIP_CheckedChanged(sender As Object, e As EventArgs) Handles OptTCPIP.CheckedChanged

        GrpConnection.Enabled = False
        GrpEthernet.Enabled = True
        PnlModbus.Enabled = False

        CmdTCPDisconnect.Enabled = False
        CmdTCPConnect.Enabled = True


        If OptTCPIP.Checked = True Then
            'TimerMonitor.Enabled = False
            'TimerMonitor.Stop()
            '    FrmSMGE_3222MSettings.ChkEthernetMode.Checked = True
            '    FrmSMGE_3222MSettings.ChkEthernetMode.Text = "ON"

            'Else

            '    FrmSMGE_3222MSettings.ChkEthernetMode.Checked = False
            '    FrmSMGE_3222MSettings.ChkEthernetMode.Text = "OFF"


        End If



    End Sub

    Private Sub TimerSerial_Tick(sender As Object, e As EventArgs) Handles TimerSerial.Tick
        'SerialPort.Open()
        SerialPort.WriteLine("PC_ENABLED")

        Try

            'Dim StrSerialIn As String = SerialPort.ReadExisting
            'Dim StrSerialInRam As String

            StrSerialIn = SerialPort.ReadExisting
            Dim TB As New TextBox
            TB.Multiline = True
            TB.Text = StrSerialIn


            '==================================================================================
            StrSerialInRam = TB.Lines(0).Substring(0, 3)
            If StrSerialInRam = "AE1" Then
                AE_1 = TB.Lines(0)
                AE_1L = AE_1.Length
            Else
                AE_1 = AE_1
            End If

            StrSerialInRam = TB.Lines(1).Substring(0, 3)
            If StrSerialInRam = "AE2" Then
                AE_2 = TB.Lines(1)
                AE_2L = AE_2.Length
            Else
                AE_2 = AE_2
            End If

            StrSerialInRam = TB.Lines(2).Substring(0, 3)
            If StrSerialInRam = "AE3" Then
                AE_3 = TB.Lines(2)
                AE_3L = AE_3.Length
            Else
                AE_3 = AE_3
            End If

            StrSerialInRam = TB.Lines(3).Substring(0, 3)
            If StrSerialInRam = "AE4" Then
                AE_4 = TB.Lines(3)
                AE_4L = AE_4.Length
            Else
                AE_4 = AE_4
            End If

            StrSerialInRam = TB.Lines(4).Substring(0, 3)
            If StrSerialInRam = "AE5" Then
                AE_5 = TB.Lines(4)
                AE_5L = AE_5.Length
            Else
                AE_5 = AE_5
            End If


            '==================================================================================
            StrSerialInRam = TB.Lines(5).Substring(0, 3)
            If StrSerialInRam = "BE1" Then
                BE_1 = TB.Lines(5)
                BE_1L = BE_1.Length
            Else
                BE_1 = BE_1
            End If

            StrSerialInRam = TB.Lines(6).Substring(0, 3)
            If StrSerialInRam = "BE2" Then
                BE_2 = TB.Lines(6)
                BE_2L = BE_2.Length
            Else
                BE_2 = BE_2
            End If

            StrSerialInRam = TB.Lines(7).Substring(0, 3)
            If StrSerialInRam = "BE3" Then
                BE_3 = TB.Lines(7)
                BE_3L = BE_3.Length
            Else
                BE_3 = BE_3
            End If

            StrSerialInRam = TB.Lines(8).Substring(0, 3)
            If StrSerialInRam = "BE4" Then
                BE_4 = TB.Lines(8)
                BE_4L = BE_4.Length
            Else
                BE_4 = BE_4
            End If

            StrSerialInRam = TB.Lines(9).Substring(0, 3)
            If StrSerialInRam = "BE5" Then
                BE_5 = TB.Lines(9)
                BE_5L = BE_5.Length
            Else
                BE_5 = BE_5
            End If

            '==================================================================================
            StrSerialInRam = TB.Lines(10).Substring(0, 3)
            If StrSerialInRam = "CE1" Then
                CE_1 = TB.Lines(10)
                CE_1L = CE_1.Length
            Else
                CE_1 = CE_1
            End If

            StrSerialInRam = TB.Lines(11).Substring(0, 3)
            If StrSerialInRam = "CE2" Then
                CE_2 = TB.Lines(11)
                CE_2L = CE_2.Length
            Else
                CE_2 = CE_2
            End If

            StrSerialInRam = TB.Lines(12).Substring(0, 3)
            If StrSerialInRam = "CE3" Then
                CE_3 = TB.Lines(12)
                CE_3L = CE_3.Length
            Else
                CE_3 = CE_3
            End If

            StrSerialInRam = TB.Lines(13).Substring(0, 3)
            If StrSerialInRam = "CE4" Then
                CE_4 = TB.Lines(13)
                CE_4L = CE_4.Length
            Else
                CE_4 = CE_4
            End If

            StrSerialInRam = TB.Lines(14).Substring(0, 3)
            If StrSerialInRam = "CE5" Then
                CE_5 = TB.Lines(14)
                CE_5L = CE_5.Length
            Else
                CE_5 = CE_5
            End If

            '==================================================================================
            StrSerialInRam = TB.Lines(15).Substring(0, 3)
            If StrSerialInRam = "TE1" Then
                TE_1 = TB.Lines(15)
                TE_1L = TE_1.Length
            Else
                BE_1 = BE_1
            End If

            StrSerialInRam = TB.Lines(16).Substring(0, 3)
            If StrSerialInRam = "TE2" Then
                TE_2 = TB.Lines(16)
                TE_2L = TE_2.Length
            Else
                TE_2 = TE_2
            End If

            StrSerialInRam = TB.Lines(17).Substring(0, 3)
            If StrSerialInRam = "TE3" Then
                TE_3 = TB.Lines(17)
                TE_3L = TE_3.Length
            Else
                TE_3 = TE_3
            End If

            StrSerialInRam = TB.Lines(18).Substring(0, 3)
            If StrSerialInRam = "TE4" Then
                TE_4 = TB.Lines(18)
                TE_4L = TE_4.Length
            Else
                TE_4 = TE_4
            End If

            StrSerialInRam = TB.Lines(19).Substring(0, 3)
            If StrSerialInRam = "TE5" Then
                TE_5 = TB.Lines(19)
                TE_5L = TE_5.Length
            Else
                TE_5 = TE_5
            End If

            StrSerialInRam = TB.Lines(20).Substring(0, 3)
            If StrSerialInRam = "TE6" Then
                TE_6 = TB.Lines(20)
                TE_6L = TE_6.Length
            Else
                TE_6 = TE_6
            End If






            FrmSM_GE3222M_Energy_Data.LblPhaA_ForwardActiveEnergyVal.Text = Mid(AE_1, 4, AE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ReverseActiveEnergyVal.Text = Mid(AE_2, 4, AE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ForwardReactiveEnergyVal.Text = Mid(AE_3, 4, AE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ReverseReactiveEnergyVal.Text = Mid(AE_4, 4, AE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaA_ApparentEnergyVal.Text = Mid(AE_5, 4, AE_5L) & " KVAh"

            FrmSM_GE3222M_Energy_Data.LblPhaB_ForwardActiveEnergyVal.Text = Mid(BE_1, 4, BE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ReverseActiveEnergyVal.Text = Mid(BE_2, 4, BE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ForwardReactiveEnergyVal.Text = Mid(BE_3, 4, BE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ReverseReactiveEnergyVal.Text = Mid(BE_4, 4, BE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaB_ApparentEnergyVal.Text = Mid(BE_5, 4, BE_5L) & " KVAh"

            FrmSM_GE3222M_Energy_Data.LblPhaC_ForwardActiveEnergyVal.Text = Mid(CE_1, 4, CE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ReverseActiveEnergyVal.Text = Mid(CE_2, 4, CE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ForwardReactiveEnergyVal.Text = Mid(CE_3, 4, CE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ReverseReactiveEnergyVal.Text = Mid(CE_4, 4, CE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblPhaC_ApparentEnergyVal.Text = Mid(CE_5, 4, CE_5L) & " KVAh"

            FrmSM_GE3222M_Energy_Data.LblTotalForwardActiveEnergyVal.Text = Mid(TE_1, 4, TE_1L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblTotalReverseActiveEnergyVal.Text = Mid(TE_2, 4, TE_2L) & " kWh"
            FrmSM_GE3222M_Energy_Data.LblTotalForwardReactiveEnergyVal.Text = Mid(TE_3, 4, TE_3L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblTotalReverseReactiveEnergyVal.Text = Mid(TE_4, 4, TE_4L) & " kvarh"
            FrmSM_GE3222M_Energy_Data.LblTotal_ArithmeticSum_ApparentEnergyVal.Text = Mid(TE_5, 4, TE_5L) & " KVAh"
            FrmSM_GE3222M_Energy_Data.LblVectorSum_TotalApparentEnergyVal.Text = Mid(TE_6, 4, TE_6L) & " KVAh"

            '==================================================================================
            StrSerialInRam = TB.Lines(21).Substring(0, 3)
            If StrSerialInRam = "AP1" Then
                AP_1 = TB.Lines(21)
                AP_1L = AP_1.Length
            Else
                AP_1 = AP_1
            End If

            StrSerialInRam = TB.Lines(22).Substring(0, 3)
            If StrSerialInRam = "AP2" Then
                AP_2 = TB.Lines(22)
                AP_2L = AP_2.Length
            Else
                AP_2 = AP_2
            End If

            StrSerialInRam = TB.Lines(23).Substring(0, 3)
            If StrSerialInRam = "AP3" Then
                AP_3 = TB.Lines(23)
                AP_3L = AP_3.Length
            Else
                AP_3 = AP_3
            End If

            StrSerialInRam = TB.Lines(24).Substring(0, 3)
            If StrSerialInRam = "AP4" Then
                AP_4 = TB.Lines(24)
                AP_4L = AP_4.Length
            Else
                AP_4 = AP_4
            End If

            StrSerialInRam = TB.Lines(25).Substring(0, 3)
            If StrSerialInRam = "AP5" Then
                AP_5 = TB.Lines(25)
                AP_5L = AP_5.Length
            Else
                AP_5 = AP_5
            End If


            StrSerialInRam = TB.Lines(26).Substring(0, 3)
            If StrSerialInRam = "AP6" Then
                AP_6 = TB.Lines(26)
                AP_6L = AP_6.Length
            Else
                AP_6 = AP_6
            End If

            StrSerialInRam = TB.Lines(27).Substring(0, 3)
            If StrSerialInRam = "AP7" Then
                AP_7 = TB.Lines(27)
                AP_7L = AP_7.Length
            Else
                AP_7 = AP_7
            End If

            StrSerialInRam = TB.Lines(28).Substring(0, 3)
            If StrSerialInRam = "AP8" Then
                AP_8 = TB.Lines(28)
                AP_8L = AP_8.Length
            Else
                AP_8 = AP_8
            End If

            StrSerialInRam = TB.Lines(29).Substring(0, 3)
            If StrSerialInRam = "AP9" Then
                AP_9 = TB.Lines(29)
                AP_9L = AP_9.Length
            Else
                AP_9 = AP_9
            End If

            StrSerialInRam = TB.Lines(30).Substring(0, 4)
            If StrSerialInRam = "AP10" Then
                AP_10 = TB.Lines(30)
                AP_10L = AP_10.Length
            Else
                AP_10 = AP_10
            End If


            '==================================================================================
            StrSerialInRam = TB.Lines(31).Substring(0, 3)
            If StrSerialInRam = "BP1" Then
                BP_1 = TB.Lines(31)
                BP_1L = BP_1.Length
            Else
                BP_1 = BP_1
            End If

            StrSerialInRam = TB.Lines(32).Substring(0, 3)
            If StrSerialInRam = "BP2" Then
                BP_2 = TB.Lines(32)
                BP_2L = BP_2.Length
            Else
                BP_2 = BP_2
            End If

            StrSerialInRam = TB.Lines(33).Substring(0, 3)
            If StrSerialInRam = "BP3" Then
                BP_3 = TB.Lines(33)
                BP_3L = BP_3.Length
            Else
                BP_3 = BP_3
            End If

            StrSerialInRam = TB.Lines(34).Substring(0, 3)
            If StrSerialInRam = "BP4" Then
                BP_4 = TB.Lines(34)
                BP_4L = BP_4.Length
            Else
                BP_4 = BP_4
            End If

            StrSerialInRam = TB.Lines(35).Substring(0, 3)
            If StrSerialInRam = "BP5" Then
                BP_5 = TB.Lines(35)
                BP_5L = BP_5.Length
            Else
                BP_5 = BP_5
            End If


            StrSerialInRam = TB.Lines(36).Substring(0, 3)
            If StrSerialInRam = "BP6" Then
                BP_6 = TB.Lines(36)
                BP_6L = BP_6.Length
            Else
                BP_6 = BP_6
            End If

            StrSerialInRam = TB.Lines(37).Substring(0, 3)
            If StrSerialInRam = "BP7" Then
                BP_7 = TB.Lines(37)
                BP_7L = BP_7.Length
            Else
                BP_7 = BP_7
            End If

            StrSerialInRam = TB.Lines(38).Substring(0, 3)
            If StrSerialInRam = "BP8" Then
                BP_8 = TB.Lines(38)
                BP_8L = BP_8.Length
            Else
                BP_8 = BP_8
            End If

            StrSerialInRam = TB.Lines(39).Substring(0, 3)
            If StrSerialInRam = "BP9" Then
                BP_9 = TB.Lines(39)
                BP_9L = BP_9.Length
            Else
                BP_9 = BP_9
            End If

            StrSerialInRam = TB.Lines(40).Substring(0, 4)
            If StrSerialInRam = "BP10" Then
                BP_10 = TB.Lines(40)
                BP_10L = BP_10.Length
            Else
                BP_10 = BP_10
            End If



            '==================================================================================
            StrSerialInRam = TB.Lines(41).Substring(0, 3)
            If StrSerialInRam = "CP1" Then
                CP_1 = TB.Lines(41)
                CP_1L = CP_1.Length
            Else
                CP_1 = CP_1
            End If

            StrSerialInRam = TB.Lines(42).Substring(0, 3)
            If StrSerialInRam = "CP2" Then
                CP_2 = TB.Lines(42)
                CP_2L = CP_2.Length
            Else
                CP_2 = CP_2
            End If

            StrSerialInRam = TB.Lines(43).Substring(0, 3)
            If StrSerialInRam = "CP3" Then
                CP_3 = TB.Lines(43)
                CP_3L = CP_3.Length
            Else
                CP_3 = CP_3
            End If

            StrSerialInRam = TB.Lines(44).Substring(0, 3)
            If StrSerialInRam = "CP4" Then
                CP_4 = TB.Lines(44)
                CP_4L = CP_4.Length
            Else
                CP_4 = CP_4
            End If

            StrSerialInRam = TB.Lines(45).Substring(0, 3)
            If StrSerialInRam = "CP5" Then
                CP_5 = TB.Lines(45)
                CP_5L = CP_5.Length
            Else
                CP_5 = CP_5
            End If


            StrSerialInRam = TB.Lines(46).Substring(0, 3)
            If StrSerialInRam = "CP6" Then
                CP_6 = TB.Lines(46)
                CP_6L = CP_6.Length
            Else
                CP_6 = CP_6
            End If

            StrSerialInRam = TB.Lines(47).Substring(0, 3)
            If StrSerialInRam = "CP7" Then
                CP_7 = TB.Lines(47)
                CP_7L = CP_7.Length
            Else
                CP_7 = CP_7
            End If

            StrSerialInRam = TB.Lines(48).Substring(0, 3)
            If StrSerialInRam = "CP8" Then
                CP_8 = TB.Lines(48)
                CP_8L = CP_8.Length
            Else
                CP_8 = CP_8
            End If

            StrSerialInRam = TB.Lines(49).Substring(0, 3)
            If StrSerialInRam = "CP9" Then
                CP_9 = TB.Lines(49)
                CP_9L = CP_9.Length
            Else
                CP_9 = CP_9
            End If

            StrSerialInRam = TB.Lines(50).Substring(0, 4)
            If StrSerialInRam = "CP10" Then
                CP_10 = TB.Lines(50)
                CP_10L = CP_10.Length
            Else
                CP_10 = CP_10
            End If



            '==================================================================================
            StrSerialInRam = TB.Lines(51).Substring(0, 3)
            If StrSerialInRam = "TP1" Then
                TP_1 = TB.Lines(51)
                TP_1L = TP_1.Length
            Else
                TP_1 = TP_1
            End If

            StrSerialInRam = TB.Lines(52).Substring(0, 3)
            If StrSerialInRam = "TP2" Then
                TP_2 = TB.Lines(52)
                TP_2L = TP_2.Length
            Else
                TP_2 = TP_2
            End If

            StrSerialInRam = TB.Lines(53).Substring(0, 3)
            If StrSerialInRam = "TP3" Then
                TP_3 = TB.Lines(53)
                TP_3L = TP_3.Length
            Else
                TP_3 = TP_3
            End If

            StrSerialInRam = TB.Lines(54).Substring(0, 3)
            If StrSerialInRam = "TP4" Then
                TP_4 = TB.Lines(54)
                TP_4L = TP_4.Length
            Else
                TP_4 = TP_4
            End If

            StrSerialInRam = TB.Lines(55).Substring(0, 3)
            If StrSerialInRam = "TP5" Then
                TP_5 = TB.Lines(55)
                TP_5L = TP_5.Length
            Else
                TP_5 = TP_5
            End If


            StrSerialInRam = TB.Lines(56).Substring(0, 3)
            If StrSerialInRam = "TP6" Then
                TP_6 = TB.Lines(56)
                TP_6L = TP_6.Length
            Else
                TP_6 = TP_6
            End If

            StrSerialInRam = TB.Lines(57).Substring(0, 3)
            If StrSerialInRam = "TP7" Then
                TP_7 = TB.Lines(57)
                TP_7L = TP_7.Length
            Else
                TP_7 = TP_7
            End If

            StrSerialInRam = TB.Lines(58).Substring(0, 3)
            If StrSerialInRam = "TP8" Then
                TP_8 = TB.Lines(58)
                TP_8L = TP_8.Length
            Else
                TP_8 = TP_8
            End If

            StrSerialInRam = TB.Lines(59).Substring(0, 3)
            If StrSerialInRam = "TP9" Then
                TP_9 = TB.Lines(59)
                TP_9L = TP_9.Length
            Else
                TP_9 = TP_9
            End If

            StrSerialInRam = TB.Lines(60).Substring(0, 4)
            If StrSerialInRam = "TP10" Then
                TP_10 = TB.Lines(60)
                TP_10L = TP_10.Length
            Else
                TP_10 = TP_10
            End If

            StrSerialInRam = TB.Lines(61).Substring(0, 3)
            If StrSerialInRam = "DP1" Then
                DP_1 = TB.Lines(61)
                DP_1L = DP_1.Length
            Else
                DP_1 = DP_1
            End If

            StrSerialInRam = TB.Lines(62).Substring(0, 3)
            If StrSerialInRam = "DP2" Then
                DP_2 = TB.Lines(62)
                DP_2L = DP_2.Length
            Else
                DP_2 = DP_2
            End If

            StrSerialInRam = TB.Lines(63).Substring(0, 3)
            If StrSerialInRam = "DP3" Then
                DP_3 = TB.Lines(63)
                DP_3L = DP_3.Length
            Else
                DP_3 = DP_3
            End If

            'Voltages
            AP_7Log = Mid(AP_7, 4, AP_7L)
            BP_7Log = Mid(BP_7, 4, BP_7L)
            CP_7Log = Mid(CP_7, 4, CP_7L)

            'Currents
            AP_8Log = Mid(AP_8, 4, AP_8L)
            BP_8Log = Mid(BP_8, 4, BP_8L)
            CP_8Log = Mid(CP_8, 4, CP_8L)
            TP_9Log = Mid(TP_9, 4, TP_9L)
            TP_10Log = Mid(TP_10, 5, TP_10L)

            'Active Power
            AP_1Log = Mid(AP_1, 4, AP_1L)
            BP_1Log = Mid(BP_1, 4, BP_1L)
            CP_1Log = Mid(CP_1, 4, CP_1L)
            TP_1Log = Mid(TP_1, 4, TP_1L)

            'Reactive Power
            AP_2Log = Mid(AP_2, 4, AP_2L)
            BP_2Log = Mid(BP_2, 4, BP_2L)
            CP_2Log = Mid(CP_2, 4, CP_2L)
            TP_2Log = Mid(TP_2, 4, TP_2L)

            'Apperant Power
            AP_3Log = Mid(AP_3, 4, AP_3L)
            BP_3Log = Mid(BP_3, 4, BP_3L)
            CP_3Log = Mid(CP_3, 4, CP_3L)
            TP_3Log = Mid(TP_3, 4, TP_3L)

            'Power Factor
            AP_5Log = Mid(AP_5, 4, AP_5L)
            BP_5Log = Mid(BP_5, 4, BP_5L)
            CP_5Log = Mid(CP_5, 4, CP_5L)
            TP_5Log = Mid(TP_5, 4, TP_5L)

            'Phase Angle
            AP_9Log = Mid(AP_9, 4, AP_9L)
            BP_9Log = Mid(BP_9, 4, BP_9L)
            CP_9Log = Mid(CP_9, 4, CP_9L)

            'Frequency
            TP_8Log = Mid(TP_8, 4, TP_8L)

            'Active Energy Imp
            AE_1Log = Mid(AE_1, 4, AE_1L)
            BE_1Log = Mid(BE_1, 4, BE_1L)
            CE_1Log = Mid(CE_1, 4, CE_1L)
            TE_1Log = Mid(TE_1, 4, TE_1L)

            'Active Energy Exp
            AE_2Log = Mid(AE_2, 4, AE_2L)
            BE_2Log = Mid(BE_2, 4, BE_2L)
            CE_2Log = Mid(CE_2, 4, CE_2L)
            TE_2Log = Mid(TE_2, 4, TE_2L)


            'Reactive Energy Imp
            AE_3Log = Mid(AP_3, 4, AP_3L)
            BE_3Log = Mid(BP_3, 4, BP_3L)
            CE_3Log = Mid(CP_3, 4, CP_3L)
            TE_3Log = Mid(TE_3, 4, TE_3L)

            'Reactive Energy Exp
            AE_4Log = Mid(AE_4, 4, AE_4L)
            BE_4Log = Mid(BE_4, 4, BE_4L)
            CE_4Log = Mid(CE_4, 4, CE_4L)
            TE_4Log = Mid(TE_4, 4, TE_4L)


            'Apperent Energy
            AE_5Log = Mid(AE_5, 4, AE_5L)
            BE_5Log = Mid(BE_5, 4, BE_5L)
            CE_5Log = Mid(CE_5, 4, CE_5L)
            TE_5Log = Mid(TE_5, 4, TE_5L)
            TE_6Log = Mid(TE_6, 4, TE_6L)


            'Temperature and Humidity
            DP_1Log = Mid(DP_1, 4, DP_1L)
            DP_2Log = Mid(DP_2, 4, DP_2L)
            DP_3Log = Mid(DP_3, 4, DP_3L)


            FrmSMGE_3222M_BoardData.LblHumidityVal.Text = Mid(DP_1, 4, DP_1L)
            FrmSMGE_3222M_BoardData.LblTempCVal.Text = Mid(DP_2, 4, DP_2L)
            FrmSMGE_3222M_VoltageData.LblV1Val.Text = Mid(AP_7, 4, AP_7L)
            FrmSMGE_3222M_VoltageData.LblV2Val.Text = Mid(BP_7, 4, BP_7L)
            FrmSMGE_3222M_VoltageData.LblV3Val.Text = Mid(CP_7, 4, CP_7L)



            FrmVoltage.TxtVoltage.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "=============================="
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & ""
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 1 Voltage :   " & Mid(AP_7, 4, AP_7L) & "V"
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 2 Voltage :   " & Mid(BP_7, 4, BP_7L) & "V"
            FrmVoltage.TxtVoltage.Text &= Environment.NewLine & "Pha 3 Voltage :   " & Mid(CP_7, 4, CP_7L) & "V"
            'FrmVoltage.TxtVoltage.Focus()
            'FrmVoltage.TxtVoltage.SelectionStart = FrmVoltage.TxtVoltage.Text.Length


            FrmSMGE_3222M_CurrentData.LblI1Val.Text = Mid(AP_8, 4, AP_8L)
            FrmSMGE_3222M_CurrentData.LblI2Val.Text = Mid(BP_8, 4, BP_8L)
            FrmSMGE_3222M_CurrentData.LblI3Val.Text = Mid(CP_8, 4, CP_8L)
            FrmSMGE_3222M_CurrentData.LB1.Text = Mid(TP_9, 4, TP_9L)
            FrmSMGE_3222M_CurrentData.LB2.Text = Mid(TP_10, 5, TP_10L)

            FrmCurrent.TxtCurrent.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "=============================="
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & ""
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 1 Current      :   " & Mid(AP_8, 4, AP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 2 Current      :   " & Mid(BP_8, 4, BP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Pha 3 Current      :   " & Mid(CP_8, 4, CP_8L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Neutral Current    :   " & Mid(TP_9, 4, TP_9L) & "A"
            FrmCurrent.TxtCurrent.Text &= Environment.NewLine & "Total Current      :   " & Mid(TP_10, 5, TP_10L) & "A"

            FrmSMGE_3222M_ActPowerData.TxtPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 1 Power      :   " & Mid(AP_1, 4, AP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 2 Power      :   " & Mid(BP_1, 4, BP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Pha 3 Power      :   " & Mid(CP_1, 4, CP_1L) & "W"
            FrmSMGE_3222M_ActPowerData.TxtPower.Text &= Environment.NewLine & "Total Power      :   " & Mid(TP_1, 4, TP_1L) & "W"



            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 1 Reactive Power :   " & Mid(AP_2, 4, AP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 2 Reactive Power :   " & Mid(BP_2, 4, BP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Pha 3 Reactive Power :   " & Mid(CP_2, 4, CP_2L) & "VAR"
            FrmSMGE_3222M_ReactPowerData.TxtReacPower.Text &= Environment.NewLine & "Total Reactive Power :   " & Mid(TP_2, 4, TP_2L) & "VAR"


            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 1 Apparent Power :   " & Mid(AP_3, 4, AP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 2 Apparent Power :   " & Mid(BP_3, 4, BP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Pha 3 Apparent Power :   " & Mid(CP_3, 4, CP_3L) & "VA"
            FrmSMGE_3222M_AppntPowerData.TxtApperantPower.Text &= Environment.NewLine & "Total Apparent Power :   " & Mid(TP_3, 4, TP_3L) & "VA"


            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 1 Power Factor :   " & Mid(AP_5, 4, AP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 2 Power Factor :   " & Mid(BP_5, 4, BP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Pha 3 Power Factor :   " & Mid(CP_5, 4, CP_5L)
            FrmSMGE_3222M_PowerFactorData.TxtPowerFactor.Text &= Environment.NewLine & "Total Power Factor :   " & Mid(TP_5, 4, TP_5L)


            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 1 Angle       :   " & Mid(AP_9, 4, AP_9L) & "°"
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 2 Angle       :   " & Mid(BP_9, 4, BP_9L) & "°"
            FrmSMGE_3222M_PhaseAngleData.TxtPhaseAngle.Text &= Environment.NewLine & "Pha 3 Angler      :   " & Mid(CP_9, 4, CP_9L) & "°"

            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & ""
            FrmSMGE_3222M_FrequencyData.TxtFrequency.Text &= Environment.NewLine & "Frequency   :   " & Mid(TP_8, 4, TP_8L) & "Hz"


            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss")
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "============================="
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Energy Imported"
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 1 Active Energy :   " & Mid(AE_1, 4, AE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 2 Active Energy :   " & Mid(BE_1, 4, BE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Pha 3 Active Energy :   " & Mid(CE_1, 4, CE_1L)
            FrmSMGE_3222M_ActiveEnergyImp.TxtActiveEnergyImp.Text &= Environment.NewLine & "Total Power Factor :   " & Mid(TE_1, 4, TE_1L)


            FrmSM_GE3222M_Power_Data.LblPhaA_ActivePowerVal.Text = Mid(AP_1, 4, AP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_ReactivePowerVal.Text = Mid(AP_2, 4, AP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaA_apparentpowerVal.Text = Mid(AP_3, 4, AP_3L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblPhaA_activefundamentalpowerVal.Text = Mid(AP_4, 4, AP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_powerfactorVal.Text = Mid(AP_5, 4, AP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaA_activeharmonicpowerVal.Text = Mid(AP_6, 4, AP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaA_voltageRMSVal.Text = Mid(AP_7, 4, AP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaA_currentRMSVal.Text = Mid(AP_8, 4, AP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaA_meanphaseangleVal.Text = Mid(AP_9, 4, AP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaA_voltagephaseangleVal.Text = Mid(AP_10, 5, AP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblPhaB_ActivePowerVal.Text = Mid(BP_1, 4, BP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_ReactivePowerVal.Text = Mid(BP_2, 4, BP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaB_apparentpowerVal.Text = Mid(BP_3, 4, BP_3L) & " VA"
            FrmSM_GE3222M_Power_Data.LblPhaB_activefundamentalpowerVal.Text = Mid(BP_4, 4, BP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_powerfactorVal.Text = Mid(BP_5, 4, BP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaB_activeharmonicpowerVal.Text = Mid(BP_6, 4, BP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaB_voltageRMSVal.Text = Mid(BP_7, 4, BP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaB_currentRMSVal.Text = Mid(BP_8, 4, BP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaB_meanphaseangleVal.Text = Mid(BP_9, 4, BP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaB_voltagephaseangleVal.Text = Mid(BP_10, 5, BP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblPhaC_ActivePowerVal.Text = Mid(CP_1, 4, CP_1L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_ReactivePowerVal.Text = Mid(CP_2, 4, CP_2L) & " VAR"
            FrmSM_GE3222M_Power_Data.LblPhaC_apparentpowerVal.Text = Mid(CP_3, 4, CP_3L) & " VA"
            FrmSM_GE3222M_Power_Data.LblPhaC_activefundamentalpowerVal.Text = Mid(CP_4, 4, CP_4L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_powerfactorVal.Text = Mid(CP_5, 4, CP_5L)
            FrmSM_GE3222M_Power_Data.LblPhaC_activeharmonicpowerVal.Text = Mid(CP_6, 4, CP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblPhaC_voltageRMSVal.Text = Mid(CP_7, 4, CP_7L) & " V"
            FrmSM_GE3222M_Power_Data.LblPhaC_currentRMSVal.Text = Mid(CP_8, 4, CP_8L) & " A"
            FrmSM_GE3222M_Power_Data.LblPhaC_meanphaseangleVal.Text = Mid(CP_9, 4, CP_9L) & " deg"
            FrmSM_GE3222M_Power_Data.LblPhaC_voltagephaseangleVal.Text = Mid(CP_10, 5, CP_10L) & " deg"

            FrmSM_GE3222M_Power_Data.LblTotal_PhasesumActivePowerVal.Text = Mid(TP_1, 4, TP_1L) & " kW"
            FrmSM_GE3222M_Power_Data.LblTotal_PhasesumReactivePowerVal.Text = Mid(TP_2, 4, TP_2L) & " kvar"
            FrmSM_GE3222M_Power_Data.LblTotal_ArithmeticSumapparentpowerVal.Text = Mid(TP_3, 4, TP_3L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblVectorSum_TotalApparentPowerVal.Text = Mid(TP_4, 4, TP_4L) & " kVA"
            FrmSM_GE3222M_Power_Data.LblTotalpowerfactorVal.Text = Mid(TP_5, 4, TP_5L)
            FrmSM_GE3222M_Power_Data.LblTotalactivefundamentalpowerVal.Text = Mid(TP_6, 4, TP_6L) & " W"
            FrmSM_GE3222M_Power_Data.LblTotalactiveharmonicpowerVal.Text = Mid(TP_7, 4, TP_7L) & " W"
            FrmSM_GE3222M_Power_Data.LblLblFrequency.Text = Mid(TP_8, 4, TP_8L) & " Hz"
            FrmSM_GE3222M_Power_Data.LblN_Line_SampledcurrentRMSVal.Text = Mid(TP_9, 4, TP_9L) & " A"
            FrmSM_GE3222M_Power_Data.LblTotalLine_currentRMSVal.Text = Mid(TP_10, 5, TP_10L) & " A"

            SerialPort.WriteLine("Req_End")
            'SerialPort.Close()

        Catch ex As Exception

        End Try


    End Sub

    Private Sub CmdReadData_Click(sender As Object, e As EventArgs) Handles CmdReadData.Click
        Me.Hide()
        FrmSM_GE3222M_Meter_Readings.Show()




    End Sub

    Private Sub FrmSMGE_3222MLoging_FormClosing(sender As Object, e As FormClosingEventArgs) Handles Me.FormClosing
        'Server.IsListening = False
    End Sub


End Class
