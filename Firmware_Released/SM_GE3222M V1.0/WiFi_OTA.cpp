


/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  IPEM-1 ESP32 ATM90E32 ATM90E36 IoT Power Energy Monitoring Energy Monitor v1.0

  Full header information in main.cpp.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and also github.com/DitroniX

  Note: Adding an SSID will Automatically ENABLE WiFi attempt to Log-In.

   Preamble.  This WiFi-OTA will:
    * Configure WiFi Parameters (e.g. ssid etc.)
    * Configure Optional Static IP Address (leave 0,0,0,0 for DHCP settings)
    * Connect to WiFi
    * Create a Network Port in Serial Devices
    * Display WiFI detail on OLED
    * Display Web Information Page (Put IP in Browser)
    * Display Push OTA Page (Remote Uploading of BIN Files)

   WebServer URL Commands.  An example is http://ipaddress/pwm-on  e.g. http:192.168.0.5/pwm-on
   * /pwm-local - This is Enable PWM to take use Local Power reading from specified variable and output.
   * /pwm-remote - This is Enable PWM to take use Remote Power reading from specified variable and output.
   * /pwm-on - This is Enable PWM to take use Local Power reading from specified variable and output.
   * /pwm-off - This is Turn Off PWM Output
   * /pwm-test - This will turn on a test PWM Loop and output
   * /dac-local - This is Enable DAC to take use Local Power reading from specified variable and output.
   * /dac-remote - This is Enable DAC to take use Remote Power reading from specified variable and output.
   * /dac-on - This is Enable DAC to take use Local Power reading from specified  variable and output.
   * /dac-off - This is Turn Off DAC Output
   * /dac-test - This will turn on a test DAC Loop and output

   To setup the Push OTA and Reflash The Board:
    * Once flashed, Open Web Page using IP Address in Web Browser (PC, Tablet, Phone).
    * Upload a BIN file. Compiled from PlatformIO. In Folder IPEM_1_Test_Code_ATM90E32_ATM90E36\.pio\build\wemos_d1_mini32\
*/

// Libraries
#include "WiFi_OTA.h"
#include <LiquidCrystal_I2C.h>
#include "emonesp.h"

// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES ****************




//const char* ssid = "Google NODE"; // Change this to your WiFi SSID
//const char* password = "36002016"; // Change this to your WiFi password

const char* ssid = "SM-GE3222M(WiFi)"; // Change this to your WiFi SSID
const char* password = ""; // Change this to your WiFi password


DNSServer dnsServer;                  // Create class DNS server, captive portal re-direct
const byte DNS_PORT = 53;

// Access Point SSID, password & IP address. SSID will be softAP_ssid + chipID to make SSID unique
const char* softAP_ssid = "SM-GE3222M(WiFi)";
const char* softAP_password = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
int apClients = 0;

bool startAPonWifiDisconnect = true;

// hostname for mDNS. Should work at least on windows. Try http://emonesp.local
const char* esp_hostname = "smenergy";


#ifdef WIFI_LED
int wifiLedState = !WIFI_LED_ON_STATE;
unsigned long wifiLedTimeOut = millis();
#endif

// Last discovered WiFi access points
String st;
String rssi;


// Wifi Network Strings
String connected_network = "";
String ipaddress = "";

int client_disconnects = 0;
bool client_retry = false;
unsigned long client_retry_time = 0;


int wifiButtonState = LOW;
unsigned long wifiButtonTimeOut = millis();
bool apMessage = false;

unsigned long previousMillis = 0;
unsigned long interval = 30000;

// WiFi. Force Disable for Testing.  !!!!! BEWARE Outside of Local Developmet Do NOT Disable as OTA will NOT work !!!!!
const boolean DisableWiFi = false; // Force Disable WiFi for Local USB Development and Testing Only.  You can leave SSID/IP etc populated.


// Set your Static IP address
//#define Client_IP   WiFiHost_IP 
//#define Client_Gateway   WiFiGateway
//#define Client_Subnet   WiFiSubnet
//#define Primary_DNS   DNS1
//#define Secondary_DNS   DNS2


IPAddress Client_IP, Client_Gateway, Client_Subnet,Primary_DNS, Secondary_DNS;
IPAddress SoftAP_IP, SoftAP_Subnet;


// Set your Static IP address and Gateway - Alternatively leave at (0, 0, 0, 0)
      


IPAddress local_IP(0, 0, 0, 0);     // Leave at (0, 0, 0, 0) if DHCP required
IPAddress gateway(0, 0, 0, 0);      // Set to your Router IP = Gateway
IPAddress subnet(255, 255, 255, 0); // Should not need to change subnet from default (255, 255, 255, 0)

