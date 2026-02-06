#include <SPI.h>
#include <Ethernet.h>

//Import Settings and Parameters
extern boolean Config_State, WiFiAP_Mode, WIFi_Mode, WIFiClient_Mode, ETH_Mode , MDBUS_Mode ;
extern int SerialBaud, ETH_TCPPort, WiFiServerPort;
extern int   MDBUS_BAUD, MDBus_TCPPort;
extern String WIFiSSID, WiFiPassword, WIFi_State;
extern String Server_IP, ETH_HostIP, MDBus_TCPHostIP, WiFiHost_IP, WiFiGateway, WiFiSubnet, DNS1, DNS2;
extern String WiFiSoftAP_IP, SoftAP_SSID, SoftAP_Password;
extern uint8_t ModSlave_ID;
extern String Config_Mode, COM_MODE;
extern String Cal_Page;
extern float  PublishInterval;
extern unsigned long systemRestartTime;
extern unsigned int MMode0_Reg, MMode1_Reg, PMPGA_Reg, PStartTh_Reg, QStartTh_Reg, SStartTh_Reg, PPhaseTh_Reg, QPhaseTh_Reg, SPhaseTh_Reg;


//Define Ethernet Settings and Parameters
#define ETHERNET_MAC            "64:B7:08:58:DC:28" // Ethernet MAC address (have to be unique between devices in the same network)
#define ETHERNET_IP             ETH_HostIP       // IP address of RoomHub when on Ethernet connection

//#define ETHERNET_RESET_PIN      13      // ESP32 pin where reset pin from W5500 is connected
#define ETHERNET_CS_PIN         14       // ESP32 pin where CS pin from W5500 is connected

#define TCP_HOSTNAME           Server_IP
//#define TCP_HOSTNAME           "169.254.33.234"
#define TCP_PORT               ETH_TCPPort

#define TCP_PUBLISH_INTERVAL_MS   PublishInterval


/*
byte Ethernet_mac[] = {
  0x64, 0xB7,0x08, 0x58, 0xDC, 0x28
};
IPAddress Ethernet_IP(169, 254, 33, 20);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer *ETHServer;


//Wiz W5500 reset function

void ethernetWizReset(const uint8_t resetPin) {
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
    delay(250);
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    delay(350);
}


*/

IPAddress ipAddress;
EthernetServer *ETHServer;
EthernetClient client;

uint32_t lastTcpPublishTime = 0;
uint8_t buffer[512];

void macCharArrayToBytes(const char* str, byte* bytes) {
    for (int i = 0; i < 6; i++) {
        bytes[i] = strtoul(str, NULL, 16);
        str = strchr(str, ':');
        if (str == NULL || *str == '\0') {
            break;
        }
        str++;
    }
}


void connectEthernet() {
    delay(500);
    byte* mac = new byte[6];
    macCharArrayToBytes(ETHERNET_MAC, mac);
    ipAddress.fromString(ETHERNET_IP);

    ETHServer = new EthernetServer(8088); //Instantiate the web server. Use port 80
    Ethernet.init(ETHERNET_CS_PIN);
    //ethernetWizReset(ETHERNET_RESET_PIN);

    DBUGS.println("Starting ETHERNET connection...");
    Ethernet.begin(mac, ipAddress);
    delay(200);

    //ETHServer->begin(); //Start the server
    DBUGS.print("Ethernet IP : ");
    DBUGS.println(Ethernet.localIP());
    
}

void connectToServer() {
    DBUGS.println("Connecting to TCP server...");
    while (!client.connect(TCP_HOSTNAME.c_str(), TCP_PORT)) {   //TCP_HOSTNAME.c_str()
        DBUGS.println("Connection failed. Reconnecting...");
        delay(1000);
    }
    DBUGS.println("Connected to TCP server");
    client.println("/SYNC/");
    DBUGS.println("/SYNC/");
    delay(50);

}


