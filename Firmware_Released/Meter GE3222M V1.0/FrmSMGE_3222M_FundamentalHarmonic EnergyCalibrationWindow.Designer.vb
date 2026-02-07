<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow
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
        Me.PnlFundamentalCalibration = New System.Windows.Forms.Panel()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.TxtPGainCF = New System.Windows.Forms.TextBox()
        Me.LblPGainCF = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.TxtPGainBF = New System.Windows.Forms.TextBox()
        Me.LblPGainBF = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.TxtPGainAF = New System.Windows.Forms.TextBox()
        Me.LblPGainAF = New System.Windows.Forms.Label()
        Me.TxtPOffsetCF = New System.Windows.Forms.TextBox()
        Me.LblPOffsetCF = New System.Windows.Forms.Label()
        Me.TxtPOffsetBF = New System.Windows.Forms.TextBox()
        Me.LblPOffsetBF = New System.Windows.Forms.Label()
        Me.TxtPOffsetAF = New System.Windows.Forms.TextBox()
        Me.LblPOffsetAF = New System.Windows.Forms.Label()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.CmdLoad = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.GrpFundamentalCalibrationReg = New System.Windows.Forms.GroupBox()
        Me.TimerHandler1_Client = New System.Windows.Forms.Timer(Me.components)
        Me.CmdSave = New System.Windows.Forms.Button()
        Me.PnlFundamentalCalibration.SuspendLayout()
        Me.GrpFundamentalCalibrationReg.SuspendLayout()
        Me.SuspendLayout()
        '
        'PnlFundamentalCalibration
        '
        Me.PnlFundamentalCalibration.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.PnlFundamentalCalibration.Controls.Add(Me.Label7)
        Me.PnlFundamentalCalibration.Controls.Add(Me.TxtPGainCF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.LblPGainCF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.Label5)
        Me.PnlFundamentalCalibration.Controls.Add(Me.TxtPGainBF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.LblPGainBF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.Label3)
        Me.PnlFundamentalCalibration.Controls.Add(Me.Label4)
        Me.PnlFundamentalCalibration.Controls.Add(Me.Label2)
        Me.PnlFundamentalCalibration.Controls.Add(Me.Label1)
        Me.PnlFundamentalCalibration.Controls.Add(Me.TxtPGainAF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.LblPGainAF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.TxtPOffsetCF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.LblPOffsetCF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.TxtPOffsetBF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.LblPOffsetBF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.TxtPOffsetAF)
        Me.PnlFundamentalCalibration.Controls.Add(Me.LblPOffsetAF)
        Me.PnlFundamentalCalibration.Location = New System.Drawing.Point(8, 22)
        Me.PnlFundamentalCalibration.Name = "PnlFundamentalCalibration"
        Me.PnlFundamentalCalibration.Size = New System.Drawing.Size(462, 191)
        Me.PnlFundamentalCalibration.TabIndex = 6
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label7.Location = New System.Drawing.Point(200, 159)
        Me.Label7.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(237, 15)
        Me.Label7.TabIndex = 21
        Me.Label7.Text = "Phase C Fundamental Active Power Gain"
        '
        'TxtPGainCF
        '
        Me.TxtPGainCF.Location = New System.Drawing.Point(87, 156)
        Me.TxtPGainCF.Name = "TxtPGainCF"
        Me.TxtPGainCF.Size = New System.Drawing.Size(100, 23)
        Me.TxtPGainCF.TabIndex = 19
        Me.TxtPGainCF.Text = "0"
        Me.TxtPGainCF.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPGainCF
        '
        Me.LblPGainCF.AutoSize = True
        Me.LblPGainCF.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPGainCF.Location = New System.Drawing.Point(13, 159)
        Me.LblPGainCF.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPGainCF.Name = "LblPGainCF"
        Me.LblPGainCF.Size = New System.Drawing.Size(67, 15)
        Me.LblPGainCF.TabIndex = 20
        Me.LblPGainCF.Text = "PGainCF :"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.Location = New System.Drawing.Point(200, 130)
        Me.Label5.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(236, 15)
        Me.Label5.TabIndex = 18
        Me.Label5.Text = "Phase B Fundamental Active Power Gain"
        '
        'TxtPGainBF
        '
        Me.TxtPGainBF.Location = New System.Drawing.Point(87, 127)
        Me.TxtPGainBF.Name = "TxtPGainBF"
        Me.TxtPGainBF.Size = New System.Drawing.Size(100, 23)
        Me.TxtPGainBF.TabIndex = 16
        Me.TxtPGainBF.Text = "0"
        Me.TxtPGainBF.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPGainBF
        '
        Me.LblPGainBF.AutoSize = True
        Me.LblPGainBF.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPGainBF.Location = New System.Drawing.Point(13, 130)
        Me.LblPGainBF.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPGainBF.Name = "LblPGainBF"
        Me.LblPGainBF.Size = New System.Drawing.Size(66, 15)
        Me.LblPGainBF.TabIndex = 17
        Me.LblPGainBF.Text = "PGainBF :"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(200, 14)
        Me.Label3.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(243, 15)
        Me.Label3.TabIndex = 15
        Me.Label3.Text = "Phase A Fundamental Active Power Offset"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(200, 43)
        Me.Label4.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(241, 15)
        Me.Label4.TabIndex = 14
        Me.Label4.Text = "Phase B Fundamental Active Power Offset"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(200, 72)
        Me.Label2.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(242, 15)
        Me.Label2.TabIndex = 13
        Me.Label2.Text = "Phase C Fundamental Active Power Offset"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(200, 101)
        Me.Label1.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(238, 15)
        Me.Label1.TabIndex = 12
        Me.Label1.Text = "Phase A Fundamental Active Power Gain"
        '
        'TxtPGainAF
        '
        Me.TxtPGainAF.Location = New System.Drawing.Point(87, 98)
        Me.TxtPGainAF.Name = "TxtPGainAF"
        Me.TxtPGainAF.Size = New System.Drawing.Size(100, 23)
        Me.TxtPGainAF.TabIndex = 10
        Me.TxtPGainAF.Text = "0"
        Me.TxtPGainAF.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPGainAF
        '
        Me.LblPGainAF.AutoSize = True
        Me.LblPGainAF.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPGainAF.Location = New System.Drawing.Point(13, 101)
        Me.LblPGainAF.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPGainAF.Name = "LblPGainAF"
        Me.LblPGainAF.Size = New System.Drawing.Size(68, 15)
        Me.LblPGainAF.TabIndex = 11
        Me.LblPGainAF.Text = "PGainAF :"
        '
        'TxtPOffsetCF
        '
        Me.TxtPOffsetCF.Location = New System.Drawing.Point(87, 69)
        Me.TxtPOffsetCF.Name = "TxtPOffsetCF"
        Me.TxtPOffsetCF.Size = New System.Drawing.Size(100, 23)
        Me.TxtPOffsetCF.TabIndex = 8
        Me.TxtPOffsetCF.Text = "0"
        Me.TxtPOffsetCF.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPOffsetCF
        '
        Me.LblPOffsetCF.AutoSize = True
        Me.LblPOffsetCF.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPOffsetCF.Location = New System.Drawing.Point(13, 72)
        Me.LblPOffsetCF.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPOffsetCF.Name = "LblPOffsetCF"
        Me.LblPOffsetCF.Size = New System.Drawing.Size(72, 15)
        Me.LblPOffsetCF.TabIndex = 9
        Me.LblPOffsetCF.Text = "POffsetCF :"
        '
        'TxtPOffsetBF
        '
        Me.TxtPOffsetBF.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.TxtPOffsetBF.Location = New System.Drawing.Point(87, 40)
        Me.TxtPOffsetBF.Name = "TxtPOffsetBF"
        Me.TxtPOffsetBF.Size = New System.Drawing.Size(100, 23)
        Me.TxtPOffsetBF.TabIndex = 6
        Me.TxtPOffsetBF.Text = "0"
        Me.TxtPOffsetBF.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPOffsetBF
        '
        Me.LblPOffsetBF.AutoSize = True
        Me.LblPOffsetBF.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPOffsetBF.Location = New System.Drawing.Point(13, 43)
        Me.LblPOffsetBF.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPOffsetBF.Name = "LblPOffsetBF"
        Me.LblPOffsetBF.Size = New System.Drawing.Size(71, 15)
        Me.LblPOffsetBF.TabIndex = 7
        Me.LblPOffsetBF.Text = "POffsetBF :"
        '
        'TxtPOffsetAF
        '
        Me.TxtPOffsetAF.Location = New System.Drawing.Point(87, 11)
        Me.TxtPOffsetAF.Name = "TxtPOffsetAF"
        Me.TxtPOffsetAF.Size = New System.Drawing.Size(100, 23)
        Me.TxtPOffsetAF.TabIndex = 4
        Me.TxtPOffsetAF.Text = "0"
        Me.TxtPOffsetAF.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPOffsetAF
        '
        Me.LblPOffsetAF.AutoSize = True
        Me.LblPOffsetAF.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPOffsetAF.Location = New System.Drawing.Point(13, 14)
        Me.LblPOffsetAF.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPOffsetAF.Name = "LblPOffsetAF"
        Me.LblPOffsetAF.Size = New System.Drawing.Size(73, 15)
        Me.LblPOffsetAF.TabIndex = 5
        Me.LblPOffsetAF.Text = "POffsetAF :"
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 279)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Padding = New System.Windows.Forms.Padding(1, 0, 16, 0)
        Me.StatusStrip1.Size = New System.Drawing.Size(504, 22)
        Me.StatusStrip1.TabIndex = 4
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'CmdLoad
        '
        Me.CmdLoad.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdLoad.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdLoad.Location = New System.Drawing.Point(42, 223)
        Me.CmdLoad.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdLoad.Name = "CmdLoad"
        Me.CmdLoad.Size = New System.Drawing.Size(104, 30)
        Me.CmdLoad.TabIndex = 28
        Me.CmdLoad.Text = "Load Settings"
        Me.CmdLoad.UseVisualStyleBackColor = True
        '
        'CmdCancel
        '
        Me.CmdCancel.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdCancel.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdCancel.Location = New System.Drawing.Point(328, 223)
        Me.CmdCancel.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdCancel.Name = "CmdCancel"
        Me.CmdCancel.Size = New System.Drawing.Size(104, 30)
        Me.CmdCancel.TabIndex = 26
        Me.CmdCancel.Text = "Cancel"
        Me.CmdCancel.UseVisualStyleBackColor = True
        '
        'GrpFundamentalCalibrationReg
        '
        Me.GrpFundamentalCalibrationReg.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpFundamentalCalibrationReg.Controls.Add(Me.CmdSave)
        Me.GrpFundamentalCalibrationReg.Controls.Add(Me.CmdLoad)
        Me.GrpFundamentalCalibrationReg.Controls.Add(Me.PnlFundamentalCalibration)
        Me.GrpFundamentalCalibrationReg.Controls.Add(Me.CmdCancel)
        Me.GrpFundamentalCalibrationReg.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpFundamentalCalibrationReg.Location = New System.Drawing.Point(12, 4)
        Me.GrpFundamentalCalibrationReg.Margin = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpFundamentalCalibrationReg.Name = "GrpFundamentalCalibrationReg"
        Me.GrpFundamentalCalibrationReg.Padding = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpFundamentalCalibrationReg.Size = New System.Drawing.Size(479, 265)
        Me.GrpFundamentalCalibrationReg.TabIndex = 5
        Me.GrpFundamentalCalibrationReg.TabStop = False
        Me.GrpFundamentalCalibrationReg.Text = "Fundamental/ Harmonic Energy Calibration registers"
        '
        'TimerHandler1_Client
        '
        Me.TimerHandler1_Client.Enabled = True
        Me.TimerHandler1_Client.Interval = 10
        '
        'CmdSave
        '
        Me.CmdSave.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdSave.Location = New System.Drawing.Point(185, 223)
        Me.CmdSave.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdSave.Name = "CmdSave"
        Me.CmdSave.Size = New System.Drawing.Size(104, 30)
        Me.CmdSave.TabIndex = 29
        Me.CmdSave.Text = "Save Settings"
        Me.CmdSave.UseVisualStyleBackColor = True
        '
        'FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(504, 301)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.GrpFundamentalCalibrationReg)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Name = "FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE3222M Fundamental / Harmonic Energy Calibration Window"
        Me.PnlFundamentalCalibration.ResumeLayout(False)
        Me.PnlFundamentalCalibration.PerformLayout()
        Me.GrpFundamentalCalibrationReg.ResumeLayout(False)
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents PnlFundamentalCalibration As Panel
    Friend WithEvents Label7 As Label
    Friend WithEvents TxtPGainCF As TextBox
    Friend WithEvents LblPGainCF As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents TxtPGainBF As TextBox
    Friend WithEvents LblPGainBF As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents TxtPGainAF As TextBox
    Friend WithEvents LblPGainAF As Label
    Friend WithEvents TxtPOffsetCF As TextBox
    Friend WithEvents LblPOffsetCF As Label
    Friend WithEvents TxtPOffsetBF As TextBox
    Friend WithEvents LblPOffsetBF As Label
    Friend WithEvents TxtPOffsetAF As TextBox
    Friend WithEvents LblPOffsetAF As Label
    Friend WithEvents StatusStrip1 As StatusStrip
    Friend WithEvents CmdLoad As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents GrpFundamentalCalibrationReg As GroupBox
    Friend WithEvents TimerHandler1_Client As Timer
    Friend WithEvents CmdSave As Button
End Class