// Optional DNS Defaults.
IPAddress primaryDNS(0, 0, 0, 0);   // Defaults to your above Gateway IP if left as (0, 0, 0, 0)
IPAddress secondaryDNS(8, 8, 4, 4); // For Google Public DNS use for Primary or Secondary (8,8,8,8) and/or (8,8,4,4)

// WiFi Other
WiFiClient wlan_client;                   // Initialize the Client Library  / Client Instance
WiFiMulti wifiMulti;                      // Multiple WiFi Options - Auto Scanning
String HostNameHeader = AppAcronym + "-"; // Hostname Prefix
String HostName;                          // Hostname
// String RSSILevel;                         // WiFi RSSI Level Information
const uint32_t connectTimeoutMs = 10000; // WiFi connect timeout per AP. Increase when connecting takes longer.

// NTP Time
WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
// United Kingdom (London, Belfast)
// TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
// TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
// Timezone UK(BST, GMT);


String page_header, page_css, page_title, page_board, page_wifi, page_detail, page_updater, page_footer;

LiquidCrystal_I2C WiFiLCD(0x27, lcdColumns, lcdRows);

// ######### FUNCTIONS #########

// Return Some Meaningful Information From RSSI
String RSSI_Info(int RSSI_Value)
{
    switch (-RSSI_Value) // Inverted dBm Level ;)
    {
        {
    case 0 ... 30:
        return "Signal Very Strong";
        break;
    case 31 ... 50:
        return "Signal Excellent";
        break;
    case 51 ... 60:
        return "Signal Healthy";
        break;
    case 61 ... 70:
        return "Signal Very Good";
        break;
    case 71 ... 80:
        return "Signal Good";
        break;
    case 81 ... 90:
        return "Signal Poor - Try Moving Position";
        break;
    case 91 ... 100:
        return "Signal Very Low! - Move Position";
        break;
    default:
        return "No Signal :(";
        break;
        }
    }

} // RSSI_Info

// Wifi Information
void DisplayWiFiConfiguration()
{

    // RSSIInformation();

    PrintUnderline("Connection Details:");
    DBUGS.println("WiFi SSID \t " + String(WiFi.SSID()) + " (Wifi Station Mode)");

    if (local_IP.toString() == WiFi.localIP().toString().c_str())
    {
        DBUGS.printf("WiFi IP \t\t %s (Static)\n", WiFi.localIP().toString().c_str());
    }
    else
    {
        DBUGS.printf("WiFi IP \t\t %s (DHCP)\n", WiFi.localIP().toString().c_str());
    }

    DBUGS.printf("WiFi MASK \t %s\n", WiFi.subnetMask().toString().c_str());
    DBUGS.printf("WiFi GW \t\t %s\n", WiFi.gatewayIP().toString().c_str());
    DBUGS.printf("WiFi DNS 1 \t %s\n", WiFi.dnsIP().toString().c_str());
    DBUGS.printf("WiFi DNS 2 \t %s\n", WiFi.dnsIP(1).toString().c_str());
    DBUGS.println("WiFi MAC \t " + WiFi.macAddress());
    DBUGS.printf("WiFi Hostname \t %s\n", WiFi.getHostname());
    DBUGS.println("WiFi RSSI \t " + String(WiFi.RSSI()) + " dBm (" + RSSI_Info(WiFi.RSSI()) + ")");

    DBUGS.println("");
} // DisplayWiFiConfiguration

// Force Hostname
void ForceHostName(void)
{
    HostName = HostNameHeader;
    HostName.concat(WiFi.macAddress().substring(WiFi.macAddress().length() - 5, WiFi.macAddress().length()));
    HostName.replace(":", "");
    WiFi.setHostname(HostName.c_str());
    HostName = WiFi.getHostname();
} // ForceHostName

// Configure Static IP address (if not 0.0.0.0)
void InitialiseStaticIP()

{
    if (local_IP.toString() != "0.0.0.0")
    {
        // DBUGS.printf("WiFI Current IP\t \t %s\n", WiFi.localIP().toString().c_str());

        if (primaryDNS.toString() != "0.0.0.0")
        {
            if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
                ;
            {

                DBUGS.println("STA Static IP Failed to configure");
            }
        }
        else // Use Gateway IP for DNS 1
        {
            if (!WiFi.config(local_IP, gateway, subnet, gateway, secondaryDNS))
            {
                DBUGS.println("STA Static IP Failed to configure");
            }
        }
    }
} // InitialiseStaticIP



