
boolean Config_State = false, WiFiAP_Mode = false, WIFi_Mode = false, WIFiClient_Mode = false, ETH_Mode = false, MDBUS_Mode = false;
int SerialBaud = 115200, ETH_TCPPort = 8088, WiFiServerPort = 80;
int  MDBUS_BAUD = 38400, MDBus_TCPPort = 502;
String WIFiSSID = "Google NODE", WiFiPassword = "36002016", WIFi_State="OFF";
String Server_IP = "169.254.33.234", ETH_HostIP = "169.254.33.20", MDBus_TCPHostIP = "169.254.33.100", WiFiHost_IP = "192.168.86.25", WiFiGateway = "192.168.86.1", WiFiSubnet = "255.255.255.0", DNS1 = "192.168.86.1", DNS2 = "0.0.0.0";
String WiFiSoftAP_IP = "192.168.4.1", SoftAP_SSID = "SM-GE3222M(WiFi)", SoftAP_Password = "";
uint8_t ModSlave_ID = 1;
String Config_Mode = "",COM_MODE = "";
float  PublishInterval = 2000;
String Cal_Page = "";

unsigned int MMode0_Reg = LineFreq, MMode1_Reg = PGAGain, PMPGA_Reg, PStartTh_Reg, QStartTh_Reg, SStartTh_Reg, PPhaseTh_Reg, QPhaseTh_Reg, SPhaseTh_Reg;

extern unsigned short VoltageGain1, VoltageGain2, VoltageGain3; // uGain = UgainA | 20200 Default Starting Value
extern unsigned short CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN; // IgainA 0x62



void LoadConfig() {

    initSPIFFS();

}


void ReadCallibration() {

    //MMode0_Reg = eic.GetValue(MMode0);
    //MMode1_Reg = eic.GetValue(MMode1);
    //PMPGA_Reg = eic.GetValue(PMPGA);
    //PStartTh_Reg = eic.GetValue(PStartTh);
    //QStartTh_Reg = eic.GetValue(QStartTh);
    //SStartTh_Reg = eic.GetValue(SStartTh);
    //PPhaseTh_Reg = eic.GetValue(PPhaseTh);
    //QPhaseTh_Reg = eic.GetValue(QPhaseTh);
    //SPhaseTh_Reg = eic.GetValue(SPhaseTh);

    //Read Configuration Registers
    MMode0_Reg = LineFreq;
    MMode1_Reg = PGAGain;
    PMPGA_Reg = 0;
    PStartTh_Reg = 0;
    QStartTh_Reg = 0;
    SStartTh_Reg = 0;
    PPhaseTh_Reg = 0;
    QPhaseTh_Reg = 0;
    SPhaseTh_Reg = 0;

    MeasurCalRegisters[0] = VoltageGain1;
    MeasurCalRegisters[4] = VoltageGain2; // Voltage 2 RMS Gain
    MeasurCalRegisters[8] = VoltageGain3; // Voltage 3 RMS Gain

    MeasurCalRegisters[1] = CurrentGainCT1; // CT1
    MeasurCalRegisters[5] = CurrentGainCT2; // CT2 - not used for single split phase meter
    MeasurCalRegisters[9] = CurrentGainCT3; // CT3 or CT2 for single split phase meter
    MeasurCalRegisters[12] = CurrentGainCTN; // CTN (Used on the ATM90E36)



    // ****************  Initialise the ATM90E3x & Pass related calibrations to its library ****************
    DeviceInit();

    /*uint8_t regadd = 0;*/

    ////Read Common Calibration Registers
    //do {
    //    CalRegisters[regadd] = eic.GetValue(regadd + 65);
    //} while (++regadd < sizeof(CalRegisters)-1);

    //regadd = 0;

    ////Read Fundamental/ Harmonic Energy Calibration registers
    //do {
    //    HarCalRegisters[regadd] = eic.GetValue(regadd + 81);
    //} while (++regadd < sizeof(HarCalRegisters)-1);

    //regadd = 0;

    ////Read Measurement Calibration registers
    //do {
    //    MeasurCalRegisters[regadd] = eic.GetValue(regadd + 97);
    //} while (++regadd < sizeof(MeasurCalRegisters)-1);


}


