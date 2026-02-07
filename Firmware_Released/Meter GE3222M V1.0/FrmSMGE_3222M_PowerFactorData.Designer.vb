<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_PowerFactorData
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
        Dim ChartArea3 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend3 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim Series9 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series10 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series11 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series12 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Background9 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background10 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background11 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background12 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background13 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background14 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background15 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background16 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim ChartArea4 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend4 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim Series13 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series14 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series15 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series16 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Me.ProgressBarSave = New System.Windows.Forms.ProgressBar()
        Me.Panel6 = New System.Windows.Forms.Panel()
        Me.LblTime = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.TxtPowerFactor = New System.Windows.Forms.TextBox()
        Me.Panel4 = New System.Windows.Forms.Panel()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Panel2 = New System.Windows.Forms.Panel()
        Me.ChartPowerFactor = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel3 = New System.Windows.Forms.Panel()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ChartControl1 = New DevComponents.DotNetBar.Charts.ChartControl()
        Me.GrpChartPowerFactor = New System.Windows.Forms.GroupBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.ButtonRecord = New System.Windows.Forms.Button()
        Me.CmdReadData = New System.Windows.Forms.Button()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.Panel7 = New System.Windows.Forms.Panel()
        Me.ChartPowerFactorRec = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel8 = New System.Windows.Forms.Panel()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.LblRecording = New System.Windows.Forms.Label()
        Me.Panel5 = New System.Windows.Forms.Panel()
        Me.CmdClearRecording = New System.Windows.Forms.Button()
        Me.CmdExportToExcel = New System.Windows.Forms.Button()
        Me.DataGridViewPowerFactor = New System.Windows.Forms.DataGridView()
        Me.Timer = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PF1 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PF2 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PF3 = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PFtot = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.PictureBoxRecordInd = New System.Windows.Forms.PictureBox()
        Me.Panel6.SuspendLayout()
        Me.Panel1.SuspendLayout()
        Me.Panel4.SuspendLayout()
        Me.Panel2.SuspendLayout()
        CType(Me.ChartPowerFactor, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel3.SuspendLayout()
        Me.GrpChartPowerFactor.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.Panel7.SuspendLayout()
        CType(Me.ChartPowerFactorRec, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel8.SuspendLayout()
        Me.Panel5.SuspendLayout()
        CType(Me.DataGridViewPowerFactor, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'ProgressBarSave
        '
        Me.ProgressBarSave.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ProgressBarSave.Location = New System.Drawing.Point(11, 913)
        Me.ProgressBarSave.Maximum = 10
        Me.ProgressBarSave.Name = "ProgressBarSave"
        Me.ProgressBarSave.Size = New System.Drawing.Size(942, 26)
        Me.ProgressBarSave.TabIndex = 14
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
        'Panel1
        '
        Me.Panel1.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel1.BackColor = System.Drawing.Color.White
        Me.Panel1.Controls.Add(Me.TxtPowerFactor)
        Me.Panel1.Location = New System.Drawing.Point(600, 25)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(331, 412)
        Me.Panel1.TabIndex = 2
        '
        'TxtPowerFactor
        '
        Me.TxtPowerFactor.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.TxtPowerFactor.BackColor = System.Drawing.SystemColors.Control
        Me.TxtPowerFactor.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TxtPowerFactor.Location = New System.Drawing.Point(3, 32)
        Me.TxtPowerFactor.Multiline = True
        Me.TxtPowerFactor.Name = "TxtPowerFactor"
        Me.TxtPowerFactor.ReadOnly = True
        Me.TxtPowerFactor.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.TxtPowerFactor.Size = New System.Drawing.Size(325, 377)
        Me.TxtPowerFactor.TabIndex = 9
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
        Me.Label2.Size = New System.Drawing.Size(248, 18)
        Me.Label2.TabIndex = 0
        Me.Label2.Text = "Power Factor (Real-Time)"
        '
        'Panel2
        '
        Me.Panel2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel2.BackColor = System.Drawing.Color.White
        Me.Panel2.Controls.Add(Me.ChartPowerFactor)
        Me.Panel2.Controls.Add(Me.Panel3)
        Me.Panel2.Location = New System.Drawing.Point(6, 25)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(588, 412)
        Me.Panel2.TabIndex = 3
        '
        'ChartPowerFactor
        '
        Me.ChartPowerFactor.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartPowerFactor.AntiAliasing = System.Windows.Forms.DataVisualization.Charting.AntiAliasingStyles.Text
        Me.ChartPowerFactor.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartPowerFactor.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea3.AxisX.Interval = 2.0R
        ChartArea3.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea3.AxisX.LabelStyle.Enabled = False
        ChartArea3.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea3.AxisX.Minimum = 0R
        ChartArea3.AxisX.Title = "Time (Sec)"
        ChartArea3.AxisX.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea3.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea3.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea3.AxisY.Maximum = 1.0R
        ChartArea3.AxisY.Minimum = -1.0R
        ChartArea3.AxisY.Title = "Power Factor "
        ChartArea3.AxisY.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea3.AxisY2.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.[False]
        ChartArea3.AxisY2.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea3.AxisY2.Maximum = 50.0R
        ChartArea3.AxisY2.Minimum = 0R
        ChartArea3.AxisY2.Title = "Temperature"
        ChartArea3.Name = "ChartArea1"
        Me.ChartPowerFactor.ChartAreas.Add(ChartArea3)
        Legend3.Alignment = System.Drawing.StringAlignment.Center
        Legend3.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend3.Name = "Legend1"
        Legend3.TextWrapThreshold = 40
        Me.ChartPowerFactor.Legends.Add(Legend3)
        Me.ChartPowerFactor.Location = New System.Drawing.Point(10, 32)
        Me.ChartPowerFactor.Name = "ChartPowerFactor"
        Series9.BorderWidth = 2
        Series9.ChartArea = "ChartArea1"
        Series9.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series9.Color = System.Drawing.Color.Crimson
        Series9.Legend = "Legend1"
        Series9.LegendText = "Power Factor (Pha 1)"
        Series9.Name = "PF1"
        Series10.BorderWidth = 2
        Series10.ChartArea = "ChartArea1"
        Series10.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series10.Color = System.Drawing.Color.Gold
        Series10.Legend = "Legend1"
        Series10.LegendText = "Power Factor (Pha 2)"
        Series10.Name = "PF2"
        Series11.BorderWidth = 2
        Series11.ChartArea = "ChartArea1"
        Series11.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series11.Color = System.Drawing.Color.DeepSkyBlue
        Series11.Legend = "Legend1"
        Series11.LegendText = "Power Factor (Pha 3)"
        Series11.Name = "PF3"
        Series12.BorderWidth = 2
        Series12.ChartArea = "ChartArea1"
        Series12.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series12.Color = System.Drawing.Color.MediumSeaGreen
        Series12.Legend = "Legend1"
        Series12.LegendText = "Total Power Factor"
        Series12.Name = "PFtot"
        Me.ChartPowerFactor.Series.Add(Series9)
        Me.ChartPowerFactor.Series.Add(Series10)
        Me.ChartPowerFactor.Series.Add(Series11)
        Me.ChartPowerFactor.Series.Add(Series12)
        Me.ChartPowerFactor.Size = New System.Drawing.Size(568, 372)
        Me.ChartPowerFactor.TabIndex = 2
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
        Me.Label1.Size = New System.Drawing.Size(478, 18)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Graph Power Factor (Power Factor Real-Time/Sec)"
        '
        'ChartControl1
        '
        Me.ChartControl1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartControl1.ChartPanel.Legend.Visible = False
        Me.ChartControl1.ChartPanel.Name = "PrimaryPanel"
        Background9.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.MouseOver.ArrowBackground = Background9
        Background10.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.MouseOver.ThumbBackground = Background10
        Background11.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.SelectedMouseOver.ArrowBackground = Background11
        Background12.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.HScrollBarVisualStyles.SelectedMouseOver.ThumbBackground = Background12
        Background13.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.MouseOver.ArrowBackground = Background13
        Background14.Color1 = System.Drawing.Color.AliceBlue
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.MouseOver.ThumbBackground = Background14
        Background15.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.SelectedMouseOver.ArrowBackground = Background15
        Background16.Color1 = System.Drawing.Color.White
        Me.ChartControl1.DefaultVisualStyles.VScrollBarVisualStyles.SelectedMouseOver.ThumbBackground = Background16
        Me.ChartControl1.Location = New System.Drawing.Point(3, 22)
        Me.ChartControl1.Name = "ChartControl1"
        Me.ChartControl1.Size = New System.Drawing.Size(933, 851)
        Me.ChartControl1.TabIndex = 0
        Me.ChartControl1.Text = "ChartControl1"
        '
        'GrpChartPowerFactor
        '
        Me.GrpChartPowerFactor.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpChartPowerFactor.Controls.Add(Me.GroupBox1)
        Me.GrpChartPowerFactor.Controls.Add(Me.Panel4)
        Me.GrpChartPowerFactor.Controls.Add(Me.Panel7)
        Me.GrpChartPowerFactor.Controls.Add(Me.Panel5)
        Me.GrpChartPowerFactor.Controls.Add(Me.Panel1)
        Me.GrpChartPowerFactor.Controls.Add(Me.Panel2)
        Me.GrpChartPowerFactor.Controls.Add(Me.ChartControl1)
        Me.GrpChartPowerFactor.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpChartPowerFactor.Location = New System.Drawing.Point(11, 21)
        Me.GrpChartPowerFactor.Name = "GrpChartPowerFactor"
        Me.GrpChartPowerFactor.Size = New System.Drawing.Size(942, 879)
        Me.GrpChartPowerFactor.TabIndex = 13
        Me.GrpChartPowerFactor.TabStop = False
        Me.GrpChartPowerFactor.Text = "Power Factor (Real-Time/Sec)"
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
        Me.Panel7.Controls.Add(Me.ChartPowerFactorRec)
        Me.Panel7.Controls.Add(Me.Panel8)
        Me.Panel7.Location = New System.Drawing.Point(6, 443)
        Me.Panel7.Name = "Panel7"
        Me.Panel7.Size = New System.Drawing.Size(588, 349)
        Me.Panel7.TabIndex = 6
        '
        'ChartPowerFactorRec
        '
        Me.ChartPowerFactorRec.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartPowerFactorRec.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartPowerFactorRec.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea4.AxisX.Interval = 1.0R
        ChartArea4.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea4.AxisX.IsLabelAutoFit = False
        ChartArea4.AxisX.LabelStyle.Angle = -45
        ChartArea4.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea4.AxisX.Minimum = 0R
        ChartArea4.AxisX.Title = "Time (Sec)"
        ChartArea4.AxisX.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea4.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea4.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea4.AxisY.Maximum = 1.0R
        ChartArea4.AxisY.Minimum = -1.0R
        ChartArea4.AxisY.Title = "Power Factor"
        ChartArea4.AxisY.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea4.Name = "ChartArea1"
        Me.ChartPowerFactorRec.ChartAreas.Add(ChartArea4)
        Legend4.Alignment = System.Drawing.StringAlignment.Center
        Legend4.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend4.Name = "Legend1"
        Me.ChartPowerFactorRec.Legends.Add(Legend4)
        Me.ChartPowerFactorRec.Location = New System.Drawing.Point(7, 37)
        Me.ChartPowerFactorRec.Name = "ChartPowerFactorRec"
        Series13.ChartArea = "ChartArea1"
        Series13.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series13.Color = System.Drawing.Color.Crimson
        Series13.IsValueShownAsLabel = True
        Series13.LabelForeColor = System.Drawing.Color.Crimson
        Series13.Legend = "Legend1"
        Series13.LegendText = "Power Factor (Pha 1)"
        Series13.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Square
        Series13.Name = "Series1"
        Series14.ChartArea = "ChartArea1"
        Series14.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series14.Color = System.Drawing.Color.FromArgb(CType(CType(255, Byte), Integer), CType(CType(192, Byte), Integer), CType(CType(128, Byte), Integer))
        Series14.IsValueShownAsLabel = True
        Series14.LabelForeColor = System.Drawing.Color.Orange
        Series14.Legend = "Legend1"
        Series14.LegendText = "Power Factor (Pha 2)"
        Series14.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Circle
        Series14.Name = "Series2"
        Series15.ChartArea = "ChartArea1"
        Series15.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series15.Color = System.Drawing.Color.RoyalBlue
        Series15.IsValueShownAsLabel = True
        Series15.LabelForeColor = System.Drawing.Color.RoyalBlue
        Series15.Legend = "Legend1"
        Series15.LegendText = "Power Factor (Pha 3)"
        Series15.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Triangle
        Series15.Name = "Series3"
        Series16.BorderWidth = 2
        Series16.ChartArea = "ChartArea1"
        Series16.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series16.Color = System.Drawing.Color.MediumSeaGreen
        Series16.IsValueShownAsLabel = True
        Series16.LabelForeColor = System.Drawing.Color.MediumSeaGreen
        Series16.Legend = "Legend1"
        Series16.LegendText = "Total Power Factor"
        Series16.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Diamond
        Series16.Name = "Series4"
        Me.ChartPowerFactorRec.Series.Add(Series13)
        Me.ChartPowerFactorRec.Series.Add(Series14)
        Me.ChartPowerFactorRec.Series.Add(Series15)
        Me.ChartPowerFactorRec.Series.Add(Series16)
        Me.ChartPowerFactorRec.Size = New System.Drawing.Size(575, 300)
        Me.ChartPowerFactorRec.TabIndex = 2
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
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.ForeColor = System.Drawing.Color.White
        Me.Label4.Location = New System.Drawing.Point(7, 5)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(278, 18)
        Me.Label4.TabIndex = 0
        Me.Label4.Text = "Record Graph (Power Factor)"
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
        Me.Panel5.Controls.Add(Me.DataGridViewPowerFactor)
        Me.Panel5.Controls.Add(Me.Panel6)
        Me.Panel5.Location = New System.Drawing.Point(600, 443)
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
        'DataGridViewPowerFactor
        '
        Me.DataGridViewPowerFactor.AllowUserToAddRows = False
        Me.DataGridViewPowerFactor.AllowUserToDeleteRows = False
        Me.DataGridViewPowerFactor.AllowUserToResizeRows = False
        Me.DataGridViewPowerFactor.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.DataGridViewPowerFactor.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.ColumnHeader
        Me.DataGridViewPowerFactor.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridViewPowerFactor.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.Timer, Me.PF1, Me.PF2, Me.PF3, Me.PFtot})
        Me.DataGridViewPowerFactor.Location = New System.Drawing.Point(3, 76)
        Me.DataGridViewPowerFactor.Name = "DataGridViewPowerFactor"
        Me.DataGridViewPowerFactor.RowHeadersVisible = False
        Me.DataGridViewPowerFactor.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect
        Me.DataGridViewPowerFactor.Size = New System.Drawing.Size(328, 354)
        Me.DataGridViewPowerFactor.TabIndex = 3
        '
        'Timer
        '
        Me.Timer.HeaderText = "Timer"
        Me.Timer.Name = "Timer"
        Me.Timer.Width = 83
        '
        'PF1
        '
        Me.PF1.HeaderText = "Power Factor (Pha 1)"
        Me.PF1.Name = "PF1"
        Me.PF1.Width = 185
        '
        'PF2
        '
        Me.PF2.HeaderText = "Power Factor (Pha 2)"
        Me.PF2.Name = "PF2"
        Me.PF2.Width = 185
        '
        'PF3
        '
        Me.PF3.HeaderText = "Power Factor (Pha 3)"
        Me.PF3.Name = "PF3"
        Me.PF3.Width = 185
        '
        'PFtot
        '
        Me.PFtot.HeaderText = "Total Power Factor"
        Me.PFtot.Name = "PFtot"
        Me.PFtot.Width = 194
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
        'FrmSMGE_3222M_PowerFactorData
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(964, 951)
        Me.Controls.Add(Me.ProgressBarSave)
        Me.Controls.Add(Me.GrpChartPowerFactor)
        Me.Name = "FrmSMGE_3222M_PowerFactorData"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE 3222M Power Factor Data"
        Me.Panel6.ResumeLayout(False)
        Me.Panel6.PerformLayout()
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        Me.Panel4.ResumeLayout(False)
        Me.Panel4.PerformLayout()
        Me.Panel2.ResumeLayout(False)
        CType(Me.ChartPowerFactor, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel3.ResumeLayout(False)
        Me.Panel3.PerformLayout()
        Me.GrpChartPowerFactor.ResumeLayout(False)
        Me.GroupBox1.ResumeLayout(False)
        Me.Panel7.ResumeLayout(False)
        CType(Me.ChartPowerFactorRec, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel8.ResumeLayout(False)
        Me.Panel8.PerformLayout()
        Me.Panel5.ResumeLayout(False)
        CType(Me.DataGridViewPowerFactor, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents ProgressBarSave As ProgressBar
    Friend WithEvents Panel6 As Panel
    Friend WithEvents LblTime As Label
    Friend WithEvents Label3 As Label
    Friend WithEvents Panel1 As Panel
    Friend WithEvents TxtPowerFactor As TextBox
    Friend WithEvents Panel4 As Panel
    Friend WithEvents Label2 As Label
    Friend WithEvents Panel2 As Panel
    Friend WithEvents ChartPowerFactor As DataVisualization.Charting.Chart
    Friend WithEvents Panel3 As Panel
    Friend WithEvents Label1 As Label
    Friend WithEvents ChartControl1 As DevComponents.DotNetBar.Charts.ChartControl
    Friend WithEvents GrpChartPowerFactor As GroupBox
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents ButtonRecord As Button
    Friend WithEvents CmdReadData As Button
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents Panel7 As Panel
    Friend WithEvents ChartPowerFactorRec As DataVisualization.Charting.Chart
    Friend WithEvents Panel8 As Panel
    Friend WithEvents PictureBoxRecordInd As PictureBox
    Friend WithEvents Label4 As Label
    Friend WithEvents LblRecording As Label
    Friend WithEvents Panel5 As Panel
    Friend WithEvents CmdClearRecording As Button
    Friend WithEvents CmdExportToExcel As Button
    Friend WithEvents DataGridViewPowerFactor As DataGridView
    Friend WithEvents Timer As DataGridViewTextBoxColumn
    Friend WithEvents PF1 As DataGridViewTextBoxColumn
    Friend WithEvents PF2 As DataGridViewTextBoxColumn
    Friend WithEvents PF3 As DataGridViewTextBoxColumn
    Friend WithEvents PFtot As DataGridViewTextBoxColumn
End Class
