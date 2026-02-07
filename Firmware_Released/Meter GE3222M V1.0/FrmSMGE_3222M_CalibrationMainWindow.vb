Public Class FrmSMGE_3222M_CalibrationMainWindow
    Private Sub CmdSave_Click(sender As Object, e As EventArgs) Handles CmdBack.Click

    End Sub

    Private Sub ListBox1_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ListBoxWindowForms.SelectedIndexChanged

    End Sub

    Private Sub CmdOpen_Click(sender As Object, e As EventArgs) Handles CmdOpen.Click
        Dim FrmNo As Integer = ListBoxWindowForms.SelectedIndex


        Select Case FrmNo
            Case 0

                FrmSMGE_3222M_Configuration.Show()
                FrmSMGE_3222M_Configuration.TimerHandler1_Client.Start()
                FrmSMGE_3222MLoging.TimerSync.Enabled = True
                FrmSMGE_3222MLoging.TimerSync.Start()


            Case 1

                FrmSMGE_3222M_CommonCalibrationWindow.Show()
                FrmSMGE_3222M_CommonCalibrationWindow.TimerHandler1_Client.Start()
                FrmSMGE_3222MLoging.TimerSync.Enabled = True
                FrmSMGE_3222MLoging.TimerSync.Start()

            Case 2

                FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.Show()
                FrmSMGE_3222M_FundamentalHarmonic_EnergyCalibrationWindow.TimerHandler1_Client.Start()
                FrmSMGE_3222MLoging.TimerSync.Enabled = True
                FrmSMGE_3222MLoging.TimerSync.Start()

            Case 3

                FrmSMGE_3222M_MeasurementCalibrationWindow.Show()
                FrmSMGE_3222M_MeasurementCalibrationWindow.TimerHandler1_Client.Start()
                FrmSMGE_3222MLoging.TimerSync.Enabled = True
                FrmSMGE_3222MLoging.TimerSync.Start()

            Case Else

                MsgBox("Please Select Form Window to open")
        End Select


    End Sub

    Private Sub CmdCancel_Click(sender As Object, e As EventArgs) Handles CmdCancel.Click
        Me.Close()
        FrmSMGE_3222MLoging.CalMode = False
        FrmSMGE_3222MLoging.TimerHandler1_Client.Start()
        'FrmSMGE_3222MLoging.TxtComTerm.Clear()
    End Sub

    Private Sub FrmSMGE_3222M_CalibrationMainWindow_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub
End Class