void Setup_Ethernet() {

    connectEthernet();
    connectToServer();
    delay(500);


    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        DBUGS.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true) {
            delay(1); // do nothing, no point running without Ethernet hardware
        }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
        DBUGS.println("Ethernet cable is not connected.");
    }

    // print initialized variables
    DBUGS.println("====================== Firmware Settings -> Before reading config: ======================");
    DBUGS.println();
    DBUGS.println("Config_State = " + String(Config_State));
    DBUGS.println("COM MODE = " + COM_MODE);
    DBUGS.println("Serial Baud = " + String(SerialBaud));
    DBUGS.println("WiFi ServerPort = " + String(WiFiServerPort));
    DBUGS.println("Publish Interval = " + String(PublishInterval / 1000) + " sec");

    if (ETH_Mode) {
        DBUGS.println("Ethernet MODE = ON"); //+ String(WIFi_Mode));
    }
    else {

        DBUGS.println("Ethernet MODE = OFF"); //+ String(WIFi_Mode));
    }

    if (MDBUS_Mode) {
        DBUGS.println("MODBUS MODE = ON"); //+ String(WIFi_Mode));
    }
    else {

        DBUGS.println("MODBUS MODE = OFF"); //+ String(WIFi_Mode));
    }

    DBUGS.println("MDBUS_BAUD = " + String(MDBUS_BAUD));
    DBUGS.println("MODBUS TCPPort = " + String(MDBus_TCPPort));
    DBUGS.println("Config Mode = " + String(Config_Mode));

    if (WIFi_Mode) {
        DBUGS.println("WiFi Mode = ON"); //+ String(WIFi_Mode));
    }
    else {

        DBUGS.println("WiFi Mode = OFF"); //+ String(WIFi_Mode));
    }


    if (WIFiClient_Mode) {
        DBUGS.println("WiFi Client Mode = ON"); //+ String(WIFi_Mode));
    }
    else {

        DBUGS.println("WiFi Client Mode = OFF"); //+ String(WIFi_Mode));
    }
    //DBUGS.println("WiFi Client Mode = " + String(WIFiClient_Mode));
    DBUGS.println("WIFi_SSID = " + WIFiSSID);
    DBUGS.println("WiFi Password = " + WiFiPassword);
    DBUGS.println("Server_IP = " + Server_IP);
    DBUGS.println("ETH_HostIP = " + ETH_HostIP);
    DBUGS.println("ETH_TCPPort = " + String(ETH_TCPPort));
    DBUGS.println("MODBUS_TCPHostIP = " + MDBus_TCPHostIP);
    DBUGS.println("WiFi Host_IP = " + WiFiHost_IP);
    DBUGS.println("WiFi Gateway = " + WiFiGateway);
    DBUGS.println("WiFi Subnet Mask = " + WiFiSubnet);
    DBUGS.println("WiFi DNS1 = " + DNS1);
    DBUGS.println("WiFi DNS2 = " + DNS2);

    if (WiFiAP_Mode) {
        DBUGS.println("WiFi AP Mode = ON"); //+ String(WIFi_Mode));
    }
    else {

        DBUGS.println("WiFi AP Mode = OFF"); //+ String(WIFi_Mode));
    }
    //DBUGS.println("WiFi AP Mode = " + String(WiFiAP_Mode));
    DBUGS.println("WiFi SoftAP IP = " + WiFiSoftAP_IP);
    DBUGS.println("WiFi SoftAP SSID = " + SoftAP_SSID);
    DBUGS.println("WiFi SoftAP Password = " + SoftAP_Password);
    DBUGS.println("MODBUS Slave_ID = " + String(ModSlave_ID));
    DBUGS.println();
    DBUGS.println("================================================================");


}


void Ethernetloop() {
    // listen for incoming clients
    //client = ETHServer->available();

    char request[30]; // char array as buffer for reading from Serial 

    if (Settings.Bit.ReadDataInProgress != 0) {
        return;
    }


    //while (1) {

		if (!client.connected()) {
			DBUGS.println("Client disconnected...");
			Settings.Bit.PcUartRequest = 0;
			connectToServer();

		}


	while (client.connected()) {
		if (client.available() > 0) {


			int number = client.readBytesUntil('\n', request, sizeof(request)); //Read input and length
			request[number] = '\0'; //Terminate the string by adding '\0' at its end

			if (strcmp(request, "Code_NewReq") == 0) SendDataToServer();
			if (strcmp(request, "Code_Cal1") == 0) SendCallibration1_ToServer();

            if (strcmp(request, "Cal1_RST") == 0) {
                SendCallibration1_ToServer();
                systemRestartTime = millis() + 1000;
            }

			if (strcmp(request, "Code_Cal2") == 0) SendCallibration2_ToServer();

            if (strcmp(request, "Cal2_RST") == 0) {
                SendCallibration2_ToServer();
                systemRestartTime = millis() + 1000;
            }


			if (strcmp(request, "Code_Cal3") == 0) SendCallibration3_ToServer();

            if (strcmp(request, "Cal3_RST") == 0) {
                SendCallibration3_ToServer();
                systemRestartTime = millis() + 1000;
            }

			if (strcmp(request, "Code_Cal4") == 0) SendCallibration4_ToServer();

            if (strcmp(request, "Cal4_RST") == 0) {
                SendCallibration4_ToServer();
                systemRestartTime = millis() + 1000;
            }

            if (strcmp(request, "ConfigSave") == 0) {
                UpdateConfigurations();

            }

            if (strcmp(request, "ConfigRst") == 0) {
                UpdateConfigurations();
                systemRestartTime = millis() + 1000;
            }

			if (strcmp(request, "Reboot") == 0) {

				client.stop();
				DBUGS.println("client disconnected");
				wifi_disconnect();
				esp_restart();
				break;
			}
			//if (strcmp(request, "Code_MDBusReq") == 0) Send_MDBusDataToServer();


		   // if (str == "TCP_Req") {


/*
					if (c == '\n') {
						// you're starting a new line
						currentLineIsBlank = true;
					}
					else if (c != '\r') {
						// you've gotten a character on the current line
						currentLineIsBlank = false;
					}
*/
// close the connection:
			client.stop();
			DBUGS.println("client disconnected");
		}
		//}
		// give the web browser time to receive the data


	}


            

        Settings.Bit.PcUartRequest = 0;

         //Do we need to restart the system?
        if (systemRestartTime > 0) {
            systemRestartTime = 0;
            wifi_disconnect();
            esp_restart();
        }
    
    
}


