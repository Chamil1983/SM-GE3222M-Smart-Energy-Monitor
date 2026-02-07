<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class FrmSMGE_3222MSettings
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.GrpTCPSettings = New System.Windows.Forms.GroupBox()
        Me.GrpWiFiSettings = New System.Windows.Forms.GroupBox()
        Me.ChkWiFiState = New System.Windows.Forms.CheckBox()
        Me.PnlWiFiSettings = New System.Windows.Forms.Panel()
        Me.RdoAPMode = New System.Windows.Forms.RadioButton()
        Me.RdoClient = New System.Windows.Forms.RadioButton()
        Me.LblWiFiClient = New System.Windows.Forms.Label()
        Me.LblWiFiAPMode = New System.Windows.Forms.Label()
        Me.TxtSoftAPIP = New System.Windows.Forms.TextBox()
        Me.LblSoftAPIP = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.TxtSoftAPSSID = New System.Windows.Forms.TextBox()
        Me.TxtSoftAP_Password = New System.Windows.Forms.TextBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.TxtDNSSecondary = New System.Windows.Forms.TextBox()
        Me.LblDNSSecondary = New System.Windows.Forms.Label()
        Me.TxtDNSPrimary = New System.Windows.Forms.TextBox()
        Me.LblDNSPrimary = New System.Windows.Forms.Label()
        Me.TxtSubnet = New System.Windows.Forms.TextBox()
        Me.LblSubnet = New System.Windows.Forms.Label()
        Me.TxtGateway = New System.Windows.Forms.TextBox()
        Me.LblGateway = New System.Windows.Forms.Label()
        Me.LblWiFiServerPort = New System.Windows.Forms.Label()
        Me.TxtLocalIPWiFi = New System.Windows.Forms.TextBox()
        Me.TxtServerPortWiFi = New System.Windows.Forms.TextBox()
        Me.LblLocalIP = New System.Windows.Forms.Label()
        Me.LblPassword = New System.Windows.Forms.Label()
        Me.TxtSSID = New System.Windows.Forms.TextBox()
        Me.TxtPassword = New System.Windows.Forms.TextBox()
        Me.LblSSID = New System.Windows.Forms.Label()
        Me.LblWiFiMode = New System.Windows.Forms.Label()
        Me.GrpEthernetSettings = New System.Windows.Forms.GroupBox()
        Me.ChkEthernetMode = New System.Windows.Forms.CheckBox()
        Me.PnlEthernetSettings = New System.Windows.Forms.Panel()
        Me.TxtServerIP = New System.Windows.Forms.TextBox()
        Me.LblIPAddress = New System.Windows.Forms.Label()
        Me.LblServerPort = New System.Windows.Forms.Label()
        Me.TxtLocalIPEthernet = New System.Windows.Forms.TextBox()
        Me.TxtServerPortEthernet = New System.Windows.Forms.TextBox()
        Me.LblETHIP = New System.Windows.Forms.Label()
        Me.LblEthernetMode = New System.Windows.Forms.Label()
        Me.Panel2 = New System.Windows.Forms.Panel()
        Me.TxtPublishInt = New System.Windows.Forms.TextBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.LblPublishIntervel = New System.Windows.Forms.Label()
        Me.CmdLoad = New System.Windows.Forms.Button()
        Me.GrpModbusSetting = New System.Windows.Forms.GroupBox()
        Me.LblModbusMode = New System.Windows.Forms.Label()
        Me.ChkModbusMode = New System.Windows.Forms.CheckBox()
        Me.PnlModbus = New System.Windows.Forms.Panel()
        Me.ComboBoxModbus_BaudRate = New System.Windows.Forms.ComboBox()
        Me.LblSlaveID = New System.Windows.Forms.Label()
        Me.TxtPortNoModbus = New System.Windows.Forms.TextBox()
        Me.LblModbusBaudrate = New System.Windows.Forms.Label()
        Me.LblMDbusIP = New System.Windows.Forms.Label()
        Me.LblMDBusPort = New System.Windows.Forms.Label()
        Me.TxtIPModbus = New System.Windows.Forms.TextBox()
        Me.TxtSlaveID = New System.Windows.Forms.TextBox()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.ChkSerial = New System.Windows.Forms.CheckBox()
        Me.ComboBoxBaudRate = New System.Windows.Forms.ComboBox()
        Me.LblBaudRate = New System.Windows.Forms.Label()
        Me.LblSerial = New System.Windows.Forms.Label()
        Me.CmdSave = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.PictureBox1 = New System.Windows.Forms.PictureBox()
        Me.LblStatus = New System.Windows.Forms.Label()
        Me.TimerHandler1_Client = New System.Windows.Forms.Timer(Me.components)
        Me.GrpTCPSettings.SuspendLayout()
        Me.GrpWiFiSettings.SuspendLayout()
        Me.PnlWiFiSettings.SuspendLayout()
        Me.GrpEthernetSettings.SuspendLayout()
        Me.PnlEthernetSettings.SuspendLayout()
        Me.Panel2.SuspendLayout()
        Me.GrpModbusSetting.SuspendLayout()
        Me.PnlModbus.SuspendLayout()
        Me.Panel1.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'GrpTCPSettings
        '
        Me.GrpTCPSettings.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.GrpTCPSettings.Controls.Add(Me.GrpWiFiSettings)
        Me.GrpTCPSettings.Controls.Add(Me.GrpEthernetSettings)
        Me.GrpTCPSettings.Controls.Add(Me.Panel2)
        Me.GrpTCPSettings.Controls.Add(Me.CmdLoad)
        Me.GrpTCPSettings.Controls.Add(Me.GrpModbusSetting)
        Me.GrpTCPSettings.Controls.Add(Me.Panel1)
        Me.GrpTCPSettings.Controls.Add(Me.CmdSave)
        Me.GrpTCPSettings.Controls.Add(Me.CmdCancel)
        Me.GrpTCPSettings.Controls.Add(Me.PictureBox1)
        Me.GrpTCPSettings.Controls.Add(Me.LblStatus)
        Me.GrpTCPSettings.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpTCPSettings.Location = New System.Drawing.Point(9, 9)
        Me.GrpTCPSettings.Name = "GrpTCPSettings"
        Me.GrpTCPSettings.Size = New System.Drawing.Size(530, 680)
        Me.GrpTCPSettings.TabIndex = 2
        Me.GrpTCPSettings.TabStop = False
        Me.GrpTCPSettings.Text = "Ethernet / Modbus / WiFi Connection"
        '
        'GrpWiFiSettings
        '
        Me.GrpWiFiSettings.Controls.Add(Me.ChkWiFiState)
        Me.GrpWiFiSettings.Controls.Add(Me.PnlWiFiSettings)
        Me.GrpWiFiSettings.Controls.Add(Me.LblWiFiMode)
        Me.GrpWiFiSettings.Location = New System.Drawing.Point(7, 121)
        Me.GrpWiFiSettings.Name = "GrpWiFiSettings"
        Me.GrpWiFiSettings.Size = New System.Drawing.Size(515, 263)
        Me.GrpWiFiSettings.TabIndex = 37
        Me.GrpWiFiSettings.TabStop = False
        Me.GrpWiFiSettings.Text = "WiFi Settings"
        '
        'ChkWiFiState
        '
        Me.ChkWiFiState.AutoSize = True
        Me.ChkWiFiState.Location = New System.Drawing.Point(437, 14)
        Me.ChkWiFiState.Name = "ChkWiFiState"
        Me.ChkWiFiState.Size = New System.Drawing.Size(52, 19)
        Me.ChkWiFiState.TabIndex = 31
        Me.ChkWiFiState.Text = "OFF"
        Me.ChkWiFiState.UseVisualStyleBackColor = True
        '
        'PnlWiFiSettings
        '
        Me.PnlWiFiSettings.Controls.Add(Me.RdoAPMode)
        Me.PnlWiFiSettings.Controls.Add(Me.RdoClient)
        Me.PnlWiFiSettings.Controls.Add(Me.LblWiFiClient)
        Me.PnlWiFiSettings.Controls.Add(Me.LblWiFiAPMode)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtSoftAPIP)
        Me.PnlWiFiSettings.Controls.Add(Me.LblSoftAPIP)
        Me.PnlWiFiSettings.Controls.Add(Me.Label3)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtSoftAPSSID)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtSoftAP_Password)
        Me.PnlWiFiSettings.Controls.Add(Me.Label4)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtDNSSecondary)
        Me.PnlWiFiSettings.Controls.Add(Me.LblDNSSecondary)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtDNSPrimary)
        Me.PnlWiFiSettings.Controls.Add(Me.LblDNSPrimary)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtSubnet)
        Me.PnlWiFiSettings.Controls.Add(Me.LblSubnet)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtGateway)
        Me.PnlWiFiSettings.Controls.Add(Me.LblGateway)
        Me.PnlWiFiSettings.Controls.Add(Me.LblWiFiServerPort)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtLocalIPWiFi)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtServerPortWiFi)
        Me.PnlWiFiSettings.Controls.Add(Me.LblLocalIP)
        Me.PnlWiFiSettings.Controls.Add(Me.LblPassword)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtSSID)
        Me.PnlWiFiSettings.Controls.Add(Me.TxtPassword)
        Me.PnlWiFiSettings.Controls.Add(Me.LblSSID)
        Me.PnlWiFiSettings.Location = New System.Drawing.Point(6, 36)
        Me.PnlWiFiSettings.Name = "PnlWiFiSettings"
        Me.PnlWiFiSettings.Size = New System.Drawing.Size(503, 225)
        Me.PnlWiFiSettings.TabIndex = 0
        '
        'RdoAPMode
        '
        Me.RdoAPMode.AutoSize = True
        Me.RdoAPMode.Location = New System.Drawing.Point(111, 161)
        Me.RdoAPMode.Name = "RdoAPMode"
        Me.RdoAPMode.Size = New System.Drawing.Size(77, 19)
        Me.RdoAPMode.TabIndex = 35
        Me.RdoAPMode.TabStop = True
        Me.RdoAPMode.Text = "AP mode"
        Me.RdoAPMode.UseVisualStyleBackColor = True
        '
        'RdoClient
        '
        Me.RdoClient.AutoSize = True
        Me.RdoClient.Location = New System.Drawing.Point(111, 3)
        Me.RdoClient.Name = "RdoClient"
        Me.RdoClient.Size = New System.Drawing.Size(93, 19)
        Me.RdoClient.TabIndex = 34
        Me.RdoClient.TabStop = True
        Me.RdoClient.Text = "Client mode"
        Me.RdoClient.UseVisualStyleBackColor = True
        '
        'LblWiFiClient
        '
        Me.LblWiFiClient.AutoSize = True
        Me.LblWiFiClient.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblWiFiClient.Location = New System.Drawing.Point(20, 7)
        Me.LblWiFiClient.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblWiFiClient.Name = "LblWiFiClient"
        Me.LblWiFiClient.Size = New System.Drawing.Size(84, 15)
        Me.LblWiFiClient.TabIndex = 33
        Me.LblWiFiClient.Text = "Client Mode :"
        '
        'LblWiFiAPMode
        '
        Me.LblWiFiAPMode.AutoSize = True
        Me.LblWiFiAPMode.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblWiFiAPMode.Location = New System.Drawing.Point(20, 165)
        Me.LblWiFiAPMode.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblWiFiAPMode.Name = "LblWiFiAPMode"
        Me.LblWiFiAPMode.Size = New System.Drawing.Size(68, 15)
        Me.LblWiFiAPMode.TabIndex = 30
        Me.LblWiFiAPMode.Text = "AP Mode :"
        '
        'TxtSoftAPIP
        '
        Me.TxtSoftAPIP.Location = New System.Drawing.Point(97, 191)
        Me.TxtSoftAPIP.Name = "TxtSoftAPIP"
        Me.TxtSoftAPIP.Size = New System.Drawing.Size(127, 23)
        Me.TxtSoftAPIP.TabIndex = 27
        Me.TxtSoftAPIP.Text = "192.168.4.1"
        Me.TxtSoftAPIP.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblSoftAPIP
        '
        Me.LblSoftAPIP.AutoSize = True
        Me.LblSoftAPIP.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblSoftAPIP.Location = New System.Drawing.Point(20, 195)
        Me.LblSoftAPIP.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblSoftAPIP.Name = "LblSoftAPIP"
        Me.LblSoftAPIP.Size = New System.Drawing.Size(70, 15)
        Me.LblSoftAPIP.TabIndex = 28
        Me.LblSoftAPIP.Text = "SoftAP IP :"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(260, 196)
        Me.Label3.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(67, 15)
        Me.Label3.TabIndex = 26
        Me.Label3.Text = "Password :"
        '
        'TxtSoftAPSSID
        '
        Me.TxtSoftAPSSID.Location = New System.Drawing.Point(363, 163)
        Me.TxtSoftAPSSID.Name = "TxtSoftAPSSID"
        Me.TxtSoftAPSSID.Size = New System.Drawing.Size(127, 23)
        Me.TxtSoftAPSSID.TabIndex = 23
        Me.TxtSoftAPSSID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'TxtSoftAP_Password
        '
        Me.TxtSoftAP_Password.Location = New System.Drawing.Point(363, 192)
        Me.TxtSoftAP_Password.Name = "TxtSoftAP_Password"
        Me.TxtSoftAP_Password.Size = New System.Drawing.Size(127, 23)
        Me.TxtSoftAP_Password.TabIndex = 25
        Me.TxtSoftAP_Password.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(262, 166)
        Me.Label4.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(44, 15)
        Me.Label4.TabIndex = 24
        Me.Label4.Text = "SSID :"
        '
        'TxtDNSSecondary
        '
        Me.TxtDNSSecondary.Location = New System.Drawing.Point(363, 121)
        Me.TxtDNSSecondary.Name = "TxtDNSSecondary"
        Me.TxtDNSSecondary.Size = New System.Drawing.Size(127, 23)
        Me.TxtDNSSecondary.TabIndex = 21
        Me.TxtDNSSecondary.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblDNSSecondary
        '
        Me.LblDNSSecondary.AutoSize = True
        Me.LblDNSSecondary.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblDNSSecondary.Location = New System.Drawing.Point(260, 122)
        Me.LblDNSSecondary.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblDNSSecondary.Name = "LblDNSSecondary"
        Me.LblDNSSecondary.Size = New System.Drawing.Size(104, 15)
        Me.LblDNSSecondary.TabIndex = 22
        Me.LblDNSSecondary.Text = "Secondary DNS :"
        '
        'TxtDNSPrimary
        '
        Me.TxtDNSPrimary.Location = New System.Drawing.Point(363, 91)
        Me.TxtDNSPrimary.Name = "TxtDNSPrimary"
        Me.TxtDNSPrimary.Size = New System.Drawing.Size(127, 23)
        Me.TxtDNSPrimary.TabIndex = 19
        Me.TxtDNSPrimary.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblDNSPrimary
        '
        Me.LblDNSPrimary.AutoSize = True
        Me.LblDNSPrimary.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblDNSPrimary.Location = New System.Drawing.Point(260, 93)
        Me.LblDNSPrimary.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblDNSPrimary.Name = "LblDNSPrimary"
        Me.LblDNSPrimary.Size = New System.Drawing.Size(92, 15)
        Me.LblDNSPrimary.TabIndex = 20
        Me.LblDNSPrimary.Text = "Primary DNS :"
        '
        'TxtSubnet
        '
        Me.TxtSubnet.Location = New System.Drawing.Point(96, 120)
        Me.TxtSubnet.Name = "TxtSubnet"
        Me.TxtSubnet.Size = New System.Drawing.Size(127, 23)
        Me.TxtSubnet.TabIndex = 17
        Me.TxtSubnet.Text = "255.255.255.0"
        Me.TxtSubnet.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblSubnet
        '
        Me.LblSubnet.AutoSize = True
        Me.LblSubnet.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblSubnet.Location = New System.Drawing.Point(19, 121)
        Me.LblSubnet.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblSubnet.Name = "LblSubnet"
        Me.LblSubnet.Size = New System.Drawing.Size(52, 15)
        Me.LblSubnet.TabIndex = 18
        Me.LblSubnet.Text = "Subnet :"
        '
        'TxtGateway
        '
        Me.TxtGateway.Location = New System.Drawing.Point(96, 90)
        Me.TxtGateway.Name = "TxtGateway"
        Me.TxtGateway.Size = New System.Drawing.Size(127, 23)
        Me.TxtGateway.TabIndex = 15
        Me.TxtGateway.Text = "192.168.86.1"
        Me.TxtGateway.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblGateway
        '
        Me.LblGateway.AutoSize = True
        Me.LblGateway.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblGateway.Location = New System.Drawing.Point(19, 92)
        Me.LblGateway.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblGateway.Name = "LblGateway"
        Me.LblGateway.Size = New System.Drawing.Size(64, 15)
        Me.LblGateway.TabIndex = 16
        Me.LblGateway.Text = "Gateway :"
        '
        'LblWiFiServerPort
        '
        Me.LblWiFiServerPort.AutoSize = True
        Me.LblWiFiServerPort.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblWiFiServerPort.Location = New System.Drawing.Point(260, 62)
        Me.LblWiFiServerPort.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblWiFiServerPort.Name = "LblWiFiServerPort"
        Me.LblWiFiServerPort.Size = New System.Drawing.Size(98, 15)
        Me.LblWiFiServerPort.TabIndex = 14
        Me.LblWiFiServerPort.Text = "Server Port No :"
        '
        'TxtLocalIPWiFi
        '
        Me.TxtLocalIPWiFi.Location = New System.Drawing.Point(96, 60)
        Me.TxtLocalIPWiFi.Name = "TxtLocalIPWiFi"
        Me.TxtLocalIPWiFi.Size = New System.Drawing.Size(127, 23)
        Me.TxtLocalIPWiFi.TabIndex = 11
        Me.TxtLocalIPWiFi.Text = "192.168.86.21"
        Me.TxtLocalIPWiFi.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'TxtServerPortWiFi
        '
        Me.TxtServerPortWiFi.Location = New System.Drawing.Point(363, 61)
        Me.TxtServerPortWiFi.Name = "TxtServerPortWiFi"
        Me.TxtServerPortWiFi.Size = New System.Drawing.Size(127, 23)
        Me.TxtServerPortWiFi.TabIndex = 13
        Me.TxtServerPortWiFi.Text = "80"
        Me.TxtServerPortWiFi.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblLocalIP
        '
        Me.LblLocalIP.AutoSize = True
        Me.LblLocalIP.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblLocalIP.Location = New System.Drawing.Point(19, 63)
        Me.LblLocalIP.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblLocalIP.Name = "LblLocalIP"
        Me.LblLocalIP.Size = New System.Drawing.Size(62, 15)
        Me.LblLocalIP.TabIndex = 12
        Me.LblLocalIP.Text = "Local IP :"
        '
        'LblPassword
        '
        Me.LblPassword.AutoSize = True
        Me.LblPassword.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPassword.Location = New System.Drawing.Point(260, 34)
        Me.LblPassword.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPassword.Name = "LblPassword"
        Me.LblPassword.Size = New System.Drawing.Size(67, 15)
        Me.LblPassword.TabIndex = 10
        Me.LblPassword.Text = "Password :"
        '
        'TxtSSID
        '
        Me.TxtSSID.Location = New System.Drawing.Point(96, 30)
        Me.TxtSSID.Name = "TxtSSID"
        Me.TxtSSID.Size = New System.Drawing.Size(127, 23)
        Me.TxtSSID.TabIndex = 2
        Me.TxtSSID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'TxtPassword
        '
        Me.TxtPassword.Location = New System.Drawing.Point(363, 31)
        Me.TxtPassword.Name = "TxtPassword"
        Me.TxtPassword.Size = New System.Drawing.Size(127, 23)
        Me.TxtPassword.TabIndex = 9
        Me.TxtPassword.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblSSID
        '
        Me.LblSSID.AutoSize = True
        Me.LblSSID.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblSSID.Location = New System.Drawing.Point(19, 34)
        Me.LblSSID.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblSSID.Name = "LblSSID"
        Me.LblSSID.Size = New System.Drawing.Size(44, 15)
        Me.LblSSID.TabIndex = 3
        Me.LblSSID.Text = "SSID :"
        '
        'LblWiFiMode
        '
        Me.LblWiFiMode.AutoSize = True
        Me.LblWiFiMode.Location = New System.Drawing.Point(333, 14)
        Me.LblWiFiMode.Name = "LblWiFiMode"
        Me.LblWiFiMode.Size = New System.Drawing.Size(80, 15)
        Me.LblWiFiMode.TabIndex = 36
        Me.LblWiFiMode.Text = "WiFi Mode :"
        '
        'GrpEthernetSettings
        '
        Me.GrpEthernetSettings.Controls.Add(Me.ChkEthernetMode)
        Me.GrpEthernetSettings.Controls.Add(Me.PnlEthernetSettings)
        Me.GrpEthernetSettings.Controls.Add(Me.LblEthernetMode)
        Me.GrpEthernetSettings.Location = New System.Drawing.Point(7, 390)
        Me.GrpEthernetSettings.Name = "GrpEthernetSettings"
        Me.GrpEthernetSettings.Size = New System.Drawing.Size(515, 116)
        Me.GrpEthernetSettings.TabIndex = 33
        Me.GrpEthernetSettings.TabStop = False
        Me.GrpEthernetSettings.Text = "Ethernet Setting"
        '
        'ChkEthernetMode
        '
        Me.ChkEthernetMode.AutoSize = True
        Me.ChkEthernetMode.Location = New System.Drawing.Point(437, 15)
        Me.ChkEthernetMode.Name = "ChkEthernetMode"
        Me.ChkEthernetMode.Size = New System.Drawing.Size(52, 19)
        Me.ChkEthernetMode.TabIndex = 32
        Me.ChkEthernetMode.Text = "OFF"
        Me.ChkEthernetMode.UseVisualStyleBackColor = True
        '
        'PnlEthernetSettings
        '
        Me.PnlEthernetSettings.Controls.Add(Me.TxtServerIP)
        Me.PnlEthernetSettings.Controls.Add(Me.LblIPAddress)
        Me.PnlEthernetSettings.Controls.Add(Me.LblServerPort)
        Me.PnlEthernetSettings.Controls.Add(Me.TxtLocalIPEthernet)
        Me.PnlEthernetSettings.Controls.Add(Me.TxtServerPortEthernet)
        Me.PnlEthernetSettings.Controls.Add(Me.LblETHIP)
        Me.PnlEthernetSettings.Location = New System.Drawing.Point(6, 37)
        Me.PnlEthernetSettings.Name = "PnlEthernetSettings"
        Me.PnlEthernetSettings.Size = New System.Drawing.Size(503, 73)
        Me.PnlEthernetSettings.TabIndex = 23
        '
        'TxtServerIP
        '
        Me.TxtServerIP.Location = New System.Drawing.Point(106, 39)
        Me.TxtServerIP.Name = "TxtServerIP"
        Me.TxtServerIP.Size = New System.Drawing.Size(127, 23)
        Me.TxtServerIP.TabIndex = 15
        Me.TxtServerIP.Text = "169.254.33.234"
        Me.TxtServerIP.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIPAddress
        '
        Me.LblIPAddress.AutoSize = True
        Me.LblIPAddress.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIPAddress.Location = New System.Drawing.Point(18, 42)
        Me.LblIPAddress.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIPAddress.Name = "LblIPAddress"
        Me.LblIPAddress.Size = New System.Drawing.Size(66, 15)
        Me.LblIPAddress.TabIndex = 16
        Me.LblIPAddress.Text = "Server IP :"
        '
        'LblServerPort
        '
        Me.LblServerPort.AutoSize = True
        Me.LblServerPort.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblServerPort.Location = New System.Drawing.Point(260, 13)
        Me.LblServerPort.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblServerPort.Name = "LblServerPort"
        Me.LblServerPort.Size = New System.Drawing.Size(98, 15)
        Me.LblServerPort.TabIndex = 14
        Me.LblServerPort.Text = "Server Port No :"
        '
        'TxtLocalIPEthernet
        '
        Me.TxtLocalIPEthernet.Location = New System.Drawing.Point(106, 9)
        Me.TxtLocalIPEthernet.Name = "TxtLocalIPEthernet"
        Me.TxtLocalIPEthernet.Size = New System.Drawing.Size(127, 23)
        Me.TxtLocalIPEthernet.TabIndex = 11
        Me.TxtLocalIPEthernet.Text = "169.254.33.20"
        Me.TxtLocalIPEthernet.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'TxtServerPortEthernet
        '
        Me.TxtServerPortEthernet.Location = New System.Drawing.Point(363, 9)
        Me.TxtServerPortEthernet.Name = "TxtServerPortEthernet"
        Me.TxtServerPortEthernet.Size = New System.Drawing.Size(127, 23)
        Me.TxtServerPortEthernet.TabIndex = 13
        Me.TxtServerPortEthernet.Text = "8088"
        Me.TxtServerPortEthernet.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblETHIP
        '
        Me.LblETHIP.AutoSize = True
        Me.LblETHIP.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblETHIP.Location = New System.Drawing.Point(19, 13)
        Me.LblETHIP.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblETHIP.Name = "LblETHIP"
        Me.LblETHIP.Size = New System.Drawing.Size(62, 15)
        Me.LblETHIP.TabIndex = 12
        Me.LblETHIP.Text = "Local IP :"
        '
        'LblEthernetMode
        '
        Me.LblEthernetMode.AutoSize = True
        Me.LblEthernetMode.Location = New System.Drawing.Point(293, 16)
        Me.LblEthernetMode.Name = "LblEthernetMode"
        Me.LblEthernetMode.Size = New System.Drawing.Size(129, 15)
        Me.LblEthernetMode.TabIndex = 0
        Me.LblEthernetMode.Text = "Ethernet Connection :"
        '
        'Panel2
        '
        Me.Panel2.Controls.Add(Me.TxtPublishInt)
        Me.Panel2.Controls.Add(Me.Label1)
        Me.Panel2.Controls.Add(Me.LblPublishIntervel)
        Me.Panel2.Location = New System.Drawing.Point(292, 34)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(230, 81)
        Me.Panel2.TabIndex = 25
        '
        'TxtPublishInt
        '
        Me.TxtPublishInt.Location = New System.Drawing.Point(101, 37)
        Me.TxtPublishInt.Name = "TxtPublishInt"
        Me.TxtPublishInt.Size = New System.Drawing.Size(101, 23)
        Me.TxtPublishInt.TabIndex = 23
        Me.TxtPublishInt.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(13, 40)
        Me.Label1.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(85, 15)
        Me.Label1.TabIndex = 4
        Me.Label1.Text = "Interval (ms) :"
        '
        'LblPublishIntervel
        '
        Me.LblPublishIntervel.AutoSize = True
        Me.LblPublishIntervel.Location = New System.Drawing.Point(26, 8)
        Me.LblPublishIntervel.Name = "LblPublishIntervel"
        Me.LblPublishIntervel.Size = New System.Drawing.Size(179, 15)
        Me.LblPublishIntervel.TabIndex = 0
        Me.LblPublishIntervel.Text = "Publish Interval (Milliseconds)"
        '
        'CmdLoad
        '
        Me.CmdLoad.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdLoad.Location = New System.Drawing.Point(70, 638)
        Me.CmdLoad.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdLoad.Name = "CmdLoad"
        Me.CmdLoad.Size = New System.Drawing.Size(104, 30)
        Me.CmdLoad.TabIndex = 25
        Me.CmdLoad.Text = "Load Settings"
        Me.CmdLoad.UseVisualStyleBackColor = True
        '
        'GrpModbusSetting
        '
        Me.GrpModbusSetting.Controls.Add(Me.LblModbusMode)
        Me.GrpModbusSetting.Controls.Add(Me.ChkModbusMode)
        Me.GrpModbusSetting.Controls.Add(Me.PnlModbus)
        Me.GrpModbusSetting.Location = New System.Drawing.Point(7, 512)
        Me.GrpModbusSetting.Name = "GrpModbusSetting"
        Me.GrpModbusSetting.Size = New System.Drawing.Size(515, 118)
        Me.GrpModbusSetting.TabIndex = 34
        Me.GrpModbusSetting.TabStop = False
        Me.GrpModbusSetting.Text = "Modbus TCP/IP Settings"
        '
        'LblModbusMode
        '
        Me.LblModbusMode.AutoSize = True
        Me.LblModbusMode.Location = New System.Drawing.Point(293, 16)
        Me.LblModbusMode.Name = "LblModbusMode"
        Me.LblModbusMode.Size = New System.Drawing.Size(120, 15)
        Me.LblModbusMode.TabIndex = 0
        Me.LblModbusMode.Text = "Modbus Connection"
        '
        'ChkModbusMode
        '
        Me.ChkModbusMode.AutoSize = True
        Me.ChkModbusMode.Location = New System.Drawing.Point(437, 15)
        Me.ChkModbusMode.Name = "ChkModbusMode"
        Me.ChkModbusMode.Size = New System.Drawing.Size(52, 19)
        Me.ChkModbusMode.TabIndex = 33
        Me.ChkModbusMode.Text = "OFF"
        Me.ChkModbusMode.UseVisualStyleBackColor = True
        '
        'PnlModbus
        '
        Me.PnlModbus.Anchor = System.Windows.Forms.AnchorStyles.Left
        Me.PnlModbus.Controls.Add(Me.ComboBoxModbus_BaudRate)
        Me.PnlModbus.Controls.Add(Me.LblSlaveID)
        Me.PnlModbus.Controls.Add(Me.TxtPortNoModbus)
        Me.PnlModbus.Controls.Add(Me.LblModbusBaudrate)
        Me.PnlModbus.Controls.Add(Me.LblMDbusIP)
        Me.PnlModbus.Controls.Add(Me.LblMDBusPort)
        Me.PnlModbus.Controls.Add(Me.TxtIPModbus)
        Me.PnlModbus.Controls.Add(Me.TxtSlaveID)
        Me.PnlModbus.Location = New System.Drawing.Point(6, 42)
        Me.PnlModbus.Name = "PnlModbus"
        Me.PnlModbus.Size = New System.Drawing.Size(503, 68)
        Me.PnlModbus.TabIndex = 12
        '
        'ComboBoxModbus_BaudRate
        '
        Me.ComboBoxModbus_BaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxModbus_BaudRate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ComboBoxModbus_BaudRate.FormattingEnabled = True
        Me.ComboBoxModbus_BaudRate.Items.AddRange(New Object() {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"})
        Me.ComboBoxModbus_BaudRate.Location = New System.Drawing.Point(327, 39)
        Me.ComboBoxModbus_BaudRate.Margin = New System.Windows.Forms.Padding(4)
        Me.ComboBoxModbus_BaudRate.Name = "ComboBoxModbus_BaudRate"
        Me.ComboBoxModbus_BaudRate.Size = New System.Drawing.Size(127, 23)
        Me.ComboBoxModbus_BaudRate.TabIndex = 16
        '
        'LblSlaveID
        '
        Me.LblSlaveID.AutoSize = True
        Me.LblSlaveID.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblSlaveID.Location = New System.Drawing.Point(20, 12)
        Me.LblSlaveID.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblSlaveID.Name = "LblSlaveID"
        Me.LblSlaveID.Size = New System.Drawing.Size(63, 15)
        Me.LblSlaveID.TabIndex = 20
        Me.LblSlaveID.Text = "Slave ID :"
        '
        'TxtPortNoModbus
        '
        Me.TxtPortNoModbus.Location = New System.Drawing.Point(327, 8)
        Me.TxtPortNoModbus.Name = "TxtPortNoModbus"
        Me.TxtPortNoModbus.Size = New System.Drawing.Size(127, 23)
        Me.TxtPortNoModbus.TabIndex = 17
        Me.TxtPortNoModbus.Text = "502"
        Me.TxtPortNoModbus.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblModbusBaudrate
        '
        Me.LblModbusBaudrate.AutoSize = True
        Me.LblModbusBaudrate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblModbusBaudrate.Location = New System.Drawing.Point(252, 43)
        Me.LblModbusBaudrate.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblModbusBaudrate.Name = "LblModbusBaudrate"
        Me.LblModbusBaudrate.Size = New System.Drawing.Size(70, 15)
        Me.LblModbusBaudrate.TabIndex = 15
        Me.LblModbusBaudrate.Text = "Baud Rate :"
        '
        'LblMDbusIP
        '
        Me.LblMDbusIP.AutoSize = True
        Me.LblMDbusIP.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblMDbusIP.Location = New System.Drawing.Point(20, 43)
        Me.LblMDbusIP.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblMDbusIP.Name = "LblMDbusIP"
        Me.LblMDbusIP.Size = New System.Drawing.Size(75, 15)
        Me.LblMDbusIP.TabIndex = 14
        Me.LblMDbusIP.Text = "IP Address :"
        '
        'LblMDBusPort
        '
        Me.LblMDBusPort.AutoSize = True
        Me.LblMDBusPort.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblMDBusPort.Location = New System.Drawing.Point(252, 12)
        Me.LblMDBusPort.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblMDBusPort.Name = "LblMDBusPort"
        Me.LblMDBusPort.Size = New System.Drawing.Size(59, 15)
        Me.LblMDBusPort.TabIndex = 18
        Me.LblMDBusPort.Text = "Port No :"
        '
        'TxtIPModbus
        '
        Me.TxtIPModbus.Location = New System.Drawing.Point(97, 39)
        Me.TxtIPModbus.Name = "TxtIPModbus"
        Me.TxtIPModbus.Size = New System.Drawing.Size(127, 23)
        Me.TxtIPModbus.TabIndex = 13
        Me.TxtIPModbus.Text = "169.254.33.100"
        Me.TxtIPModbus.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'TxtSlaveID
        '
        Me.TxtSlaveID.Location = New System.Drawing.Point(97, 8)
        Me.TxtSlaveID.Name = "TxtSlaveID"
        Me.TxtSlaveID.Size = New System.Drawing.Size(127, 23)
        Me.TxtSlaveID.TabIndex = 19
        Me.TxtSlaveID.Text = "1"
        Me.TxtSlaveID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Panel1
        '
        Me.Panel1.Controls.Add(Me.ChkSerial)
        Me.Panel1.Controls.Add(Me.ComboBoxBaudRate)
        Me.Panel1.Controls.Add(Me.LblBaudRate)
        Me.Panel1.Controls.Add(Me.LblSerial)
        Me.Panel1.Location = New System.Drawing.Point(7, 34)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(279, 81)
        Me.Panel1.TabIndex = 24
        '
        'ChkSerial
        '
        Me.ChkSerial.AutoSize = True
        Me.ChkSerial.Location = New System.Drawing.Point(207, 7)
        Me.ChkSerial.Name = "ChkSerial"
        Me.ChkSerial.Size = New System.Drawing.Size(52, 19)
        Me.ChkSerial.TabIndex = 33
        Me.ChkSerial.Text = "OFF"
        Me.ChkSerial.UseVisualStyleBackColor = True
        '
        'ComboBoxBaudRate
        '
        Me.ComboBoxBaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxBaudRate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ComboBoxBaudRate.FormattingEnabled = True
        Me.ComboBoxBaudRate.Items.AddRange(New Object() {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"})
        Me.ComboBoxBaudRate.Location = New System.Drawing.Point(127, 42)
        Me.ComboBoxBaudRate.Margin = New System.Windows.Forms.Padding(4)
        Me.ComboBoxBaudRate.Name = "ComboBoxBaudRate"
        Me.ComboBoxBaudRate.Size = New System.Drawing.Size(132, 23)
        Me.ComboBoxBaudRate.TabIndex = 5
        '
        'LblBaudRate
        '
        Me.LblBaudRate.AutoSize = True
        Me.LblBaudRate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblBaudRate.Location = New System.Drawing.Point(26, 46)
        Me.LblBaudRate.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblBaudRate.Name = "LblBaudRate"
        Me.LblBaudRate.Size = New System.Drawing.Size(70, 15)
        Me.LblBaudRate.TabIndex = 4
        Me.LblBaudRate.Text = "Baud Rate :"
        '
        'LblSerial
        '
        Me.LblSerial.AutoSize = True
        Me.LblSerial.Location = New System.Drawing.Point(60, 7)
        Me.LblSerial.Name = "LblSerial"
        Me.LblSerial.Size = New System.Drawing.Size(106, 15)
        Me.LblSerial.TabIndex = 0
        Me.LblSerial.Text = "Serial Connection"
        '
        'CmdSave
        '
        Me.CmdSave.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdSave.Location = New System.Drawing.Point(213, 638)
        Me.CmdSave.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdSave.Name = "CmdSave"
        Me.CmdSave.Size = New System.Drawing.Size(104, 30)
        Me.CmdSave.TabIndex = 12
        Me.CmdSave.Text = "Save Settings"
        Me.CmdSave.UseVisualStyleBackColor = True
        '
        'CmdCancel
        '
        Me.CmdCancel.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdCancel.Location = New System.Drawing.Point(356, 638)
        Me.CmdCancel.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdCancel.Name = "CmdCancel"
        Me.CmdCancel.Size = New System.Drawing.Size(104, 30)
        Me.CmdCancel.TabIndex = 8
        Me.CmdCancel.Text = "Cancel"
        Me.CmdCancel.UseVisualStyleBackColor = True
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = Global.Meter_GE3222M_V1._0.My.Resources.Resources.Red
        Me.PictureBox1.Location = New System.Drawing.Point(503, 12)
        Me.PictureBox1.Margin = New System.Windows.Forms.Padding(4)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(15, 15)
        Me.PictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBox1.TabIndex = 9
        Me.PictureBox1.TabStop = False
        '
        'LblStatus
        '
        Me.LblStatus.AutoSize = True
        Me.LblStatus.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblStatus.Location = New System.Drawing.Point(382, 12)
        Me.LblStatus.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblStatus.Name = "LblStatus"
        Me.LblStatus.Size = New System.Drawing.Size(113, 15)
        Me.LblStatus.TabIndex = 8
        Me.LblStatus.Text = "Status : Disconnect"
        '
        'TimerHandler1_Client
        '
        Me.TimerHandler1_Client.Enabled = True
        Me.TimerHandler1_Client.Interval = 50
        '
        'FrmSMGE_3222MSettings
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(549, 701)
        Me.Controls.Add(Me.GrpTCPSettings)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Name = "FrmSMGE_3222MSettings"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE 3222M Protocol Settings"
        Me.GrpTCPSettings.ResumeLayout(False)
        Me.GrpTCPSettings.PerformLayout()
        Me.GrpWiFiSettings.ResumeLayout(False)
        Me.GrpWiFiSettings.PerformLayout()
        Me.PnlWiFiSettings.ResumeLayout(False)
        Me.PnlWiFiSettings.PerformLayout()
        Me.GrpEthernetSettings.ResumeLayout(False)
        Me.GrpEthernetSettings.PerformLayout()
        Me.PnlEthernetSettings.ResumeLayout(False)
        Me.PnlEthernetSettings.PerformLayout()
        Me.Panel2.ResumeLayout(False)
        Me.Panel2.PerformLayout()
        Me.GrpModbusSetting.ResumeLayout(False)
        Me.GrpModbusSetting.PerformLayout()
        Me.PnlModbus.ResumeLayout(False)
        Me.PnlModbus.PerformLayout()
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents GrpTCPSettings As GroupBox
    Friend WithEvents CmdSave As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents PictureBox1 As PictureBox
    Friend WithEvents PnlWiFiSettings As Panel
    Friend WithEvents LblPassword As Label
    Friend WithEvents TxtSSID As TextBox
    Friend WithEvents TxtPassword As TextBox
    Friend WithEvents LblSSID As Label
    Friend WithEvents LblStatus As Label
    Friend WithEvents LblWiFiServerPort As Label
    Friend WithEvents TxtLocalIPWiFi As TextBox
    Friend WithEvents TxtServerPortWiFi As TextBox
    Friend WithEvents LblLocalIP As Label
    Friend WithEvents TxtGateway As TextBox
    Friend WithEvents LblGateway As Label
    Friend WithEvents TxtSubnet As TextBox
    Friend WithEvents LblSubnet As Label
    Friend WithEvents TxtDNSSecondary As TextBox
    Friend WithEvents LblDNSSecondary As Label
    Friend WithEvents TxtDNSPrimary As TextBox
    Friend WithEvents LblDNSPrimary As Label
    Friend WithEvents PnlEthernetSettings As Panel
    Friend WithEvents LblServerPort As Label
    Friend WithEvents TxtLocalIPEthernet As TextBox
    Friend WithEvents TxtServerPortEthernet As TextBox
    Friend WithEvents LblETHIP As Label
    Friend WithEvents LblEthernetMode As Label
    Friend WithEvents Panel1 As Panel
    Friend WithEvents LblSerial As Label
    Friend WithEvents ComboBoxBaudRate As ComboBox
    Friend WithEvents LblBaudRate As Label
    Friend WithEvents TimerHandler1_Client As Timer
    Friend WithEvents CmdLoad As Button
    Friend WithEvents TxtServerIP As TextBox
    Friend WithEvents LblIPAddress As Label
    Friend WithEvents Panel2 As Panel
    Friend WithEvents TxtPublishInt As TextBox
    Friend WithEvents Label1 As Label
    Friend WithEvents LblPublishIntervel As Label
    Friend WithEvents LblWiFiAPMode As Label
    Friend WithEvents TxtSoftAPIP As TextBox
    Friend WithEvents LblSoftAPIP As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents TxtSoftAPSSID As TextBox
    Friend WithEvents TxtSoftAP_Password As TextBox
    Friend WithEvents Label4 As Label
    Friend WithEvents LblWiFiClient As Label
    Friend WithEvents ChkWiFiState As CheckBox
    Friend WithEvents RdoAPMode As RadioButton
    Friend WithEvents RdoClient As RadioButton
    Friend WithEvents LblWiFiMode As Label
    Friend WithEvents ChkEthernetMode As CheckBox
    Friend WithEvents GrpModbusSetting As GroupBox
    Friend WithEvents ChkModbusMode As CheckBox
    Friend WithEvents PnlModbus As Panel
    Friend WithEvents LblModbusMode As Label
    Friend WithEvents ComboBoxModbus_BaudRate As ComboBox
    Friend WithEvents LblSlaveID As Label
    Friend WithEvents TxtPortNoModbus As TextBox
    Friend WithEvents LblModbusBaudrate As Label
    Friend WithEvents LblMDbusIP As Label
    Friend WithEvents LblMDBusPort As Label
    Friend WithEvents TxtIPModbus As TextBox
    Friend WithEvents TxtSlaveID As TextBox
    Friend WithEvents GrpEthernetSettings As GroupBox
    Friend WithEvents GrpWiFiSettings As GroupBox
    Friend WithEvents ChkSerial As CheckBox
End Class
