Public Class FrmSMGE_3222M_CurrentData
    Public LB1, LB2, TB3, TB4 As New Label
    Dim value1, value2, value3, value4, value5 As Decimal

    Private Sub ChkPha1_CheckedChanged(sender As Object, e As EventArgs) Handles ChkPha1.CheckedChanged

        If ChkPha1.Checked = True Then
            ChartCurrent.Series(0).Enabled = True
            ChartCurrent.Series(1).Enabled = False
            ChartCurrent.Series(2).Enabled = False
            ChartCurrent.Series(3).Enabled = False
            ChartCurrent.Series(4).Enabled = False

        Else

            ChartCurrent.Series(0).Enabled = True
            ChartCurrent.Series(1).Enabled = True
            ChartCurrent.Series(2).Enabled = True
            ChartCurrent.Series(3).Enabled = True
            ChartCurrent.Series(4).Enabled = True

        End If

    End Sub



    Private Sub CmdBack_Click(sender As Object, e As EventArgs) Handles CmdBack.Click
        DataGridViewCurrent.Rows.Clear()
        ChartCurrentRec.Series("Series1").Points.Clear()
        ChartCurrentRec.Series("Series2").Points.Clear()
        ChartCurrentRec.Series("Series3").Points.Clear()
        ChartCurrentRec.Series("Series4").Points.Clear()
        ChartCurrentRec.Series("Series5").Points.Clear()

        LblTime.Text = "Timer 00: 00"
        ButtonRecord.Text = "Start Recording"
        SR = False
        FrmSMGE_3222MLoging.TimerChartRecord.Stop()
        Me.Hide()
        FrmSMGE_3222MDataLogger.Show()
    End Sub

    Private Sub ChkTotal_CheckedChanged(sender As Object, e As EventArgs) Handles ChkTotal.CheckedChanged

        If ChkTotal.Checked = True Then
            ChartCurrent.Series(0).Enabled = False
            ChartCurrent.Series(1).Enabled = False
            ChartCurrent.Series(2).Enabled = False
            ChartCurrent.Series(3).Enabled = False
            ChartCurrent.Series(4).Enabled = True

        Else

            ChartCurrent.Series(0).Enabled = True
            ChartCurrent.Series(1).Enabled = True
            ChartCurrent.Series(2).Enabled = True
            ChartCurrent.Series(3).Enabled = True
            ChartCurrent.Series(4).Enabled = True
        End If
    End Sub

    Private Sub ChkN_CheckedChanged(sender As Object, e As EventArgs) Handles ChkN.CheckedChanged
        If ChkN.Checked = True Then
            ChartCurrent.Series(0).Enabled = False
            ChartCurrent.Series(1).Enabled = False
            ChartCurrent.Series(2).Enabled = False
            ChartCurrent.Series(3).Enabled = True
            ChartCurrent.Series(4).Enabled = False

        Else

            ChartCurrent.Series(0).Enabled = True
            ChartCurrent.Series(1).Enabled = True
            ChartCurrent.Series(2).Enabled = True
            ChartCurrent.Series(3).Enabled = True
            ChartCurrent.Series(4).Enabled = True
        End If
    End Sub

    Private Sub CmdCancel_Click(sender As Object, e As EventArgs) Handles CmdCancel.Click
        Me.Hide()
        FrmSMGE_3222MLoging.Show()
    End Sub

    Private Sub CmdClearRecording_Click(sender As Object, e As EventArgs) Handles CmdClearRecording.Click
        DataGridViewCurrent.Rows.Clear()
        ChartCurrentRec.Series("Series1").Points.Clear()
        ChartCurrentRec.Series("Series2").Points.Clear()
        ChartCurrentRec.Series("Series3").Points.Clear()
        ChartCurrentRec.Series("Series4").Points.Clear()
        ChartCurrentRec.Series("Series5").Points.Clear()
        For i = 0 To 13
            ChartCurrentRec.Series("Series1").Points.AddXY("00:00", 0)
        Next
    End Sub

    Private Sub CmdReadData_Click(sender As Object, e As EventArgs) Handles CmdReadData.Click
        Me.Hide()
        FrmSM_GE3222M_Meter_Readings.Show()
    End Sub

    'Dim RecordCheck As Boolean = False

    Public SR As Boolean = False
    Private Sub ButtonRecord_Click(sender As Object, e As EventArgs) Handles ButtonRecord.Click
        If ButtonRecord.Text = "Start Recording" Then
            ButtonRecord.Text = "Stop Recording"
            DataGridViewCurrent.Rows.Clear()
            ChartCurrentRec.Series("Series1").Points.Clear()

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

    Private Sub ChkPha2_CheckedChanged(sender As Object, e As EventArgs) Handles ChkPha2.CheckedChanged
        If ChkPha2.Checked = True Then
            ChartCurrent.Series(0).Enabled = False
            ChartCurrent.Series(1).Enabled = True
            ChartCurrent.Series(2).Enabled = False
            ChartCurrent.Series(3).Enabled = False
            ChartCurrent.Series(4).Enabled = False

        Else

            ChartCurrent.Series(0).Enabled = True
            ChartCurrent.Series(1).Enabled = True
            ChartCurrent.Series(2).Enabled = True
            ChartCurrent.Series(3).Enabled = True
            ChartCurrent.Series(4).Enabled = True


        End If

    End Sub

    Private Sub ChkPha3_CheckedChanged(sender As Object, e As EventArgs) Handles ChkPha3.CheckedChanged
        If ChkPha3.Checked = True Then
            ChartCurrent.Series(0).Enabled = False
            ChartCurrent.Series(1).Enabled = False
            ChartCurrent.Series(2).Enabled = True
            ChartCurrent.Series(3).Enabled = False
            ChartCurrent.Series(4).Enabled = False

        Else

            ChartCurrent.Series(0).Enabled = True
            ChartCurrent.Series(1).Enabled = True
            ChartCurrent.Series(2).Enabled = True
            ChartCurrent.Series(3).Enabled = True
            ChartCurrent.Series(4).Enabled = True


        End If
    End Sub


End Class