// Initialize SPIFFS
void initSPIFFS() {

    //Display Preloaded Configuration Settings
    DBUGS.println("====================== Display Original Configuration Settings ======================");
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

    //SPIFFS.begin(); // mount the fs

  // Mount SPIFFS and read in config file
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
        DBUGS.println("setup -> SPIFFS Mount Failed");
    }
    else {
        DBUGS.println("setup -> SPIFFS mounted successfully");
        if (readConfig(config_filename) == false) {
            DBUGS.println("Setup -> Could not read Settings file -> initializing new file");
            // if not possible -> save new config file
            if (saveConfig(config_filename)) {
                DBUGS.println("Setup -> Settings file saved");
            }
        }

        //Display Created Configuration Settings
        DBUGS.println("====================== Display Configuration File Settings ======================");
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

        ReadCallibration();

        //Read and Save Config Registers (Calibration1)
        if (readConfigRegisters(Cali1_filename) == false) {
            DBUGS.println("Setup -> Could not read Config file -> initializing new file");
            // if not possible -> save new config file
            if (saveConfigRegisters(Cali1_filename)) {
                DBUGS.println("Setup -> Config file saved");
            }
        }

        //Read and Save Common Calibration Registers (Calibration2)
        if (readCalRegisters(Cali2_filename) == false) {
            DBUGS.println("Setup -> Could not read Calibration file -> initializing new file");
            // if not possible -> save new config file
            if (saveCalRegisters(Cali2_filename)) {
                DBUGS.println("Setup -> Calibration_1 file saved");
            }
        }

        //Read and Save //Fundamental/ Harmonic Energy Calibration registers (Calibration3) 
        if (readFund_HarCalRegisters(Cali3_filename) == false) {
            DBUGS.println("Setup -> Could not read Calibration file -> initializing new file");
            // if not possible -> save new config file
            if (saveFund_HarCalRegisters(Cali3_filename)) {
                DBUGS.println("Setup -> Calibration_2 file saved");
            }
        }

        //Read and Save //Fundamental/ Harmonic Energy Calibration registers (Calibration4) 
        if (readMeasureCalRegisters(Cali4_filename) == false) {
            DBUGS.println("Setup -> Could not read Calibration file -> initializing new file");
            // if not possible -> save new config file
            if (saveMeasureCalRegisters(Cali4_filename)) {
                DBUGS.println("Setup -> Calibration_3 file saved");
            }
        }






    }
}

bool readConfigRegisters(String file_name) {
    String file_content = readFile(SPIFFS, file_name);

    int config_file_size = file_content.length();
    DBUGS.println("Config file size: " + String(config_file_size));

    if (config_file_size > 240) {
        DBUGS.println("Config file too large");
        return false;
    }
    StaticJsonDocument<240> Doc;
    auto error = deserializeJson(Doc, file_content);
    if (error) {
        DBUGS.println("Error interpreting config file");
        return false;
    }



    //Configuration Registers 
    const String _Cal_Page = Doc["Page_Name"];
    const int _MMode0_Reg = Doc["MMode0"];
    const int _MMode1_Reg = Doc["MMode1"];
    const int _PMPGA_Reg = Doc["PMPGA"];
    const int _PStartTh_Reg = Doc["PStartTh"];
    const int _QStartTh_Reg = Doc["QStartTh"];
    const int _SStartTh_Reg = Doc["SStartTh"];
    const int _PPhaseTh_Reg = Doc["PPhaseTh"];
    const int _QPhaseTh_Reg = Doc["QPhaseTh"];
    const int _SPhaseTh_Reg = Doc["SPhaseTh"];

    
    Cal_Page = _Cal_Page;
    MMode0_Reg = _MMode0_Reg;
    MMode1_Reg = _MMode1_Reg;
    PMPGA_Reg = _PMPGA_Reg;
    PStartTh_Reg = _PStartTh_Reg;
    QStartTh_Reg = _QStartTh_Reg;
    SStartTh_Reg = _SStartTh_Reg;
    PPhaseTh_Reg = _PPhaseTh_Reg;
    QPhaseTh_Reg = _QPhaseTh_Reg;
    SPhaseTh_Reg = _SPhaseTh_Reg;


    return true;
}


