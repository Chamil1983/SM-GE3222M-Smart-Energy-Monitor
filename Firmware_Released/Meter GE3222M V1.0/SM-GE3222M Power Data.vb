Public Class FrmSM_GE3222M_Power_Data
    Private Sub LblVectorSum_TotalApparentPowerVal_Click(sender As Object, e As EventArgs) Handles LblVectorSum_TotalApparentPowerVal.Click

    End Sub

    Private Sub CmdEnergyrData_Click(sender As Object, e As EventArgs) Handles CmdEnergyrData.Click
        Me.Hide()
        FrmSM_GE3222M_Energy_Data.Show()
    End Sub

    Private Sub CmdBack_Click(sender As Object, e As EventArgs) Handles CmdBack.Click
        Me.Hide()
        FrmSMGE_3222MLoging.Show()
    End Sub
End Class