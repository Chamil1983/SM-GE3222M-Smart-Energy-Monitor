Imports System.Linq
Imports System.Data.SqlClient
Imports System.Data.OleDb
Imports Microsoft.Office.Core
Imports Excel = Microsoft.Office.Interop.Excel
Imports ExcelAutoFormat = Microsoft.Office.Interop.Excel.XlRangeAutoFormat
Imports Microsoft.Office.Interop
Imports System.IO
Imports System.Xml.XPath
Imports System.Data
Imports System.Xml
Imports System.IO.Ports
Imports DevComponents.DotNetBar.Controls
Imports System.Reflection
Imports System.Security.Cryptography
Imports DevComponents.DotNetBar




Public Class FrmSMGE_3222M_BoardData
    Dim Limit As Integer = 10
    Dim value1 As Decimal
    Dim value2 As Decimal

    'Dim RecordCheck As Boolean = False

    Public SR As Boolean = False

    Dim DataSerIn As String '-> Variable to hold incoming serial data

    Private stopwatch As New Diagnostics.Stopwatch '-> Stopwatch declaration used as a timer
    Dim _sec, _min As String '-> Variable to hold the minutes and seconds of the stopwatch
    Private Sub FrmSMGE_3222M_Humidity_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        For i = 0 To 30 Step 1
            ChartHumTemp.Series("Humidity").Points.AddXY(DateTime.Now.ToLongTimeString, 0)
            If ChartHumTemp.Series(0).Points.Count = Limit Then
                ChartHumTemp.Series(0).Points.RemoveAt(0)
            End If
            ChartHumTemp.ChartAreas(0).AxisY.Maximum = 100


        Next
        ChartHumTemp.ChartAreas(0).AxisY.Maximum = 100

        For i = 0 To 15
            ChartHumidityRec.Series("Series1").Points.AddXY("00:00", 0)
        Next


        DoubleBuffer.DoubleBuffered(DataGridViewHumidity, True) '-> To speed up DataGridView performance




    End Sub

    Private Sub TimerGaugeCom_Tick(sender As Object, e As EventArgs) Handles TimerGaugeCom.Tick


        Dim TB1, TB2, TB3, TB4 As New TextBox
        TB1.Multiline = True
        TB2.Multiline = False
        TB3.Multiline = False
        TB4.Multiline = False

        's = TextBox1.Text + "," + "," + "," + ","

        ' Dim somestring() As String
        ' Split string based on comma
        'somestring = s.Split(New Char() {","c})
        'TB2.Text = somestring(0)



        'TB3.Text = somestring(1)

        Try
            value1 = Convert.ToDecimal(LblHumidityVal.Text)
            value2 = Convert.ToDecimal(LblTempCVal.Text)

        Catch ex As Exception

        End Try

        'TB4.Text = somestring(2)

        GCHumidity.SetPointerValue("Scale1", "Pointer1", value1)
        GCTempC.SetPointerValue("Scale1", "Pointer1", value2)


    End Sub


    Private Sub ButtonRecord_Click(sender As Object, e As EventArgs) Handles ButtonRecord.Click
        If ButtonRecord.Text = "Start Recording" Then
            ButtonRecord.Text = "Stop Recording"
            DataGridViewHumidity.Rows.Clear()
            ChartHumidityRec.Series("Series1").Points.Clear()

            CmdExportToExcel.Enabled = False
            CmdClearRecording.Enabled = False

            SR = True

        Else
            ButtonRecord.Text = "Start Recording"

            CmdExportToExcel.Enabled = True
            CmdClearRecording.Enabled = True
            SR = False


            LblTime.Text = "Timer 00: 00"
        End If
        '----------------------------------------
    End Sub

    Private Sub CmdClearRecording_Click(sender As Object, e As EventArgs) Handles CmdClearRecording.Click
        DataGridViewHumidity.Rows.Clear()
        ChartHumidityRec.Series("Series1").Points.Clear()
        ChartHumidityRec.Series("Series2").Points.Clear()
        For i = 0 To 13
            ChartHumidityRec.Series("Series1").Points.AddXY("00:00", 0)
        Next
    End Sub

    Private Sub CmdExportToExcel_Click(sender As Object, e As EventArgs) Handles CmdExportToExcel.Click
        Try
            If DataGridViewHumidity.Rows.Count > 0 Then
                Dim filename As String = ""
                Dim SV As SaveFileDialog = New SaveFileDialog()
                SV.Filter = "EXCEL FILES|*.xlsx;*.xls"
                Dim result As DialogResult = SV.ShowDialog()

                If result = DialogResult.OK Then
                    Me.Text = "Monitoring Heart Rate (Saving to Excel. Please wait...)"
                    ProgressBarSave.Visible = True
                    ProgressBarSave.Value = 2
                    filename = SV.FileName
                    Dim multiselect As Boolean = DataGridViewHumidity.MultiSelect
                    DataGridViewHumidity.MultiSelect = True
                    DataGridViewHumidity.SelectAll()
                    DataGridViewHumidity.ClipboardCopyMode = DataGridViewClipboardCopyMode.EnableAlwaysIncludeHeaderText
                    Clipboard.SetDataObject(DataGridViewHumidity.GetClipboardContent())
                    Dim results = System.Convert.ToString(Clipboard.GetData(DataFormats.Text))
                    DataGridViewHumidity.ClearSelection()
                    DataGridViewHumidity.MultiSelect = multiselect
                    Dim XCELAPP As Microsoft.Office.Interop.Excel.Application = Nothing
                    Dim XWORKBOOK As Microsoft.Office.Interop.Excel.Workbook = Nothing
                    Dim XSHEET As Microsoft.Office.Interop.Excel.Worksheet = Nothing
                    Dim misValue As Object = System.Reflection.Missing.Value
                    ProgressBarSave.Value = 4
                    XCELAPP = New Excel.Application()
                    XWORKBOOK = XCELAPP.Workbooks.Add(misValue)
                    XCELAPP.DisplayAlerts = False
                    XCELAPP.Visible = False
                    XSHEET = XWORKBOOK.ActiveSheet
                    ProgressBarSave.Value = 6
                    XSHEET.Paste()
                    XWORKBOOK.SaveAs(filename, Excel.XlFileFormat.xlOpenXMLWorkbook)
                    XWORKBOOK.Close(False)
                    XCELAPP.Quit()
                    ProgressBarSave.Value = 8
                    Try
                        System.Runtime.InteropServices.Marshal.ReleaseComObject(XSHEET)
                        System.Runtime.InteropServices.Marshal.ReleaseComObject(XWORKBOOK)
                        System.Runtime.InteropServices.Marshal.ReleaseComObject(XCELAPP)
                    Catch
                    End Try
                    Me.Text = "Monitoring Heart Rate"
                    ProgressBarSave.Value = 10
                    ProgressBarSave.Visible = False
                    MessageBox.Show("Save Succesfully")
                End If
            End If
        Catch ex As Exception
            Me.Text = "Data Log"
            MessageBox.Show(ex.Message, "Error Message", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try
    End Sub
End Class