//==================================================================================================================
bool readCalRegisters(String file_name) {
    String file_content = readFile(SPIFFS, file_name);

    int config_file_size = file_content.length();
    DBUGS.println("Calibration_1 file size: " + String(config_file_size));

    if (config_file_size > 340) {
        DBUGS.println("Calibration_1 file too large");
        return false;
    }
    StaticJsonDocument<340> Doc;
    auto error = deserializeJson(Doc, file_content);
    if (error) {
        DBUGS.println("Error interpreting config file");
        return false;
    }




    //Common Calibration Registers (Calibration1)

    const String _Cal_Page = Doc["Page_Name"];
    const int _PoffsetA = Doc["PoffsetA"];
    const int _QoffsetA = Doc["QoffsetA"];
    const int _POffsetB = Doc["POffsetB"];
    const int _QOffsetB = Doc["QOffsetB"];
    const int _POffsetC = Doc["POffsetC"];
    const int _QOffsetC = Doc["QOffsetC"];
    const int _GainA = Doc["GainA"];
    const int _PhiA = Doc["PhiA"];
    const int _GainB = Doc["GainB"];
    const int _PhiB = Doc["PhiB"];
    const int _GainC = Doc["GainC"];
    const int _PhiC = Doc["PhiC"];

    Cal_Page = _Cal_Page;
    CalRegisters[0] = _PoffsetA;
    CalRegisters[1] = _QoffsetA;
    CalRegisters[2] = _POffsetB;
    CalRegisters[3] = _QOffsetB;
    CalRegisters[4] = _POffsetC;
    CalRegisters[5] = _QOffsetC;
    CalRegisters[6] = _GainA;
    CalRegisters[7] = _PhiA;
    CalRegisters[8] = _GainB;
    CalRegisters[9] = _PhiB;
    CalRegisters[10] = _GainC;
    CalRegisters[11] = _PhiC;


    return true;
}


//==================================================================================================================

bool readFund_HarCalRegisters(String file_name) {
    String file_content = readFile(SPIFFS, file_name);

    int config_file_size = file_content.length();
    DBUGS.println("Calibration_2 file size: " + String(config_file_size));

    if (config_file_size > 180) {
        DBUGS.println("Calibration_2 file too large");
        return false;
    }
    StaticJsonDocument<180> Doc;
    auto error = deserializeJson(Doc, file_content);
    if (error) {
        DBUGS.println("Error interpreting config file");
        return false;
    }



    //Fundamental/ Harmonic Energy Calibration registers (Calibration2)
    const String _Cal_Page = Doc["Page_Name"];
    const int _POffsetAF = Doc["POffsetAF"];
    const int _POffsetBF = Doc["POffsetBF"];
    const int _POffsetCF = Doc["POffsetCF"];
    const int _PGainAF = Doc["PGainAF"];
    const int _PGainBF = Doc["PGainBF"];
    const int _PGainCF = Doc["PGainCF"];

    
    Cal_Page = _Cal_Page;
    HarCalRegisters[0] = _POffsetAF;
    HarCalRegisters[1] = _POffsetBF;
    HarCalRegisters[2] = _POffsetCF;
    HarCalRegisters[3] = _PGainAF;
    HarCalRegisters[4] = _PGainBF;
    HarCalRegisters[5] = _PGainCF;

    return true;
}


