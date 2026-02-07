<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_PhaseAngleData
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
        Dim ChartArea1 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend1 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim Series1 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series2 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series3 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim ChartArea2 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend2 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim Series4 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series5 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series6 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Background1 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background2 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background3 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background4 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background5 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background6 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background7 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background8 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Me.PictureBoxRecordInd = New System.Windows.Forms.PictureBox()
        Me.CmdClearRecording = New System.Windows.Forms.Button()
        Me.CmdExportToExcel = New System.Windows.Forms.Button()
        Me.DataGridViewPhaseAngle = New System.Windows.Forms.DataGridView()
        Me.Timer = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Ang1 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Ang2 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Ang3 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Panel5 = New System.Windows.Forms.Panel()
        Me.Panel6 = New System.Windows.Forms.Panel()
        Me.LblTime = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.LblRecording = New System.Windows.Forms.Label()
        Me.ChartPhaseAngleRec = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel8 = New System.Windows.Forms.Panel()
        Me.Panel7 = New System.Windows.Forms.Panel()
        Me.ButtonRecord = New System.Windows.Forms.Button()
        Me.CmdReadData = New System.Windows.Forms.Button()
        Me.ProgressBarSave = New System.Windows.Forms.ProgressBar()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.TxtPhaseAngle = New System.Windows.Forms.TextBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.Panel4 = New System.Windows.Forms.Panel()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.ChartPhaseAngle = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel3 = New System.Windows.Forms.Panel()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ChartControl1 = New DevComponents.DotNetBar.Charts.ChartControl()
        Me.GrpChartPhaseAngle = New System.Windows.Forms.GroupBox()
        Me.Panel2 = New System.Windows.Forms.Panel()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.DataGridViewPhaseAngle, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel5.SuspendLayout()
        Me.Panel6.SuspendLayout()
        CType(Me.ChartPhaseAngleRec, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel8.SuspendLayout()
        Me.Panel7.SuspendLayout()
        Me.Panel1.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.Panel4.SuspendLayout()
        CType(Me.ChartPhaseAngle, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel3.SuspendLayout()
        Me.GrpChartPhaseAngle.SuspendLayout()
        Me.Panel2.SuspendLayout()
        Me.SuspendLayout()
        '
        'PictureBoxRecordInd
        '
        Me.PictureBoxRecordInd.Anchor = System.Windows.Forms.AnchorStyles.Right
        Me.PictureBoxRecordInd.Image = Global.Meter_GE3222M_V1._0.My.Resources.Resources.Red
        Me.PictureBoxRecordInd.Location = New System.Drawing.Point(547, 3)
        Me.PictureBoxRecordInd.Name = "PictureBoxRecordInd"
        Me.PictureBoxRecordInd.Size = New System.Drawing.Size(20, 20)
        Me.PictureBoxRecordInd.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBoxRecordInd.TabIndex = 6
        Me.PictureBoxRecordInd.TabStop = False
        '
        'CmdClearRecording
        '
        Me.CmdClearRecording.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdClearRecording.Font = New System.Drawing.Font("Courier New", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdClearRecording.Location = New System.Drawing.Point(180, 40)
        Me.CmdClearRecording.Name = "CmdClearRecording"
        Me.CmdClearRecording.Size = New System.Drawing.Size(148, 30)
        Me.CmdClearRecording.TabIndex = 5
        Me.CmdClearRecording.Text = "Clear Recording"
        Me.CmdClearRecording.UseVisualStyleBackColor = True
        '
        'CmdExportToExcel
        '
        Me.CmdExportToExcel.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdExportToExcel.Font = New System.Drawing.Font("Courier New", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdExportToExcel.Location = New System.Drawing.Point(3, 40)
        Me.CmdExportToExcel.Name = "CmdExportToExcel"
        Me.CmdExportToExcel.Size = New System.Drawing.Size(148, 30)
        Me.CmdExportToExcel.TabIndex = 4
        Me.CmdExportToExcel.Text = "Export To Excel"
        Me.CmdExportToExcel.UseVisualStyleBackColor = True
        '
        'DataGridViewPhaseAngle
        '
        Me.DataGridViewPhaseAngle.AllowUserToAddRows = False
        Me.DataGridViewPhaseAngle.AllowUserToDeleteRows = False
        Me.DataGridViewPhaseAngle.AllowUserToResizeRows = False
        Me.DataGridViewPhaseAngle.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.DataGridViewPhaseAngle.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.ColumnHeader
        Me.DataGridViewPhaseAngle.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridViewPhaseAngle.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.Timer, Me.Ang1, Me.Ang2, Me.Ang3})
        Me.DataGridViewPhaseAngle.Location = New System.Drawing.Point(3, 76)
        Me.DataGridViewPhaseAngle.Name = "DataGridViewPhaseAngle"
        Me.DataGridViewPhaseAngle.RowHeadersVisible = False
        Me.DataGridViewPhaseAngle.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect
        Me.DataGridViewPhaseAngle.Size = New System.Drawing.Size(328, 354)
        Me.DataGridViewPhaseAngle.TabIndex = 3
        '
        'Timer
        '
        Me.Timer.HeaderText = "Timer"
        Me.Timer.Name = "Timer"
        Me.Timer.Width = 83
        '
        'Ang1
        '
        Me.Ang1.HeaderText = "Phase Angle (Pha 1)"
        Me.Ang1.Name = "Ang1"
        Me.Ang1.Width = 185
        '
        'Ang2
        '
        Me.Ang2.HeaderText = "Phase Angle (Pha 2)"
        Me.Ang2.Name = "Ang2"
        Me.Ang2.Width = 185
        '
        'Ang3
        '
        Me.Ang3.HeaderText = "Phase Angle (Pha 3)"
        Me.Ang3.Name = "Ang3"
        Me.Ang3.Width = 185
        '
        'Panel5
        '
        Me.Panel5.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel5.BackColor = System.Drawing.Color.White
        Me.Panel5.Controls.Add(Me.CmdClearRecording)
        Me.Panel5.Controls.Add(Me.CmdExportToExcel)
        Me.Panel5.Controls.Add(Me.DataGridViewPhaseAngle)
        Me.Panel5.Controls.Add(Me.Panel6)
        Me.Panel5.Location = New System.Drawing.Point(600, 443)
        Me.Panel5.Name = "Panel5"
        Me.Panel5.Size = New System.Drawing.Size(331, 433)
        Me.Panel5.TabIndex = 1
        '
        'Panel6
        '
        Me.Panel6.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel6.Controls.Add(Me.LblTime)
        Me.Panel6.Controls.Add(Me.Label3)
        Me.Panel6.Dock = System.Windows.Forms.DockStyle.Top
        Me.Panel6.Location = New System.Drawing.Point(0, 0)
        Me.Panel6.Name = "Panel6"
        Me.Panel6.Size = New System.Drawing.Size(331, 28)
        Me.Panel6.TabIndex = 1
        '
        'LblTime
        '
        Me.LblTime.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.LblTime.AutoSize = True
        Me.LblTime.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblTime.ForeColor = System.Drawing.Color.White
        Me.LblTime.Location = New System.Drawing.Point(200, 5)
        Me.LblTime.Margin = New System.Windows.Forms.Padding(4, 0, 4, 0)
        Me.LblTime.Name = "LblTime"
        Me.LblTime.Size = New System.Drawing.Size(118, 18)
        Me.LblTime.TabIndex = 7
        Me.LblTime.Text = "Timer 00:00"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.ForeColor = System.Drawing.Color.White
        Me.Label3.Location = New System.Drawing.Point(7, 5)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(128, 18)
        Me.Label3.TabIndex = 0
        Me.Label3.Text = "Record Table"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.ForeColor = System.Drawing.Color.White
        Me.Label4.Location = New System.Drawing.Point(7, 5)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(268, 18)
        Me.Label4.TabIndex = 0
        Me.Label4.Text = "Record Graph (Phase Angle)"
        '
        'LblRecording
        '
        Me.LblRecording.Anchor = System.Windows.Forms.AnchorStyles.Right
        Me.LblRecording.AutoSize = True
        Me.LblRecording.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblRecording.ForeColor = System.Drawing.Color.White
        Me.LblRecording.Location = New System.Drawing.Point(444, 5)
        Me.LblRecording.Name = "LblRecording"
        Me.LblRecording.Size = New System.Drawing.Size(108, 18)
        Me.LblRecording.TabIndex = 5
        Me.LblRecording.Text = "Recording "
        '
        'ChartPhaseAngleRec
        '
        Me.ChartPhaseAngleRec.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartPhaseAngleRec.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartPhaseAngleRec.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea1.AxisX.Interval = 1.0R
        ChartArea1.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea1.AxisX.IsLabelAutoFit = False
        ChartArea1.AxisX.LabelStyle.Angle = -45
        ChartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea1.AxisX.Minimum = 0R
        ChartArea1.AxisX.Title = "Time (Sec)"
        ChartArea1.AxisX.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea1.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea1.AxisY.Maximum = 180.0R
        ChartArea1.AxisY.Minimum = -180.0R
        ChartArea1.AxisY.Title = "Phase Ang °"
        ChartArea1.AxisY.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea1.Name = "ChartArea1"
        Me.ChartPhaseAngleRec.ChartAreas.Add(ChartArea1)
        Legend1.Alignment = System.Drawing.StringAlignment.Center
        Legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend1.Name = "Legend1"
        Me.ChartPhaseAngleRec.Legends.Add(Legend1)
        Me.ChartPhaseAngleRec.Location = New System.Drawing.Point(7, 37)
        Me.ChartPhaseAngleRec.Name = "ChartPhaseAngleRec"
        Series1.ChartArea = "ChartArea1"
        Series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series1.Color = System.Drawing.Color.Crimson
        Series1.IsValueShownAsLabel = True
        Series1.LabelForeColor = System.Drawing.Color.Crimson
        Series1.Legend = "Legend1"
        Series1.LegendText = "Phase Ang (Pha 1)"
        Series1.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Square
        Series1.Name = "Series1"
        Series2.ChartArea = "ChartArea1"
        Series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series2.Color = System.Drawing.Color.FromArgb(CType(CType(255, Byte), Integer), CType(CType(192, Byte), Integer), CType(CType(128, Byte), Integer))
        Series2.IsValueShownAsLabel = True
        Series2.LabelForeColor = System.Drawing.Color.Orange
        Series2.Legend = "Legend1"
        Series2.LegendText = "Phase Ang (Pha 2)"
        Series2.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Circle
        Series2.Name = "Series2"
        Series3.ChartArea = "ChartArea1"
        Series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series3.Color = System.Drawing.Color.RoyalBlue
        Series3.IsValueShownAsLabel = True
        Series3.LabelForeColor = System.Drawing.Color.RoyalBlue
        Series3.Legend = "Legend1"
        Series3.LegendText = "Phase Ang (Pha 3)"
        Series3.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Triangle
        Series3.Name = "Series3"
        Me.ChartPhaseAngleRec.Series.Add(Series1)
        Me.ChartPhaseAngleRec.Series.Add(Series2)
        Me.ChartPhaseAngleRec.Series.Add(Series3)
        Me.ChartPhaseAngleRec.Size = New System.Drawing.Size(575, 300)
        Me.ChartPhaseAngleRec.TabIndex = 2
        '
        'Panel8
        '
        Me.Panel8.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel8.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel8.Controls.Add(Me.PictureBoxRecordInd)
        Me.Panel8.Controls.Add(Me.Label4)
        Me.Panel8.Controls.Add(Me.LblRecording)
        Me.Panel8.Location = New System.Drawing.Point(0, 0)
        Me.Panel8.Name = "Panel8"
        Me.Panel8.Size = New System.Drawing.Size(588, 28)
        Me.Panel8.TabIndex = 1
        '
        'Panel7
        '
        Me.Panel7.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel7.BackColor = System.Drawing.Color.White
        Me.Panel7.Controls.Add(Me.ChartPhaseAngleRec)
        Me.Panel7.Controls.Add(Me.Panel8)
        Me.Panel7.Location = New System.Drawing.Point(6, 443)
        Me.Panel7.Name = "Panel7"
        Me.Panel7.Size = New System.Drawing.Size(588, 349)
        Me.Panel7.TabIndex = 6
        '
        'ButtonRecord
        '
        Me.ButtonRecord.Anchor = System.Windows.Forms.AnchorStyles.Bottom
        Me.ButtonRecord.Font = New System.Drawing.Font("Courier New", 11.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ButtonRecord.Location = New System.Drawing.Point(148, 25)
        Me.ButtonRecord.Margin = New System.Windows.Forms.Padding(4)
        Me.ButtonRecord.Name = "ButtonRecord"
        Me.ButtonRecord.Size = New System.Drawing.Size(170, 30)
        Me.ButtonRecord.TabIndex = 10
        Me.ButtonRecord.Text = "Start Recording"
        Me.ButtonRecord.UseVisualStyleBackColor = True
        '
        'CmdReadData
        '
        Me.CmdReadData.Anchor = System.Windows.Forms.AnchorStyles.Bottom
        Me.CmdReadData.Font = New System.Drawing.Font("Courier New", 11.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdReadData.Location = New System.Drawing.Point(18, 25)
        Me.CmdReadData.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdReadData.Name = "CmdReadData"
        Me.CmdReadData.Size = New System.Drawing.Size(108, 30)
        Me.CmdReadData.TabIndex = 9
        Me.CmdReadData.Text = "Read Data"
        Me.CmdReadData.UseVisualStyleBackColor = True
        '
        'ProgressBarSave
        '
        Me.ProgressBarSave.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ProgressBarSave.Location = New System.Drawing.Point(11, 908)
        Me.ProgressBarSave.Maximum = 10
        Me.ProgressBarSave.Name = "ProgressBarSave"
        Me.ProgressBarSave.Size = New System.Drawing.Size(942, 26)
        Me.ProgressBarSave.TabIndex = 16
        Me.ProgressBarSave.Visible = False
        '
        'Panel1
        '
        Me.Panel1.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel1.BackColor = System.Drawing.Color.White
        Me.Panel1.Controls.Add(Me.TxtPhaseAngle)
        Me.Panel1.Location = New System.Drawing.Point(600, 25)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(331, 412)
        Me.Panel1.TabIndex = 2
        '
        'TxtPhaseAngle
        '
        Me.TxtPhaseAngle.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TxtPhaseAngle.BackColor = System.Drawing.SystemColors.Control
        Me.TxtPhaseAngle.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TxtPhaseAngle.Location = New System.Drawing.Point(3, 32)
        Me.TxtPhaseAngle.Multiline = True
        Me.TxtPhaseAngle.Name = "TxtPhaseAngle"
        Me.TxtPhaseAngle.ReadOnly = True
        Me.TxtPhaseAngle.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.TxtPhaseAngle.Size = New System.Drawing.Size(325, 377)
        Me.TxtPhaseAngle.TabIndex = 9
        '
        'GroupBox1
        '
        Me.GroupBox1.Anchor = System.Windows.Forms.AnchorStyles.Bottom
        Me.GroupBox1.Controls.Add(Me.ButtonRecord)
        Me.GroupBox1.Controls.Add(Me.CmdReadData)
        Me.GroupBox1.Controls.Add(Me.CmdBack)
        Me.GroupBox1.Controls.Add(Me.CmdCancel)
        Me.GroupBox1.Location = New System.Drawing.Point(-13, 800)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(569, 70)
        Me.GroupBox1.TabIndex = 9
        Me.GroupBox1.TabStop = False
        '
        'CmdBack
        '
        Me.CmdBack.Anchor = System.Windows.Forms.AnchorStyles.Bottom
        Me.CmdBack.Font = New System.Drawing.Font("Courier New", 11.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdBack.Location = New System.Drawing.Point(340, 25)
        Me.CmdBack.Name = "CmdBack"
        Me.CmdBack.Size = New System.Drawing.Size(93, 30)
        Me.CmdBack.TabIndex = 3
        Me.CmdBack.Text = "Back"
        Me.CmdBack.UseVisualStyleBackColor = True
        '
        'CmdCancel
        '
        Me.CmdCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom
        Me.CmdCancel.Font = New System.Drawing.Font("Courier New", 11.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdCancel.Location = New System.Drawing.Point(455, 25)
        Me.CmdCancel.Name = "CmdCancel"
        Me.CmdCancel.Size = New System.Drawing.Size(93, 30)
        Me.CmdCancel.TabIndex = 4
        Me.CmdCancel.Text = "Cancel"
        Me.CmdCancel.UseVisualStyleBackColor = True
        '
        'Panel4
        '
        Me.Panel4.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel4.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel4.Controls.Add(Me.Label2)
        Me.Panel4.Location = New System.Drawing.Point(600, 25)
        Me.Panel4.Name = "Panel4"
        Me.Panel4.Size = New System.Drawing.Size(331, 28)
        Me.Panel4.TabIndex = 6
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.ForeColor = System.Drawing.Color.White
        Me.Label2.Location = New System.Drawing.Point(17, 5)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(238, 18)
        Me.Label2.TabIndex = 0
        Me.Label2.Text = "Phase Angle (Real-Time)"
        '
        'ChartPhaseAngle
        '
        Me.ChartPhaseAngle.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartPhaseAngle.AntiAliasing = System.Windows.Forms.DataVisualization.Charting.AntiAliasingStyles.Text
        Me.ChartPhaseAngle.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartPhaseAngle.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea2.AxisX.Interval = 2.0R
        ChartArea2.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisX.LabelStyle.Enabled = False
        ChartArea2.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea2.AxisX.Minimum = 0R
        ChartArea2.AxisX.Title = "Time (Sec)"
        ChartArea2.AxisX.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea2.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea2.AxisY.Maximum = 180.0R
        ChartArea2.AxisY.Minimum = -180.0R
        ChartArea2.AxisY.Title = "Phase Angle °"
        ChartArea2.AxisY.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea2.AxisY2.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.[False]
        ChartArea2.AxisY2.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisY2.Maximum = 50.0R
        ChartArea2.AxisY2.Minimum = 0R
        ChartArea2.AxisY2.Title = "Temperature"
        ChartArea2.Name = "ChartArea1"
        Me.ChartPhaseAngle.ChartAreas.Add(ChartArea2)
        Legend2.Alignment = System.Drawing.StringAlignment.Center
        Legend2.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend2.Name = "Legend1"
        Legend2.TextWrapThreshold = 40
        Me.ChartPhaseAngle.Legends.Add(Legend2)
        Me.ChartPhaseAngle.Location = New System.Drawing.Point(10, 32)
        Me.ChartPhaseAngle.Name = "ChartPhaseAngle"
        Series4.BorderWidth = 2
        Series4.ChartArea = "ChartArea1"
        Series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series4.Color = System.Drawing.Color.Crimson
        Series4.Legend = "Legend1"
        Series4.LegendText = "Phase Angle (Pha 1)"
        Series4.Name = "Ang1"
        Series5.BorderWidth = 2
        Series5.ChartArea = "ChartArea1"
        Series5.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series5.Color = System.Drawing.Color.Gold
        Series5.Legend = "Legend1"
        Series5.LegendText = "Phase Angle (Pha 2)"
        Series5.Name = "Ang2"
        Series6.BorderWidth = 2
        Series6.ChartArea = "ChartArea1"
        Series6.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series6.Color = System.Drawing.Color.DeepSkyBlue
        Series6.Legend = "Legend1"
        Series6.LegendText = "Phase Angle (Pha 3)"
        Series6.Name = "Ang3"
        Me.ChartPhaseAngle.Series.Add(Series4)
        Me.ChartPhaseAngle.Series.Add(Series5)
        Me.ChartPhaseAngle.Series.Add(Series6)
        Me.ChartPhaseAngle.Size = New System.Drawing.Size(568, 372)
        Me.ChartPhaseAngle.TabIndex = 2
        '
        'Panel3
        '
        Me.Panel3.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel3.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel3.Controls.Add(Me.Label1)
        Me.Panel3.Location = New System.Drawing.Point(0, 0)
        Me.Panel3.Name = "Panel3"
        Me.Panel3.Size = New System.Drawing.Size(588, 28)
        Me.Panel3.TabIndex = 0
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.ForeColor = System.Drawing.Color.White
        Me.Label1.Location = New System.Drawing.Point(7, 5)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(458, 18)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Graph Phase Angle (Phase Angle Real-Time/Sec)"
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
        Me.ChartControl1.Location = New System.Drawing.Point(3, 22)
        Me.ChartControl1.Name = "ChartControl1"
        Me.ChartControl1.Size = New System.Drawing.Size(933, 851)
        Me.ChartControl1.TabIndex = 0
        Me.ChartControl1.Text = "ChartControl1"
        '
        'GrpChartPhaseAngle
        '
        Me.GrpChartPhaseAngle.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpChartPhaseAngle.Controls.Add(Me.GroupBox1)
        Me.GrpChartPhaseAngle.Controls.Add(Me.Panel4)
        Me.GrpChartPhaseAngle.Controls.Add(Me.Panel7)
        Me.GrpChartPhaseAngle.Controls.Add(Me.Panel5)
        Me.GrpChartPhaseAngle.Controls.Add(Me.Panel1)
        Me.GrpChartPhaseAngle.Controls.Add(Me.Panel2)
        Me.GrpChartPhaseAngle.Controls.Add(Me.ChartControl1)
        Me.GrpChartPhaseAngle.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpChartPhaseAngle.Location = New System.Drawing.Point(11, 16)
        Me.GrpChartPhaseAngle.Name = "GrpChartPhaseAngle"
        Me.GrpChartPhaseAngle.Size = New System.Drawing.Size(942, 879)
        Me.GrpChartPhaseAngle.TabIndex = 15
        Me.GrpChartPhaseAngle.TabStop = False
        Me.GrpChartPhaseAngle.Text = "Phase Angle (Real-Time/Sec)"
        '
        'Panel2
        '
        Me.Panel2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel2.BackColor = System.Drawing.Color.White
        Me.Panel2.Controls.Add(Me.ChartPhaseAngle)
        Me.Panel2.Controls.Add(Me.Panel3)
        Me.Panel2.Location = New System.Drawing.Point(6, 25)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(588, 412)
        Me.Panel2.TabIndex = 3
        '
        'FrmSMGE_3222M_PhaseAngleData
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoScroll = True
        Me.ClientSize = New System.Drawing.Size(964, 951)
        Me.Controls.Add(Me.ProgressBarSave)
        Me.Controls.Add(Me.GrpChartPhaseAngle)
        Me.DoubleBuffered = True
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow
        Me.Name = "FrmSMGE_3222M_PhaseAngleData"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE 3222M Phase Angle Data"
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.DataGridViewPhaseAngle, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel5.ResumeLayout(False)
        Me.Panel6.ResumeLayout(False)
        Me.Panel6.PerformLayout()
        CType(Me.ChartPhaseAngleRec, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel8.ResumeLayout(False)
        Me.Panel8.PerformLayout()
        Me.Panel7.ResumeLayout(False)
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        Me.GroupBox1.ResumeLayout(False)
        Me.Panel4.ResumeLayout(False)
        Me.Panel4.PerformLayout()
        CType(Me.ChartPhaseAngle, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel3.ResumeLayout(False)
        Me.Panel3.PerformLayout()
        Me.GrpChartPhaseAngle.ResumeLayout(False)
        Me.Panel2.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents PictureBoxRecordInd As PictureBox
    Friend WithEvents CmdClearRecording As Button
    Friend WithEvents CmdExportToExcel As Button
    Friend WithEvents DataGridViewPhaseAngle As DataGridView
    Friend WithEvents Panel5 As Panel
    Friend WithEvents Panel6 As Panel
    Friend WithEvents LblTime As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Label4 As Label
    Friend WithEvents LblRecording As Label
    Friend WithEvents ChartPhaseAngleRec As DataVisualization.Charting.Chart
    Friend WithEvents Panel8 As Panel
    Friend WithEvents Panel7 As Panel
    Friend WithEvents ButtonRecord As Button
    Friend WithEvents CmdReadData As Button
    Friend WithEvents ProgressBarSave As ProgressBar
    Friend WithEvents Panel1 As Panel
    Friend WithEvents TxtPhaseAngle As TextBox
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents Panel4 As Panel
    Friend WithEvents Label2 As Label
    Friend WithEvents ChartPhaseAngle As DataVisualization.Charting.Chart
    Friend WithEvents Panel3 As Panel
    Friend WithEvents Label1 As Label
    Friend WithEvents ChartControl1 As DevComponents.DotNetBar.Charts.ChartControl
    Friend WithEvents GrpChartPhaseAngle As GroupBox
    Friend WithEvents Panel2 As Panel
    Friend WithEvents Timer As DataGridViewTextBoxColumn
    Friend WithEvents Ang1 As DataGridViewTextBoxColumn
    Friend WithEvents Ang2 As DataGridViewTextBoxColumn
    Friend WithEvents Ang3 As DataGridViewTextBoxColumn
End Class