void SendDataToServer() {

    uint32_t now = millis();
    Settings.Bit.PcUartRequest = 1;

    if (now - lastTcpPublishTime >= TCP_PUBLISH_INTERVAL_MS) {
        Load_Power_Registers();
        delay(10);
        ReadPCBTemperature();
        delay(10);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        client.print("AE1"); client.println(PhaA_ForwardActiveEnergy);
        //delay(2);
        client.print("AE2"); client.println(PhaA_ReverseActiveEnergy);
        //delay(2);
        client.print("AE3"); client.println(PhaA_ForwardReactiveEnergy);
        //delay(2);
        client.print("AE4"); client.println(PhaA_ReverseReactiveEnergy);
        //delay(2);
        client.print("AE5"); client.println(PhaA_ApparentEnergy);
        //delay(2);

        client.print("BE1"); client.println(PhaB_ForwardActiveEnergy);
        //delay(2);
        client.print("BE2"); client.println(PhaB_ReverseActiveEnergy);
        //delay(2);
        client.print("BE3"); client.println(PhaB_ForwardReactiveEnergy);
       // delay(2);
        client.print("BE4"); client.println(PhaB_ReverseReactiveEnergy);
        //delay(2);
        client.print("BE5"); client.println(PhaB_ApparentEnergy);
        //delay(2);

        client.print("CE1"); client.println(PhaC_ForwardActiveEnergy);
        //delay(2);
        client.print("CE2"); client.println(PhaC_ReverseActiveEnergy);
        //delay(2);
        client.print("CE3"); client.println(PhaC_ForwardReactiveEnergy);
        //delay(2);
        client.print("CE4"); client.println(PhaC_ReverseReactiveEnergy);
        //delay(2);
        client.print("CE5"); client.println(PhaC_ApparentEnergy);
        //delay(2);

        client.print("TE1"); client.println(TotalForwardActiveEnergy);
        //delay(2);
        client.print("TE2"); client.println(TotalReverseActiveEnergy);
        //delay(2);
        client.print("TE3"); client.println(TotalForwardReactiveEnergy);
        //delay(2);
        client.print("TE4"); client.println(TotalReverseReactiveEnergy);
        //delay(2);
        client.print("TE5"); client.println(Total_ArithmeticSum_ApparentEnergy);
        //delay(2);
        client.print("TE6"); client.println(VectorSum_TotalApparentEnergy);
        //delay(2);
        //===========================================================================
        //Send Power Data

        client.print("AP1"); client.println(PhaA_ActivePower);
        //delay(2);
        client.print("AP2"); client.println(PhaA_ReactivePower);
        //delay(2);
        client.print("AP3"); client.println(PhaA_apparentpower);
        //delay(2);
        client.print("AP4"); client.println(PhaA_activefundamentalpower);
        //delay(2);
        client.print("AP5"); client.println(PhaA_powerfactor);
        //delay(2);

        client.print("AP6"); client.println(PhaA_activeharmonicpower);
        //delay(2);
        client.print("AP7"); client.println(PhaA_voltageRMS);
        //delay(2);
        client.print("AP8"); client.println(PhaA_currentRMS);
        //delay(2);
        client.print("AP9"); client.println(PhaA_meanphaseangle);
        //delay(2);
        client.print("AP10"); client.println(PhaA_voltagephaseangle);
        //delay(2);

        client.print("BP1"); client.println(PhaB_ActivePower);
        //delay(2);
        client.print("BP2"); client.println(PhaB_ReactivePower);
        //delay(2);
        client.print("BP3"); client.println(PhaB_apparentpower);
        //delay(2);
        client.print("BP4"); client.println(PhaB_activefundamentalpower);
        //delay(2);
        client.print("BP5"); client.println(PhaB_powerfactor);
        //delay(2);
        client.print("BP6"); client.println(PhaB_activeharmonicpower);
        //delay(2);
        client.print("BP7"); client.println(PhaB_voltageRMS);
        //delay(2);
        client.print("BP8"); client.println(PhaB_currentRMS);
        //delay(2);
        client.print("BP9"); client.println(PhaB_meanphaseangle);
        //delay(2);
        client.print("BP10"); client.println(PhaB_voltagephaseangle);
        //delay(2);

        client.print("CP1"); client.println(PhaC_ActivePower);
        //delay(2);
        client.print("CP2"); client.println(PhaC_ReactivePower);
        //delay(2);
        client.print("CP3"); client.println(PhaC_apparentpower);
        //delay(2);
        client.print("CP4"); client.println(PhaC_activefundamentalpower);
        //delay(2);
        client.print("CP5"); client.println(PhaC_powerfactor);
        //delay(2);

        client.print("CP6"); client.println(PhaC_activeharmonicpower);
        //delay(2);
        client.print("CP7"); client.println(PhaC_voltageRMS);
        //delay(2);
        client.print("CP8"); client.println(PhaC_currentRMS);
        //delay(2);
        client.print("CP9"); client.println(PhaC_meanphaseangle);
        //delay(2);
        client.print("CP10"); client.println(PhaC_voltagephaseangle);
        //delay(2);

        client.print("TP1"); client.println(Total_PhasesumActivePower);
        //delay(2);
        client.print("TP2"); client.println(Total_PhasesumReactivePower);
        //delay(2);
        client.print("TP3"); client.println(Total_ArithmeticSumapparentpower);
        //delay(2);
        client.print("TP4"); client.println(VectorSum_TotalApparentPower);
        //delay(2);
        client.print("TP5"); client.println(Totalpowerfactor);
        //delay(2);
        client.print("TP6"); client.println(Totalactivefundamentalpower);
        //delay(2);
        client.print("TP7"); client.println(Totalactiveharmonicpower);
        //delay(2);
        client.print("TP8"); client.println(Frequency);
        //delay(2);
        client.print("TP9"); client.println(LineCurrentCTN);
        //delay(2);
        client.print("TP10"); client.println(LineCurrentTotal);
        //delay(2);
        client.print("DP1"); client.println(Hum);
        //delay(2);
        client.print("DP2"); client.println(Temp_C);
        //delay(2);
        client.print("DP3"); client.println(Temp_F);
        //delay(2);
        client.print("MD1"); client.println(ModSlave_ID);
        //delay(2);
        client.print("MD2"); client.println(MDBus_TCPPort);
        //delay(2);
        client.print("MD3"); client.println(MDBus_TCPHostIP);
        //delay(2);
        client.print("MD4"); client.println(MDBUS_BAUD);

        client.print("ST1"); client.println(SerialBaud);
        client.print("ST2"); client.println(WIFiSSID);
        client.print("ST3"); client.println(WiFiPassword);
        client.print("ST4"); client.println(WiFiHost_IP);
        client.print("ST5"); client.println(WiFiServerPort);
        client.print("ST6"); client.println(WiFiGateway);
        client.print("ST7"); client.println(WiFiSubnet);
        client.print("ST8"); client.println(DNS1);
        client.print("ST9"); client.println(DNS2);
        client.print("ST10"); client.println(ETH_HostIP);
        client.print("ST11"); client.println(ETH_TCPPort);
        client.print("ST12"); client.println(ModSlave_ID);
        client.print("ST13"); client.println(MDBus_TCPPort);
        client.print("ST14"); client.println(MDBus_TCPHostIP);
        client.print("ST15"); client.println(MDBUS_BAUD);



    }

}

