<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class FrmSMGE_3222M_CommonCalibrationWindow
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
        Me.CmdLoad = New System.Windows.Forms.Button()
        Me.CmdSave = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.Label13 = New System.Windows.Forms.Label()
        Me.TxtGainB = New System.Windows.Forms.TextBox()
        Me.LblGainB = New System.Windows.Forms.Label()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.TxtPhiA = New System.Windows.Forms.TextBox()
        Me.LblPhiA = New System.Windows.Forms.Label()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.TxtGainA = New System.Windows.Forms.TextBox()
        Me.LblGainA = New System.Windows.Forms.Label()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.TxtQOffsetC = New System.Windows.Forms.TextBox()
        Me.LblQOffsetC = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.TxtPOffsetC = New System.Windows.Forms.TextBox()
        Me.LblPOffsetC = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.TxtQOffsetB = New System.Windows.Forms.TextBox()
        Me.LblQOffsetB = New System.Windows.Forms.Label()
        Me.TxtPOffsetB = New System.Windows.Forms.TextBox()
        Me.LblPOffsetB = New System.Windows.Forms.Label()
        Me.TxtQoffsetA = New System.Windows.Forms.TextBox()
        Me.LblQoffsetA = New System.Windows.Forms.Label()
        Me.TxtPoffsetA = New System.Windows.Forms.TextBox()
        Me.LblPoffsetA = New System.Windows.Forms.Label()
        Me.GrpCommonCalibrationReg = New System.Windows.Forms.GroupBox()
        Me.PnlCommonCalibration = New System.Windows.Forms.Panel()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.TxtPhiC = New System.Windows.Forms.TextBox()
        Me.LblPhiC = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.TxtGainC = New System.Windows.Forms.TextBox()
        Me.LblGainC = New System.Windows.Forms.Label()
        Me.Label14 = New System.Windows.Forms.Label()
        Me.TxtPhiB = New System.Windows.Forms.TextBox()
        Me.LblPhiB = New System.Windows.Forms.Label()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.TimerHandler1_Client = New System.Windows.Forms.Timer(Me.components)
        Me.GrpCommonCalibrationReg.SuspendLayout()
        Me.PnlCommonCalibration.SuspendLayout()
        Me.SuspendLayout()
        '
        'CmdLoad
        '
        Me.CmdLoad.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdLoad.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdLoad.Location = New System.Drawing.Point(42, 401)
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
        Me.CmdSave.Location = New System.Drawing.Point(185, 401)
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
        Me.CmdCancel.Location = New System.Drawing.Point(328, 401)
        Me.CmdCancel.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdCancel.Name = "CmdCancel"
        Me.CmdCancel.Size = New System.Drawing.Size(104, 30)
        Me.CmdCancel.TabIndex = 26
        Me.CmdCancel.Text = "Cancel"
        Me.CmdCancel.UseVisualStyleBackColor = True
        '
        'Label13
        '
        Me.Label13.AutoSize = True
        Me.Label13.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label13.Location = New System.Drawing.Point(200, 246)
        Me.Label13.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label13.Name = "Label13"
        Me.Label13.Size = New System.Drawing.Size(139, 15)
        Me.Label13.TabIndex = 30
        Me.Label13.Text = "Phase B calibration gain"
        '
        'TxtGainB
        '
        Me.TxtGainB.Location = New System.Drawing.Point(87, 243)
        Me.TxtGainB.Name = "TxtGainB"
        Me.TxtGainB.Size = New System.Drawing.Size(100, 23)
        Me.TxtGainB.TabIndex = 28
        Me.TxtGainB.Text = "0"
        Me.TxtGainB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblGainB
        '
        Me.LblGainB.AutoSize = True
        Me.LblGainB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblGainB.Location = New System.Drawing.Point(13, 246)
        Me.LblGainB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblGainB.Name = "LblGainB"
        Me.LblGainB.Size = New System.Drawing.Size(50, 15)
        Me.LblGainB.TabIndex = 29
        Me.LblGainB.Text = "GainB :"
        '
        'Label11
        '
        Me.Label11.AutoSize = True
        Me.Label11.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label11.Location = New System.Drawing.Point(200, 217)
        Me.Label11.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label11.Name = "Label11"
        Me.Label11.Size = New System.Drawing.Size(181, 15)
        Me.Label11.TabIndex = 27
        Me.Label11.Text = "Phase A calibration phase angle"
        '
        'TxtPhiA
        '
        Me.TxtPhiA.Location = New System.Drawing.Point(87, 214)
        Me.TxtPhiA.Name = "TxtPhiA"
        Me.TxtPhiA.Size = New System.Drawing.Size(100, 23)
        Me.TxtPhiA.TabIndex = 25
        Me.TxtPhiA.Text = "0"
        Me.TxtPhiA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPhiA
        '
        Me.LblPhiA.AutoSize = True
        Me.LblPhiA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPhiA.Location = New System.Drawing.Point(13, 217)
        Me.LblPhiA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPhiA.Name = "LblPhiA"
        Me.LblPhiA.Size = New System.Drawing.Size(43, 15)
        Me.LblPhiA.TabIndex = 26
        Me.LblPhiA.Text = "PhiA :"
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label9.Location = New System.Drawing.Point(200, 188)
        Me.Label9.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(141, 15)
        Me.Label9.TabIndex = 24
        Me.Label9.Text = "Phase A calibration gain"
        '
        'TxtGainA
        '
        Me.TxtGainA.Location = New System.Drawing.Point(87, 185)
        Me.TxtGainA.Name = "TxtGainA"
        Me.TxtGainA.Size = New System.Drawing.Size(100, 23)
        Me.TxtGainA.TabIndex = 22
        Me.TxtGainA.Text = "0"
        Me.TxtGainA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblGainA
        '
        Me.LblGainA.AutoSize = True
        Me.LblGainA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblGainA.Location = New System.Drawing.Point(13, 188)
        Me.LblGainA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblGainA.Name = "LblGainA"
        Me.LblGainA.Size = New System.Drawing.Size(52, 15)
        Me.LblGainA.TabIndex = 23
        Me.LblGainA.Text = "GainA :"
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label7.Location = New System.Drawing.Point(200, 159)
        Me.Label7.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(177, 15)
        Me.Label7.TabIndex = 21
        Me.Label7.Text = "Phase C Reactive Power Offset"
        '
        'TxtQOffsetC
        '
        Me.TxtQOffsetC.Location = New System.Drawing.Point(87, 156)
        Me.TxtQOffsetC.Name = "TxtQOffsetC"
        Me.TxtQOffsetC.Size = New System.Drawing.Size(100, 23)
        Me.TxtQOffsetC.TabIndex = 19
        Me.TxtQOffsetC.Text = "0"
        Me.TxtQOffsetC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblQOffsetC
        '
        Me.LblQOffsetC.AutoSize = True
        Me.LblQOffsetC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblQOffsetC.Location = New System.Drawing.Point(13, 159)
        Me.LblQOffsetC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblQOffsetC.Name = "LblQOffsetC"
        Me.LblQOffsetC.Size = New System.Drawing.Size(66, 15)
        Me.LblQOffsetC.TabIndex = 20
        Me.LblQOffsetC.Text = "QOffsetC :"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.Location = New System.Drawing.Point(200, 130)
        Me.Label5.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(170, 15)
        Me.Label5.TabIndex = 18
        Me.Label5.Text = "Phase C Active Power Offset."
        '
        'TxtPOffsetC
        '
        Me.TxtPOffsetC.Location = New System.Drawing.Point(87, 127)
        Me.TxtPOffsetC.Name = "TxtPOffsetC"
        Me.TxtPOffsetC.Size = New System.Drawing.Size(100, 23)
        Me.TxtPOffsetC.TabIndex = 16
        Me.TxtPOffsetC.Text = "0"
        Me.TxtPOffsetC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPOffsetC
        '
        Me.LblPOffsetC.AutoSize = True
        Me.LblPOffsetC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPOffsetC.Location = New System.Drawing.Point(13, 130)
        Me.LblPOffsetC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPOffsetC.Name = "LblPOffsetC"
        Me.LblPOffsetC.Size = New System.Drawing.Size(64, 15)
        Me.LblPOffsetC.TabIndex = 17
        Me.LblPOffsetC.Text = "POffsetC :"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(200, 14)
        Me.Label3.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(167, 15)
        Me.Label3.TabIndex = 15
        Me.Label3.Text = "Phase A Active Power Offset"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(200, 43)
        Me.Label4.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(178, 15)
        Me.Label4.TabIndex = 14
        Me.Label4.Text = "Phase A Reactive Power Offset"
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
        Me.Label2.Text = "Phase B Active Power Offset"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(200, 101)
        Me.Label1.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(176, 15)
        Me.Label1.TabIndex = 12
        Me.Label1.Text = "Phase B Reactive Power Offset"
        '
        'TxtQOffsetB
        '
        Me.TxtQOffsetB.Location = New System.Drawing.Point(87, 98)
        Me.TxtQOffsetB.Name = "TxtQOffsetB"
        Me.TxtQOffsetB.Size = New System.Drawing.Size(100, 23)
        Me.TxtQOffsetB.TabIndex = 10
        Me.TxtQOffsetB.Text = "0"
        Me.TxtQOffsetB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblQOffsetB
        '
        Me.LblQOffsetB.AutoSize = True
        Me.LblQOffsetB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblQOffsetB.Location = New System.Drawing.Point(13, 101)
        Me.LblQOffsetB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblQOffsetB.Name = "LblQOffsetB"
        Me.LblQOffsetB.Size = New System.Drawing.Size(65, 15)
        Me.LblQOffsetB.TabIndex = 11
        Me.LblQOffsetB.Text = "QOffsetB :"
        '
        'TxtPOffsetB
        '
        Me.TxtPOffsetB.Location = New System.Drawing.Point(87, 69)
        Me.TxtPOffsetB.Name = "TxtPOffsetB"
        Me.TxtPOffsetB.Size = New System.Drawing.Size(100, 23)
        Me.TxtPOffsetB.TabIndex = 8
        Me.TxtPOffsetB.Text = "0"
        Me.TxtPOffsetB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPOffsetB
        '
        Me.LblPOffsetB.AutoSize = True
        Me.LblPOffsetB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPOffsetB.Location = New System.Drawing.Point(13, 72)
        Me.LblPOffsetB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPOffsetB.Name = "LblPOffsetB"
        Me.LblPOffsetB.Size = New System.Drawing.Size(63, 15)
        Me.LblPOffsetB.TabIndex = 9
        Me.LblPOffsetB.Text = "POffsetB :"
        '
        'TxtQoffsetA
        '
        Me.TxtQoffsetA.Location = New System.Drawing.Point(87, 40)
        Me.TxtQoffsetA.Name = "TxtQoffsetA"
        Me.TxtQoffsetA.Size = New System.Drawing.Size(100, 23)
        Me.TxtQoffsetA.TabIndex = 6
        Me.TxtQoffsetA.Text = "0"
        Me.TxtQoffsetA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblQoffsetA
        '
        Me.LblQoffsetA.AutoSize = True
        Me.LblQoffsetA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblQoffsetA.Location = New System.Drawing.Point(13, 43)
        Me.LblQoffsetA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblQoffsetA.Name = "LblQoffsetA"
        Me.LblQoffsetA.Size = New System.Drawing.Size(64, 15)
        Me.LblQoffsetA.TabIndex = 7
        Me.LblQoffsetA.Text = "QoffsetA :"
        '
        'TxtPoffsetA
        '
        Me.TxtPoffsetA.Location = New System.Drawing.Point(87, 11)
        Me.TxtPoffsetA.Name = "TxtPoffsetA"
        Me.TxtPoffsetA.Size = New System.Drawing.Size(100, 23)
        Me.TxtPoffsetA.TabIndex = 4
        Me.TxtPoffsetA.Text = "0"
        Me.TxtPoffsetA.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPoffsetA
        '
        Me.LblPoffsetA.AutoSize = True
        Me.LblPoffsetA.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPoffsetA.Location = New System.Drawing.Point(13, 14)
        Me.LblPoffsetA.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPoffsetA.Name = "LblPoffsetA"
        Me.LblPoffsetA.Size = New System.Drawing.Size(62, 15)
        Me.LblPoffsetA.TabIndex = 5
        Me.LblPoffsetA.Text = "PoffsetA :"
        '
        'GrpCommonCalibrationReg
        '
        Me.GrpCommonCalibrationReg.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpCommonCalibrationReg.Controls.Add(Me.CmdLoad)
        Me.GrpCommonCalibrationReg.Controls.Add(Me.PnlCommonCalibration)
        Me.GrpCommonCalibrationReg.Controls.Add(Me.CmdSave)
        Me.GrpCommonCalibrationReg.Controls.Add(Me.CmdCancel)
        Me.GrpCommonCalibrationReg.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpCommonCalibrationReg.Location = New System.Drawing.Point(12, 9)
        Me.GrpCommonCalibrationReg.Margin = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpCommonCalibrationReg.Name = "GrpCommonCalibrationReg"
        Me.GrpCommonCalibrationReg.Padding = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpCommonCalibrationReg.Size = New System.Drawing.Size(454, 443)
        Me.GrpCommonCalibrationReg.TabIndex = 3
        Me.GrpCommonCalibrationReg.TabStop = False
        Me.GrpCommonCalibrationReg.Text = "Common Calibration Registers"
        '
        'PnlCommonCalibration
        '
        Me.PnlCommonCalibration.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.PnlCommonCalibration.Controls.Add(Me.Label6)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtPhiC)
        Me.PnlCommonCalibration.Controls.Add(Me.LblPhiC)
        Me.PnlCommonCalibration.Controls.Add(Me.Label10)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtGainC)
        Me.PnlCommonCalibration.Controls.Add(Me.LblGainC)
        Me.PnlCommonCalibration.Controls.Add(Me.Label14)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtPhiB)
        Me.PnlCommonCalibration.Controls.Add(Me.LblPhiB)
        Me.PnlCommonCalibration.Controls.Add(Me.Label13)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtGainB)
        Me.PnlCommonCalibration.Controls.Add(Me.LblGainB)
        Me.PnlCommonCalibration.Controls.Add(Me.Label11)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtPhiA)
        Me.PnlCommonCalibration.Controls.Add(Me.LblPhiA)
        Me.PnlCommonCalibration.Controls.Add(Me.Label9)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtGainA)
        Me.PnlCommonCalibration.Controls.Add(Me.LblGainA)
        Me.PnlCommonCalibration.Controls.Add(Me.Label7)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtQOffsetC)
        Me.PnlCommonCalibration.Controls.Add(Me.LblQOffsetC)
        Me.PnlCommonCalibration.Controls.Add(Me.Label5)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtPOffsetC)
        Me.PnlCommonCalibration.Controls.Add(Me.LblPOffsetC)
        Me.PnlCommonCalibration.Controls.Add(Me.Label3)
        Me.PnlCommonCalibration.Controls.Add(Me.Label4)
        Me.PnlCommonCalibration.Controls.Add(Me.Label2)
        Me.PnlCommonCalibration.Controls.Add(Me.Label1)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtQOffsetB)
        Me.PnlCommonCalibration.Controls.Add(Me.LblQOffsetB)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtPOffsetB)
        Me.PnlCommonCalibration.Controls.Add(Me.LblPOffsetB)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtQoffsetA)
        Me.PnlCommonCalibration.Controls.Add(Me.LblQoffsetA)
        Me.PnlCommonCalibration.Controls.Add(Me.TxtPoffsetA)
        Me.PnlCommonCalibration.Controls.Add(Me.LblPoffsetA)
        Me.PnlCommonCalibration.Location = New System.Drawing.Point(8, 22)
        Me.PnlCommonCalibration.Name = "PnlCommonCalibration"
        Me.PnlCommonCalibration.Size = New System.Drawing.Size(437, 369)
        Me.PnlCommonCalibration.TabIndex = 6
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label6.Location = New System.Drawing.Point(200, 333)
        Me.Label6.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(180, 15)
        Me.Label6.TabIndex = 39
        Me.Label6.Text = "Phase C calibration phase angle"
        '
        'TxtPhiC
        '
        Me.TxtPhiC.Location = New System.Drawing.Point(87, 330)
        Me.TxtPhiC.Name = "TxtPhiC"
        Me.TxtPhiC.Size = New System.Drawing.Size(100, 23)
        Me.TxtPhiC.TabIndex = 37
        Me.TxtPhiC.Text = "0"
        Me.TxtPhiC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPhiC
        '
        Me.LblPhiC.AutoSize = True
        Me.LblPhiC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPhiC.Location = New System.Drawing.Point(13, 333)
        Me.LblPhiC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPhiC.Name = "LblPhiC"
        Me.LblPhiC.Size = New System.Drawing.Size(42, 15)
        Me.LblPhiC.TabIndex = 38
        Me.LblPhiC.Text = "PhiC :"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label10.Location = New System.Drawing.Point(200, 304)
        Me.Label10.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(140, 15)
        Me.Label10.TabIndex = 36
        Me.Label10.Text = "Phase C calibration gain"
        '
        'TxtGainC
        '
        Me.TxtGainC.Location = New System.Drawing.Point(87, 301)
        Me.TxtGainC.Name = "TxtGainC"
        Me.TxtGainC.Size = New System.Drawing.Size(100, 23)
        Me.TxtGainC.TabIndex = 34
        Me.TxtGainC.Text = "0"
        Me.TxtGainC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblGainC
        '
        Me.LblGainC.AutoSize = True
        Me.LblGainC.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblGainC.Location = New System.Drawing.Point(13, 304)
        Me.LblGainC.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblGainC.Name = "LblGainC"
        Me.LblGainC.Size = New System.Drawing.Size(51, 15)
        Me.LblGainC.TabIndex = 35
        Me.LblGainC.Text = "GainC :"
        '
        'Label14
        '
        Me.Label14.AutoSize = True
        Me.Label14.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label14.Location = New System.Drawing.Point(200, 275)
        Me.Label14.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.Label14.Name = "Label14"
        Me.Label14.Size = New System.Drawing.Size(179, 15)
        Me.Label14.TabIndex = 33
        Me.Label14.Text = "Phase B calibration phase angle"
        '
        'TxtPhiB
        '
        Me.TxtPhiB.Location = New System.Drawing.Point(87, 272)
        Me.TxtPhiB.Name = "TxtPhiB"
        Me.TxtPhiB.Size = New System.Drawing.Size(100, 23)
        Me.TxtPhiB.TabIndex = 31
        Me.TxtPhiB.Text = "0"
        Me.TxtPhiB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center
        '
        'LblPhiB
        '
        Me.LblPhiB.AutoSize = True
        Me.LblPhiB.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblPhiB.Location = New System.Drawing.Point(13, 275)
        Me.LblPhiB.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblPhiB.Name = "LblPhiB"
        Me.LblPhiB.Size = New System.Drawing.Size(41, 15)
        Me.LblPhiB.TabIndex = 32
        Me.LblPhiB.Text = "PhiB :"
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 469)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Padding = New System.Windows.Forms.Padding(1, 0, 16, 0)
        Me.StatusStrip1.Size = New System.Drawing.Size(479, 22)
        Me.StatusStrip1.TabIndex = 2
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'TimerHandler1_Client
        '
        Me.TimerHandler1_Client.Enabled = True
        Me.TimerHandler1_Client.Interval = 10
        '
        'FrmSMGE_3222M_CommonCalibrationWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(479, 491)
        Me.Controls.Add(Me.GrpCommonCalibrationReg)
        Me.Controls.Add(Me.StatusStrip1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.Name = "FrmSMGE_3222M_CommonCalibrationWindow"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE 3222M Common Calibration Window"
        Me.GrpCommonCalibrationReg.ResumeLayout(False)
        Me.PnlCommonCalibration.ResumeLayout(False)
        Me.PnlCommonCalibration.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents CmdLoad As Button
    Friend WithEvents CmdSave As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents Label13 As Label
    Friend WithEvents TxtGainB As TextBox
    Friend WithEvents LblGainB As Label
    Friend WithEvents Label11 As Label
    Friend WithEvents TxtPhiA As TextBox
    Friend WithEvents LblPhiA As Label
    Friend WithEvents Label9 As Label
    Friend WithEvents TxtGainA As TextBox
    Friend WithEvents LblGainA As Label
    Friend WithEvents Label7 As Label
    Friend WithEvents TxtQOffsetC As TextBox
    Friend WithEvents LblQOffsetC As Label
    Friend WithEvents Label5 As Label
    Friend WithEvents TxtPOffsetC As TextBox
    Friend WithEvents LblPOffsetC As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents TxtQOffsetB As TextBox
    Friend WithEvents LblQOffsetB As Label
    Friend WithEvents TxtPOffsetB As TextBox
    Friend WithEvents LblPOffsetB As Label
    Friend WithEvents TxtQoffsetA As TextBox
    Friend WithEvents LblQoffsetA As Label
    Friend WithEvents TxtPoffsetA As TextBox
    Friend WithEvents LblPoffsetA As Label
    Friend WithEvents GrpCommonCalibrationReg As GroupBox
    Friend WithEvents PnlCommonCalibration As Panel
    Friend WithEvents StatusStrip1 As StatusStrip
    Friend WithEvents Label6 As Label
    Friend WithEvents TxtPhiC As TextBox
    Friend WithEvents LblPhiC As Label
    Friend WithEvents Label10 As Label
    Friend WithEvents TxtGainC As TextBox
    Friend WithEvents LblGainC As Label
    Friend WithEvents Label14 As Label
    Friend WithEvents TxtPhiB As TextBox
    Friend WithEvents LblPhiB As Label
    Friend WithEvents TimerHandler1_Client As Timer
End Class
