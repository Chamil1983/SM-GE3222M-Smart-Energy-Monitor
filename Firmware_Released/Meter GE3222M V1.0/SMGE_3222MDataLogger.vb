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
Public Class FrmSMGE_3222MDataLogger

    Dim Limit As Integer = 10
    Dim FilePathAndName As String
    Private Sub FrmSMGE_3222MDataLogger_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        CmdStartRecording.Enabled = True
        CmdStopRecording.Enabled = False
        'FrmSMGE_3222MLoging.TimerDataLogRecord.Enabled = False

    End Sub

    Private Sub CmdStartRecording_Click(sender As Object, e As EventArgs) Handles CmdStartRecording.Click
        CmdStartRecording.Enabled = False
        CmdStopRecording.Enabled = True
        CmdSavetoExcel.Enabled = False
        'FrmSMGE_3222MLoging.TimerDataLogRecord.Enabled = True
        FrmSMGE_3222MLoging.TimerDataLogRecord.Start()
    End Sub

    Private Sub CmdStopRecording_Click(sender As Object, e As EventArgs) Handles CmdStopRecording.Click
        CmdStartRecording.Enabled = True
        CmdStopRecording.Enabled = False
        CmdSavetoExcel.Enabled = True
        FrmSMGE_3222MLoging.TimerDataLogRecord.Stop()
        PictureBoxRecordInd.Visible = True
    End Sub

    Private Sub CmdSavetoExcel_Click(sender As Object, e As EventArgs) Handles CmdSavetoExcel.Click

        CmdSavetoExcel.Text = "Please Wait..."
        CmdSavetoExcel.Enabled = False
        CmdStartRecording.Enabled = False
        ProgressBarProcess.Visible = True
        ProgressBarProcess.Value = 1

        Dim xlApp As Microsoft.Office.Interop.Excel.Application
        Dim xlWorkBook As Microsoft.Office.Interop.Excel.Workbook
        Dim xlWorkSheet As Microsoft.Office.Interop.Excel.Worksheet
        Dim misValue As Object = System.Reflection.Missing.Value
        Dim i As Integer
        Dim j As Integer

        ProgressBarProcess.Value = 3

        xlApp = New Microsoft.Office.Interop.Excel.Application
        xlWorkBook = xlApp.Workbooks.Add(misValue)
        xlWorkSheet = xlWorkBook.Sheets("sheet1")

        ProgressBarProcess.Value = 5

        For i = 0 To DataGridView1.RowCount - 2
            For j = 0 To DataGridView1.ColumnCount - 1
                For k As Integer = 1 To DataGridView1.Columns.Count
                    xlWorkSheet.Cells(1, k) = DataGridView1.Columns(k - 1).HeaderText
                    xlWorkSheet.Cells(i + 2, j + 1) = DataGridView1(j, i).Value.ToString()
                Next
            Next
        Next

        ProgressBarProcess.Value = 8

        FilePathAndName = Application.StartupPath & "\" & Now.Day & "-" & Now.Month & "-" & Now.Year & ".xlsx"
        If File.Exists(FilePathAndName) Then File.Delete(FilePathAndName)

        xlWorkSheet.SaveAs(FilePathAndName)
        xlWorkBook.Close()
        xlApp.Quit()

        releaseObject(xlApp)
        releaseObject(xlWorkBook)
        releaseObject(xlWorkSheet)

        ProgressBarProcess.Value = 10

        MsgBox("Successfully saved" & vbCrLf & "File are saved at : " & FilePathAndName, MsgBoxStyle.Information, "Information")

        ProgressBarProcess.Visible = False

        Process.Start(FilePathAndName)


        CmdSavetoExcel.Text = "Save To MS Excel"
        CmdSavetoExcel.Enabled = True
        CmdStartRecording.Enabled = True
    End Sub


    Private Sub releaseObject(ByVal obj As Object)
        Try
            System.Runtime.InteropServices.Marshal.ReleaseComObject(obj)
            obj = Nothing
        Catch ex As Exception
            obj = Nothing
        Finally
            GC.Collect()
        End Try
    End Sub

    Private Sub CmdClearGridviewandGraph_Click(sender As Object, e As EventArgs) Handles CmdClearGridviewandGraph.Click
        DataGridView1.Rows.Clear()
    End Sub



    Private Sub CmdBack_Click(sender As Object, e As EventArgs) Handles CmdBack.Click
        Me.Hide()
        FrmSMGE_3222MLoging.Show()
    End Sub



    Private Sub CheckedListBox11_SelectedIndexChanged(sender As Object, e As EventArgs)

    End Sub

    Private Sub ChartMeterCon_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ChartMeterCon.SelectedIndexChanged



        'If ChartMeterCon.SelectedItem = "Temp C" Then
        'FrmSMGE_3222M_TempC.Show()
        'End If

        ' If ChartMeterCon.SelectedItem = "Temp F" Then
        'FrmSMGE_3222M_TempF.Show()
        'End If

    End Sub

    Private Sub CmdLoadGraph_Click(sender As Object, e As EventArgs) Handles CmdLoadGraph.Click

        FrmSMGE_3222MLoging.TimerChartRecord.Start()
        If ChartMeterCon.SelectedItem = "Hum" And ChartMeterCon.GetItemChecked(1) = False Then
            FrmSMGE_3222M_BoardData.Show()
            FrmSMGE_3222M_BoardData.ChartHumTemp.Series(1).Enabled = False
            FrmSMGE_3222M_BoardData.ChartHumTemp.Series(0).Enabled = True
        End If

        If ChartMeterCon.GetItemChecked(0) = True And ChartMeterCon.GetItemChecked(1) = True Then

            FrmSMGE_3222M_BoardData.ChartHumTemp.Series(0).Enabled = True
            FrmSMGE_3222M_BoardData.ChartHumTemp.Series(1).Enabled = True
            FrmSMGE_3222M_BoardData.Show()
        End If
        '=====================================================================================
        If ChartVoltage.GetItemChecked(0) = True And ChartVoltage.GetItemChecked(1) = False And ChartVoltage.GetItemChecked(2) = False Then

            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Enabled = False

            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(0).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(1).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(2).Enabled = False

            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(1).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(2).Visible = False
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(3).Visible = False
            FrmSMGE_3222M_VoltageData.Show()

            FrmVoltage.Show()
        End If

        If ChartVoltage.GetItemChecked(0) = False And ChartVoltage.GetItemChecked(1) = True And ChartVoltage.GetItemChecked(2) = False Then

            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Enabled = False

            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(0).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(1).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(2).Enabled = False

            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(1).Visible = False
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(2).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(3).Visible = False
            FrmSMGE_3222M_VoltageData.Show()
        End If

        If ChartVoltage.GetItemChecked(0) = False And ChartVoltage.GetItemChecked(1) = False And ChartVoltage.GetItemChecked(2) = True Then

            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Enabled = True

            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(0).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(1).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(2).Enabled = True

            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(1).Visible = False
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(2).Visible = False
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(3).Visible = True
            FrmSMGE_3222M_VoltageData.Show()
        End If

        If ChartVoltage.GetItemChecked(0) = True And ChartVoltage.GetItemChecked(1) = True And ChartVoltage.GetItemChecked(2) = False Then

            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Enabled = False

            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(0).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(1).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(2).Enabled = False


            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(1).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(2).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(3).Visible = False
            FrmSMGE_3222M_VoltageData.Show()
        End If

        If ChartVoltage.GetItemChecked(0) = True And ChartVoltage.GetItemChecked(1) = False And ChartVoltage.GetItemChecked(2) = True Then

            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Enabled = True

            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(0).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(1).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(2).Enabled = True

            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(1).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(2).Visible = False
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(3).Visible = True

            FrmSMGE_3222M_VoltageData.Show()
        End If

        If ChartVoltage.GetItemChecked(0) = False And ChartVoltage.GetItemChecked(1) = True And ChartVoltage.GetItemChecked(2) = True Then

            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Enabled = True

            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(0).Enabled = False
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(1).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltageRec.Series(2).Enabled = True

            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(1).Visible = False
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(2).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(3).Visible = True

            FrmSMGE_3222M_VoltageData.Show()
        End If

        If ChartVoltage.GetItemChecked(0) = True And ChartVoltage.GetItemChecked(1) = True And ChartVoltage.GetItemChecked(2) = True Then

            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(0).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(1).Enabled = True
            FrmSMGE_3222M_VoltageData.ChartVoltage.Series(2).Enabled = True

            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(1).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(2).Visible = True
            FrmSMGE_3222M_VoltageData.DataGridViewVoltage.Columns(3).Visible = True
            FrmSMGE_3222M_VoltageData.Show()
        End If


        '=====================================================================================

        If ChartCurrent.Checked = True Then
            FrmSMGE_3222M_CurrentData.Show()
            FrmCurrent.Show()
        End If

        If ChartActPower.Checked = True Then
            FrmSMGE_3222M_ActPowerData.Show()

        End If

        If ChartReactPower.Checked = True Then
            FrmSMGE_3222M_ReactPowerData.Show()

        End If

        If ChartApptPower.Checked = True Then
            FrmSMGE_3222M_AppntPowerData.Show()

        End If

        If ChartPFactor.Checked = True Then
            FrmSMGE_3222M_PowerFactorData.Show()

        End If

        If ChartPhaAngle.Checked = True Then
            FrmSMGE_3222M_PhaseAngleData.Show()

        End If

        If ChartFreq.Checked = True Then
            FrmSMGE_3222M_FrequencyData.Show()

        End If

        If ChartActEngyImp.Checked = True Then
            FrmSMGE_3222M_ActiveEnergyImp.Show()

        End If
    End Sub

    Private Sub CmdReadData_Click(sender As Object, e As EventArgs) Handles CmdReadData.Click
        Me.Hide()
        FrmSM_GE3222M_Meter_Readings.Show()
    End Sub
End Class