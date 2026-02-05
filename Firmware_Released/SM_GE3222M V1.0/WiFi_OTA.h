// WiFi_OTA.h

#ifndef _WIFI_OTA_h
#define _WIFI_OTA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern void PrintUnderline(String sText);


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

#include <AsyncTCP.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESPmDNS.h>



#define wifi_is_client_configured()   (WiFi.SSID() != "")

    // Wifi mode
#define wifi_mode_is_sta()            (WIFI_STA == (WiFi.getMode() & WIFI_STA))
#define wifi_mode_is_sta_only()       (WIFI_STA == WiFi.getMode())
#define wifi_mode_is_ap()             (WIFI_AP == (WiFi.getMode() & WIFI_AP))

// Performing a scan enables STA so we end up in AP+STA mode so treat AP+STA with no
// ssid set as AP only
#define wifi_mode_is_ap_only()        ((WIFI_AP == WiFi.getMode()) || \
                                       (WIFI_AP_STA == WiFi.getMode() && !wifi_is_client_configured()))

// Global config varables
extern const String node_type;
extern const String node_description;
extern uint16_t node_id;
extern String node_name;
extern String node_describe;
extern char id[41];

extern boolean Config_State, WiFiAP_Mode,WIFiClient_Mode;
extern String WiFiSoftAP_IP, SoftAP_SSID, SoftAP_Password, WIFi_State;
extern String WIFiSSID, WiFiPassword;
extern String WiFiSoftAP_IP,WiFiHost_IP, WiFiGateway, WiFiSubnet, DNS1, DNS2;
extern const char* ssid; // Change this to your WiFi SSID
extern const char* password; // Change this to your WiFi password

extern const byte DNS_PORT;

// Access Point SSID, password & IP address. SSID will be softAP_ssid + chipID to make SSID unique
extern const char* softAP_ssid;
extern const char* softAP_password ;

extern int apClients;

extern bool startAPonWifiDisconnect;

// hostname for mDNS. Should work at least on windows. Try http://emonesp.local
extern const char* esp_hostname;

// Last discovered WiFi access points
extern String st;
extern String rssi;


// Wifi Network Strings
extern String connected_network;
extern String ipaddress;

extern int client_disconnects;
extern bool client_retry;
extern unsigned long client_retry_time;

extern boolean  WiFiState;
extern boolean  WiFi_APMode;
extern boolean  WiFi_StaMode;

extern unsigned long previousMillis;
extern unsigned long interval;


#ifndef WIFI_LED
#define WIFI_LED 2
#endif


#ifndef FAULT_LED
#define FAULT_LED 4
#endif

#ifdef WIFI_LED


#ifndef WIFI_LED_ON_STATE
#define WIFI_LED_ON_STATE LOW
#endif

//the time the LED actually stays on
#ifndef WIFI_LED_ON_TIME
#define WIFI_LED_ON_TIME 50
#endif


//times the LED is off...
#ifndef WIFI_LED_AP_TIME
#define WIFI_LED_AP_TIME 2000
#endif

#ifndef WIFI_LED_AP_CONNECTED_TIME
#define WIFI_LED_AP_CONNECTED_TIME 1000
#endif

#ifndef WIFI_LED_STA_CONNECTING_TIME
#define WIFI_LED_STA_CONNECTING_TIME 500
#endif

#ifndef WIFI_LED_STA_CONNECTED_TIME
#define WIFI_LED_STA_CONNECTED_TIME 4000
#endif



#ifndef WIFI_LED_AP_TIME
#define WIFI_LED_AP_TIME 1000
#endif

#ifndef WIFI_LED_AP_CONNECTED_TIME
#define WIFI_LED_AP_CONNECTED_TIME 100
#endif

#ifndef WIFI_LED_STA_CONNECTING_TIME
#define WIFI_LED_STA_CONNECTING_TIME 500
#endif

#endif

#ifndef WIFI_BUTTON
#define WIFI_BUTTON 32
#endif

#ifndef WIFI_BUTTON_AP_TIMEOUT
#define WIFI_BUTTON_AP_TIMEOUT              (5 * 1000)
#endif

#ifndef WIFI_BUTTON_FACTORY_RESET_TIMEOUT
#define WIFI_BUTTON_FACTORY_RESET_TIMEOUT   (10 * 1000)
#endif

#ifndef WIFI_CLIENT_RETRY_TIMEOUT
#define WIFI_CLIENT_RETRY_TIMEOUT (5 * 60 * 1000)
#endif

#ifndef LED_ON
#define LED_ON 0
#endif

#ifndef LED_OFF
#define LED_OFF 1
#endif



extern int wifiButtonState;
extern unsigned long wifiButtonTimeOut;
extern bool apMessage;

// WiFi. Force Disable for Testing.  !!!!! BEWARE Outside of Local Developmet Do NOT Disable as OTA will NOT work !!!!!
extern const boolean DisableWiFi; // Force Disable WiFi for Local USB Development and Testing Only.  You can leave SSID/IP etc populated.

// Set your Static IP address and Gateway - Alternatively leave at (0, 0, 0, 0)
extern IPAddress local_IP;     // Leave at (0, 0, 0, 0) if DHCP required
extern IPAddress gateway;      // Set to your Router IP = Gateway
extern IPAddress subnet; // Should not need to change subnet from default (255, 255, 255, 0)

// Optional DNS Defaults.
extern IPAddress primaryDNS;   // Defaults to your above Gateway IP if left as (0, 0, 0, 0)
extern IPAddress secondaryDNS; // For Google Public DNS use for Primary or Secondary (8,8,8,8) and/or (8,8,4,4)

// WiFi Other

extern String HostNameHeader; // Hostname Prefix
extern String HostName;                          // Hostname
// String RSSILevel;                         // WiFi RSSI Level Information
extern const uint32_t connectTimeoutMs; // WiFi connect timeout per AP. Increase when connecting takes longer.

// App
extern String AppVersion;
extern String AppAcronym;
extern String AppName;
extern int lcdColumns;
extern int lcdRows;


// Last discovered WiFi access points
extern String st;
extern String rssi;

// Network state
extern String ipaddress;

// mDNS hostname
extern const char* esp_hostname;

extern void wifi_setup();
extern void wifi_loop();
extern void wifi_scan();

extern void wifi_restart();
extern void wifi_disconnect();

extern void wifi_turn_off_ap();
extern void wifi_turn_on_ap();
extern bool wifi_client_connected();

// NTP Time

//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
// United Kingdom (London, Belfast)
// TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
// TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
// Timezone UK(BST, GMT);

extern String RSSI_Info(int RSSI_Value);
extern void DisplayWiFiConfiguration();
extern void ForceHostName(void);
extern void InitialiseStaticIP();

extern void OnOff_Output(uint8_t SelectOutput, uint8_t OnOff);
extern String getMacAddress();
extern void led_wififlash(int ton, int toff);
extern void led_runflash(int ton, int toff);
extern void led_faultflash(uint8_t SelectOutput, int ton, int toff);
extern void led_ModbusFlash(int ton, int toff);
extern void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
extern void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
extern void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

#endif