String getMacAddress()
{
    uint8_t baseMac[6];

    // Get MAC address for WiFi station
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);

    char baseMacChr[18] = { 0 };
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

    String macAddress = String(baseMacChr);



    return String(baseMacChr);

}


//===================================================================================================================
// -------------------------------------------------------------------
// Start Access Point
// Access point is used for wifi network selection
// -------------------------------------------------------------------
void startAP() {
    DBUGS.println("Starting AP");

    if (wifi_mode_is_sta()) {
        WiFi.disconnect(true);
    }

    WiFi.enableAP(true);


    SoftAP_IP.fromString(WiFiSoftAP_IP);
    SoftAP_Subnet.fromString(WiFiSubnet);

    //WiFi.softAPConfig(apIP, apIP, netMsk);

    WiFi.softAPConfig(SoftAP_IP, SoftAP_IP, SoftAP_Subnet);
    // Create Unique SSID e.g "emonESP_XXXXXX"

    String softAP_ssid_ID =
#ifdef ESP32
        String(softAP_ssid) + "_" + String((uint32_t)ESP.getEfuseMac());
#else
        String(softAP_ssid) + "_" + String(ESP.getChipId());
#endif

    int channel = (random(3) * 5) + 1;
    WiFi.softAP(node_name.c_str(), softAP_password, channel);

    // Setup the DNS server redirecting all the domains to the apIP
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    //dnsServer.start(DNS_PORT, "*", apIP);
    dnsServer.start(DNS_PORT, "*", SoftAP_IP);

    IPAddress myIP = WiFi.softAPIP();
    char tmpStr[40];
    sprintf(tmpStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
    DBUGS.print(F("AP IP Address: "));
    DBUGS.println(tmpStr);
    WiFi_APMode = true;
    WiFi_StaMode = false;
    WiFiState = true;
    ipaddress = tmpStr;

    apClients = 0;
}

// -------------------------------------------------------------------
// Start Client, attempt to connect to Wifi network
// -------------------------------------------------------------------
void startClient()
{
    WiFi.disconnect();
    delay(10);
    DBUGS.print(F("Connecting to SSID: "));
    DBUGS.println(WIFiSSID.c_str());
    //DBUGS.println(ssid);
    //DEBUG.print(F(" PSK:"));
    

    client_disconnects = 0;

    WiFi.begin(WIFiSSID.c_str(), WiFiPassword.c_str());
    //WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DBUGS.print(".");
        OnOff_Output(WIFI_LED, LED_OFF);
        OnOff_Output(FAULT_LED, LED_ON);
        WiFiState = false;


    }



    Client_IP.fromString(WiFiHost_IP);
    Client_Gateway.fromString(WiFiGateway);
    Client_Subnet.fromString(WiFiSubnet);
    Primary_DNS.fromString(DNS1);
    Secondary_DNS.fromString(DNS2);


    // Configuring static IP
    if (!WiFi.config(Client_IP, Client_Gateway, Client_Subnet, Primary_DNS, Secondary_DNS)) {
        DBUGS.println("Failed to configure Static IP");
    }
    else {
        DBUGS.println("Static IP Configured!");
    }


    WiFiState = true;
    WiFi_APMode = false;
    WiFi_StaMode = true;
    WiFi.hostname(node_name.c_str());
    OnOff_Output(WIFI_LED, LED_ON);
    OnOff_Output(FAULT_LED, LED_OFF);
    DBUGS.println("");
    
    DBUGS.println("Connected to " + String(WiFi.SSID()) + "\n");
    WiFi.enableSTA(true);

 

}

static void wifi_start()
{
    // 1) If no network configured start up access point
    DBUGVAR(ssid);
    if (WiFiAP_Mode) {

        //if (ssid == 0 || ssid == "SM-GE3222M(WiFi)")
        //{

            startAP();
            ForceHostName();

            // Wifi Information
           // DisplayWiFiConfiguration();
        //}


    }

    // 2) else try and connect to the configured network
    if (WIFiClient_Mode) {

        startClient();
        ForceHostName();

        // Wifi Information
       DisplayWiFiConfiguration();
    }
}


