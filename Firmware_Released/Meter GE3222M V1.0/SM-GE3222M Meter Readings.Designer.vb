<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class FrmSM_GE3222M_Meter_Readings
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
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
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.CmdEnergyData = New System.Windows.Forms.Button()
        Me.CmdPowerData = New System.Windows.Forms.Button()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdExit = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'CmdEnergyData
        '
        Me.CmdEnergyData.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdEnergyData.Location = New System.Drawing.Point(48, 48)
        Me.CmdEnergyData.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdEnergyData.Name = "CmdEnergyData"
        Me.CmdEnergyData.Size = New System.Drawing.Size(104, 30)
        Me.CmdEnergyData.TabIndex = 9
        Me.CmdEnergyData.Text = "Energy Data"
        Me.CmdEnergyData.UseVisualStyleBackColor = True
        '
        'CmdPowerData
        '
        Me.CmdPowerData.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdPowerData.Location = New System.Drawing.Point(185, 48)
        Me.CmdPowerData.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdPowerData.Name = "CmdPowerData"
        Me.CmdPowerData.Size = New System.Drawing.Size(104, 30)
        Me.CmdPowerData.TabIndex = 10
        Me.CmdPowerData.Text = "Power Data"
        Me.CmdPowerData.UseVisualStyleBackColor = True
        '
        'CmdBack
        '
        Me.CmdBack.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdBack.Location = New System.Drawing.Point(322, 48)
        Me.CmdBack.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdBack.Name = "CmdBack"
        Me.CmdBack.Size = New System.Drawing.Size(104, 30)
        Me.CmdBack.TabIndex = 11
        Me.CmdBack.Text = "Back"
        Me.CmdBack.UseVisualStyleBackColor = True
        '
        'CmdExit
        '
        Me.CmdExit.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdExit.Location = New System.Drawing.Point(459, 48)
        Me.CmdExit.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdExit.Name = "CmdExit"
        Me.CmdExit.Size = New System.Drawing.Size(104, 30)
        Me.CmdExit.TabIndex = 12
        Me.CmdExit.Text = "Exit"
        Me.CmdExit.UseVisualStyleBackColor = True
        '
        'FrmSM_GE3222M_Meter_Readings
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 17.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(612, 143)
        Me.Controls.Add(Me.CmdExit)
        Me.Controls.Add(Me.CmdBack)
        Me.Controls.Add(Me.CmdPowerData)
        Me.Controls.Add(Me.CmdEnergyData)
        Me.DoubleBuffered = True
        Me.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Margin = New System.Windows.Forms.Padding(4)
        Me.Name = "FrmSM_GE3222M_Meter_Readings"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "SM-GE3222M Meter Readings"
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents CmdEnergyData As Button
    Friend WithEvents CmdPowerData As Button
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdExit As Button
End Class
