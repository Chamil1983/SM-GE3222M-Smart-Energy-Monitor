Imports System.IO
Imports System.Data
Imports System.IO.Ports
Imports System.Net
Imports System.Net.Sockets
Imports Newtonsoft.Json
Imports Newtonsoft.Json.Linq
Imports Xenocode
Imports Meter_GE3222M_V1._0.ClsSettings



Public Class FrmSMGE_3222MSettings
    Dim Config As Configurations
    Public ConfigStr As String
    'Public TBox As New TextBox
    Public SaveConfig As Boolean = False
    Public Config_State As Boolean = False
    Public Config_Mode As String
    Dim Page_name As String
    Dim Ack1 As Boolean = False
    Dim AP_Mode As Boolean = False
    Dim Client_Mode As Boolean = False
    Dim WiFi_Mode As Boolean = False
    Dim WIFi_State As String = ""
    Public ETH_Mode As Boolean = False
    Public MDBUS_Mode As Boolean = False
    Public COM_MODE As String = ""



    Private Sub CmdCancel_Click(sender As Object, e As EventArgs) Handles CmdCancel.Click
        Me.Hide()
        FrmSMGE_3222MLoging.Show()
        FrmSMGE_3222MLoging.SetMode = False
        FrmSMGE_3222MLoging.TimerHandler1_Client.Start()

        FrmSMGE_3222MLoging.TimerMonitor.Start()
        FrmSMGE_3222MLoging.TimerMonitor.Enabled = True
        FrmSMGE_3222MLoging.TimerCommands.Enabled = False
        FrmSMGE_3222MLoging.TimerCommands.Stop()
        FrmSMGE_3222MLoging.TimerSync.Enabled = False
        FrmSMGE_3222MLoging.TimerSync.Stop()
        '==============================================
        'FrmSMGE_3222MLoging.TimerMonitor.Stop()
    End Sub

    Private Sub FrmSMGE_3222MSettings_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        WiFiDisable()
        WiFi_Mode = False
        'CmdSave.Enabled = False

        TxtServerIP.Text = FrmSMGE_3222MLoging.TxtServerIP.Text

    End Sub

    Private Sub CmdSave_Click(sender As Object, e As EventArgs) Handles CmdSave.Click

        FrmSMGE_3222MLoging.TimerSync.Enabled = False
        FrmSMGE_3222MLoging.TimerSync.Stop()
        FrmSMGE_3222MLoging.TxtComTerm.Clear()

        Dim intResponse As Integer


        intResponse = MsgBox("Are you sure you want to save settings?", vbYesNo + vbQuestion, "New Settings Save")




        If intResponse = vbYes Then

            CmdSave.Enabled = False
            SaveConfig = True

            'TimerHandler1_Client.Start()
            SendConfigToHost()


        End If






    End Sub


    Function SendConfigToHost()

        Dim SetState As Boolean = FrmSMGE_3222MLoging.SetMode

        If SetState = True Then

            Try


                Config = New Configurations

                If ChkSerial.Checked = True Then

                    COM_MODE = "USB"

                ElseIf ChkEthernetMode.Checked = True Then

                    COM_MODE = "TCP"

                    ' ElseIf (FrmSMGE_3222MLoging.OptTCPIP.Checked = True AndAlso ChkSerial.Checked = True AndAlso ChkEthernetMode.Checked = True) Then

                    'COM_MODE = "USB/TCP"

                    'ElseIf ChkSerial.Checked = True AndAlso FrmSMGE_3222MLoging.OptSerialConnect.Checked = True AndAlso ChkEthernetMode.Checked = True Then

                    ' COM_MODE = "USB/TCP"


                ElseIf FrmSMGE_3222MLoging.ChkTCPModbus.Checked = True Or ChkModbusMode.Checked = True Then

                    COM_MODE = "MODBUS"

                Else
                    COM_MODE = "NONE"
                End If

                Config.Page_Name = "Config"
                Config.COM_MODE = COM_MODE
                Config.Config_State = True
                Config.SerialBaud = ComboBoxBaudRate.SelectedItem
                Config.WIFi_Mode = ChkWiFiState.CheckState
                Config.WIFi_State = ChkWiFiState.Text
                Config.WIFiClient_Mode = Client_Mode
                Config.WiFiAP_Mode = AP_Mode
                Config.WIFiSSID = TxtSSID.Text
                Config.WiFiPassword = TxtPassword.Text
                Config.Server_IP = TxtServerIP.Text
                Config.WiFiHost_IP = TxtLocalIPWiFi.Text
                Config.WiFiServerPort = TxtServerPortWiFi.Text
                Config.WiFiGateway = TxtGateway.Text
                Config.WiFiSubnet = TxtSubnet.Text
                Config.DNS1 = TxtDNSPrimary.Text
                Config.DNS2 = TxtDNSSecondary.Text
                Config.WiFiSoftAP_IP = TxtSoftAPIP.Text
                Config.SoftAP_SSID = TxtSoftAPSSID.Text
                Config.SoftAP_Password = TxtSoftAP_Password.Text
                Config.ETH_Mode = ETH_Mode
                Config.ETH_HostIP = TxtLocalIPEthernet.Text
                Config.ETH_TCPPort = TxtServerPortEthernet.Text
                Config.MDBUS_Mode = MDBUS_Mode
                Config.MDBUS_ID = TxtSlaveID.Text
                Config.MDBus_TCPHostIP = TxtIPModbus.Text
                Config.MDBus_TCPPort = TxtPortNoModbus.Text
                Config.MDBUS_BAUD = ComboBoxModbus_BaudRate.SelectedItem
                Config.Publish_Interval = TxtPublishInt.Text
                Config.Config_Mode = "Saving"

                ConfigStr = JsonConvert.SerializeObject(Config, Formatting.Indented)
                'MsgBox(ConfigStr)

                If FrmSMGE_3222MLoging.SerialPort.IsOpen = False Then
                    Dim intResponse As Integer

                    intResponse = MsgBox("Do you want to reset the Meter?", vbYesNo + vbQuestion, "Reset Meter")



                    If intResponse = vbNo Then

                        Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, "ConfigSave" + vbLf)

                    ElseIf intResponse = vbYes Then

                        Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, "ConfigRst" + vbLf)

                    End If
                End If


                'If String.Equals(COM_MODE, "TCP") Then

                'End If




                If String.IsNullOrEmpty(FrmSMGE_3222MLoging.TxtComTerm.Text) Then
                    If FrmSMGE_3222MLoging.SerialPort.IsOpen Then
                        FrmSMGE_3222MLoging.SerialPort.WriteLine("ConfigSave")

                    End If


                Else
                    FrmSMGE_3222MLoging.TxtComTerm.Clear()
                    'FrmSMGE_3222MLoging.SerialPort.WriteLine("ConfigSave")
                    'FrmSMGE_3222MLoging.TimerCommands.Enabled = True
                    'FrmSMGE_3222MLoging.TimerCommands.Start()
                End If



                'File.WriteAllText("C:\Users\chami\Documents\Arduino\FileSys\ConfigESPTCP\data\config.txt", JsonConvert.SerializeObject(Config, Formatting.Indented))
                File.WriteAllText("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\config.txt", JsonConvert.SerializeObject(Config, Formatting.Indented))




                'If String.Equals(COM_MODE, "TCP") Then
                Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, ConfigStr + vbLf)
                'End If

                If FrmSMGE_3222MLoging.SerialPort.IsOpen Then
                    FrmSMGE_3222MLoging.SerialPort.WriteLine(ConfigStr + vbLf)
                    FrmSMGE_3222MLoging.TimerCommands.Enabled = True
                    FrmSMGE_3222MLoging.TimerCommands.Start()
                    '    'MsgBox(FrmSMGE_3222MLoging.SerialPort.ReadExisting(ConfigStr + vbLf))

                End If

                'End If




                'Threading.ThreadPool.QueueUserWorkItem(AddressOf ReadString)

            Catch ex As Exception


            Finally

                'Staying on TCP conncetion
                If FrmSMGE_3222MLoging.SerialPort.IsOpen = False Then
                    If String.Equals(COM_MODE, "TCP") Then
                        FrmSMGE_3222MLoging.Show()
                        FrmSMGE_3222MLoging.SetMode = False
                        FrmSMGE_3222MLoging.TimerHandler1_Client.Start()

                        FrmSMGE_3222MLoging.TimerMonitor.Start()
                        FrmSMGE_3222MLoging.TimerMonitor.Enabled = True
                        FrmSMGE_3222MLoging.TimerCommands.Enabled = False
                        FrmSMGE_3222MLoging.TimerCommands.Stop()
                        FrmSMGE_3222MLoging.TimerSync.Enabled = False
                        FrmSMGE_3222MLoging.TimerSync.Stop()
                        Me.Close()

                    End If

                End If



                'Connection request to USB port if not connected
                If FrmSMGE_3222MLoging.SerialPort.IsOpen = False Then

                    If String.Equals(COM_MODE, "USB") Then
                        MsgBox("Please Connect to Serial Port!", MessageBoxIcon.Information, "USB Connection")
                        Me.Close()
                    End If
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






        'SendConfigToHost()

        'Dim Config As New Configurations
        'Dim JSerilizer As New JsonSerializer()
        'Threading.ThreadPool.QueueUserWorkItem(AddressOf TCP.SendToClients, "ConfigSave" + vbLf)
        'Try

        '    Dim JString As String = TBox.Text

        '    Dim Configurations = JsonConvert.DeserializeObject(Of MeterSettings)(JString)

        '    Label1.Text = Configurations.ConfigMode
        '    Label2.Text = Configurations.ETHHostIP
        '    Label3.Text = Configurations.ETHTCPPort
        '    Label4.Text = Configurations.MDBusTCPHostIP

        '    Dim ConfigState As Boolean = Config.Config_State
        '    Dim Serial_Boad As Integer = Config.SerialBaud
        '    Dim WIFi_SSID As String = Config.WIFiSSID
        '    Dim WiFi_Password As String = Config.WiFiPassword
        '    Dim ServerIP As String = Config.Server_IP
        '    Dim WiFiHostIP As String = Config.WiFiHost_IP
        '    Dim WiFiServerPort As Integer = Config.WiFiServerPort
        '    Dim WiFiGateway As String = Config.WiFiGateway
        '    Dim WiFiSubnet As String = Config.WiFiSubnet
        '    Dim DNS1 As String = Config.DNS1
        '    Dim DNS2 As String = Config.DNS2
        '    Dim ETH_HostIP As String = Config.ETH_HostIP
        '    Dim ETH_TCPPort As Integer = Config.WiFiPassword
        '    Dim MDBUS_ID As Integer = Config.MDBUS_ID
        '    Dim MDBus_TCPHostIP As String = Config.MDBus_TCPHostIP
        '    Dim MDBus_TCPPort As Integer = Config.MDBus_TCPPort
        '    Dim MDBUS_BAUD As Integer = Config.MDBUS_BAUD
        '    Dim Publish_Interval As Integer = Config.Publish_Interval
        '    Dim Config_Mode As String = Config.Config_Mode




        '    TCP.Refresh_Form()
        'Catch ex As Exception

        'End Try



    End Sub



    Private Sub CmdLoad_Click(sender As Object, e As EventArgs) Handles CmdLoad.Click
        Dim Settings As New Configurations

        Dim SRSerial As StreamReader
        SRSerial = New StreamReader("C:\Users\chami\Documents\Arduino\SM_GE3222M V1.0\SM_GE3222M V1.0\data\NewConfig.txt")
        Dim ConfigSerializer As New JsonSerializer()
        Settings = ConfigSerializer.Deserialize(SRSerial, Settings.GetType)
        SRSerial.Close()

        WiFiEnable()

        Page_name = Settings.Page_Name
        COM_MODE = Settings.COM_MODE

        If String.Equals(COM_MODE, "USB") Then
            ChkSerial.Checked = True


        Else
            ChkSerial.Checked = False


        End If

        Config_State = Settings.Config_State
        GetBaud(Settings.SerialBaud, ComboBoxBaudRate)
        WiFi_Mode = Settings.WIFi_Mode

        WIFi_State = Settings.WIFi_State


        'If WIFi_State = "Enable" Then
        '    ChkWiFiState.Checked = True

        'Else

        '    ChkWiFiState.Checked = False
        'End If
        'If String.Compare(WiFi_Mode, "ON", True) Then
        '    ChkWiFiState.Checked = True
        '    ChkWiFiState.Text = "Enable"
        'Else
        '    ChkWiFiState.Checked = False
        '    ChkWiFiState.Text = "Disable"

        'End If

        If String.Equals(WIFi_State, "ON") Then
            ChkWiFiState.Checked = True


        Else
            ChkWiFiState.Checked = False


        End If


        Client_Mode = Settings.WIFiClient_Mode

        If Client_Mode = True Then
            RdoClient.Checked = True

        Else

            RdoClient.Checked = False
        End If

        TxtSSID.Text = Settings.WIFiSSID
        TxtPassword.Text = Settings.WiFiPassword
        TxtServerIP.Text = Settings.Server_IP
        TxtLocalIPWiFi.Text = Settings.WiFiHost_IP
        TxtServerPortWiFi.Text = Settings.WiFiServerPort
        TxtGateway.Text = Settings.WiFiGateway
        TxtSubnet.Text = Settings.WiFiSubnet
        TxtDNSPrimary.Text = Settings.DNS1
        TxtDNSSecondary.Text = Settings.DNS2
        AP_Mode = Settings.WiFiAP_Mode

        If AP_Mode = True Then
            RdoAPMode.Checked = True

        Else
            RdoAPMode.Checked = False
        End If
        TxtSoftAPIP.Text = Settings.WiFiSoftAP_IP
        TxtSoftAPSSID.Text = Settings.SoftAP_SSID
        TxtSoftAP_Password.Text = Settings.SoftAP_Password
        ETH_Mode = Settings.ETH_Mode

        If ETH_Mode = True Then
            ChkEthernetMode.Checked = True

        Else
            ChkEthernetMode.Checked = False
        End If


        TxtLocalIPEthernet.Text = Settings.ETH_HostIP
        TxtServerPortEthernet.Text = Settings.ETH_TCPPort
        MDBUS_Mode = Settings.MDBUS_Mode

        If MDBUS_Mode = True Then
            ChkModbusMode.Checked = True

        Else
            ChkModbusMode.Checked = False
        End If

        TxtSlaveID.Text = Settings.MDBUS_ID
        TxtIPModbus.Text = Settings.MDBus_TCPHostIP
        TxtPortNoModbus.Text = Settings.MDBus_TCPPort
        GetBaud(Settings.MDBUS_BAUD, ComboBoxModbus_BaudRate)
        TxtPublishInt.Text = Settings.Publish_Interval
        Config_Mode = Settings.Config_Mode



    End Sub

    Function GetBaud(ByVal Baud As Integer, ByRef CBox As ComboBox)

        Select Case Baud

            Case 1200
                CBox.SelectedIndex = 0

            Case 2400
                CBox.SelectedIndex = 1
            Case 4800
                CBox.SelectedIndex = 2

            Case 9600
                CBox.SelectedIndex = 3
            Case 19200
                CBox.SelectedIndex = 4
            Case 38400
                CBox.SelectedIndex = 5
            Case 57600
                CBox.SelectedIndex = 6
            Case 115200
                CBox.SelectedIndex = 7

            Case Else
                CBox.SelectedIndex = -1

        End Select

        Return CBox
    End Function


    Private Sub TxtPublishInt_KeyPress(sender As Object, e As KeyPressEventArgs) Handles TxtPublishInt.KeyPress

        '****To Allow only Numbers with Decimal and BACKSPACE enabled****
        If Not (Char.IsDigit(e.KeyChar) Or e.KeyChar = ".") And Not Char.IsControl(e.KeyChar) Then
            e.Handled = True
        End If
    End Sub



    Private Sub RdoAPMode_CheckedChanged(sender As Object, e As EventArgs) Handles RdoAPMode.CheckedChanged
        Try
            If RdoAPMode.Checked = True Then
                AP_Mode = True
                Client_Mode = False
                TxtSoftAPIP.Enabled = True
                TxtSoftAPSSID.Enabled = True
                TxtSoftAP_Password.Enabled = True
                Client_Mode = False
                TxtSSID.Enabled = False
                TxtLocalIPWiFi.Enabled = False
                TxtGateway.Enabled = False
                TxtSubnet.Enabled = False
                TxtPassword.Enabled = False
                TxtServerPortWiFi.Enabled = False
                TxtDNSPrimary.Enabled = False
                TxtDNSSecondary.Enabled = False

                'Else
                '    AP_Mode = False
                '    TxtSoftAPIP.Enabled = False
                '    TxtSoftAPSSID.Enabled = False
                '    TxtSoftAP_Password.Enabled = False
                '    Client_Mode = True
                '    TxtSSID.Enabled = True
                '    TxtLocalIPWiFi.Enabled = True
                '    TxtGateway.Enabled = True
                '    TxtSubnet.Enabled = True
                '    TxtPassword.Enabled = True
                '    TxtServerPortWiFi.Enabled = True
                '    TxtDNSPrimary.Enabled = True
                '    TxtDNSSecondary.Enabled = True
            End If
        Catch ex As Exception

        End Try
    End Sub

    Private Sub RdoClient_CheckedChanged(sender As Object, e As EventArgs) Handles RdoClient.CheckedChanged
        Try
            If RdoClient.Checked = True Then
                Client_Mode = True
                AP_Mode = False
                TxtSSID.Enabled = True
                TxtLocalIPWiFi.Enabled = True
                TxtGateway.Enabled = True
                TxtSubnet.Enabled = True
                TxtPassword.Enabled = True
                TxtServerPortWiFi.Enabled = True
                TxtDNSPrimary.Enabled = True
                TxtDNSSecondary.Enabled = True
                TxtSoftAPIP.Enabled = False
                TxtSoftAPSSID.Enabled = False
                TxtSoftAP_Password.Enabled = False

                'Else
                '    Client_Mode = False
                '    TxtSSID.Enabled = False
                '    TxtLocalIPWiFi.Enabled = False
                '    TxtGateway.Enabled = False
                '    TxtSubnet.Enabled = False
                '    TxtPassword.Enabled = False
                '    TxtServerPortWiFi.Enabled = False
                '    TxtDNSPrimary.Enabled = False
                '    TxtDNSSecondary.Enabled = False
                '    TxtSoftAPIP.Enabled = True
                '    TxtSoftAPSSID.Enabled = True
                '    TxtSoftAP_Password.Enabled = True
            End If
        Catch ex As Exception

        End Try
    End Sub

    Private Sub ChkWiFiState_CheckedChanged(sender As Object, e As EventArgs) Handles ChkWiFiState.CheckedChanged


        If ChkWiFiState.Checked = True Then

            WiFi_Mode = True
            'LblWiFiMode.Text = "Enable"
            WiFiEnable()
            ChkWiFiState.Text = "ON"
            'RdoClient.Checked = True


        End If

        If ChkWiFiState.Checked = False Then
            WiFi_Mode = False
            'LblWiFiMode.Text = "Disable"
            ChkWiFiState.Text = "OFF"
            RdoClient.Checked = False
            RdoAPMode.Checked = False
            Client_Mode = False
            AP_Mode = False
            WiFiDisable()
        End If





    End Sub

    Function WiFiEnable()

        TxtSSID.Enabled = True
        TxtLocalIPWiFi.Enabled = True
        TxtGateway.Enabled = True
        TxtSubnet.Enabled = True
        TxtPassword.Enabled = True
        TxtServerPortWiFi.Enabled = True
        TxtDNSPrimary.Enabled = True
        TxtDNSSecondary.Enabled = True
        TxtSoftAPIP.Enabled = True
        TxtSoftAPSSID.Enabled = True
        TxtSoftAP_Password.Enabled = True
        RdoAPMode.Enabled = True
        RdoClient.Enabled = True

        Return True
    End Function

    Function WiFiDisable()

        TxtSSID.Enabled = False
        TxtLocalIPWiFi.Enabled = False
        TxtGateway.Enabled = False
        TxtSubnet.Enabled = False
        TxtPassword.Enabled = False
        TxtServerPortWiFi.Enabled = False
        TxtDNSPrimary.Enabled = False
        TxtDNSSecondary.Enabled = False
        TxtSoftAPIP.Enabled = False
        TxtSoftAPSSID.Enabled = False
        TxtSoftAP_Password.Enabled = False
        RdoAPMode.Enabled = False
        RdoClient.Enabled = False

        Return True
    End Function

    Private Sub ChkLblEthernetMode_CheckedChanged(sender As Object, e As EventArgs) Handles ChkEthernetMode.CheckedChanged
        If ChkEthernetMode.Checked = True Then

            ETH_Mode = True
            MDBUS_Mode = False
            'LblWiFiMode.Text = "Enable"
            GrpModbusSetting.Enabled = False
            ChkEthernetMode.Text = "ON"
            ChkSerial.Enabled = False
            ChkSerial.Checked = False
            'ChkSerial.Text = "OFF"
            'RdoClient.Checked = True

        Else




        End If

        If ChkEthernetMode.Checked = False Then
            ETH_Mode = False
            ChkSerial.Enabled = True
            'LblWiFiMode.Text = "Disable"
            If FrmSMGE_3222MLoging.OptTCPIP.Checked = False Then
                GrpModbusSetting.Enabled = True
            End If

            ChkEthernetMode.Text = "OFF"

        End If
    End Sub

    Private Sub ChkModbusMode_CheckedChanged(sender As Object, e As EventArgs) Handles ChkModbusMode.CheckedChanged
        If ChkModbusMode.Checked = True Then

            MDBUS_Mode = True
            ETH_Mode = False
            'LblWiFiMode.Text = "Enable"
            GrpEthernetSettings.Enabled = False
            ChkModbusMode.Text = "ON"
            'RdoClient.Checked = True


        End If

        If ChkModbusMode.Checked = False Then
            ETH_Mode = False
            'LblWiFiMode.Text = "Disable"
            GrpEthernetSettings.Enabled = True
            ChkModbusMode.Text = "OFF"

        End If
    End Sub

    Private Sub ChkSerial_CheckedChanged(sender As Object, e As EventArgs) Handles ChkSerial.CheckedChanged
        If ChkSerial.Checked = True Then

            'COM_MODE = "USB"
            'LblWiFiMode.Text = "Enable"

            ChkSerial.Text = "ON"
            ChkEthernetMode.Enabled = False
            ChkEthernetMode.Checked = False
            'ChkEthernetMode.Text = "OFF"
            'RdoClient.Checked = True

        Else
            ChkSerial.Text = "OFF"
            ChkEthernetMode.Enabled = True


        End If




    End Sub

    Private Sub TimerCommands_Tick(sender As Object, e As EventArgs)
        Dim StrTerminal As String


        Try
            FrmSMGE_3222MLoging.SerialPort.WriteLine("ConfigSave" + vbLf)

            FrmSMGE_3222MLoging.TxtComTerm.Text = FrmSMGE_3222MLoging.SerialPort.ReadExisting
            StrTerminal = FrmSMGE_3222MLoging.TxtComTerm.Text

            If StrTerminal.Contains("/SYNC/") Then


                'TBox.Clear()
                FrmSMGE_3222MLoging.TxtComTerm.Clear()


                FrmSMGE_3222MLoging.SerialPort.WriteLine(ConfigStr + vbLf)

            End If

        Catch ex As Exception

        End Try






    End Sub


End Class