void UpdateConfigurations() {

    JsonDocument doc;

    DeserializationError Error = deserializeJson(doc, client);

    if (Error) {
        Serial.println("Invalid JSON Object");
        return;
    }

    Serial.println("Valid JSON Object Received");

  
   
    
    const char* _Cal_Page = doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    const char* _COM_MODE = doc["COM_MODE"];
    COM_MODE = String(_COM_MODE);
    Config_State = doc["Config_State"];
    SerialBaud = doc["SerialBaud"];
    WIFi_Mode = doc["WIFi_Mode"];
    const char* _WIFi_State = doc["WIFi_State"];
    WIFi_State = String(_WIFi_State);
    WIFiClient_Mode = doc["WIFiClient_Mode"];
    const char* _WIFi_SSID = doc["WIFiSSID"];
    WIFiSSID = String(_WIFi_SSID);
    const char* _WiFi_Password = doc["WiFiPassword"];
    WiFiPassword = String(_WiFi_Password);
    const char* _ServerIP = doc["Server_IP"];
    Server_IP = String(_ServerIP);
    const char* WiFi_Host_IP = doc["WiFiHost_IP"];
    WiFiHost_IP = String(WiFi_Host_IP);
    WiFiServerPort = doc["WiFiServerPort"];
    const char* _WiFi_Gateway = doc["WiFiGateway"];
    WiFiGateway = String(_WiFi_Gateway);
    const char* WiFi_Subnet = doc["WiFiSubnet"];
    WiFiSubnet = String(WiFi_Subnet);
    const char* DNS_1 = doc["DNS1"];
    DNS1 = String(DNS_1);
    const char* DNS_2 = doc["DNS2"];
    DNS2 = String(DNS_2);
    WiFiAP_Mode = doc["WiFiAP_Mode"];
    const char* _WiFiSoftAP_IP = doc["WiFiSoftAP_IP"];
    WiFiSoftAP_IP = String(_WiFiSoftAP_IP);
    const char* _SoftAP_SSID = doc["SoftAP_SSID"];
    SoftAP_SSID = String(_SoftAP_SSID);
    const char* _SoftAP_Password = doc["SoftAP_Password"];
    SoftAP_Password = String(_SoftAP_Password);
    ETH_Mode = doc["ETH_Mode"];
    const char* _ETHHostIP = doc["ETH_HostIP"];
    ETH_HostIP = String(_ETHHostIP);
    ETH_TCPPort = doc["ETH_TCPPort"];
    MDBUS_Mode = doc["MDBUS_Mode"];
    ModSlave_ID = doc["MDBUS_ID"];
    const char* _MDBusTCPHostIP = doc["MDBus_TCPHostIP"];
    MDBus_TCPHostIP = String(_MDBusTCPHostIP);
    MDBus_TCPPort = doc["MDBus_TCPPort"];
    MDBUS_BAUD = doc["MDBUS_BAUD"];
    PublishInterval = doc["Publish_Interval"];
    const char* _ConfigMode = doc["Config_Mode"];
    Config_Mode = String(_ConfigMode);


    JSONVar settings;

    settings["Page_Name"] = Cal_Page;
    settings["COM_MODE"] = COM_MODE;
    settings["Config_State"] = Config_State;
    settings["SerialBaud"] = SerialBaud;
    settings["WIFi_Mode"] = WIFi_Mode;
    settings["WIFi_State"] = WIFi_State;
    settings["WIFiClient_Mode"] = WIFiClient_Mode;
    settings["WIFiSSID"] = WIFiSSID;
    settings["WiFiPassword"] = WiFiPassword;
    settings["Server_IP"] = Server_IP;
    settings["WiFiHost_IP"] = WiFiHost_IP;
    settings["WiFiServerPort"] = WiFiServerPort;
    settings["WiFiGateway"] = WiFiGateway;
    settings["WiFiSubnet"] = WiFiSubnet;
    settings["DNS1"] = DNS1;
    settings["DNS2"] = DNS2;
    settings["WiFiAP_Mode"] = WiFiAP_Mode;
    settings["WiFiSoftAP_IP"] = WiFiSoftAP_IP;
    settings["SoftAP_SSID"] = SoftAP_SSID;
    settings["SoftAP_Password"] = SoftAP_Password;
    settings["ETH_Mode"] = ETH_Mode;
    settings["ETH_HostIP"] = ETH_HostIP;
    settings["ETH_TCPPort"] = ETH_TCPPort;
    settings["MDBUS_Mode"] = MDBUS_Mode;
    settings["MDBUS_ID"] = ModSlave_ID;
    settings["MDBus_TCPHostIP"] = MDBus_TCPHostIP;
    settings["MDBus_TCPPort"] = MDBus_TCPPort;
    settings["MDBUS_BAUD"] = MDBUS_BAUD;
    settings["Publish_Interval"] = PublishInterval;
    settings["Config_Mode"] = "Saved";

    String jsonString = JSON.stringify(settings);

    if (saveConfig(config_filename)) {
        Serial.println("Setup -> Config file saved");
        //serializeJson(doc, client);
        
        //if (COM_MODE == "TCP") {
            client.println(jsonString);
        //}
            
        
        
        //if (COM_MODE == "USB") {
        DBUGS.println(jsonString);
        //}
        

    }

    if (COM_MODE == "TCP") {
        //Display Saved Configuration Settings
        DBUGS.println("====================== Display Saved Configuration Settings ======================");
        DBUGS.println();
        DBUGS.println("Config_State = " + String(Config_State));
        DBUGS.println("COM MODE = " + COM_MODE);
        DBUGS.println("Serial Baud = " + String(SerialBaud));
        DBUGS.println("WiFi ServerPort = " + String(WiFiServerPort));
        DBUGS.println("Publish Interval = " + String(PublishInterval / 1000) + " sec");

        if (ETH_Mode) {
            DBUGS.println("Ethernet MODE = ON"); //+ String(WIFi_Mode));
        }
        else {

            DBUGS.println("Ethernet MODE = OFF"); //+ String(WIFi_Mode));
        }

        if (MDBUS_Mode) {
            DBUGS.println("MODBUS MODE = ON"); //+ String(WIFi_Mode));
        }
        else {

            DBUGS.println("MODBUS MODE = OFF"); //+ String(WIFi_Mode));
        }

        DBUGS.println("MDBUS_BAUD = " + String(MDBUS_BAUD));
        DBUGS.println("MODBUS TCPPort = " + String(MDBus_TCPPort));
        DBUGS.println("Config Mode = " + String(Config_Mode));

        if (WIFi_Mode) {
            DBUGS.println("WiFi Mode = ON"); //+ String(WIFi_Mode));
        }
        else {

            DBUGS.println("WiFi Mode = OFF"); //+ String(WIFi_Mode));
        }


        if (WIFiClient_Mode) {
            DBUGS.println("WiFi Client Mode = ON"); //+ String(WIFi_Mode));
        }
        else {

            DBUGS.println("WiFi Client Mode = OFF"); //+ String(WIFi_Mode));
        }
        //DBUGS.println("WiFi Client Mode = " + String(WIFiClient_Mode));
        DBUGS.println("WIFi_SSID = " + WIFiSSID);
        DBUGS.println("WiFi Password = " + WiFiPassword);
        DBUGS.println("Server_IP = " + Server_IP);
        DBUGS.println("ETH_HostIP = " + ETH_HostIP);
        DBUGS.println("ETH_TCPPort = " + String(ETH_TCPPort));
        DBUGS.println("MODBUS_TCPHostIP = " + MDBus_TCPHostIP);
        DBUGS.println("WiFi Host_IP = " + WiFiHost_IP);
        DBUGS.println("WiFi Gateway = " + WiFiGateway);
        DBUGS.println("WiFi Subnet Mask = " + WiFiSubnet);
        DBUGS.println("WiFi DNS1 = " + DNS1);
        DBUGS.println("WiFi DNS2 = " + DNS2);

        if (WiFiAP_Mode) {
            DBUGS.println("WiFi AP Mode = ON"); //+ String(WIFi_Mode));
        }
        else {

            DBUGS.println("WiFi AP Mode = OFF"); //+ String(WIFi_Mode));
        }
        //DBUGS.println("WiFi AP Mode = " + String(WiFiAP_Mode));
        DBUGS.println("WiFi SoftAP IP = " + WiFiSoftAP_IP);
        DBUGS.println("WiFi SoftAP SSID = " + SoftAP_SSID);
        DBUGS.println("WiFi SoftAP Password = " + SoftAP_Password);
        DBUGS.println("MODBUS Slave_ID = " + String(ModSlave_ID));
        DBUGS.println();
        DBUGS.println("================================================================");;

        //systemRestartTime = millis() + 1000;

    }



 




}

