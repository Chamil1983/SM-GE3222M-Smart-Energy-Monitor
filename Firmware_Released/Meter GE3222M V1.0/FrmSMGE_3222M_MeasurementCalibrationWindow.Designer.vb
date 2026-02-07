<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_MeasurementCalibrationWindow
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
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
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.TxtIoffsetC = New System.Windows.Forms.TextBox()
        Me.LblIoffsetC = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.TxtUoffsetC = New System.Windows.Forms.TextBox()
        Me.LblUoffsetC = New System.Windows.Forms.Label()
        Me.Label14 = New System.Windows.Forms.Label()
        Me.TxtIgainC = New System.Windows.Forms.TextBox()
        Me.LblIgainC = New System.Windows.Forms.Label()
        Me.Label13 = New System.Windows.Forms.Label()
        Me.TxtUgainC = New System.Windows.Forms.TextBox()
        Me.LblUgainC = New System.Windows.Forms.Label()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.TxtIoffsetB = New System.Windows.Forms.TextBox()
        Me.LblIoffsetB = New System.Windows.Forms.Label()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.TxtUoffsetB = New System.Windows.Forms.TextBox()
        Me.LblUoffsetB = New System.Windows.Forms.Label()
        Me.PnlMeasurementCalibration = New System.Windows.Forms.Panel()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.TxtIoffsetN = New System.Windows.Forms.TextBox()
        Me.LblIoffsetN = New System.Windows.Forms.Label()
        Me.Label15 = New System.Windows.Forms.Label()
        Me.TxtIgainN = New System.Windows.Forms.TextBox()
        Me.LblIgainN = New System.Windows.Forms.Label()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.TxtIgainB = New System.Windows.Forms.TextBox()
        Me.LblIgainB = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.TxtUgainB = New System.Windows.Forms.TextBox()
        Me.LblUgainB = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.TxtIoffsetA = New System.Windows.Forms.TextBox()
        Me.LblIoffsetA = New System.Windows.Forms.Label()
        Me.TxtUoffsetA = New System.Windows.Forms.TextBox()
        Me.LblUoffsetA = New System.Windows.Forms.Label()
        Me.TxtIgainA = New System.Windows.Forms.TextBox()
        Me.LblIgainA = New System.Windows.Forms.Label()
        Me.TxtUgainA = New System.Windows.Forms.TextBox()
        Me.LblUgainA = New System.Windows.Forms.Label()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.CmdLoad = New System.Windows.Forms.Button()
        Me.CmdSave = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.GrpMeasurementCalibrationReg = New System.Windows.Forms.GroupBox()
        Me.TimerHandler1_Client = New System.Windows.Forms.Timer(Me.components)
        Me.PnlMeasurementCalibration.SuspendLayout()
        Me.GrpMeasurementCalibrationReg.SuspendLayout()
        Me.SuspendLayout()
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label6.Location = New System.Drawing.Point(200, 333)
        Me.Label6.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(165, 15)
        Me.Label6.TabIndex = 39
        Me.Label6.Text = "Phase C Current RMS Offset"
        '
        'TxtIoffsetC
        '
        Me.TxtIoffsetC.Location = New System.Drawing.Point(87, 330)
        Me.TxtIoffsetC.Name = "TxtIoffsetC"
        Me.TxtIoffsetC.Size = New System.Drawing.Size(100, 23)
        Me.TxtIoffsetC.TabIndex = 37
        Me.TxtIoffsetC.Text = "0"
        Me.TxtIoffsetC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIoffsetC
        '
        Me.LblIoffsetC.AutoSize = True
        Me.LblIoffsetC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIoffsetC.Location = New System.Drawing.Point(13, 333)
        Me.LblIoffsetC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIoffsetC.Name = "LblIoffsetC"
        Me.LblIoffsetC.Size = New System.Drawing.Size(58, 15)
        Me.LblIoffsetC.TabIndex = 38
        Me.LblIoffsetC.Text = "IoffsetC :"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label10.Location = New System.Drawing.Point(200, 304)
        Me.Label10.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(164, 15)
        Me.Label10.TabIndex = 36
        Me.Label10.Text = "Phase C Voltage RMS Offset"
        '
        'TxtUoffsetC
        '
        Me.TxtUoffsetC.Location = New System.Drawing.Point(87, 301)
        Me.TxtUoffsetC.Name = "TxtUoffsetC"
        Me.TxtUoffsetC.Size = New System.Drawing.Size(100, 23)
        Me.TxtUoffsetC.TabIndex = 34
        Me.TxtUoffsetC.Text = "0"
        Me.TxtUoffsetC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblUoffsetC
        '
        Me.LblUoffsetC.AutoSize = True
        Me.LblUoffsetC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblUoffsetC.Location = New System.Drawing.Point(13, 304)
        Me.LblUoffsetC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblUoffsetC.Name = "LblUoffsetC"
        Me.LblUoffsetC.Size = New System.Drawing.Size(63, 15)
        Me.LblUoffsetC.TabIndex = 35
        Me.LblUoffsetC.Text = "UoffsetC :"
        '
        'Label14
        '
        Me.Label14.AutoSize = True
        Me.Label14.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label14.Location = New System.Drawing.Point(200, 275)
        Me.Label14.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label14.Name = "Label14"
        Me.Label14.Size = New System.Drawing.Size(160, 15)
        Me.Label14.TabIndex = 33
        Me.Label14.Text = "Phase C Current RMS Gain"
        '
        'TxtIgainC
        '
        Me.TxtIgainC.Location = New System.Drawing.Point(87, 272)
        Me.TxtIgainC.Name = "TxtIgainC"
        Me.TxtIgainC.Size = New System.Drawing.Size(100, 23)
        Me.TxtIgainC.TabIndex = 31
        Me.TxtIgainC.Text = "0"
        Me.TxtIgainC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIgainC
        '
        Me.LblIgainC.AutoSize = True
        Me.LblIgainC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIgainC.Location = New System.Drawing.Point(13, 275)
        Me.LblIgainC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIgainC.Name = "LblIgainC"
        Me.LblIgainC.Size = New System.Drawing.Size(51, 15)
        Me.LblIgainC.TabIndex = 32
        Me.LblIgainC.Text = "IgainC :"
        '
        'Label13
        '
        Me.Label13.AutoSize = True
        Me.Label13.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label13.Location = New System.Drawing.Point(200, 246)
        Me.Label13.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label13.Name = "Label13"
        Me.Label13.Size = New System.Drawing.Size(159, 15)
        Me.Label13.TabIndex = 30
        Me.Label13.Text = "Phase C Voltage RMS Gain"
        '
        'TxtUgainC
        '
        Me.TxtUgainC.Location = New System.Drawing.Point(87, 243)
        Me.TxtUgainC.Name = "TxtUgainC"
        Me.TxtUgainC.Size = New System.Drawing.Size(100, 23)
        Me.TxtUgainC.TabIndex = 28
        Me.TxtUgainC.Text = "0"
        Me.TxtUgainC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblUgainC
        '
        Me.LblUgainC.AutoSize = True
        Me.LblUgainC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblUgainC.Location = New System.Drawing.Point(13, 246)
        Me.LblUgainC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblUgainC.Name = "LblUgainC"
        Me.LblUgainC.Size = New System.Drawing.Size(56, 15)
        Me.LblUgainC.TabIndex = 29
        Me.LblUgainC.Text = "UgainC :"
        '
        'Label11
        '
        Me.Label11.AutoSize = True
        Me.Label11.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label11.Location = New System.Drawing.Point(200, 217)
        Me.Label11.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label11.Name = "Label11"
        Me.Label11.Size = New System.Drawing.Size(164, 15)
        Me.Label11.TabIndex = 27
        Me.Label11.Text = "Phase B Current RMS Offset"
        '
        'TxtIoffsetB
        '
        Me.TxtIoffsetB.Location = New System.Drawing.Point(87, 214)
        Me.TxtIoffsetB.Name = "TxtIoffsetB"
        Me.TxtIoffsetB.Size = New System.Drawing.Size(100, 23)
        Me.TxtIoffsetB.TabIndex = 25
        Me.TxtIoffsetB.Text = "0"
        Me.TxtIoffsetB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIoffsetB
        '
        Me.LblIoffsetB.AutoSize = True
        Me.LblIoffsetB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIoffsetB.Location = New System.Drawing.Point(13, 217)
        Me.LblIoffsetB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIoffsetB.Name = "LblIoffsetB"
        Me.LblIoffsetB.Size = New System.Drawing.Size(57, 15)
        Me.LblIoffsetB.TabIndex = 26
        Me.LblIoffsetB.Text = "IoffsetB :"
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label9.Location = New System.Drawing.Point(200, 188)
        Me.Label9.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(163, 15)
        Me.Label9.TabIndex = 24
        Me.Label9.Text = "Phase B Voltage RMS Offset"
        '
        'TxtUoffsetB
        '
        Me.TxtUoffsetB.Location = New System.Drawing.Point(87, 185)
        Me.TxtUoffsetB.Name = "TxtUoffsetB"
        Me.TxtUoffsetB.Size = New System.Drawing.Size(100, 23)
        Me.TxtUoffsetB.TabIndex = 22
        Me.TxtUoffsetB.Text = "0"
        Me.TxtUoffsetB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblUoffsetB
        '
        Me.LblUoffsetB.AutoSize = True
        Me.LblUoffsetB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblUoffsetB.Location = New System.Drawing.Point(13, 188)
        Me.LblUoffsetB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblUoffsetB.Name = "LblUoffsetB"
        Me.LblUoffsetB.Size = New System.Drawing.Size(62, 15)
        Me.LblUoffsetB.TabIndex = 23
        Me.LblUoffsetB.Text = "UoffsetB :"
        '
        'PnlMeasurementCalibration
        '
        Me.PnlMeasurementCalibration.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label8)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIoffsetN)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIoffsetN)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label15)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIgainN)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIgainN)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label6)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIoffsetC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIoffsetC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label10)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtUoffsetC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblUoffsetC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label14)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIgainC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIgainC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label13)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtUgainC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblUgainC)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label11)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIoffsetB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIoffsetB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label9)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtUoffsetB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblUoffsetB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label7)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIgainB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIgainB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label5)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtUgainB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblUgainB)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label3)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label4)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label2)
        Me.PnlMeasurementCalibration.Controls.Add(Me.Label1)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIoffsetA)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIoffsetA)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtUoffsetA)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblUoffsetA)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtIgainA)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblIgainA)
        Me.PnlMeasurementCalibration.Controls.Add(Me.TxtUgainA)
        Me.PnlMeasurementCalibration.Controls.Add(Me.LblUgainA)
        Me.PnlMeasurementCalibration.Location = New System.Drawing.Point(8, 22)
        Me.PnlMeasurementCalibration.Name = "PnlMeasurementCalibration"
        Me.PnlMeasurementCalibration.Size = New System.Drawing.Size(437, 424)
        Me.PnlMeasurementCalibration.TabIndex = 6
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label8.Location = New System.Drawing.Point(200, 391)
        Me.Label8.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(207, 15)
        Me.Label8.TabIndex = 45
        Me.Label8.Text = "Sampled N line Current RMS Offset"
        '
        'TxtIoffsetN
        '
        Me.TxtIoffsetN.Location = New System.Drawing.Point(87, 388)
        Me.TxtIoffsetN.Name = "TxtIoffsetN"
        Me.TxtIoffsetN.Size = New System.Drawing.Size(100, 23)
        Me.TxtIoffsetN.TabIndex = 43
        Me.TxtIoffsetN.Text = "0"
        Me.TxtIoffsetN.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIoffsetN
        '
        Me.LblIoffsetN.AutoSize = True
        Me.LblIoffsetN.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIoffsetN.Location = New System.Drawing.Point(13, 391)
        Me.LblIoffsetN.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIoffsetN.Name = "LblIoffsetN"
        Me.LblIoffsetN.Size = New System.Drawing.Size(60, 15)
        Me.LblIoffsetN.TabIndex = 44
        Me.LblIoffsetN.Text = "IoffsetN :"
        '
        'Label15
        '
        Me.Label15.AutoSize = True
        Me.Label15.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label15.Location = New System.Drawing.Point(200, 362)
        Me.Label15.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label15.Name = "Label15"
        Me.Label15.Size = New System.Drawing.Size(202, 15)
        Me.Label15.TabIndex = 42
        Me.Label15.Text = "Sampled N line Current RMS Gain"
        '
        'TxtIgainN
        '
        Me.TxtIgainN.Location = New System.Drawing.Point(87, 359)
        Me.TxtIgainN.Name = "TxtIgainN"
        Me.TxtIgainN.Size = New System.Drawing.Size(100, 23)
        Me.TxtIgainN.TabIndex = 40
        Me.TxtIgainN.Text = "0"
        Me.TxtIgainN.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIgainN
        '
        Me.LblIgainN.AutoSize = True
        Me.LblIgainN.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIgainN.Location = New System.Drawing.Point(13, 362)
        Me.LblIgainN.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIgainN.Name = "LblIgainN"
        Me.LblIgainN.Size = New System.Drawing.Size(53, 15)
        Me.LblIgainN.TabIndex = 41
        Me.LblIgainN.Text = "IgainN :"
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label7.Location = New System.Drawing.Point(200, 159)
        Me.Label7.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(159, 15)
        Me.Label7.TabIndex = 21
        Me.Label7.Text = "Phase B Current RMS Gain"
        '
        'TxtIgainB
        '
        Me.TxtIgainB.Location = New System.Drawing.Point(87, 156)
        Me.TxtIgainB.Name = "TxtIgainB"
        Me.TxtIgainB.Size = New System.Drawing.Size(100, 23)
        Me.TxtIgainB.TabIndex = 19
        Me.TxtIgainB.Text = "0"
        Me.TxtIgainB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIgainB
        '
        Me.LblIgainB.AutoSize = True
        Me.LblIgainB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIgainB.Location = New System.Drawing.Point(13, 159)
        Me.LblIgainB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIgainB.Name = "LblIgainB"
        Me.LblIgainB.Size = New System.Drawing.Size(50, 15)
        Me.LblIgainB.TabIndex = 20
        Me.LblIgainB.Text = "IgainB :"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.Location = New System.Drawing.Point(200, 130)
        Me.Label5.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(158, 15)
        Me.Label5.TabIndex = 18
        Me.Label5.Text = "Phase B Voltage RMS Gain"
        '
        'TxtUgainB
        '
        Me.TxtUgainB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.TxtUgainB.Location = New System.Drawing.Point(87, 127)
        Me.TxtUgainB.Name = "TxtUgainB"
        Me.TxtUgainB.Size = New System.Drawing.Size(100, 23)
        Me.TxtUgainB.TabIndex = 16
        Me.TxtUgainB.Text = "0"
        Me.TxtUgainB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblUgainB
        '
        Me.LblUgainB.AutoSize = True
        Me.LblUgainB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblUgainB.Location = New System.Drawing.Point(13, 130)
        Me.LblUgainB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblUgainB.Name = "LblUgainB"
        Me.LblUgainB.Size = New System.Drawing.Size(55, 15)
        Me.LblUgainB.TabIndex = 17
        Me.LblUgainB.Text = "UgainB :"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(200, 14)
        Me.Label3.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(160, 15)
        Me.Label3.TabIndex = 15
        Me.Label3.Text = "Phase A Voltage RMS Gain"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(200, 43)
        Me.Label4.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(161, 15)
        Me.Label4.TabIndex = 14
        Me.Label4.Text = "Phase A Current RMS Gain"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(200, 72)
        Me.Label2.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(165, 15)
        Me.Label2.TabIndex = 13
        Me.Label2.Text = "Phase A Voltage RMS Offset"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(200, 101)
        Me.Label1.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(166, 15)
        Me.Label1.TabIndex = 12
        Me.Label1.Text = "Phase A Current RMS Offset"
        '
        'TxtIoffsetA
        '
        Me.TxtIoffsetA.Location = New System.Drawing.Point(87, 98)
        Me.TxtIoffsetA.Name = "TxtIoffsetA"
        Me.TxtIoffsetA.Size = New System.Drawing.Size(100, 23)
        Me.TxtIoffsetA.TabIndex = 10
        Me.TxtIoffsetA.Text = "0"
        Me.TxtIoffsetA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIoffsetA
        '
        Me.LblIoffsetA.AutoSize = True
        Me.LblIoffsetA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIoffsetA.Location = New System.Drawing.Point(13, 101)
        Me.LblIoffsetA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIoffsetA.Name = "LblIoffsetA"
        Me.LblIoffsetA.Size = New System.Drawing.Size(59, 15)
        Me.LblIoffsetA.TabIndex = 11
        Me.LblIoffsetA.Text = "IoffsetA :"
        '
        'TxtUoffsetA
        '
        Me.TxtUoffsetA.Location = New System.Drawing.Point(87, 69)
        Me.TxtUoffsetA.Name = "TxtUoffsetA"
        Me.TxtUoffsetA.Size = New System.Drawing.Size(100, 23)
        Me.TxtUoffsetA.TabIndex = 8
        Me.TxtUoffsetA.Text = "0"
        Me.TxtUoffsetA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblUoffsetA
        '
        Me.LblUoffsetA.AutoSize = True
        Me.LblUoffsetA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblUoffsetA.Location = New System.Drawing.Point(13, 72)
        Me.LblUoffsetA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblUoffsetA.Name = "LblUoffsetA"
        Me.LblUoffsetA.Size = New System.Drawing.Size(64, 15)
        Me.LblUoffsetA.TabIndex = 9
        Me.LblUoffsetA.Text = "UoffsetA :"
        '
        'TxtIgainA
        '
        Me.TxtIgainA.Location = New System.Drawing.Point(87, 40)
        Me.TxtIgainA.Name = "TxtIgainA"
        Me.TxtIgainA.Size = New System.Drawing.Size(100, 23)
        Me.TxtIgainA.TabIndex = 6
        Me.TxtIgainA.Text = "0"
        Me.TxtIgainA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblIgainA
        '
        Me.LblIgainA.AutoSize = True
        Me.LblIgainA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblIgainA.Location = New System.Drawing.Point(13, 43)
        Me.LblIgainA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblIgainA.Name = "LblIgainA"
        Me.LblIgainA.Size = New System.Drawing.Size(52, 15)
        Me.LblIgainA.TabIndex = 7
        Me.LblIgainA.Text = "IgainA :"
        '
        'TxtUgainA
        '
        Me.TxtUgainA.Location = New System.Drawing.Point(87, 11)
        Me.TxtUgainA.Name = "TxtUgainA"
        Me.TxtUgainA.Size = New System.Drawing.Size(100, 23)
        Me.TxtUgainA.TabIndex = 4
        Me.TxtUgainA.Text = "0"
        Me.TxtUgainA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblUgainA
        '
        Me.LblUgainA.AutoSize = True
        Me.LblUgainA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblUgainA.Location = New System.Drawing.Point(13, 14)
        Me.LblUgainA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblUgainA.Name = "LblUgainA"
        Me.LblUgainA.Size = New System.Drawing.Size(57, 15)
        Me.LblUgainA.TabIndex = 5
        Me.LblUgainA.Text = "UgainA :"
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 609)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Padding = New System.Windows.Forms.Padding(1, 0, 16, 0)
        Me.StatusStrip1.Size = New System.Drawing.Size(479, 22)
        Me.StatusStrip1.TabIndex = 4
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'CmdLoad
        '
        Me.CmdLoad.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdLoad.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdLoad.Location = New System.Drawing.Point(42, 453)
        Me.CmdLoad.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdLoad.Name = "CmdLoad"
        Me.CmdLoad.Size = New System.Drawing.Size(104, 30)
        Me.CmdLoad.TabIndex = 28
        Me.CmdLoad.Text = "Load Settings"
        Me.CmdLoad.UseVisualStyleBackColor = True
        '
        'CmdSave
        '
        Me.CmdSave.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdSave.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdSave.Location = New System.Drawing.Point(185, 453)
        Me.CmdSave.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdSave.Name = "CmdSave"
        Me.CmdSave.Size = New System.Drawing.Size(104, 30)
        Me.CmdSave.TabIndex = 27
        Me.CmdSave.Text = "Save Settings"
        Me.CmdSave.UseVisualStyleBackColor = True
        '
        'CmdCancel
        '
        Me.CmdCancel.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdCancel.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdCancel.Location = New System.Drawing.Point(328, 453)
        Me.CmdCancel.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdCancel.Name = "CmdCancel"
        Me.CmdCancel.Size = New System.Drawing.Size(104, 30)
        Me.CmdCancel.TabIndex = 26
        Me.CmdCancel.Text = "Cancel"
        Me.CmdCancel.UseVisualStyleBackColor = True
        '
        'GrpMeasurementCalibrationReg
        '
        Me.GrpMeasurementCalibrationReg.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpMeasurementCalibrationReg.Controls.Add(Me.CmdLoad)
        Me.GrpMeasurementCalibrationReg.Controls.Add(Me.PnlMeasurementCalibration)
        Me.GrpMeasurementCalibrationReg.Controls.Add(Me.CmdSave)
        Me.GrpMeasurementCalibrationReg.Controls.Add(Me.CmdCancel)
        Me.GrpMeasurementCalibrationReg.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpMeasurementCalibrationReg.Location = New System.Drawing.Point(12, 9)
        Me.GrpMeasurementCalibrationReg.Margin = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpMeasurementCalibrationReg.Name = "GrpMeasurementCalibrationReg"
        Me.GrpMeasurementCalibrationReg.Padding = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpMeasurementCalibrationReg.Size = New System.Drawing.Size(454, 495)
        Me.GrpMeasurementCalibrationReg.TabIndex = 5
        Me.GrpMeasurementCalibrationReg.TabStop = False
        Me.GrpMeasurementCalibrationReg.Text = "Measurement Calibration Registers"
        '
        'TimerHandler1_Client
        '
        Me.TimerHandler1_Client.Enabled = True
        Me.TimerHandler1_Client.Interval = 10
        '
        'FrmSMGE_3222M_MeasurementCalibrationWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(479, 631)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.GrpMeasurementCalibrationReg)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.Name = "FrmSMGE_3222M_MeasurementCalibrationWindow"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE3222M Measurement Calibration Window"
        Me.PnlMeasurementCalibration.ResumeLayout(False)
        Me.PnlMeasurementCalibration.PerformLayout()
        Me.GrpMeasurementCalibrationReg.ResumeLayout(False)
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents Label6 As Label
    Friend WithEvents TxtIoffsetC As TextBox
    Friend WithEvents LblIoffsetC As Label
    Friend WithEvents Label10 As Label
    Friend WithEvents TxtUoffsetC As TextBox
    Friend WithEvents LblUoffsetC As Label
    Friend WithEvents Label14 As Label
    Friend WithEvents TxtIgainC As TextBox
    Friend WithEvents LblIgainC As Label
    Friend WithEvents Label13 As Label
    Friend WithEvents TxtUgainC As TextBox
    Friend WithEvents LblUgainC As Label
    Friend WithEvents Label11 As Label
    Friend WithEvents TxtIoffsetB As TextBox
    Friend WithEvents LblIoffsetB As Label
    Friend WithEvents Label9 As Label
    Friend WithEvents TxtUoffsetB As TextBox
    Friend WithEvents LblUoffsetB As Label
    Friend WithEvents PnlMeasurementCalibration As Panel
    Friend WithEvents Label7 As Label
    Friend WithEvents TxtIgainB As TextBox
    Friend WithEvents LblIgainB As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents TxtUgainB As TextBox
    Friend WithEvents LblUgainB As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents TxtIoffsetA As TextBox
    Friend WithEvents LblIoffsetA As Label
    Friend WithEvents TxtUoffsetA As TextBox
    Friend WithEvents LblUoffsetA As Label
    Friend WithEvents TxtIgainA As TextBox
    Friend WithEvents LblIgainA As Label
    Friend WithEvents TxtUgainA As TextBox
    Friend WithEvents LblUgainA As Label
    Friend WithEvents StatusStrip1 As StatusStrip
    Friend WithEvents CmdLoad As Button
    Friend WithEvents CmdSave As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents GrpMeasurementCalibrationReg As GroupBox
    Friend WithEvents Label8 As Label
    Friend WithEvents TxtIoffsetN As TextBox
    Friend WithEvents LblIoffsetN As Label
    Friend WithEvents Label15 As Label
    Friend WithEvents TxtIgainN As TextBox
    Friend WithEvents LblIgainN As Label
    Friend WithEvents TimerHandler1_Client As Timer
End Class
