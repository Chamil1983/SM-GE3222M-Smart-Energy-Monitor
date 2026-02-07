<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_BoardData
    Inherits System.Windows.Forms.Form
    'Inherits DevComponents.DotNetBar.Office2007Form
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
        Dim GaugeCircularScale1 As DevComponents.Instrumentation.GaugeCircularScale = New DevComponents.Instrumentation.GaugeCircularScale()
        Dim GradientFillColor1 As DevComponents.Instrumentation.GradientFillColor = New DevComponents.Instrumentation.GradientFillColor()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(FrmSMGE_3222M_BoardData))
        Dim GaugePointer1 As DevComponents.Instrumentation.GaugePointer = New DevComponents.Instrumentation.GaugePointer()
        Dim GaugeRange1 As DevComponents.Instrumentation.GaugeRange = New DevComponents.Instrumentation.GaugeRange()
        Dim GradientFillColor2 As DevComponents.Instrumentation.GradientFillColor = New DevComponents.Instrumentation.GradientFillColor()
        Dim GradientFillColor3 As DevComponents.Instrumentation.GradientFillColor = New DevComponents.Instrumentation.GradientFillColor()
        Dim GradientFillColor4 As DevComponents.Instrumentation.GradientFillColor = New DevComponents.Instrumentation.GradientFillColor()
        Dim GradientFillColor5 As DevComponents.Instrumentation.GradientFillColor = New DevComponents.Instrumentation.GradientFillColor()
        Dim GaugeText1 As DevComponents.Instrumentation.GaugeText = New DevComponents.Instrumentation.GaugeText()
        Dim GaugeText2 As DevComponents.Instrumentation.GaugeText = New DevComponents.Instrumentation.GaugeText()
        Dim GaugeLinearScale1 As DevComponents.Instrumentation.GaugeLinearScale = New DevComponents.Instrumentation.GaugeLinearScale()
        Dim GaugePointer2 As DevComponents.Instrumentation.GaugePointer = New DevComponents.Instrumentation.GaugePointer()
        Dim GaugeSection1 As DevComponents.Instrumentation.GaugeSection = New DevComponents.Instrumentation.GaugeSection()
        Dim GaugeLinearScale2 As DevComponents.Instrumentation.GaugeLinearScale = New DevComponents.Instrumentation.GaugeLinearScale()
        Dim ChartArea2 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim ChartArea3 As System.Windows.Forms.DataVisualization.Charting.ChartArea = New System.Windows.Forms.DataVisualization.Charting.ChartArea()
        Dim Legend2 As System.Windows.Forms.DataVisualization.Charting.Legend = New System.Windows.Forms.DataVisualization.Charting.Legend()
        Dim Series3 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Series4 As System.Windows.Forms.DataVisualization.Charting.Series = New System.Windows.Forms.DataVisualization.Charting.Series()
        Dim Background1 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background2 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background3 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background4 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background5 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background6 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background7 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Dim Background8 As DevComponents.DotNetBar.Charts.Style.Background = New DevComponents.DotNetBar.Charts.Style.Background()
        Me.GrpChartHumidity = New System.Windows.Forms.GroupBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.ButtonRecord = New System.Windows.Forms.Button()
        Me.CmdReadData = New System.Windows.Forms.Button()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.Panel7 = New System.Windows.Forms.Panel()
        Me.ChartHumidityRec = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel8 = New System.Windows.Forms.Panel()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.LblRecording = New System.Windows.Forms.Label()
        Me.Panel5 = New System.Windows.Forms.Panel()
        Me.CmdClearRecording = New System.Windows.Forms.Button()
        Me.CmdExportToExcel = New System.Windows.Forms.Button()
        Me.DataGridViewHumidity = New System.Windows.Forms.DataGridView()
        Me.Timer = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Humidity = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Temperature_C = New System.Windows.Forms.DataGridViewTextBoxColumn()
        Me.Panel6 = New System.Windows.Forms.Panel()
        Me.LblTime = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.LblTempCVal = New System.Windows.Forms.Label()
        Me.LblTempC = New System.Windows.Forms.Label()
        Me.GCHumidity = New DevComponents.Instrumentation.GaugeControl()
        Me.GCTempC = New DevComponents.Instrumentation.GaugeControl()
        Me.Panel4 = New System.Windows.Forms.Panel()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.LblHumidityVal = New System.Windows.Forms.Label()
        Me.LblHumidity = New System.Windows.Forms.Label()
        Me.Panel2 = New System.Windows.Forms.Panel()
        Me.ChartHumTemp = New System.Windows.Forms.DataVisualization.Charting.Chart()
        Me.Panel3 = New System.Windows.Forms.Panel()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.ChartControl1 = New DevComponents.DotNetBar.Charts.ChartControl()
        Me.TimerGaugeCom = New System.Windows.Forms.Timer(Me.components)
        Me.ProgressBarSave = New System.Windows.Forms.ProgressBar()
        Me.PictureBoxRecordInd = New System.Windows.Forms.PictureBox()
        Me.GrpChartHumidity.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.Panel7.SuspendLayout()
        CType(Me.ChartHumidityRec, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel8.SuspendLayout()
        Me.Panel5.SuspendLayout()
        CType(Me.DataGridViewHumidity, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel6.SuspendLayout()
        Me.Panel1.SuspendLayout()
        CType(Me.GCHumidity, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.GCTempC, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel4.SuspendLayout()
        Me.Panel2.SuspendLayout()
        CType(Me.ChartHumTemp, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel3.SuspendLayout()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'GrpChartHumidity
        '
        Me.GrpChartHumidity.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpChartHumidity.Controls.Add(Me.GroupBox1)
        Me.GrpChartHumidity.Controls.Add(Me.Panel7)
        Me.GrpChartHumidity.Controls.Add(Me.Panel5)
        Me.GrpChartHumidity.Controls.Add(Me.Panel1)
        Me.GrpChartHumidity.Controls.Add(Me.Panel2)
        Me.GrpChartHumidity.Controls.Add(Me.ChartControl1)
        Me.GrpChartHumidity.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpChartHumidity.Location = New System.Drawing.Point(8, 12)
        Me.GrpChartHumidity.Name = "GrpChartHumidity"
        Me.GrpChartHumidity.Size = New System.Drawing.Size(902, 767)
        Me.GrpChartHumidity.TabIndex = 0
        Me.GrpChartHumidity.TabStop = False
        Me.GrpChartHumidity.Text = "Graph Humidity (Real-Time/Sec)"
        '
        'GroupBox1
        '
        Me.GroupBox1.Anchor = System.Windows.Forms.AnchorStyles.Bottom
        Me.GroupBox1.Controls.Add(Me.ButtonRecord)
        Me.GroupBox1.Controls.Add(Me.CmdReadData)
        Me.GroupBox1.Controls.Add(Me.CmdBack)
        Me.GroupBox1.Controls.Add(Me.CmdCancel)
        Me.GroupBox1.Location = New System.Drawing.Point(6, 688)
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
        Me.ButtonRecord.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
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
        Me.CmdReadData.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
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
        Me.Panel7.Controls.Add(Me.ChartHumidityRec)
        Me.Panel7.Controls.Add(Me.Panel8)
        Me.Panel7.Location = New System.Drawing.Point(6, 331)
        Me.Panel7.Name = "Panel7"
        Me.Panel7.Size = New System.Drawing.Size(569, 349)
        Me.Panel7.TabIndex = 6
        '
        'ChartHumidityRec
        '
        Me.ChartHumidityRec.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartHumidityRec.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartHumidityRec.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea1.AxisX.Interval = 1.0R
        ChartArea1.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea1.AxisX.IsLabelAutoFit = False
        ChartArea1.AxisX.LabelStyle.Angle = -45
        ChartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea1.AxisX.Minimum = 0R
        ChartArea1.AxisY.Interval = 20.0R
        ChartArea1.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea1.AxisY.Maximum = 100.0R
        ChartArea1.AxisY.Minimum = 0R
        ChartArea1.Name = "ChartArea1"
        Me.ChartHumidityRec.ChartAreas.Add(ChartArea1)
        Legend1.Alignment = System.Drawing.StringAlignment.Center
        Legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend1.Name = "Legend1"
        Me.ChartHumidityRec.Legends.Add(Legend1)
        Me.ChartHumidityRec.Location = New System.Drawing.Point(7, 37)
        Me.ChartHumidityRec.Name = "ChartHumidityRec"
        Series1.ChartArea = "ChartArea1"
        Series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series1.Color = System.Drawing.Color.Green
        Series1.IsValueShownAsLabel = True
        Series1.LabelForeColor = System.Drawing.Color.Green
        Series1.Legend = "Legend1"
        Series1.LegendText = "Humidity %"
        Series1.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Diamond
        Series1.Name = "Series1"
        Series2.ChartArea = "ChartArea1"
        Series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line
        Series2.Color = System.Drawing.Color.FromArgb(CType(CType(255, Byte), Integer), CType(CType(128, Byte), Integer), CType(CType(128, Byte), Integer))
        Series2.IsValueShownAsLabel = True
        Series2.Legend = "Legend1"
        Series2.LegendText = "Temperature °C"
        Series2.MarkerStyle = System.Windows.Forms.DataVisualization.Charting.MarkerStyle.Circle
        Series2.Name = "Series2"
        Me.ChartHumidityRec.Series.Add(Series1)
        Me.ChartHumidityRec.Series.Add(Series2)
        Me.ChartHumidityRec.Size = New System.Drawing.Size(556, 300)
        Me.ChartHumidityRec.TabIndex = 2
        Me.ChartHumidityRec.Text = "Chart1"
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
        Me.Panel8.Size = New System.Drawing.Size(569, 28)
        Me.Panel8.TabIndex = 1
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.ForeColor = System.Drawing.Color.White
        Me.Label4.Location = New System.Drawing.Point(7, 5)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(238, 18)
        Me.Label4.TabIndex = 0
        Me.Label4.Text = "Record Graph (Humidity)"
        '
        'LblRecording
        '
        Me.LblRecording.Anchor = System.Windows.Forms.AnchorStyles.Right
        Me.LblRecording.AutoSize = True
        Me.LblRecording.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblRecording.ForeColor = System.Drawing.Color.White
        Me.LblRecording.Location = New System.Drawing.Point(425, 5)
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
        Me.Panel5.Controls.Add(Me.DataGridViewHumidity)
        Me.Panel5.Controls.Add(Me.Panel6)
        Me.Panel5.Location = New System.Drawing.Point(583, 334)
        Me.Panel5.Name = "Panel5"
        Me.Panel5.Size = New System.Drawing.Size(308, 427)
        Me.Panel5.TabIndex = 1
        '
        'CmdClearRecording
        '
        Me.CmdClearRecording.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.CmdClearRecording.Font = New System.Drawing.Font("Courier New", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdClearRecording.Location = New System.Drawing.Point(160, 40)
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
        Me.CmdExportToExcel.Location = New System.Drawing.Point(6, 40)
        Me.CmdExportToExcel.Name = "CmdExportToExcel"
        Me.CmdExportToExcel.Size = New System.Drawing.Size(148, 30)
        Me.CmdExportToExcel.TabIndex = 4
        Me.CmdExportToExcel.Text = "Export To Excel"
        Me.CmdExportToExcel.UseVisualStyleBackColor = True
        '
        'DataGridViewHumidity
        '
        Me.DataGridViewHumidity.AllowUserToAddRows = False
        Me.DataGridViewHumidity.AllowUserToDeleteRows = False
        Me.DataGridViewHumidity.AllowUserToResizeRows = False
        Me.DataGridViewHumidity.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.DataGridViewHumidity.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill
        Me.DataGridViewHumidity.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize
        Me.DataGridViewHumidity.Columns.AddRange(New System.Windows.Forms.DataGridViewColumn() {Me.Timer, Me.Humidity, Me.Temperature_C})
        Me.DataGridViewHumidity.Location = New System.Drawing.Point(6, 76)
        Me.DataGridViewHumidity.Name = "DataGridViewHumidity"
        Me.DataGridViewHumidity.RowHeadersVisible = False
        Me.DataGridViewHumidity.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect
        Me.DataGridViewHumidity.Size = New System.Drawing.Size(302, 348)
        Me.DataGridViewHumidity.TabIndex = 3
        '
        'Timer
        '
        Me.Timer.HeaderText = "Timer"
        Me.Timer.Name = "Timer"
        '
        'Humidity
        '
        Me.Humidity.HeaderText = "Humidity (%)"
        Me.Humidity.Name = "Humidity"
        '
        'Temperature_C
        '
        Me.Temperature_C.HeaderText = "Temperature (°C)"
        Me.Temperature_C.Name = "Temperature_C"
        '
        'Panel6
        '
        Me.Panel6.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel6.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel6.Controls.Add(Me.LblTime)
        Me.Panel6.Controls.Add(Me.Label3)
        Me.Panel6.Location = New System.Drawing.Point(0, 0)
        Me.Panel6.Name = "Panel6"
        Me.Panel6.Size = New System.Drawing.Size(308, 28)
        Me.Panel6.TabIndex = 1
        '
        'LblTime
        '
        Me.LblTime.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.LblTime.AutoSize = True
        Me.LblTime.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblTime.ForeColor = System.Drawing.Color.White
        Me.LblTime.Location = New System.Drawing.Point(177, 5)
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
        Me.Panel1.Controls.Add(Me.LblTempCVal)
        Me.Panel1.Controls.Add(Me.LblTempC)
        Me.Panel1.Controls.Add(Me.GCHumidity)
        Me.Panel1.Controls.Add(Me.GCTempC)
        Me.Panel1.Controls.Add(Me.Panel4)
        Me.Panel1.Controls.Add(Me.LblHumidityVal)
        Me.Panel1.Controls.Add(Me.LblHumidity)
        Me.Panel1.Location = New System.Drawing.Point(583, 25)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(308, 300)
        Me.Panel1.TabIndex = 2
        '
        'LblTempCVal
        '
        Me.LblTempCVal.AutoSize = True
        Me.LblTempCVal.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblTempCVal.Location = New System.Drawing.Point(142, 269)
        Me.LblTempCVal.Name = "LblTempCVal"
        Me.LblTempCVal.Size = New System.Drawing.Size(108, 18)
        Me.LblTempCVal.TabIndex = 7
        Me.LblTempCVal.Text = "Waiting..."
        '
        'LblTempC
        '
        Me.LblTempC.AutoSize = True
        Me.LblTempC.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblTempC.Location = New System.Drawing.Point(46, 269)
        Me.LblTempC.Name = "LblTempC"
        Me.LblTempC.Size = New System.Drawing.Size(78, 18)
        Me.LblTempC.TabIndex = 6
        Me.LblTempC.Text = "Temp °C"
        '
        'GCHumidity
        '
        GaugeCircularScale1.Labels.Layout.Font = New System.Drawing.Font("Microsoft Sans Serif", 4.0!)
        GaugeCircularScale1.Labels.Layout.ScaleOffset = 0.075!
        GaugeCircularScale1.MajorTickMarks.Interval = 50.0R
        GaugeCircularScale1.MajorTickMarks.Layout.Length = 0.063!
        GaugeCircularScale1.MajorTickMarks.Layout.Width = 0.033!
        GaugeCircularScale1.MaxPin.EndOffset = 0.01!
        GaugeCircularScale1.MaxPin.Length = 0.026!
        GaugeCircularScale1.MaxPin.Name = "MaxPin"
        GaugeCircularScale1.MaxPin.ScaleOffset = -0.06!
        GaugeCircularScale1.MaxPin.Width = 0.026!
        GaugeCircularScale1.MinorTickMarks.Interval = 10.0R
        GradientFillColor1.BorderColor = System.Drawing.Color.DimGray
        GradientFillColor1.BorderWidth = 1
        GradientFillColor1.Color1 = System.Drawing.Color.LightYellow
        GaugeCircularScale1.MinorTickMarks.Layout.FillColor = GradientFillColor1
        GaugeCircularScale1.MinorTickMarks.Layout.Length = 0.042!
        GaugeCircularScale1.MinorTickMarks.Layout.Width = 0.013!
        GaugeCircularScale1.MinPin.EndOffset = 0.01!
        GaugeCircularScale1.MinPin.Length = 0.026!
        GaugeCircularScale1.MinPin.Name = "MinPin"
        GaugeCircularScale1.MinPin.ScaleOffset = -0.06!
        GaugeCircularScale1.MinPin.Width = 0.026!
        GaugeCircularScale1.Name = "Scale1"
        GaugeCircularScale1.PivotPoint = CType(resources.GetObject("GaugeCircularScale1.PivotPoint"), System.Drawing.PointF)
        GaugePointer1.CapFillColor.BorderColor = System.Drawing.Color.DimGray
        GaugePointer1.CapFillColor.BorderWidth = 1
        GaugePointer1.CapFillColor.Color1 = System.Drawing.Color.Gainsboro
        GaugePointer1.CapFillColor.Color2 = System.Drawing.Color.Silver
        GaugePointer1.FillColor.BorderColor = System.Drawing.Color.DimGray
        GaugePointer1.FillColor.BorderWidth = 1
        GaugePointer1.FillColor.Color1 = System.Drawing.Color.Black
        GaugePointer1.Name = "Pointer1"
        GaugePointer1.ScaleOffset = -0.057!
        GaugePointer1.Style = DevComponents.Instrumentation.PointerStyle.Needle
        GaugePointer1.Width = 0.032!
        GaugeCircularScale1.Pointers.AddRange(New DevComponents.Instrumentation.GaugePointer() {GaugePointer1})
        GaugeCircularScale1.Radius = 2.5!
        GaugeRange1.EndValue = 100.0R
        GaugeRange1.EndWidth = 0.07!
        GaugeRange1.FillColor.BorderColor = System.Drawing.Color.Gray
        GaugeRange1.FillColor.BorderWidth = 1
        GaugeRange1.FillColor.Color1 = System.Drawing.Color.Yellow
        GaugeRange1.FillColor.Color2 = System.Drawing.Color.Lime
        GaugeRange1.Name = "Range1"
        GaugeRange1.ScaleOffset = 0.033!
        GaugeRange1.StartValue = 0R
        GaugeRange1.StartWidth = 0.07!
        GaugeCircularScale1.Ranges.AddRange(New DevComponents.Instrumentation.GaugeRange() {GaugeRange1})
        GaugeCircularScale1.StartAngle = 240.0!
        GaugeCircularScale1.SweepAngle = 60.0!
        GaugeCircularScale1.Width = 0.011!
        Me.GCHumidity.CircularScales.AddRange(New DevComponents.Instrumentation.GaugeCircularScale() {GaugeCircularScale1})
        GradientFillColor2.Color1 = System.Drawing.Color.Gainsboro
        GradientFillColor2.Color2 = System.Drawing.Color.DarkGray
        Me.GCHumidity.Frame.BackColor = GradientFillColor2
        GradientFillColor3.BorderColor = System.Drawing.Color.Gainsboro
        GradientFillColor3.BorderWidth = 1
        GradientFillColor3.Color1 = System.Drawing.Color.White
        GradientFillColor3.Color2 = System.Drawing.Color.DimGray
        Me.GCHumidity.Frame.FrameColor = GradientFillColor3
        Me.GCHumidity.Frame.Style = DevComponents.Instrumentation.GaugeFrameStyle.Rectangular
        Me.GCHumidity.Location = New System.Drawing.Point(29, 34)
        Me.GCHumidity.Name = "GCHumidity"
        Me.GCHumidity.Size = New System.Drawing.Size(250, 83)
        Me.GCHumidity.TabIndex = 9
        Me.GCHumidity.Text = "GaugeControl2"
        '
        'GCTempC
        '
        GradientFillColor4.Color1 = System.Drawing.Color.Gainsboro
        GradientFillColor4.Color2 = System.Drawing.Color.DarkGray
        Me.GCTempC.Frame.BackColor = GradientFillColor4
        GradientFillColor5.BorderColor = System.Drawing.Color.Gainsboro
        GradientFillColor5.BorderWidth = 1
        GradientFillColor5.Color1 = System.Drawing.Color.White
        GradientFillColor5.Color2 = System.Drawing.Color.DimGray
        Me.GCTempC.Frame.FrameColor = GradientFillColor5
        Me.GCTempC.Frame.Style = DevComponents.Instrumentation.GaugeFrameStyle.Rectangular
        GaugeText1.BackColor.BorderColor = System.Drawing.Color.Black
        GaugeText1.Location = CType(resources.GetObject("GaugeText1.Location"), System.Drawing.PointF)
        GaugeText1.Name = "Text1"
        GaugeText1.Text = "C°"
        GaugeText2.BackColor.BorderColor = System.Drawing.Color.Black
        GaugeText2.Location = CType(resources.GetObject("GaugeText2.Location"), System.Drawing.PointF)
        GaugeText2.Name = "Text2"
        GaugeText2.Text = "F°"
        Me.GCTempC.GaugeItems.AddRange(New DevComponents.Instrumentation.GaugeItem() {GaugeText1, GaugeText2})
        GaugeLinearScale1.Labels.FormatString = "0°"
        GaugeLinearScale1.Location = CType(resources.GetObject("GaugeLinearScale1.Location"), System.Drawing.PointF)
        GaugeLinearScale1.MajorTickMarks.Layout.Placement = DevComponents.Instrumentation.DisplayPlacement.Near
        GaugeLinearScale1.MajorTickMarks.Layout.Style = DevComponents.Instrumentation.GaugeMarkerStyle.Rectangle
        GaugeLinearScale1.MajorTickMarks.Layout.Width = 0.008!
        GaugeLinearScale1.MaxPin.Name = "MaxPin"
        GaugeLinearScale1.MaxPin.Visible = False
        GaugeLinearScale1.MaxValue = 40.0R
        GaugeLinearScale1.MinorTickMarks.Layout.Placement = DevComponents.Instrumentation.DisplayPlacement.Near
        GaugeLinearScale1.MinorTickMarks.Layout.Width = 0.016!
        GaugeLinearScale1.MinPin.Name = "MinPin"
        GaugeLinearScale1.MinPin.Visible = False
        GaugeLinearScale1.MinValue = -30.0R
        GaugeLinearScale1.Name = "Scale1"
        GaugePointer2.BulbOffset = 0.026!
        GaugePointer2.BulbSize = 0.132!
        GaugePointer2.CapFillColor.BorderColor = System.Drawing.Color.DimGray
        GaugePointer2.CapFillColor.BorderWidth = 1
        GaugePointer2.CapFillColor.Color1 = System.Drawing.Color.WhiteSmoke
        GaugePointer2.CapFillColor.Color2 = System.Drawing.Color.DimGray
        GaugePointer2.FillColor.Color1 = System.Drawing.Color.Red
        GaugePointer2.Name = "Pointer1"
        GaugePointer2.Style = DevComponents.Instrumentation.PointerStyle.Thermometer
        GaugePointer2.ThermoBackColor.Color1 = System.Drawing.Color.FromArgb(CType(CType(100, Byte), Integer), CType(CType(60, Byte), Integer), CType(CType(60, Byte), Integer), CType(CType(60, Byte), Integer))
        GaugePointer2.Value = 12.0R
        GaugePointer2.Width = 0.1!
        GaugeLinearScale1.Pointers.AddRange(New DevComponents.Instrumentation.GaugePointer() {GaugePointer2})
        GaugeSection1.FillColor.Color1 = System.Drawing.Color.SteelBlue
        GaugeSection1.FillColor.Color2 = System.Drawing.Color.LightCyan
        GaugeSection1.FillColor.GradientFillType = DevComponents.Instrumentation.GradientFillType.HorizontalCenter
        GaugeSection1.Name = "Section1"
        GaugeLinearScale1.Sections.AddRange(New DevComponents.Instrumentation.GaugeSection() {GaugeSection1})
        GaugeLinearScale1.Width = 0.1!
        GaugeLinearScale2.Labels.FormatString = "0°"
        GaugeLinearScale2.Labels.Layout.Placement = DevComponents.Instrumentation.DisplayPlacement.Far
        GaugeLinearScale2.Labels.ShowMaxLabel = False
        GaugeLinearScale2.Labels.ShowMinLabel = False
        GaugeLinearScale2.Location = CType(resources.GetObject("GaugeLinearScale2.Location"), System.Drawing.PointF)
        GaugeLinearScale2.MajorTickMarks.Interval = 20.0R
        GaugeLinearScale2.MajorTickMarks.IntervalOffset = 2.0R
        GaugeLinearScale2.MajorTickMarks.Layout.Placement = DevComponents.Instrumentation.DisplayPlacement.Far
        GaugeLinearScale2.MajorTickMarks.Layout.Style = DevComponents.Instrumentation.GaugeMarkerStyle.Rectangle
        GaugeLinearScale2.MajorTickMarks.Layout.Width = 0.008!
        GaugeLinearScale2.MaxPin.Name = "MaxPin"
        GaugeLinearScale2.MaxPin.Visible = False
        GaugeLinearScale2.MaxValue = 104.0R
        GaugeLinearScale2.MinorTickMarks.Interval = 4.0R
        GaugeLinearScale2.MinorTickMarks.IntervalOffset = 2.0R
        GaugeLinearScale2.MinorTickMarks.Layout.Placement = DevComponents.Instrumentation.DisplayPlacement.Far
        GaugeLinearScale2.MinorTickMarks.Layout.Width = 0.016!
        GaugeLinearScale2.MinPin.Name = "MinPin"
        GaugeLinearScale2.MinPin.Visible = False
        GaugeLinearScale2.MinValue = -22.0R
        GaugeLinearScale2.Name = "Scale2"
        GaugeLinearScale2.Width = 0.1!
        Me.GCTempC.LinearScales.AddRange(New DevComponents.Instrumentation.GaugeLinearScale() {GaugeLinearScale1, GaugeLinearScale2})
        Me.GCTempC.Location = New System.Drawing.Point(29, 170)
        Me.GCTempC.Name = "GCTempC"
        Me.GCTempC.Size = New System.Drawing.Size(250, 83)
        Me.GCTempC.TabIndex = 8
        Me.GCTempC.Text = "GaugeControl1"
        '
        'Panel4
        '
        Me.Panel4.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel4.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel4.Controls.Add(Me.Label2)
        Me.Panel4.Location = New System.Drawing.Point(0, 0)
        Me.Panel4.Name = "Panel4"
        Me.Panel4.Size = New System.Drawing.Size(308, 28)
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
        Me.Label2.Text = "Humidity (Real-Time/Sec)"
        '
        'LblHumidityVal
        '
        Me.LblHumidityVal.AutoSize = True
        Me.LblHumidityVal.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblHumidityVal.Location = New System.Drawing.Point(142, 137)
        Me.LblHumidityVal.Name = "LblHumidityVal"
        Me.LblHumidityVal.Size = New System.Drawing.Size(108, 18)
        Me.LblHumidityVal.TabIndex = 5
        Me.LblHumidityVal.Text = "Waiting..."
        '
        'LblHumidity
        '
        Me.LblHumidity.AutoSize = True
        Me.LblHumidity.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.LblHumidity.Location = New System.Drawing.Point(46, 137)
        Me.LblHumidity.Name = "LblHumidity"
        Me.LblHumidity.Size = New System.Drawing.Size(58, 18)
        Me.LblHumidity.TabIndex = 4
        Me.LblHumidity.Text = "Hum %"
        '
        'Panel2
        '
        Me.Panel2.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel2.BackColor = System.Drawing.Color.White
        Me.Panel2.Controls.Add(Me.ChartHumTemp)
        Me.Panel2.Controls.Add(Me.Panel3)
        Me.Panel2.Location = New System.Drawing.Point(6, 25)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(570, 300)
        Me.Panel2.TabIndex = 3
        '
        'ChartHumTemp
        '
        Me.ChartHumTemp.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.ChartHumTemp.AntiAliasing = System.Windows.Forms.DataVisualization.Charting.AntiAliasingStyles.Text
        Me.ChartHumTemp.BorderlineColor = System.Drawing.Color.RoyalBlue
        Me.ChartHumTemp.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid
        ChartArea2.AxisX.Interval = 2.0R
        ChartArea2.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisX.LabelStyle.Enabled = False
        ChartArea2.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea2.AxisX.Minimum = 0R
        ChartArea2.AxisY.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea2.AxisY.Maximum = 100.0R
        ChartArea2.AxisY.Minimum = 0R
        ChartArea2.AxisY.Title = "Humidity"
        ChartArea2.AxisY2.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.[False]
        ChartArea2.AxisY2.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea2.AxisY2.Maximum = 50.0R
        ChartArea2.AxisY2.Minimum = 0R
        ChartArea2.AxisY2.Title = "Temperature"
        ChartArea2.Name = "ChartArea1"
        ChartArea3.AxisX.Interval = 2.0R
        ChartArea3.AxisX.IntervalAutoMode = System.Windows.Forms.DataVisualization.Charting.IntervalAutoMode.VariableCount
        ChartArea3.AxisX.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea3.AxisX.Minimum = 0R
        ChartArea3.AxisX.TitleFont = New System.Drawing.Font("Microsoft Sans Serif", 6.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ChartArea3.AxisY.MajorGrid.LineColor = System.Drawing.Color.WhiteSmoke
        ChartArea3.AxisY.Minimum = 0R
        ChartArea3.AxisY.Title = "Temperature (°C)"
        ChartArea3.Name = "ChartArea2"
        Me.ChartHumTemp.ChartAreas.Add(ChartArea2)
        Me.ChartHumTemp.ChartAreas.Add(ChartArea3)
        Legend2.Alignment = System.Drawing.StringAlignment.Center
        Legend2.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom
        Legend2.Name = "Legend1"
        Legend2.TextWrapThreshold = 40
        Me.ChartHumTemp.Legends.Add(Legend2)
        Me.ChartHumTemp.Location = New System.Drawing.Point(10, 32)
        Me.ChartHumTemp.Name = "ChartHumTemp"
        Series3.BorderWidth = 3
        Series3.ChartArea = "ChartArea1"
        Series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline
        Series3.Color = System.Drawing.Color.CornflowerBlue
        Series3.Legend = "Legend1"
        Series3.LegendText = "Real Time Humidity (%)"
        Series3.Name = "Humidity"
        Series4.ChartArea = "ChartArea2"
        Series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine
        Series4.Color = System.Drawing.Color.FromArgb(CType(CType(255, Byte), Integer), CType(CType(128, Byte), Integer), CType(CType(128, Byte), Integer))
        Series4.Legend = "Legend1"
        Series4.LegendText = "Real Time Temperature (°C)"
        Series4.Name = "Temp_C"
        Me.ChartHumTemp.Series.Add(Series3)
        Me.ChartHumTemp.Series.Add(Series4)
        Me.ChartHumTemp.Size = New System.Drawing.Size(550, 260)
        Me.ChartHumTemp.TabIndex = 2
        Me.ChartHumTemp.Text = "Chart1"
        '
        'Panel3
        '
        Me.Panel3.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Panel3.BackColor = System.Drawing.Color.FromArgb(CType(CType(2, Byte), Integer), CType(CType(120, Byte), Integer), CType(CType(174, Byte), Integer))
        Me.Panel3.Controls.Add(Me.Label1)
        Me.Panel3.Location = New System.Drawing.Point(0, 0)
        Me.Panel3.Name = "Panel3"
        Me.Panel3.Size = New System.Drawing.Size(570, 28)
        Me.Panel3.TabIndex = 0
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Courier New", 12.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.ForeColor = System.Drawing.Color.White
        Me.Label1.Location = New System.Drawing.Point(7, 5)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(308, 18)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Graph Humidity (Real-Time/Sec)"
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
        Me.ChartControl1.Size = New System.Drawing.Size(893, 739)
        Me.ChartControl1.TabIndex = 0
        Me.ChartControl1.Text = "ChartControl1"
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
        Me.ProgressBarSave.Location = New System.Drawing.Point(8, 779)
        Me.ProgressBarSave.Maximum = 10
        Me.ProgressBarSave.Name = "ProgressBarSave"
        Me.ProgressBarSave.Size = New System.Drawing.Size(902, 23)
        Me.ProgressBarSave.TabIndex = 2
        Me.ProgressBarSave.Visible = False
        '
        'PictureBoxRecordInd
        '
        Me.PictureBoxRecordInd.Anchor = System.Windows.Forms.AnchorStyles.Right
        Me.PictureBoxRecordInd.Image = Global.Meter_GE3222M_V1._0.My.Resources.Resources.Red
        Me.PictureBoxRecordInd.Location = New System.Drawing.Point(528, 3)
        Me.PictureBoxRecordInd.Name = "PictureBoxRecordInd"
        Me.PictureBoxRecordInd.Size = New System.Drawing.Size(20, 20)
        Me.PictureBoxRecordInd.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.PictureBoxRecordInd.TabIndex = 6
        Me.PictureBoxRecordInd.TabStop = False
        '
        'FrmSMGE_3222M_BoardData
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(919, 800)
        Me.Controls.Add(Me.ProgressBarSave)
        Me.Controls.Add(Me.GrpChartHumidity)
        Me.DoubleBuffered = True
        Me.Name = "FrmSMGE_3222M_BoardData"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE3222M Board Data"
        Me.GrpChartHumidity.ResumeLayout(False)
        Me.GroupBox1.ResumeLayout(False)
        Me.Panel7.ResumeLayout(False)
        CType(Me.ChartHumidityRec, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel8.ResumeLayout(False)
        Me.Panel8.PerformLayout()
        Me.Panel5.ResumeLayout(False)
        CType(Me.DataGridViewHumidity, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel6.ResumeLayout(False)
        Me.Panel6.PerformLayout()
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        CType(Me.GCHumidity, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.GCTempC, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel4.ResumeLayout(False)
        Me.Panel4.PerformLayout()
        Me.Panel2.ResumeLayout(False)
        CType(Me.ChartHumTemp, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel3.ResumeLayout(False)
        Me.Panel3.PerformLayout()
        CType(Me.PictureBoxRecordInd, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents GrpChartHumidity As GroupBox
    Friend WithEvents ChartControl1 As DevComponents.DotNetBar.Charts.ChartControl
    Friend WithEvents Panel1 As Panel
    Friend WithEvents LblHumidity As Label
    Friend WithEvents LblHumidityVal As Label
    Friend WithEvents TimerGaugeCom As Timer
    Friend WithEvents Panel2 As Panel
    Friend WithEvents Panel3 As Panel
    Friend WithEvents Label1 As Label
    Friend WithEvents ChartHumTemp As DataVisualization.Charting.Chart
    Friend WithEvents Panel4 As Panel
    Friend WithEvents Label2 As Label
    Friend WithEvents Panel5 As Panel
    Friend WithEvents Panel6 As Panel
    Friend WithEvents Label3 As Label
    Friend WithEvents LblTime As Label
    Friend WithEvents ProgressBarSave As ProgressBar
    Friend WithEvents CmdExportToExcel As Button
    Friend WithEvents DataGridViewHumidity As DataGridView
    Friend WithEvents CmdClearRecording As Button
    Friend WithEvents Timer As DataGridViewTextBoxColumn
    Friend WithEvents Humidity As DataGridViewTextBoxColumn
    Friend WithEvents Panel7 As Panel
    Friend WithEvents Panel8 As Panel
    Friend WithEvents Label4 As Label
    Friend WithEvents ChartHumidityRec As DataVisualization.Charting.Chart
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents CmdReadData As Button
    Friend WithEvents ButtonRecord As Button
    Friend WithEvents PictureBoxRecordInd As PictureBox
    Friend WithEvents LblRecording As Label
    Friend WithEvents GCTempC As DevComponents.Instrumentation.GaugeControl
    Friend WithEvents GCHumidity As DevComponents.Instrumentation.GaugeControl
    Friend WithEvents LblTempCVal As Label
    Friend WithEvents LblTempC As Label
    Friend WithEvents Temperature_C As DataGridViewTextBoxColumn
End Class
