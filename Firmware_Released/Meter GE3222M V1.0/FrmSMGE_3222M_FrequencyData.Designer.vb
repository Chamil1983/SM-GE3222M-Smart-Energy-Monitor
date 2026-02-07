<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_FrequencyData
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
        Dim ChartArea15 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Series15 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim ChartArea16 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Series16 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Background57 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background58 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background59 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background60 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background61 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background62 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background63 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background64 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Me.CmdClearRecording = New System.Windows.Forms.Button()
        Me.CmdExportToExcel = New System.Windows.Forms.Button()
        Me.DataGridViewFrequency = New System.Windows.Forms.DataGridView()
        Me.Panel5 = New System.Windows.Forms.Panel()
        Me.Panel6 = New System.Windows.Forms.Panel()
        Me.LblTime = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.PictureBoxRecordInd = New System.Windows.Forms.PictureBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.LblRecording = New System.Windows.Forms.Label()
        Me.ChartFrequencyRec = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel8 = New System.Windows.Forms.Panel()
        Me.Panel7 = New System.Windows.Forms.Panel()
        Me.ButtonRecord = New System.Windows.Forms.Button()
        Me.CmdReadData = New System.Windows.Forms.Button()
        Me.ProgressBarSave = New System.Windows.Forms.ProgressBar()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.TxtFrequency = New System.Windows.Forms.TextBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.Panel4 = New System.Windows.Forms.Panel()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.ChartFrequency = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel3 = New System.Windows.Forms.Panel()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ChartControl1 = New DevComponents.DotNetBar.Charts.ChartControl()
        Me.GrpChartFrequency = New System.Windows.Forms.GroupBox()
        Me.Panel2 = New System.Windows.Forms.Panel()
        Me.Timer = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Freq = New System.Windows.Forms.DataGridViewTextBoxColumn()
        CType(Me.DataGridViewFrequency, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel5.SuspendLayout()
        Me.Panel6.SuspendLayout()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.ChartFrequencyRec, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel8.SuspendLayout()
        Me.Panel7.SuspendLayout()
        Me.Panel1.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.Panel4.SuspendLayout()
        CType(Me.ChartFrequency, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel3.SuspendLayout()
        Me.GrpChartFrequency.SuspendLayout()
        Me.Panel2.SuspendLayout()
        Me.SuspendLayout()
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
        'DataGridViewFrequency
        '
        Me.DataGridViewFrequency.AllowUserToAddRows = False
        Me.DataGridViewFrequency.AllowUserToDeleteRows = False
        Me.DataGridViewFrequency.AllowUserToResizeRows = False
        Me.DataGridViewFrequency.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.DataGridViewFrequency.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill
        Me.DataGridViewFrequency.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.Disable
        Me.DataGridViewFrequency.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridViewFrequency.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.Timer, Me.Freq})
        Me.DataGridViewFrequency.Location = New System.Drawing.Point(3, 76)
        Me.DataGridViewFrequency.Name = "DataGridViewFrequency"
        Me.DataGridViewFrequency.RowHeadersVisible = False
        Me.DataGridViewFrequency.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect
        Me.DataGridViewFrequency.Size = New System.Drawing.Size(328, 354)
        Me.DataGridViewFrequency.TabIndex = 3
        '
        'Panel5
        '
        Me.Panel5.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel5.BackColor = System.Drawing.Color.White
        Me.Panel5.Controls.Add(Me.CmdClearRecording)
        Me.Panel5.Controls.Add(Me.CmdExportToExcel)
        Me.Panel5.Controls.Add(Me.DataGridViewFrequency)
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
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.ForeColor = System.Drawing.Color.White
        Me.Label4.Location = New System.Drawing.Point(7, 5)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(248, 18)
        Me.Label4.TabIndex = 0
        Me.Label4.Text = "Record Graph (Frequency)"
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
        'ChartFrequencyRec
        '
        Me.ChartFrequencyRec.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartFrequencyRec.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartFrequencyRec.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea15.AxisX.Interval = 1.0R
        ChartArea15.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea15.AxisX.IsLabelAutoFit = False
        ChartArea15.AxisX.LabelStyle.Angle = -45
        ChartArea15.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea15.AxisX.Minimum = 0R
        ChartArea15.AxisX.Title = "Time (Sec)"
        ChartArea15.AxisX.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea15.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea15.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea15.AxisY.Maximum = 65.0R
        ChartArea15.AxisY.Minimum = 0R
        ChartArea15.AxisY.Title = "Frequency (Hz)"
        ChartArea15.AxisY.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea15.Name = "ChartArea1"
        Me.ChartFrequencyRec.ChartAreas.Add(ChartArea15)
        Me.ChartFrequencyRec.Location = New System.Drawing.Point(7, 37)
        Me.ChartFrequencyRec.Name = "ChartFrequencyRec"
        Series15.ChartArea = "ChartArea1"
        Series15.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series15.Color = System.Drawing.Color.DarkTurquoise
        Series15.IsValueShownAsLabel = True
        Series15.LabelForeColor = System.Drawing.Color.DarkTurquoise
        Series15.LegendText = "Power Factor (Pha 1)"
        Series15.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Circle
        Series15.Name = "Series1"
        Me.ChartFrequencyRec.Series.Add(Series15)
        Me.ChartFrequencyRec.Size = New System.Drawing.Size(575, 300)
        Me.ChartFrequencyRec.TabIndex = 2
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
        Me.Panel7.Controls.Add(Me.ChartFrequencyRec)
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
        Me.Panel1.Controls.Add(Me.TxtFrequency)
        Me.Panel1.Location = New System.Drawing.Point(600, 25)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(331, 412)
        Me.Panel1.TabIndex = 2
        '
        'TxtFrequency
        '
        Me.TxtFrequency.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TxtFrequency.BackColor = System.Drawing.SystemColors.Control
        Me.TxtFrequency.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TxtFrequency.Location = New System.Drawing.Point(3, 32)
        Me.TxtFrequency.Multiline = True
        Me.TxtFrequency.Name = "TxtFrequency"
        Me.TxtFrequency.ReadOnly = True
        Me.TxtFrequency.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.TxtFrequency.Size = New System.Drawing.Size(325, 377)
        Me.TxtFrequency.TabIndex = 9
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
        Me.Label2.Size = New System.Drawing.Size(218, 18)
        Me.Label2.TabIndex = 0
        Me.Label2.Text = "Frequency (Real-Time)"
        '
        'ChartFrequency
        '
        Me.ChartFrequency.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartFrequency.AntiAliasing = System.Windows.Forms.DataVisualization.Charting.AntiAliasingStyles.Text
        Me.ChartFrequency.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartFrequency.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea16.AxisX.Interval = 2.0R
        ChartArea16.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea16.AxisX.LabelStyle.Enabled = False
        ChartArea16.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea16.AxisX.Minimum = 0R
        ChartArea16.AxisX.Title = "Time (Sec)"
        ChartArea16.AxisX.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea16.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea16.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea16.AxisY.Maximum = 70.0R
        ChartArea16.AxisY.Minimum = 0R
        ChartArea16.AxisY.Title = "Frequency (Hz)"
        ChartArea16.AxisY.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea16.AxisY2.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.[False]
        ChartArea16.AxisY2.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea16.AxisY2.Maximum = 50.0R
        ChartArea16.AxisY2.Minimum = 0R
        ChartArea16.AxisY2.Title = "Temperature"
        ChartArea16.Name = "ChartArea1"
        Me.ChartFrequency.ChartAreas.Add(ChartArea16)
        Me.ChartFrequency.Location = New System.Drawing.Point(10, 32)
        Me.ChartFrequency.Name = "ChartFrequency"
        Series16.BorderWidth = 2
        Series16.ChartArea = "ChartArea1"
        Series16.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series16.Color = System.Drawing.Color.DarkTurquoise
        Series16.LegendText = "Frequency"
        Series16.Name = "Freq"
        Me.ChartFrequency.Series.Add(Series16)
        Me.ChartFrequency.Size = New System.Drawing.Size(568, 372)
        Me.ChartFrequency.TabIndex = 2
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
        Me.Label1.Size = New System.Drawing.Size(418, 18)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Graph Frequency (Frequency Real-Time/Sec)"
        '
        'ChartControl1
        '
        Me.ChartControl1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartControl1.ChartPanel.Legend.Visible = False
        Me.ChartControl1.ChartPanel.Name = "PrimaryPanel"
        Background57.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.MouseOver.ArrowBackground = Background57
        Background58.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.MouseOver.ThumbBackground = Background58
        Background59.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.SelectedMouseOver.ArrowBackground = Background59
        Background60.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.SelectedMouseOver.ThumbBackground = Background60
        Background61.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.MouseOver.ArrowBackground = Background61
        Background62.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.MouseOver.ThumbBackground = Background62
        Background63.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.SelectedMouseOver.ArrowBackground = Background63
        Background64.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.SelectedMouseOver.ThumbBackground = Background64
        Me.ChartControl1.Location = New System.Drawing.Point(3, 22)
        Me.ChartControl1.Name = "ChartControl1"
        Me.ChartControl1.Size = New System.Drawing.Size(933, 851)
        Me.ChartControl1.TabIndex = 0
        Me.ChartControl1.Text = "ChartControl1"
        '
        'GrpChartFrequency
        '
        Me.GrpChartFrequency.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpChartFrequency.Controls.Add(Me.GroupBox1)
        Me.GrpChartFrequency.Controls.Add(Me.Panel4)
        Me.GrpChartFrequency.Controls.Add(Me.Panel7)
        Me.GrpChartFrequency.Controls.Add(Me.Panel5)
        Me.GrpChartFrequency.Controls.Add(Me.Panel1)
        Me.GrpChartFrequency.Controls.Add(Me.Panel2)
        Me.GrpChartFrequency.Controls.Add(Me.ChartControl1)
        Me.GrpChartFrequency.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpChartFrequency.Location = New System.Drawing.Point(11, 16)
        Me.GrpChartFrequency.Name = "GrpChartFrequency"
        Me.GrpChartFrequency.Size = New System.Drawing.Size(942, 879)
        Me.GrpChartFrequency.TabIndex = 15
        Me.GrpChartFrequency.TabStop = False
        Me.GrpChartFrequency.Text = "Frequency (Real-Time/Sec)"
        '
        'Panel2
        '
        Me.Panel2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel2.BackColor = System.Drawing.Color.White
        Me.Panel2.Controls.Add(Me.ChartFrequency)
        Me.Panel2.Controls.Add(Me.Panel3)
        Me.Panel2.Location = New System.Drawing.Point(6, 25)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(588, 412)
        Me.Panel2.TabIndex = 3
        '
        'Timer
        '
        Me.Timer.HeaderText = "Timer"
        Me.Timer.Name = "Timer"
        '
        'Freq
        '
        Me.Freq.HeaderText = "Frequency (Hz)"
        Me.Freq.Name = "Freq"
        '
        'FrmSMGE_3222M_FrequencyData
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(964, 951)
        Me.Controls.Add(Me.ProgressBarSave)
        Me.Controls.Add(Me.GrpChartFrequency)
        Me.Name = "FrmSMGE_3222M_FrequencyData"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE 3222M Frequency Data"
        CType(Me.DataGridViewFrequency, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel5.ResumeLayout(False)
        Me.Panel6.ResumeLayout(False)
        Me.Panel6.PerformLayout()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.ChartFrequencyRec, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel8.ResumeLayout(False)
        Me.Panel8.PerformLayout()
        Me.Panel7.ResumeLayout(False)
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        Me.GroupBox1.ResumeLayout(False)
        Me.Panel4.ResumeLayout(False)
        Me.Panel4.PerformLayout()
        CType(Me.ChartFrequency, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel3.ResumeLayout(False)
        Me.Panel3.PerformLayout()
        Me.GrpChartFrequency.ResumeLayout(False)
        Me.Panel2.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents CmdClearRecording As Button
    Friend WithEvents CmdExportToExcel As Button
    Friend WithEvents DataGridViewFrequency As DataGridView
    Friend WithEvents Panel5 As Panel
    Friend WithEvents Panel6 As Panel
    Friend WithEvents LblTime As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents PictureBoxRecordInd As PictureBox
    Friend WithEvents Label4 As Label
    Friend WithEvents LblRecording As Label
    Friend WithEvents ChartFrequencyRec As DataVisualization.Charting.Chart
    Friend WithEvents Panel8 As Panel
    Friend WithEvents Panel7 As Panel
    Friend WithEvents ButtonRecord As Button
    Friend WithEvents CmdReadData As Button
    Friend WithEvents ProgressBarSave As ProgressBar
    Friend WithEvents Panel1 As Panel
    Friend WithEvents TxtFrequency As TextBox
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents Panel4 As Panel
    Friend WithEvents Label2 As Label
    Friend WithEvents ChartFrequency As DataVisualization.Charting.Chart
    Friend WithEvents Panel3 As Panel
    Friend WithEvents Label1 As Label
    Friend WithEvents ChartControl1 As DevComponents.DotNetBar.Charts.ChartControl
    Friend WithEvents GrpChartFrequency As GroupBox
    Friend WithEvents Panel2 As Panel
    Friend WithEvents Timer As DataGridViewTextBoxColumn
    Friend WithEvents Freq As DataGridViewTextBoxColumn
End Class
