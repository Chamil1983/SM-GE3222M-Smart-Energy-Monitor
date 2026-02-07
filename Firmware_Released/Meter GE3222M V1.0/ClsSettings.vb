Imports Newtonsoft.Json

Public Class ClsSettings
    Public Class MeterSettings


        <JsonProperty("Page_Name")>
        Public Property Page_Name As String

        <JsonProperty("COM_MODE")>
        Public Property COM_MODE As String

        <JsonProperty("Config_State")>
        Public Property ConfigState As Boolean

        <JsonProperty("SerialBaud")>
        Public Property SerialBaud As Integer

        <JsonProperty("WIFi_Mode")>
        Public Property WIFi_Mode As Boolean

        <JsonProperty("WIFi_State")>
        Public Property WIFi_State As String

        <JsonProperty("WiFiClient_Mode")>
        Public Property WiFiClient_Mode As Boolean

        <JsonProperty("WIFiSSID")>
        Public Property WIFiSSID As String

        <JsonProperty("WiFiPassword")>
        Public Property WiFiPassword As String

        <JsonProperty("WiFiHost_IP")>
        Public Property WiFiHostIP As String

        <JsonProperty("Server_IP")>
        Public Property ServerIP As String

        <JsonProperty("WiFiServerPort")>
        Public Property WiFiServerPort As Integer

        <JsonProperty("WiFiGateway")>
        Public Property WiFiGateway As String

        <JsonProperty("WiFiSubnet")>
        Public Property WiFiSubnet As String

        <JsonProperty("DNS1")>
        Public Property DNS1 As String

        <JsonProperty("DNS2")>
        Public Property DNS2 As String


        <JsonProperty("WiFiAP_Mode")>
        Public Property WiFiAP_Mode As Boolean

        <JsonProperty("WiFiSoftAP_IP")>
        Public Property WiFiSoftAP_IP As String

        <JsonProperty("SoftAP_SSID")>
        Public Property SoftAP_SSID As String

        <JsonProperty("SoftAP_Password")>
        Public Property SoftAP_Password As String

        <JsonProperty("ETH_Mode")>
        Public Property ETH_Mode As Boolean

        <JsonProperty("ETH_HostIP")>
        Public Property ETHHostIP As String

        <JsonProperty("ETH_TCPPort")>
        Public Property ETHTCPPort As Integer

        <JsonProperty("Publish_Interval")>
        Public Property PublishInterval As Decimal

        <JsonProperty("MDBUS_Mode")>
        Public Property MDBUS_Mode As Boolean

        <JsonProperty("MDBUS_ID")>
        Public Property MDBUSID As Integer

        <JsonProperty("MDBus_TCPHostIP")>
        Public Property MDBusTCPHostIP As String

        <JsonProperty("MDBus_TCPPort")>
        Public Property MDBusTCPPort As Integer

        <JsonProperty("MDBUS_BAUD")>
        Public Property MDBUSBAUD As Integer

        <JsonProperty("Config_Mode")>
        Public Property ConfigMode As String
    End Class


End Class