//==================================================================================================================
bool readMeasureCalRegisters(String file_name) {
    String file_content = readFile(SPIFFS, file_name);

    int config_file_size = file_content.length();
    DBUGS.println("Calibration_3 file size: " + String(config_file_size));

    if (config_file_size > 380) {
        DBUGS.println("Calibration_3 file too large");
        return false;
    }
    StaticJsonDocument<380> Doc;
    auto error = deserializeJson(Doc, file_content);
    if (error) {
        DBUGS.println("Error interpreting config file");
        return false;
    }



    //Measurement Calibration (Calibration3)
    const String _Cal_Page = Doc["Page_Name"];
    const int _UgainA = Doc["UgainA"];
    const int _IgainA = Doc["IgainA"];
    const int _UoffsetA = Doc["UoffsetA"];
    const int _IoffsetA = Doc["IoffsetA"];
    const int _UgainB = Doc["UgainB"];
    const int _IgainB = Doc["IgainB"];
    const int _UoffsetB = Doc["UoffsetB"];
    const int _IoffsetB = Doc["IoffsetB"];
    const int _UgainC = Doc["UgainC"];
    const int _IgainC = Doc["IgainC"];
    const int _UoffsetC = Doc["UoffsetC"];
    const int _IoffsetC = Doc["IoffsetC"];
    const int _IgainN = Doc["IgainN"];
    const int _IoffsetN = Doc["IoffsetN"];

    
    Cal_Page = _Cal_Page;
    MeasurCalRegisters[0] = _UgainA;
    MeasurCalRegisters[1] = _IgainA;
    MeasurCalRegisters[2] = _UoffsetA;
    MeasurCalRegisters[3] = _IoffsetA;
    MeasurCalRegisters[4] = _UgainB;
    MeasurCalRegisters[5] = _IgainB;
    MeasurCalRegisters[6] = _UoffsetB;
    MeasurCalRegisters[7] = _IoffsetB;
    MeasurCalRegisters[8] = _UgainC;
    MeasurCalRegisters[9] = _IgainC;
    MeasurCalRegisters[10] = _UoffsetC;
    MeasurCalRegisters[11] = _IoffsetC;
    MeasurCalRegisters[12] = _IgainN;
    MeasurCalRegisters[13] = _IoffsetN;


    return true;
}


//bool readConfigRegisters(String file_name) {
//    String file_content = readFile(SPIFFS, file_name);
//
//    int config_file_size = file_content.length();
//    DBUGS.println("Config file size: " + String(config_file_size));
//
//    if (config_file_size > 1024) {
//        DBUGS.println("Config file too large");
//        return false;
//    }
//    StaticJsonDocument<1024> Doc;
//    auto error = deserializeJson(Doc, file_content);
//    if (error) {
//        DBUGS.println("Error interpreting config file");
//        return false;
//    }
//
//    
//
//    //Configuration Registers
//    const int _MMode0_Reg = Doc["MMode0"];
//    const int _MMode1_Reg = Doc["MMode1"];
//    const int _PMPGA_Reg = Doc["PMPGA"];
//    const int _PStartTh_Reg = Doc["PStartTh"];
//    const int _QStartTh_Reg = Doc["QStartTh"];
//    const int _SStartTh_Reg = Doc["SStartTh"];
//    const int _PPhaseTh_Reg = Doc["PPhaseTh"];
//    const int _QPhaseTh_Reg = Doc["QPhaseTh"];
//    const int _SPhaseTh_Reg = Doc["SPhaseTh"];
//    //Calibration Registers
//    const int _PoffsetA = Doc["PoffsetA"];
//    const int _QoffsetA = Doc["QoffsetA"];
//    const int _POffsetB = Doc["POffsetB"];
//    const int _QOffsetB = Doc["QOffsetB"];
//    const int _POffsetC = Doc["POffsetC"];
//    const int _QOffsetC = Doc["QOffsetC"];
//    const int _GainA = Doc["GainA"];
//    const int _PhiA = Doc["PhiA"];
//    const int _GainB = Doc["GainB"];
//    const int _PhiB = Doc["PhiB"];
//    const int _GainC = Doc["GainC"];
//    const int _PhiC = Doc["PhiC"];
//
//    //Fundamental/ Harmonic Energy Calibration registers
//    const int _POffsetAF = Doc["POffsetAF"];
//    const int _POffsetBF = Doc["POffsetBF"];
//    const int _POffsetCF = Doc["POffsetCF"];
//    const int _PGainAF = Doc["PGainAF"];
//    const int _PGainBF = Doc["PGainBF"];
//    const int _PGainCF = Doc["PGainCF"];
//
//    //Measurement Calibration
//    const int _UgainA = Doc["UgainA"];
//    const int _IgainA = Doc["IgainA"];
//    const int _UoffsetA = Doc["UoffsetA"];
//    const int _IoffsetA = Doc["IoffsetA"];
//    const int _UgainB = Doc["UgainB"];
//    const int _IgainB = Doc["IgainB"];
//    const int _UoffsetB = Doc["UoffsetB"];
//    const int _IoffsetB = Doc["IoffsetB"];
//    const int _UgainC = Doc["UgainC"];
//    const int _IgainC = Doc["IgainC"];
//    const int _UoffsetC = Doc["UoffsetC"];
//    const int _IoffsetC = Doc["IoffsetC"];
//    const int _IgainN = Doc["IgainN"];
//    const int _IoffsetN = Doc["IoffsetN"];
//
//
//    MMode0_Reg = _MMode0_Reg;
//    MMode1_Reg = _MMode1_Reg;
//    PMPGA_Reg = _PMPGA_Reg;
//    PStartTh_Reg = _PStartTh_Reg;
//    QStartTh_Reg = _QStartTh_Reg;
//    SStartTh_Reg = _SStartTh_Reg;
//    PPhaseTh_Reg = _PPhaseTh_Reg;
//    QPhaseTh_Reg = _QPhaseTh_Reg;
//    SPhaseTh_Reg = _SPhaseTh_Reg;
//
//    CalRegisters[0] = _PoffsetA;
//    CalRegisters[1] = _QoffsetA;
//    CalRegisters[2] = _POffsetB;
//    CalRegisters[3] = _QOffsetB;
//    CalRegisters[4] = _POffsetC;
//    CalRegisters[5] = _QOffsetC;
//    CalRegisters[6] = _GainA;
//    CalRegisters[7] = _PhiA;
//    CalRegisters[8] = _GainB;
//    CalRegisters[9] = _PhiB;
//    CalRegisters[10] = _GainC;
//    CalRegisters[11] = _PhiC;
//
//    HarCalRegisters[0] = _POffsetAF;
//    HarCalRegisters[1] = _POffsetBF;
//    HarCalRegisters[2] = _POffsetCF;
//    HarCalRegisters[3] = _PGainAF;
//    HarCalRegisters[4] = _PGainBF;
//    HarCalRegisters[5] = _PGainCF;
//
//    MeasurCalRegisters[0] = _UgainA;
//    MeasurCalRegisters[1] = _IgainA;
//    MeasurCalRegisters[2] = _UoffsetA;
//    MeasurCalRegisters[3] = _IoffsetA;
//    MeasurCalRegisters[4] = _UgainB;
//    MeasurCalRegisters[5] = _IgainB;
//    MeasurCalRegisters[6] = _UoffsetB;
//    MeasurCalRegisters[7] = _IoffsetB;
//    MeasurCalRegisters[8] = _UgainC;
//    MeasurCalRegisters[9] = _IgainC;
//    MeasurCalRegisters[10] = _UoffsetC;
//    MeasurCalRegisters[11] = _IoffsetC;
//    MeasurCalRegisters[12] = _IgainN;
//    MeasurCalRegisters[13] = _IoffsetN;
//
//
//
//    return true;
//}



