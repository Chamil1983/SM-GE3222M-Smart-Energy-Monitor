Public Class FrmSM_GE3222M_Meter_Readings
    Private Sub CmdEnergyData_Click(sender As Object, e As EventArgs) Handles CmdEnergyData.Click
        Me.Hide()
        FrmSM_GE3222M_Energy_Data.Show()
    End Sub

    Private Sub CmdPowerData_Click(sender As Object, e As EventArgs) Handles CmdPowerData.Click
        Me.Hide()
        FrmSM_GE3222M_Power_Data.Show()
    End Sub

    Private Sub FrmSM_GE3222M_Meter_Readings_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub
End Class