//===========================================================================================================

void SendCallibration1_ToServer() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, client);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");

    //Configuration Registers
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    MMode0_Reg = Doc["MMode0"];
    MMode1_Reg = Doc["MMode1"];
    PMPGA_Reg = Doc["PMPGA"];
    PStartTh_Reg = Doc["PStartTh"];
    QStartTh_Reg = Doc["QStartTh"];
    SStartTh_Reg = Doc["SStartTh"];
    PPhaseTh_Reg = Doc["PPhaseTh"];
    QPhaseTh_Reg = Doc["QPhaseTh"];
    SPhaseTh_Reg = Doc["SPhaseTh"];

    JSONVar CAL1;

    CAL1["Page_Name"] = Cal_Page;
    CAL1["Trans"] = "CAL1_SUCCESS";
    CAL1["MMode0"] = MMode0_Reg;
    CAL1["MMode1"] = MMode1_Reg;
    CAL1["PMPGA"] = PMPGA_Reg;
    CAL1["PStartTh"] = PStartTh_Reg;
    CAL1["QStartTh"] = QStartTh_Reg;
    CAL1["SStartTh"] = SStartTh_Reg;
    CAL1["PPhaseTh"] = PPhaseTh_Reg;
    CAL1["QPhaseTh"] = QPhaseTh_Reg;
    CAL1["SPhaseTh"] = SPhaseTh_Reg;


    String jsonString = JSON.stringify(CAL1);

    if (saveConfigRegisters(Cali1_filename)) {
        DBUGS.println("Setup -> Config file saved");
        //serializeJson(Doc, client);
        client.println(jsonString);
    }


}

