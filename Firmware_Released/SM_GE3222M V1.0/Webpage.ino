
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif

#include <WiFiType.h>
#include <FS.h>

#define wifi_is_client_configured()   (WiFi.SSID() != "")

// Wifi mode
#define wifi_mode_is_sta()            (WIFI_STA == (WiFi.getMode() & WIFI_STA))
#define wifi_mode_is_sta_only()       (WIFI_STA == WiFi.getMode())
#define wifi_mode_is_ap()             (WIFI_AP == (WiFi.getMode() & WIFI_AP))

// Performing a scan enables STA so we end up in AP+STA mode so treat AP+STA with no
// ssid set as AP only
#define wifi_mode_is_ap_only()        ((WIFI_AP == WiFi.getMode()) || \
                                       (WIFI_AP_STA == WiFi.getMode() && !wifi_is_client_configured()))

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// Event timeouts
unsigned long wifiRestartTime = 0;
unsigned long mqttRestartTime = 0;
unsigned long systemRestartTime = 0;
unsigned long systemRebootTime = 0;



// Get Sensor Readings and return JSON object
String getSystemInfo() {
    sysdata["vendorname"] = Vendor_Name;
    sysdata["productcode"] = Product_Code;
    sysdata["rev_no"] = Revision_Number;

    sysdata["meter_name"] = meter_name;
    sysdata["model"] = model;
    sysdata["manufacturer"] = manufacturer;
    sysdata["serial_no"] = serial_no;
    sysdata["hardware_rev"] = hardware_rev;
    sysdata["firmware_ver"] = firmware_ver;
    sysdata["mac_add"] = mac_add;
    sysdata["node"] = node;
    sysdata["node_ID"] = String(node_ID);
    sysdata["ssid"] = String(ssid);
    sysdata["ipaddress"] = ipaddress;


    String jsonString = JSON.stringify(sysdata);
    return jsonString;
}

// Get Sensor Readings and return JSON object
String getConfigInfo1() {

    //Send Calibration Register Data
    configdata["PoffsetA"]  = String(CalRegisters[0]);
    configdata["QoffsetA"]  = String(CalRegisters[1]);
    configdata["POffsetB"]  = String(CalRegisters[2]);
    configdata["QOffsetB"]  = String(CalRegisters[3]);
    configdata["POffsetC"]  = String(CalRegisters[4]);
    configdata["QOffsetC"]  = String(CalRegisters[5]);
    configdata["GainA"]     = String(CalRegisters[6]);
    configdata["PhiA"]      = String(CalRegisters[7]);
    configdata["GainB"]     = String(CalRegisters[8]);
    configdata["PhiB"]      = String(CalRegisters[9]);
    configdata["GainC"]     = String(CalRegisters[10]);
    configdata["PhiC"]      = String(CalRegisters[11]);
    configdata["CS1"]       = String(CalRegisters[12]);

    String jsonString = JSON.stringify(configdata);
    return jsonString;
}

    //Fundamental/ Harmonic Energy Calibration register Data

String getConfigInfo2() {
    configdata["POffsetAF"] = String(HarCalRegisters[0]);
    configdata["POffsetBF"] = String(HarCalRegisters[1]);
    configdata["POffsetCF"] = String(HarCalRegisters[2]);
    configdata["PGainAF"]   = String(HarCalRegisters[3]);
    configdata["PGainBF"]   = String(HarCalRegisters[4]);
    configdata["PGainCF"]   = String(HarCalRegisters[5]);
    configdata["CS2"]       = String(HarCalRegisters[6]);

    String jsonString = JSON.stringify(configdata);
    return jsonString;
}