bool readConfig(String file_name) {
    String file_content = readFile(SPIFFS, file_name);

    int config_file_size = file_content.length();
    DBUGS.println("Config file size: " + String(config_file_size));

    if (config_file_size > 1024) {
        DBUGS.println("Config file too large");
        return false;
    }
    StaticJsonDocument<1024> doc;
    auto error = deserializeJson(doc, file_content);
    if (error) {
        DBUGS.println("Error interpreting config file");
        return false;
    }

    const String _Cal_Page = doc["Page_Name"];
    const String _COM_MODE = doc["COM_MODE"];
    const boolean _Config_State = doc["Config_State"];
    const int _SerialBaud = doc["SerialBaud"];
    const boolean _WIFi_Mode = doc["WIFi_Mode"];
    const String  _WIFi_State = doc["WIFi_State"];
    const boolean _WIFiClient_Mode = doc["WIFiClient_Mode"];
    const String  _WIFiSSID = doc["WIFiSSID"];
    const String  _WiFiPassword = doc["WiFiPassword"];
    const String  _Server_IP = doc["Server_IP"];
    const String  _WiFiHost_IP = doc["WiFiHost_IP"];
    const int _WiFiServerPort = doc["WiFiServerPort"];
    const String _WiFiGateway = doc["WiFiGateway"];
    const String _WiFiSubnet = doc["WiFiSubnet"];
    const String _DNS1 = doc["DNS1"];
    const String _DNS2 = doc["DNS2"];
    const boolean _WiFiAP_Mode = doc["WiFiAP_Mode"];
    const String _WiFiSoftAP_IP = doc["WiFiSoftAP_IP"];
    const String _SoftAP_SSID = doc["SoftAP_SSID"];
    const String _SoftAP_Password = doc["SoftAP_Password"];
    const boolean _ETH_Mode = doc["ETH_Mode"];
    const String _ETH_HostIP = doc["ETH_HostIP"];
    const int _ETH_TCPPort = doc["ETH_TCPPort"];
    const boolean _MDBUS_Mode = doc["MDBUS_Mode"];
    const int _ModSlave_ID = doc["MDBUS_ID"];
    const String _MDBus_TCPHostIP = doc["MDBus_TCPHostIP"];
    const int _MDBus_TCPPort = doc["MDBus_TCPPort"];
    const int _MDBUS_BAUD = doc["MDBUS_BAUD"];
    const int _Publish_Interval = doc["Publish_Interval"];
    const String  _Config_Mode = doc["Config_Mode"];


    Cal_Page = _Cal_Page;
    COM_MODE = _COM_MODE;
    Config_State = _Config_State;
    SerialBaud = _SerialBaud;
    WIFi_Mode = _WIFi_Mode;
    WIFi_State = _WIFi_State;
    WIFiClient_Mode = _WIFiClient_Mode;
    WIFiSSID = _WIFiSSID;
    WiFiPassword = _WiFiPassword;
    Server_IP = _Server_IP;
    WiFiHost_IP = _WiFiHost_IP;
    WiFiServerPort = _WiFiServerPort;
    WiFiGateway = _WiFiGateway;
    WiFiSubnet = _WiFiSubnet;
    DNS1 = _DNS1;
    DNS2 = _DNS2;
    WiFiAP_Mode = _WiFiAP_Mode;
    WiFiSoftAP_IP = _WiFiSoftAP_IP;
    SoftAP_SSID = _SoftAP_SSID;
    SoftAP_Password = _SoftAP_Password;
    ETH_Mode = _ETH_Mode;
    ETH_HostIP = _ETH_HostIP;
    ETH_TCPPort = _ETH_TCPPort;
    MDBUS_Mode = _MDBUS_Mode;
    ModSlave_ID = _ModSlave_ID;
    MDBus_TCPHostIP = _MDBus_TCPHostIP;
    MDBus_TCPPort = _MDBus_TCPPort;
    MDBUS_BAUD = _MDBUS_BAUD;
    PublishInterval = _Publish_Interval;
    Config_Mode = _Config_Mode;


    return true;
}

