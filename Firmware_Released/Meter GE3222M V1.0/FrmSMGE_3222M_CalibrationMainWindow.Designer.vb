<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FrmSMGE_3222M_CalibrationMainWindow
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
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
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.GrpCalibrations = New System.Windows.Forms.GroupBox()
        Me.CmdOpen = New System.Windows.Forms.Button()
        Me.CmdBack = New System.Windows.Forms.Button()
        Me.CmdCancel = New System.Windows.Forms.Button()
        Me.ListBoxWindowForms = New System.Windows.Forms.ListBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.GrpCalibrations.SuspendLayout()
        Me.SuspendLayout()
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 257)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Padding = New System.Windows.Forms.Padding(1, 0, 16, 0)
        Me.StatusStrip1.Size = New System.Drawing.Size(523, 22)
        Me.StatusStrip1.TabIndex = 0
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'GrpCalibrations
        '
        Me.GrpCalibrations.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.GrpCalibrations.Controls.Add(Me.CmdOpen)
        Me.GrpCalibrations.Controls.Add(Me.CmdBack)
        Me.GrpCalibrations.Controls.Add(Me.CmdCancel)
        Me.GrpCalibrations.Controls.Add(Me.ListBoxWindowForms)
        Me.GrpCalibrations.Controls.Add(Me.Label2)
        Me.GrpCalibrations.Controls.Add(Me.Label1)
        Me.GrpCalibrations.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.GrpCalibrations.Location = New System.Drawing.Point(14, 12)
        Me.GrpCalibrations.Margin = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpCalibrations.Name = "GrpCalibrations"
        Me.GrpCalibrations.Padding = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.GrpCalibrations.Size = New System.Drawing.Size(502, 242)
        Me.GrpCalibrations.TabIndex = 1
        Me.GrpCalibrations.TabStop = False
        Me.GrpCalibrations.Text = "Calibration Window Selection"
        '
        'CmdOpen
        '
        Me.CmdOpen.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdOpen.Location = New System.Drawing.Point(20, 118)
        Me.CmdOpen.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdOpen.Name = "CmdOpen"
        Me.CmdOpen.Size = New System.Drawing.Size(100, 23)
        Me.CmdOpen.TabIndex = 16
        Me.CmdOpen.Text = "Open"
        Me.CmdOpen.UseVisualStyleBackColor = True
        '
        'CmdBack
        '
        Me.CmdBack.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdBack.Location = New System.Drawing.Point(20, 159)
        Me.CmdBack.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdBack.Name = "CmdBack"
        Me.CmdBack.Size = New System.Drawing.Size(100, 23)
        Me.CmdBack.TabIndex = 14
        Me.CmdBack.Text = "Back"
        Me.CmdBack.UseVisualStyleBackColor = True
        '
        'CmdCancel
        '
        Me.CmdCancel.Font = New System.Drawing.Font("Calisto MT", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.CmdCancel.Location = New System.Drawing.Point(20, 200)
        Me.CmdCancel.Margin = New System.Windows.Forms.Padding(4)
        Me.CmdCancel.Name = "CmdCancel"
        Me.CmdCancel.Size = New System.Drawing.Size(100, 23)
        Me.CmdCancel.TabIndex = 13
        Me.CmdCancel.Text = "Cancel"
        Me.CmdCancel.UseVisualStyleBackColor = True
        '
        'ListBoxWindowForms
        '
        Me.ListBoxWindowForms.FormattingEnabled = True
        Me.ListBoxWindowForms.ItemHeight = 17
        Me.ListBoxWindowForms.Items.AddRange(New Object() {"Configuration Window", "Common Calibration Window", "Fundamental/ Harmonic Energy Calibration Window", "Measurement Calibration Window"})
        Me.ListBoxWindowForms.Location = New System.Drawing.Point(162, 84)
        Me.ListBoxWindowForms.Name = "ListBoxWindowForms"
        Me.ListBoxWindowForms.Size = New System.Drawing.Size(327, 140)
        Me.ListBoxWindowForms.TabIndex = 4
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(17, 84)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(139, 17)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = "Calibration Window : "
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(17, 28)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(283, 17)
        Me.Label1.TabIndex = 2
        Me.Label1.Text = "Please Select Calibration from the Listbox below"
        '
        'FrmSMGE_3222M_CalibrationMainWindow
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(7.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(523, 279)
        Me.Controls.Add(Me.GrpCalibrations)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Font = New System.Drawing.Font("Calisto MT", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.Margin = New System.Windows.Forms.Padding(4, 3, 4, 3)
        Me.Name = "FrmSMGE_3222M_CalibrationMainWindow"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "FrmSMGE_3222M_CalibrationMainWindow"
        Me.GrpCalibrations.ResumeLayout(False)
        Me.GrpCalibrations.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents StatusStrip1 As StatusStrip
    Friend WithEvents GrpCalibrations As GroupBox
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents ListBoxWindowForms As ListBox
    Friend WithEvents CmdBack As Button
    Friend WithEvents CmdCancel As Button
    Friend WithEvents CmdOpen As Button
End Class
