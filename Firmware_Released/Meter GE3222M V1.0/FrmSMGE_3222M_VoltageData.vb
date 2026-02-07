Imports System.Reflection
Imports Microsoft.Office.Interop

Public Class FrmSMGE_3222M_VoltageData

    Dim value1, value2, value3 As Decimal
    Dim Limit As Integer = 10

    Public SR As Boolean = False

    Private Sub FrmSMGE_3222M_VoltageData_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        For i = 0 To 30 Step 1
            ChartVoltage.Series("V1").Points.AddXY(DateTime.Now.ToLongTimeString, 0)
            If ChartVoltage.Series(0).Points.Count = Limit Then
                ChartVoltage.Series(0).Points.RemoveAt(0)
            End If
            ChartVoltage.ChartAreas(0).AxisY.Maximum = 320


        Next
        ChartVoltage.ChartAreas(0).AxisY.Maximum = 320

        For i = 0 To 15
            ChartVoltageRec.Series("Series1").Points.AddXY("00:00", 0)
        Next


        DoubleBuffer.DoubleBuffered(DataGridViewVoltage, True) '-> To speed up DataGridView performance
    End Sub

    Private Sub CmdClearRecording_Click(sender As Object, e As EventArgs) Handles CmdClearRecording.Click
        DataGridViewVoltage.Rows.Clear()
        ChartVoltageRec.Series("Series1").Points.Clear()
        ChartVoltageRec.Series("Series2").Points.Clear()
        ChartVoltageRec.Series("Series3").Points.Clear()
        For i = 0 To 13
            ChartVoltageRec.Series("Series1").Points.AddXY("00:00", 0)
        Next
    End Sub

    Private Sub CmdExportToExcel_Click(sender As Object, e As EventArgs) Handles CmdExportToExcel.Click
        Try
            If DataGridViewVoltage.Rows.Count > 0 Then
                Dim filename As String = ""
                Dim SV As SaveFileDialog = New SaveFileDialog()
                SV.Filter = "EXCEL FILES|*.xlsx;*.xls"
                Dim result As DialogResult = SV.ShowDialog()

                If result = DialogResult.OK Then
                    Me.Text = "Monitoring Heart Rate (Saving to Excel. Please wait...)"
                    ProgressBarSave.Visible = True
                    ProgressBarSave.Value = 2
                    filename = SV.FileName
                    Dim multiselect As Boolean = DataGridViewVoltage.MultiSelect
                    DataGridViewVoltage.MultiSelect = True
                    DataGridViewVoltage.SelectAll()
                    DataGridViewVoltage.ClipboardCopyMode = DataGridViewClipboardCopyMode.EnableAlwaysIncludeHeaderText
                    Clipboard.SetDataObject(DataGridViewVoltage.GetClipboardContent())
                    Dim results = System.Convert.ToString(Clipboard.GetData(DataFormats.Text))
                    DataGridViewVoltage.ClearSelection()
                    DataGridViewVoltage.MultiSelect = multiselect
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

    Private Sub CmdBack_Click(sender As Object, e As EventArgs) Handles CmdBack.Click
        DataGridViewVoltage.Rows.Clear()
        ChartVoltageRec.Series("Series1").Points.Clear()
        ChartVoltageRec.Series("Series2").Points.Clear()
        ChartVoltageRec.Series("Series3").Points.Clear()

        LblTime.Text = "Timer 00: 00"
        ButtonRecord.Text = "Start Recording"
        SR = False
        FrmSMGE_3222MLoging.TimerChartRecord.Stop()
        Me.Hide()
        FrmSMGE_3222MDataLogger.Show()
    End Sub

    Private Sub CmdCancel_Click(sender As Object, e As EventArgs) Handles CmdCancel.Click
        Me.Hide()
        FrmSMGE_3222MLoging.Show()
    End Sub

    Private Sub CmdReadData_Click(sender As Object, e As EventArgs) Handles CmdReadData.Click
        Me.Hide()
        FrmSM_GE3222M_Meter_Readings.Show()

    End Sub

    Private Sub ButtonRecord_Click(sender As Object, e As EventArgs) Handles ButtonRecord.Click
        If ButtonRecord.Text = "Start Recording" Then
            ButtonRecord.Text = "Stop Recording"
            DataGridViewVoltage.Rows.Clear()
            ChartVoltageRec.Series("Series1").Points.Clear()

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

    Private Sub TimerGaugeCom_Tick(sender As Object, e As EventArgs) Handles TimerGaugeCom.Tick
        Try
            value1 = Convert.ToDecimal(LblV1Val.Text)
            value2 = Convert.ToDecimal(LblV2Val.Text)
            value3 = Convert.ToDecimal(LblV3Val.Text)

        Catch ex As Exception

        End Try

        'TB4.Text = somestring(2)

        GCVoltage1.SetPointerValue("Scale1", "Pointer1", value1)
        GCVoltage2.SetPointerValue("Scale1", "Pointer1", value2)
        GCVoltage3.SetPointerValue("Scale1", "Pointer1", value3)
    End Sub
End Class