bool saveConfig(String file_name) {
    StaticJsonDocument<1024> doc;

    // write variables to JSON file
    doc["Page_Name"] = Cal_Page;
    doc["COM_MODE"] = COM_MODE;
    doc["Config_State"] = Config_State;
    doc["SerialBaud"] = SerialBaud;
    doc["WIFi_Mode"] = WIFi_Mode;
    doc["WIFi_State"] = WIFi_State;
    doc["WIFiClient_Mode"] = WIFiClient_Mode;
    doc["WIFiSSID"] = WIFiSSID;
    doc["WiFiPassword"] = WiFiPassword;
    doc["Server_IP"] = Server_IP;
    doc["WiFiHost_IP"] = WiFiHost_IP;
    doc["WiFiServerPort"] = WiFiServerPort;
    doc["WiFiGateway"] = WiFiGateway;
    doc["WiFiSubnet"] = WiFiSubnet;
    doc["DNS1"] = DNS1;
    doc["DNS2"] = DNS2;
    doc["WiFiAP_Mode"] = WiFiAP_Mode;
    doc["WiFiSoftAP_IP"] = WiFiSoftAP_IP;
    doc["SoftAP_SSID"] = SoftAP_SSID;
    doc["SoftAP_Password"] = SoftAP_Password;
    doc["ETH_Mode"] = ETH_Mode;
    doc["ETH_HostIP"] = ETH_HostIP;
    doc["ETH_TCPPort"] = ETH_TCPPort;
    doc["MDBUS_Mode"] = MDBUS_Mode;
    doc["MDBUS_ID"] = ModSlave_ID;
    doc["MDBus_TCPHostIP"] = MDBus_TCPHostIP;
    doc["MDBus_TCPPort"] = MDBus_TCPPort;
    doc["MDBUS_BAUD"] = MDBUS_BAUD;
    doc["Publish_Interval"] = PublishInterval;
    doc["Config_Mode"] = Config_Mode;



    // write config file
    String tmp = "";
    serializeJson(doc, tmp);
    writeFile(SPIFFS, config_filename, tmp);

    return true;
}

