Public Class FrmSM_GE3222M_Energy_Data
    Private Sub SM_GE3222M_Energy_Data_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub LblPhaC_ReverseActiveEnergyVal_Click(sender As Object, e As EventArgs) Handles LblPhaC_ReverseActiveEnergyVal.Click

    End Sub

    Private Sub CmdPowerData_Click(sender As Object, e As EventArgs) Handles CmdPowerData.Click
        FrmSM_GE3222M_Power_Data.Show()
        Me.Hide()

    End Sub

    Private Sub CmdBack_Click(sender As Object, e As EventArgs) Handles CmdBack.Click
        Me.Hide()
        FrmSMGE_3222MLoging.Show()
    End Sub
End Class