void WiFiEvent(WiFiEvent_t event)
{

#ifdef WIFI_LED
    wifiLedState = WIFI_LED_ON_STATE;
    OnOff_Output(WIFI_LED, wifiLedState);
#endif

    switch (event) {
    case SYSTEM_EVENT_WIFI_READY:
        DBUGS.println("WiFi interface ready");

        break;
    case SYSTEM_EVENT_SCAN_DONE:
        DBUGS.println("Completed scan for access points");
        break;
    case SYSTEM_EVENT_STA_START:
        DBUGS.println("WiFi client started");
        break;
    case SYSTEM_EVENT_STA_STOP:
        DBUGS.println("WiFi clients stopped");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        DBUGS.print("Connected to SSID: ");
        DBUGS.println(ssid);
        WiFiLCD.setCursor(1, 0);
        WiFiLCD.print("WiFi Connected");
        client_disconnects = 0;
        
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        DBUGS.println("Disconnected from WiFi access point");

        
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        DBUGS.println("Authentication mode of access point has changed");
        break;
    case SYSTEM_EVENT_STA_GOT_IP: {
        IPAddress myAddress = WiFi.localIP();
        char tmpStr[40];
        sprintf(tmpStr, "%d.%d.%d.%d", myAddress[0], myAddress[1], myAddress[2], myAddress[3]);
        ipaddress = tmpStr;
        DBUGS.print("SM-GE3222M(WiFi) IP: ");
        DBUGS.println(tmpStr);
        
        WiFiLCD.setCursor(2,0);
        WiFiLCD.print("SSID => " + String(WiFi.SSID()));
        WiFiLCD.setCursor(3,0);
        WiFiLCD.print("IP => " + String(tmpStr));

        // Copy the connected network and ipaddress to global strings for use in status request
        connected_network = ssid;

        // Clear any error state
        client_disconnects = 0;
        client_retry = false;
    }
                                break;
    case SYSTEM_EVENT_STA_LOST_IP:
        DBUGS.println("Lost IP address and IP address is reset to 0");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        DBUGS.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        DBUGS.println("WiFi Protected Setup (WPS): failed in enrollee mode");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        DBUGS.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        DBUGS.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
        break;
    case SYSTEM_EVENT_AP_START:
        DBUGS.println("WiFi access point started");
        break;
    case SYSTEM_EVENT_AP_STOP:
        DBUGS.println("WiFi access point stopped");
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        DBUGS.println("Client connected");
        apClients++;
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        DBUGS.println("Client disconnected");
        apClients--;
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        DBUGS.println("Assigned IP address to client");
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        DBUGS.println("Received probe request");
        break;
    default:
        break;
    }
}



void wifi_setup() {
#ifdef WIFI_LED

    OnOff_Output(WIFI_LED, wifiLedState);
#endif

    randomSeed(analogRead(0));


#ifdef ESP32
    WiFi.onEvent(WiFiEvent);

#else
    static auto _onStationModeConnected = WiFi.onStationModeConnected([](const WiFiEventStationModeConnected& event) {
        DBUGF("Connected to %s", event.ssid.c_str());
        });
    static auto _onStationModeGotIP = WiFi.onStationModeGotIP(wifi_onStationModeGotIP);
    static auto _onStationModeDisconnected = WiFi.onStationModeDisconnected(wifi_onStationModeDisconnected);
    static auto _onSoftAPModeStationConnected = WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected& event) {
        apClients++;
        });
    static auto _onSoftAPModeStationDisconnected = WiFi.onSoftAPModeStationDisconnected([](const WiFiEventSoftAPModeStationDisconnected& event) {
        apClients--;
        });
#endif


   

    //WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    //WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    

    wifi_start();


    if (MDNS.begin(esp_hostname)) {
        DBUGS.println(esp_hostname);
        MDNS.addService("http", "tcp", 80);
    }

    client_retry_time = millis();

}


void wifi_loop()
{

  // Profile_Start(wifi_loop);

    bool isClient = wifi_mode_is_sta();
    bool isClientOnly = wifi_mode_is_sta_only();
    bool isAp = wifi_mode_is_ap();
    bool isApOnly = wifi_mode_is_ap_only();


    unsigned long currentMillis = millis();
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
        DBUGS.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        previousMillis = currentMillis;
    }




#ifdef WIFI_LED
    if ((isApOnly || !WiFi.isConnected()) && millis() > wifiLedTimeOut) {
        wifiLedState = !wifiLedState;
        OnOff_Output(WIFI_LED, wifiLedState);

        if (wifiLedState) {
            wifiLedTimeOut = millis() + WIFI_LED_ON_TIME;
        }
        else {
            int ledTime = isApOnly ? (0 == apClients ? WIFI_LED_AP_TIME : WIFI_LED_AP_CONNECTED_TIME) : WIFI_LED_STA_CONNECTING_TIME;
            wifiLedTimeOut = millis() + ledTime;
        }
    }
    if ((isClientOnly || WiFi.isConnected()) && millis() > wifiLedTimeOut) {
        wifiLedState = !wifiLedState;
        OnOff_Output(WIFI_LED, wifiLedState);

        if (wifiLedState) {
            wifiLedTimeOut = millis() + WIFI_LED_ON_TIME;
        }
        else {
            int ledTime = WIFI_LED_STA_CONNECTED_TIME;
            wifiLedTimeOut = millis() + ledTime;
        }
    }
