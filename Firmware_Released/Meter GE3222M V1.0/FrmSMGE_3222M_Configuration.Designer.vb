<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_Configuration
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
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.GrpConfigurationReg = New System.Windows.Forms.GroupBox()
        Me.CmdLoad = New System.Windows.Forms.Button()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.Label13 = New System.Windows.Forms.Label()
        Me.TxtSPhaseTh = New System.Windows.Forms.TextBox()
        Me.LblSPhaseTh = New System.Windows.Forms.Label()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.TxtQPhaseTh = New System.Windows.Forms.TextBox()
        Me.LblQPhaseTh = New System.Windows.Forms.Label()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.TxtPPhaseTh = New System.Windows.Forms.TextBox()
        Me.LblPPhaseTh = New System.Windows.Forms.Label()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.TxtSStartTh = New System.Windows.Forms.TextBox()
        Me.LblSStartTh = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.TxtQStartTh = New System.Windows.Forms.TextBox()
        Me.LblQStartTh = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.TxtPStartTh = New System.Windows.Forms.TextBox()
        Me.LblPStartTh = New System.Windows.Forms.Label()
        Me.TxtPMPGA = New System.Windows.Forms.TextBox()
        Me.LblPMPGA = New System.Windows.Forms.Label()
        Me.TxtMMode1 = New System.Windows.Forms.TextBox()
        Me.LblMMode1 = New System.Windows.Forms.Label()
        Me.TxtMMode0 = New System.Windows.Forms.TextBox()
        Me.LblMMode0 = New System.Windows.Forms.Label()
        Me.CmdSave = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.TimerHandler1_Client = New System.Windows.Forms.Timer(Me.components)
        Me.GrpConfigurationReg.SuspendLayout()
        Me.Panel1.SuspendLayout()
        Me.SuspendLayout()
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 639)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Padding = New System.Windows.Forms.Padding(1, 0, 16, 0)
        Me.StatusStrip1.Size = New System.Drawing.Size(579, 22)
        Me.StatusStrip1.TabIndex = 0
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'GrpConfigurationReg
        '
        Me.GrpConfigurationReg.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpConfigurationReg.Controls.Add(Me.CmdLoad)
        Me.GrpConfigurationReg.Controls.Add(Me.Panel1)
        Me.GrpConfigurationReg.Controls.Add(Me.CmdSave)
        Me.GrpConfigurationReg.Controls.Add(Me.CmdCancel)
        Me.GrpConfigurationReg.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpConfigurationReg.Location = New System.Drawing.Point(12, 12)
        Me.GrpConfigurationReg.Margin = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpConfigurationReg.Name = "GrpConfigurationReg"
        Me.GrpConfigurationReg.Padding = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpConfigurationReg.Size = New System.Drawing.Size(554, 358)
        Me.GrpConfigurationReg.TabIndex = 1
        Me.GrpConfigurationReg.TabStop = False
        Me.GrpConfigurationReg.Text = "Configuration Registers"
        '
        'CmdLoad
        '
        Me.CmdLoad.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdLoad.Location = New System.Drawing.Point(82, 316)
        Me.CmdLoad.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdLoad.Name = "CmdLoad"
        Me.CmdLoad.Size = New System.Drawing.Size(104, 30)
        Me.CmdLoad.TabIndex = 28
        Me.CmdLoad.Text = "Load Settings"
        Me.CmdLoad.UseVisualStyleBackColor = True
        '
        'Panel1
        '
        Me.Panel1.Controls.Add(Me.Label13)
        Me.Panel1.Controls.Add(Me.TxtSPhaseTh)
        Me.Panel1.Controls.Add(Me.LblSPhaseTh)
        Me.Panel1.Controls.Add(Me.Label11)
        Me.Panel1.Controls.Add(Me.TxtQPhaseTh)
        Me.Panel1.Controls.Add(Me.LblQPhaseTh)
        Me.Panel1.Controls.Add(Me.Label9)
        Me.Panel1.Controls.Add(Me.TxtPPhaseTh)
        Me.Panel1.Controls.Add(Me.LblPPhaseTh)
        Me.Panel1.Controls.Add(Me.Label7)
        Me.Panel1.Controls.Add(Me.TxtSStartTh)
        Me.Panel1.Controls.Add(Me.LblSStartTh)
        Me.Panel1.Controls.Add(Me.Label5)
        Me.Panel1.Controls.Add(Me.TxtQStartTh)
        Me.Panel1.Controls.Add(Me.LblQStartTh)
        Me.Panel1.Controls.Add(Me.Label3)
        Me.Panel1.Controls.Add(Me.Label4)
        Me.Panel1.Controls.Add(Me.Label2)
        Me.Panel1.Controls.Add(Me.Label1)
        Me.Panel1.Controls.Add(Me.TxtPStartTh)
        Me.Panel1.Controls.Add(Me.LblPStartTh)
        Me.Panel1.Controls.Add(Me.TxtPMPGA)
        Me.Panel1.Controls.Add(Me.LblPMPGA)
        Me.Panel1.Controls.Add(Me.TxtMMode1)
        Me.Panel1.Controls.Add(Me.LblMMode1)
        Me.Panel1.Controls.Add(Me.TxtMMode0)
        Me.Panel1.Controls.Add(Me.LblMMode0)
        Me.Panel1.Location = New System.Drawing.Point(8, 22)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(539, 284)
        Me.Panel1.TabIndex = 6
        '
        'Label13
        '
        Me.Label13.AutoSize = True
        Me.Label13.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label13.Location = New System.Drawing.Point(200, 246)
        Me.Label13.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label13.Name = "Label13"
        Me.Label13.Size = New System.Drawing.Size(334, 15)
        Me.Label13.TabIndex = 30
        Me.Label13.Text = "Startup Power Threshold (Apparent Energy Accumulation)"
        '
        'TxtSPhaseTh
        '
        Me.TxtSPhaseTh.Location = New System.Drawing.Point(87, 243)
        Me.TxtSPhaseTh.Name = "TxtSPhaseTh"
        Me.TxtSPhaseTh.Size = New System.Drawing.Size(100, 23)
        Me.TxtSPhaseTh.TabIndex = 28
        Me.TxtSPhaseTh.Text = "0"
        Me.TxtSPhaseTh.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblSPhaseTh
        '
        Me.LblSPhaseTh.AutoSize = True
        Me.LblSPhaseTh.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblSPhaseTh.Location = New System.Drawing.Point(13, 246)
        Me.LblSPhaseTh.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblSPhaseTh.Name = "LblSPhaseTh"
        Me.LblSPhaseTh.Size = New System.Drawing.Size(69, 15)
        Me.LblSPhaseTh.TabIndex = 29
        Me.LblSPhaseTh.Text = "SPhaseTh :"
        '
        'Label11
        '
        Me.Label11.AutoSize = True
        Me.Label11.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label11.Location = New System.Drawing.Point(200, 217)
        Me.Label11.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label11.Name = "Label11"
        Me.Label11.Size = New System.Drawing.Size(334, 15)
        Me.Label11.TabIndex = 27
        Me.Label11.Text = "Startup Power Threshold (ReActive Energy Accumulation)"
        '
        'TxtQPhaseTh
        '
        Me.TxtQPhaseTh.Location = New System.Drawing.Point(87, 214)
        Me.TxtQPhaseTh.Name = "TxtQPhaseTh"
        Me.TxtQPhaseTh.Size = New System.Drawing.Size(100, 23)
        Me.TxtQPhaseTh.TabIndex = 25
        Me.TxtQPhaseTh.Text = "0"
        Me.TxtQPhaseTh.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblQPhaseTh
        '
        Me.LblQPhaseTh.AutoSize = True
        Me.LblQPhaseTh.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblQPhaseTh.Location = New System.Drawing.Point(13, 217)
        Me.LblQPhaseTh.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblQPhaseTh.Name = "LblQPhaseTh"
        Me.LblQPhaseTh.Size = New System.Drawing.Size(72, 15)
        Me.LblQPhaseTh.TabIndex = 26
        Me.LblQPhaseTh.Text = "QPhaseTh :"
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label9.Location = New System.Drawing.Point(200, 188)
        Me.Label9.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(319, 15)
        Me.Label9.TabIndex = 24
        Me.Label9.Text = "Startup Power Threshold (Active Energy Accumulation)"
        '
        'TxtPPhaseTh
        '
        Me.TxtPPhaseTh.Location = New System.Drawing.Point(87, 185)
        Me.TxtPPhaseTh.Name = "TxtPPhaseTh"
        Me.TxtPPhaseTh.Size = New System.Drawing.Size(100, 23)
        Me.TxtPPhaseTh.TabIndex = 22
        Me.TxtPPhaseTh.Text = "0"
        Me.TxtPPhaseTh.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPPhaseTh
        '
        Me.LblPPhaseTh.AutoSize = True
        Me.LblPPhaseTh.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPPhaseTh.Location = New System.Drawing.Point(13, 188)
        Me.LblPPhaseTh.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPPhaseTh.Name = "LblPPhaseTh"
        Me.LblPPhaseTh.Size = New System.Drawing.Size(70, 15)
        Me.LblPPhaseTh.TabIndex = 23
        Me.LblPPhaseTh.Text = "PPhaseTh :"
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label7.Location = New System.Drawing.Point(200, 159)
        Me.Label7.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(205, 15)
        Me.Label7.TabIndex = 21
        Me.Label7.Text = "Apparent Startup Power Threshold."
        '
        'TxtSStartTh
        '
        Me.TxtSStartTh.Location = New System.Drawing.Point(87, 156)
        Me.TxtSStartTh.Name = "TxtSStartTh"
        Me.TxtSStartTh.Size = New System.Drawing.Size(100, 23)
        Me.TxtSStartTh.TabIndex = 19
        Me.TxtSStartTh.Text = "0"
        Me.TxtSStartTh.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblSStartTh
        '
        Me.LblSStartTh.AutoSize = True
        Me.LblSStartTh.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblSStartTh.Location = New System.Drawing.Point(13, 159)
        Me.LblSStartTh.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblSStartTh.Name = "LblSStartTh"
        Me.LblSStartTh.Size = New System.Drawing.Size(63, 15)
        Me.LblSStartTh.TabIndex = 20
        Me.LblSStartTh.Text = "SStartTh :"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.Location = New System.Drawing.Point(200, 130)
        Me.Label5.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(201, 15)
        Me.Label5.TabIndex = 18
        Me.Label5.Text = "Reactive Startup Power Threshold."
        '
        'TxtQStartTh
        '
        Me.TxtQStartTh.Location = New System.Drawing.Point(87, 127)
        Me.TxtQStartTh.Name = "TxtQStartTh"
        Me.TxtQStartTh.Size = New System.Drawing.Size(100, 23)
        Me.TxtQStartTh.TabIndex = 16
        Me.TxtQStartTh.Text = "0"
        Me.TxtQStartTh.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblQStartTh
        '
        Me.LblQStartTh.AutoSize = True
        Me.LblQStartTh.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblQStartTh.Location = New System.Drawing.Point(13, 130)
        Me.LblQStartTh.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblQStartTh.Name = "LblQStartTh"
        Me.LblQStartTh.Size = New System.Drawing.Size(66, 15)
        Me.LblQStartTh.TabIndex = 17
        Me.LblQStartTh.Text = "QStartTh :"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(200, 14)
        Me.Label3.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(180, 15)
        Me.Label3.TabIndex = 15
        Me.Label3.Text = "Metering method configuration"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(200, 43)
        Me.Label4.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(139, 15)
        Me.Label4.TabIndex = 14
        Me.Label4.Text = "PGA gain configuration"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(200, 72)
        Me.Label2.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(308, 15)
        Me.Label2.TabIndex = 13
        Me.Label2.Text = "PGAgain Configuration in Partial Measurement Mode"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(200, 101)
        Me.Label1.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(190, 15)
        Me.Label1.TabIndex = 12
        Me.Label1.Text = "Active Startup Power Threshold."
        '
        'TxtPStartTh
        '
        Me.TxtPStartTh.Location = New System.Drawing.Point(87, 98)
        Me.TxtPStartTh.Name = "TxtPStartTh"
        Me.TxtPStartTh.Size = New System.Drawing.Size(100, 23)
        Me.TxtPStartTh.TabIndex = 10
        Me.TxtPStartTh.Text = "0"
        Me.TxtPStartTh.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPStartTh
        '
        Me.LblPStartTh.AutoSize = True
        Me.LblPStartTh.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPStartTh.Location = New System.Drawing.Point(13, 101)
        Me.LblPStartTh.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPStartTh.Name = "LblPStartTh"
        Me.LblPStartTh.Size = New System.Drawing.Size(64, 15)
        Me.LblPStartTh.TabIndex = 11
        Me.LblPStartTh.Text = "PStartTh :"
        '
        'TxtPMPGA
        '
        Me.TxtPMPGA.Location = New System.Drawing.Point(87, 69)
        Me.TxtPMPGA.Name = "TxtPMPGA"
        Me.TxtPMPGA.Size = New System.Drawing.Size(100, 23)
        Me.TxtPMPGA.TabIndex = 8
        Me.TxtPMPGA.Text = "0"
        Me.TxtPMPGA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPMPGA
        '
        Me.LblPMPGA.AutoSize = True
        Me.LblPMPGA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPMPGA.Location = New System.Drawing.Point(13, 72)
        Me.LblPMPGA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPMPGA.Name = "LblPMPGA"
        Me.LblPMPGA.Size = New System.Drawing.Size(64, 15)
        Me.LblPMPGA.TabIndex = 9
        Me.LblPMPGA.Text = "PMPGA :"
        '
        'TxtMMode1
        '
        Me.TxtMMode1.Location = New System.Drawing.Point(87, 40)
        Me.TxtMMode1.Name = "TxtMMode1"
        Me.TxtMMode1.Size = New System.Drawing.Size(100, 23)
        Me.TxtMMode1.TabIndex = 6
        Me.TxtMMode1.Text = "0"
        Me.TxtMMode1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblMMode1
        '
        Me.LblMMode1.AutoSize = True
        Me.LblMMode1.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblMMode1.Location = New System.Drawing.Point(13, 43)
        Me.LblMMode1.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblMMode1.Name = "LblMMode1"
        Me.LblMMode1.Size = New System.Drawing.Size(67, 15)
        Me.LblMMode1.TabIndex = 7
        Me.LblMMode1.Text = "MMode1 :"
        '
        'TxtMMode0
        '
        Me.TxtMMode0.Location = New System.Drawing.Point(87, 11)
        Me.TxtMMode0.Name = "TxtMMode0"
        Me.TxtMMode0.Size = New System.Drawing.Size(100, 23)
        Me.TxtMMode0.TabIndex = 4
        Me.TxtMMode0.Text = "389"
        Me.TxtMMode0.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblMMode0
        '
        Me.LblMMode0.AutoSize = True
        Me.LblMMode0.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblMMode0.Location = New System.Drawing.Point(13, 14)
        Me.LblMMode0.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblMMode0.Name = "LblMMode0"
        Me.LblMMode0.Size = New System.Drawing.Size(67, 15)
        Me.LblMMode0.TabIndex = 5
        Me.LblMMode0.Text = "MMode0 :"
        '
        'CmdSave
        '
        Me.CmdSave.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdSave.Location = New System.Drawing.Point(225, 316)
        Me.CmdSave.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdSave.Name = "CmdSave"
        Me.CmdSave.Size = New System.Drawing.Size(104, 30)
        Me.CmdSave.TabIndex = 27
        Me.CmdSave.Text = "Save Settings"
        Me.CmdSave.UseVisualStyleBackColor = True
        '
        'CmdCancel
        '
        Me.CmdCancel.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdCancel.Location = New System.Drawing.Point(368, 316)
        Me.CmdCancel.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdCancel.Name = "CmdCancel"
        Me.CmdCancel.Size = New System.Drawing.Size(104, 30)
        Me.CmdCancel.TabIndex = 26
        Me.CmdCancel.Text = "Cancel"
        Me.CmdCancel.UseVisualStyleBackColor = True
        '
        'TimerHandler1_Client
        '
        Me.TimerHandler1_Client.Enabled = True
        Me.TimerHandler1_Client.Interval = 10
        '
        'FrmSMGE_3222M_Configuration
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(7.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(579, 661)
        Me.Controls.Add(Me.GrpConfigurationReg)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Font = New System.Drawing.Font("Calisto MT", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Margin = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.MaximizeBox = False
        Me.Name = "FrmSMGE_3222M_Configuration"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE3222M Configuration"
        Me.GrpConfigurationReg.ResumeLayout(False)
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents StatusStrip1 As StatusStrip
    Friend WithEvents GrpConfigurationReg As GroupBox
    Friend WithEvents Panel1 As Panel
    Friend WithEvents TxtMMode0 As TextBox
    Friend WithEvents LblMMode0 As Label
    Friend WithEvents TxtMMode1 As TextBox
    Friend WithEvents LblMMode1 As Label
    Friend WithEvents TxtPMPGA As TextBox
    Friend WithEvents LblPMPGA As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents TxtPStartTh As TextBox
    Friend WithEvents LblPStartTh As Label
    Friend WithEvents Label13 As Label
    Friend WithEvents TxtSPhaseTh As TextBox
    Friend WithEvents LblSPhaseTh As Label
    Friend WithEvents Label11 As Label
    Friend WithEvents TxtQPhaseTh As TextBox
    Friend WithEvents LblQPhaseTh As Label
    Friend WithEvents Label9 As Label
    Friend WithEvents TxtPPhaseTh As TextBox
    Friend WithEvents LblPPhaseTh As Label
    Friend WithEvents Label7 As Label
    Friend WithEvents TxtSStartTh As TextBox
    Friend WithEvents LblSStartTh As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents TxtQStartTh As TextBox
    Friend WithEvents LblQStartTh As Label
    Friend WithEvents CmdLoad As Button
    Friend WithEvents CmdSave As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents TimerHandler1_Client As Timer
End Class
