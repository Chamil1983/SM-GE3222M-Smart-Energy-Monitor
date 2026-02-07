<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class FrmSMGE_3222MLoging
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
        Me.GrpConnection = New System.Windows.Forms.GroupBox()
        Me.CmdlSettings = New System.Windows.Forms.Button()
        Me.PictureBoxConnectionInd = New System.Windows.Forms.PictureBox()
        Me.LblStatus = New System.Windows.Forms.Label()
        Me.CmdDisconnect = New System.Windows.Forms.Button()
        Me.CmdConnect = New System.Windows.Forms.Button()
        Me.ComboBoxBaudRate = New System.Windows.Forms.ComboBox()
        Me.LblBaudRate = New System.Windows.Forms.Label()
        Me.ComboBoxPort = New System.Windows.Forms.ComboBox()
        Me.CmdScanPort = New System.Windows.Forms.Button()
        Me.SerialPort = New System.IO.Ports.SerialPort(Me.components)
        Me.TimerSerial = New System.Windows.Forms.Timer(Me.components)
        Me.CmdTCPDisconnect = New System.Windows.Forms.Button()
        Me.LblEthernetStatus = New System.Windows.Forms.Label()
        Me.CmdTCPConnect = New System.Windows.Forms.Button()
        Me.LblPort = New System.Windows.Forms.Label()
        Me.TxtTCPPort = New System.Windows.Forms.TextBox()
        Me.LblIPAddress = New System.Windows.Forms.Label()
        Me.TxtServerIP = New System.Windows.Forms.TextBox()
        Me.GrpConnectionType = New System.Windows.Forms.GroupBox()
        Me.ChkTCPModbus = New System.Windows.Forms.CheckBox()
        Me.OptTCPIP = New System.Windows.Forms.RadioButton()
        Me.OptSerialConnect = New System.Windows.Forms.RadioButton()
        Me.GrpMeterAccess = New System.Windows.Forms.GroupBox()
        Me.CmdExit = New System.Windows.Forms.Button()
        Me.CmdCallibrate = New System.Windows.Forms.Button()
        Me.CmdDataLogger = New System.Windows.Forms.Button()
        Me.CmdReadData = New System.Windows.Forms.Button()
        Me.TimerGaugeCom = New System.Windows.Forms.Timer(Me.components)
        Me.TimerDataLogRecord = New System.Windows.Forms.Timer(Me.components)
        Me.TimerChartRecord = New System.Windows.Forms.Timer(Me.components)
        Me.LblSlaveID = New System.Windows.Forms.Label()
        Me.TxtSlaveID = New System.Windows.Forms.TextBox()
        Me.GrpEthernet = New System.Windows.Forms.GroupBox()
        Me.CmdTCPSettings = New System.Windows.Forms.Button()
        Me.PictureBoxConnectionIndTCP = New System.Windows.Forms.PictureBox()
        Me.PnlModbus = New System.Windows.Forms.Panel()
        Me.ComboBoxModbus_BaudRate = New System.Windows.Forms.ComboBox()
        Me.LblModbusBaudrate = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.LblModbus = New System.Windows.Forms.Label()
        Me.TxtIPModbus = New System.Windows.Forms.TextBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.TxtPortModbus = New System.Windows.Forms.TextBox()
        Me.PnlEthernet = New System.Windows.Forms.Panel()
        Me.LblEthernet = New System.Windows.Forms.Label()
        Me.TimerHandler1_Client = New System.Windows.Forms.Timer(Me.components)
        Me.TxtComTerm = New System.Windows.Forms.TextBox()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.StatusLabeladapter = New System.Windows.Forms.ToolStripStatusLabel()
        Me.StatusLblClientIP = New System.Windows.Forms.ToolStripStatusLabel()
        Me.ProgressBarDataRead = New System.Windows.Forms.ToolStripProgressBar()
        Me.TimerRefreshForm = New System.Windows.Forms.Timer(Me.components)
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.TimerMonitor = New System.Windows.Forms.Timer(Me.components)
        Me.TimerCommands = New System.Windows.Forms.Timer(Me.components)
        Me.TimerSync = New System.Windows.Forms.Timer(Me.components)
        Me.GrpConnection.SuspendLayout()
        CType(Me.PictureBoxConnectionInd, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GrpConnectionType.SuspendLayout()
        Me.GrpMeterAccess.SuspendLayout()
        Me.GrpEthernet.SuspendLayout()
        CType(Me.PictureBoxConnectionIndTCP, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.PnlModbus.SuspendLayout()
        Me.PnlEthernet.SuspendLayout()
        Me.StatusStrip1.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.SuspendLayout()
        '
        'GrpConnection
        '
        Me.GrpConnection.Controls.Add(Me.CmdlSettings)
        Me.GrpConnection.Controls.Add(Me.PictureBoxConnectionInd)
        Me.GrpConnection.Controls.Add(Me.LblStatus)
        Me.GrpConnection.Controls.Add(Me.CmdDisconnect)
        Me.GrpConnection.Controls.Add(Me.CmdConnect)
        Me.GrpConnection.Controls.Add(Me.ComboBoxBaudRate)
        Me.GrpConnection.Controls.Add(Me.LblBaudRate)
        Me.GrpConnection.Controls.Add(Me.ComboBoxPort)
        Me.GrpConnection.Controls.Add(Me.CmdScanPort)
        Me.GrpConnection.Location = New System.Drawing.Point(12, 96)
        Me.GrpConnection.Margin = New System.Windows.Forms.Padding(4)
        Me.GrpConnection.Name = "GrpConnection"
        Me.GrpConnection.Padding = New System.Windows.Forms.Padding(4)
        Me.GrpConnection.Size = New System.Drawing.Size(552, 197)
        Me.GrpConnection.TabIndex = 0
        Me.GrpConnection.TabStop = False
        Me.GrpConnection.Text = "Serial Connction"
        '
        'CmdlSettings
        '
        Me.CmdlSettings.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdlSettings.Location = New System.Drawing.Point(267, 145)
        Me.CmdlSettings.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdlSettings.Name = "CmdlSettings"
        Me.CmdlSettings.Size = New System.Drawing.Size(223, 30)
        Me.CmdlSettings.TabIndex = 7
        Me.CmdlSettings.Text = "Settings"
        Me.CmdlSettings.UseVisualStyleBackColor = True
        '
        'PictureBoxConnectionInd
        '
        Me.PictureBoxConnectionInd.Image = Global.Meter_GE3222M_V1._0.My.Resources.Resources.Red
        Me.PictureBoxConnectionInd.Location = New System.Drawing.Point(518, 16)
        Me.PictureBoxConnectionInd.Margin = New System.Windows.Forms.Padding(4)
        Me.PictureBoxConnectionInd.Name = "PictureBoxConnectionInd"
        Me.PictureBoxConnectionInd.Size = New System.Drawing.Size(15, 15)
        Me.PictureBoxConnectionInd.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBoxConnectionInd.TabIndex = 6
        Me.PictureBoxConnectionInd.TabStop = False
        '
        'LblStatus
        '
        Me.LblStatus.AutoSize = True
        Me.LblStatus.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblStatus.Location = New System.Drawing.Point(397, 16)
        Me.LblStatus.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblStatus.Name = "LblStatus"
        Me.LblStatus.Size = New System.Drawing.Size(113, 15)
        Me.LblStatus.TabIndex = 1
        Me.LblStatus.Text = "Status : Disconnect"
        '
        'CmdDisconnect
        '
        Me.CmdDisconnect.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdDisconnect.Location = New System.Drawing.Point(16, 145)
        Me.CmdDisconnect.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdDisconnect.Name = "CmdDisconnect"
        Me.CmdDisconnect.Size = New System.Drawing.Size(223, 30)
        Me.CmdDisconnect.TabIndex = 5
        Me.CmdDisconnect.Text = "Disconnect"
        Me.CmdDisconnect.UseVisualStyleBackColor = True
        '
        'CmdConnect
        '
        Me.CmdConnect.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdConnect.Location = New System.Drawing.Point(16, 107)
        Me.CmdConnect.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdConnect.Name = "CmdConnect"
        Me.CmdConnect.Size = New System.Drawing.Size(223, 30)
        Me.CmdConnect.TabIndex = 4
        Me.CmdConnect.Text = "Connect"
        Me.CmdConnect.UseVisualStyleBackColor = True
        '
        'ComboBoxBaudRate
        '
        Me.ComboBoxBaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxBaudRate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ComboBoxBaudRate.FormattingEnabled = True
        Me.ComboBoxBaudRate.Items.AddRange(New Object() {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"})
        Me.ComboBoxBaudRate.Location = New System.Drawing.Point(400, 52)
        Me.ComboBoxBaudRate.Margin = New System.Windows.Forms.Padding(4)
        Me.ComboBoxBaudRate.Name = "ComboBoxBaudRate"
        Me.ComboBoxBaudRate.Size = New System.Drawing.Size(132, 23)
        Me.ComboBoxBaudRate.TabIndex = 3
        '
        'LblBaudRate
        '
        Me.LblBaudRate.AutoSize = True
        Me.LblBaudRate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblBaudRate.Location = New System.Drawing.Point(299, 56)
        Me.LblBaudRate.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblBaudRate.Name = "LblBaudRate"
        Me.LblBaudRate.Size = New System.Drawing.Size(70, 15)
        Me.LblBaudRate.TabIndex = 2
        Me.LblBaudRate.Text = "Baud Rate :"
        '
        'ComboBoxPort
        '
        Me.ComboBoxPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxPort.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ComboBoxPort.FormattingEnabled = True
        Me.ComboBoxPort.Location = New System.Drawing.Point(136, 50)
        Me.ComboBoxPort.Margin = New System.Windows.Forms.Padding(4)
        Me.ComboBoxPort.Name = "ComboBoxPort"
        Me.ComboBoxPort.Size = New System.Drawing.Size(101, 23)
        Me.ComboBoxPort.TabIndex = 1
        '
        'CmdScanPort
        '
        Me.CmdScanPort.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdScanPort.Location = New System.Drawing.Point(16, 50)
        Me.CmdScanPort.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdScanPort.Name = "CmdScanPort"
        Me.CmdScanPort.Size = New System.Drawing.Size(112, 30)
        Me.CmdScanPort.TabIndex = 0
        Me.CmdScanPort.Text = "Scan Port"
        Me.CmdScanPort.UseVisualStyleBackColor = True
        '
        'TimerSerial
        '
        Me.TimerSerial.Interval = 600
        '
        'CmdTCPDisconnect
        '
        Me.CmdTCPDisconnect.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdTCPDisconnect.Location = New System.Drawing.Point(160, 240)
        Me.CmdTCPDisconnect.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdTCPDisconnect.Name = "CmdTCPDisconnect"
        Me.CmdTCPDisconnect.Size = New System.Drawing.Size(104, 30)
        Me.CmdTCPDisconnect.TabIndex = 9
        Me.CmdTCPDisconnect.Text = "Disconnect"
        Me.CmdTCPDisconnect.UseVisualStyleBackColor = True
        '
        'LblEthernetStatus
        '
        Me.LblEthernetStatus.AutoSize = True
        Me.LblEthernetStatus.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblEthernetStatus.Location = New System.Drawing.Point(397, 14)
        Me.LblEthernetStatus.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblEthernetStatus.Name = "LblEthernetStatus"
        Me.LblEthernetStatus.Size = New System.Drawing.Size(113, 15)
        Me.LblEthernetStatus.TabIndex = 8
        Me.LblEthernetStatus.Text = "Status : Disconnect"
        '
        'CmdTCPConnect
        '
        Me.CmdTCPConnect.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdTCPConnect.Location = New System.Drawing.Point(31, 240)
        Me.CmdTCPConnect.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdTCPConnect.Name = "CmdTCPConnect"
        Me.CmdTCPConnect.Size = New System.Drawing.Size(104, 30)
        Me.CmdTCPConnect.TabIndex = 8
        Me.CmdTCPConnect.Text = "Connect"
        Me.CmdTCPConnect.UseVisualStyleBackColor = True
        '
        'LblPort
        '
        Me.LblPort.AutoSize = True
        Me.LblPort.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPort.Location = New System.Drawing.Point(258, 38)
        Me.LblPort.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPort.Name = "LblPort"
        Me.LblPort.Size = New System.Drawing.Size(59, 15)
        Me.LblPort.TabIndex = 10
        Me.LblPort.Text = "Port No :"
        '
        'TxtTCPPort
        '
        Me.TxtTCPPort.Location = New System.Drawing.Point(335, 34)
        Me.TxtTCPPort.Name = "TxtTCPPort"
        Me.TxtTCPPort.Size = New System.Drawing.Size(127, 23)
        Me.TxtTCPPort.TabIndex = 9
        Me.TxtTCPPort.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIPAddress
        '
        Me.LblIPAddress.AutoSize = True
        Me.LblIPAddress.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIPAddress.Location = New System.Drawing.Point(27, 38)
        Me.LblIPAddress.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIPAddress.Name = "LblIPAddress"
        Me.LblIPAddress.Size = New System.Drawing.Size(66, 15)
        Me.LblIPAddress.TabIndex = 3
        Me.LblIPAddress.Text = "Server IP :"
        '
        'TxtServerIP
        '
        Me.TxtServerIP.Location = New System.Drawing.Point(104, 34)
        Me.TxtServerIP.Name = "TxtServerIP"
        Me.TxtServerIP.Size = New System.Drawing.Size(127, 23)
        Me.TxtServerIP.TabIndex = 2
        Me.TxtServerIP.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'GrpConnectionType
        '
        Me.GrpConnectionType.Controls.Add(Me.ChkTCPModbus)
        Me.GrpConnectionType.Controls.Add(Me.OptTCPIP)
        Me.GrpConnectionType.Controls.Add(Me.OptSerialConnect)
        Me.GrpConnectionType.Location = New System.Drawing.Point(12, 16)
        Me.GrpConnectionType.Name = "GrpConnectionType"
        Me.GrpConnectionType.Size = New System.Drawing.Size(554, 73)
        Me.GrpConnectionType.TabIndex = 8
        Me.GrpConnectionType.TabStop = False
        Me.GrpConnectionType.Text = "Meter Connection Type"
        '
        'ChkTCPModbus
        '
        Me.ChkTCPModbus.AutoSize = True
        Me.ChkTCPModbus.Location = New System.Drawing.Point(359, 37)
        Me.ChkTCPModbus.Name = "ChkTCPModbus"
        Me.ChkTCPModbus.Size = New System.Drawing.Size(135, 21)
        Me.ChkTCPModbus.TabIndex = 11
        Me.ChkTCPModbus.Text = "Modbus (TCP/IP)"
        Me.ChkTCPModbus.UseVisualStyleBackColor = True
        '
        'OptTCPIP
        '
        Me.OptTCPIP.AutoSize = True
        Me.OptTCPIP.Location = New System.Drawing.Point(173, 37)
        Me.OptTCPIP.Name = "OptTCPIP"
        Me.OptTCPIP.Size = New System.Drawing.Size(143, 21)
        Me.OptTCPIP.TabIndex = 11
        Me.OptTCPIP.TabStop = True
        Me.OptTCPIP.Text = "TCP/IP Connection"
        Me.OptTCPIP.UseVisualStyleBackColor = True
        '
        'OptSerialConnect
        '
        Me.OptSerialConnect.AutoSize = True
        Me.OptSerialConnect.Checked = True
        Me.OptSerialConnect.Location = New System.Drawing.Point(7, 37)
        Me.OptSerialConnect.Name = "OptSerialConnect"
        Me.OptSerialConnect.Size = New System.Drawing.Size(127, 21)
        Me.OptSerialConnect.TabIndex = 10
        Me.OptSerialConnect.TabStop = True
        Me.OptSerialConnect.Text = "Serial Connection"
        Me.OptSerialConnect.UseVisualStyleBackColor = True
        '
        'GrpMeterAccess
        '
        Me.GrpMeterAccess.Controls.Add(Me.CmdExit)
        Me.GrpMeterAccess.Controls.Add(Me.CmdCallibrate)
        Me.GrpMeterAccess.Controls.Add(Me.CmdDataLogger)
        Me.GrpMeterAccess.Controls.Add(Me.CmdReadData)
        Me.GrpMeterAccess.Location = New System.Drawing.Point(12, 588)
        Me.GrpMeterAccess.Name = "GrpMeterAccess"
        Me.GrpMeterAccess.Size = New System.Drawing.Size(552, 56)
        Me.GrpMeterAccess.TabIndex = 9
        Me.GrpMeterAccess.TabStop = False
        '
        'CmdExit
        '
        Me.CmdExit.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdExit.Location = New System.Drawing.Point(418, 17)
        Me.CmdExit.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdExit.Name = "CmdExit"
        Me.CmdExit.Size = New System.Drawing.Size(104, 30)
        Me.CmdExit.TabIndex = 10
        Me.CmdExit.Text = "Exit"
        Me.CmdExit.UseVisualStyleBackColor = True
        '
        'CmdCallibrate
        '
        Me.CmdCallibrate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdCallibrate.Location = New System.Drawing.Point(289, 17)
        Me.CmdCallibrate.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdCallibrate.Name = "CmdCallibrate"
        Me.CmdCallibrate.Size = New System.Drawing.Size(104, 30)
        Me.CmdCallibrate.TabIndex = 9
        Me.CmdCallibrate.Text = "Callibrate"
        Me.CmdCallibrate.UseVisualStyleBackColor = True
        '
        'CmdDataLogger
        '
        Me.CmdDataLogger.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdDataLogger.Location = New System.Drawing.Point(160, 17)
        Me.CmdDataLogger.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdDataLogger.Name = "CmdDataLogger"
        Me.CmdDataLogger.Size = New System.Drawing.Size(104, 30)
        Me.CmdDataLogger.TabIndex = 8
        Me.CmdDataLogger.Text = "Data Logger"
        Me.CmdDataLogger.UseVisualStyleBackColor = True
        '
        'CmdReadData
        '
        Me.CmdReadData.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdReadData.Location = New System.Drawing.Point(31, 17)
        Me.CmdReadData.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdReadData.Name = "CmdReadData"
        Me.CmdReadData.Size = New System.Drawing.Size(104, 30)
        Me.CmdReadData.TabIndex = 7
        Me.CmdReadData.Text = "Read Data"
        Me.CmdReadData.UseVisualStyleBackColor = True
        '
        'TimerGaugeCom
        '
        Me.TimerGaugeCom.Interval = 1000
        '
        'TimerDataLogRecord
        '
        Me.TimerDataLogRecord.Interval = 1000
        '
        'TimerChartRecord
        '
        Me.TimerChartRecord.Enabled = True
        Me.TimerChartRecord.Interval = 1000
        '
        'LblSlaveID
        '
        Me.LblSlaveID.AutoSize = True
        Me.LblSlaveID.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblSlaveID.Location = New System.Drawing.Point(27, 39)
        Me.LblSlaveID.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblSlaveID.Name = "LblSlaveID"
        Me.LblSlaveID.Size = New System.Drawing.Size(63, 15)
        Me.LblSlaveID.TabIndex = 12
        Me.LblSlaveID.Text = "Slave ID :"
        '
        'TxtSlaveID
        '
        Me.TxtSlaveID.Location = New System.Drawing.Point(102, 35)
        Me.TxtSlaveID.Name = "TxtSlaveID"
        Me.TxtSlaveID.Size = New System.Drawing.Size(129, 23)
        Me.TxtSlaveID.TabIndex = 11
        Me.TxtSlaveID.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'GrpEthernet
        '
        Me.GrpEthernet.Controls.Add(Me.CmdTCPSettings)
        Me.GrpEthernet.Controls.Add(Me.CmdTCPConnect)
        Me.GrpEthernet.Controls.Add(Me.PictureBoxConnectionIndTCP)
        Me.GrpEthernet.Controls.Add(Me.PnlModbus)
        Me.GrpEthernet.Controls.Add(Me.CmdTCPDisconnect)
        Me.GrpEthernet.Controls.Add(Me.PnlEthernet)
        Me.GrpEthernet.Controls.Add(Me.LblEthernetStatus)
        Me.GrpEthernet.Enabled = False
        Me.GrpEthernet.Location = New System.Drawing.Point(12, 299)
        Me.GrpEthernet.Name = "GrpEthernet"
        Me.GrpEthernet.Size = New System.Drawing.Size(552, 285)
        Me.GrpEthernet.TabIndex = 1
        Me.GrpEthernet.TabStop = False
        Me.GrpEthernet.Text = "Ethernet / Modbus Connection"
        '
        'CmdTCPSettings
        '
        Me.CmdTCPSettings.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdTCPSettings.Location = New System.Drawing.Point(289, 240)
        Me.CmdTCPSettings.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdTCPSettings.Name = "CmdTCPSettings"
        Me.CmdTCPSettings.Size = New System.Drawing.Size(104, 30)
        Me.CmdTCPSettings.TabIndex = 12
        Me.CmdTCPSettings.Text = "Settings"
        Me.CmdTCPSettings.UseVisualStyleBackColor = True
        '
        'PictureBoxConnectionIndTCP
        '
        Me.PictureBoxConnectionIndTCP.Image = Global.Meter_GE3222M_V1._0.My.Resources.Resources.Red
        Me.PictureBoxConnectionIndTCP.Location = New System.Drawing.Point(518, 14)
        Me.PictureBoxConnectionIndTCP.Margin = New System.Windows.Forms.Padding(4)
        Me.PictureBoxConnectionIndTCP.Name = "PictureBoxConnectionIndTCP"
        Me.PictureBoxConnectionIndTCP.Size = New System.Drawing.Size(15, 15)
        Me.PictureBoxConnectionIndTCP.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBoxConnectionIndTCP.TabIndex = 9
        Me.PictureBoxConnectionIndTCP.TabStop = False
        '
        'PnlModbus
        '
        Me.PnlModbus.Controls.Add(Me.ComboBoxModbus_BaudRate)
        Me.PnlModbus.Controls.Add(Me.LblSlaveID)
        Me.PnlModbus.Controls.Add(Me.LblModbusBaudrate)
        Me.PnlModbus.Controls.Add(Me.Label2)
        Me.PnlModbus.Controls.Add(Me.LblModbus)
        Me.PnlModbus.Controls.Add(Me.TxtSlaveID)
        Me.PnlModbus.Controls.Add(Me.TxtIPModbus)
        Me.PnlModbus.Controls.Add(Me.Label4)
        Me.PnlModbus.Controls.Add(Me.TxtPortModbus)
        Me.PnlModbus.Location = New System.Drawing.Point(7, 113)
        Me.PnlModbus.Name = "PnlModbus"
        Me.PnlModbus.Size = New System.Drawing.Size(515, 114)
        Me.PnlModbus.TabIndex = 11
        '
        'ComboBoxModbus_BaudRate
        '
        Me.ComboBoxModbus_BaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.ComboBoxModbus_BaudRate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ComboBoxModbus_BaudRate.FormattingEnabled = True
        Me.ComboBoxModbus_BaudRate.Items.AddRange(New Object() {"9600", "19200", "38400", "57600", "115200"})
        Me.ComboBoxModbus_BaudRate.Location = New System.Drawing.Point(334, 73)
        Me.ComboBoxModbus_BaudRate.Margin = New System.Windows.Forms.Padding(4)
        Me.ComboBoxModbus_BaudRate.Name = "ComboBoxModbus_BaudRate"
        Me.ComboBoxModbus_BaudRate.Size = New System.Drawing.Size(127, 23)
        Me.ComboBoxModbus_BaudRate.TabIndex = 8
        '
        'LblModbusBaudrate
        '
        Me.LblModbusBaudrate.AutoSize = True
        Me.LblModbusBaudrate.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblModbusBaudrate.Location = New System.Drawing.Point(259, 77)
        Me.LblModbusBaudrate.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblModbusBaudrate.Name = "LblModbusBaudrate"
        Me.LblModbusBaudrate.Size = New System.Drawing.Size(70, 15)
        Me.LblModbusBaudrate.TabIndex = 7
        Me.LblModbusBaudrate.Text = "Baud Rate :"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(259, 39)
        Me.Label2.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(59, 15)
        Me.Label2.TabIndex = 10
        Me.Label2.Text = "Port No :"
        '
        'LblModbus
        '
        Me.LblModbus.AutoSize = True
        Me.LblModbus.Location = New System.Drawing.Point(203, 9)
        Me.LblModbus.Name = "LblModbus"
        Me.LblModbus.Size = New System.Drawing.Size(126, 17)
        Me.LblModbus.TabIndex = 0
        Me.LblModbus.Text = "Modbus Connection"
        '
        'TxtIPModbus
        '
        Me.TxtIPModbus.Location = New System.Drawing.Point(104, 73)
        Me.TxtIPModbus.Name = "TxtIPModbus"
        Me.TxtIPModbus.Size = New System.Drawing.Size(127, 23)
        Me.TxtIPModbus.TabIndex = 2
        Me.TxtIPModbus.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(27, 77)
        Me.Label4.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(75, 15)
        Me.Label4.TabIndex = 3
        Me.Label4.Text = "IP Address :"
        '
        'TxtPortModbus
        '
        Me.TxtPortModbus.Location = New System.Drawing.Point(334, 35)
        Me.TxtPortModbus.Name = "TxtPortModbus"
        Me.TxtPortModbus.Size = New System.Drawing.Size(127, 23)
        Me.TxtPortModbus.TabIndex = 9
        Me.TxtPortModbus.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'PnlEthernet
        '
        Me.PnlEthernet.Controls.Add(Me.LblPort)
        Me.PnlEthernet.Controls.Add(Me.LblEthernet)
        Me.PnlEthernet.Controls.Add(Me.TxtServerIP)
        Me.PnlEthernet.Controls.Add(Me.TxtTCPPort)
        Me.PnlEthernet.Controls.Add(Me.LblIPAddress)
        Me.PnlEthernet.Location = New System.Drawing.Point(6, 32)
        Me.PnlEthernet.Name = "PnlEthernet"
        Me.PnlEthernet.Size = New System.Drawing.Size(516, 75)
        Me.PnlEthernet.TabIndex = 0
        '
        'LblEthernet
        '
        Me.LblEthernet.AutoSize = True
        Me.LblEthernet.Location = New System.Drawing.Point(203, 5)
        Me.LblEthernet.Name = "LblEthernet"
        Me.LblEthernet.Size = New System.Drawing.Size(127, 17)
        Me.LblEthernet.TabIndex = 0
        Me.LblEthernet.Text = "Ethernet Connection"
        '
        'TimerHandler1_Client
        '
        Me.TimerHandler1_Client.Enabled = True
        Me.TimerHandler1_Client.Interval = 500
        '
        'TxtComTerm
        '
        Me.TxtComTerm.BackColor = System.Drawing.Color.DarkBlue
        Me.TxtComTerm.Font = New System.Drawing.Font("Consolas", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TxtComTerm.ForeColor = System.Drawing.Color.LemonChiffon
        Me.TxtComTerm.Location = New System.Drawing.Point(7, 26)
        Me.TxtComTerm.Multiline = True
        Me.TxtComTerm.Name = "TxtComTerm"
        Me.TxtComTerm.ReadOnly = True
        Me.TxtComTerm.ScrollBars = System.Windows.Forms.ScrollBars.Both
        Me.TxtComTerm.Size = New System.Drawing.Size(284, 582)
        Me.TxtComTerm.TabIndex = 10
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Font = New System.Drawing.Font("Calisto MT", 9.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.StatusStrip1.ImageScalingSize = New System.Drawing.Size(24, 24)
        Me.StatusStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.StatusLabeladapter, Me.StatusLblClientIP, Me.ProgressBarDataRead})
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 654)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional
        Me.StatusStrip1.Size = New System.Drawing.Size(879, 22)
        Me.StatusStrip1.TabIndex = 11
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'StatusLabeladapter
        '
        Me.StatusLabeladapter.Name = "StatusLabeladapter"
        Me.StatusLabeladapter.Size = New System.Drawing.Size(32, 17)
        Me.StatusLabeladapter.Text = "Host"
        '
        'StatusLblClientIP
        '
        Me.StatusLblClientIP.Name = "StatusLblClientIP"
        Me.StatusLblClientIP.Size = New System.Drawing.Size(40, 17)
        Me.StatusLblClientIP.Text = "Client"
        '
        'ProgressBarDataRead
        '
        Me.ProgressBarDataRead.Name = "ProgressBarDataRead"
        Me.ProgressBarDataRead.Size = New System.Drawing.Size(100, 16)
        '
        'TimerRefreshForm
        '
        Me.TimerRefreshForm.Interval = 500
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.TxtComTerm)
        Me.GroupBox1.Location = New System.Drawing.Point(572, 27)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(299, 617)
        Me.GroupBox1.TabIndex = 12
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Meter Status: Communication Terminal"
        '
        'TimerMonitor
        '
        Me.TimerMonitor.Enabled = True
        Me.TimerMonitor.Interval = 500
        '
        'TimerCommands
        '
        Me.TimerCommands.Enabled = True
        Me.TimerCommands.Interval = 1000
        '
        'TimerSync
        '
        Me.TimerSync.Enabled = True
        Me.TimerSync.Interval = 1000
        '
        'FrmSMGE_3222MLoging
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 17.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(879, 676)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.GrpMeterAccess)
        Me.Controls.Add(Me.GrpConnectionType)
        Me.Controls.Add(Me.GrpEthernet)
        Me.Controls.Add(Me.GrpConnection)
        Me.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Margin = New System.Windows.Forms.Padding(4)
        Me.MaximizeBox = False
        Me.Name = "FrmSMGE_3222MLoging"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE3222M Logging"
        Me.GrpConnection.ResumeLayout(False)
        Me.GrpConnection.PerformLayout()
        CType(Me.PictureBoxConnectionInd, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GrpConnectionType.ResumeLayout(False)
        Me.GrpConnectionType.PerformLayout()
        Me.GrpMeterAccess.ResumeLayout(False)
        Me.GrpEthernet.ResumeLayout(False)
        Me.GrpEthernet.PerformLayout()
        CType(Me.PictureBoxConnectionIndTCP, System.ComponentModel.ISupportInitialize).EndInit()
        Me.PnlModbus.ResumeLayout(False)
        Me.PnlModbus.PerformLayout()
        Me.PnlEthernet.ResumeLayout(False)
        Me.PnlEthernet.PerformLayout()
        Me.StatusStrip1.ResumeLayout(False)
        Me.StatusStrip1.PerformLayout()
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents GrpConnection As GroupBox
    Friend WithEvents CmdScanPort As Button
    Friend WithEvents LblBaudRate As Label
    Friend WithEvents ComboBoxPort As ComboBox
    Friend WithEvents ComboBoxBaudRate As ComboBox
    Friend WithEvents CmdConnect As Button
    Friend WithEvents CmdDisconnect As Button
    Friend WithEvents LblStatus As Label
    Friend WithEvents PictureBoxConnectionInd As PictureBox
    Friend WithEvents SerialPort As IO.Ports.SerialPort
    Friend WithEvents TimerSerial As Timer
    Friend WithEvents LblIPAddress As Label
    Friend WithEvents TxtServerIP As TextBox
    Friend WithEvents LblPort As Label
    Friend WithEvents TxtTCPPort As TextBox
    Friend WithEvents CmdTCPDisconnect As Button
    Friend WithEvents CmdTCPConnect As Button
    Friend WithEvents PictureBoxConnectionIndTCP As PictureBox
    Friend WithEvents LblEthernetStatus As Label
    Friend WithEvents GrpConnectionType As GroupBox
    Friend WithEvents GrpMeterAccess As GroupBox
    Friend WithEvents CmdReadData As Button
    Friend WithEvents CmdDataLogger As Button
    Friend WithEvents OptTCPIP As RadioButton
    Friend WithEvents OptSerialConnect As RadioButton
    Friend WithEvents CmdCallibrate As Button
    Friend WithEvents CmdExit As Button
    Friend WithEvents TimerGaugeCom As Timer
    Friend WithEvents TimerDataLogRecord As Timer
    Friend WithEvents TimerChartRecord As Timer
    Friend WithEvents ChkTCPModbus As CheckBox
    Friend WithEvents LblSlaveID As Label
    Friend WithEvents TxtSlaveID As TextBox
    Friend WithEvents GrpEthernet As GroupBox
    Friend WithEvents PnlEthernet As Panel
    Friend WithEvents LblEthernet As Label
    Friend WithEvents PnlModbus As Panel
    Friend WithEvents Label2 As Label
    Friend WithEvents LblModbus As Label
    Friend WithEvents TxtIPModbus As TextBox
    Friend WithEvents TxtPortModbus As TextBox
    Friend WithEvents Label4 As Label
    Friend WithEvents ComboBoxModbus_BaudRate As ComboBox
    Friend WithEvents LblModbusBaudrate As Label
    Friend WithEvents TimerHandler1_Client As Timer
    Friend WithEvents CmdTCPSettings As Button
    Friend WithEvents TxtComTerm As TextBox
    Friend WithEvents StatusStrip1 As StatusStrip
    Friend WithEvents StatusLabeladapter As ToolStripStatusLabel
    Friend WithEvents TimerRefreshForm As Timer
    Friend WithEvents StatusLblClientIP As ToolStripStatusLabel
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents ProgressBarDataRead As ToolStripProgressBar
    Friend WithEvents TimerMonitor As Timer
    Friend WithEvents CmdlSettings As Button
    Friend WithEvents TimerCommands As Timer
    Friend WithEvents TimerSync As Timer
End Class