//===========================================================================================================

void SendCallibration2_ToServer() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, client);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");



    //Calibration Registers
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    CalRegisters[0] = Doc["PoffsetA"];
    CalRegisters[1] = Doc["QoffsetA"];
    CalRegisters[2] = Doc["POffsetB"];
    CalRegisters[3] = Doc["QOffsetB"];
    CalRegisters[4] = Doc["POffsetC"];
    CalRegisters[5] = Doc["QOffsetC"];
    CalRegisters[6] = Doc["GainA"];
    CalRegisters[7] = Doc["PhiA"];
    CalRegisters[8] = Doc["GainB"];
    CalRegisters[9] = Doc["PhiB"];
    CalRegisters[10] = Doc["GainC"];
    CalRegisters[11] = Doc["PhiC"];

    JSONVar CAL2;

    CAL2["Page_Name"] = Cal_Page;
    CAL2["Trans"] = "CAL2_SUCCESS";
    CAL2["PoffsetA"] = CalRegisters[0];
    CAL2["QoffsetA"] = CalRegisters[1];
    CAL2["POffsetB"] = CalRegisters[2];
    CAL2["QOffsetB"] = CalRegisters[3];
    CAL2["POffsetC"] = CalRegisters[4];
    CAL2["QOffsetC"] = CalRegisters[5];
    CAL2["GainA"] = CalRegisters[6];
    CAL2["PhiA"] = CalRegisters[7];
    CAL2["GainB"] = CalRegisters[8];
    CAL2["PhiB"] = CalRegisters[9];
    CAL2["GainC"] = CalRegisters[10];
    CAL2["PhiC"] = CalRegisters[11];

    String jsonString = JSON.stringify(CAL2);

    if (saveCalRegisters(Cali2_filename)) {
        DBUGS.println("Setup -> Calibration_1 file saved");

        client.println(jsonString);

        //StaticJsonDocument<240> doc;
        //JsonObject root = doc.to<JsonObject>();
        //JsonObject menu = root["Cal_1"].to<JsonObject>();
        //menu["MMode0"] = MMode0_Reg;
        //menu["MMode1"] = MMode1_Reg;
        //menu["PMPGA"] = PMPGA_Reg;
        //menu["PStartTh"] = PStartTh_Reg;
        //menu["QStartTh"] = QStartTh_Reg;
        //menu["SStartTh"] = SStartTh_Reg;
        //menu["PPhaseTh"] = PPhaseTh_Reg;
        //menu["QPhaseTh"] = QPhaseTh_Reg;
        //menu["SPhaseTh"] = SPhaseTh_Reg;

        //serializeJsonPretty(doc, DBUGS);
        //serializeJson(Doc, client);

    }


}

