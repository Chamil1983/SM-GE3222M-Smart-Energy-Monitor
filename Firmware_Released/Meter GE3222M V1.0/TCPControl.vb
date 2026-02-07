Imports System.IO
Imports System.Net
Imports System.Net.Sockets
Imports System.Threading

Public Class TCPControl
    Public Event MessageReceived(sender As TCPControl, Data As String)

    Public ServerIP As IPAddress = IPAddress.Parse(FrmSMGE_3222MLoging.TxtServerIP.Text)
    Public ServerPort As Integer = FrmSMGE_3222MLoging.TxtTCPPort.Text
    Public Server As TcpListener

    Private CommThread As Thread
    Public IsListening As Boolean = True

    Private Client As TcpClient
    Private ClientData As StreamReader


    Public Sub New()

        Server = New TcpListener(ServerIP, ServerPort)
        Server.Start()

        CommThread = New Thread(New ThreadStart(AddressOf Listening))
        CommThread.Start()

    End Sub


    Private Sub Listening()

        Do Until IsListening = False
            If Server.Pending = True Then
                Client = Server.AcceptTcpClient
                ClientData = New StreamReader(Client.GetStream)
            End If

            Try
                RaiseEvent MessageReceived(Me, ClientData.ReadLine)


            Catch ex As Exception

            End Try

            Thread.Sleep(100)
        Loop
    End Sub

End Class