#endif

#if defined(WIFI_LED) && WIFI_BUTTON == WIFI_LED
    digitalWrite(WIFI_BUTTON, HIGH);
    pinMode(WIFI_BUTTON, INPUT_PULLUP);
#endif

    // Pressing the WIFI_BUTTON for 5 seconds will turn on AP mode, 10 seconds will factory reset
    int button = digitalRead(WIFI_BUTTON);

#if defined(WIFI_LED) && WIFI_BUTTON == WIFI_LED
    pinMode(WIFI_BUTTON, OUTPUT);
    digitalWrite(WIFI_LED, wifiLedState);
#endif

    //DBUGF("%lu %d %d", millis() - wifiButtonTimeOut, button, wifiButtonState);
    if (wifiButtonState != button)
    {
        wifiButtonState = button;
        if (HIGH == button) {
            DBUGS.println("Button pressed");
            wifiButtonTimeOut = millis();
            apMessage = false;
        }
        else {
            DBUGS.println("Button released");
            if (millis() > wifiButtonTimeOut + WIFI_BUTTON_AP_TIMEOUT) {
                wifi_turn_on_ap();
            }
        }
    }


    // Manage state while connecting
    if (startAPonWifiDisconnect) {
        while (wifi_mode_is_sta_only() && !wifi_mode_is_ap_only() && !WiFi.isConnected())
        {
            client_disconnects++; //set to 0 when connection to AP is made

            // If we have failed to connect 3 times, turn on the AP
            if (client_disconnects > 2) {
                DBUGS.println("Start AP if WiFi can not reconnect to AP");
                startAP();
                client_retry = true;
                //client_retry_time = millis() + WIFI_CLIENT_RETRY_TIMEOUT;
                client_disconnects = 0;
            }
            else {
                // wait 10 seconds and retry
#ifdef ENABLE_WDT
        // so watchdog (hard coded to 5 seconds) is not triggered by delay
                if (WIFI_CLIENT_DISCONNECT_RETRY >= 5000) {
                    int disconnect_retry;
                    int dr_div;
                    int i = 0;
                    dr_div = WIFI_CLIENT_DISCONNECT_RETRY / 1000;
                    disconnect_retry = WIFI_CLIENT_DISCONNECT_RETRY / dr_div;
                    DBUGS.print("disconnect retry time: ");
                    DBUGS.println(disconnect_retry);
                    while (i < dr_div) {
                        delay(disconnect_retry);
                        feedLoopWDT();
                        i++;
                    }
                }
                else {
#endif
                    delay(WIFI_CLIENT_DISCONNECT_RETRY);
#ifdef ENABLE_WDT
                }
#endif
                wifi_restart();
            }
#ifdef ENABLE_WDT
            feedLoopWDT();
#endif
        }
    }





    // flash the LED according to what state wifi is in
    // if AP mode & disconnected - blink every 2 seconds
    // if AP mode & someone is connected - blink fast
    // if Client mode - slow blink every 4 seconds




   //Profile_End(wifi_loop, 20);
}


void wifi_scan() {
    int n = WiFi.scanNetworks();
    DBUGS.print(n);
    DBUGS.println(" networks found");
    st = "";
    rssi = "";
    for (int i = 0; i < n; ++i) {
        st += "\"" + WiFi.SSID(i) + "\"";
        rssi += "\"" + String(WiFi.RSSI(i)) + "\"";
        if (i < n - 1)
            st += ",";
        if (i < n - 1)
            rssi += ",";
    }
}


void wifi_restart() {
    wifi_disconnect();
    wifi_start();
}

void wifi_disconnect() {
    wifi_turn_off_ap();
    if (wifi_mode_is_sta()) {
        WiFi.disconnect(true);
    }
}

void wifi_turn_off_ap()
{
    if (wifi_mode_is_ap())
    {
        WiFi.softAPdisconnect(true);
        dnsServer.stop();
    }
}

void wifi_turn_on_ap()
{
    DBUGF("wifi_turn_on_ap %d", WiFi.getMode());
    if (!wifi_mode_is_ap()) {
        startAP();
    }
}

bool wifi_client_connected()
{
    return WiFi.isConnected() && (WIFI_STA == (WiFi.getMode() & WIFI_STA));
}