//===========================================================================================================

void SendCallibration3_ToServer() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, client);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");



    //Fundamental/ Harmonic Energy Calibration registers
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    HarCalRegisters[0] = Doc["POffsetAF"];
    HarCalRegisters[1] = Doc["POffsetBF"];
    HarCalRegisters[2] = Doc["POffsetCF"];
    HarCalRegisters[3] = Doc["PGainAF"];
    HarCalRegisters[4] = Doc["PGainBF"];
    HarCalRegisters[5] = Doc["PGainCF"];

    JSONVar CAL3;

    CAL3["Page_Name"] = Cal_Page;
    CAL3["Trans"] = "CAL3_SUCCESS";
    CAL3["POffsetAF"] = HarCalRegisters[0];
    CAL3["POffsetBF"] = HarCalRegisters[1];
    CAL3["POffsetCF"] = HarCalRegisters[2];
    CAL3["PGainAF"] = HarCalRegisters[3];
    CAL3["PGainBF"] = HarCalRegisters[4];
    CAL3["PGainCF"] = HarCalRegisters[5];

    String jsonString = JSON.stringify(CAL3);


    if (saveFund_HarCalRegisters(Cali3_filename)) {
        DBUGS.println("Setup -> Calibration_2 file saved");
        //serializeJson(Doc, client);
        client.println(jsonString);

    }



}


//===========================================================================================================

