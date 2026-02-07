Imports Newtonsoft.Json

Public Class ClsCalibrations


    Public Class Meter_calibration1


        <JsonProperty("Page_Name")>
        Public Property Page_Name As String

        <JsonProperty("Trans")>
        Public Property Trans As String

        <JsonProperty("MMode0")>
        Public Property MMode0 As Integer

        <JsonProperty("MMode1")>
        Public Property MMode1 As Integer

        <JsonProperty("PMPGA")>
        Public Property PMPGA As Integer

        <JsonProperty("PStartTh")>
        Public Property PStartTh As Integer

        <JsonProperty("QStartTh")>
        Public Property QStartTh As Integer

        <JsonProperty("SStartTh")>
        Public Property SStartTh As Integer

        <JsonProperty("PPhaseTh")>
        Public Property PPhaseTh As Integer

        <JsonProperty("QPhaseTh")>
        Public Property QPhaseTh As Integer

        <JsonProperty("SPhaseTh")>
        Public Property SPhaseTh As Integer

    End Class

    Public Class Meter_calibration2

        <JsonProperty("Page_Name")>
        Public Property Page_Name As String

        <JsonProperty("Trans")>
        Public Property Trans As String

        <JsonProperty("PoffsetA")>
        Public Property PoffsetA As Integer

        <JsonProperty("QoffsetA")>
        Public Property QoffsetA As Integer

        <JsonProperty("POffsetB")>
        Public Property POffsetB As Integer

        <JsonProperty("QOffsetB")>
        Public Property QOffsetB As Integer

        <JsonProperty("POffsetC")>
        Public Property POffsetC As Integer

        <JsonProperty("QOffsetC")>
        Public Property QOffsetC As Integer

        <JsonProperty("GainA")>
        Public Property GainA As Integer

        <JsonProperty("PhiA")>
        Public Property PhiA As Integer

        <JsonProperty("GainB")>
        Public Property GainB As Integer

        <JsonProperty("PhiB")>
        Public Property PhiB As Integer

        <JsonProperty("GainC")>
        Public Property GainC As Integer

        <JsonProperty("PhiC")>
        Public Property PhiC As Integer

    End Class

    Public Class Meter_calibration3

        <JsonProperty("Page_Name")>
        Public Property Page_Name As String

        <JsonProperty("Trans")>
        Public Property Trans As String

        <JsonProperty("POffsetAF")>
        Public Property POffsetAF As Integer

        <JsonProperty("POffsetBF")>
        Public Property POffsetBF As Integer

        <JsonProperty("POffsetCF")>
        Public Property POffsetCF As Integer

        <JsonProperty("PGainAF")>
        Public Property PGainAF As Integer

        <JsonProperty("PGainBF")>
        Public Property PGainBF As Integer

        <JsonProperty("PGainCF")>
        Public Property PGainCF As Integer

    End Class

    Public Class Meter_calibration4

        <JsonProperty("Page_Name")>
        Public Property Page_Name As String

        <JsonProperty("Trans")>
        Public Property Trans As String

        <JsonProperty("UgainA")>
        Public Property UgainA As Integer

        <JsonProperty("IgainA")>
        Public Property IgainA As Integer

        <JsonProperty("UoffsetA")>
        Public Property UoffsetA As Integer

        <JsonProperty("IoffsetA")>
        Public Property IoffsetA As Integer

        <JsonProperty("UgainB")>
        Public Property UgainB As Integer

        <JsonProperty("IgainB")>
        Public Property IgainB As Integer

        <JsonProperty("UoffsetB")>
        Public Property UoffsetB As Integer

        <JsonProperty("IoffsetB")>
        Public Property IoffsetB As Integer

        <JsonProperty("UgainC")>
        Public Property UgainC As Integer

        <JsonProperty("IgainC")>
        Public Property IgainC As Integer

        <JsonProperty("UoffsetC")>
        Public Property UoffsetC As Integer

        <JsonProperty("IoffsetC")>
        Public Property IoffsetC As Integer

        <JsonProperty("IgainN")>
        Public Property IgainN As Integer

        <JsonProperty("IoffsetN")>
        Public Property IoffsetN As Integer

    End Class

End Class