String getConfigInfo3() {
    //Measurement Calibration Register Data 
    configdata["UgainA"]    = String(MeasurCalRegisters[0]);
    configdata["IgainA"]    = String(MeasurCalRegisters[1]);
    configdata["UoffsetA"]  = String(MeasurCalRegisters[2]);
    configdata["IoffsetA"]  = String(MeasurCalRegisters[3]);
    configdata["UgainB"]    = String(MeasurCalRegisters[4]);
    configdata["IgainB"]    = String(MeasurCalRegisters[5]);
    configdata["UoffsetB"]  = String(MeasurCalRegisters[6]);
    configdata["IoffsetB"]  = String(MeasurCalRegisters[7]);
    configdata["UgainC"]    = String(MeasurCalRegisters[8]);
    configdata["IgainC"]    = String(MeasurCalRegisters[9]);
    configdata["UoffsetC"]  = String(MeasurCalRegisters[10]);
    configdata["IoffsetC"]  = String(MeasurCalRegisters[11]);
    configdata["IgainN"]    = String(MeasurCalRegisters[12]);
    configdata["IoffsetN"]  = String(MeasurCalRegisters[13]);
    configdata["CS3"]       = String(MeasurCalRegisters[14]);

    String jsonString = JSON.stringify(configdata);
    return jsonString;
}


// Get Sensor Readings and return JSON object
String getReadings() {

//================================================================================
// Energy Register VARIABLE
//================================================================================
    readings["APenergyT"]   = String(TotalForwardActiveEnergy);
    readings["APenergyA"]   = String(PhaA_ForwardActiveEnergy);
    readings["APenergyB"]   = String(PhaB_ForwardActiveEnergy);
    readings["APenergyC"]   = String(PhaC_ForwardActiveEnergy);
    readings["ANenergyT"]   = String(TotalReverseActiveEnergy);
    readings["ANenergyA"]   = String(PhaA_ReverseActiveEnergy);
    readings["ANenergyB"]   = String(PhaB_ReverseActiveEnergy);
    readings["ANenergyC"]   = String(PhaC_ReverseActiveEnergy);
    readings["RPenergyT"]   = String(TotalForwardReactiveEnergy);
    readings["RPenergyA"]   = String(PhaA_ForwardReactiveEnergy);
    readings["RPenergyB"]   = String(PhaB_ForwardReactiveEnergy);
    readings["RPenergyC"]   = String(PhaC_ForwardReactiveEnergy);
    readings["RNenergyT"]   = String(TotalReverseReactiveEnergy);
    readings["RNenergyA"]   = String(PhaA_ReverseReactiveEnergy);
    readings["RNenergyB"]   = String(PhaB_ReverseReactiveEnergy);
    readings["RNenergyC"]   = String(PhaC_ReverseReactiveEnergy);
    readings["SAenergyT"]   = String(Total_ArithmeticSum_ApparentEnergy);
    readings["SVenergyT"] = String(VectorSum_TotalApparentEnergy);
    readings["SenergyA"]    = String(PhaA_ApparentEnergy);
    readings["SenergyB"]    = String(PhaB_ApparentEnergy);
    readings["SenergyC"]    = String(PhaC_ApparentEnergy);
    
   // readings["EnStatus0"]   = String(MeteringStatus0);
   // readings["EnStatus1"]   = String(MeteringStatus1);
    

    //================================================================================
//Power and Power Factor Register VARIABLES
//================================================================================


        readings["PmeanT"] = String(Total_PhasesumActivePower);
        readings["PmeanA"] = String(PhaA_ActivePower);
        readings["PmeanB"] = String(PhaB_ActivePower);
        readings["PmeanC"] = String(PhaC_ActivePower);
        readings["QmeanT"] = String(Total_PhasesumReactivePower);
        readings["QmeanA"] = String(PhaA_ReactivePower);
        readings["QmeanB"] = String(PhaB_ReactivePower);
        readings["QmeanC"] = String(PhaC_ReactivePower);
        readings["SAmeanT"] = String(Total_ArithmeticSumapparentpower);
        readings["SVmeanT"] = String(VectorSum_TotalApparentPower);
        readings["SmeanA"] = String(PhaA_apparentpower);
        readings["SmeanB"] = String(PhaB_apparentpower);
        readings["SmeanC"] = String(PhaC_apparentpower);
        readings["PFmeanT"] = String(Totalpowerfactor);
        readings["PFmeanA"] = String(PhaA_powerfactor);
        readings["PFmeanB"] = String(PhaB_powerfactor);
        readings["PFmeanC"] = String(PhaC_powerfactor);
        readings["PmeanTF"] = String(Totalactivefundamentalpower);
        readings["PmeanAF"] = String(PhaA_activefundamentalpower);
        readings["PmeanBF"] = String(PhaB_activefundamentalpower);
        readings["PmeanCF"] = String(PhaC_activefundamentalpower);
        readings["PmeanTH"] = String(Totalactiveharmonicpower);
        readings["PmeanAH"] = String(PhaA_activeharmonicpower);
        readings["PmeanBH"] = String(PhaB_activeharmonicpower);
        readings["PmeanCH"] = String(PhaC_activeharmonicpower);
        readings["IrmsN1"] = String(N_Line_SampledcurrentRMS);
        readings["IrmsN0"] = String(N_Line_calculatedcurrentRMS);
        readings["IrmsA"] = String(PhaA_currentRMS);
        readings["IrmsB"] = String(PhaB_currentRMS);
        readings["IrmsC"] = String(PhaC_currentRMS);
        readings["UrmsA"] = String(PhaA_voltageRMS);
        readings["UrmsB"] = String(PhaB_voltageRMS);
        readings["UrmsC"] = String(PhaC_voltageRMS);


//================================================================================
//THD+N, Frequency, Angle and Temperature Registers VARIABLES
//================================================================================

        readings["THDNUA"] = String(PhaA_voltageTHDN);
        readings["THDNUB"] = String(PhaB_voltageTHDN);
        readings["THDNUC"] = String(PhaC_voltageTHDN);
        readings["THDNIA"] = String(PhaA_currentTHDN);
        readings["THDNIB"] = String(PhaB_currentTHDN);
        readings["THDNIC"] = String(PhaC_currentTHDN);
        readings["Freq"] = String(Frequency);
        readings["PAngleA"] = String(PhaA_meanphaseangle);
        readings["PAngleB"] = String(PhaB_meanphaseangle);
        readings["PAngleC"] = String(PhaC_meanphaseangle);
        readings["Temp"] = String(Measuredtemperature);
        readings["UangleA"] = String(PhaA_voltagephaseangle);
        readings["UangleB"] = String(PhaB_voltagephaseangle);
        readings["UangleC"] = String(PhaC_voltagephaseangle);

    String jsonString = JSON.stringify(readings);
    return jsonString;
}