void SendCallibration4_ToServer() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, client);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");



    //Measurement Calibration
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    MeasurCalRegisters[0] = Doc["UgainA"];
    MeasurCalRegisters[1] = Doc["IgainA"];
    MeasurCalRegisters[2] = Doc["UoffsetA"];
    MeasurCalRegisters[3] = Doc["IoffsetA"];
    MeasurCalRegisters[4] = Doc["UgainB"];
    MeasurCalRegisters[5] = Doc["IgainB"];
    MeasurCalRegisters[6] = Doc["UoffsetB"];
    MeasurCalRegisters[7] = Doc["IoffsetB"];
    MeasurCalRegisters[8] = Doc["UgainC"];
    MeasurCalRegisters[9] = Doc["IgainC"];
    MeasurCalRegisters[10] = Doc["UoffsetC"];
    MeasurCalRegisters[11] = Doc["IoffsetC"];
    MeasurCalRegisters[12] = Doc["IgainN"];
    MeasurCalRegisters[13] = Doc["IoffsetN"];

    JSONVar CAL4;

    CAL4["Page_Name"] = Cal_Page;
    CAL4["Trans"] = "CAL4_SUCCESS";
    CAL4["UgainA"] = MeasurCalRegisters[0];
    CAL4["IgainA"] = MeasurCalRegisters[1];
    CAL4["UoffsetA"] = MeasurCalRegisters[2];
    CAL4["IoffsetA"] = MeasurCalRegisters[3];
    CAL4["UgainB"] = MeasurCalRegisters[4];
    CAL4["IgainB"] = MeasurCalRegisters[5];
    CAL4["UoffsetB"] = MeasurCalRegisters[6];
    CAL4["IoffsetB"] = MeasurCalRegisters[7];
    CAL4["UgainC"] = MeasurCalRegisters[8];
    CAL4["IgainC"] = MeasurCalRegisters[9];
    CAL4["UoffsetC"] = MeasurCalRegisters[10];
    CAL4["IoffsetC"] = MeasurCalRegisters[11];
    CAL4["IgainN"] = MeasurCalRegisters[12];
    CAL4["IoffsetN"] = MeasurCalRegisters[13];

    String jsonString = JSON.stringify(CAL4);

    if (saveMeasureCalRegisters(Cali4_filename)) {
        DBUGS.println("Setup -> Calibration_3 file saved");
        //serializeJson(Doc, client);
        client.println(jsonString);

    }


}


//==========================================================================================================
//void SendCallibrationToServer() {
//
//    JsonDocument Doc;
//
//    DeserializationError Error = deserializeJson(Doc, client);
//
//    if (Error) {
//        DBUGS.println("Invalid JSON Object");
//        return;
//    }
//
//    DBUGS.println("Valid JSON Object Received");
//
//    //Configuration Registers
//    MMode0_Reg = Doc["MMode0"];
//    MMode1_Reg = Doc["MMode1"];
//    PMPGA_Reg = Doc["PMPGA"];
//    PStartTh_Reg = Doc["PStartTh"];
//    QStartTh_Reg = Doc["QStartTh"];
//    SStartTh_Reg = Doc["SStartTh"];
//    PPhaseTh_Reg = Doc["PPhaseTh"];
//    QPhaseTh_Reg = Doc["QPhaseTh"];
//    SPhaseTh_Reg = Doc["SPhaseTh"];
//
//    //Calibration Registers
//    CalRegisters[0] = Doc["PoffsetA"];
//    CalRegisters[1] = Doc["QoffsetA"];
//    CalRegisters[2] = Doc["POffsetB"];
//    CalRegisters[3] = Doc["QOffsetB"];
//    CalRegisters[4] = Doc["POffsetC"];
//    CalRegisters[5] = Doc["QOffsetC"];
//    CalRegisters[6] = Doc["GainA"];
//    CalRegisters[7] = Doc["PhiA"];
//    CalRegisters[8] = Doc["GainB"];
//    CalRegisters[9] = Doc["PhiB"];
//    CalRegisters[10] = Doc["GainC"];
//    CalRegisters[11] = Doc["PhiC"];
//
//    //Fundamental/ Harmonic Energy Calibration registers
//    HarCalRegisters[0] = Doc["POffsetAF"];
//    HarCalRegisters[1] = Doc["POffsetBF"];
//    HarCalRegisters[2] = Doc["POffsetCF"];
//    HarCalRegisters[3] = Doc["PGainAF"];
//    HarCalRegisters[4] = Doc["PGainBF"];
//    HarCalRegisters[5] = Doc["PGainCF"];
//
//    //Measurement Calibration
//    MeasurCalRegisters[0] = Doc["UgainA"];
//    MeasurCalRegisters[1] = Doc["IgainA"];
//    MeasurCalRegisters[2] = Doc["UoffsetA"];
//    MeasurCalRegisters[3] = Doc["IoffsetA"];
//    MeasurCalRegisters[4] = Doc["UgainB"];
//    MeasurCalRegisters[5] = Doc["IgainB"];
//    MeasurCalRegisters[6] = Doc["UoffsetB"];
//    MeasurCalRegisters[7] = Doc["IoffsetB"];
//    MeasurCalRegisters[8] = Doc["UgainC"];
//    MeasurCalRegisters[9] = Doc["IgainC"];
//    MeasurCalRegisters[10] = Doc["UoffsetC"];
//    MeasurCalRegisters[11] = Doc["IoffsetC"];
//    MeasurCalRegisters[12] = Doc["IgainN"];
//    MeasurCalRegisters[13] = Doc["IoffsetN"];
//
//    if (saveConfig(config_filename)) {
//        DBUGS.println("Setup -> Calibration file saved");
//        serializeJson(Doc, client);
//
//    }
//
//
//
//
//
//}