//=============================================================================
bool saveConfigRegisters(String file_name) {
    StaticJsonDocument<240> Doc;


    //Configuration Registers
    Doc["Page_Name"] = Cal_Page;
    Doc["MMode0"] = MMode0_Reg;
    Doc["MMode1"] = MMode1_Reg;
    Doc["PMPGA"] = PMPGA_Reg;
    Doc["PStartTh"] = PStartTh_Reg;
    Doc["QStartTh"] = QStartTh_Reg;
    Doc["SStartTh"] = SStartTh_Reg;
    Doc["PPhaseTh"] = PPhaseTh_Reg;
    Doc["QPhaseTh"] = QPhaseTh_Reg;
    Doc["SPhaseTh"] = SPhaseTh_Reg;


    // write config file
    String tmp = "";
    serializeJson(Doc, tmp);
    writeFile(SPIFFS, Cali1_filename, tmp);



    


    return true;
}

//=============================================================================

bool saveCalRegisters(String file_name) {
    StaticJsonDocument<340> Doc;



    //Calibration Registers
    Doc["Page_Name"] = Cal_Page;
    Doc["PoffsetA"] = CalRegisters[0];
    Doc["QoffsetA"] = CalRegisters[1];
    Doc["POffsetB"] = CalRegisters[2];
    Doc["QOffsetB"] = CalRegisters[3];
    Doc["POffsetC"] = CalRegisters[4];
    Doc["QOffsetC"] = CalRegisters[5];
    Doc["GainA"] = CalRegisters[6];
    Doc["PhiA"] = CalRegisters[7];
    Doc["GainB"] = CalRegisters[8];
    Doc["PhiB"] = CalRegisters[9];
    Doc["GainC"] = CalRegisters[10];
    Doc["PhiC"] = CalRegisters[11];





    // write config file
    String tmp = "";
    serializeJson(Doc, tmp);
    writeFile(SPIFFS, Cali2_filename, tmp);

    return true;
}



//============================================================================

bool saveFund_HarCalRegisters(String file_name) {
    StaticJsonDocument<180> Doc;


    //Fundamental/ Harmonic Energy Calibration registers
    Doc["Page_Name"] = Cal_Page;
    Doc["POffsetAF"] = HarCalRegisters[0];
    Doc["POffsetBF"] = HarCalRegisters[1];
    Doc["POffsetCF"] = HarCalRegisters[2];
    Doc["PGainAF"] = HarCalRegisters[3];
    Doc["PGainBF"] = HarCalRegisters[4];
    Doc["PGainCF"] = HarCalRegisters[5];



    // write config file
    String tmp = "";
    serializeJson(Doc, tmp);
    writeFile(SPIFFS, Cali3_filename, tmp);

    return true;
}



//============================================================================


bool saveMeasureCalRegisters(String file_name) {
    StaticJsonDocument<380> Doc;


    //Measurement Calibration
    Doc["Page_Name"] = Cal_Page;
    Doc["UgainA"] = MeasurCalRegisters[0];
    Doc["IgainA"] = MeasurCalRegisters[1];
    Doc["UoffsetA"] = MeasurCalRegisters[2];
    Doc["IoffsetA"] = MeasurCalRegisters[3];
    Doc["UgainB"] = MeasurCalRegisters[4];
    Doc["IgainB"] = MeasurCalRegisters[5];
    Doc["UoffsetB"] = MeasurCalRegisters[6];
    Doc["IoffsetB"] = MeasurCalRegisters[7];
    Doc["UgainC"] = MeasurCalRegisters[8];
    Doc["IgainC"] = MeasurCalRegisters[9];
    Doc["UoffsetC"] = MeasurCalRegisters[10];
    Doc["IoffsetC"] = MeasurCalRegisters[11];
    Doc["IgainN"] = MeasurCalRegisters[12];
    Doc["IoffsetN"] = MeasurCalRegisters[13];



    // write config file
    String tmp = "";
    serializeJson(Doc, tmp);
    writeFile(SPIFFS, Cali4_filename, tmp);

    return true;
}



