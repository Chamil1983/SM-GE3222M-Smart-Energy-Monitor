<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_ActPowerData
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
        Dim ChartArea1 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend1 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim Series1 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series2 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series3 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series4 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim ChartArea2 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend2 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim Series5 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series6 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series7 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series8 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Background1 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background2 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background3 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background4 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background5 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background6 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background7 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background8 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.TxtPower = New System.Windows.Forms.TextBox()
        Me.Panel4 = New System.Windows.Forms.Panel()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Panel2 = New System.Windows.Forms.Panel()
        Me.ChartActPower = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel3 = New System.Windows.Forms.Panel()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.TimerGaugeCom = New System.Windows.Forms.Timer(Me.components)
        Me.ProgressBarSave = New System.Windows.Forms.ProgressBar()
        Me.Panel6 = New System.Windows.Forms.Panel()
        Me.LblTime = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.GrpChartActPower = New System.Windows.Forms.GroupBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.ButtonRecord = New System.Windows.Forms.Button()
        Me.CmdReadData = New System.Windows.Forms.Button()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.Panel7 = New System.Windows.Forms.Panel()
        Me.ChartPowerRec = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel8 = New System.Windows.Forms.Panel()
        Me.PictureBoxRecordInd = New System.Windows.Forms.PictureBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.LblRecording = New System.Windows.Forms.Label()
        Me.Panel5 = New System.Windows.Forms.Panel()
        Me.CmdClearRecording = New System.Windows.Forms.Button()
        Me.CmdExportToExcel = New System.Windows.Forms.Button()
        Me.DataGridViewActPower = New System.Windows.Forms.DataGridView()
        Me.Timer = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P1 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P2 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.P3 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Ptot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.ChartControl1 = New DevComponents.DotNetBar.Charts.ChartControl()
        Me.Panel1.SuspendLayout()
        Me.Panel4.SuspendLayout()
        Me.Panel2.SuspendLayout()
        CType(Me.ChartActPower, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel3.SuspendLayout()
        Me.Panel6.SuspendLayout()
        Me.GrpChartActPower.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.Panel7.SuspendLayout()
        CType(Me.ChartPowerRec, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel8.SuspendLayout()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel5.SuspendLayout()
        CType(Me.DataGridViewActPower, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'Panel1
        '
        Me.Panel1.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel1.BackColor = System.Drawing.Color.White
        Me.Panel1.Controls.Add(Me.TxtPower)
        Me.Panel1.Controls.Add(Me.Panel4)
        Me.Panel1.Location = New System.Drawing.Point(600, 25)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(331, 409)
        Me.Panel1.TabIndex = 2
        '
        'TxtPower
        '
        Me.TxtPower.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TxtPower.BackColor = System.Drawing.SystemColors.Control
        Me.TxtPower.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TxtPower.Location = New System.Drawing.Point(3, 32)
        Me.TxtPower.Multiline = True
        Me.TxtPower.Name = "TxtPower"
        Me.TxtPower.ReadOnly = True
        Me.TxtPower.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.TxtPower.Size = New System.Drawing.Size(325, 372)
        Me.TxtPower.TabIndex = 9
        '
        'Panel4
        '
        Me.Panel4.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel4.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel4.Controls.Add(Me.Label2)
        Me.Panel4.Location = New System.Drawing.Point(0, 0)
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
        Me.Label2.Size = New System.Drawing.Size(248, 18)
        Me.Label2.TabIndex = 0
        Me.Label2.Text = "Active Power (Real-Time)"
        '
        'Panel2
        '
        Me.Panel2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel2.BackColor = System.Drawing.Color.White
        Me.Panel2.Controls.Add(Me.ChartActPower)
        Me.Panel2.Controls.Add(Me.Panel3)
        Me.Panel2.Location = New System.Drawing.Point(6, 25)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(588, 412)
        Me.Panel2.TabIndex = 3
        '
        'ChartActPower
        '
        Me.ChartActPower.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartActPower.AntiAliasing = System.Windows.Forms.DataVisualization.Charting.AntiAliasingStyles.Text
        Me.ChartActPower.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartActPower.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea1.AxisX.Interval = 2.0R
        ChartArea1.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea1.AxisX.LabelStyle.Enabled = False
        ChartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea1.AxisX.Minimum = 0R
        ChartArea1.AxisX.Title = "Time (Sec)"
        ChartArea1.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea1.AxisY.Minimum = 0R
        ChartArea1.AxisY.Title = "Active Power (W)"
        ChartArea1.AxisY2.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.[False]
        ChartArea1.AxisY2.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea1.AxisY2.Maximum = 50.0R
        ChartArea1.AxisY2.Minimum = 0R
        ChartArea1.AxisY2.Title = "Temperature"
        ChartArea1.Name = "ChartArea1"
        Me.ChartActPower.ChartAreas.Add(ChartArea1)
        Legend1.Alignment = System.Drawing.StringAlignment.Center
        Legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend1.Name = "Legend1"
        Legend1.TextWrapThreshold = 40
        Me.ChartActPower.Legends.Add(Legend1)
        Me.ChartActPower.Location = New System.Drawing.Point(10, 32)
        Me.ChartActPower.Name = "ChartActPower"
        Series1.BorderWidth = 2
        Series1.ChartArea = "ChartArea1"
        Series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series1.Color = System.Drawing.Color.Crimson
        Series1.Legend = "Legend1"
        Series1.LegendText = "Pha 1 Power"
        Series1.Name = "P1"
        Series2.BorderWidth = 2
        Series2.ChartArea = "ChartArea1"
        Series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series2.Color = System.Drawing.Color.Gold
        Series2.Legend = "Legend1"
        Series2.LegendText = "Pha 2 Power"
        Series2.Name = "P2"
        Series3.BorderWidth = 2
        Series3.ChartArea = "ChartArea1"
        Series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series3.Color = System.Drawing.Color.DodgerBlue
        Series3.Legend = "Legend1"
        Series3.LegendText = "Pha 3 Power"
        Series3.Name = "P3"
        Series4.BorderWidth = 2
        Series4.ChartArea = "ChartArea1"
        Series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series4.Color = System.Drawing.Color.DarkOrchid
        Series4.Legend = "Legend1"
        Series4.LegendText = "Total Power"
        Series4.Name = "Ptot"
        Me.ChartActPower.Series.Add(Series1)
        Me.ChartActPower.Series.Add(Series2)
        Me.ChartActPower.Series.Add(Series3)
        Me.ChartActPower.Series.Add(Series4)
        Me.ChartActPower.Size = New System.Drawing.Size(568, 372)
        Me.ChartActPower.TabIndex = 2
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
        Me.Label1.Size = New System.Drawing.Size(408, 18)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Graph Power (Active Power Real-Time/Sec)"
        '
        'TimerGaugeCom
        '
        Me.TimerGaugeCom.Enabled = True
        Me.TimerGaugeCom.Interval = 1000
        '
        'ProgressBarSave
        '
        Me.ProgressBarSave.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ProgressBarSave.Location = New System.Drawing.Point(11, 891)
        Me.ProgressBarSave.Maximum = 10
        Me.ProgressBarSave.Name = "ProgressBarSave"
        Me.ProgressBarSave.Size = New System.Drawing.Size(942, 26)
        Me.ProgressBarSave.TabIndex = 8
        Me.ProgressBarSave.Visible = False
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
        'GrpChartActPower
        '
        Me.GrpChartActPower.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpChartActPower.Controls.Add(Me.GroupBox1)
        Me.GrpChartActPower.Controls.Add(Me.Panel7)
        Me.GrpChartActPower.Controls.Add(Me.Panel5)
        Me.GrpChartActPower.Controls.Add(Me.Panel1)
        Me.GrpChartActPower.Controls.Add(Me.Panel2)
        Me.GrpChartActPower.Controls.Add(Me.ChartControl1)
        Me.GrpChartActPower.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpChartActPower.Location = New System.Drawing.Point(11, 8)
        Me.GrpChartActPower.Name = "GrpChartActPower"
        Me.GrpChartActPower.Size = New System.Drawing.Size(942, 879)
        Me.GrpChartActPower.TabIndex = 7
        Me.GrpChartActPower.TabStop = False
        Me.GrpChartActPower.Text = "Active Power (Real-Time/Sec)"
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
        'Panel7
        '
        Me.Panel7.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel7.BackColor = System.Drawing.Color.White
        Me.Panel7.Controls.Add(Me.ChartPowerRec)
        Me.Panel7.Controls.Add(Me.Panel8)
        Me.Panel7.Location = New System.Drawing.Point(6, 443)
        Me.Panel7.Name = "Panel7"
        Me.Panel7.Size = New System.Drawing.Size(588, 349)
        Me.Panel7.TabIndex = 6
        '
        'ChartPowerRec
        '
        Me.ChartPowerRec.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartPowerRec.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartPowerRec.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea2.AxisX.Interval = 1.0R
        ChartArea2.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisX.IsLabelAutoFit = False
        ChartArea2.AxisX.LabelStyle.Angle = -45
        ChartArea2.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea2.AxisX.Minimum = 0R
        ChartArea2.AxisX.Title = "Time (Sec)"
        ChartArea2.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea2.AxisY.Minimum = 0R
        ChartArea2.AxisY.Title = "Active Power (W)"
        ChartArea2.Name = "ChartArea1"
        Me.ChartPowerRec.ChartAreas.Add(ChartArea2)
        Legend2.Alignment = System.Drawing.StringAlignment.Center
        Legend2.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend2.Name = "Legend1"
        Me.ChartPowerRec.Legends.Add(Legend2)
        Me.ChartPowerRec.Location = New System.Drawing.Point(7, 37)
        Me.ChartPowerRec.Name = "ChartPowerRec"
        Series5.ChartArea = "ChartArea1"
        Series5.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series5.Color = System.Drawing.Color.Crimson
        Series5.IsValueShownAsLabel = True
        Series5.LabelForeColor = System.Drawing.Color.Crimson
        Series5.Legend = "Legend1"
        Series5.LegendText = "Pha 1 Power"
        Series5.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Square
        Series5.Name = "Series1"
        Series6.ChartArea = "ChartArea1"
        Series6.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series6.Color = System.Drawing.Color.FromArgb(CType(CType(255, Byte), Integer), CType(CType(192, Byte), Integer), CType(CType(128, Byte), Integer))
        Series6.IsValueShownAsLabel = True
        Series6.LabelForeColor = System.Drawing.Color.Orange
        Series6.Legend = "Legend1"
        Series6.LegendText = "Pha 2 Power"
        Series6.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Circle
        Series6.Name = "Series2"
        Series7.ChartArea = "ChartArea1"
        Series7.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series7.Color = System.Drawing.Color.RoyalBlue
        Series7.IsValueShownAsLabel = True
        Series7.LabelForeColor = System.Drawing.Color.RoyalBlue
        Series7.Legend = "Legend1"
        Series7.LegendText = "Pha 3 Power"
        Series7.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Triangle
        Series7.Name = "Series3"
        Series8.ChartArea = "ChartArea1"
        Series8.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series8.Color = System.Drawing.Color.DarkViolet
        Series8.IsValueShownAsLabel = True
        Series8.LabelForeColor = System.Drawing.Color.DarkViolet
        Series8.Legend = "Legend1"
        Series8.LegendText = "Total Power"
        Series8.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Diamond
        Series8.Name = "Series4"
        Me.ChartPowerRec.Series.Add(Series5)
        Me.ChartPowerRec.Series.Add(Series6)
        Me.ChartPowerRec.Series.Add(Series7)
        Me.ChartPowerRec.Series.Add(Series8)
        Me.ChartPowerRec.Size = New System.Drawing.Size(575, 300)
        Me.ChartPowerRec.TabIndex = 2
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
        Me.Label4.Size = New System.Drawing.Size(278, 18)
        Me.Label4.TabIndex = 0
        Me.Label4.Text = "Record Graph (Active Power)"
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
        'Panel5
        '
        Me.Panel5.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel5.BackColor = System.Drawing.Color.White
        Me.Panel5.Controls.Add(Me.CmdClearRecording)
        Me.Panel5.Controls.Add(Me.CmdExportToExcel)
        Me.Panel5.Controls.Add(Me.DataGridViewActPower)
        Me.Panel5.Controls.Add(Me.Panel6)
        Me.Panel5.Location = New System.Drawing.Point(600, 440)
        Me.Panel5.Name = "Panel5"
        Me.Panel5.Size = New System.Drawing.Size(331, 433)
        Me.Panel5.TabIndex = 1
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
        'DataGridViewActPower
        '
        Me.DataGridViewActPower.AllowUserToAddRows = False
        Me.DataGridViewActPower.AllowUserToDeleteRows = False
        Me.DataGridViewActPower.AllowUserToResizeRows = False
        Me.DataGridViewActPower.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.DataGridViewActPower.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.ColumnHeader
        Me.DataGridViewActPower.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridViewActPower.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.Timer, Me.P1, Me.P2, Me.P3, Me.Ptot})
        Me.DataGridViewActPower.Location = New System.Drawing.Point(3, 76)
        Me.DataGridViewActPower.Name = "DataGridViewActPower"
        Me.DataGridViewActPower.RowHeadersVisible = False
        Me.DataGridViewActPower.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect
        Me.DataGridViewActPower.Size = New System.Drawing.Size(328, 354)
        Me.DataGridViewActPower.TabIndex = 3
        '
        'Timer
        '
        Me.Timer.HeaderText = "Timer"
        Me.Timer.Name = "Timer"
        Me.Timer.Width = 83
        '
        'P1
        '
        Me.P1.HeaderText = "Pha 1 Power (W)"
        Me.P1.Name = "P1"
        Me.P1.Width = 140
        '
        'P2
        '
        Me.P2.HeaderText = "Pha 2 Power (W)"
        Me.P2.Name = "P2"
        Me.P2.Width = 140
        '
        'P3
        '
        Me.P3.HeaderText = "Pha 3 Power (W)"
        Me.P3.Name = "P3"
        Me.P3.Width = 140
        '
        'Ptot
        '
        Me.Ptot.HeaderText = "Total Power (W)"
        Me.Ptot.Name = "Ptot"
        Me.Ptot.Width = 140
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
        'FrmSMGE_3222M_ActPowerData
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(964, 951)
        Me.Controls.Add(Me.ProgressBarSave)
        Me.Controls.Add(Me.GrpChartActPower)
        Me.Name = "FrmSMGE_3222M_ActPowerData"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE 3222M Active Power Data"
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        Me.Panel4.ResumeLayout(False)
        Me.Panel4.PerformLayout()
        Me.Panel2.ResumeLayout(False)
        CType(Me.ChartActPower, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel3.ResumeLayout(False)
        Me.Panel3.PerformLayout()
        Me.Panel6.ResumeLayout(False)
        Me.Panel6.PerformLayout()
        Me.GrpChartActPower.ResumeLayout(False)
        Me.GroupBox1.ResumeLayout(False)
        Me.Panel7.ResumeLayout(False)
        CType(Me.ChartPowerRec, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel8.ResumeLayout(False)
        Me.Panel8.PerformLayout()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel5.ResumeLayout(False)
        CType(Me.DataGridViewActPower, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents Panel1 As Panel
    Friend WithEvents Panel4 As Panel
    Friend WithEvents Label2 As Label
    Friend WithEvents Panel2 As Panel
    Friend WithEvents ChartActPower As DataVisualization.Charting.Chart
    Friend WithEvents Panel3 As Panel
    Friend WithEvents Label1 As Label
    Friend WithEvents PictureBoxRecordInd As PictureBox
    Friend WithEvents TimerGaugeCom As Timer
    Friend WithEvents ProgressBarSave As ProgressBar
    Friend WithEvents Panel6 As Panel
    Friend WithEvents LblTime As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents GrpChartActPower As GroupBox
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents ButtonRecord As Button
    Friend WithEvents CmdReadData As Button
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents Panel7 As Panel
    Friend WithEvents ChartPowerRec As DataVisualization.Charting.Chart
    Friend WithEvents Panel8 As Panel
    Friend WithEvents Label4 As Label
    Friend WithEvents LblRecording As Label
    Friend WithEvents Panel5 As Panel
    Friend WithEvents CmdClearRecording As Button
    Friend WithEvents CmdExportToExcel As Button
    Friend WithEvents DataGridViewActPower As DataGridView
    Friend WithEvents ChartControl1 As DevComponents.DotNetBar.Charts.ChartControl
    Friend WithEvents TxtPower As TextBox
    Friend WithEvents Timer As DataGridViewTextBoxColumn
    Friend WithEvents P1 As DataGridViewTextBoxColumn
    Friend WithEvents P2 As DataGridViewTextBoxColumn
    Friend WithEvents P3 As DataGridViewTextBoxColumn
    Friend WithEvents Ptot As DataGridViewTextBoxColumn
End Class
