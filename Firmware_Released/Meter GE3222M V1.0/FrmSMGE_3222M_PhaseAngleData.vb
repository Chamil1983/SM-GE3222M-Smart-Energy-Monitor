Imports Microsoft.Office.Interop

Public Class FrmSMGE_3222M_PhaseAngleData

    Public SR As Boolean = False
    Private Sub CmdReadData_Click(sender As Object, e As EventArgs) Handles CmdReadData.Click
        Me.Hide()
        FrmSM_GE3222M_Meter_Readings.Show()
    End Sub

    Private Sub ButtonRecord_Click(sender As Object, e As EventArgs) Handles ButtonRecord.Click
        If ButtonRecord.Text = "Start Recording" Then
            ButtonRecord.Text = "Stop Recording"
            DataGridViewPhaseAngle.Rows.Clear()
            ChartPhaseAngleRec.Series("Series1").Points.Clear()

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

    Private Sub CmdBack_Click(sender As Object, e As EventArgs) Handles CmdBack.Click
        DataGridViewPhaseAngle.Rows.Clear()
        ChartPhaseAngleRec.Series("Series1").Points.Clear()
        ChartPhaseAngleRec.Series("Series2").Points.Clear()
        ChartPhaseAngleRec.Series("Series3").Points.Clear()

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

    Private Sub CmdExportToExcel_Click(sender As Object, e As EventArgs) Handles CmdExportToExcel.Click
        Try
            If DataGridViewPhaseAngle.Rows.Count > 0 Then
                Dim filename As String = ""
                Dim SV As SaveFileDialog = New SaveFileDialog()
                SV.Filter = "EXCEL FILES|*.xlsx;*.xls"
                Dim result As DialogResult = SV.ShowDialog()

                If result = DialogResult.OK Then
                    Me.Text = "Monitoring Heart Rate (Saving to Excel. Please wait...)"
                    ProgressBarSave.Visible = True
                    ProgressBarSave.Value = 2
                    filename = SV.FileName
                    Dim multiselect As Boolean = DataGridViewPhaseAngle.MultiSelect
                    DataGridViewPhaseAngle.MultiSelect = True
                    DataGridViewPhaseAngle.SelectAll()
                    DataGridViewPhaseAngle.ClipboardCopyMode = DataGridViewClipboardCopyMode.EnableAlwaysIncludeHeaderText
                    Clipboard.SetDataObject(DataGridViewPhaseAngle.GetClipboardContent())
                    Dim results = System.Convert.ToString(Clipboard.GetData(DataFormats.Text))
                    DataGridViewPhaseAngle.ClearSelection()
                    DataGridViewPhaseAngle.MultiSelect = multiselect
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

    Private Sub CmdClearRecording_Click(sender As Object, e As EventArgs) Handles CmdClearRecording.Click
        DataGridViewPhaseAngle.Rows.Clear()
        ChartPhaseAngleRec.Series("Series1").Points.Clear()
        ChartPhaseAngleRec.Series("Series2").Points.Clear()
        ChartPhaseAngleRec.Series("Series3").Points.Clear()

    End Sub
End Class