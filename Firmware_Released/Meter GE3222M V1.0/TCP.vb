Imports System.IO
Imports System.Data
Imports System.IO.Ports
Imports System.Net
Imports System.Net.Sockets
Imports Newtonsoft.Json
Imports Newtonsoft.Json.Linq


Module TCP

    Public ServerStatus As Boolean = False
    Public ServerTrying As Boolean = False
    Public Server As TcpListener
    Public Clients As New List(Of TcpClient)
    Public Client As TcpClient
    Public TBox As New TextBox

    Public Function StartServer()
        If ServerStatus = False Then
            ServerTrying = True
            Try
                Server = New TcpListener(IPAddress.Parse(FrmSMGE_3222MLoging.TxtServerIP.Text), FrmSMGE_3222MLoging.TxtTCPPort.Text)
                Server.Start()
                ServerStatus = True
                FrmSMGE_3222MLoging.TimerHandler1_Client.Enabled = True
                FrmSMGE_3222MLoging.TimerHandler1_Client.Start()
                'TimerHandler2_Client.Enabled = True
                'TimerHandler2_Client.Start()
                FrmSMGE_3222MLoging.ProgressBarDataRead.Value = 0


                FrmSMGE_3222MLoging.TimerRefreshForm.Start()
                FrmSMGE_3222MLoging.LblEthernetStatus.Text = "Status : Connected"
                FrmSMGE_3222MLoging.PictureBoxConnectionIndTCP.Image = My.Resources.Green
                Threading.ThreadPool.QueueUserWorkItem(AddressOf Handler_Client)
            Catch ex As Exception
                ServerStatus = False
            End Try
            ServerTrying = False
        End If
        Return True
    End Function


    Public Function StopServer()

        If ServerStatus = True Then
            ServerTrying = True
            Try
                For Each Client As TcpClient In Clients
                    Client.Close()
                Next
                Server.Stop()
                FrmSMGE_3222MLoging.LblEthernetStatus.Text = "Status : Disconnect"
                FrmSMGE_3222MLoging.PictureBoxConnectionIndTCP.Image = My.Resources.Red
                FrmSMGE_3222MLoging.TimerRefreshForm.Stop()
                ServerStatus = False
                FrmSMGE_3222MLoging.TimerHandler1_Client.Enabled = False
                FrmSMGE_3222MLoging.TimerHandler1_Client.Stop()
                'TimerHandler2_Client.Enabled = False
                'TimerHandler2_Client.Stop()



            Catch ex As Exception
                StopServer()
            End Try
            ServerTrying = False
        End If
        Return True

        Return True
    End Function

    Public Function Handler_Client(ByVal state As Object)
        Dim TempClient As TcpClient = Nothing

        Try
            Using Client As TcpClient = Server.AcceptTcpClient
                ''     Console.Beep()

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


                                REM For some reason this seems to stop the comon tcp connection bug vvv
                                Dim RawDataLength As String = RawData.Length.ToString
                                REM ^^^^ Comment it out and test it in your own projects. Mine might be the only stupid one.
                                'Dim TB As TextBox
                                'TB.Multiline = True


                                TBox.Text += RawData + vbNewLine

                                'Dim Config As New Configurations
                                'Config = JsonConvert.DeserializeObject(RawData, Config.GetType)

                                'File.WriteAllText("C:\Users\chami\Documents\Arduino\FileSys\ConfigESPTCP\data\config.txt", JsonConvert.SerializeObject(Config, Formatting.Indented))

                            Else Exit While
                            End If
                        End While
                    End If
                Catch ex As Exception
                    If Clients.Contains(Client) Then
                        Clients.Remove(Client)
                        Client.Close()
                    End If


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
        End Try

        Return True
    End Function

    Public Function SendToClients(ByVal Data As String)
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

    Public Function Handle_Receive(ByVal state As Object)
        Dim TempClient As TcpClient = Nothing

        Try
            Using Client As TcpClient = Server.AcceptTcpClient
                ''     Console.Beep()

                If ServerTrying = False Then
                    Threading.ThreadPool.QueueUserWorkItem(AddressOf Handle_Receive)
                End If

                Clients.Add(Client)
                TempClient = Client

                'Dim TX As New StreamWriter(Client.GetStream)
                Dim RX As New StreamReader(Client.GetStream)

                Dim Config As New Configurations
                Try
                    If RX.BaseStream.CanRead = True Then
                        While RX.BaseStream.CanRead = True


                            Dim RawData As String = RX.ReadLine
                            If Client.Client.Connected = True AndAlso Client.Connected = True AndAlso Client.GetStream.CanRead = True Then


                                REM For some reason this seems to stop the comon tcp connection bug vvv
                                Dim RawDataLength As String = RawData.Length.ToString
                                REM ^^^^ Comment it out and test it in your own projects. Mine might be the only stupid one.
                                'Dim TB As TextBox
                                'TB.Multiline = True

                                Config = JsonConvert.DeserializeObject(RawData, Config.GetType)

                                Dim ConfigState As Boolean = Config.Config_State

                                Dim Serial_Boad As Integer = Config.SerialBaud
                                Dim WIFi_SSID As String = Config.WIFiSSID
                                Dim WiFi_Password As String = Config.WiFiPassword
                                Dim ServerIP As String = Config.Server_IP
                                Dim WiFiHostIP As String = Config.WiFiHost_IP
                                Dim WiFiServerPort As Integer = Config.WiFiServerPort
                                Dim WiFiGateway As String = Config.WiFiGateway
                                Dim WiFiSubnet As String = Config.WiFiSubnet
                                Dim DNS1 As String = Config.DNS1
                                Dim DNS2 As String = Config.DNS2

                                Dim ETH_HostIP As String = Config.ETH_HostIP
                                Dim ETH_TCPPort As Integer = Config.WiFiPassword
                                Dim MDBUS_ID As Integer = Config.MDBUS_ID
                                Dim MDBus_TCPHostIP As String = Config.MDBus_TCPHostIP
                                Dim MDBus_TCPPort As Integer = Config.MDBus_TCPPort

                                Dim MDBUS_BAUD As Integer = Config.MDBUS_BAUD
                                Dim Publish_Interval As Integer = Config.Publish_Interval
                                Dim Config_Mode As String = Config.Config_Mode


                                'Config.Config_State = ConfigState
                                'Config.SerialBaud = FrmSMGE_3222MSettings.ComboBoxBaudRate.SelectedItem
                                'Config.WIFiSSID = FrmSMGE_3222MSettings.TxtSSID.Text
                                'Config.WiFiPassword = FrmSMGE_3222MSettings.TxtPassword.Text
                                'Config.Server_IP = FrmSMGE_3222MLoging.TxtServerIP.Text
                                'Config.WiFiHost_IP = FrmSMGE_3222MSettings.TxtLocalIPWiFi.Text
                                'Config.WiFiServerPort = FrmSMGE_3222MSettings.TxtServerPortWiFi.Text
                                'Config.WiFiGateway = FrmSMGE_3222MSettings.TxtGateway.Text
                                'Config.WiFiSubnet = FrmSMGE_3222MSettings.TxtSubnet.Text
                                'Config.DNS1 = FrmSMGE_3222MSettings.TxtDNSPrimary.Text
                                'Config.DNS2 = FrmSMGE_3222MSettings.TxtDNSSecondary.Text
                                'Config.ETH_HostIP = FrmSMGE_3222MSettings.TxtLocalIPEthernet.Text
                                'Config.ETH_TCPPort = FrmSMGE_3222MSettings.TxtServerPortEthernet.Text
                                'Config.MDBUS_ID = FrmSMGE_3222MSettings.TxtSlaveID.Text
                                'Config.MDBus_TCPHostIP = FrmSMGE_3222MSettings.TxtIPModbus.Text
                                'Config.MDBus_TCPPort = FrmSMGE_3222MSettings.TxtPortNoModbus.Text
                                'Config.MDBUS_BAUD = FrmSMGE_3222MSettings.ComboBoxModbus_BaudRate.SelectedItem
                                'Config.Publish_Interval = Publish_Interval
                                'Config.Config_Mode = Config_Mode





                                Dim StrReceived As String
                                StrReceived = JsonConvert.SerializeObject(Config, Formatting.Indented)

                                MsgBox(StrReceived)

                                File.WriteAllText("C:\Users\chami\Documents\Arduino\FileSys\ConfigESPTCP\data\config.txt", JsonConvert.SerializeObject(Config, Formatting.Indented))

                            Else Exit While
                            End If
                        End While
                    End If
                Catch ex As Exception
                    If Clients.Contains(Client) Then
                        Clients.Remove(Client)
                        Client.Close()
                    End If


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
        End Try

        Return True
    End Function

    Public Sub Refresh_Form()
        TBox.Clear()
        FrmSMGE_3222MLoging.TxtComTerm.Clear()

    End Sub
End Module