//============================================================================

//bool saveCallibration(String file_name) {
//    StaticJsonDocument<1024> Doc;
//
//
//	//Configuration Registers
//	Doc["MMode0"] = MMode0_Reg;
//	Doc["MMode1"] = MMode1_Reg;
//	Doc["PMPGA"] = PMPGA_Reg;
//	Doc["PStartTh"] = PStartTh_Reg;
//	Doc["QStartTh"] = QStartTh_Reg;
//	Doc["SStartTh"] = SStartTh_Reg;
//	Doc["PPhaseTh"] = PPhaseTh_Reg;
//	Doc["QPhaseTh"] = QPhaseTh_Reg;
//	Doc["SPhaseTh"] = SPhaseTh_Reg;
//	//Calibration Registers
//	Doc["PoffsetA"] = CalRegisters[0];
//	Doc["QoffsetA"] = CalRegisters[1];
//	Doc["POffsetB"] = CalRegisters[2];
//	Doc["QOffsetB"] = CalRegisters[3];
//	Doc["POffsetC"] = CalRegisters[4];
//	Doc["QOffsetC"] = CalRegisters[5];
//	Doc["GainA"] = CalRegisters[6];
//	Doc["PhiA"] = CalRegisters[7];
//	Doc["GainB"] = CalRegisters[8];
//	Doc["PhiB"] = CalRegisters[9];
//	Doc["GainC"] = CalRegisters[10];
//	Doc["PhiC"] = CalRegisters[11];
//
//	//Fundamental/ Harmonic Energy Calibration registers
//	Doc["POffsetAF"] = HarCalRegisters[0];
//	Doc["POffsetBF"] = HarCalRegisters[1];
//	Doc["POffsetCF"] = HarCalRegisters[2];
//	Doc["PGainAF"] = HarCalRegisters[3];
//	Doc["PGainBF"] = HarCalRegisters[4];
//	Doc["PGainCF"] = HarCalRegisters[5];
//
//
//	//Measurement Calibration
//	Doc["UgainA"] = MeasurCalRegisters[0];
//	Doc["IgainA"] = MeasurCalRegisters[1];
//	Doc["UoffsetA"] = MeasurCalRegisters[2];
//	Doc["IoffsetA"] = MeasurCalRegisters[3];
//	Doc["UgainB"] = MeasurCalRegisters[4];
//	Doc["IgainB"] = MeasurCalRegisters[5];
//	Doc["UoffsetB"] = MeasurCalRegisters[6];
//	Doc["IoffsetB"] = MeasurCalRegisters[7];
//	Doc["UgainC"] = MeasurCalRegisters[8];
//	Doc["IgainC"] = MeasurCalRegisters[9];
//	Doc["UoffsetC"] = MeasurCalRegisters[10];
//	Doc["IoffsetC"] = MeasurCalRegisters[11];
//	Doc["IgainN"] = MeasurCalRegisters[12];
//	Doc["IoffsetN"] = MeasurCalRegisters[13];
//
//
//
//
//    // write config file
//    String tmp = "";
//    serializeJson(Doc, tmp);
//    writeFile(SPIFFS, config_filename, tmp);
//
//    return true;
//}



//============================================================================

void writeFile(fs::FS& fs, String filename, String message) {
    DBUGS.println("WriteFile -> Writing file: " + filename);

    File file = fs.open(filename, FILE_WRITE);
    if (!file) {
        DBUGS.println("WriteFile -> Failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        DBUGS.println("WriteFile -> File written");
    }
    else {
        DBUGS.println("WriteFile -> WriteFile failed");
    }
    file.close();
}


String readFile(fs::FS& fs, String filename) {
    DBUGS.println("ReadFile -> Reading file: " + filename);

    File file = fs.open(filename);
    if (!file || file.isDirectory()) {
        Serial.println("ReadFile -> failed to open file for reading");
        return "";
    }

    String fileText = "";
    while (file.available()) {
        fileText = file.readString();
    }

    file.close();
    return fileText;
}