void notifyClients(String meterdata) {
    ws.textAll(meterdata);

}


void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        String message = (char*)data;
        //Check if the message is "getReadings"
        if (strcmp((char*)data, "getmeterInfo") == 0) {
            //if it is, send current sensor readings
            String meterdata = getSystemInfo();
            DBUGS.print(meterdata);
            notifyClients(meterdata);
        }

        if (strcmp((char*)data, "getmeterconfig1") == 0) {
            //if it is, send current sensor readings

            String configdata1 = getConfigInfo1();
            DBUGS.print(configdata1);
            notifyClients(configdata1);
        }

        if (strcmp((char*)data, "getmeterconfig2") == 0) {
            //if it is, send current sensor readings

            String configdata2 = getConfigInfo2();
            DBUGS.print(configdata2);
            notifyClients(configdata2);
        }

        if (strcmp((char*)data, "getmeterconfig3") == 0) {
            //if it is, send current sensor readings

            String configdata3 = getConfigInfo3();
            DBUGS.print(configdata3);
            notifyClients(configdata3);
        }


        if (strcmp((char*)data, "getreadings") == 0) {
            //if it is, send current sensor readings

            String meterreading = getReadings();
            DBUGS.print(meterreading);
            notifyClients(meterreading);
        }



              // try to decipher the JSON string received
        StaticJsonDocument<200> doc;                    // create a JSON container
        DeserializationError error = deserializeJson(doc, data);
        if (error) {
            DBUGS.print(F("deserializeJson() failed: "));
            DBUGS.println(error.f_str());
            return;
        }
        else {
            // JSON string was received correctly, so information can be retrieved:



            //Send Calibration Register Data
            CalRegisters[0]     = doc["PoffsetAtxt"];
            CalRegisters[1]     = doc["QoffsetAtxt"];
            CalRegisters[2]     = doc["POffsetBtxt"];
            CalRegisters[3]     = doc["QOffsetBtxt"];
            CalRegisters[4]     = doc["POffsetCtxt"];
            CalRegisters[5]     = doc["QOffsetCtxt"];
            CalRegisters[6]     = doc["GainAtxt"];
            CalRegisters[7]     = doc["PhiAtxt"];
            CalRegisters[8]     = doc["GainBtxt"];
            CalRegisters[9]     = doc["PhiBtxt"];
            CalRegisters[10]    = doc["GainCtxt"];
            CalRegisters[11]    = doc["PhiCtxt"];
            CalRegisters[12]    = doc["CS1txt"];


            //Fundamental/ Harmonic Energy Calibration register Data
            HarCalRegisters[0]  = doc["POffsetAFtxt"];
            HarCalRegisters[1]  = doc["POffsetBFtxt"];
            HarCalRegisters[2]  = doc["POffsetCFtxt"];
            HarCalRegisters[3]  = doc["PGainAFtxt"];
            HarCalRegisters[4]  = doc["PGainBFtxt"];
            HarCalRegisters[5]  = doc["PGainCFtxt"];
            HarCalRegisters[6]  = doc["CS2txt"];

            MeasurCalRegisters[0]   = doc["UgainAtxt"];
            MeasurCalRegisters[1]   = doc["IgainAtxt"];
            MeasurCalRegisters[2]   = doc["UoffsetAtxt"];
            MeasurCalRegisters[3]   = doc["IoffsetAtxt"];
            MeasurCalRegisters[4]   = doc["UgainBtxt"];
            MeasurCalRegisters[5]   = doc["IgainBtxt"];
            MeasurCalRegisters[6]   = doc["UoffsetBtxt"];
            MeasurCalRegisters[7]   = doc["IoffsetBtxt"];
            MeasurCalRegisters[8]   = doc["UgainCtxt"];
            MeasurCalRegisters[9]   = doc["IgainCtxt"];
            MeasurCalRegisters[10]  = doc["UoffsetCtxt"];
            MeasurCalRegisters[11]  = doc["IoffsetCtxt"];
            MeasurCalRegisters[12]  = doc["IgainNtxt"];
            MeasurCalRegisters[13]  = doc["IoffsetNtxt"];
            MeasurCalRegisters[14]  = doc["CS3txt"];
        
            
                        
            Save_CalConfiguration();

        }
        DBUGS.println("");

        systemRestartTime = millis() + 1000;
        
    } 
    
}


