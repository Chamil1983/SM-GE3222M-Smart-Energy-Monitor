Public Class Configurations
    Public Page_Name As String
    Public COM_MODE As String

    Public Config_State As Boolean
    Public SerialBaud As Integer

    Public WIFi_Mode As Boolean
    Public WIFi_State As String
    Public WIFiClient_Mode As Boolean
    Public WIFiSSID As String
    Public WiFiPassword As String
    Public WiFiHost_IP As String
    Public Server_IP As String

    Public WiFiServerPort As Integer
    Public WiFiGateway As String
    Public WiFiSubnet As String
    Public DNS1 As String
    Public DNS2 As String

    Public WiFiAP_Mode As Boolean
    Public WiFiSoftAP_IP As String
    Public SoftAP_SSID As String
    Public SoftAP_Password As String

    Public ETH_Mode As Boolean
    Public ETH_HostIP As String
    Public ETH_TCPPort As Integer
    Public Publish_Interval As Decimal

    Public MDBUS_Mode As Boolean
    Public MDBUS_ID As Integer
    Public MDBus_TCPHostIP As String
    Public MDBus_TCPPort As Integer
    Public MDBUS_BAUD As Integer
    Public Config_Mode As String
End Class
