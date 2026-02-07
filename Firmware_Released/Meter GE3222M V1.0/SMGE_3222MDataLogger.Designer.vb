<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222MDataLogger
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
        Dim Background1 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background2 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background3 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background4 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background5 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background6 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background7 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background8 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(FrmSMGE_3222MDataLogger))
        Me.GrpControl = New System.Windows.Forms.GroupBox()
        Me.LblRecording = New System.Windows.Forms.Label()
        Me.CmdClearGridviewandGraph = New System.Windows.Forms.Button()
        Me.CmdStopRecording = New System.Windows.Forms.Button()
        Me.CmdStartRecording = New System.Windows.Forms.Button()
        Me.GrpExport = New System.Windows.Forms.GroupBox()
        Me.ProgressBarProcess = New System.Windows.Forms.ProgressBar()
        Me.CmdSavetoExcel = New System.Windows.Forms.Button()
        Me.GrpDataGridview = New System.Windows.Forms.GroupBox()
        Me.DataGridView1 = New System.Windows.Forms.DataGridView()
        Me.ID = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.V1_Line = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.V2_Line = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.V3_Line = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.I1_Line = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.I2_Line = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.I3_Line = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.I_N = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P1act = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P2act = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P3act = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P_t = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P_tcal = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Q1 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Q2 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Q3 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Qtot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.S1 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.S2 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.S3 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Stot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PF1 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PF2 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PF3 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PF = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ang1 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ang2 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Ang3 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Freq = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyImpPhaA = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyImpPhaB = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyImpPhaC = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyImpTot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyExpPhaA = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyExpPhaB = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyExpPhaC = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ActiveEnergyExpTot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyImpPhaA = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyImpPhaB = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyImpPhaC = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyImptot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyExpPhaA = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyExpPhaB = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyExpPhaC = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.RectiveEnergyExpTot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.AppntEnergyExpPhaA = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.AppntEnergyExpPhaB = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.AppntEnergyExpPhaC = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.AppntEnergyExpTotArth = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.AppntEnergyExpTotvec = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Humidity = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.TemperatureC = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.TemperatureF = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Time_ = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Date_ = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.GrpGraph = New System.Windows.Forms.GroupBox()
        Me.ChartAppntEngy = New System.Windows.Forms.CheckBox()
        Me.ChartReactEngyExp = New System.Windows.Forms.CheckBox()
        Me.ChartReactEngyImp = New System.Windows.Forms.CheckBox()
        Me.ChartActEngyExp = New System.Windows.Forms.CheckBox()
        Me.ChartActEngyImp = New System.Windows.Forms.CheckBox()
        Me.ChartFreq = New System.Windows.Forms.CheckBox()
        Me.ChartPFactor = New System.Windows.Forms.CheckBox()
        Me.ChartPhaAngle = New System.Windows.Forms.CheckBox()
        Me.ChartApptPower = New System.Windows.Forms.CheckBox()
        Me.ChartReactPower = New System.Windows.Forms.CheckBox()
        Me.ChartActPower = New System.Windows.Forms.CheckBox()
        Me.ChartCurrent = New System.Windows.Forms.CheckBox()
        Me.ChartMeterCon = New System.Windows.Forms.CheckedListBox()
        Me.ChartVoltage = New System.Windows.Forms.CheckedListBox()
        Me.ChartControl1 = New DevComponents.DotNetBar.Charts.ChartControl()
        Me.GrpMeterAccess = New System.Windows.Forms.GroupBox()
        Me.CmdLoadGraph = New System.Windows.Forms.Button()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdReadData = New System.Windows.Forms.Button()
        Me.PictureBoxRecordInd = New System.Windows.Forms.PictureBox()
        Me.GrpControl.SuspendLayout()
        Me.GrpExport.SuspendLayout()
        Me.GrpDataGridview.SuspendLayout()
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.GrpGraph.SuspendLayout()
        Me.GrpMeterAccess.SuspendLayout()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'GrpControl
        '
        Me.GrpControl.Controls.Add(Me.PictureBoxRecordInd)
        Me.GrpControl.Controls.Add(Me.LblRecording)
        Me.GrpControl.Controls.Add(Me.CmdClearGridviewandGraph)
        Me.GrpControl.Controls.Add(Me.CmdStopRecording)
        Me.GrpControl.Controls.Add(Me.CmdStartRecording)
        Me.GrpControl.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpControl.Location = New System.Drawing.Point(12, 16)
        Me.GrpControl.Name = "GrpControl"
        Me.GrpControl.Size = New System.Drawing.Size(325, 173)
        Me.GrpControl.TabIndex = 0
        Me.GrpControl.TabStop = False
        Me.GrpControl.Text = "Control"
        '
        'LblRecording
        '
        Me.LblRecording.AutoSize = True
        Me.LblRecording.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblRecording.Location = New System.Drawing.Point(166, 14)
        Me.LblRecording.Name = "LblRecording"
        Me.LblRecording.Size = New System.Drawing.Size(108, 18)
        Me.LblRecording.TabIndex = 3
        Me.LblRecording.Text = "Recording "
        '
        'CmdClearGridviewandGraph
        '
        Me.CmdClearGridviewandGraph.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdClearGridviewandGraph.Location = New System.Drawing.Point(6, 103)
        Me.CmdClearGridviewandGraph.Name = "CmdClearGridviewandGraph"
        Me.CmdClearGridviewandGraph.Size = New System.Drawing.Size(291, 53)
        Me.CmdClearGridviewandGraph.TabIndex = 2
        Me.CmdClearGridviewandGraph.Text = "Clear DataGridview and Graph"
        Me.CmdClearGridviewandGraph.UseVisualStyleBackColor = True
        '
        'CmdStopRecording
        '
        Me.CmdStopRecording.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdStopRecording.Location = New System.Drawing.Point(162, 40)
        Me.CmdStopRecording.Name = "CmdStopRecording"
        Me.CmdStopRecording.Size = New System.Drawing.Size(135, 48)
        Me.CmdStopRecording.TabIndex = 1
        Me.CmdStopRecording.Text = "Stop Recording"
        Me.CmdStopRecording.UseVisualStyleBackColor = True
        '
        'CmdStartRecording
        '
        Me.CmdStartRecording.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdStartRecording.Location = New System.Drawing.Point(6, 40)
        Me.CmdStartRecording.Name = "CmdStartRecording"
        Me.CmdStartRecording.Size = New System.Drawing.Size(135, 48)
        Me.CmdStartRecording.TabIndex = 0
        Me.CmdStartRecording.Text = "Start Recording"
        Me.CmdStartRecording.UseVisualStyleBackColor = True
        '
        'GrpExport
        '
        Me.GrpExport.Controls.Add(Me.ProgressBarProcess)
        Me.GrpExport.Controls.Add(Me.CmdSavetoExcel)
        Me.GrpExport.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpExport.Location = New System.Drawing.Point(350, 16)
        Me.GrpExport.Name = "GrpExport"
        Me.GrpExport.Size = New System.Drawing.Size(175, 173)
        Me.GrpExport.TabIndex = 1
        Me.GrpExport.TabStop = False
        Me.GrpExport.Text = "Export"
        '
        'ProgressBarProcess
        '
        Me.ProgressBarProcess.Location = New System.Drawing.Point(6, 124)
        Me.ProgressBarProcess.Maximum = 10
        Me.ProgressBarProcess.Name = "ProgressBarProcess"
        Me.ProgressBarProcess.Size = New System.Drawing.Size(163, 17)
        Me.ProgressBarProcess.TabIndex = 2
        Me.ProgressBarProcess.Visible = False
        '
        'CmdSavetoExcel
        '
        Me.CmdSavetoExcel.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdSavetoExcel.Location = New System.Drawing.Point(17, 40)
        Me.CmdSavetoExcel.Name = "CmdSavetoExcel"
        Me.CmdSavetoExcel.Size = New System.Drawing.Size(135, 61)
        Me.CmdSavetoExcel.TabIndex = 1
        Me.CmdSavetoExcel.Text = "Save to MS Excel"
        Me.CmdSavetoExcel.UseVisualStyleBackColor = True
        '
        'GrpDataGridview
        '
        Me.GrpDataGridview.Controls.Add(Me.DataGridView1)
        Me.GrpDataGridview.Font = New System.Drawing.Font("Courier New", 11.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpDataGridview.Location = New System.Drawing.Point(12, 209)
        Me.GrpDataGridview.Name = "GrpDataGridview"
        Me.GrpDataGridview.Size = New System.Drawing.Size(513, 408)
        Me.GrpDataGridview.TabIndex = 3
        Me.GrpDataGridview.TabStop = False
        Me.GrpDataGridview.Text = "Data Grid View (Real-Time/Sec)"
        '
        'DataGridView1
        '
        Me.DataGridView1.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
        Me.DataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.DisplayedCells
        Me.DataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridView1.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.ID, Me.V1_Line, Me.V2_Line, Me.V3_Line, Me.I1_Line, Me.I2_Line, Me.I3_Line, Me.I_N, Me.P1act, Me.P2act, Me.P3act, Me.P_t, Me.P_tcal, Me.Q1, Me.Q2, Me.Q3, Me.Qtot, Me.S1, Me.S2, Me.S3, Me.Stot, Me.PF1, Me.PF2, Me.PF3, Me.PF, Me.ang1, Me.ang2, Me.Ang3, Me.Freq, Me.ActiveEnergyImpPhaA, Me.ActiveEnergyImpPhaB, Me.ActiveEnergyImpPhaC, Me.ActiveEnergyImpTot, Me.ActiveEnergyExpPhaA, Me.ActiveEnergyExpPhaB, Me.ActiveEnergyExpPhaC, Me.ActiveEnergyExpTot, Me.RectiveEnergyImpPhaA, Me.RectiveEnergyImpPhaB, Me.RectiveEnergyImpPhaC, Me.RectiveEnergyImptot, Me.RectiveEnergyExpPhaA, Me.RectiveEnergyExpPhaB, Me.RectiveEnergyExpPhaC, Me.RectiveEnergyExpTot, Me.AppntEnergyExpPhaA, Me.AppntEnergyExpPhaB, Me.AppntEnergyExpPhaC, Me.AppntEnergyExpTotArth, Me.AppntEnergyExpTotvec, Me.Humidity, Me.TemperatureC, Me.TemperatureF, Me.Time_, Me.Date_})
        Me.DataGridView1.Location = New System.Drawing.Point(6, 25)
        Me.DataGridView1.Name = "DataGridView1"
        Me.DataGridView1.Size = New System.Drawing.Size(501, 377)
        Me.DataGridView1.TabIndex = 4
        '
        'ID
        '
        Me.ID.HeaderText = "ID"
        Me.ID.Name = "ID"
        Me.ID.Width = 51
        '
        'V1_Line
        '
        Me.V1_Line.HeaderText = "V1_Line (V)"
        Me.V1_Line.Name = "V1_Line"
        Me.V1_Line.Width = 97
        '
        'V2_Line
        '
        Me.V2_Line.HeaderText = "V2_Line (V)"
        Me.V2_Line.Name = "V2_Line"
        Me.V2_Line.Width = 97
        '
        'V3_Line
        '
        Me.V3_Line.HeaderText = "V3_Line (V)"
        Me.V3_Line.Name = "V3_Line"
        Me.V3_Line.Width = 97
        '
        'I1_Line
        '
        Me.I1_Line.HeaderText = "I1_Line (A)"
        Me.I1_Line.Name = "I1_Line"
        Me.I1_Line.Width = 97
        '
        'I2_Line
        '
        Me.I2_Line.HeaderText = "I2_Line (A)"
        Me.I2_Line.Name = "I2_Line"
        Me.I2_Line.Width = 97
        '
        'I3_Line
        '
        Me.I3_Line.HeaderText = "I3_Line (A)"
        Me.I3_Line.Name = "I3_Line"
        Me.I3_Line.Width = 97
        '
        'I_N
        '
        Me.I_N.HeaderText = "I_N (A)"
        Me.I_N.Name = "I_N"
        Me.I_N.Width = 64
        '
        'P1act
        '
        Me.P1act.HeaderText = "P1act (W)"
        Me.P1act.Name = "P1act"
        Me.P1act.Width = 80
        '
        'P2act
        '
        Me.P2act.HeaderText = "P2act (W)"
        Me.P2act.Name = "P2act"
        Me.P2act.Width = 80
        '
        'P3act
        '
        Me.P3act.HeaderText = "P3act (W)"
        Me.P3act.Name = "P3act"
        Me.P3act.Width = 80
        '
        'P_t
        '
        Me.P_t.HeaderText = "P_tot (W)"
        Me.P_t.Name = "P_t"
        Me.P_t.Width = 80
        '
        'P_tcal
        '
        Me.P_tcal.HeaderText = "P_tcal (W)"
        Me.P_tcal.Name = "P_tcal"
        Me.P_tcal.Width = 88
        '
        'Q1
        '
        Me.Q1.HeaderText = "Q1 (VAR)"
        Me.Q1.Name = "Q1"
        Me.Q1.Width = 97
        '
        'Q2
        '
        Me.Q2.HeaderText = "Q2 (VAR)"
        Me.Q2.Name = "Q2"
        Me.Q2.Width = 97
        '
        'Q3
        '
        Me.Q3.HeaderText = "Q3 (VAR)"
        Me.Q3.Name = "Q3"
        Me.Q3.Width = 97
        '
        'Qtot
        '
        Me.Qtot.HeaderText = "Qtot (VAR)"
        Me.Qtot.Name = "Qtot"
        Me.Qtot.Width = 113
        '
        'S1
        '
        Me.S1.HeaderText = "S1 (VA)"
        Me.S1.Name = "S1"
        Me.S1.Width = 88
        '
        'S2
        '
        Me.S2.HeaderText = "S2 (VA)"
        Me.S2.Name = "S2"
        Me.S2.Width = 88
        '
        'S3
        '
        Me.S3.HeaderText = "S3 (VA)"
        Me.S3.Name = "S3"
        Me.S3.Width = 88
        '
        'Stot
        '
        Me.Stot.HeaderText = "Stot (VA)"
        Me.Stot.Name = "Stot"
        Me.Stot.Width = 72
        '
        'PF1
        '
        Me.PF1.HeaderText = "PF1"
        Me.PF1.Name = "PF1"
        Me.PF1.Width = 60
        '
        'PF2
        '
        Me.PF2.HeaderText = "PF2"
        Me.PF2.Name = "PF2"
        Me.PF2.Width = 60
        '
        'PF3
        '
        Me.PF3.HeaderText = "PF3"
        Me.PF3.Name = "PF3"
        Me.PF3.Width = 60
        '
        'PF
        '
        Me.PF.HeaderText = "PF"
        Me.PF.Name = "PF"
        Me.PF.Width = 51
        '
        'ang1
        '
        Me.ang1.HeaderText = "Ang1 (Deg)"
        Me.ang1.Name = "ang1"
        Me.ang1.Width = 113
        '
        'ang2
        '
        Me.ang2.HeaderText = "Ang2 (Deg)"
        Me.ang2.Name = "ang2"
        Me.ang2.Width = 113
        '
        'Ang3
        '
        Me.Ang3.HeaderText = "Ang3 (Deg)"
        Me.Ang3.Name = "Ang3"
        Me.Ang3.Width = 113
        '
        'Freq
        '
        Me.Freq.HeaderText = "Freq (Hz)"
        Me.Freq.Name = "Freq"
        Me.Freq.Width = 72
        '
        'ActiveEnergyImpPhaA
        '
        Me.ActiveEnergyImpPhaA.HeaderText = "Energy Pha A Imp (kWh)"
        Me.ActiveEnergyImpPhaA.Name = "ActiveEnergyImpPhaA"
        Me.ActiveEnergyImpPhaA.Width = 137
        '
        'ActiveEnergyImpPhaB
        '
        Me.ActiveEnergyImpPhaB.HeaderText = "Energy Pha B Imp (kWh)"
        Me.ActiveEnergyImpPhaB.Name = "ActiveEnergyImpPhaB"
        Me.ActiveEnergyImpPhaB.Width = 137
        '
        'ActiveEnergyImpPhaC
        '
        Me.ActiveEnergyImpPhaC.HeaderText = "Energy Pha C Imp (kWh)"
        Me.ActiveEnergyImpPhaC.Name = "ActiveEnergyImpPhaC"
        Me.ActiveEnergyImpPhaC.Width = 137
        '
        'ActiveEnergyImpTot
        '
        Me.ActiveEnergyImpTot.HeaderText = "Tot Energy Imp (kWh)"
        Me.ActiveEnergyImpTot.Name = "ActiveEnergyImpTot"
        Me.ActiveEnergyImpTot.Width = 153
        '
        'ActiveEnergyExpPhaA
        '
        Me.ActiveEnergyExpPhaA.HeaderText = "Energy Pha A Exp (kWh)"
        Me.ActiveEnergyExpPhaA.Name = "ActiveEnergyExpPhaA"
        Me.ActiveEnergyExpPhaA.Width = 137
        '
        'ActiveEnergyExpPhaB
        '
        Me.ActiveEnergyExpPhaB.HeaderText = "Energy Pha B Exp (kWh)"
        Me.ActiveEnergyExpPhaB.Name = "ActiveEnergyExpPhaB"
        Me.ActiveEnergyExpPhaB.Width = 137
        '
        'ActiveEnergyExpPhaC
        '
        Me.ActiveEnergyExpPhaC.HeaderText = "Energy Pha C Exp (kWh)"
        Me.ActiveEnergyExpPhaC.Name = "ActiveEnergyExpPhaC"
        Me.ActiveEnergyExpPhaC.Width = 137
        '
        'ActiveEnergyExpTot
        '
        Me.ActiveEnergyExpTot.HeaderText = "Total Energy Exp (kWh)"
        Me.ActiveEnergyExpTot.Name = "ActiveEnergyExpTot"
        Me.ActiveEnergyExpTot.Width = 169
        '
        'RectiveEnergyImpPhaA
        '
        Me.RectiveEnergyImpPhaA.HeaderText = "React Energy Pha A Imp (kVarh)"
        Me.RectiveEnergyImpPhaA.Name = "RectiveEnergyImpPhaA"
        Me.RectiveEnergyImpPhaA.Width = 186
        '
        'RectiveEnergyImpPhaB
        '
        Me.RectiveEnergyImpPhaB.HeaderText = "React Energy Pha B Imp (kVarh)"
        Me.RectiveEnergyImpPhaB.Name = "RectiveEnergyImpPhaB"
        Me.RectiveEnergyImpPhaB.Width = 186
        '
        'RectiveEnergyImpPhaC
        '
        Me.RectiveEnergyImpPhaC.HeaderText = "React Energy Pha C Imp (kVarh)"
        Me.RectiveEnergyImpPhaC.Name = "RectiveEnergyImpPhaC"
        Me.RectiveEnergyImpPhaC.Width = 186
        '
        'RectiveEnergyImptot
        '
        Me.RectiveEnergyImptot.HeaderText = "Total React Energy Imp (kVarh)"
        Me.RectiveEnergyImptot.Name = "RectiveEnergyImptot"
        Me.RectiveEnergyImptot.Width = 218
        '
        'RectiveEnergyExpPhaA
        '
        Me.RectiveEnergyExpPhaA.HeaderText = "React Energy Pha A Exp (kVarh)"
        Me.RectiveEnergyExpPhaA.Name = "RectiveEnergyExpPhaA"
        Me.RectiveEnergyExpPhaA.Width = 186
        '
        'RectiveEnergyExpPhaB
        '
        Me.RectiveEnergyExpPhaB.HeaderText = "React Energy Pha B Exp (kVarh)"
        Me.RectiveEnergyExpPhaB.Name = "RectiveEnergyExpPhaB"
        Me.RectiveEnergyExpPhaB.Width = 186
        '
        'RectiveEnergyExpPhaC
        '
        Me.RectiveEnergyExpPhaC.HeaderText = "React Energy Pha C Exp (kVarh)"
        Me.RectiveEnergyExpPhaC.Name = "RectiveEnergyExpPhaC"
        Me.RectiveEnergyExpPhaC.Width = 186
        '
        'RectiveEnergyExpTot
        '
        Me.RectiveEnergyExpTot.HeaderText = "Total React Energy Exp (kVarh)"
        Me.RectiveEnergyExpTot.Name = "RectiveEnergyExpTot"
        Me.RectiveEnergyExpTot.Width = 218
        '
        'AppntEnergyExpPhaA
        '
        Me.AppntEnergyExpPhaA.HeaderText = "Appernt Energy Pha A (kVAh)"
        Me.AppntEnergyExpPhaA.Name = "AppntEnergyExpPhaA"
        Me.AppntEnergyExpPhaA.Width = 178
        '
        'AppntEnergyExpPhaB
        '
        Me.AppntEnergyExpPhaB.HeaderText = "Appernt Energy Pha B (kVAh)"
        Me.AppntEnergyExpPhaB.Name = "AppntEnergyExpPhaB"
        Me.AppntEnergyExpPhaB.Width = 178
        '
        'AppntEnergyExpPhaC
        '
        Me.AppntEnergyExpPhaC.HeaderText = "Appernt Energy Pha C (kVAh)"
        Me.AppntEnergyExpPhaC.Name = "AppntEnergyExpPhaC"
        Me.AppntEnergyExpPhaC.Width = 178
        '
        'AppntEnergyExpTotArth
        '
        Me.AppntEnergyExpTotArth.HeaderText = "Total Appernt Energy - Arith. Sum (kVAh)"
        Me.AppntEnergyExpTotArth.Name = "AppntEnergyExpTotArth"
        Me.AppntEnergyExpTotArth.Width = 218
        '
        'AppntEnergyExpTotvec
        '
        Me.AppntEnergyExpTotvec.HeaderText = "Total Appernt Energy - Vec Sum (kVAh)"
        Me.AppntEnergyExpTotvec.Name = "AppntEnergyExpTotvec"
        Me.AppntEnergyExpTotvec.Width = 218
        '
        'Humidity
        '
        Me.Humidity.HeaderText = "Humidity (%)"
        Me.Humidity.Name = "Humidity"
        Me.Humidity.Width = 105
        '
        'TemperatureC
        '
        Me.TemperatureC.HeaderText = "Temperature (°C)"
        Me.TemperatureC.Name = "TemperatureC"
        Me.TemperatureC.Width = 161
        '
        'TemperatureF
        '
        Me.TemperatureF.HeaderText = "Temperature (°F)"
        Me.TemperatureF.Name = "TemperatureF"
        Me.TemperatureF.Width = 161
        '
        'Time_
        '
        Me.Time_.HeaderText = "Time"
        Me.Time_.Name = "Time_"
        Me.Time_.Width = 69
        '
        'Date_
        '
        Me.Date_.HeaderText = "Date"
        Me.Date_.Name = "Date_"
        Me.Date_.Width = 69
        '
        'GrpGraph
        '
        Me.GrpGraph.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpGraph.Controls.Add(Me.ChartAppntEngy)
        Me.GrpGraph.Controls.Add(Me.ChartReactEngyExp)
        Me.GrpGraph.Controls.Add(Me.ChartReactEngyImp)
        Me.GrpGraph.Controls.Add(Me.ChartActEngyExp)
        Me.GrpGraph.Controls.Add(Me.ChartActEngyImp)
        Me.GrpGraph.Controls.Add(Me.ChartFreq)
        Me.GrpGraph.Controls.Add(Me.ChartPFactor)
        Me.GrpGraph.Controls.Add(Me.ChartPhaAngle)
        Me.GrpGraph.Controls.Add(Me.ChartApptPower)
        Me.GrpGraph.Controls.Add(Me.ChartReactPower)
        Me.GrpGraph.Controls.Add(Me.ChartActPower)
        Me.GrpGraph.Controls.Add(Me.ChartCurrent)
        Me.GrpGraph.Controls.Add(Me.ChartMeterCon)
        Me.GrpGraph.Controls.Add(Me.ChartVoltage)
        Me.GrpGraph.Controls.Add(Me.ChartControl1)
        Me.GrpGraph.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpGraph.Location = New System.Drawing.Point(531, 16)
        Me.GrpGraph.Name = "GrpGraph"
        Me.GrpGraph.Size = New System.Drawing.Size(295, 662)
        Me.GrpGraph.TabIndex = 4
        Me.GrpGraph.TabStop = False
        Me.GrpGraph.Text = "Graph Parameters"
        '
        'ChartAppntEngy
        '
        Me.ChartAppntEngy.AutoSize = True
        Me.ChartAppntEngy.Location = New System.Drawing.Point(16, 490)
        Me.ChartAppntEngy.Name = "ChartAppntEngy"
        Me.ChartAppntEngy.Size = New System.Drawing.Size(177, 22)
        Me.ChartAppntEngy.TabIndex = 28
        Me.ChartAppntEngy.Text = "Apperent Energy"
        Me.ChartAppntEngy.UseVisualStyleBackColor = True
        '
        'ChartReactEngyExp
        '
        Me.ChartReactEngyExp.AutoSize = True
        Me.ChartReactEngyExp.Location = New System.Drawing.Point(16, 455)
        Me.ChartReactEngyExp.Name = "ChartReactEngyExp"
        Me.ChartReactEngyExp.Size = New System.Drawing.Size(237, 22)
        Me.ChartReactEngyExp.TabIndex = 11
        Me.ChartReactEngyExp.Text = "Reactive Energy (Exp)"
        Me.ChartReactEngyExp.UseVisualStyleBackColor = True
        '
        'ChartReactEngyImp
        '
        Me.ChartReactEngyImp.AutoSize = True
        Me.ChartReactEngyImp.Location = New System.Drawing.Point(16, 420)
        Me.ChartReactEngyImp.Name = "ChartReactEngyImp"
        Me.ChartReactEngyImp.Size = New System.Drawing.Size(237, 22)
        Me.ChartReactEngyImp.TabIndex = 27
        Me.ChartReactEngyImp.Text = "Reactive Energy (Imp)"
        Me.ChartReactEngyImp.UseVisualStyleBackColor = True
        '
        'ChartActEngyExp
        '
        Me.ChartActEngyExp.AutoSize = True
        Me.ChartActEngyExp.Location = New System.Drawing.Point(16, 385)
        Me.ChartActEngyExp.Name = "ChartActEngyExp"
        Me.ChartActEngyExp.Size = New System.Drawing.Size(217, 22)
        Me.ChartActEngyExp.TabIndex = 26
        Me.ChartActEngyExp.Text = "Active Energy (Exp)"
        Me.ChartActEngyExp.UseVisualStyleBackColor = True
        '
        'ChartActEngyImp
        '
        Me.ChartActEngyImp.AutoSize = True
        Me.ChartActEngyImp.Location = New System.Drawing.Point(16, 350)
        Me.ChartActEngyImp.Name = "ChartActEngyImp"
        Me.ChartActEngyImp.Size = New System.Drawing.Size(217, 22)
        Me.ChartActEngyImp.TabIndex = 25
        Me.ChartActEngyImp.Text = "Active Energy (Imp)"
        Me.ChartActEngyImp.UseVisualStyleBackColor = True
        '
        'ChartFreq
        '
        Me.ChartFreq.AutoSize = True
        Me.ChartFreq.Location = New System.Drawing.Point(16, 315)
        Me.ChartFreq.Name = "ChartFreq"
        Me.ChartFreq.Size = New System.Drawing.Size(117, 22)
        Me.ChartFreq.TabIndex = 23
        Me.ChartFreq.Text = "Frequency"
        Me.ChartFreq.UseVisualStyleBackColor = True
        '
        'ChartPFactor
        '
        Me.ChartPFactor.AutoSize = True
        Me.ChartPFactor.Location = New System.Drawing.Point(16, 280)
        Me.ChartPFactor.Name = "ChartPFactor"
        Me.ChartPFactor.Size = New System.Drawing.Size(147, 22)
        Me.ChartPFactor.TabIndex = 11
        Me.ChartPFactor.Text = "Power Factor"
        Me.ChartPFactor.UseVisualStyleBackColor = True
        '
        'ChartPhaAngle
        '
        Me.ChartPhaAngle.AutoSize = True
        Me.ChartPhaAngle.Location = New System.Drawing.Point(16, 245)
        Me.ChartPhaAngle.Name = "ChartPhaAngle"
        Me.ChartPhaAngle.Size = New System.Drawing.Size(137, 22)
        Me.ChartPhaAngle.TabIndex = 22
        Me.ChartPhaAngle.Text = "Phase Angle"
        Me.ChartPhaAngle.UseVisualStyleBackColor = True
        '
        'ChartApptPower
        '
        Me.ChartApptPower.AutoSize = True
        Me.ChartApptPower.Location = New System.Drawing.Point(16, 210)
        Me.ChartApptPower.Name = "ChartApptPower"
        Me.ChartApptPower.Size = New System.Drawing.Size(167, 22)
        Me.ChartApptPower.TabIndex = 11
        Me.ChartApptPower.Text = "Apperent Power"
        Me.ChartApptPower.UseVisualStyleBackColor = True
        '
        'ChartReactPower
        '
        Me.ChartReactPower.AutoSize = True
        Me.ChartReactPower.BackColor = System.Drawing.SystemColors.Control
        Me.ChartReactPower.Location = New System.Drawing.Point(16, 175)
        Me.ChartReactPower.Name = "ChartReactPower"
        Me.ChartReactPower.Size = New System.Drawing.Size(167, 22)
        Me.ChartReactPower.TabIndex = 21
        Me.ChartReactPower.Text = "Reactive Power"
        Me.ChartReactPower.UseVisualStyleBackColor = False
        '
        'ChartActPower
        '
        Me.ChartActPower.AutoSize = True
        Me.ChartActPower.BackColor = System.Drawing.SystemColors.Control
        Me.ChartActPower.Location = New System.Drawing.Point(16, 140)
        Me.ChartActPower.Name = "ChartActPower"
        Me.ChartActPower.Size = New System.Drawing.Size(127, 22)
        Me.ChartActPower.TabIndex = 20
        Me.ChartActPower.Text = "Real Power"
        Me.ChartActPower.UseVisualStyleBackColor = False
        '
        'ChartCurrent
        '
        Me.ChartCurrent.AutoSize = True
        Me.ChartCurrent.BackColor = System.Drawing.SystemColors.Control
        Me.ChartCurrent.Location = New System.Drawing.Point(16, 105)
        Me.ChartCurrent.Name = "ChartCurrent"
        Me.ChartCurrent.Size = New System.Drawing.Size(97, 22)
        Me.ChartCurrent.TabIndex = 19
        Me.ChartCurrent.Text = "Current"
        Me.ChartCurrent.UseVisualStyleBackColor = False
        '
        'ChartMeterCon
        '
        Me.ChartMeterCon.BackColor = System.Drawing.SystemColors.Control
        Me.ChartMeterCon.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.ChartMeterCon.CheckOnClick = True
        Me.ChartMeterCon.FormattingEnabled = True
        Me.ChartMeterCon.Items.AddRange(New Object() {"Hum", "Temp C", "Temp F"})
        Me.ChartMeterCon.Location = New System.Drawing.Point(16, 525)
        Me.ChartMeterCon.Name = "ChartMeterCon"
        Me.ChartMeterCon.Size = New System.Drawing.Size(145, 65)
        Me.ChartMeterCon.TabIndex = 16
        '
        'ChartVoltage
        '
        Me.ChartVoltage.BackColor = System.Drawing.SystemColors.Control
        Me.ChartVoltage.CheckOnClick = True
        Me.ChartVoltage.FormattingEnabled = True
        Me.ChartVoltage.Items.AddRange(New Object() {"V1_Line", "V2_Line", "V3_Line"})
        Me.ChartVoltage.Location = New System.Drawing.Point(16, 25)
        Me.ChartVoltage.Name = "ChartVoltage"
        Me.ChartVoltage.Size = New System.Drawing.Size(145, 67)
        Me.ChartVoltage.TabIndex = 11
        '
        'ChartControl1
        '
        Me.ChartControl1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartControl1.ChartPanel.Legend.Visible = False
        Me.ChartControl1.ChartPanel.Name = "PrimaryPanel"
        Background1.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.MouseOver.ArrowBackground = Background1
        Background2.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.MouseOver.ThumbBackground = Background2
        Background3.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.SelectedMouseOver.ArrowBackground = Background3
        Background4.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.SelectedMouseOver.ThumbBackground = Background4
        Background5.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.MouseOver.ArrowBackground = Background5
        Background6.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.MouseOver.ThumbBackground = Background6
        Background7.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.SelectedMouseOver.ArrowBackground = Background7
        Background8.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.SelectedMouseOver.ThumbBackground = Background8
        Me.ChartControl1.Location = New System.Drawing.Point(6, 25)
        Me.ChartControl1.Name = "ChartControl1"
        Me.ChartControl1.Size = New System.Drawing.Size(283, 631)
        Me.ChartControl1.TabIndex = 0
        Me.ChartControl1.Text = "ChartControl1"
        '
        'GrpMeterAccess
        '
        Me.GrpMeterAccess.Controls.Add(Me.CmdLoadGraph)
        Me.GrpMeterAccess.Controls.Add(Me.CmdBack)
        Me.GrpMeterAccess.Controls.Add(Me.CmdReadData)
        Me.GrpMeterAccess.Location = New System.Drawing.Point(18, 623)
        Me.GrpMeterAccess.Name = "GrpMeterAccess"
        Me.GrpMeterAccess.Size = New System.Drawing.Size(470, 54)
        Me.GrpMeterAccess.TabIndex = 10
        Me.GrpMeterAccess.TabStop = False
        '
        'CmdLoadGraph
        '
        Me.CmdLoadGraph.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdLoadGraph.Location = New System.Drawing.Point(183, 17)
        Me.CmdLoadGraph.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdLoadGraph.Name = "CmdLoadGraph"
        Me.CmdLoadGraph.Size = New System.Drawing.Size(104, 30)
        Me.CmdLoadGraph.TabIndex = 11
        Me.CmdLoadGraph.Text = "Load Graph"
        Me.CmdLoadGraph.UseVisualStyleBackColor = True
        '
        'CmdBack
        '
        Me.CmdBack.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdBack.Location = New System.Drawing.Point(330, 17)
        Me.CmdBack.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdBack.Name = "CmdBack"
        Me.CmdBack.Size = New System.Drawing.Size(104, 30)
        Me.CmdBack.TabIndex = 10
        Me.CmdBack.Text = "Back"
        Me.CmdBack.UseVisualStyleBackColor = True
        '
        'CmdReadData
        '
        Me.CmdReadData.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdReadData.Location = New System.Drawing.Point(36, 17)
        Me.CmdReadData.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdReadData.Name = "CmdReadData"
        Me.CmdReadData.Size = New System.Drawing.Size(104, 30)
        Me.CmdReadData.TabIndex = 7
        Me.CmdReadData.Text = "Read Data"
        Me.CmdReadData.UseVisualStyleBackColor = True
        '
        'PictureBoxRecordInd
        '
        Me.PictureBoxRecordInd.Image = CType(resources.GetObject("PictureBoxRecordInd.Image"), System.Drawing.Image)
        Me.PictureBoxRecordInd.Location = New System.Drawing.Point(277, 14)
        Me.PictureBoxRecordInd.Name = "PictureBoxRecordInd"
        Me.PictureBoxRecordInd.Size = New System.Drawing.Size(20, 20)
        Me.PictureBoxRecordInd.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBoxRecordInd.TabIndex = 4
        Me.PictureBoxRecordInd.TabStop = False
        '
        'FrmSMGE_3222MDataLogger
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoScroll = True
        Me.ClientSize = New System.Drawing.Size(834, 701)
        Me.Controls.Add(Me.GrpMeterAccess)
        Me.Controls.Add(Me.GrpGraph)
        Me.Controls.Add(Me.GrpDataGridview)
        Me.Controls.Add(Me.GrpExport)
        Me.Controls.Add(Me.GrpControl)
        Me.DoubleBuffered = True
        Me.MaximizeBox = False
        Me.Name = "FrmSMGE_3222MDataLogger"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE3222M Data Logger"
        Me.GrpControl.ResumeLayout(False)
        Me.GrpControl.PerformLayout()
        Me.GrpExport.ResumeLayout(False)
        Me.GrpDataGridview.ResumeLayout(False)
        CType(Me.DataGridView1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.GrpGraph.ResumeLayout(False)
        Me.GrpGraph.PerformLayout()
        Me.GrpMeterAccess.ResumeLayout(False)
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents GrpControl As GroupBox
    Friend WithEvents CmdStopRecording As Button
    Friend WithEvents CmdStartRecording As Button
    Friend WithEvents CmdClearGridviewandGraph As Button
    Friend WithEvents LblRecording As Label
    Friend WithEvents PictureBoxRecordInd As PictureBox
    Friend WithEvents GrpExport As GroupBox
    Friend WithEvents CmdSavetoExcel As Button
    Friend WithEvents ProgressBarProcess As ProgressBar
    Friend WithEvents GrpDataGridview As GroupBox
    Friend WithEvents DataGridView1 As DataGridView
    Friend WithEvents GrpGraph As GroupBox
    Friend WithEvents GrpMeterAccess As GroupBox
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdReadData As Button
    Friend WithEvents ID As DataGridViewTextBoxColumn
    Friend WithEvents V1_Line As DataGridViewTextBoxColumn
    Friend WithEvents V2_Line As DataGridViewTextBoxColumn
    Friend WithEvents V3_Line As DataGridViewTextBoxColumn
    Friend WithEvents I1_Line As DataGridViewTextBoxColumn
    Friend WithEvents I2_Line As DataGridViewTextBoxColumn
    Friend WithEvents I3_Line As DataGridViewTextBoxColumn
    Friend WithEvents I_N As DataGridViewTextBoxColumn
    Friend WithEvents P1act As DataGridViewTextBoxColumn
    Friend WithEvents P2act As DataGridViewTextBoxColumn
    Friend WithEvents P3act As DataGridViewTextBoxColumn
    Friend WithEvents P_t As DataGridViewTextBoxColumn
    Friend WithEvents P_tcal As DataGridViewTextBoxColumn
    Friend WithEvents Q1 As DataGridViewTextBoxColumn
    Friend WithEvents Q2 As DataGridViewTextBoxColumn
    Friend WithEvents Q3 As DataGridViewTextBoxColumn
    Friend WithEvents Qtot As DataGridViewTextBoxColumn
    Friend WithEvents S1 As DataGridViewTextBoxColumn
    Friend WithEvents S2 As DataGridViewTextBoxColumn
    Friend WithEvents S3 As DataGridViewTextBoxColumn
    Friend WithEvents Stot As DataGridViewTextBoxColumn
    Friend WithEvents PF1 As DataGridViewTextBoxColumn
    Friend WithEvents PF2 As DataGridViewTextBoxColumn
    Friend WithEvents PF3 As DataGridViewTextBoxColumn
    Friend WithEvents PF As DataGridViewTextBoxColumn
    Friend WithEvents ang1 As DataGridViewTextBoxColumn
    Friend WithEvents ang2 As DataGridViewTextBoxColumn
    Friend WithEvents Ang3 As DataGridViewTextBoxColumn
    Friend WithEvents Freq As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyImpPhaA As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyImpPhaB As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyImpPhaC As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyImpTot As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyExpPhaA As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyExpPhaB As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyExpPhaC As DataGridViewTextBoxColumn
    Friend WithEvents ActiveEnergyExpTot As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyImpPhaA As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyImpPhaB As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyImpPhaC As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyImptot As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyExpPhaA As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyExpPhaB As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyExpPhaC As DataGridViewTextBoxColumn
    Friend WithEvents RectiveEnergyExpTot As DataGridViewTextBoxColumn
    Friend WithEvents AppntEnergyExpPhaA As DataGridViewTextBoxColumn
    Friend WithEvents AppntEnergyExpPhaB As DataGridViewTextBoxColumn
    Friend WithEvents AppntEnergyExpPhaC As DataGridViewTextBoxColumn
    Friend WithEvents AppntEnergyExpTotArth As DataGridViewTextBoxColumn
    Friend WithEvents AppntEnergyExpTotvec As DataGridViewTextBoxColumn
    Friend WithEvents Humidity As DataGridViewTextBoxColumn
    Friend WithEvents TemperatureC As DataGridViewTextBoxColumn
    Friend WithEvents TemperatureF As DataGridViewTextBoxColumn
    Friend WithEvents Time_ As DataGridViewTextBoxColumn
    Friend WithEvents Date_ As DataGridViewTextBoxColumn
    Friend WithEvents ChartControl1 As DevComponents.DotNetBar.Charts.ChartControl
    Friend WithEvents ChartVoltage As CheckedListBox
    Friend WithEvents ChartMeterCon As CheckedListBox
    Friend WithEvents CmdLoadGraph As Button
    Friend WithEvents ChartCurrent As CheckBox
    Friend WithEvents ChartActPower As CheckBox
    Friend WithEvents ChartReactPower As CheckBox
    Friend WithEvents ChartApptPower As CheckBox
    Friend WithEvents ChartActEngyExp As CheckBox
    Friend WithEvents ChartActEngyImp As CheckBox
    Friend WithEvents ChartFreq As CheckBox
    Friend WithEvents ChartPFactor As CheckBox
    Friend WithEvents ChartPhaAngle As CheckBox
    Friend WithEvents ChartAppntEngy As CheckBox
    Friend WithEvents ChartReactEngyExp As CheckBox
    Friend WithEvents ChartReactEngyImp As CheckBox
End Class