void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {

    case WS_EVT_CONNECT:
        DBUGS.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    
    case WS_EVT_DISCONNECT:
        DBUGS.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}


void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}


void Setup_Webserver(void) {


    //initSPIFFS();
    initWebSocket();
    Load_Reg_Callibration();
    Load_Power_Registers();


    // Setup the static files

    server.serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html");



    // Web Server Root URL

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/index.html", "text/html");
        });


    server.on("/config", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/callibration.html", "text/html");
        });


    server.on("/readdata", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/readings.html", "text/html");
        });

    server.serveStatic("/", SPIFFS, "/");


    // Start server
    server.begin();


}


void Webserver_loop(void) {



    if ((millis() - lastTime) > timerDelay) {
        String meterdata = getSystemInfo();
        String configdata1 = getConfigInfo1();
        String configdata2 = getConfigInfo2();
        String configdata3 = getConfigInfo3();
        String meterreading = getReadings();

        DBUGS.print(meterdata);
        notifyClients(meterdata);

        DBUGS.print(configdata1);
        notifyClients(configdata1);

        DBUGS.print(configdata2);
        notifyClients(configdata2);

        DBUGS.print(configdata3);
        notifyClients(configdata3);

        DBUGS.print(meterreading);
        notifyClients(meterreading);



        lastTime = millis();

    }

    ws.cleanupClients();

    // Do we need to restart the system?
    if (systemRestartTime > 0 && millis() > systemRestartTime) {
        systemRestartTime = 0;
        //eic.InitCallibrate();
    }


}

void notFound(AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "Not found");
}

void getconfig() {



    server.onNotFound(notFound);
    server.begin();
}

//==============================================================================================/






